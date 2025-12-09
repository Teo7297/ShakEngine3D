#pragma once

#include "Includes.h"

namespace Shak
{
    class GameObject;
    class Transform;
    class Component
    {
        friend class GameObject; // This allows to have only gameobject to have access to the constructor

    protected:
        Component();

    public:
        virtual ~Component();

        void SetOwner(GameObject* owner);
        GameObject* GetOwner() const;

        void SetStarted(bool started);
        bool HasStarted() const;

        void SetActive(bool active);
        bool IsActive() const;

        void SetPendingKill(bool pendingKill);
        bool IsPendingKill() const;

        virtual void OnAwake() {}
        virtual void OnStart() {}
        virtual void OnUpdate(float deltaTime) {}
        virtual void OnDestroy() {}

    protected:
        bool m_active;
        GameObject* m_owner;
        Transform* m_transform;
        bool m_started;
        bool m_pendingKill;
    };
}