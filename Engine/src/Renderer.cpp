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
    if(!camera)
        return;

    // get camera data and set UBO for vp matrix
    m_sceneData.viewProjectionMatrix = camera->GetViewProjectionMatrix();
    m_sceneData.cameraPosition = camera->GetOwner()->GetTransform()->GetPosition();
    m_cameraSetThisFrame = true;
    m_camera = camera;
}

void Renderer::Submit(RenderCommand command)
{
    m_renderQueue.emplace_back(std::move(command));
}

void Renderer::RenderScene()
{
    GL_CHECK(glEnable(GL_DEPTH_TEST));
    GL_CHECK(glEnable(GL_CULL_FACE));
    GL_CHECK(glCullFace(GL_BACK));
    GL_CHECK(glClearColor(0.0, 0.3, 0.5, 1.0));
    GL_CHECK(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));

    GL_CHECK(glViewport(0, 0, (int)ImGui::GetIO().DisplaySize.x, (int)ImGui::GetIO().DisplaySize.y));

    if(!m_cameraSetThisFrame)
    {
        // TODO: render custom text on the background with a "NO CAMERAS" warning
        SDL_LogWarn(0, "[Renderer] Camera not present!");
        return;
    }

    MatrixBlock mvp =
    {
        .viewProjection = m_sceneData.viewProjectionMatrix
    };

    RenderCommand skyboxRC;
    bool skyboxSet = false;
    Shader* lastBoundShader = nullptr;

    std::sort(m_renderQueue.begin(), m_renderQueue.end());

    for(auto command : m_renderQueue)
    {
        if(command.isSkybox)
        {
            skyboxRC = command; // we deal with this as last
            skyboxSet = true;
            continue;
        }

        mvp.model = command.modelMatrix;

        auto shader = command.material->GetShader();

        // Avoid binding the same shader multiple times, maybe pass generic shared uniforms here as well
        // uWorld uView uProjection uNormalMatrix uResolution uTexelSize uAspectRatio  uCameraPosition  uCameraDirection uNearClip uFarClip uMouse uDeltaTime + light data
        if(shader != lastBoundShader)
        {
            shader->Bind();
            shader->SetUniformInt(shader->GetUniformLocation("uTime"), SDL_GetTicks());
        }
        lastBoundShader = shader;

        shader->SetMVP(mvp);
        command.material->SetTextureUniforms();
        command.material->SetInstanceSpecificUniforms();

        command.material->BindTextures();
        command.mesh->Bind();

        GL_CHECK(glDrawElements(command.renderMode, command.mesh->GetIndicesCount(), GL_UNSIGNED_INT, 0));
    }

    // Render Skybox last to save some fragment execution
    if(skyboxSet)
    {
        auto v = m_camera->GetViewMatrix();
        v = glm::mat4(glm::mat3(v));
        mvp.viewProjection = m_camera->GetProjectionMatrix() * v;

        //? change depth function so depth test passes when values are equal to depth buffer's content
        GL_CHECK(glDepthFunc(GL_LEQUAL));
        GL_CHECK(glCullFace(GL_FRONT));

        auto shader = skyboxRC.material->GetShader();
        shader->Bind();
        shader->SetMVP(mvp);
        shader->SetUniformInt(shader->GetUniformLocation("uTexture"), 0);

        skyboxRC.material->BindTextures();

        skyboxRC.mesh->Bind();
        GL_CHECK(glDrawElements(GL_TRIANGLES, skyboxRC.mesh->GetIndicesCount(), GL_UNSIGNED_INT, 0));

        GL_CHECK(glDepthFunc(GL_LESS));
        GL_CHECK(glCullFace(GL_BACK));
    }

    m_renderQueue.clear();
    m_cameraSetThisFrame = false;
}

void Renderer::RenderUI()
{
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}
