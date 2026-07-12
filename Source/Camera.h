#pragma once
#include "DxLib.h"
#include "Model.h"
#include"Object3D.h"

class Skill;

// 繧ｲ繝ｼ繝蜀・・3D繧ｫ繝｡繝ｩ蛻ｶ蠕｡縲∽ｽ咲ｽｮ・豕ｨ隕也せ險育ｮ励√♀繧医・謖ｯ蜍包ｼ医す繧ｧ繧､繧ｯ・画ｼ泌・繧堤ｮ｡逅・☆繧九け繝ｩ繧ｹ
class Camera
{
public:
	/*
	 * @brief 繝｡繝ｳ繝仙､画焚繧貞・譛溯ｨｭ螳夊ｧ貞ｺｦ縺ｨ繝・ヵ繧ｩ繝ｫ繝亥､縺ｫ蛻晄悄險ｭ螳壹☆繧・
	 * [蜈･蜉嫋 なし
	 * [蜃ｺ蜉嫋 なし
	 * [蜑ｯ菴懃畑] なし
	 */
	Camera();
	~Camera();

	/*
	 * @brief 繧ｫ繝｡繝ｩ縺ｮ菴咲ｽｮ縲∬ｧ貞ｺｦ縲√♀繧医・繧ｷ繧ｧ繧､繧ｯ繝代Λ繝｡繝ｼ繧ｿ繧貞・譛溷､縺ｫ繝ｪ繧ｻ繝・ヨ縺吶ｋ
	 * [蜈･蜉嫋 なし
	 * [蜃ｺ蜉嫋 なし
	 * [蜑ｯ菴懃畑] 場合ｨｮ繧ｫ繝｡繝ｩ繝代Λ繝｡繝ｼ繧ｿ螟画焚縺ｮ繝ｪ繧ｻ繝・ヨ
	 */
	void Initialize();

	/*
	 * @brief 繧ｫ繝｡繝ｩ隗｣謾ｾ蜃ｦ逅・ｼ育樟蝨ｨ縺ｯ遨ｺ繝｡繧ｽ繝・ラ・・
	 * [蜈･蜉嫋 なし
	 * [蜃ｺ蜉嫋 なし
	 * [蜑ｯ菴懃畑] なし
	 */
	void Finalize();

	/*
	 * @brief 繧ｫ繝｡繝ｩ縺ｮ迴ｾ蝨ｨ諠・ｱ繧偵ョ繝舌ャ繧ｰ遲峨・逶ｮ逧・〒逕ｻ髱｢謠冗判縺吶ｋ・育樟蝨ｨ縺ｯ遨ｺ繝｡繧ｽ繝・ラ・・
	 * [蜈･蜉嫋 なし
	 * [蜃ｺ蜉嫋 なし
	 * [蜑ｯ菴懃畑] なし
	 */
	void Draw();

	/*
	 * @brief 繝槭え繧ｹ縺ｮ遘ｻ蜍暮㍼繧・ご繝ｼ繝縺ｮ騾ｲ陦後ヵ繧ｧ繝ｼ繧ｺ縲√す繧ｧ繧､繧ｯ迥ｶ諷九ｒ險育ｮ励＠縲∵怙邨ら噪縺ｪ3D繧ｫ繝｡繝ｩ菴咲ｽｮ縺ｨ豕ｨ隕也せ繧奪X繝ｩ繧､繝悶Λ繝ｪ縺ｫ蜿肴丐縺吶ｋ
	 * [蜈･蜉嫋 なし
	 * [蜃ｺ蜉嫋 なし
	 * [蜑ｯ菴懃畑] DX繝ｩ繧､繝悶Λ繝ｪ of 3D繧ｫ繝｡繝ｩ迥ｶ諷九・險ｭ螳壼､画峩
	 */
	void Update();

	/*
	 * @brief 3D遨ｺ髢謎ｸ翫〒繝槭え繧ｹ遘ｻ蜍暮㍼縺ｨ繧ｭ繝ｼ蜈･蜉・0繧ｭ繝ｼ)縺ｫ蝓ｺ縺･縺上き繝｡繝ｩ隗貞ｺｦ縺ｮ譖ｴ譁ｰ險育ｮ励ｒ陦後≧
	 * [蜈･蜉嫋 なし
	 * [蜃ｺ蜉嫋 なし
	 * [蜑ｯ菴懃畑] 豌ｴ蟷ｳ蝗櫁ｻ｢隗偵・蝙ら峩蝗櫁ｻ｢隗貞､画焚縺ｮ譖ｴ譁ｰ
	 */
	void UpdateRotate();

