#include "SkyBox.h"
#include "Model.h"

SkyBox::SkyBox(std::string filename, VECTOR pos)
	: Object3D(pos)
{
	// 繧ｹ繧ｫ繧､繝懊ャ繧ｯ繧ｹ繝｢繝・Ν縺ｮ驟咲ｽｮ蛻晄悄蛹・
	model_ = new Model(filename, VGet(0.0f, 0.0f, 0.0f));
}

SkyBox::~SkyBox()
{
	if (model_ != nullptr)
	{
		delete model_;
	}
}

void SkyBox::Update()
{
	if (model_ != nullptr)
	{
		model_->Update();
	}
}

void SkyBox::Draw()
{
	if (model_ != nullptr)
	{
		// 遨ｺ繝｢繝・Ν縺悟・貅占ｨ育ｮ励ｄ蜻ｨ蝗ｲ縺ｮ蠖ｱ縺ｮ蠖ｱ髻ｿ繧貞女縺代※證励￥豐医∪なしｈ縺・↓縲√Λ繧､繝・ぅ繝ｳ繧ｰ繧堤┌蜉ｹ蛹悶＠縺ｦ謠冗判縺吶ｋ
		SetUseLighting(FALSE);
		model_->Draw();
		SetUseLighting(TRUE);
	}
}

void SkyBox::SetScale(float scale)
{
	if (model_ != nullptr)
	{
		model_->SetScale(scale);
	}
}

void SkyBox::SetModelTexture(std::string filename, int index)
{
	if (model_ != nullptr)
	{
		model_->SetTexture(filename, index);
	}
}
