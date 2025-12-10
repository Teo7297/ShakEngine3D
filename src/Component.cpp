#include "Component.h"
#include "GameObject.h"
#include "Transform.h"

using namespace Shak;

Component::Component()
    :
    m_active{ false },
    m_owner{ nullptr },
    m_transform{ nullptr },
    m_started{ false },
    m_pendingKill{ false }
{

}

Component::~Component()
{

}

void Component::SetName(const std::string& name)
{
    m_name = name;
}

void Component::SetOwner(GameObject* owner)
{
    m_owner = owner;
    m_transform = owner->GetTransform();
}

GameObject* Component::GetOwner() const
{
    return m_owner;
}

void Component::SetStarted(bool started)
{
    m_started = started;
}

bool Component::HasStarted() const
{
    return m_started;
}

void Component::SetActive(bool active)
{
    m_active = active;
}

bool Component::IsActive() const
{
    return m_active;
}

void Shak::Component::SetPendingKill(bool pendingKill)
{
    m_pendingKill = pendingKill;
}

bool Shak::Component::IsPendingKill() const
{
    return m_pendingKill;
}
