#include "ClearScene.h"
#include "Engine/DirectX2DManager.h"
#include "GameEngine.hpp"
#include "Engine/SceneManager.h"

using namespace DirectX2DManager;

ClearScene::ClearScene()
	: BaseScene("ClearScene") {
	count_ = 0;
}

ClearScene::~ClearScene()
{
}

void ClearScene::Init()
{
}

void ClearScene::Update() {
	count_ += GameEngine::GetDeltaTime();
	if (count_ > 10) {
		SceneManager::ChangeScene("TitleScene");
	}
}

void ClearScene::Draw() {
	DrawFontText(0, 0, L"ゲーム終了!");
}

void ClearScene::Release() {
}
