#pragma once
#include "Engine//BaseScene.h"

class RunningScene : public BaseScene {
private:
public:
    RunningScene();
    ~RunningScene();

    void Init() override;
    void Update() override;
    void Draw() override;
    void Release() override;
};
