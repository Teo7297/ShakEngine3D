#include "Scene.h"
#include "GameObject.h"
#include "Transform.h"
#include "Component.h"

using namespace Shak;

Scene::Scene()
{

}

Scene::~Scene()
{

}

void Scene::RegisterComponent(Component* comp)
{
    m_pendingComponentsAdd.emplace_back(comp);
}

void Scene::UnregisterComponent(Component* comp)
{
    std::erase_if(m_components, [&](auto* c) {return c == comp;});
}

void Scene::DestroyGameObject(GameObject* toDestroy)
{
    for(auto* child : toDestroy->GetTransform()->GetChildren())
        m_pendingDestroy.emplace_back(child->GetGameObject());
    m_pendingDestroy.emplace_back(toDestroy);

    toDestroy->SetPendingKill(true); // This marks itself, its components and children recursively 
}

GameObject* Scene::FindGameObjectByName(const std::string& name)
{
    for(const auto& go : m_gameObjects)
    {
        if(go->m_name == name)
        {
            return go.get();
        }
    }
    return nullptr;
}

void Scene::Update(float deltaTime)
{
    UpdateGameObjects(deltaTime);
    UpdateComponents(deltaTime);
    PostUpdateGameObjects();
}

void Scene::UpdateTransformHierarchy()
{
    for(auto& go : m_gameObjects)
    {
        auto tx = go->GetTransform();
        // We call this on root objects, they will propagate recursively
        if(tx->GetParent() == nullptr)
            tx->UpdateMatrices(glm::identity<glm::mat4>());
    }
}

void Scene::UpdateGameObjects(float deltaTime)
{
    // Add pending objects
    m_gameObjects.insert
    (
        m_gameObjects.end(),
        std::make_move_iterator(m_pendingAdd.begin()),
        std::make_move_iterator(m_pendingAdd.end())
    );
    m_pendingAdd.clear();

    // Main logic
    for(auto& go : m_gameObjects)
    {
        if(!go->IsActive())
            continue;

        if(!go->HasStarted())
        {
            go->OnStart();
            go->SetStarted(true);
        }

        go->OnUpdate(deltaTime);
    }

    for(auto& go : m_gameObjects)
    {
        if(!go->IsActive())
            continue;
        go->OnLateUpdate(deltaTime);
    }

    this->UpdateTransformHierarchy();
}

void Scene::UpdateComponents(float deltaTime)
{
    // Add pending objects
    m_components.insert
    (
        m_components.end(),
        m_pendingComponentsAdd.begin(),
        m_pendingComponentsAdd.end()
    );
    m_pendingComponentsAdd.clear();

    // Main logic
    for(auto* comp : m_components)
    {
        if(!comp->IsActive())
            continue;

        if(!comp->HasStarted())
        {
            comp->OnStart();
            comp->SetStarted(true);
        }

        comp->OnUpdate(deltaTime);
    }
}

void Scene::PostUpdateGameObjects()
{
    // GameObjects can clear their pendingKill components for example
    for(auto& go : m_gameObjects)
    {
        if(!go->IsActive())
            continue;

        go->OnPostUpdate();
    }

    // Process destroyed objects
    for(auto& toDestroy : m_pendingDestroy)
    {
        toDestroy->OnDestroy();
        std::erase_if(m_gameObjects, [&](const auto& go) {return go.get() == toDestroy;});
    }
    m_pendingDestroy.clear();
}
