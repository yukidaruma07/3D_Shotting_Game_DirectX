#pragma once
#include "Engine/BaseObject.h"
#include "Engine/FBX.h"

class Ground : public BaseObject {
private:
	FBX* fbx_;
public:

	Ground();
	~Ground();

	void Init() override;
	void Update() override;
	void Draw() override;
	void Release() override;

	FBX* GetFBX() const { return fbx_; }
};