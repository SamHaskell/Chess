#pragma once

#include "Wigner/Base/Base.hpp"

namespace Wigner
{
    class Layer
    {
    public:
        virtual ~Layer() {};
        virtual void Update() {};
        virtual void Render() {};
        virtual bool OnEvent(Event &e) { LOG_WARN("AAAAAAAA"); };
        virtual void OnEnable() {};
        virtual void OnDisable() {};
    private:
    };
}