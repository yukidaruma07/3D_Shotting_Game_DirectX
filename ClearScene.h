#pragma once
#include "Engine//BaseScene.h"

class ClearScene : public BaseScene {
private:
    float count_;
public:
    ClearScene();
    ~ClearScene();

    void Init() override;
    void Update() override;
    void Draw() override;
    void Release() override;
};
