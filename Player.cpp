#include "Player.h"
#include "Engine/InputManager.h"
#include "Engine/ObjectManager.h"
#include "Bullet.h"

Player::Player(DirectX::XMFLOAT3 postion)
	: BaseObject("Player") {
	postion_ = postion;
	velocity_ = { 5.0f, 5.0f, 5.0f };
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
	if (InputManager::IsPushKey(DIK_SPACE)) {
		ObjectManager::AddObject(new Bullet(postion_));
	}
	if (InputManager::IsPushKey(DIK_UP)) {
		postion_.z += velocity_.z;
	}
	if (InputManager::IsPushKey(DIK_DOWN)) {
		postion_.z -= velocity_.z;
	}
	if (InputManager::IsPushKey(DIK_LEFT)) {
		postion_.x -= velocity_.x;
	}
	if (InputManager::IsPushKey(DIK_RIGHT)) {
		postion_.x += velocity_.x;
	}
	if (InputManager::IsPushKey(DIK_A)) {
		rotation_.z -= DirectX::XMConvertToRadians(1.0f);
	}
	if (InputManager::IsPushKey(DIK_D)) {
		rotation_.z += DirectX::XMConvertToRadians(1.0f);
	}

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
