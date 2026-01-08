/* シーン"バトル画面"の定義(キャラクターのセットアップ部分) */

/* 使用する要素のインクルード */
// ヘッダファイル
#include "Scene_Battle.h"
// 関連クラス
#include "DataList_Battle.h"
// 関連クラス(キャラクター)
#include "Character_Player.h"
#include "Character_Npc_Slime_Green.h"
#include "Character_Npc_Bat.h"

// キャラクターセットアップ
void Scene_Battle::Character_Setup(int Level, bool BossFlg)
{
	// 引数
	// Level	<- 出てくるキャラクターの種類の判別に使用
	// BossFlg	<- ボス戦であるかの判別に使用

	/* プレイヤー側キャラクターを設定 */
	this->pDataList_Battle->SetFriendCharacter(DataList_Battle::POSITION_MIDDLE, std::make_shared<Character_Player>());

	/* ボスバトルであるか確認 */
	if (BossFlg)
	{
		// ボスである場合
		switch (Level)
		{
			case 0:
				this->pDataList_Battle->SetEnemyCharacter(DataList_Battle::POSITION_FRONT,	std::make_shared<Character_Slime_Green>());
				this->pDataList_Battle->SetEnemyCharacter(DataList_Battle::POSITION_MIDDLE, std::make_shared<Character_Slime_Green>());
				this->pDataList_Battle->SetEnemyCharacter(DataList_Battle::POSITION_BACK,	std::make_shared<Character_Bat>());
				break;

			case 1:
				this->pDataList_Battle->SetEnemyCharacter(DataList_Battle::POSITION_FRONT,	std::make_shared<Character_Bat>());
				this->pDataList_Battle->SetEnemyCharacter(DataList_Battle::POSITION_MIDDLE, std::make_shared<Character_Bat>());
				this->pDataList_Battle->SetEnemyCharacter(DataList_Battle::POSITION_BACK,	std::make_shared<Character_Slime_Green>());
				break;
		}
	}
	else
	{
		// ボスでない場合
		switch (Level)
		{
			case 0:
				this->pDataList_Battle->SetEnemyCharacter(DataList_Battle::POSITION_FRONT,	std::make_shared<Character_Slime_Green>());
				break;

			case 1:
				this->pDataList_Battle->SetEnemyCharacter(DataList_Battle::POSITION_FRONT,	std::make_shared<Character_Slime_Green>());
				this->pDataList_Battle->SetEnemyCharacter(DataList_Battle::POSITION_MIDDLE,	std::make_shared<Character_Bat>());
				break;

			case 2:
				this->pDataList_Battle->SetEnemyCharacter(DataList_Battle::POSITION_FRONT,	std::make_shared<Character_Slime_Green>());
				this->pDataList_Battle->SetEnemyCharacter(DataList_Battle::POSITION_MIDDLE,	std::make_shared<Character_Slime_Green>());
				break;
		}
	}
}
