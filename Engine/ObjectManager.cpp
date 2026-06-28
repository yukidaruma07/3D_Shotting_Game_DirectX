#include "ObjectManager.h"
#include "CircleCollider.h"

void ObjectManager::AddObject(BaseObject* obj) {
    objList.push_back(obj);
    obj->Init();
}

void ObjectManager::RemoveObject(BaseObject* obj) {
    for (auto it = objList.begin(); it != objList.end(); ) {
        if ((*it) == obj) {
            delete* it;
            it = objList.erase(it);
        }
        else {
            it++;
        }
    }
}

void ObjectManager::ClearObject() {
    objList.clear();
}

void ObjectManager::UpdateManager() {
    for (int n = 0; n < objList.size(); n++) {
        auto obj = objList[n];
        if (obj == nullptr) continue;
        if (obj->IsDead()) {
            RemoveObject(obj);
        }
        else {
            obj->Update();
            UpdateCollider();
            obj->Draw();
            #ifdef _DEBUG
                if (!obj->IsShowImGUI()) return;
                obj->DrawObjectInfoImGUI();
            #endif
        }
    }
}

void ObjectManager::UpdateCollider() {
    for (int n = 0; n < objList.size(); n++) {
        for (int i = 1; i < objList.size(); i++) {
            auto obj1 = objList[n];
            auto obj2 = objList[i];
            
            bool isHit = false;
            for (CircleCollider* cirlceCol1 : obj1->GetColiderList()) {
                for (CircleCollider* cirlceCol2 : obj2->GetColiderList()) {
                    isHit = cirlceCol1->IsCircle(cirlceCol2);
                }
            }

            if (isHit) {
                obj1->OnCollide(obj2);
                obj2->OnCollide(obj1);
            }
        }
    }
}
