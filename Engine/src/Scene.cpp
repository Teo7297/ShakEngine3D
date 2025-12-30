#include "Scene.h"
#include "GameObject.h"
#include "Transform.h"
#include "Component.h"
#include "components/CameraComponent.h"
#include "Renderer.h"
#include "AssetManager.h"
#include "SceneManager.h"
#include "UIManager.h"
#include "interfaces/IRenderable.h"
#include "interfaces/IAppControl.h"

using namespace Shak;

Scene::Scene()
    : m_name{ "Unnamed scene" },
    m_gameObjects{},
    m_pendingAdd{},
    m_pendingDestroy{},
    m_components{},
    m_pendingComponentsAdd{},
    m_gameObjectHandles{},
    m_mainCamera{ nullptr },
    m_renderables{},
    m_appContext{}
{

}

Scene::~Scene()
{

}

std::string Scene::GetName()
{
    return m_name;
}

void Scene::SetName(const std::string& name)
{
    m_name = name;
}

bool Scene::IsGameObjectValid(const GameObjectHandle& handle)
{
    return
        m_gameObjectHandles.contains(handle.watermark) &&
        m_gameObjectHandles[handle.watermark] == handle.gameObject;
}

void Scene::RegisterComponent(Component* comp)
{
    m_pendingComponentsAdd.emplace_back(comp);
    IRenderable* iren = dynamic_cast<IRenderable*>(comp);
    if(iren)
        m_renderables.emplace_back(iren);
}

void Scene::UnregisterComponent(Component* comp)
{
    std::erase_if(m_components, [&](auto* c) {return c == comp;});

    IRenderable* iren = dynamic_cast<IRenderable*>(comp);
    if(iren)
        std::erase_if(m_renderables, [&](auto* c) {return c == iren;});
}

void Scene::DestroyGameObject(GameObjectHandle handle)
{
    // 1. Check validity
    if(!this->IsGameObjectValid(handle))
        return;

    // 2. Invalidate
    m_gameObjectHandles.erase(handle.watermark);

    // 3. Apply destroy logic
    auto* toDestroy = handle.gameObject;
    for(auto* child : toDestroy->GetTransform()->GetChildren())
        m_pendingDestroy.emplace_back(child->GetGameObject());
    m_pendingDestroy.emplace_back(toDestroy);

    toDestroy->SetPendingKill(true); // This marks itself, its components and children recursively 
}

GameObjectHandle Scene::FindGameObjectByName(const std::string& name)
{
    for(const auto [wm, ptr] : m_gameObjectHandles)
        if(ptr->GetName() == name)
            return GameObjectHandle{ .watermark = wm, .gameObject = ptr };
    return InvalidHandle;
}

void Scene::Update(Renderer* renderer, float deltaTime)
{
    // Logic update
    UpdateGameObjects(deltaTime);
    UpdateComponents(deltaTime);
    PostUpdateGameObjects();

    // Renderer update
    renderer->Setup(m_mainCamera);
    for(auto* renderable : m_renderables)
        renderable->Draw(renderer);
}

void Scene::ProcessEvent(SDL_Event event)
{
    for(auto& go : m_gameObjects)
    {
        if(!go->IsActive())
            continue;

        go->ProcessEvent(event);
    }
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

CameraComponent* Scene::GetMainCamera()
{
    return m_mainCamera;
}

void Scene::SetMainCamera(CameraComponent* camera)
{
    m_mainCamera = camera;
}

void Scene::SetAppContext(AppContext& ctx)
{
    m_appContext = ctx;
}

AssetManager* Scene::GetAssetManager()
{
    return m_appContext.assetManager;
}

SceneManager* Scene::GetSceneManager()
{
    return m_appContext.sceneManager;
}

UIManager* Scene::GetUIManager()
{
    return m_appContext.uiManager;
}

Renderer* Scene::GetRenderer()
{
    return m_appContext.renderer;
}

IAppControl* Shak::Scene::GetAppControl()
{
    return m_appContext.appControl;
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

int Scene::CreateWatermark()
{
    return rand() % 100001;
}