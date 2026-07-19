#pragma once
#include"dxlib.h"

// 設計ルール：マジックナンバーによる画面遷移のバグを防ぎ、メニュー選択肢のIDを一元管理する静的メンバクラス
class SelectionManager
{
public:
	enum Title
	{
		NewGame = 0,
		Tutorial,
		OperationProcedures,
		titleOUT,
		title,
	};

public:
	SelectionManager();
	~SelectionManager();

	// 仕様制約：マウス操作を持たないコントローラーやキーボードでの項目選択状態を、シーンを跨いで同期・保持するための静的インデックス
	static int TitlecrrentIndex;
};
