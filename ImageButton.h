#pragma once
#include "Engine/BaseObject.h"
#include "Engine/Image.h"

class ImageButton : public BaseObject {
private:
	Image* image_;
	bool isClicked_;
	bool isFocused_;
	int width_, height_;
public:
	ImageButton(std::string path, int width, int height, DirectX::XMFLOAT3 postion);
	~ImageButton();

	void Init() override;
	void Update() override;
	void Draw() override;
	void Release() override;


	bool IsClicked() const { return isClicked_; }
	bool IsFocused() const { return isFocused_; }

	bool IsGray() const { return image_->IsGray(); }
	void SetGray(bool flag) { image_->SetGray(flag); }
};

