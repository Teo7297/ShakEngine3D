#include "Renderer.h"
#include "Mesh.h"
#include "components/CameraComponent.h"

using namespace Shak;

Renderer::Renderer()
    : m_renderQueue{}
    , m_cameraSetThisFrame{ false }
{
    m_sceneData.viewProjectionMatrix = glm::identity<glm::mat4>();
    m_sceneData.cameraPosition = glm::vec3(0.f);
}

Renderer::~Renderer()
{

}

void Renderer::Setup(CameraComponent* camera)
{
    // get camera data and set UBO for vp matrix
    m_sceneData.viewProjectionMatrix = camera->GetViewProjectionMatrix();
    m_sceneData.cameraPosition = camera->GetOwner()->GetTransform()->GetPosition();
    m_cameraSetThisFrame = true;
}

void Renderer::Submit(RenderCommand command)
{
    m_renderQueue.emplace_back(std::move(command));
}

void Renderer::Render()
{
    if(!m_cameraSetThisFrame)
    {
        // TODO: render custom text on the background with a "NO CAMERAS" warning
        SDL_LogWarn(0, "[Renderer] Camera not present!");
        return;
    }

    for(auto command : m_renderQueue)
    {
        MatrixBlock mvp =
        {
            .model = command.modelMatrix,
            .viewProjection = m_sceneData.viewProjectionMatrix
        };

        auto shader = command.material->GetShader();
        shader->Bind();
        shader->SetMVP(mvp);
        // shader->SetUniformFloat(0, (float)SDL_GetTicks());
        shader->SetUniformInt(1, 0);
        // shader->SetUniformInt(2, 1);

        command.material->BindTextures();

        command.mesh->Bind();
        GL_CHECK(glDrawElements(GL_TRIANGLES, command.mesh->GetIndicesCount(), GL_UNSIGNED_INT, 0));
    }

    m_renderQueue.clear();
    m_cameraSetThisFrame = false;
}