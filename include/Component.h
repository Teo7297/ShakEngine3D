#pragma once

#include "Includes.h"

namespace Shak
{
    class GameObject;
    class Transform;
    class Component
    {
    public:
        Component();
        virtual ~Component();

        void SetOwner(GameObject* owner);
        GameObject* GetOwner() const;

        void SetStarted(bool started);
        bool HasStarted() const;

        virtual void OnAwake() {}
        virtual void OnStart() {}
        virtual void OnUpdate() {}
        virtual void OnDestroy() {}

    protected:
        GameObject* m_owner;
        Transform* m_transform;
        bool m_started;
    };
}