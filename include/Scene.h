#pragma once

#include "Includes.h"

namespace Shak
{
    class GameObject;
    class Component;
    class Scene
    {
    public:
        Scene();
        virtual ~Scene();

        template<typename T>
        T* CreateGameObject(const std::string& name)
        {
            static_assert(std::is_base_of<GameObject, T>::value, "T must be a GameObject");

            // This hack allows to call the protected/private constructor of gameObject since make_shared is templated.
            auto go = std::shared_ptr<T>(new T());

            go->SetName(name);
            go->SetActive(true);
            go->OnAwake();
            m_pendingAdd.emplace_back(go);
            return go.get();
        }

        // This is for internal use only! Do not call this from game logic.
        void RegisterNewComponent(Component* comp);

        void DestroyGameObject(std::shared_ptr<GameObject> toDestroy);
        void Update(float deltaTime);
        void UpdateTransformHierarchy();

    protected:
        std::vector<std::shared_ptr<GameObject>> m_gameObjects, m_pendingAdd, m_pendingDestroy;
        std::vector<Component*> m_components, m_pendingComponentsAdd, m_pendingComponentsDestroy;
    };
}