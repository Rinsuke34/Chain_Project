/* キャラクター(テストNPC)の定義 */

/* 使用する要素のインクルード */
// ヘッダファイル
#include "Character_Npc_Test.h"

// コンストラクタ
Character_Npc_Test::Character_Npc_Test()
{
	/* 初期化 */
	this->iHP_Max = 100;			// 体力(最大値)
	this->iHP_Now = 80;				// 体力(現在値)

	SetUpImage("Character_Ilust/Monster/Test");
}
