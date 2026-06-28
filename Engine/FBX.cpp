#include "FBX.h"
#include "fbxsdk.h"
#include "DirectX3DManager.h"
#include "BootScene.h"
#include "Texture.h"
#include <vector>
#include "../ImGUI/imgui.h"
#include "CameraManager.h"
#include "../GameEngine.hpp"
#include "LoggerManager.h"

using namespace DirectX3DManager;

using namespace fbxsdk;
using namespace DirectX;

FBX::FBX(const std::string fName)
	: BaseObject("FBX") {
	this->path_ = fName;
	isShowTexture_ = false;
	indexCount_ = -1;
	materialCount_ = -1;
	polygonCount_ = -1;
	vertexCount_ = -1;
	materials_.clear();
	pConstantBuffer_ = nullptr;
	pIndexBuffer_ = nullptr;
	pVertexBuffer_ = nullptr;
	vertices_.clear();
}

FBX::~FBX() {
}

void FBX::Init() {
	fbxManager_ = FbxManager::Create();
	fbxImporter_ = FbxImporter::Create(fbxManager_, "imp");
	fbxImporter_->Initialize(path_.c_str(), -1, fbxManager_->GetIOSettings());
	FbxScene* fbxScene = FbxScene::Create(fbxManager_, "fbxscene");
	fbxImporter_->Import(fbxScene);
	fbxImporter_->Destroy();


	FbxGeometryConverter converter(fbxManager_);
	converter.Triangulate(fbxScene, true);

	FbxNode* rootNode = fbxScene->GetRootNode();
	FbxNode* node = rootNode->GetChild(0); //結合済み前提
	FbxMesh* mesh = nullptr;
	for (int i = 0; i < rootNode->GetChildCount(); i++) {
		FbxNode* childNode = rootNode->GetChild(i);
		if (childNode->GetMesh() != nullptr) {
			mesh = childNode->GetMesh();
			node = childNode;
			break;
		}
	}

	vertexCount_ = mesh->GetControlPointsCount();	//頂点数を取得する
	polygonCount_ = mesh->GetPolygonCount();		//ポリゴンを取得する
	materialCount_ = node->GetMaterialCount();		//マテリアルを取得する
	indexMaterialCount_.resize(materialCount_);

	InitVertex(mesh);		//頂点バッファを初期化する
	InitIndex(mesh);		//インデックスバッファを初期化する
	InitConstantBuffer();	//コンスタントバッファ（GPUに送るデータ）を初期化する
	InitMaterial(node);		//マテリアルを初期化する
}

void FBX::InitVertex(FbxMesh* mesh) {

	FbxNode* node = mesh->GetNode();
	FbxAMatrix geomTransform = {};
	// ジオメトリ変換
	FbxVector4 geoT = node->GetGeometricTranslation(FbxNode::eSourcePivot);
	FbxVector4 geoR = node->GetGeometricRotation(FbxNode::eSourcePivot);
	FbxVector4 geoS = node->GetGeometricScaling(FbxNode::eSourcePivot);
	geomTransform.SetT(geoT);
	geomTransform.SetR(geoR);
	geomTransform.SetS(geoS);

	FbxAMatrix globalTransform = node->EvaluateGlobalTransform() * geomTransform;

	for (DWORD poly = 0; poly < polygonCount_; poly++) {	//ポリゴン分ループする
		for (int vertexCount = 0; vertexCount < 3; vertexCount++) {		//頂点分ループする（※結合済み前提
			Vertex vertex = {};

			int index = mesh->GetPolygonVertex(poly, vertexCount);
			FbxVector4 pos = mesh->GetControlPointAt(index);	//頂点座標（ローカル）
			FbxVector4 worldPos = globalTransform.MultT(pos);

			vertex.postion.x = (float)worldPos[0];			//頂点のX座標を代入する
			vertex.postion.y = (float)worldPos[1];			//頂点のY座標を代入する
			vertex.postion.z = (float)worldPos[2];			//頂点のZ座標を代入する

			FbxLayerElementUV* uvLayer = mesh->GetLayer(0)->GetUVs();
			int uvIndex = mesh->GetTextureUVIndex(poly, vertexCount);
			FbxVector2 uv = uvLayer->GetDirectArray().GetAt(uvIndex);	//UV座標を取得する
			vertex.uv.x = (float)uv.mData[0];					//UV座標の横軸を代入する
			vertex.uv.y = (float)(1.0f - uv.mData[1]);		//UV座標の縦軸を代入する

			vertices_.push_back(vertex);
		}
	}
	vertexCount_ = vertices_.size();

	D3D11_BUFFER_DESC bd = {};
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(Vertex) * vertexCount_;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;

	D3D11_SUBRESOURCE_DATA data = {};
	data.pSysMem = vertices_.data();			//頂点データ

	HRESULT hr = GetDevice()->CreateBuffer(&bd, &data, &pVertexBuffer_);	//頂点バッファを作成する
}

