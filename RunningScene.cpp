#include "RunningScene.h"
#include "Engine/ObjectManager.h"
#include "Engine/FBX.h"
#include "Player.h"
#include "Bullet.h"
#include "Enemy.h"
#include "Engine/DirectX2DManager.h"
#include "GameEngine.hpp"
#include "Engine/SceneManager.h"

using namespace DirectX2DManager;

namespace {
	const float DEFAULT_TIME = 60.0f;
}

RunningScene::RunningScene()
	: BaseScene("RunningScene") {
	time_ = DEFAULT_TIME;
}

RunningScene::~RunningScene() {
}

void RunningScene::Init() {
	ObjectManager::AddObject(new Enemy({ 0, 0, 20 }));
	ObjectManager::AddObject(new Player({}));
	//ObjectManager::AddObject(new Bullet({}));
}

void RunningScene::Update() {
	time_ -= GameEngine::GetDeltaTime();

	if (time_ <= 0) {
		SceneManager::ChangeScene("ClearScene");
	}
}

void RunningScene::Draw() {
	DrawFontText(-100, -100, L"タイマー：" + std::to_wstring(time_));
}

void RunningScene::Release() {
}
