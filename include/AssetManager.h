#pragma once

#include "Includes.h"
#include "Mesh.h" //???
#include "CubeMap.h" //???

namespace Shak
{
    class Shader;
    class Texture;
    // class CubeMap;
    // class Mesh;
    class Material;
    /// @brief Simple asset manager class.
    /// Memory of the assets is managed and owned by this class.
    class AssetManager
    {
    public:
        AssetManager();
        ~AssetManager() = default;
        
        Shader* GetShader(const std::string& name, bool createIfNotPresent = true);
        Texture* GetTexture(const std::string& name, bool createIfNotPresent = true);
        CubeMap* GetCubeMap(const std::string& name, bool createIfNotPresent = true);
        Mesh* GetMesh(const std::string& name, bool createIfNotPresent = true);
        Material* GetMaterial(const std::string& name, bool createIfNotPresent = true);

    private:
        std::unordered_map<std::string, std::unique_ptr<Shader>> m_shaders;
        std::unordered_map<std::string, std::unique_ptr<Texture>> m_textures;
        std::unordered_map<std::string, std::unique_ptr<CubeMap>> m_cubemaps;
        std::unordered_map<std::string, std::unique_ptr<Mesh>> m_meshes;
        std::unordered_map<std::string, std::unique_ptr<Material>> m_materials;
    };
}