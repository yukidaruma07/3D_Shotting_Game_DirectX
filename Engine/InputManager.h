#pragma once
#include <dinput.h>

namespace InputManager {

	/// <summary>
	/// InputManagerを初期化する関数
	/// </summary>
	/// <param name="hInstance">ウインドウのインスタンス</param>
	/// <param name="hwnd">ウインドウのハンドル</param>
	/// <returns></returns>
	int initialize(HINSTANCE hInstance, HWND hwnd);
	void update();
	void release();

	/// <summary>
	/// キーボードが押されたかを返す関数（※押しっぱなし関係無し）
	/// </summary>
	/// <param name="key">キーボードのキーの種類</param>
	/// <returns>押されている場合はtrueを返し、押されていない場合はfalseを返す。</returns>
	bool IsPushKey(int key);

	/// <summary>
	/// マウスが押されたかを返す関数（※押しっぱなし関係無し）
	/// </summary>
	/// <param name="button">マウスのボタンの種類</param>
	/// <returns>押されている場合はtrueを返し、押されていない場合はfalseを返す。</returns>
	bool IsPushMouse(int button);

	/// <summary>
	/// 現在のマウスの座標を取得する関数。（※InputManagerのupdateで更新される）
	/// </summary>
	/// <returns></returns>
	POINT GetMousePoint();
}