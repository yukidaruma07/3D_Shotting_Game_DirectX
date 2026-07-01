#include "Player.h"
#include "Engine/InputManager.h"
#include "Engine/ObjectManager.h"
#include "Bullet.h"
using namespace DirectX;

Player::Player(DirectX::XMFLOAT3 postion)
	: BaseObject("Player") {
	postion_ = postion;
	velocity_ = { 0.1f, 0.1f, 0.1f };
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


void Player::DrawObjectInfoImGUI() {
	ImGui::Begin(name_.c_str());
	ImGui::SliderFloat("X", &postion_.x, -1280.0f, 1280.0f);
	ImGui::SliderFloat("Y", &postion_.y, -1280.0f, 1280.0f);
	ImGui::SliderFloat("Z", &postion_.z, -1280.0f, 1280.0f);
	ImGui::SliderFloat("angleX", &rotation_.x, 0.0, 90.0f);
	ImGui::Text("(%2.2f)", XMConvertToRadians(rotation_.x));
	ImGui::SliderFloat("angleY", &rotation_.y, 0.0f, 90.0f);
	ImGui::Text("(%2.2f)", XMConvertToRadians(rotation_.y));
	ImGui::SliderFloat("angleZ", &rotation_.z, 0.0f, 90.0f);
	ImGui::Text("(%2.2f)", XMConvertToRadians(rotation_.z));
	ImGui::SliderFloat("scaleX", &scale_.x, 0.0f, 10.0f);
	ImGui::SliderFloat("scaleY", &scale_.y, 0.0f, 10.0f);
	ImGui::SliderFloat("scaleZ", &scale_.z, 0.0f, 10.0f);
	ImGui::End();
}