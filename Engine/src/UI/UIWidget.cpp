#include "UI/UIWidget.h"
#include "Renderer.h"

using namespace Shak;

UIWidget::UIWidget()
{

}

UIWidget::~UIWidget()
{

}

void UIWidget::Initialize()
{

}

void UIWidget::Update(float dt)
{

}

void UIWidget::SetActive(bool flag)
{
    m_active = flag;
    flag ? this->OnActivate() : this->OnDeactivate();
}

bool UIWidget::IsActive()
{
    return m_active;
}

void UIWidget::SetName(const std::string& name)
{
    m_name = name;
}

std::string UIWidget::GetName()
{
    return m_name;
}

void UIWidget::Draw(Renderer* renderer)
{
    renderer->Submit({
        .mesh = m_mesh,
        .material = m_material,
        .screenCoords = m_screenCoords,
        .renderMode = GL_TRIANGLES
        });
}

void UIWidget::OnActivate()
{

}

void UIWidget::OnDeactivate()
{

}
