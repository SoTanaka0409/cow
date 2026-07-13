import os

object_stage_h = """\xef\xbb\xbf#pragma once
#include "Model.h"
#include "Dxlib.h"
#include "Object3D.h"

class Object_Stage : public Object3D
{
public:
\t/*
\t * ステージ背景用オブジェクトの初期化
\t * [入力] filename: モデルのパス, initPos: 初期座標, scale: 拡大率, angle: 回転角度
\t * [出力] なし
\t * [副作用] Modelインスタンスを確保
\t */
\tObject_Stage(std::string filename, VECTOR initPos, float scale, VECTOR angle);
\tvirtual ~Object_Stage();

\tvoid Update() override;
\tvoid Draw() override;

\tvoid SetScale(float scale);
\tvoid SetRotation(VECTOR angle);
\tvoid SetColor(float r, float g, float b, float a = 1.0f);

private:
\tModel* model_;
};
"""

object_stage_cpp = """\xef\xbb\xbf#include "Object_Stage.h"

Object_Stage::Object_Stage(std::string filename, VECTOR initPos, float scale, VECTOR angle)
\t: Object3D(initPos)
{
\tmodel_ = new Model(filename, initPos);
\tmodel_->SetScale(scale);
\tmodel_->SetRotation(angle);
}

Object_Stage::~Object_Stage()
{
\t// メモリリーク防止のため破棄
\tif (model_ != nullptr)
\t{
\t\tdelete model_;
\t\tmodel_ = nullptr;
\t}
}

void Object_Stage::Update()
{
\tif (model_ != nullptr)
\t{
\t\tmodel_->Update();
\t}
}

void Object_Stage::Draw()
{
\tif (model_ != nullptr)
\t{
\t\tmodel_->Draw();
\t}
}

void Object_Stage::SetScale(float scale)
{
\tif (model_ != nullptr)
\t{
\t\tmodel_->SetScale(scale);
\t}
}

void Object_Stage::SetRotation(VECTOR angle)
{
\tif (model_ != nullptr)
\t{
\t\tmodel_->SetRotation(angle);
\t}
}

void Object_Stage::SetColor(float r, float g, float b, float a)
{
\tif (model_ != nullptr)
\t{
\t\tmodel_->SetColor(r, g, b, a);
\t}
}
"""

with open("Source/Object_Stage.h", "wb") as f: f.write(object_stage_h.encode("utf-8"))
with open("Source/Object_Stage.cpp", "wb") as f: f.write(object_stage_cpp.encode("utf-8"))
