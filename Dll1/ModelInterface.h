#pragma once
#include <DirectXMath.h>


namespace V10
{
    struct Material
    {
        float ambient;
        float diffuse;
        float specular;
    };

    class ModelInterface
    {

    public:

        virtual void ResetTransform() = 0;
        virtual void Move(DirectX::XMVECTOR translation) = 0;
        virtual void Rotate(DirectX::FXMVECTOR axis, float angle) = 0;
        virtual void Scale(float scale) = 0;

    };
}

