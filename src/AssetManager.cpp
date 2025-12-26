#include "AssetManager.h"
#include "Texture.h"
#include "Shader.h"
#include "Mesh.h"
#include "Material.h"
#include "CubeMap.h"

using namespace Shak;

AssetManager::AssetManager()
    : m_shaders{},
    m_textures{},
    m_cubemaps{},
    m_meshes{},
    m_materials{}
{

}

Shader* AssetManager::GetShader(const std::string& name, bool createIfNotPresent)
{
    if(m_shaders.contains(name))
        return m_shaders[name].get();

    if(!createIfNotPresent)
        return nullptr;

    m_shaders[name] = std::unique_ptr<Shader>(new Shader()); // Friend constructor hack

    return m_shaders[name].get();
}

Texture* AssetManager::GetTexture(const std::string& name, bool createIfNotPresent)
{
    if(m_textures.contains(name))
        return m_textures[name].get();

    if(!createIfNotPresent)
        return nullptr;

    m_textures[name] = std::make_unique<Texture>();

    return m_textures[name].get();
}

CubeMap* AssetManager::GetCubeMap(const std::string& name, bool createIfNotPresent)
{
    if(m_cubemaps.contains(name))
        return m_cubemaps[name].get();

    if(!createIfNotPresent)
        return nullptr;

    m_cubemaps[name] = std::make_unique<CubeMap>();

    return m_cubemaps[name].get();
}

Mesh* AssetManager::GetMesh(const std::string& name, bool createIfNotPresent)
{
    if(m_meshes.contains(name))
        return m_meshes[name].get();

    if(!createIfNotPresent)
        return nullptr;

    m_meshes[name] = std::make_unique<Mesh>();

    return m_meshes[name].get();
}

Material* AssetManager::GetMaterial(const std::string& name, bool createIfNotPresent)
{
    if(m_materials.contains(name))
        return m_materials[name].get();

    if(!createIfNotPresent)
        return nullptr;

    m_materials[name] = std::make_unique<Material>();

    return m_materials[name].get();
}