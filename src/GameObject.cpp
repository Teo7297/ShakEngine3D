#include "GameObject.h"
#include "Transform.h"
#include "Material.h"
#include "Shader.h"
#include "Component.h"
#include "Scene.h"

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
    m_components{},
    m_scene{ nullptr }
{
}

GameObject::~GameObject()
{
}

const std::shared_ptr<Transform>& GameObject::GetTransform() const
{
    return m_transform;
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

void GameObject::SetActive(bool active)
{
    m_active = active;
}

bool GameObject::IsActive() const
{
    return m_active;
}

void GameObject::DestroyComponent(Component* comp)
{
    comp->SetPendingKill(true);
}

void GameObject::RegisterComponentOnScene(Component* comp)
{
    m_scene->RegisterComponent(comp);
}

void GameObject::OnPostUpdate()
{
    // Actually remove pending kill components
    auto it = std::remove_if(m_components.begin(), m_components.end(),
        [this](const std::unique_ptr<Component>& c) {
            if(c->IsPendingKill()) {
                c->OnDestroy();
                m_scene->UnregisterComponent(c.get());
                return true; // Delete it
            }
            return false;
        });

    m_components.erase(it, m_components.end());
}

Component* GameObject::AttachComponent(std::unique_ptr<Component> component)
{
    component->SetOwner(this);
    m_components.push_back(std::move(component));
    return (m_components.end() - 1)->get();
}

std::vector<Component*> GameObject::GetComponents() const
{
    std::vector<Component*> componentsPtrs;
    componentsPtrs.reserve(m_components.size());
    for(const auto& comp : m_components)
        componentsPtrs.emplace_back(comp.get());

    return componentsPtrs;
}
