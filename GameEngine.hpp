#pragma once

namespace GameEngine {
	inline const int DEFAULT_WIDTH = 620;
	inline const int DEFAULT_HEIGHT = 310;
	inline const float BACKGROUND_COLOR[4] = { 1.0f, 1.0f, 1.0f, 1.0f };

	HWND GetWindowHandle();
	float GetDeltaTime();

	void AppExit();
	bool CanExit();
}