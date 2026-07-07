#include "Ground.h"

Ground::Ground()
	: BaseObject("Ground") {
	fbx_ = nullptr;
}

Ground::~Ground()
{
}

void Ground::Init()
{
	fbx_ = new FBX("Asset/Ground.fbx");
	fbx_->Init();
}

void Ground::Update() {
	if (fbx_ != nullptr) {
		fbx_->Update();
		fbx_->SetPosition(postion_);
		fbx_->SetRotation(rotation_);
		fbx_->SetScale(scale_);
	}
}

void Ground::Draw() {
	if (fbx_ != nullptr) {
		fbx_->Draw();
	}
}

void Ground::Release()
{
}
