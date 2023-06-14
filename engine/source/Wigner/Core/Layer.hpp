#pragma once

#include "Wigner/Base/Base.hpp"

namespace Wigner
{
    class Layer
    {
    public:
        virtual ~Layer() {};
        virtual void Update(f64 dt) {};
        virtual void Render() {};
        virtual bool OnEvent(Event &e) { return true; };
        virtual void OnEnable() {};
        virtual void OnDisable() {};
    private:
    };
}