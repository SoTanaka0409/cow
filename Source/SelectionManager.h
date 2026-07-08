#pragma once
#include"dxlib.h"

// 繧ｿ繧､繝医Ν逕ｻ髱｢繧・Μ繧ｶ繝ｫ繝育判髱｢縺ｧ縺ｮ繝｡繝九Η繝ｼ鬆・岼驕ｸ謚曵D縺翫ｈ縺ｳ迴ｾ蝨ｨ繧､繝ｳ繝・ャ繧ｯ繧ｹ繧堤ｮ｡逅・☆繧九け繝ｩ繧ｹ
class SelectionManager
{
public:
	// 繝懊ち繝ｳ縺ｪ縺ｩ縺ｮ繝｡繝九Η繝ｼ鬆・岼驕ｸ謚曵D
	enum Title
	{
		NewGame = 0,            // 譁ｰ隕上ご繝ｼ繝髢句ｧ・
		Tutorial,               // 繝√Η繝ｼ繝医Μ繧｢繝ｫ髢句ｧ・
		OperationProcedures,    // 謫堺ｽ懆ｪｬ譏守判髱｢縺ｸ驕ｷ遘ｻ
		titleOUT,               // 繧ｲ繝ｼ繝繧堤ｵゆｺ・
		title,                  // 繧ｿ繧､繝医Ν縺ｸ謌ｻ繧・
	};

public:
	SelectionManager();
	~SelectionManager();
	
	static int TitlecrrentIndex; // 迴ｾ蝨ｨ驕ｸ謚樔ｸｭ縺ｮ鬆・岼繧､繝ｳ繝・ャ繧ｯ繧ｹ・医く繝ｼ繝懊・繝蛾∈謚樒畑・・
};

