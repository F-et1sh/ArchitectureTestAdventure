/*=================================================

    Copyright (C) 2025 Farrakh.  All Rights Reserved.
    
    This source is a part of ArchitectureTestAdventure.
    Check README.md for more information.
    
    File : Renderer.hpp
    
    Content : Universal Renderer API

=================================================*/

#pragma once
#include "Renderer/ResourceManager/ResourceManager.hpp"

namespace ata {
    using namespace ata::render;

    class Renderer {
    public:
        Renderer()  = default;
        ~Renderer() = default;

        void Release();
        void Initialize();

    private:
        ResourceManager m_resourceManager;
    };
} // namespace ata
