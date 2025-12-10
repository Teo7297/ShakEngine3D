#pragma once

#include "Includes.h"

namespace Shak
{
    class Component;
    class GameObject;

    struct GameObjectHandle
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

        bool IsGameObjectValid(const GameObjectHandle& handle);

        template<typename T>
        GameObjectHandle CreateGameObject(const std::string& name)
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

            auto rawAdded = (m_pendingAdd.end() - 1)->get();
            int watermark = this->CreateWatermark();

            m_gameObjectHandles[watermark] = rawAdded;

            return GameObjectHandle{ .watermark = watermark, .gameObject = rawAdded };
        }

        void DestroyGameObject(GameObjectHandle handle);

        /// @brief Returns the first found GameObject with the given name
        /// @param name 
        /// @return GameObject*
        GameObjectHandle FindGameObjectByName(const std::string& name);

        template<typename T>
        std::vector<GameObjectHandle> FindGameObjectsByType()
        {
            static_assert(std::is_base_of<GameObject, T>::value, "T must be a GameObject");

            std::vector<GameObjectHandle> result;
            for(const auto [wm, ptr] : m_gameObjectHandles)
            {
                auto* tPtr = dynamic_cast<T*>(ptr);
                if(tPtr)
                    result.emplace_back(GameObjectHandle{ .watermark = wm, .gameObject = ptr });
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
        int CreateWatermark();

    public:
        GameObjectHandle InvalidHandle = GameObjectHandle{ .watermark = -1, .gameObject = nullptr };

    protected:
        std::vector<std::unique_ptr<GameObject>> m_gameObjects, m_pendingAdd;
        std::vector<GameObject*> m_pendingDestroy;
        std::vector<Component*> m_components, m_pendingComponentsAdd;

        std::unordered_map<int, GameObject*> m_gameObjectHandles;
    };
}