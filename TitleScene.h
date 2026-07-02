#pragma once
#include "Engine//BaseScene.h"
#include "ImageButton.h"

class TitleScene : public BaseScene {
private:
    ImageButton* startButton_;
    ImageButton* exitButton_;
public:
    TitleScene();
    ~TitleScene();

    void Init() override;
    void Update() override;
    void Draw() override;
    void Release() override;
};
