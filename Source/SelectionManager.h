#pragma once
#include"dxlib.h"

// タイトル画面やリザルト画面でのメニュー項目選択IDおよび現在インデックスを管理するクラス
class SelectionManager
{
public:
	// ボタンなどのメニュー項目選択ID
	enum Title
	{
		NewGame = 0,            // 新規ゲーム開始
		Tutorial,               // チュートリアル開始
		OperationProcedures,    // 操作説明画面へ遷移
		titleOUT,               // ゲームを終了
		title,                  // タイトルへ戻る
	};

public:
	SelectionManager();
	~SelectionManager();
	
	static int TitlecrrentIndex; // 現在選択中の項目インデックス（キーボード選択用）
};
