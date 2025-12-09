#pragma once

#include "Includes.h"

namespace Shak
{
    class Component;
    class GameObject;
    class Scene
    {
        friend class GameObject;
    public:
        Scene();
        virtual ~Scene();

        template<typename T>
        T* CreateGameObject(const std::string& name)
        {
            static_assert(std::is_base_of<GameObject, T>::value, "T must be a GameObject");

            // This hack allows to call the protected/private constructor of gameObject since make_shared is templated.
            auto go = std::unique_ptr<T>(new T());

            // Set metadata here
            go->m_scene = this;
            go->SetName(name);
            go->SetActive(true);

            // !Call awake after all expected data is set!
            go->OnAwake();

            m_pendingAdd.emplace_back(std::move(go));

            auto& added = *(m_pendingAdd.end() - 1);
            return (T*)added.get();
        }


        void DestroyGameObject(GameObject* toDestroy);

        GameObject* FindGameObjectByName(const std::string& name);

        void Update(float deltaTime);
        void UpdateTransformHierarchy();

    private:
        void RegisterComponent(Component* comp);
        void UnregisterComponent(Component* comp);
        void UpdateGameObjects(float deltaTime);
        void UpdateComponents(float deltaTime);
        /// @brief internal post update used to clean GameObjects state like pending components to destroy
        void PostUpdateGameObjects();

    protected:
        std::vector<std::unique_ptr<GameObject>> m_gameObjects, m_pendingAdd;
        std::vector<GameObject*> m_pendingDestroy;
        std::vector<Component*> m_components, m_pendingComponentsAdd;
    };
}