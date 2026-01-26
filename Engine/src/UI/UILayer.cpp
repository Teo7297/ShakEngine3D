#include "UI/UILayer.h"
#include "Renderer.h"

using namespace Shak;

UILayer::UILayer()
    : m_widgets{}
{

}

UILayer::~UILayer()
{

}

void UILayer::Initialize()
{

}

void UILayer::OnActivate()
{

}

void UILayer::OnDeactivate()
{

}

void UILayer::Update(float dt)
{
    for(const auto& [name, widget] : m_widgets)
        widget->Update(dt);
}

void UILayer::Render(Renderer* renderer)
{
    for(const auto& [name, widget] : m_widgets)
        widget->Draw(renderer);
}

void UILayer::SetName(const std::string& name)
{
    m_name = name;
}