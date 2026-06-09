#include "Effect.h"

Effect::Effect(VECTOR initPos, std::string filename, COLOR_U8 Changecolor, float Size, float VisibleTime)
	: Object3D(initPos)
{
	mpEffect = new EffectInfo();
	mpEffect->color = Changecolor;

	for (int i = 0; i < PARTICLE_NUM; i++)
	{
		mpEffect->particle[i].pos = initPos;

		// 3D空間上で全方位へランダムに拡散するように移動初期ベクトルを計算する
		mpEffect->particle[i].dir.x = ((float)GetRand(200) - 100.0f) / 100.0f;
		mpEffect->particle[i].dir.y = ((float)GetRand(200) - 100.0f) / 100.0f;
		mpEffect->particle[i].dir.z = ((float)GetRand(200) - 100.0f) / 100.0f;

		mpEffect->particle[i].speed = ((float)GetRand(SPEED_RAND_MAX) + SPEED_RAND_MIN) / 100.0f;
		mpEffect->particle[i].alpha = 1.0f;
		mpEffect->particle[i].size = Size;
		mpEffect->particle[i].visibleTime = VisibleTime;
	}

	mnGraphHandle = LoadGraph(filename.c_str());
}

Effect::~Effect()
{
	// 動的確保した構造体およびロードしたグラフィックハンドルを安全に解放する
	delete mpEffect;
	DeleteGraph(mnGraphHandle);
}

void Effect::Update()
{
	bool isEnd = true;
	float stepTime = 1.0f / 60.0f; // 60FPS動作を想定した時間刻み幅

	for (int i = 0; i < PARTICLE_NUM; i++)
	{
		if (mpEffect->particle[i].alpha <= 0.0f)
		{
			continue;
		}

		isEnd = false;

		if (mpEffect->particle[i].speed > 0.0f)
		{
			// パーティクルを指定方向へ移動させ、徐々に速度を減衰させる
			mpEffect->particle[i].pos = VAdd(mpEffect->particle[i].pos, VScale(mpEffect->particle[i].dir, mpEffect->particle[i].speed));
			mpEffect->particle[i].speed -= 2.0f * stepTime;

			if (mpEffect->particle[i].speed <= 0.0f)
			{
				mpEffect->particle[i].visibleTime = 0.0f;
			}
		}

		if (mpEffect->particle[i].visibleTime > 0.0f)
		{
			mpEffect->particle[i].visibleTime -= 0.75f * stepTime;
		}
		else
		{
			// 表示時間が切れたら、徐々にアルファ値を減少させてフェードアウトさせる
			mpEffect->particle[i].alpha -= 12.0f * stepTime;
		}
	}

	// すべてのパーティクルがフェードアウトしきったら自身を破棄フラグに設定する
	if (isEnd)
	{
		SetDeleteFlag(true);
	}
}

void Effect::Draw()
{
	SetUseZBufferFlag(TRUE);
	SetWriteZBufferFlag(FALSE); // パーティクル後方のオブジェクトが描画欠けしないよう、Zバッファへの書き込みを禁止

	SetDrawBright(mpEffect->color.r, mpEffect->color.g, mpEffect->color.b);

	// 反転カラー転送パス（輪郭や暗部を際立たせるためのブレンド）
	for (int i = 0; i < PARTICLE_NUM; i++)
	{
		if (mpEffect->particle[i].alpha <= 0.0f)
		{
			continue;
		}

		SetDrawBlendMode(DX_BLENDMODE_INVSRC, (int)(mpEffect->particle[i].alpha * 255.0f));
		DrawBillboard3D(
			mpEffect->particle[i].pos, 0.5f, 0.5f,
			mpEffect->particle[i].size * mpEffect->particle[i].alpha,
			0.0f,
			mnGraphHandle,
			true
		);
	}

	// 加算合成パス（明るく発光するエフェクト表現のためのブレンド）
	for (int i = 0; i < PARTICLE_NUM; i++)
	{
		if (mpEffect->particle[i].alpha <= 0.0f)
		{
			continue;
		}

		SetDrawBlendMode(DX_BLENDMODE_ADD, (int)(mpEffect->particle[i].alpha * 255.0f));
		DrawBillboard3D(
			mpEffect->particle[i].pos, 0.5f, 0.5f,
			mpEffect->particle[i].size * mpEffect->particle[i].alpha,
			0.0f,
			mnGraphHandle,
			true
		);
	}

	// デフォルトのアルファブレンド描画設定に復元する
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255);
	SetDrawBright(255, 255, 255);
	SetUseZBufferFlag(TRUE);
	SetWriteZBufferFlag(TRUE);
}
