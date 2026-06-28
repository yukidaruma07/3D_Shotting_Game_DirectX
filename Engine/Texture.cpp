#include "Texture.h"
#include "BaseObject.h"
#include "BootScene.h"
#include "../ImGUI/imgui.h"
#include <wincodec.h>
#include <vector>
#include "CameraManager.h"
#include "DirectX3DManager.h"

using namespace DirectX3DManager;

using namespace DirectX;

Texture::Texture(const std::string& path, const float leftX, const float leftY)
	: BaseObject("Texture") {
	this->path_ = path;

	// DirectXでは、2個の三角形を組み合わせて描画するイメージ
	//  |------------|
	//  |[0,1,2] /   |
	//  |       /    |
	//  |     /      |
	//  |   / [3,4,5]|
	//  |------------|

	vertices_[0] = { {0.0f, 0.0f, 0.0f}, {0,1,0,1}, {0, 0} }; // 左上 
	vertices_[1] = { {0.0f, leftY, 0.0f }, { 1,0,0,1 }, { 0, 1 }}; // 左下
	vertices_[2] = { {leftX, 0.0f, 0.0f}, {0,0,1,1}, {1, 0} }; // 右上

	vertices_[3] = { {leftX, 0.0f, 0.0f}, {0,0,1,1}, {1, 0} }; // 右上
	vertices_[4] = { {leftX, leftY, 0.0f }, { 1,1,0,1 }, { 1, 1 }}; // 右下
	vertices_[5] = { {0.0f, leftY, 0.0f}, {1,0,0,1}, {0, 1} }; // 左下

	isShowImGUI_ = false; //直接描画するわけではないため、ImGUIの表示はfalse
}

void Texture::Init() {
	HRESULT result = { 0 };
	std::wstring wPath(path_.begin(), path_.end());

	result = CoCreateInstance(CLSID_WICImagingFactory, nullptr, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&pFactory));
	assert(SUCCEEDED(result));
	result = pFactory->CreateDecoderFromFilename(
		wPath.c_str(),
		nullptr,
		GENERIC_READ,
		WICDecodeMetadataCacheOnLoad,
		&pDecoder
	);
	assert(SUCCEEDED(result));
	result = pDecoder->GetFrame(0, &pFrame);
	assert(SUCCEEDED(result));

	result = pFactory->CreateFormatConverter(&pConverter);

	pConverter->Initialize(
		pFrame,
		GUID_WICPixelFormat32bppRGBA,
		WICBitmapDitherTypeNone,
		nullptr,
		1.0f,
		WICBitmapPaletteTypeMedianCut
	);
	pConverter->GetSize(&width_, &height_);

	//参考： https://learn.microsoft.com/ja-jp/windows/win32/api/d3d11/ns-d3d11-d3d11_texture2d_desc
	D3D11_TEXTURE2D_DESC desc = {};
	desc.Width = width_;
	desc.Height = height_;
	desc.MipLevels = 1;
	desc.ArraySize = 1;
	desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	desc.SampleDesc.Count = 1;
	desc.SampleDesc.Quality = 0;
	desc.Usage = D3D11_USAGE_DEFAULT;
	desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	desc.CPUAccessFlags = 0;
	desc.MipLevels = 1;

	std::vector<BYTE> TextureData;
	size_t rowBytes = static_cast<size_t>(width_) * 4;
	size_t TextureSize = rowBytes * static_cast<size_t>(height_);
	TextureData.resize(TextureSize);
	result = pConverter->CopyPixels(nullptr, static_cast<UINT>(rowBytes), static_cast<UINT>(TextureSize), TextureData.data());

	// 参考：https://learn.microsoft.com/ja-jp/windows/win32/api/d3d11/ns-d3d11-d3d11_subresource_data
	D3D11_SUBRESOURCE_DATA textureData = {};
	textureData.pSysMem = TextureData.data();
	textureData.SysMemPitch = static_cast<UINT>(rowBytes);

	auto texture2D = GetTexture2D();
	GetDevice()->CreateTexture2D(
		&desc,
		&textureData,
		&texture2D
	);

	// 参考： https://learn.microsoft.com/ja-jp/windows/win32/api/d3d11/ns-d3d11-d3d11_shader_resource_view_desc
	D3D11_SHADER_RESOURCE_VIEW_DESC viewDesc = {};
	viewDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	viewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	viewDesc.Texture2D.MipLevels = 1;
	GetDevice()->CreateShaderResourceView(texture2D, &viewDesc, &shaderResourceView_);

	// 参考： https://learn.microsoft.com/ja-jp/windows/win32/api/d3d11/ns-d3d11-d3d11_sampler_desc
	D3D11_SAMPLER_DESC samplerDesc = {};
	samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	GetDevice()->CreateSamplerState(&samplerDesc, &samplerState_);

	// 参考： https://learn.microsoft.com/ja-jp/windows/win32/api/d3d11/ns-d3d11-d3d11_buffer_desc
	D3D11_BUFFER_DESC bufferDesc = {};
	bufferDesc.Usage = D3D11_USAGE_DEFAULT;
	bufferDesc.ByteWidth = sizeof(vertices_);
	bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bufferDesc.CPUAccessFlags = 0;
	bufferDesc.MiscFlags = 0;
	bufferDesc.StructureByteStride = 0;

	// 参考： https://learn.microsoft.com/ja-jp/windows/win32/api/d3d11/ns-d3d11-d3d11_subresource_data
	D3D11_SUBRESOURCE_DATA bufferData = {};
	bufferData.pSysMem = vertices_;

	// 参考： https://learn.microsoft.com/ja-jp/windows/win32/api/d3d11/ns-d3d11-d3d11_buffer_desc
	D3D11_BUFFER_DESC constantBufferDesc = {};
	constantBufferDesc.ByteWidth = sizeof(ConstantBuffer);
	constantBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	constantBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	GetDevice()->CreateBuffer(&constantBufferDesc, nullptr, &constantBuffer_);

	GetDevice()->CreateBuffer(&bufferDesc, &bufferData, &vertexBuffer_);
}

