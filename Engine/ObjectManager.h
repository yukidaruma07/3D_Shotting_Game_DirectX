#pragma once
#include <vector>
#include "BaseObject.h"

namespace ObjectManager {
    inline std::vector<BaseObject*> objList;

    /// <summary>
    /// オブジェクトを追加する関数
    /// </summary>
    /// <param name="obj">追加したいオブジェクト</param>
    /// <returns>オブジェクトの初期化が完了すると、追加したオブジェクトのポインタを返します</returns>
    BaseObject* AddObject(BaseObject* obj);
    void RemoveObject(BaseObject* obj);
    void ClearObject();
    void UpdateManager();
    void UpdateCollider();

    template<class T> T* GetDrawObject() {
        for (BaseObject* obj : objList) {
            if (obj == nullptr) continue;
            T* instance = dynamic_cast<T*>(obj);
            if (instance != nullptr) {
                return instance;
            }
        }
        return nullptr;
    }

    template<class T> std::vector<T*> GetDrawObjectList() {
        std::vector<T*> getList;
        for (BaseObject* obj : objList) {
            if (obj == nullptr) continue;
            T* instance = dynamic_cast<T*>(obj);
            if (instance != nullptr) {
                getList.push_back(obj);
            }
        }
        return getList;
    }
}