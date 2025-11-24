/* キャラクター(プレイヤー)の定義 */

/* 使用する要素のインクルード */
// ヘッダファイル
#include "Character_Player.h"

// コンストラクタ
Character_Player::Character_Player() : Character_Base()
{
	/* 初期化 */
	this->iHP_Max	= 100;				// 体力(最大値)
	this->iHP_Now	= 50;				// 体力(現在値)
	this->SizeX		= PLAYER_WIDE;		// キャラクターの幅
	this->SizeY		= PLAYER_HEIGHT;		// キャラクターの高さ
	SetUpImage("Character_Ilust/Player/Test_Player");
}