void Texture::Update() {
	Camera* camera = CameraManager::getCurentCamera();
	if (camera == nullptr) return;

	XMMATRIX scaleMat = XMMatrixScaling(scale_.x, scale_.y, scale_.z);
	XMMATRIX rotMat = XMMatrixRotationZ(rotation_.z) * XMMatrixRotationX(rotation_.x) * XMMatrixRotationY(rotation_.y);
	XMMATRIX transMat = XMMatrixTranslation(postion_.x, postion_.y, postion_.z);
	XMMATRIX world = scaleMat * rotMat * transMat;
	XMMATRIX view = camera->getMatrix();
	XMMATRIX projection = camera->GetProjection();

	ConstantBuffer cb = {};
	cb.wvpMat = XMMatrixTranspose(world * view * projection);
	GetContext()->UpdateSubresource(constantBuffer_, 0, nullptr, &cb, 0, 0);
}

void Texture::Draw() {
	UINT stride = sizeof(Vertex);
	UINT offset = 0;

	GetContext()->RSSetState(GetRasterizer());
	GetContext()->IASetInputLayout(ShaderManager::inputLayout_);
	GetContext()->IASetVertexBuffers(0, 1, &vertexBuffer_, &stride, &offset);
	GetContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	GetContext()->VSSetShader(ShaderManager::vertexShader_, nullptr, 0);
	GetContext()->PSSetShader(ShaderManager::pixelShader_, nullptr, 0);
	GetContext()->PSSetShaderResources(0, 1, &shaderResourceView_);
	GetContext()->PSSetSamplers(0, 1, &samplerState_);
	GetContext()->VSSetConstantBuffers(0, 1, &constantBuffer_);

	//GetContext()->Draw(6, 0);

	GetContext()->RSSetState(nullptr);
}

void Texture::Release() {
	if (samplerState_ != nullptr) samplerState_->Release();
	if (shaderResourceView_ != nullptr) shaderResourceView_->Release();
	if (constantBuffer_ != nullptr) constantBuffer_->Release();
	if (vertexBuffer_ != nullptr) vertexBuffer_->Release();
	if (pFactory != nullptr) pFactory->Release();
	if (pDecoder != nullptr) pDecoder->Release();
	if (pFrame != nullptr) pFrame->Release();
	if (pConverter != nullptr) pConverter->Release();
}
