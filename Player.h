#pragma once
#include "Engine/BaseObject.h"
#include <DirectXMath.h>
#include "Engine/FBX.h"

class Player : public BaseObject{
private:
	FBX* fbxModel_;
public:
	Player(DirectX::XMFLOAT3 postion);
	~Player();

	void Init() override;
	void Update() override;
	void Draw() override;
	void Release() override;
	void DrawObjectInfoImGUI() override;
};

