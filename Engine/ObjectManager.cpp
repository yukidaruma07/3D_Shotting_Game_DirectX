#include "ObjectManager.h"
#include "CircleCollider.h"

BaseObject* ObjectManager::AddObject(BaseObject* obj) {
    objList.push_back(obj);
    obj->Init();
    return obj;
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
                cirlceCol1->Update();
                for (CircleCollider* cirlceCol2 : obj2->GetColiderList()) {
                    cirlceCol2->Update();
                    isHit = cirlceCol1->IsCircle(cirlceCol2) && cirlceCol2->IsCircle(cirlceCol1);
                }
            }

            if (isHit) {
                obj1->OnCollide(obj2);
                obj2->OnCollide(obj1);
            }
        }
    }
}
