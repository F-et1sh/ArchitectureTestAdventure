/*=================================================

    Copyright (C) 2025 Farrakh. All Rights Reserved.
    
    This file is a part of ArchitectureTestAdventure.
    Check README.md for more information.
    
    File : ResourceManager.hpp
    
    Content : GPU Resource Manager

=================================================*/

#pragma once

#include "Common/Attributes.hpp"
#include "Common/Resource.hpp"

#include <vector>

namespace rhi {
    class ResourceManager {
    public:
        ResourceManager()  = default;
        ~ResourceManager() { this->Release(); }

        void Release();

        TextureHandle                 CreateTexture(const TextureDesc& desc);
        inline RHI_NODISCARD Texture& getTexture(TextureHandle handle) { return m_Textures[handle]; }

    private:
        std::vector<Texture> m_Textures;
    };
} // namespace rhi
