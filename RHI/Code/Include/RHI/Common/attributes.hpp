/*=================================================

    Copyright (C) 2025 Farrakh. All Rights Reserved.
    
    This file is a part of ArchitectureTestAdventure.
    Check README.md for more information.
    
    File : attributes.hpp
    
    Content : useful defines

=================================================*/

#pragma once

#if __cplusplus >= 201703L // C++17
#define RHI_MAYBE_UNUSED [[maybe_unused]]
#else
#define RHI_MAYBE_UNUSED
#endif

#define RHI_NODISCARD [[nodiscard]]
