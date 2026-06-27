#include"Model.h"
#include"Master.h"

Model::Model(std::string filename, VECTOR initPos, bool isSeparateAnimation)
    : mvPosition(initPos)
    , scale(1.0f)
    , changeTextureHandle(-1)
{
    handle = Master::resourceManager->LoadModel(filename.c_str());
   
}



Model::~Model()
{
    
    if (changeTextureHandle != -1)
    {
        DeleteGraph(changeTextureHandle);
    }
    MV1DeleteModel(handle);
}

void Model::Update()
{
   
    MV1SetPosition(handle, mvPosition);
    MV1SetRotationXYZ(handle, mvRotation);
}

void Model::Draw()
{
    MV1SetUseDrawMulAlphaColor(handle, FALSE);
    MV1DrawModel(handle);
}


void Model::SetScale(VECTOR scale)
{
    MV1SetScale(handle, scale);
}
void Model::SetScale(float scale)
{
    SetScale(VGet(scale, scale, scale));
}   

void Model::SetTexture(std::string filename, int index)
{
    if (changeTextureHandle != -1)
    {
        DeleteGraph(changeTextureHandle);
    }
    changeTextureHandle = Master::resourceManager->LoadGraphics(filename);
    MV1SetTextureGraphHandle(handle, index, changeTextureHandle, FALSE);
}

void Model::SetColor(float r, float g, float b, float a)
{
    int matNum = MV1GetMaterialNum(handle);
    for (int i = 0; i < matNum; ++i)
    {
        MV1SetMaterialDifColor(handle, i, GetColorF(r, g, b, a));
    }
}
