#include "AssetManager.h"
#include "Texture.h"
#include "Shader.h"
#include "Mesh.h"
#include "Material.h"
#include "CubeMap.h"

using namespace Shak;

AssetManager::AssetManager()
    :
    m_shaders{},
    m_persistentShaders{},
    m_textures{},
    m_persistentTextures{},
    m_cubemaps{},
    m_persistentCubemaps{},
    m_meshes{},
    m_persistentMeshes{},
    m_materials{},
    m_persistentMaterials{}
{

}

Shader* AssetManager::GetShader(const std::string& name, bool createIfNotPresent, bool persistent)
{
    if(m_shaders.contains(name))
        return m_shaders[name].get();

    if(m_persistentShaders.contains(name))
        return m_persistentShaders[name].get();

    if(!createIfNotPresent)
        return nullptr;

    if(persistent)
    {
        m_persistentShaders[name] = std::unique_ptr<Shader>(new Shader()); // Friend constructor hack
        return m_persistentShaders[name].get();
    }
    else
    {
        m_shaders[name] = std::unique_ptr<Shader>(new Shader()); // Friend constructor hack
        return m_shaders[name].get();
    }
}

Texture* AssetManager::GetTexture(const std::string& name, bool createIfNotPresent, bool persistent)
{
    if(m_textures.contains(name))
        return m_textures[name].get();

    if(m_persistentTextures.contains(name))
        return m_persistentTextures[name].get();

    if(!createIfNotPresent)
        return nullptr;

    if(persistent)
    {
        m_persistentTextures[name] = std::make_unique<Texture>();
        return m_persistentTextures[name].get();
    }
    else
    {
        m_textures[name] = std::make_unique<Texture>();
        return m_textures[name].get();
    }
}

CubeMap* AssetManager::GetCubeMap(const std::string& name, bool createIfNotPresent, bool persistent)
{
    if(m_cubemaps.contains(name))
        return m_cubemaps[name].get();

    if(m_persistentCubemaps.contains(name))
        return m_persistentCubemaps[name].get();

    if(!createIfNotPresent)
        return nullptr;

    if(persistent)
    {
        m_persistentCubemaps[name] = std::make_unique<CubeMap>();
        return m_persistentCubemaps[name].get();
    }
    else
    {
        m_cubemaps[name] = std::make_unique<CubeMap>();
        return m_cubemaps[name].get();
    }
}

Mesh* AssetManager::GetMesh(const std::string& name, bool createIfNotPresent, bool persistent)
{
    if(m_meshes.contains(name))
        return m_meshes[name].get();

    if(m_persistentMeshes.contains(name))
        return m_persistentMeshes[name].get();

    if(!createIfNotPresent)
        return nullptr;

    if(persistent)
    {
        m_persistentMeshes[name] = std::make_unique<Mesh>();
        return m_persistentMeshes[name].get();
    }
    else
    {
        m_meshes[name] = std::make_unique<Mesh>();
        return m_meshes[name].get();
    }
}

Material* AssetManager::GetMaterial(const std::string& name, bool createIfNotPresent, bool persistent)
{
    if(m_materials.contains(name))
        return m_materials[name].get();

    if(m_persistentMaterials.contains(name))
        return m_persistentMaterials[name].get();

    if(!createIfNotPresent)
        return nullptr;

    if(persistent)
    {
        m_persistentMaterials[name] = std::make_unique<Material>();
        return m_persistentMaterials[name].get();
    }
    else
    {
        m_materials[name] = std::make_unique<Material>();
        return m_materials[name].get();
    }
}

void AssetManager::DestroyShader(const std::string& name)
{
    if(m_shaders.contains(name))
        m_shaders.erase(name);
    else if(m_persistentShaders.contains(name))
        m_persistentShaders.erase(name);
}

void AssetManager::DestroyTexture(const std::string& name)
{
    if(m_textures.contains(name))
        m_textures.erase(name);
    else if(m_persistentTextures.contains(name))
        m_persistentTextures.erase(name);
}

void AssetManager::DestroyCubeMap(const std::string& name)
{
    if(m_cubemaps.contains(name))
        m_cubemaps.erase(name);
    else if(m_persistentCubemaps.contains(name))
        m_persistentCubemaps.erase(name);
}

void AssetManager::DestroyMesh(const std::string& name)
{
    if(m_meshes.contains(name))
        m_meshes.erase(name);
    else if(m_persistentMeshes.contains(name))
        m_persistentMeshes.erase(name);
}

void AssetManager::DestroyMaterial(const std::string& name)
{
    if(m_materials.contains(name))
        m_materials.erase(name);
    else if(m_persistentMaterials.contains(name))
        m_persistentMaterials.erase(name);
}

void AssetManager::ClearSceneData()
{
    m_shaders.clear();
    m_textures.clear();
    m_cubemaps.clear();
    m_meshes.clear();
    m_materials.clear();
}