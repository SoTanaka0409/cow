#include "ServiceLocator.h"
#include "Fever.h"
#include "GoldCow.h"
#include "CowManager.h"
#include "Master.h"
#include "SceneManager.h"
#include "ObjectManager.h"
#include "Player3D.h"
#include "Cow.h"
#include "Utility.h"
#include "GameConstants.h"

Fever::Fever()
	: drop_time_(0)
	, drop_count_(0)
	, fever_player_(nullptr)
{
	gauge_ = 0;
	timer_ = 0;
	is_fever_ = false;
}

Fever::~Fever()
{
}

void Fever::AddGauge(int value)
{
}

void Fever::StartFever(Player3D* player)
{
	if (player == nullptr) return;
	fever_player_ = player;
	player_status_ = player->GetStatusAttack();
	player->SetStatusAttack(player_status_ * 2.0f);
	is_fever_ = true;
	timer_ = 600; // 髮｣譏灘ｺｦ隱ｿ謨ｴ縺ｮ縺溘ａ繝輔ぅ繝ｼ繝舌・邯咏ｶ壽凾髢薙ｒ10遘・600繝輔Ξ繝ｼ繝)縺ｫ蝗ｺ螳壹☆繧・
	drop_count_ = 0;
	drop_time_ = 60;
	Master::FeverFlag = true;
}

void Fever::EndFever()
{
	if (fever_player_ != nullptr)
	{
		fever_player_->SetStatusAttack(player_status_);
		fever_player_ = nullptr;
	}
	is_fever_ = false;
	
	// 繧ｹ繝・・繧ｸ荳翫・迯ｲ迚ｩ縺梧椡貂・☆繧九・繧帝亟縺舌◆繧∫ｵゆｺ・凾縺ｫ蝓ｺ譛ｬ讒区・縺ｧ蜀埼・鄂ｮ縺吶ｋ
	VECTOR spawnPos = Utility::StageSize;
	ServiceLocator::GetCowManager()->SpawnCow(GameConstants::kCowGold.model_path, spawnPos, 50.0f, CowMove::kCowGold, 1);
	ServiceLocator::GetCowManager()->SpawnCow(GameConstants::kCowDefault.model_path, spawnPos, 50.0f, CowMove::kCow1, 10);
	ServiceLocator::GetAnimalManager()->SpawnAnimal(GameConstants::kAnimalSheep.model_path, spawnPos, 50.0f, AnimalMove::kAnimal1, 5);
	Master::FeverFlag = false;
}

void Fever::Update()
{
	if (!is_fever_) return;
	
	SetDrawBlendMode(DX_BLENDMODE_ADD, 180);

	// 繝輔ぅ繝ｼ繝舌・迥ｶ諷九〒縺ゅｋ縺薙→繧定ｦ冶ｦ夂噪縺ｫ莨昴∴繧九◆繧∫判髱｢蜈ｨ菴薙↓蜉邂励ヶ繝ｬ繝ｳ繝峨〒繧ｨ繝輔ぉ繧ｯ繝医ｒ謠冗判縺吶ｋ
	DrawBox(0, 0, Utility::kScreenWidth, Utility::kScreenHeight, GetColor(255, 200, 50), FALSE);
	DrawBox(1, 1, Utility::kScreenWidth, Utility::kScreenHeight, GetColor(255, 220, 100), FALSE);
	DrawBox(2, 2, Utility::kScreenWidth, Utility::kScreenHeight, GetColor(255, 255, 180), FALSE);

	for (int i = 0; i < 30; i++)
	{
		DrawBox(
			i,
			i,
			Utility::kScreenWidth - i,
			Utility::kScreenHeight - i,
			GetColor(255, 200, 50),
			FALSE
		);
	}

	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

	drop_count_++;
	timer_--;
	
	// 繝懊・繝翫せ繧ｹ繧ｳ繧｢迯ｲ蠕玲ｩ滉ｼ壹ｒ謠蝉ｾ帙☆繧九◆繧∽ｸ螳夐俣髫斐〒驥代・迚帙ｒ逕滓・縺吶ｋ
	if (drop_count_ > drop_time_)
	{
		drop_count_ = 0;
		VECTOR spawnPos = Utility::StageSize;
		ServiceLocator::GetCowManager()->SpawnCow(GameConstants::kCowGold.model_path, spawnPos, 50.0f, CowMove::kCowGold, 2, true);
	}

	// 繝輔ぅ繝ｼ繝舌・邯咏ｶ壽凾髢薙ｒ雜・℃縺励◆縺溘ａ迥ｶ諷九ｒ騾壼ｸｸ縺ｫ謌ｻ縺・
	if (timer_ <= 0)
	{
		EndFever();
	}
}

bool Fever::IsFever()
{
	return is_fever_;
}