	/*
	 * @brief 繧ｲ繝ｼ繝縺ｮ繝輔ぉ繝ｼ繧ｺ・域ｼ泌・繧ｫ繝｡繝ｩ縲√・繝ｬ繧､繝､繝ｼ霑ｽ蠕薙↑縺ｩ・峨↓蜷医ｏ縺帙◆蟆ら畑縺ｮ繧ｫ繝｡繝ｩ菴咲ｽｮ・豕ｨ隕也せ譖ｴ譁ｰ蜃ｦ逅・ｒ陦後≧
	 * [蜈･蜉嫋 phase: 迴ｾ蝨ｨ縺ｮ繧ｲ繝ｼ繝繝輔ぉ繝ｼ繧ｺ, ufoPos: 繝励Ξ繧､繝､繝ｼUFO縺ｮ蠎ｧ讓・ tornadoPos: 遶懷ｷｻ縺ｮ蠎ｧ讓・
	 * [蜃ｺ蜉嫋 なし
	 * [蜑ｯ菴懃畑] 繧ｫ繝｡繝ｩ蠎ｧ讓吶♀繧医・豕ｨ隕也せ蠎ｧ讓吶・譖ｴ譁ｰ
	 */
	void UpdateCameraByPhase(int phase, VECTOR ufoPos, VECTOR tornadoPos);

	// 繝昴ず繧ｷ繝ｧ繝ｳ縲∵ｳｨ隕也せ縺ｪ縺ｩ縺ｮ繧ｲ繝・ち繝ｼ・繧ｻ繝・ち繝ｼ
	void SetPosition(VECTOR pos) { position_ = pos; }
	void SetTarget(Object3D* target) { target_ = target; }
	VECTOR GetPosition() { return position_; }

	VECTOR GetLookAtPosition() { return look_at_position_; }

	/*
	 * @brief 蜑阪ヵ繝ｬ繝ｼ繝縺九ｉ繝槭え繧ｹ縺檎ｧｻ蜍輔＠縺溘°縺ｩ縺・°繧呈､懃衍縺吶ｋ
	 * [蜈･蜉嫋 なし
	 * [蜃ｺ蜉嫋 繝槭え繧ｹ縺檎ｧｻ蜍輔＠縺溷ｴ蜷医・true
	 * [蜑ｯ菴懃畑] なし
	 */
	bool IsMouseMoved();

	/*
	 * @brief 繧ｫ繝｡繝ｩ縺ｮ蠎ｧ讓吶↓蟇ｾ縺励※迴ｾ蝨ｨ邏ｯ遨阪＆繧後※縺・ｋ繧ｷ繧ｧ繧､繧ｯ驥上・貂幄｡ｰ縺ｨ蜉邂怜・逅・ｒ陦後≧
	 * [蜈･蜉嫋 なし
	 * [蜃ｺ蜉嫋 なし
	 * [蜑ｯ菴懃畑] mvShakePosition縺ｮ譖ｸ縺肴鋤縺・
	 */
	void Shake();

	/*
	 * @brief 繧ｫ繝｡繝ｩ繧呈昭繧峨☆繧ｷ繧ｧ繧､繧ｯ貍泌・縺ｮ蛻晄悄蛟､繧定ｨｭ螳壹☆繧・
	 * [蜈･蜉嫋 time: 邯咏ｶ壽凾髢難ｼ育ｧ抵ｼ・ width: 謠ｺ繧悟ｹ・ angleSpeed: 謠ｺ繧悟捉譛滄溷ｺｦ, stepTime: 譎る俣邨碁℃髢馴囈
	 * [蜃ｺ蜉嫋 なし
	 * [蜑ｯ菴懃畑] 繧ｷ繧ｧ繧､繧ｯ蜀咲函逕ｨ繧ｿ繧､繝槭・縺ｨ繝代Λ繝｡繝ｼ繧ｿ縺ｮ蛻晄悄險ｭ螳・
	 */
	void SetupShake(float time, float width, float angleSpeed, float stepTime = 1.0f);

	// 繝輔ぉ繝ｼ繧ｺ縺斐→縺ｮ貍泌・逕ｨ迚ｹ谿翫き繝｡繝ｩ縺梧怏蜉ｹ蛹悶＆繧後※縺・ｋ縺九・蛻､螳壹ご繝・ち繝ｼ
	bool GetIsPhaseCameraActive() const { return is_phase_camera_active_; }

private:
	float horizontal_angle_;  // 繧ｫ繝｡繝ｩ縺ｮ豌ｴ蟷ｳ蝗櫁ｻ｢隗抵ｼ医Λ繧ｸ繧｢繝ｳ・・
	float vertical_angle_;    // 繧ｫ繝｡繝ｩ縺ｮ蝙ら峩蝗櫁ｻ｢隗抵ｼ医Λ繧ｸ繧｢繝ｳ・・

