/*=================================================

    Copyright (C) 2025 Farrakh. All Rights Reserved.
    
    This file is a part of ArchitectureTestAdventure.
    Check README.md for more information.
    
    File : Layer.hpp
    
    Content : Application layers

=================================================*/

#pragma once

namespace ata {
    class Layer {
    public:
        Layer()          = default;
        virtual ~Layer() = default;

        //virtual void OnEvent(Event& event) {}

        virtual void OnAttach() {}
        virtual void OnDetach() {}

        virtual void OnUpdate(float delta_time) {}
        virtual void OnRender() {}

        //template<std::derived_from<Layer> T, typename... Args>
        //void TransitionTo(Args&&... args) {
        //	QueueTransition(std::move(std::make_unique<T>(std::forward<Args>(args)...)));
        //}
    private:
        //void QueueTransition(std::unique_ptr<Layer> layer);
    };
} // namespace ata
