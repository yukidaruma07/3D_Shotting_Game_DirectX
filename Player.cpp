#include "Player.h"

Player::Player(DirectX::XMFLOAT3 postion)
	: BaseObject("Player") {
	postion_ = postion;
	fbxModel_ = nullptr;
}

Player::~Player() {
	delete fbxModel_;
}

void Player::Init() {
	fbxModel_ = new FBX("Asset/player.fbx");
	fbxModel_->Init();
}

void Player::Update() {
	if (fbxModel_ != nullptr) {
		fbxModel_->Update();
		fbxModel_->SetPosition(postion_);
		fbxModel_->SetScale(scale_);
		fbxModel_->SetRotation(rotation_);
	}
}

void Player::Draw() {
	if (fbxModel_ != nullptr) {
		fbxModel_->Draw();
	}
}

void Player::Release() {
}
