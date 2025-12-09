#include "GameObject.h"
#include "Transform.h"
#include "Material.h"
#include "Shader.h"
#include "Component.h"

using namespace Shak;
struct MatrixBlock {
    glm::mat4 model;
    glm::mat4 view;
    glm::mat4 projection;
};

GameObject::GameObject()
    :
    m_active{ false },
    m_name{ "New Game Object" },
    m_started{ false },
    m_transform{ std::make_shared<Transform>() },
    m_components{}
{
}

GameObject::~GameObject()
{
}

const std::shared_ptr<Transform>& GameObject::GetTransform() const
{
    return m_transform;
}

void GameObject::AttachComponent(std::shared_ptr<Component> component)
{
    component->SetOwner(this);
    m_components.emplace_back(std::move(component));
}

bool GameObject::HasStarted() const
{
    return m_started;
}

void GameObject::SetStarted(bool started)
{
    m_started = started;
}

void GameObject::SetName(const std::string& name)
{
    m_name = name;
}

std::string GameObject::GetName() const
{
    return m_name;
}

void Shak::GameObject::SetActive(bool active)
{
    m_active = active;
}

bool Shak::GameObject::IsActive() const
{
    return m_active;
}

Component* GameObject::GetComponent(int index)
{
    return m_components[index].get();
}
