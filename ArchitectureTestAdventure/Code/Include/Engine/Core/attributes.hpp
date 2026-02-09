/*=================================================

    Copyright (C) 2025 Farrakh. All Rights Reserved.
    
    This file is a part of ArchitectureTestAdventure.
    Check README.md for more information.
    
    File : attributes.hpp
    
    Content : useful defines

=================================================*/

#pragma once

#if __cplusplus >= 201703L // C++17
#define ATA_MAYBE_UNUSED [[maybe_unused]]
#else
#define ATA_MAYBE_UNUSED
#endif

#define ATA_NODISCARD [[nodiscard]]

#define ATA_CLASS_NONCOPYABLE(T)     \
    T(const T&)            = delete; \
    T& operator=(const T&) = delete;

#define ATA_CLASS_MOVABLE(T)              \
    T(T&&) noexcept            = default; \
    T& operator=(T&&) noexcept = default;
