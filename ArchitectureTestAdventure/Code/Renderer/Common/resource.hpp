/*=================================================

    Copyright (C) 2025 Farrakh.  All Rights Reserved.
    
    This source is a part of ArchitectureTestAdventure.
    Check README.md for more information.
    
    File : resource.h
    
    Content : Some useful classes to work with RHI. Mostly a copy of NVRHI ( https://github.com/NVIDIA-RTX/NVRHI )

=================================================*/

#pragma once
#include <iostream>
#include <wrl/client.h>
#include "attributes.hpp"

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

        enum class GraphicsAPI : uint8_t {
            None   = 0,
            Vulkan = 1,
            OpenGL = 2
        };

        enum class Layer : uint8_t {
            Native    = 0,
            Reference = 1,
            User      = 2
        };

        constexpr ObjectType make_object_type(
            GraphicsAPI api,
            Layer       layer,
            uint16_t    id) {

            return (uint32_t(api) << 16) |
                   (uint32_t(layer) << 8) |
                   id;
        }

        namespace object_types {
            constexpr ObjectType SharedHandle = make_object_type(GraphicsAPI::None, Layer::Native, 0);

            constexpr ObjectType VK_Device                   = make_object_type(GraphicsAPI::Vulkan, Layer::Native, 1);
            constexpr ObjectType VK_PhysicalDevice           = make_object_type(GraphicsAPI::Vulkan, Layer::Native, 2);
            constexpr ObjectType VK_Instance                 = make_object_type(GraphicsAPI::Vulkan, Layer::Native, 3);
            constexpr ObjectType VK_Queue                    = make_object_type(GraphicsAPI::Vulkan, Layer::Native, 4);
            constexpr ObjectType VK_CommandBuffer            = make_object_type(GraphicsAPI::Vulkan, Layer::Native, 5);
            constexpr ObjectType VK_DeviceMemory             = make_object_type(GraphicsAPI::Vulkan, Layer::Native, 6);
            constexpr ObjectType VK_Buffer                   = make_object_type(GraphicsAPI::Vulkan, Layer::Native, 7);
            constexpr ObjectType VK_Image                    = make_object_type(GraphicsAPI::Vulkan, Layer::Native, 8);
            constexpr ObjectType VK_ImageView                = make_object_type(GraphicsAPI::Vulkan, Layer::Native, 9);
            constexpr ObjectType VK_AccelerationStructureKHR = make_object_type(GraphicsAPI::Vulkan, Layer::Native, 10);
            constexpr ObjectType VK_Sampler                  = make_object_type(GraphicsAPI::Vulkan, Layer::Native, 11);
            constexpr ObjectType VK_ShaderModule             = make_object_type(GraphicsAPI::Vulkan, Layer::Native, 12);
            constexpr ObjectType VK_DescriptorPool           = make_object_type(GraphicsAPI::Vulkan, Layer::Native, 13);
            constexpr ObjectType VK_DescriptorSetLayout      = make_object_type(GraphicsAPI::Vulkan, Layer::Native, 14);
            constexpr ObjectType VK_DescriptorSet            = make_object_type(GraphicsAPI::Vulkan, Layer::Native, 15);
            constexpr ObjectType VK_PipelineLayout           = make_object_type(GraphicsAPI::Vulkan, Layer::Native, 16);
            constexpr ObjectType VK_Pipeline                 = make_object_type(GraphicsAPI::Vulkan, Layer::Native, 17);
            constexpr ObjectType VK_Micromap                 = make_object_type(GraphicsAPI::Vulkan, Layer::Native, 18);
            constexpr ObjectType VK_ImageCreateInfo          = make_object_type(GraphicsAPI::Vulkan, Layer::Native, 19);

            constexpr ObjectType GL_Context     = make_object_type(GraphicsAPI::OpenGL, Layer::Native, 1);
            constexpr ObjectType GL_Buffer      = make_object_type(GraphicsAPI::OpenGL, Layer::Native, 2);
            constexpr ObjectType GL_VertexArray = make_object_type(GraphicsAPI::OpenGL, Layer::Native, 3);
            constexpr ObjectType GL_Texture     = make_object_type(GraphicsAPI::OpenGL, Layer::Native, 4);
            constexpr ObjectType GL_Sampler     = make_object_type(GraphicsAPI::OpenGL, Layer::Native, 5);
            constexpr ObjectType GL_Shader      = make_object_type(GraphicsAPI::OpenGL, Layer::Native, 6);
            constexpr ObjectType GL_Program     = make_object_type(GraphicsAPI::OpenGL, Layer::Native, 7);
            constexpr ObjectType GL_Framebuffer = make_object_type(GraphicsAPI::OpenGL, Layer::Native, 8);

        } // namespace object_types

        struct Object {
            uint64_t value = 0;

            constexpr Object() = default;
            constexpr Object(uint64_t value) : value(value) {}
            constexpr Object(void* pointer) : value(reinterpret_cast<uint64_t>(pointer)) {}

            template <typename T>
            T* as() const { return reinterpret_cast<T*>(value); }
        };

        class IResource {
        protected:
            IResource()          = default;
            virtual ~IResource() = default;

        public:
            virtual unsigned long AddReference()      = 0;
            virtual unsigned long Release()           = 0;
            virtual unsigned long GetReferenceCount() = 0;

            // returns a native object or interface or null Object if the requested interface is unavailable
            // does *not* add_reference() the returned interface
            virtual Object GetNativeObject(ATA_MAYBE_UNUSED ObjectType object_type) {
                return {};
            }

            // non-copyable and non-movable
            IResource(const IResource&)             = delete;
            IResource(const IResource&&)            = delete;
            IResource& operator=(const IResource&)  = delete;
            IResource& operator=(const IResource&&) = delete;
        };

        // Mostly a copy of Microsoft::WRL::ComPtr<T>
        template <typename T>
        class RefCountPtr {
        public:
            RefCountPtr() noexcept : m_pointer(nullptr) {}

            RefCountPtr(std::nullptr_t) noexcept : m_pointer(nullptr) {}

            template <class U>
            RefCountPtr(U* other) noexcept : m_pointer(other) {
                InternalAddReference();
            }

            RefCountPtr(const RefCountPtr& other) noexcept : m_pointer(other.m_pointer) {
                InternalAddReference();
            }

            // copy constructor that allows to instanatiate class when U* is convertible to T*
            template <class U>
                requires std::is_convertible_v<U*, T*>
            RefCountPtr(const RefCountPtr<U>& other) noexcept : m_pointer(other.m_pointer) {
                InternalAddReference();
            }

            RefCountPtr(RefCountPtr&& other) noexcept : m_pointer(nullptr) {
                if (this != reinterpret_cast<RefCountPtr*>(&reinterpret_cast<unsigned char&>(other))) {
                    Swap(other);
                }
            }

            // move constructor that allows instantiation of a class when U* is convertible to T*
            template <class U>
                requires std::is_convertible_v<U*, T*>
            RefCountPtr(RefCountPtr<U>&& other) noexcept : m_pointer(other.m_pointer) {
                other.m_pointer = nullptr;
            }

            ~RefCountPtr() noexcept {
                InternalRelease();
            }

            RefCountPtr& operator=(std::nullptr_t) noexcept {
                InternalRelease();
                return *this;
            }

            RefCountPtr& operator=(T* other) noexcept {
                if (m_pointer != other) {
                    RefCountPtr(other).Swap(*this);
                }
                return *this;
            }

            template <typename U>
            RefCountPtr& operator=(U* other) noexcept {
                RefCountPtr(other).Swap(*this);
                return *this;
            }

            RefCountPtr& operator=(const RefCountPtr& other) noexcept {
                if (m_pointer != other.m_pointer) {
                    RefCountPtr(other).Swap(*this);
                }
                return *this;
            }

            template <class U>
            RefCountPtr& operator=(const RefCountPtr<U>& other) noexcept {
                RefCountPtr(other).Swap(*this);
                return *this;
            }

            RefCountPtr& operator=(RefCountPtr&& other) noexcept {
                RefCountPtr(static_cast<RefCountPtr&&>(other)).Swap(*this);
                return *this;
            }

            template <class U>
            RefCountPtr& operator=(RefCountPtr<U>&& other) noexcept {
                RefCountPtr(static_cast<RefCountPtr<U>&&>(other)).Swap(*this);
                return *this;
            }

            void Swap(RefCountPtr&& r) noexcept {
                T* tmp      = m_pointer;
                m_pointer   = r.m_pointer;
                r.m_pointer = tmp;
            }

            void Swap(RefCountPtr& r) noexcept {
                T* tmp      = m_pointer;
                m_pointer   = r.m_pointer;
                r.m_pointer = tmp;
            }

            ATA_NODISCARD T* Get() const noexcept {
                return m_pointer;
            }

            operator T*() const {
                return m_pointer;
            }

            T* operator->() const noexcept {
                return m_pointer;
            }

            T** operator&() {
                return &m_pointer;
            }

            ATA_NODISCARD T* const* GetAddressOf() const noexcept {
                return &m_pointer;
            }

            ATA_NODISCARD T** GetAddressOf() noexcept {
                return &m_pointer;
            }

            ATA_NODISCARD T** ReleaseAndGetAddressOf() noexcept {
                InternalRelease();
                return &m_pointer;
            }

            T* Detach() noexcept {
                T* pointer = m_pointer;
                m_pointer  = nullptr;
                return pointer;
            }

            // set the pointer while keeping the object's reference count unchanged
            void Attach(T* other) {
                if (m_pointer != nullptr) {
                    auto reference = m_pointer->Release();

                    // attaching to the same object only works if duplicate references are being coalesced. Otherwise
                    // re-attaching will cause the pointer to be released and may cause a crash on a subsequent dereference.
                    assert(reference != 0 || m_pointer != other);
                }

                m_pointer = other;
            }

            // create a wrapper around a raw object while keeping the object's reference count unchanged
            static RefCountPtr<T> Create(T* other) {
                RefCountPtr<T> Ptr;
                Ptr.Attach(other);
                return Ptr;
            }

            unsigned long Reset() {
                return InternalRelease();
            }

        protected:
            void InternalAddReference() const noexcept {
                if (m_pointer != nullptr) {
                    m_pointer->AddReference();
                }
            }

            unsigned long InternalRelease() noexcept {
                unsigned long reference = 0;
                T*            temp      = m_pointer;

                if (temp != nullptr) {
                    m_pointer = nullptr;
                    reference = temp->Release();
                }

                return reference;
            }

        protected:
            T* m_pointer = nullptr;

            template <class U>
            friend class RefCountPtr;
        };

        // A class that implements reference counting in a way compatible with RefCountPtr.
        // Intended usage is to use it as a base class for interface implementations, like so :
        // class Texture : public RefCounter<ITexture> { ... }
        template <class T>
        class RefCounter : public T {
        protected:
            RefCounter()          = default;
            virtual ~RefCounter() = default;

        public:
            unsigned long AddReference() final override {
                return m_referenceCount.fetch_add(1, std::memory_order_relaxed) + 1;
            }

            unsigned long Release() final override {
                unsigned long result = m_referenceCount.fetch_sub(1, std::memory_order_acq_rel) - 1;
                if (result == 0) {
                    delete this;
                }
                return result;
            }

            unsigned long GetReferenceCount() const final override {
                return m_referenceCount.load(std::memory_order_relaxed);
            }

        private:
            std::atomic<unsigned long> m_referenceCount{ 1 };
        };

        using ResourceHandle = RefCountPtr<IResource>;

    } // namespace rhi

} // namespace ata
