/*=================================================

    Copyright (C) 2025 Farrakh. All Rights Reserved.
    
    This file is a part of ArchitectureTestAdventure.
    Check README.md for more information.
    
    File : ResourceManager.cpp
    
    Content : GPU Resource Manager

=================================================*/

#include "RHI/ResourceManager.hpp"

void rhi::ResourceManager::Release() {
    for (auto& texture : m_Textures)
        m_Device.DestroyBackendTexture(texture.backend_handle);
    m_Textures.clear();
}

RHI_NODISCARD rhi::TextureHandle rhi::ResourceManager::CreateTexture(const rhi::TextureDesc& desc) {
    Texture texture{};
    texture.width  = desc.width;
    texture.height = desc.height;
    texture.format = desc.format;

    texture.backend_handle = m_Device.CreateBackendTexture(desc);

    m_Textures.push_back(texture);
    return TextureHandle(m_Textures.size() - 1);
}

void rhi::ResourceManager::DestroyTexture(TextureHandle handle) {
    size_t index = static_cast<size_t>(handle);
    if (index >= m_Textures.size()) {
        // TODO : ERROR LOGGING
        return;
    }

    auto& texture = this->getTexture(handle);
    m_Device.DestroyBackendTexture(texture.backend_handle);

    m_Textures.erase(m_Textures.begin() + index);
}
