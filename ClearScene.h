#pragma once
#include "Engine//BaseScene.h"

class ClearScene : public BaseScene {
private:
public:
    ClearScene();
    ~ClearScene();

    void Init() override;
    void Update() override;
    void Draw() override;
    void Release() override;
};
