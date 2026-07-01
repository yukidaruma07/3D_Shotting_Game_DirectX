#include "CircleCollider.h"

using namespace DirectX;

CircleCollider::CircleCollider(BaseObject* parent, int radius)
	: BaseObject("CircleCollider", true) {
	this->parent_ = parent;
	this->radius_ = radius;
	isShowImGUI_ = false; //直接描画するわけではないため、ImGUIの表示はfalse
}

CircleCollider::~CircleCollider()
{
}

void CircleCollider::Init()
{
}

void CircleCollider::Update()
{
	postion_ = parent_->GetPosition();
}

void CircleCollider::Draw()
{
}

void CircleCollider::Release()
{
}

bool CircleCollider::IsCircle(CircleCollider* circleCol) {
	XMFLOAT3 aPos = GetPosition();
	XMFLOAT3 bPos = circleCol->GetPosition();
	XMVECTOR vectorA = XMLoadFloat3(&aPos);
	XMVECTOR vectorB = XMLoadFloat3(&bPos);

	XMVECTOR diff = XMVectorSubtract(vectorA, vectorB);
	float distance = XMVectorGetX(XMVector3Length(diff));
	const float merge = radius_ + circleCol->GetRadius();
	if (distance < merge) {
		return true;
	}
	return false;
}
