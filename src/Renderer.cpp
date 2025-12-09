#include "Renderer.h"
#include "Mesh.h"
#include "components/CameraComponent.h"

using namespace Shak;

Renderer::Renderer()
    : m_renderQueue{}
{
}

Renderer::~Renderer()
{

}

void Renderer::Setup(CameraComponent* camera)
{
    // get camera data and set UBO for vp matrix
    m_sceneData.viewProjectionMatrix = camera->GetViewProjectionMatrix();
    m_sceneData.cameraPosition = camera->GetOwner()->GetTransform()->GetPosition();
}

void Renderer::Submit(RenderCommand command)
{
    m_renderQueue.emplace_back(std::move(command));
}

void Renderer::Render()
{
    auto V = glm::identity<glm::mat4>();
    V = glm::translate(V, glm::vec3(0.f, 0.f, -6.f));

    auto P = glm::perspective(glm::radians(45.0f), 800.f / 600.f, 0.1f, 100.0f);

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
        shader->SetUniformFloat(0, (float)SDL_GetTicks());
        shader->SetUniformInt(1, 0);
        // shader->SetUniformInt(2, 1);

        command.material->BindTextures();

        command.mesh->Bind();
        GL_CHECK(glDrawElements(GL_TRIANGLES, command.mesh->GetIndicesCount(), GL_UNSIGNED_INT, 0));
    }

    m_renderQueue.clear();
}