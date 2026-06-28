#pragma once
#include <string>
#include <DirectXMath.h>
#include <vector>
#include "../ImGUI/imgui.h"

class CircleCollider;

class BaseObject {
private:
    
protected:
    std::vector<CircleCollider*> colliderList;
    std::string name_;
    bool isDead_;
    bool isShowImGUI_;

    DirectX::XMFLOAT3 postion_;
    DirectX::XMFLOAT3 velocity_;
    DirectX::XMFLOAT3 rotation_;
    DirectX::XMFLOAT3 scale_;
public:

    BaseObject(const std::string& name, const bool isDead = false) {
        name_ = name;
        isDead_ = isDead;

        postion_ = { 0.0f, 0.0f, 0.0f };
        velocity_ = { 0.0f, 0.0f, 0.0f };
        rotation_ = { 0.0f, 0.0f, 0.0f };
        scale_ = { 1.0f, 1.0f, 1.0f };

        isShowImGUI_ = true;
    };
    virtual ~BaseObject() {}

    /// <summary>
    /// オブジェクトの状態を初期化する関数（※実装必須）
    /// </summary>
    virtual void Init() = 0;

    /// <summary>
    /// オブジェクトの状態を更新する関数（※実装必須）
    /// </summary>
    virtual void Update() = 0;

    /// <summary>
    /// オブジェクトを描画する関数（※実装必須）
    /// </summary>
    virtual void Draw() = 0;

    /// <summary>
    /// オブジェクトのデータを開放する関数（※実装必須）
    /// </summary>
    virtual void Release() = 0;

    /// <summary>
    /// オブジェクトの情報をImGUIで描画する関数
    /// </summary>
    virtual void DrawObjectInfoImGUI() {
        ImGui::Begin(name_.c_str());
        ImGui::SliderFloat("X", &postion_.x, -1280.0f, 1280.0f);
        ImGui::SliderFloat("Y", &postion_.y, -1280.0f, 1280.0f);
        ImGui::SliderFloat("Z", &postion_.z, -1280.0f, 1280.0f);
        ImGui::SliderFloat("angleX", &rotation_.x, 0.0f, 90.0f);
        ImGui::SliderFloat("angleY", &rotation_.y, 0.0f, 90.0f);
        ImGui::SliderFloat("angleZ", &rotation_.z, 0.0f, 90.0f);
        ImGui::SliderFloat("scaleX", &scale_.x, 0.0f, 10.0f);
        ImGui::SliderFloat("scaleY", &scale_.y, 0.0f, 10.0f);
        ImGui::SliderFloat("scaleZ", &scale_.z, 0.0f, 10.0f);
        ImGui::End();
    }

    /// <summary>
    /// 何かと当たったかを検出する関数
    /// </summary>
    /// <param name="obj">当たったオブジェクト</param>
    virtual void OnCollide(BaseObject* obj) {}

    std::string GetName() { return name_; };
    bool IsDead() { return isDead_; };
    void KillMe() { this->isDead_ = true; }
    bool IsShowImGUI() { return isShowImGUI_; }
    void SetShowImGUI(bool flag) { this->isShowImGUI_ = flag; }

    DirectX::XMFLOAT3 GetPosition() const { return postion_; }
    void SetPosition(const DirectX::XMFLOAT3 position) { this->postion_ = position; }
    DirectX::XMFLOAT3 GetVelocity() const { return velocity_; }
    void SetVelocity(const DirectX::XMFLOAT3 velocity) { this->velocity_ = velocity; }
    DirectX::XMFLOAT3 GetRotation() const { return rotation_; }
    void SetRotation(const DirectX::XMFLOAT3 rotation) { this->rotation_ = rotation; }
    DirectX::XMFLOAT3 GetScale() const { return scale_; }
    void SetScale(const DirectX::XMFLOAT3 scale) { this->scale_ = scale; }

    std::vector<CircleCollider*>& GetColiderList() { return colliderList; }
};