#include "Enemy.h"
#include "Engine//CircleCollider.h"

Enemy::Enemy(DirectX::XMFLOAT3 postion)
	: BaseObject("Enemy") {
	postion_ = postion;
	fbxModel_ = nullptr;
}

Enemy::~Enemy() {
	delete fbxModel_;
}

void Enemy::Init() {
	fbxModel_ = new FBX("Asset/enemy.fbx");
	fbxModel_->Init();
	colliderList.push_back(new CircleCollider(this, 1.0f));
}

void Enemy::Update() {
	if (fbxModel_ != nullptr) {
		fbxModel_->Update();
		fbxModel_->SetPosition(postion_);
		fbxModel_->SetScale(scale_);
		fbxModel_->SetRotation(rotation_);
	}
}

void Enemy::Draw() {
	if (fbxModel_ != nullptr) {
		fbxModel_->Draw();
	}
}

void Enemy::Release() {
}

