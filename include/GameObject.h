#pragma once

#include "Includes.h"

namespace Shak
{
    class Transform;
    class Material;
    class Component;
    class GameObject
    {
    public:
        GameObject();
        virtual ~GameObject();

        const std::shared_ptr<Transform>& GetTransform() const;
        void AttachComponent(std::shared_ptr<Component> component);

        bool HasStarted() const;
        void SetStarted(bool started);

        void SetName(const std::string& name);
        std::string GetName() const;

        void SetActive(bool active);
        bool IsActive() const;

        Component* GetComponent(int index);

        virtual void OnAwake() {}
        virtual void OnStart() {}
        virtual void OnUpdate(float deltaTime) {}
        virtual void OnDestroy() {}

    protected:

    protected:
        bool m_active;
        std::string m_name;
        bool m_started;
        std::shared_ptr<Transform> m_transform;
        std::vector<std::shared_ptr<Component>> m_components;
    };
}