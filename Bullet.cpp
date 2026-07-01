#include "Bullet.h"
#include "Engine//CircleCollider.h"

Bullet::Bullet(DirectX::XMFLOAT3 postion)
	: BaseObject("Bullet") {
	postion_ = postion;
	velocity_ = { 0.1f, 0.1f, 0.1f };
	scale_ = { 0.5, 0.5, 0.5 };
	fbxModel_ = nullptr;
}

Bullet::~Bullet() {
	delete fbxModel_;
}

void Bullet::Init() {
	fbxModel_ = new FBX("Asset/bullet.fbx");
	fbxModel_->Init();
	colliderList.push_back(new CircleCollider(this, 1.0f));
}

void Bullet::Update() {
	if (fbxModel_ != nullptr) {
		fbxModel_->Update();
		fbxModel_->SetPosition(postion_);
		fbxModel_->SetScale(scale_);
		fbxModel_->SetRotation(rotation_);
	}

	postion_.z += velocity_.z;
}

void Bullet::Draw() {
	if (fbxModel_ != nullptr) {
		fbxModel_->Draw();
	}
}

void Bullet::Release() {
}

void Bullet::OnCollide(BaseObject* obj)	{
	if (obj->GetName() == "Enemy") {
		KillMe();
	}
}
