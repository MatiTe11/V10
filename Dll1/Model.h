#pragma once

#include "Graphics.h"
#include "ISimpleShadingObject.h"
#include "ModelInterface.h"

namespace V10
{
    class Camera;
    class DescriptorHeap;
    class Mesh;
    class Texture2D;

    class Model : public ModelInterface, public ISimpleShadingObject
    {
    private:
        Graphics& m_graphics;
        std::vector<Mesh> m_Meshes;
        std::unique_ptr<Texture2D> m_Textures;
        std::unique_ptr<Texture2D> m_NormalTextures;
        std::unique_ptr<DescriptorHeap> m_descHeap;
        DirectX::XMMATRIX m_modelMat;

    public:
        Model(Graphics& graphics, std::string modelPath);
        ~Model();

        void Update(double elapsedSeconds);
        void ResetTransform() override;
        void Move(DirectX::XMVECTOR translation) override;
        void Rotate(DirectX::FXMVECTOR axis, float angle) override;


        void Draw(ID3D12GraphicsCommandList* cl);
        virtual DirectX::XMMATRIX GetModelMatrix() override;
        virtual DirectX::XMFLOAT3X3 GetNormalMatrix() override;
        virtual DescLocation GetTextureDescriptor() override;
    private:
        void ProcessNode(aiNode* node, const aiScene* scene);
    };
}