	bool is_phase_camera_active_; // 繝輔ぉ繝ｼ繧ｺ蟆ら畑繧ｫ繝｡繝ｩ縺梧怏蜉ｹ縺九←縺・°縺ｮ繝輔Λ繧ｰ

	VECTOR position_;        // 繧ｫ繝｡繝ｩ縺ｮ蝓ｺ貅紋ｽ咲ｽｮ蠎ｧ讓・
	VECTOR look_at_position_;  // 繧ｫ繝｡繝ｩ縺ｮ豕ｨ隕也せ・医Ν繝・け繧｢繝・ヨ・牙ｺｧ讓・
	VECTOR dir_;               // 繧ｫ繝｡繝ｩ縺ｮ隕也ｷ壼髄縺阪・繧ｯ繝医Ν

	Object3D* target_;       // 豕ｨ隕門ｯｾ雎｡縺ｨ縺ｪ繧・D繧ｪ繝悶ず繧ｧ繧ｯ繝医∈縺ｮ繝昴う繝ｳ繧ｿ

	const float kRotateSpeed = 0.2f; // 繧ｫ繝｡繝ｩ蝗櫁ｻ｢縺ｮ霑ｽ蠕馴溷ｺｦ菫よ焚
	float target_angle_;      // 逶ｮ讓吶→縺吶ｋ豌ｴ蟷ｳ蝗櫁ｻ｢隗貞ｺｦ
	float angle_;            // 迴ｾ蝨ｨ縺ｮ陬憺俣荳ｭ豌ｴ蟷ｳ蝗櫁ｻ｢隗貞ｺｦ
	int center_x_;              // 逕ｻ髱｢荳ｭ必要・X蠎ｧ讓・
	int center_y_;              // 逕ｻ髱｢荳ｭ必要・Y蠎ｧ讓・

	int mouse_x_, mouse_y_;       // 繝槭え繧ｹ縺ｮ迴ｾ蝨ｨ菴咲ｽｮ蠎ｧ讓呻ｼ井ｸ譎ょ､画焚・・

	int current_mouse_x_ = 0;          // 迴ｾ蝨ｨ繝輔Ξ繝ｼ繝縺ｧ縺ｮ繝槭え繧ｹ菴咲ｽｮX
	int current_mouse_y_ = 0;          // 迴ｾ蝨ｨ繝輔Ξ繝ｼ繝縺ｧ縺ｮ繝槭え繧ｹ菴咲ｽｮY

	int prev_mouse_x_ = 0;      // 1繝輔Ξ繝ｼ繝蜑阪〒縺ｮ繝槭え繧ｹ菴咲ｽｮX
	int prev_mouse_y_ = 0;      // 1繝輔Ξ繝ｼ繝蜑阪〒縺ｮ繝槭え繧ｹ菴咲ｽｮY

	int shake_time_;          // 繧ｷ繧ｧ繧､繧ｯ謖∫ｶ壽凾髢薙ヵ繝ｬ繝ｼ繝繧ｫ繧ｦ繝ｳ繧ｿ
	int shake_time_count_;     // 繧ｷ繧ｧ繧､繧ｯ邨碁℃繝輔Ξ繝ｼ繝繧ｫ繧ｦ繝ｳ繧ｿ

	float shake_angle_;       // 繧ｷ繧ｧ繧､繧ｯ險育ｮ礼畑縺ｮ豁｣蠑ｦ豕｢隗貞ｺｦ繧ｫ繧ｦ繝ｳ繧ｿ
	float shake_time_counter_; // 繧ｷ繧ｧ繧､繧ｯ縺ｮ繝輔ぉ繝ｼ繝峨い繧ｦ繝育畑譎る俣繧ｫ繧ｦ繝ｳ繧ｿ
	float mfShakeTime;        // 險ｭ螳壹＆繧後◆繧ｷ繧ｧ繧､繧ｯ譎る俣
	float shake_width_;       // 險ｭ螳壹＆繧後◆譛螟ｧ謠ｺ繧悟ｹ・
	float shake_angle_speed_;  // 險ｭ螳壹＆繧後◆謠ｺ繧後・蜻ｨ譛滄溷ｺｦ
	float step_time_;         // 豈弱ヵ繝ｬ繝ｼ繝縺ｮ繧ｷ繧ｧ繧､繧ｯ譎る俣貂帷ｮ怜ｹ・
	VECTOR shake_position_;   // 繧ｷ繧ｧ繧､繧ｯ險育ｮ励↓繧医▲縺ｦ蜉邂励＆繧後ｋ繧ｫ繝｡繝ｩ繧ｪ繝輔そ繝・ヨ蠎ｧ讓吝､
};
