#pragma once
#include <string>
#include <DirectXMath.h>

class Camera {
private:
	std::string name_;
	DirectX::XMFLOAT3 cameraPostion_;
	DirectX::XMFLOAT3 foucusPostion_;
	DirectX::XMFLOAT3 upDirection_;
	DirectX::XMMATRIX projection_;
public:

	Camera(std::string name) {
		name_ = name;
		cameraPostion_ = { 0.0f, 0.0f, -5.0f };
		foucusPostion_ = { 0.0f, 2.0f, 0.0f };
		upDirection_ = { 0.0f, 1.0f, 0.0f };
		projection_ = DirectX::XMMatrixIdentity();
	}

	Camera(std::string name, DirectX::XMFLOAT3 cameraPos, DirectX::XMFLOAT3 foucusPos) {
		name_ = name;
		this->cameraPostion_ = cameraPos;
		this->foucusPostion_ = foucusPos;
		this->upDirection_ = { 0.0f, 1.0f, 0.0f };
		projection_ = DirectX::XMMatrixIdentity();
	}

	inline std::string getName() { return name_; }

	/// <summary>
	/// 場所+注視点+カメラの上方向を組み合わせた、行列を返す関数。
	/// </summary>
	inline DirectX::XMMATRIX getMatrix() {
		return DirectX::XMMatrixLookAtLH(
			DirectX::XMVectorSet(cameraPostion_.x, cameraPostion_.y, cameraPostion_.z, 1.0f),
			DirectX::XMVectorSet(foucusPostion_.x, foucusPostion_.y, foucusPostion_.z, 1.0f),
			DirectX::XMVectorSet(upDirection_.x, upDirection_.y, upDirection_.z, 1.0f)
		);
	}

	/// <summary>
	/// カメラの場所を取得する関数
	/// </summary>
	inline DirectX::XMFLOAT3 getCameraPostion() {
		return cameraPostion_;
	}

	/// <summary>
	/// カメラの場所を変更する関数
	/// </summary>
	inline void setCameraPostion(DirectX::XMFLOAT3 cameraPos) {
		this->cameraPostion_ = cameraPos;
	}

	/// <summary>
	/// カメラの注視点（※見る場所）を取得する関数
	/// </summary>
	inline DirectX::XMFLOAT3 getFoucsPostion() {
		return foucusPostion_;
	}

	/// <summary>
	/// カメラの注視点（※見る場所）を変更する関数
	/// </summary>
	inline void setFoucsPostion(DirectX::XMFLOAT3 foucsPos) {
		this->foucusPostion_ = foucsPos;
	}

	inline DirectX::XMMATRIX GetProjection() {
		return projection_;
	}

	inline void SetProjection(DirectX::XMMATRIX matrix) {
		this->projection_ = matrix;
	}
};

/// <summary>
/// カメラを管理する名前空間
/// </summary>
namespace CameraManager {

	/// <summary>
	/// 新しいカメラを追加する関数
	/// </summary>
	/// <param name="name">追加したいカメラの名前</param>
	void addCamera(const std::string& name);

	/// <summary>
	/// 現在のカメラを変更する関数
	/// </summary>
	/// <param name="name">変更したいカメラの名前</param>
	void setCurentCamera(const std::string& name);

	/// <summary>
	/// カメラを取得する関数
	/// </summary>
	/// <param name="name">取得したいカメラの名前</param>
	/// <returns>引数に指定したカメラ（※見つからない場合はnullptrを返します。）</returns>
	Camera* getCamera(const std::string& name);

	/// <summary>
	/// 現在のカメラを取得する関数
	/// </summary>
	/// <returns>現在のカメラを返す（※見つからない場合はnullptrを返します。）</returns>
	Camera* getCurentCamera();
}

