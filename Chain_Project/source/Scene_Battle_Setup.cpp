/* シーン"バトル画面"の定義(キャラクターのセットアップ部分) */

/* 使用する要素のインクルード */
// ヘッダファイル
#include "Scene_Battle.h"
// 関連クラス
#include "DataList_Battle.h"
#include "DataList_GameResource.h"
#include "DataList_SaveData.h"
// 関連クラス(キャラクター)
#include "Character_Player.h"
#include "Character_Npc_Include.h"

// キャラクターセットアップ
void Scene_Battle::Character_Setup(int Level, bool BossFlg)
{
	// 引数
	// Level	<- 出てくるキャラクターの種類の判別に使用
	// BossFlg	<- ボス戦であるかの判別に使用

	/* プレイヤー側キャラクターを設定 */
	// 主人公
	this->pDataList_Battle->SetFriendCharacter(DataList_Battle::POSITION_MIDDLE, this->pDataList_GameResource->GetPlayerCharacter());

	// 魔法使いであるなら前衛に使い魔を設置
	std::shared_ptr<DataList_SaveData> SaveData = std::dynamic_pointer_cast<DataList_SaveData>(gpDataListServer->GetDataList("DataList_SaveData"));
	if (SaveData->GetPlayerClassNo() == DataList_SaveData::CLASS_WIZARD)
	{
		this->pDataList_Battle->SetFriendCharacter(DataList_Battle::POSITION_FRONT, std::make_shared<Character_Npc_Servant>());
	}

	/* ボスバトルであるか確認 */
	if (BossFlg)
	{
		// ボスである場合
		switch (Level)
		{
			// ステージ1
			case 1:
				this->pDataList_Battle->SetEnemyCharacter(DataList_Battle::POSITION_BACK,	std::make_shared<Character_Boss_BigSlime_Green>());
				break;

			// ステージ2
			case 2:
				this->pDataList_Battle->SetEnemyCharacter(DataList_Battle::POSITION_MIDDLE, std::make_shared<Character_Boss_Dragon>());
				break;

			// ステージ3
			case 3:
				this->pDataList_Battle->SetEnemyCharacter(DataList_Battle::POSITION_FRONT, std::make_shared<Character_Npc_Robot_Security>());
				this->pDataList_Battle->SetEnemyCharacter(DataList_Battle::POSITION_MIDDLE, std::make_shared<Character_Npc_Robot_Security>());
				this->pDataList_Battle->SetEnemyCharacter(DataList_Battle::POSITION_BACK, std::make_shared<Character_Npc_Robot_Mother>());
				break;
		}
	}
	else
	{
		// ボスでない場合
		switch (Level)
		{
			// ステージ1
			case 0:
				this->pDataList_Battle->SetEnemyCharacter(DataList_Battle::POSITION_MIDDLE,	std::make_shared<Character_Npc_Slime_Green>());
				break;

			case 1:
				this->pDataList_Battle->SetEnemyCharacter(DataList_Battle::POSITION_FRONT,	std::make_shared<Character_Npc_Slime_Blue>());
				this->pDataList_Battle->SetEnemyCharacter(DataList_Battle::POSITION_MIDDLE,	std::make_shared<Character_Npc_Slime_Red>());
				this->pDataList_Battle->SetEnemyCharacter(DataList_Battle::POSITION_BACK,	std::make_shared<Character_Npc_Slime_Green>());
				break;

			case 2:
				this->pDataList_Battle->SetEnemyCharacter(DataList_Battle::POSITION_FRONT,	std::make_shared<Character_Npc_Golem_Soil>());
				this->pDataList_Battle->SetEnemyCharacter(DataList_Battle::POSITION_MIDDLE,	std::make_shared<Character_Npc_Slime_Red>());
				this->pDataList_Battle->SetEnemyCharacter(DataList_Battle::POSITION_BACK,	std::make_shared<Character_Npc_Slime_Green>());
				break;

			// ステージ2
			case 10:
				this->pDataList_Battle->SetEnemyCharacter(DataList_Battle::POSITION_FRONT,	std::make_shared<Character_Npc_Goblin>());
				this->pDataList_Battle->SetEnemyCharacter(DataList_Battle::POSITION_MIDDLE,	std::make_shared<Character_Npc_Skeleton>());
				this->pDataList_Battle->SetEnemyCharacter(DataList_Battle::POSITION_BACK,	std::make_shared<Character_Npc_Bat>());
				break;

			case 11:
				this->pDataList_Battle->SetEnemyCharacter(DataList_Battle::POSITION_FRONT,	std::make_shared<Character_Npc_Goblin>());
				this->pDataList_Battle->SetEnemyCharacter(DataList_Battle::POSITION_MIDDLE,	std::make_shared<Character_Npc_Goblin>());
				this->pDataList_Battle->SetEnemyCharacter(DataList_Battle::POSITION_BACK,	std::make_shared<Character_Npc_Goblin>());
				break;

			case 12:
				{
					// NPCのリスト
					std::vector<std::function<std::shared_ptr<Character_Base>()>> npcFactories = {
						[]() { return std::make_shared<Character_Npc_Goblin>(); },
						[]() { return std::make_shared<Character_Npc_Bat>(); },
						[]() { return std::make_shared<Character_Npc_Skeleton>(); },
						[]() { return std::make_shared<Character_Npc_Golem_Soil>(); },
					};

					// 前衛
					int RandIndex = GetRand(static_cast<int>(npcFactories.size() - 1));
					std::shared_ptr<Character_Base> randomNpc = npcFactories[RandIndex]();
					this->pDataList_Battle->SetEnemyCharacter(DataList_Battle::POSITION_FRONT, randomNpc);
					// 中衛
					RandIndex = GetRand(static_cast<int>(npcFactories.size() - 1));
					randomNpc = npcFactories[RandIndex]();
					this->pDataList_Battle->SetEnemyCharacter(DataList_Battle::POSITION_MIDDLE, randomNpc);
					// 後衛
					RandIndex = GetRand(static_cast<int>(npcFactories.size() - 1));
					randomNpc = npcFactories[RandIndex]();
					this->pDataList_Battle->SetEnemyCharacter(DataList_Battle::POSITION_BACK, randomNpc);
				}
				break;

			// ステージ3
			case 20:
				this->pDataList_Battle->SetEnemyCharacter(DataList_Battle::POSITION_FRONT,	std::make_shared<Character_Npc_Chimera>());
				break;

			case 23:
				{
					this->pDataList_Battle->SetEnemyCharacter(DataList_Battle::POSITION_MIDDLE, std::make_shared<Character_Npc_Chimera>());
					int RandIndex = GetRand(1);
					if (RandIndex >= 1)
					{
						this->pDataList_Battle->SetEnemyCharacter(DataList_Battle::POSITION_FRONT, std::make_shared<Character_Npc_Chimera>());
					}
					if (RandIndex >= 2)
					{
						this->pDataList_Battle->SetEnemyCharacter(DataList_Battle::POSITION_BACK, std::make_shared<Character_Npc_Chimera>());
					}
				}
				break;

			case 21:
				{
					// NPCのリスト
					std::vector<std::function<std::shared_ptr<Character_Base>()>> npcFactories = {
						[]() { return std::make_shared<Character_Npc_Chimera>(); },
						[]() { return std::make_shared<Character_Npc_Chimera>(); },
						[]() { return std::make_shared<Character_Npc_Robot_Security>(); },
					};

					// 前衛
					int RandIndex = GetRand(static_cast<int>(npcFactories.size() - 1));
					std::shared_ptr<Character_Base> randomNpc = npcFactories[RandIndex]();
					this->pDataList_Battle->SetEnemyCharacter(DataList_Battle::POSITION_FRONT, randomNpc);
					// 中衛
					RandIndex = GetRand(static_cast<int>(npcFactories.size() - 1));
					randomNpc = npcFactories[RandIndex]();
					this->pDataList_Battle->SetEnemyCharacter(DataList_Battle::POSITION_MIDDLE, randomNpc);
					// 後衛
					RandIndex = GetRand(static_cast<int>(npcFactories.size() - 1));
					randomNpc = npcFactories[RandIndex]();
					this->pDataList_Battle->SetEnemyCharacter(DataList_Battle::POSITION_BACK, randomNpc);
				}
				break;

			case 22:
				{
					// NPCのリスト
					std::vector<std::function<std::shared_ptr<Character_Base>()>> npcFactories = {
						[]() { return std::make_shared<Character_Npc_Chimera>(); },
						[]() { return std::make_shared<Character_Npc_Robot_Security>(); },
					};

					// 前衛
					int RandIndex = GetRand(static_cast<int>(npcFactories.size() - 1));
					std::shared_ptr<Character_Base> randomNpc = npcFactories[RandIndex]();
					this->pDataList_Battle->SetEnemyCharacter(DataList_Battle::POSITION_FRONT, randomNpc);
					// 中衛
					RandIndex = GetRand(static_cast<int>(npcFactories.size() - 1));
					randomNpc = npcFactories[RandIndex]();
					this->pDataList_Battle->SetEnemyCharacter(DataList_Battle::POSITION_MIDDLE, randomNpc);
					// 後衛
					RandIndex = GetRand(static_cast<int>(npcFactories.size() - 1));
					randomNpc = npcFactories[RandIndex]();
					this->pDataList_Battle->SetEnemyCharacter(DataList_Battle::POSITION_BACK, randomNpc);
				}
				break;
		}
	}
}
