#pragma once
#include "Engine/BaseObject.h"
#include <DirectXMath.h>
#include "Engine/FBX.h"

class Bullet : public BaseObject {
private:
	FBX* fbxModel_;
public:
	Bullet(DirectX::XMFLOAT3 postion);
	~Bullet();

	void Init() override;
	void Update() override;
	void Draw() override;
	void Release() override;
};

