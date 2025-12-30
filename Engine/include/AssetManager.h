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
        
        Shader* GetShader(const std::string& name, bool createIfNotPresent = true, bool persistent = false);
        Texture* GetTexture(const std::string& name, bool createIfNotPresent = true, bool persistent = false);
        CubeMap* GetCubeMap(const std::string& name, bool createIfNotPresent = true, bool persistent = false);
        Mesh* GetMesh(const std::string& name, bool createIfNotPresent = true, bool persistent = false);
        Material* GetMaterial(const std::string& name, bool createIfNotPresent = true, bool persistent = false);

        void DestroyShader(const std::string& name);
        void DestroyTexture(const std::string& name);
        void DestroyCubeMap(const std::string& name);
        void DestroyMesh(const std::string& name);
        void DestroyMaterial(const std::string& name);

        void ClearSceneData();

    private:
        std::unordered_map<std::string, std::unique_ptr<Shader>> m_shaders;
        std::unordered_map<std::string, std::unique_ptr<Shader>> m_persistentShaders;
        std::unordered_map<std::string, std::unique_ptr<Texture>> m_textures;
        std::unordered_map<std::string, std::unique_ptr<Texture>> m_persistentTextures;
        std::unordered_map<std::string, std::unique_ptr<CubeMap>> m_cubemaps;
        std::unordered_map<std::string, std::unique_ptr<CubeMap>> m_persistentCubemaps;
        std::unordered_map<std::string, std::unique_ptr<Mesh>> m_meshes;
        std::unordered_map<std::string, std::unique_ptr<Mesh>> m_persistentMeshes;
        std::unordered_map<std::string, std::unique_ptr<Material>> m_materials;
        std::unordered_map<std::string, std::unique_ptr<Material>> m_persistentMaterials;
    };
}