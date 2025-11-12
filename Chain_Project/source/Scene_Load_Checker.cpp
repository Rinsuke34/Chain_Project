/* シーン"ロード画面(完了チェッカー)"の定義 */
// ※このシーンは非同期読み込み中に他のシーンが動作しないようにするためのシーンです

#pragma once

/* 使用する要素のインクルード */
// ヘッダファイル
#include "Scene_Load_Checker.h"

// コンストラクタ
Scene_Load_Checker::Scene_Load_Checker() : Scene_Base("Scene_Checker", 99999, true, false)
{

}

// 更新
void Scene_Load_Checker::Update()
{
	/* 非同期読み込み中の処理が完了しているか確認 */
	if (GetASyncLoadNum() == 0)
	{
		// 完了している場合
		/* シーンの削除フラグを有効にする */
		this->bDeleteFlg = true;
		return;
	}
}

