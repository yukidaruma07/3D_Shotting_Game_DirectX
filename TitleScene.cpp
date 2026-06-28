#include "TitleScene.h"
#include "Engine/InputManager.h"
#include "Engine/DirectX2DManager.h"
#include "Engine/SceneManager.h"

TitleScene::TitleScene()
	: BaseScene("TitleScene") {
}

TitleScene::~TitleScene() {
}

void TitleScene::Init() {
}

void TitleScene::Update() {
	if (InputManager::IsPushKey(DIK_SPACE)) {
		SceneManager::ChangeScene("RunningScene");
	}
}

void TitleScene::Draw() {
	DirectX2DManager::DrawFontText(0, 0, L"ゲームを開始するにはスペースを押してください");
}

void TitleScene::Release() {
}
