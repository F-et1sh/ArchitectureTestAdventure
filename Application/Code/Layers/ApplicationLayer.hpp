/*=================================================

    Copyright (C) 2025 Farrakh. All Rights Reserved.
    
    This file is a part of ArchitectureTestAdventure.
    Check README.md for more information.
    
    File : ApplicationLayer.hpp
    
    Content : Application layer

=================================================*/

#pragma once

#include "Engine/Layer.hpp"

class ApplicationLayer : public ata::Layer {
public:
    ApplicationLayer()  = default;
    ~ApplicationLayer() = default;

    void OnAttach()override {}
    void OnDetach()override {}

    void OnUpdate(float delta_time)override {}
    void OnRender()override {}
};
