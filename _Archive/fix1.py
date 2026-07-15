import os

mountain_h = """\xef\xbb\xbf#pragma once
#include "Model.h"
#include "Dxlib.h"
#include "Object3D.h"

class Mountain : public Object3D
{
public:
\t/*
\t * 背景モデルの初期化
\t * [入力] filename: モデルのパス, initPos: 初期座標, scale: 拡大率, angle: 回転角度
\t * [出力] なし
\t * [副作用] Modelインスタンスを確保
\t */
\tMountain(std::string filename, VECTOR initPos, VECTOR scale, VECTOR angle);
\tvirtual ~Mountain();

\tvoid Update() override;
\tvoid Draw() override;

\tvoid SetScale(VECTOR scale);
\tvoid SetRotation(VECTOR angle);
\tvoid SetColor(float r, float g, float b, float a = 1.0f);

private:
\tModel* model_;
};
"""

mountain_cpp = """\xef\xbb\xbf#include "Mountain.h"

Mountain::Mountain(std::string filename, VECTOR initPos, VECTOR scale, VECTOR angle)
\t: Object3D(initPos)
{
\tmodel_ = new Model(filename, initPos);
\tmodel_->SetScale(scale);
\tmodel_->SetRotation(angle);
}

Mountain::~Mountain()
{
\t// メモリリーク防止のため破棄
\tif (model_ != nullptr)
\t{
\t\tdelete model_;
\t\tmodel_ = nullptr;
\t}
}

void Mountain::Update()
{
\tif (model_ != nullptr)
\t{
\t\tmodel_->Update();
\t}
}

void Mountain::Draw()
{
\tif (model_ != nullptr)
\t{
\t\tmodel_->Draw();
\t}
}

void Mountain::SetScale(VECTOR scale)
{
\tif (model_ != nullptr)
\t{
\t\tmodel_->SetScale(scale);
\t}
}

void Mountain::SetRotation(VECTOR angle)
{
\tif (model_ != nullptr)
\t{
\t\tmodel_->SetRotation(angle);
\t}
}

void Mountain::SetColor(float r, float g, float b, float a)
{
\tif (model_ != nullptr)
\t{
\t\tmodel_->SetColor(r, g, b, a);
\t}
}
"""

object2d_h = """\xef\xbb\xbf#pragma once
#include <string>
#include <vector>
#include "Dxlib.h"

class Object2D
{
public:
\tenum Tag2D
\t{
\t\tkNone2d = 0,
\t\tkTag2dDino1 = 1,
\t\tkTag2dDino2 = 100,
\t};

public:
\t/*
\t * 2Dオブジェクトの生成とマネージャーへの登録
\t * [入力] initPos: 初期座標
\t * [出力] なし
\t * [副作用] シーンのObjectManagerに自身を登録
\t */
\tObject2D(VECTOR initPos);
\tvirtual ~Object2D();

\tvirtual void Update();
\tvirtual void Draw();

\tvoid SetPosition(VECTOR pos) { position_ = pos; };
\tVECTOR GetPosition() { return position_; }

\tvoid SetRotation(VECTOR rot) { rotation_ = rot; }
\tVECTOR GETRotation() { return rotation_; }

\tvoid SetDeleteFlag(bool flag) { delete_flag_ = flag; }
\tbool IsDeleteFlag() { return delete_flag_; }

\tvoid SetDrawFlag(bool flag) { draw_flag_ = flag; }
\tbool IsDrawFlag() { return draw_flag_; }

\tvoid SetTag(Tag2D tag) { tag_ = tag; }
\tTag2D GetTag() { return tag_; }

protected:
\tVECTOR position_;
\tVECTOR rotation_;

private:
\tbool delete_flag_;
\tTag2D tag_;
\tbool draw_flag_;
};
"""

object2d_cpp = """\xef\xbb\xbf#include "ServiceLocator.h"
#include "Object2D.h"
#include "Master.h"
#include "ObjectManager.h"
#include "Scene.h"

Object2D::Object2D(VECTOR initPos)
\t: position_(initPos)
\t, rotation_(VGet(0.0f, 0.0f, 0.0f))
\t, delete_flag_(false)
\t, tag_(Tag2D::kNone2d)
\t, draw_flag_(true)
{
\t// マネージャーによる更新・描画の自動管理のため
\tServiceLocator::GetObjectManager()->AddObject(this);
}

Object2D::~Object2D()
{
}

void Object2D::Draw()
{
}

void Object2D::Update()
{
}
"""

with open("Source/Mountain.h", "wb") as f: f.write(mountain_h.encode("utf-8"))
with open("Source/Mountain.cpp", "wb") as f: f.write(mountain_cpp.encode("utf-8"))
with open("Source/Object2D.h", "wb") as f: f.write(object2d_h.encode("utf-8"))
with open("Source/Object2D.cpp", "wb") as f: f.write(object2d_cpp.encode("utf-8"))
