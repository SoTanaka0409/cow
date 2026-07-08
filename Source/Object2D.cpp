#include "ServiceLocator.h"
#include "Object2D.h"
#include "Master.h"
#include "ObjectManager.h"
#include "Scene.h"

Object2D::Object2D(VECTOR initPos)
	: position_(initPos)
	, rotation_(VGet(0.0f, 0.0f, 0.0f))
	, delete_flag_(false)
	, tag_(Tag2D::kNone2d)
	, draw_flag_(true)
{
	// 逕滓・縺励◆繧ｪ繝悶ず繧ｧ繧ｯ繝医ｒ邂｡逅・・譖ｴ譁ｰ縺吶ｋ縺溘ａ縲∫樟蝨ｨ縺ｮ繧｢繧ｯ繝・ぅ繝悶↑繧ｷ繝ｼ繝ｳ縺ｮ繝槭ロ繝ｼ繧ｸ繝｣繝ｼ縺ｸ閾ｪ蜍輔〒逋ｻ骭ｲ縺吶ｋ
	ServiceLocator::GetObjectManager()->AddObject(this);
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