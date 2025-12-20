/*=================================================

    Copyright (C) 2025 Farrakh.  All Rights Reserved.
    
    This source is a part of ArchitectureTestAdventure.
    Check README.md for more information.
    
    File : resource.h
    
    Content : Some useful classes to work with RHI

=================================================*/

#pragma once
#include <iostream>
#include <wrl/client.h>

namespace ata {
    namespace rhi {
        using ObjectType = uint32_t;

        // object_types namespace contains identifiers for various object types.
        // All constants have to be distinct. This list can be extended.
        //
        // The encoding is chosen to minimize potential conflicts between implementations.
        // 0x00aabbcc, where:
        //   aa is Graphics API, 1 for Vulkan, 2 for OpenGL
        //   bb is layer, 0 for native Graphics API objects, 1 for reference of backend, 2 for user-defined backends
        //   cc is a sequential number

        namespace object_types {
            constexpr ObjectType SharedHandle = 0x00000001;



        }

    } // namespace rhi

} // namespace ata
