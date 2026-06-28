#include "SceneManager.h"
#include <vector>
#include "BootScene.h"
#include "ObjectManager.h"
#include "../TitleScene.h"
#include "../RunningScene.h"
#include "../ClearScene.h"

namespace {
	std::vector<BaseScene*> sceneList;
	BaseScene* currentScene = nullptr;
}

namespace SceneManager {

	void InitManager() {
		sceneList.clear();
		auto bootScene = new BootScene();
		auto titleScene = new TitleScene();
		auto runningScene = new RunningScene();
		auto clearScene = new ClearScene();
		sceneList.push_back(bootScene);
		sceneList.push_back(titleScene);
		sceneList.push_back(runningScene);
		sceneList.push_back(clearScene);
		ChangeScene("BootScene");
	}

	BaseScene* GetCurrentScene() {
		return currentScene;
	}

	void ChangeScene(const std::string& name) {
		for (auto& scene : sceneList) {
			if (scene->GetName() == name) {
				currentScene = scene;
				ObjectManager::ClearObject();
				currentScene->Init();
				ObjectManager::InitManager();
				break;
			}
		}
	}

	void DrawScene() {
		if (currentScene == nullptr) return;
		currentScene->Draw();
	}

	void InitScene() {
		if (currentScene == nullptr) return;
		currentScene->Init();
	}

	void UpdateScene() {
		if (currentScene == nullptr) return;
		currentScene->Update();
	}

	void ReleaseScene() {
		if (currentScene == nullptr) return;
		currentScene->Release();
	}

}
