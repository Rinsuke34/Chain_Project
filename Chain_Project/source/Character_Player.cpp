/* キャラクター(プレイヤー)の定義 */

/* 使用する要素のインクルード */
// ヘッダファイル
#include "Character_Player.h"

// コンストラクタ
Character_Player::Character_Player() : Character_Base()
{
	/* 初期化 */
	SetUpImage("Character/Player/Test_Player");
}

//// 体力バー描画
//void Character_Player::Draw_HPBar()
//{
//
//}
