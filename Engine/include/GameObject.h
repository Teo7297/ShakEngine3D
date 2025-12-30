#pragma once

#include "Includes.h"
#include "Scene.h"

namespace Shak
{
    class Transform;
    class Material;
    class Component;
    class GameObject
    {
        friend class Scene; // This allows to have only scene to have access to the constructor

    protected:
        GameObject();

    public:
        virtual ~GameObject();

        Transform* GetTransform() const;

        bool HasStarted() const;
        void SetStarted(bool started);

        void SetName(const std::string& name);
        std::string GetName() const;

        void SetActive(bool active);
        bool IsActive() const;

        // Add a component to this gameobject and returns a ptr to it. Memory is owned by GameObject.
        template<typename T>
        T* AddComponent(const std::string& name)
        {
            static_assert(std::is_base_of<Component, T>::value, "T must be a Component");

            auto comp = std::unique_ptr<T>(new T()); //? we use this manual new "hack" for portability even though it works with MSVC.
            comp->SetName(name);
            comp->SetActive(true);
            comp->OnAwake();
            m_scene->RegisterComponent(comp.get());
            return (T*)this->AttachComponent(std::move(comp));
        }

        template<typename T>
        std::vector<T*> GetComponentsByType() const
        {
            static_assert(std::is_base_of<Component, T>::value, "T must be a subclass of Component");
            std::vector<T*> result;
            for(const auto& comp : m_components)
            {
                auto rawTPtr = dynamic_cast<T*>(comp.get());
                if(rawTPtr)
                    result.emplace_back(rawTPtr);
            }
            return result;
        }

        std::vector<Component*> GetComponents() const;

        void SetPendingKill(bool kill);
        bool IsPendingKill() const;

        void DestroyComponent(Component* comp);
        void DestroyAllComponents();

        virtual void OnAwake() {}
        virtual void OnStart() {}
        virtual void OnUpdate(float deltaTime) {}
        virtual void ProcessEvent(SDL_Event event) {};
        virtual void OnLateUpdate(float deltaTime) {}
        virtual void OnDestroy() {}


    private:
        void OnPostUpdate();
        Component* AttachComponent(std::unique_ptr<Component> component);

    protected:
        bool m_active;
        std::string m_name;
        bool m_started;
        std::unique_ptr<Transform> m_transform;
        std::vector<std::unique_ptr<Component>> m_components;
        std::vector<Component*> m_pendingComponentsDestroy;
        Scene* m_scene;
        bool m_pendingKill;
    };
}