#include"InputManager.h"
#include"DxLib.h"

int InputManager::down_buffer_[256] = { 0 };

InputManager::InputManager()
{
}

InputManager::~InputManager()
{
}

/*
 * 騾｣邯壼・蜉帙ｒ髦ｲ縺舌◆繧√∵款縺怜ｧ九ａ縺ｮ1繝輔Ξ繝ｼ繝縺ｮ縺ｿ繧呈､懃衍縺吶ｋ
 * [蜈･蜉嫋 KeyCode: DxLib縺ｮ繧ｭ繝ｼ繧ｳ繝ｼ繝・
 * [蜃ｺ蜉嫋 1: 謚ｼ荳矩幕蟋九ヵ繝ｬ繝ｼ繝縲・: 縺昴ｌ莉･螟・
 * [蜑ｯ菴懃畑] mDownBuffer繧呈峩譁ｰ
 */
int InputManager::CheckDownKey(int KeyCode)
{
    int result = 0;
    int keyState = CheckHitKey(KeyCode);
    
    // 髟ｷ謚ｼ縺励↓繧医ｋ騾｣邯壼・逅・ｒ髦ｲ縺舌◆繧√・繝医Μ繧ｬ繝ｼ蛻､螳・
    if (down_buffer_[KeyCode] == 0 && keyState == 1)
    {
        result = 1;
    }

    // 谺｡繝輔Ξ繝ｼ繝縺ｮ繝医Μ繧ｬ繝ｼ蛻､螳夂畑縺ｫ迥ｶ諷九ｒ險倬鹸
    down_buffer_[KeyCode] = keyState;
    return result;
}

/*
 * 謚ｼ縺礼ｶ壹￠繧定ｨｱ螳ｹ縺吶ｋ騾壼ｸｸ縺ｮ繧ｭ繝ｼ蜈･蜉帛愛螳・
 * [蜈･蜉嫋 KeyCode: DxLib縺ｮ繧ｭ繝ｼ繧ｳ繝ｼ繝・
 * [蜃ｺ蜉嫋 1: 謚ｼ荳倶ｸｭ縲・: 髱樊款荳・
 * [蜑ｯ菴懃畑] 縺ｪ縺・
 */
int InputManager::CheckPressKey(int KeyCode)
{
    return CheckHitKey(KeyCode);
}