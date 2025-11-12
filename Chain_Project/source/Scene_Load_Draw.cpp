/* シーン"ロード画面(描写)"の定義 */
// ※このシーンは非同期読み込み中に描写を行うシーンです。

#pragma once

/* 使用する要素のインクルード */
// ヘッダファイル
#include "Scene_Load_Draw.h"

// コンストラクタ
Scene_Load_Draw::Scene_Load_Draw() : Scene_Base("Scene_Load_Draw", -99999, false, true)
{

}

// 更新
void Scene_Load_Draw::Update()
{
	/* シーンの削除フラグを有効にする */
	// ※このシーンの更新処理が行われるのは非同期読み込みが完了してからなので、更新処理が行われた時点でシーンを削除して問題ない
	this->bDeleteFlg = true;
}

// 描画
void Scene_Load_Draw::Draw()
{
	DrawString(16, 16, "Now Loading...", GetColor(255, 255, 255));
	DrawFormatString(16, 48, GetColor(255, 255, 255), "Remaining Load Num : %d", GetASyncLoadNum());
}
