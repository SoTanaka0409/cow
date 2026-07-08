#pragma once

// DxLib縺ｮ繧ｭ繝ｼ蜈･蜉帙ｒ繝ｩ繝・・縺励√ヵ繝ｬ繝ｼ繝髢薙・迥ｶ諷句､牙喧繧堤ｮ｡逅・☆繧・
class InputManager
{
public:
    InputManager();
    ~InputManager();

    /*
     * 騾｣邯壼・蜉帙ｒ髦ｲ縺舌◆繧√∵款縺怜ｧ九ａ縺ｮ1繝輔Ξ繝ｼ繝縺ｮ縺ｿ繧呈､懃衍縺吶ｋ
     * [蜈･蜉嫋 KeyCode: DxLib縺ｮ繧ｭ繝ｼ繧ｳ繝ｼ繝・
     * [蜃ｺ蜉嫋 1: 謚ｼ荳矩幕蟋九ヵ繝ｬ繝ｼ繝縲・: 縺昴ｌ莉･螟・
     * [蜑ｯ菴懃畑] mDownBuffer繧呈峩譁ｰ
     */
    static int CheckDownKey(int KeyCode);

    /*
     * 謚ｼ縺礼ｶ壹￠繧定ｨｱ螳ｹ縺吶ｋ騾壼ｸｸ縺ｮ繧ｭ繝ｼ蜈･蜉帛愛螳・
     * [蜈･蜉嫋 KeyCode: DxLib縺ｮ繧ｭ繝ｼ繧ｳ繝ｼ繝・
     * [蜃ｺ蜉嫋 1: 謚ｼ荳倶ｸｭ縲・: 髱樊款荳・
     * [蜑ｯ菴懃畑] 縺ｪ縺・
     */
    static int CheckPressKey(int KeyCode);

private:
    // 繝医Μ繧ｬ繝ｼ蛻､螳壹↓蠢・ｦ√↑蜑阪ヵ繝ｬ繝ｼ繝縺ｮ繧ｭ繝ｼ迥ｶ諷九ｒ菫晄戟
    static int down_buffer_[256];
};