#pragma once
#include <DirectXMath.h>


namespace V10
{

    class ModelInterface
    {

    public:

        virtual void ResetTransform() = 0;
        virtual void Move(DirectX::XMVECTOR translation) = 0;
        virtual void Rotate(DirectX::FXMVECTOR axis, float angle) = 0;

    };
}

