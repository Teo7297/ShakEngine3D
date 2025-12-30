#pragma once

#include "Includes.h"
#include "Material.h"
#include "Shader.h"

namespace Shak
{
    class Mesh;
    class CameraComponent;
    struct RenderCommand
    {
        Mesh* mesh;
        Material* material;
        glm::mat4 modelMatrix;
        bool isSkybox;
        GLint renderMode;

        // Helper for sorting
        bool operator<(const RenderCommand& other) const {
            auto thisShader = material->GetShader();
            auto otherShader = other.material->GetShader();
            if(thisShader != otherShader)
                return thisShader < otherShader; // Group by Shader first
            return material < other.material; // Group by Material instance second
        }
    };

    struct SceneData
    {
        glm::mat4 viewProjectionMatrix;
        glm::vec3 cameraPosition;
        // lights pos?
    };

    class Renderer
    {
    public:
        Renderer();
        ~Renderer();
        void Setup(CameraComponent* camera);
        void Submit(RenderCommand command);
        void RenderScene();
        void RenderUI();

    private:
        std::vector<RenderCommand> m_renderQueue;
        SceneData m_sceneData;
        bool m_cameraSetThisFrame;
        CameraComponent* m_camera;
    };
}