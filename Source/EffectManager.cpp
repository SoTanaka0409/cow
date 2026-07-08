#include "EffectManager.h"

EffectManager::EffectManager()
	: effect_resource_handle_(-1)
	, playing_effect_handle_(-1)
{
	Initalize();
}

EffectManager::~EffectManager()
{
}

void EffectManager::Initalize()
{
	// DX繝ｩ繧､繝悶Λ繝ｪ蛛ｴ縺ｧDirect3D11繧剃ｽｿ逕ｨ縺吶ｋ繧医≧縺ｫ險ｭ螳壹☆繧具ｼ・ffekseer縺ｮ隕∵ｱゆｻ墓ｧ假ｼ・
	SetUseDirect3DVersion(DX_DIRECT3D_11);

	// 陦ｨ遉ｺ蜿ｯ閭ｽ縺ｪ繝代・繝・ぅ繧ｯ繝ｫ縺ｮ譛螟ｧ謨ｰ繧呈欠螳壹＠縺ｦ縲・ffekseer繧ｷ繧ｹ繝・Β繧貞・譛溷喧縺吶ｋ
	if (Effekseer_Init(kEffectParticleLimit) == -1)
	{
		DxLib_End();
	}

	// 逕ｻ髱｢繝｢繝ｼ繝牙・譖ｿ・医え繧｣繝ｳ繝峨え/繝輔Ν繧ｹ繧ｯ繝ｪ繝ｼ繝ｳ・画凾縲・ffekseer縺ｮ繧ｰ繝ｩ繝輔ぅ繝・け繧ｹ繝・ヰ繧､繧ｹ繝ｪ繧ｻ繝・ヨ繧帝亟縺・
	SetChangeScreenModeGraphicsSystemResetFlag(FALSE);

	// DX繝ｩ繧､繝悶Λ繝ｪ縺ｧ縺ｮ繝・ヰ繧､繧ｹ蝟ｪ螟ｱ繝ｻ蠕ｩ蟶ｰ縺ｨEffekseer縺ｮ蜀・Κ繝ｪ繧ｽ繝ｼ繧ｹ蜀咲函謌舌ｒ邨舌・縺､縺代ｋ繧ｳ繝ｼ繝ｫ繝舌ャ繧ｯ繧定ｨｭ螳壹☆繧・
	Effekseer_SetGraphicsDeviceLostCallbackFunctions();

	// 3D繧ｨ繝輔ぉ繧ｯ繝域緒逕ｻ譎ゅ↓蜑榊ｾ碁未菫ゑｼ域ｷｱ蠎ｦ蛟､・峨′豁｣縺励￥閠・・縺輔ｌ繧九ｈ縺・〇繝舌ャ繝輔ぃ蜃ｦ逅・ｒ譛牙柑縺ｫ縺吶ｋ
	SetUseZBuffer3D(TRUE);
	SetWriteZBuffer3D(TRUE);
}

void EffectManager::Update()
{
	// 豈弱ヵ繝ｬ繝ｼ繝蜻ｼ縺ｳ蜃ｺ縺励∫樟蝨ｨ蜀咲函荳ｭ縺ｮ蜈ｨEffekseer3D繧ｨ繝輔ぉ繧ｯ繝医・繧｢繝九Γ繝ｼ繧ｷ繝ｧ繝ｳ繧ｹ繝・ャ繝励ｒ騾ｲ繧√ｋ
	UpdateEffekseer3D();
}

void EffectManager::Draw()
{
	// 蜀咲函荳ｭ縺九▽逕ｻ髱｢蜀・↓蜿弱∪繧畿ffekseer繧ｨ繝輔ぉ繧ｯ繝医ｒ3D謠冗判繝代せ縺ｸ騾√ｋ
	DrawEffekseer3D();
}
