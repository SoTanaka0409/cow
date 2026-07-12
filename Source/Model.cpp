#include"Model.h"
#include"Master.h"

Model::Model(std::string filename, VECTOR initPos, bool isSeparateAnimation)
    : position_(initPos)
    , mfScale(1.0f)
    , mnChangeTextureHandle(-1)
{
    mnHandle = Master::mpResourceManager->LoadModel(filename.c_str());
   
}



Model::~Model()
{
    
    if (mnChangeTextureHandle != -1)
    {
        // DeleteGraph(mnChangeTextureHandle); // ResourceManager縺ｧ邂｡逅・＠縺ｦ縺・ｋ縺溘ａ蜑企勁縺励↑縺・
    }
    MV1DeleteModel(mnHandle);
}

void Model::Update()
{
   
    MV1SetPosition(mnHandle, position_);
    MV1SetRotationXYZ(mnHandle, rotation_);
}

void Model::Draw()
{
    MV1SetUseDrawMulAlphaColor(mnHandle, FALSE);
    MV1DrawModel(mnHandle);
}


void Model::SetScale(VECTOR scale)
{
    MV1SetScale(mnHandle, scale);
}
void Model::SetScale(float scale)
{
    SetScale(VGet(scale, scale, scale));
}   

void Model::SetTexture(std::string filename, int index)
{
    if (mnChangeTextureHandle != -1)
    {
        // DeleteGraph(mnChangeTextureHandle); // ResourceManager縺ｧ邂｡逅・＠縺ｦ縺・ｋ縺溘ａ蜑企勁縺励↑縺・
    }
    mnChangeTextureHandle = Master::mpResourceManager->LoadGraphics(filename);
    MV1SetTextureGraphHandle(mnHandle, index, mnChangeTextureHandle, FALSE);
}

void Model::SetColor(float r, float g, float b, float a)
{
    int matNum = MV1GetMaterialNum(mnHandle);
    for (int i = 0; i < matNum; ++i)
    {
        MV1SetMaterialDifColor(mnHandle, i, GetColorF(r, g, b, a));
    }
}