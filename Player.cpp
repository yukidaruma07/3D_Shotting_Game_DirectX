#include "Player.h"
#include "Engine/InputManager.h"
#include "Engine/ObjectManager.h"
#include "Bullet.h"

Player::Player(DirectX::XMFLOAT3 postion)
	: BaseObject("Player") {
	postion_ = postion;
	scale_ = { 0.05, 0.5, 0.05 };
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

	if (InputManager::IsPushKey(DIK_SPACE)) {
		ObjectManager::AddObject(new Bullet(postion_));
	}
}

void Player::Draw() {
	if (fbxModel_ != nullptr) {
		fbxModel_->Draw();
	}
}

void Player::Release() {
}
