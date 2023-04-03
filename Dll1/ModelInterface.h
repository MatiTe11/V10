#pragma once
#include <DirectXMath.h>


namespace V10
{

    class ModelInterface
    {

    public:

        virtual void Move(DirectX::XMVECTOR translation) = 0;

    };
}

