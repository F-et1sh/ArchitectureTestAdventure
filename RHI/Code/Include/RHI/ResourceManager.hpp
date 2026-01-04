/*=================================================

    Copyright (C) 2025 Farrakh. All Rights Reserved.
    
    This file is a part of ArchitectureTestAdventure.
    Check README.md for more information.
    
    File : ResourceManager.hpp
    
    Content : GPU Resource Manager

=================================================*/

#pragma once

#include <vector>

namespace rhi {
    class ResourceManager {
    public:
        ResourceManager() = default;
        ~ResourceManager() = default;

    private:
        std::vector<Texture> m_Textures;
    };
}