#pragma once
#include "DirectX3DManager.h"
#include <string>
#include <wincodec.h>
#include <vector>
#include "BaseObject.h"
#include "Texture.h"

/// <summary>
/// 画像を表示するクラス
/// </summary>
class Image : public BaseObject {
private:
	UINT width_, height_;
	std::string path_;

	ID3D11Texture2D* texture2D_ = nullptr;
	ID3D11SamplerState* samplerState_ = nullptr;
	ID3D11ShaderResourceView* shaderResourceView_ = nullptr;
	ID3D11Buffer* constantBuffer_ = nullptr;
	ID3D11Buffer* vertexBuffer_ = nullptr;
	IWICImagingFactory* pFactory = nullptr;
	IWICBitmapDecoder* pDecoder = nullptr;
	IWICBitmapFrameDecode* pFrame = nullptr;
	IWICFormatConverter* pConverter = nullptr;
	Vertex vertices_[6];

	bool isGray_;
public:
	Image(const std::string& path, const float leftX, const float leftY);
	~Image() {};
	void Init() override;
	void Update() override;
	void Draw() override;
	void Release() override;

	bool IsGray() const { return isGray_; }
	void SetGray(bool flag) { isGray_ = flag; }
};
