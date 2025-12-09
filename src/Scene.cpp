#include "Scene.h"
#include "GameObject.h"
#include "Transform.h"

using namespace Shak;

Scene::Scene()
{

}

Scene::~Scene()
{

}

void Scene::DestroyGameObject(std::shared_ptr<GameObject> toDestroy)
{
    m_pendingDestroy.emplace_back(toDestroy);
}

void Scene::Update(float deltaTime)
{
    // Add pending objects
    for(auto& go : m_pendingAdd)
        m_gameObjects.emplace_back(go);
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

    // Process destroyed objects
    for(auto& toDestroy : m_pendingDestroy)
    {
        toDestroy->OnDestroy();
        std::erase_if(m_gameObjects, [&](const auto& go) {return go == toDestroy;});
    }
    m_pendingDestroy.clear();

    this->UpdateTransformHierarchy();
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
