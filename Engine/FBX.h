#pragma once
#include <Windows.h>
#include "BaseObject.h"
#include <DirectXMath.h>
#include <d3d11.h>
#include <string>
#include <vector>
#include "fbxsdk.h"
#include "Texture.h"
#pragma comment(lib, "LibFbxSDK-MD.lib")
#pragma comment(lib, "LibXml2-MD.lib")
#pragma comment(lib, "zlib-MD.lib")

struct MATERIAL {
	Texture* texture;
	DirectX::XMFLOAT4	diffuse;
};

/// <summary>
/// FBXの3Dモデルを表示するクラス
/// </summary>
class FBX : public BaseObject {
private:
	bool isShowTexture_;
	std::string path_;

	ID3D11Buffer* pVertexBuffer_; //頂点バッファ
	ID3D11Buffer** pIndexBuffer_; //インデックスバッファ（※数は分からないため、ポインタの配列）
	//ID3D11Buffer* pConstantBuffer_; //コンスタントバッファ

	FbxManager* fbxManager_;
	FbxImporter* fbxImporter_;
	FbxSkin* pSkinInfo_;
	std::vector<std::vector<int>> index_;
	std::vector<int> indexMaterialCount_;
	std::vector<ID3D11Buffer*> pMaterialConstantBuffers_;

	int materialCount_;
	int vertexCount_;
	int polygonCount_;
	int indexCount_;
	std::vector<MATERIAL> materials_; //マテリアルの配列データ（※数は分からないため、ポインタ）
	std::vector<Vertex> vertices_; //頂点の配列データ（※数は分からないため、ポインタ）
public:
	DirectX::XMMATRIX world_;

	FBX(const std::string fName);
	~FBX();

	ID3D11Buffer* GetVertexBuffer() const { return pVertexBuffer_; }
	ID3D11Buffer** GetIndexBuffer() const { return pIndexBuffer_; }
	//ID3D11Buffer* GetConstantBuffer() const { return pConstantBuffer_; }

	void Init() override;

	/// <summary>
	/// 頂点を初期化する関数
	/// </summary>
	/// <param name="mesh">読み込むFBXのメッシュ</param>
	void InitVertex(fbxsdk::FbxMesh* mesh);

	/// <summary>
	/// インデックスバッファを初期化する関数
	/// </summary>
	/// <param name="mesh">読み込むFBXのメッシュ</param>
	void InitIndex(fbxsdk::FbxMesh* mesh);

	/// <summary>
	/// コンスタントバッファを初期化する関数
	/// </summary>
	void InitConstantBuffer();

	/// <summary>
	/// マテリアルを初期化する関数
	/// </summary>
	/// <param name="node">FBXのノード</param>
	void InitMaterial(fbxsdk::FbxNode* node);

	void InitSkeleton(fbxsdk::FbxMesh* mesh);

	void Update() override;
	void Draw() override;
	void Release() override;

	/// <summary>
	/// レイキャストを行う関数
	/// </summary>
	static bool Raycast(FBX* fbx, DirectX::XMFLOAT3 rayPos, DirectX::XMFLOAT3 rayDir, float& distance);
};

