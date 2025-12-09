#pragma once

#include "Includes.h"

namespace Shak
{
    class GameObject;
    class Scene
    {
    public:
        Scene();
        virtual ~Scene();

        template<typename T>
        T* CreateGameObject(const std::string& name)
        {
            static_assert(std::is_base_of<GameObject, T>::value, "T must be a GameObject");

            auto go = std::make_shared<T>();
            go->SetName(name);
            go->SetActive(true);
            go->OnAwake();
            m_pendingAdd.emplace_back(go);
            return go.get();
        }

        void DestroyGameObject(std::shared_ptr<GameObject> toDestroy);
        void Update(float deltaTime);
        void UpdateTransformHierarchy();

    protected:
        std::vector<std::shared_ptr<GameObject>> m_gameObjects, m_pendingAdd, m_pendingDestroy;
    };
}