void FBX::InitIndex(FbxMesh* mesh) {
	pIndexBuffer_ = new ID3D11Buffer*[materialCount_];
	index_.resize((size_t) polygonCount_ * 3);

	for (int i = 0; i < materialCount_; i++) {
		int count = 0;

		for (DWORD poly = 0; poly < polygonCount_; poly++) {
			FbxLayerElementMaterial* materialLayer = mesh->GetLayer(0)->GetMaterials();
			int materialId = materialLayer->GetIndexArray().GetAt(poly);

			if (materialId == i) {
				for (DWORD vertex = 0; vertex < 3; vertex++) {
					index_[i].push_back(poly * 3 + vertex);
					count++;
				}
			}

		}

		indexMaterialCount_[i] = count;

		D3D11_BUFFER_DESC bd = {};
		bd.ByteWidth = sizeof(int) * count;
		bd.Usage = D3D11_USAGE_DEFAULT;
		bd.BindFlags = D3D11_BIND_INDEX_BUFFER;

		D3D11_SUBRESOURCE_DATA data = {};
		data.pSysMem = index_[i].data();				//インデックスバッファ

		HRESULT hr = GetDevice()->CreateBuffer(&bd, &data, &pIndexBuffer_[i]);
	}
}

void FBX::InitConstantBuffer() {
	D3D11_BUFFER_DESC constantBufferDesc = {};
	constantBufferDesc.ByteWidth = sizeof(ConstantBuffer);
	constantBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	constantBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	HRESULT hr = GetDevice()->CreateBuffer(&constantBufferDesc, nullptr, &pConstantBuffer_);
}

void FBX::InitMaterial(fbxsdk::FbxNode* node) {
	materials_.resize(materialCount_);	// vectorをmaterialcount分に変更

	for (int i = 0; i < materialCount_; i++) {	//マテリアル分ループする
		FbxSurfaceMaterial* material = node->GetMaterial(i);
		FbxProperty property = material->FindProperty(FbxSurfaceMaterial::sDiffuse);
		int fileTextureCount = property.GetSrcObjectCount <FbxFileTexture>();
		FbxSurfaceLambert* pMaterial = (FbxSurfaceLambert*)node->GetMaterial(i);
		FbxDouble3  diffuse = pMaterial->Diffuse;
		materials_[i].diffuse = XMFLOAT4((float)diffuse[0], (float)diffuse[1], (float)diffuse[2], 1.0f);

		if (fileTextureCount > 0) {	//　マテリアルにテクスチャがある場合
			FbxFileTexture* textureInfo = property.GetSrcObject<FbxFileTexture>();
			const char* texturePath = textureInfo->GetFileName();			
			materials_[i].texture = new Texture(texturePath, -0.5f, 0.5f);	//　テクスチャデータを代入する
			materials_[i].texture->Init();
		}
		else {	// マテリアルにテクスチャがない場合
			materials_[i].texture = nullptr;	// 無いので、nullptrにする
		}
	}
}

void FBX::Update() {
	auto currentCamera = CameraManager::getCurentCamera();
	XMMATRIX scaleMat = XMMatrixScaling(scale_.x, scale_.y, scale_.z);
	XMMATRIX rotMat = XMMatrixRotationZ(rotation_.z) * XMMatrixRotationX(rotation_.x) * XMMatrixRotationY(rotation_.y);
	XMMATRIX transMat = XMMatrixTranslation(postion_.x, postion_.y, postion_.z);
	XMMATRIX world = scaleMat * rotMat * transMat;
	XMMATRIX view = currentCamera->getMatrix();
	XMMATRIX projection = currentCamera->GetProjection();

	ConstantBuffer cb = {};
	for (int i = 0; i < materialCount_; i++) {
		auto material = materials_[i];
		cb.wvpMat = XMMatrixTranspose(world * view * projection);
		cb.diffUse = material.diffuse;
		cb.isTexture = material.texture != nullptr ? TRUE : FALSE;
		GetContext()->UpdateSubresource(pConstantBuffer_, 0, nullptr, &cb, 0, 0);
	}

}

void FBX::Draw() {
	UINT stride = sizeof(Vertex);
	UINT offset = 0;
	ID3D11ShaderResourceView* nullSRV[1] = { nullptr };

	GetContext()->RSSetState(GetRasterizer());
	GetContext()->IASetInputLayout(ShaderManager::inputLayout_);
	GetContext()->IASetVertexBuffers(0, 1, &pVertexBuffer_, &stride, &offset);
	GetContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	GetContext()->VSSetShader(ShaderManager::vertexShader_, nullptr, 0);
	GetContext()->PSSetShader(ShaderManager::pixelShader_, nullptr, 0);
		
	for (int i = 0; i < materialCount_; i++) {
		GetContext()->IASetIndexBuffer(pIndexBuffer_[i], DXGI_FORMAT_R32_UINT, 0);
		if (!isShowTexture_) {
			GetContext()->PSSetShaderResources(0, 1, nullSRV);
			GetContext()->PSSetShader(ShaderManager::pixelDebugShader_, nullptr, 0);
		}
		else {
			if (materials_[i].texture != nullptr) {
				auto srv = (materials_[i].texture->GetShaderReasourceView());
				auto samplerState = materials_[i].texture->GetSampleState();
				GetContext()->PSSetShaderResources(0, 1, &srv);
				GetContext()->PSSetSamplers(0, 1, &samplerState);
			}
			else {
				GetContext()->PSSetShaderResources(0, 1, nullSRV);
			}
		}

		GetContext()->VSSetConstantBuffers(0, 1, &pConstantBuffer_);
		
		GetContext()->DrawIndexed(indexMaterialCount_[i], 0, 0);

		GetContext()->RSSetState(nullptr);
	}
}

void FBX::Release() {

}
