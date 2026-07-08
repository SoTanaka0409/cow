#include "Texture.h"
#include "DxLib.h"
#include "Master.h"

Texture::Texture(std::string filename, VECTOR centerPosition, int graphsize_x, int graphsize_y, int transFlag)
	: mnHandle(-1)
	, position_(centerPosition)
	, mNewGameW(graphsize_x)
	, mNewGameH(graphsize_y)
	, mnTransFlag(transFlag)
{
	mnHandle = Master::mpResourceManager->LoadGraphics(filename.c_str());
	GetGraphSize(mnHandle, &mnSizeX, &mnSizeY); // 諡｡螟ｧ邵ｮ蟆乗緒逕ｻ縺ｮ蝓ｺ貅悶↓縺吶ｋ縺溘ａ繧ｪ繝ｪ繧ｸ繝翫Ν繧ｵ繧､繧ｺ繧貞叙蠕・
}

Texture::~Texture()
{

}

/*
	* @brief 險ｭ螳壹＆繧後◆謖・ｮ壹し繧､繧ｺ(mNewGameW, mNewGameH)縺ｧ縲［vPosition繧剃ｸｭ蠢・↓蠑輔″莨ｸ縺ｰ縺玲緒逕ｻ縺吶ｋ
	* [蜈･蜉嫋 縺ｪ縺・
	* [蜃ｺ蜉嫋 縺ｪ縺・
	* [蜑ｯ菴懃畑] 謖・ｮ夂ｯ・峇縺ｸ繝・け繧ｹ繝√Ε謠冗判
	*/
void Texture::Draw()
{
	DrawExtendGraph(
		static_cast<int>(position_.x - (mNewGameW / 2)),
		static_cast<int>(position_.y - (mNewGameH / 2)),
		static_cast<int>(position_.x + (mNewGameW / 2)),
		static_cast<int>(position_.y + (mNewGameH / 2)),
		mnHandle, mnTransFlag
	);
}

/*
	* @brief 繝懊ち繝ｳ繝帙ヰ繝ｼ貍泌・逕ｨ縺ｪ縺ｩ縺ｫ縲・壼ｸｸ繧ｵ繧､繧ｺ縺ｫ迚ｹ螳壹・諡｡蠑ｵ蟷・expand)繧貞刈縺医※諡｡螟ｧ謠冗判縺吶ｋ・育樟蝨ｨ譛ｪ菴ｿ逕ｨ・・
	* [蜈･蜉嫋 縺ｪ縺・
	* [蜃ｺ蜉嫋 縺ｪ縺・
	* [蜑ｯ菴懃畑] 謖・ｮ壹＆繧後◆諡｡蠑ｵ鬆伜沺縺ｸ諡｡螟ｧ謠冗判
	*/
void Texture::SizeDraw()
{
	int expand = 15; // 諡｡螟ｧ陦ｨ遉ｺ縺吶ｋ髫帙・諡｡蠑ｵ繝斐け繧ｻ繝ｫ謨ｰ
	int halfW = (mNewGameW + expand) / 2;
	int halfH = (mNewGameH + expand) / 2;

	DrawExtendGraph(
		static_cast<int>(position_.x - halfW),
		static_cast<int>(position_.y - halfH),
		static_cast<int>(position_.x + halfW),
		static_cast<int>(position_.y + halfH),
		mnHandle, mnTransFlag
	);
}

void Texture::Update()
{
}

/*
	* @brief 荳ｭ蠢・ｺｧ讓吶ｒ霆ｸ縺ｫ縺励※縲∵ｯ皮紫謖・ｮ・scale)縺ｫ繧医ｋ諡｡螟ｧ邵ｮ蟆乗緒逕ｻ繧定｡後≧
	* [蜈･蜉嫋 scale: 繧ｹ繧ｱ繝ｼ繝ｪ繝ｳ繧ｰ蛟咲紫・・.0f縺檎ｭ牙搾ｼ・
	* [蜃ｺ蜉嫋 縺ｪ縺・
	* [蜑ｯ菴懃畑] 謖・ｮ壹し繧､繧ｺ縺ｧ繝・け繧ｹ繝√Ε謠冗判
	*/
void Texture::DrawScale(float scale)
{
	float halfW = (mNewGameW * scale) / 2.0f;
	float halfH = (mNewGameH * scale) / 2.0f;

	DrawExtendGraph(
		static_cast<int>(position_.x - halfW),
		static_cast<int>(position_.y - halfH),
		static_cast<int>(position_.x + halfW),
		static_cast<int>(position_.y + halfH),
		mnHandle, mnTransFlag
	);
}
