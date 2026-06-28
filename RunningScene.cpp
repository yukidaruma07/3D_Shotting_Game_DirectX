#include "RunningScene.h"
#include "Engine/ObjectManager.h"
#include "Engine/FBX.h"
#include "Player.h"
#include "Bullet.h"
#include "Enemy.h"

RunningScene::RunningScene()
	: BaseScene("RunningScene") {
}

RunningScene::~RunningScene() {
}

void RunningScene::Init() {
	ObjectManager::AddObject(new Enemy({ 0, 0, 20 }));
	ObjectManager::AddObject(new Player({}));
	//ObjectManager::AddObject(new Bullet({}));
}

void RunningScene::Update() {
}

void RunningScene::Draw() {
}

void RunningScene::Release() {
}
