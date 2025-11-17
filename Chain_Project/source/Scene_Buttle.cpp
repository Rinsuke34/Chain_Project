/* シーン"バトル画面"の定義 */

/* 使用する要素のインクルード */
// ヘッダファイル
#include "Scene_Battle.h"

// コンストラクタ
Scene_Battle::Scene_Battle() : Scene_Base("Scene_Battle", 0, false, false)
{

}

// デストラクタ
Scene_Battle::~Scene_Battle()
{

}

// 更新
void Scene_Battle::Update()
{

}

// 描画
void Scene_Battle::Draw()
{
	DrawString(16, 16, "Battle", GetColor(255, 255, 255));
	DrawBox(0, 0, 120, 170, GetColor(0, 0, 255), TRUE);
}
