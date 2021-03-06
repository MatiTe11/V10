#pragma once

#include "Graphics.h"
#include "ISimpleShadingObject.h"

namespace V10
{
    class Camera;
    class DescriptorHeap;
    class Mesh;
    class Texture2D;

    class Model : public ISimpleShadingObject
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
        void Move(DirectX::XMVECTOR translation);

        void Draw(ID3D12GraphicsCommandList* cl);
        virtual DirectX::XMMATRIX GetModelMatrix() override;
        virtual DescLocation GetTextureDescriptor() override;
    private:
        void ProcessNode(aiNode* node, const aiScene* scene);
    };
}

