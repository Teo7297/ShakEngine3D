#pragma once

#include "Includes.h"

namespace Shak
{
    typedef int GameObjectHandle;

    class Component;
    class GameObject;

    struct GameObjectData
    {
        int watermark;
        GameObject* gameObject;
    };

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

        /// @brief Returns the first found GameObject with the given name
        /// @param name 
        /// @return GameObject*
        GameObject* FindGameObjectByName(const std::string& name);

        template<typename T>
        std::vector<T*> FindGameObjectsByType()
        {
            static_assert(std::is_base_of<GameObject, T>::value, "T must be a GameObject");

            std::vector<T*> result;
            for(const auto& go : m_gameObjects)
            {
                auto raw = dynamic_cast<T*>(go.get());
                if(raw)
                    result.push_back(raw);
            }
            return result;
        }

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

        std::unordered_map<GameObjectHandle, GameObjectData> m_gameObjectHandles;
    };
}