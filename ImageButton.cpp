#include "ImageButton.h"
#include "Engine/InputManager.h"

ImageButton::ImageButton(std::string path, int width, int height, DirectX::XMFLOAT3 postion) 
	: BaseObject("ImageButton") {
	isClicked_ = false;
	isFocused_ = false;

	postion_ = postion;
	image_ = new Image(path, width, height);
	width_ = width;
	height_ = height;
}

ImageButton::~ImageButton()
{
}

void ImageButton::Init() {
	image_->Init();
}

void ImageButton::Update() {
	if (image_ == nullptr) return;

	image_->SetPosition(postion_);
	image_->SetRotation(rotation_);
	image_->SetScale(scale_);
	image_->Update();
	auto point = InputManager::GetMousePoint();
	if (postion_.x <= point.x && point.x <= postion_.x + width_
		&& postion_.y <= point.y && point.y <= postion_.y + height_) {
		isFocused_ = true;
		isClicked_ = InputManager::IsPushMouse(0);
	}
	else {
		isFocused_ = false;
	}
}

void ImageButton::Draw() {
	if (image_ == nullptr) return;
	image_->Draw();
}

void ImageButton::Release() {
	if (image_ == nullptr) return;
	image_->Release();
}
