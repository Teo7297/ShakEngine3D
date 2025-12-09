#include "Component.h"
#include "GameObject.h"
#include "Transform.h"

using namespace Shak;

Component::Component()
    :
    m_owner{ nullptr },
    m_transform{ nullptr },
    m_started{ false }
{

}

Component::~Component()
{

}

void Component::SetOwner(GameObject* owner)
{
    m_owner = owner;
    m_transform = owner->GetTransform().get();
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