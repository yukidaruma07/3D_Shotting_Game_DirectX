#include "Bullet.h"

Bullet::Bullet(DirectX::XMFLOAT3 postion)
	: BaseObject("Bullet") {
	postion_ = postion;
	fbxModel_ = nullptr;
}

Bullet::~Bullet() {
	delete fbxModel_;
}

void Bullet::Init() {
	fbxModel_ = new FBX("Asset/bullet.fbx");
	fbxModel_->Init();
}

void Bullet::Update() {
	if (fbxModel_ != nullptr) {
		fbxModel_->Update();
		fbxModel_->SetPosition(postion_);
		fbxModel_->SetScale(scale_);
		fbxModel_->SetRotation(rotation_);
	}
}

void Bullet::Draw() {
	if (fbxModel_ != nullptr) {
		fbxModel_->Draw();
	}
}

void Bullet::Release() {
}
