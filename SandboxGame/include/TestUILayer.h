#pragma once

#include <UI/UILayer.h>

#include "TestWidget.h"

using namespace Shak;

class TestUILayer : public UILayer
{
    void Initialize() override
    {
        auto* widget = this->AddWidget<TestWidget>("TestWidget");
    }
};