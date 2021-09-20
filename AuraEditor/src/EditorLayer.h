#pragma once

#include "Aura/Core/Layer.h"

class EditorLayer : public Aura::Layer
{
public:
    EditorLayer();

    virtual void OnImGuiRender();
    float DrawTitlebar();
    void HandleManualWindowResize();
private:
};