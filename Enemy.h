#pragma once
#include "Engine/BaseObject.h"
#include <DirectXMath.h>
#include "Engine/FBX.h"

class Enemy : public BaseObject {
private:
	FBX* fbxModel_;
public:
	Enemy(DirectX::XMFLOAT3 postion);
	~Enemy();

	void Init() override;
	void Update() override;
	void Draw() override;
	void Release() override;
};
