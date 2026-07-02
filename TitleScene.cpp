#include "TitleScene.h"
#include "Engine/InputManager.h"
#include "Engine/DirectX2DManager.h"
#include "Engine/SceneManager.h"
#include "Engine/ObjectManager.h"
#include "GameEngine.hpp"

TitleScene::TitleScene()
	: BaseScene("TitleScene") {
	startButton_ = nullptr;
	exitButton_ = nullptr;
}

TitleScene::~TitleScene() {
}

void TitleScene::Init() {
	//ObjectManager::AddObject(new Image("test.PNG", 64, 64));
	//ObjectManager::AddObject(new Image("Asset/StartButton.png", 512, 256));
	startButton_ = (ImageButton*) ObjectManager::AddObject(new ImageButton("Asset/StartButton.png", 512, 256, DirectX::XMFLOAT3(20, 20, 0)));
	exitButton_ = (ImageButton*) ObjectManager::AddObject(new ImageButton("Asset/ExitButton.png", 512, 256, DirectX::XMFLOAT3(20, 250, 0)));
}

void TitleScene::Update() {

	if (InputManager::IsPushKey(DIK_SPACE)) {
		SceneManager::ChangeScene("RunningScene");
	}

	if (startButton_->IsClicked()) {
		SceneManager::ChangeScene("RunningScene");
	}
	if (exitButton_->IsClicked()) {
		GameEngine::AppExit();
	}
}

void TitleScene::Draw() {
	//DirectX2DManager::DrawFontText(0, 0, L"ゲームを開始するにはスペースを押してください");
}

void TitleScene::Release() {
}
