/* データリスト"ゲームリソース管理"クラスの定義(カードのセットアップ部分) */

/* 使用する要素のインクルード */
// ヘッダファイル
#include "DataList_GameResource.h"
// カードクラス
#include "Card_Include.h"
// 関連クラス
#include "DataList_SaveData.h"

// カードのセットアップ処理
void DataList_GameResource::CardSetup(std::vector<std::shared_ptr<Card_Base>>&	DeckCardList)
{
	// 引数
	// DeckCardList : デッキカードリスト(カードのセットアップ後のデッキカードリストを格納するための引数)
	// 以下、デッキ編成のコンセプト
	// レベル0 : 共通のデッキとし、長所無し
	// レベル1 : 木製の装備を各種1つコンセプトに対応した装備に変更
	// レベル2 : 木製の装備をすべてコンセプトに対応した装備に変更
	// レベル3 : 道具、スペルをコンセプトに対応した物に変更

	/* セーブデータを取得 */
	std::shared_ptr<DataList_SaveData> pDataList_SaveData = std::dynamic_pointer_cast<DataList_SaveData>(gpDataListServer->GetDataList("DataList_SaveData"));

	/* レベルを取得 */
	int Level_Deck = pDataList_SaveData->GetLevel_Deck(pDataList_SaveData->GetPlayerClassNo());

	/* レベル0であるなら全クラス共通のデッキとする */
	if (Level_Deck == 0)
	{
		// 木の剣×3
		DeckCardList.push_back(std::make_shared<Card_Arms_WoodenSword>());
		DeckCardList.push_back(std::make_shared<Card_Arms_WoodenSword>());
		DeckCardList.push_back(std::make_shared<Card_Arms_WoodenSword>());
		// 木の盾×3
		DeckCardList.push_back(std::make_shared<Card_Arms_WoodenShield>());
		DeckCardList.push_back(std::make_shared<Card_Arms_WoodenShield>());
		DeckCardList.push_back(std::make_shared<Card_Arms_WoodenShield>());
		// 木の弓×3
		DeckCardList.push_back(std::make_shared<Card_Arms_WoodenBow>());
		DeckCardList.push_back(std::make_shared<Card_Arms_WoodenBow>());
		DeckCardList.push_back(std::make_shared<Card_Arms_WoodenBow>());
		// 回復ポーション × 1
		DeckCardList.push_back(std::make_shared<Card_Item_HealingPotion>());
		// 装備強化スペル × 1
		DeckCardList.push_back(std::make_shared<Card_Spell_ArmsEnhancement>());
		return;
	}

	/* クラスに応じた処理 */
	switch (pDataList_SaveData->GetPlayerClassNo())
	{
		// 旅人
		case DataList_SaveData::CLASS_TRAVELER:
			switch (Level_Deck)
			{
				case 1:
					// 木の剣 × 2
					DeckCardList.push_back(std::make_shared<Card_Arms_WoodenSword>());
					DeckCardList.push_back(std::make_shared<Card_Arms_WoodenSword>());
					// 旅人の剣 × 1
					DeckCardList.push_back(std::make_shared<Card_Arms_TravelerSword>());
					// 木の盾 × 2
					DeckCardList.push_back(std::make_shared<Card_Arms_WoodenShield>());
					DeckCardList.push_back(std::make_shared<Card_Arms_WoodenShield>());
					// 旅人の盾 × 1
					DeckCardList.push_back(std::make_shared<Card_Arms_TravelerShield>());
					// 木の弓 × 2
					DeckCardList.push_back(std::make_shared<Card_Arms_WoodenBow>());
					DeckCardList.push_back(std::make_shared<Card_Arms_WoodenBow>());
					// 旅人の弓 × 1
					DeckCardList.push_back(std::make_shared<Card_Arms_TravelerBow>());
					// 装備強化スペル × 1
					DeckCardList.push_back(std::make_shared<Card_Spell_ArmsEnhancement>());
					break;

				case 2:
					// 旅人の剣 × 3
					DeckCardList.push_back(std::make_shared<Card_Arms_TravelerSword>());
					DeckCardList.push_back(std::make_shared<Card_Arms_TravelerSword>());
					DeckCardList.push_back(std::make_shared<Card_Arms_TravelerSword>());
					// 旅人の盾 × 3
					DeckCardList.push_back(std::make_shared<Card_Arms_TravelerShield>());
					DeckCardList.push_back(std::make_shared<Card_Arms_TravelerShield>());
					DeckCardList.push_back(std::make_shared<Card_Arms_TravelerShield>());
					// 旅人の弓 × 3
					DeckCardList.push_back(std::make_shared<Card_Arms_TravelerBow>());
					DeckCardList.push_back(std::make_shared<Card_Arms_TravelerBow>());
					DeckCardList.push_back(std::make_shared<Card_Arms_TravelerBow>());
					// 回復ポーション × 2
					DeckCardList.push_back(std::make_shared<Card_Item_HealingPotion>());
					DeckCardList.push_back(std::make_shared<Card_Item_HealingPotion>());
					// 装備強化スペル × 1
					DeckCardList.push_back(std::make_shared<Card_Spell_ArmsEnhancement>());
					break;

				case 3:
					// 旅人の剣 × 3
					DeckCardList.push_back(std::make_shared<Card_Arms_TravelerSword>());
					DeckCardList.push_back(std::make_shared<Card_Arms_TravelerSword>());
					DeckCardList.push_back(std::make_shared<Card_Arms_TravelerSword>());
					// 旅人の盾 × 3
					DeckCardList.push_back(std::make_shared<Card_Arms_TravelerShield>());
					DeckCardList.push_back(std::make_shared<Card_Arms_TravelerShield>());
					DeckCardList.push_back(std::make_shared<Card_Arms_TravelerShield>());
					// 旅人の弓 × 3
					DeckCardList.push_back(std::make_shared<Card_Arms_TravelerBow>());
					DeckCardList.push_back(std::make_shared<Card_Arms_TravelerBow>());
					DeckCardList.push_back(std::make_shared<Card_Arms_TravelerBow>());
					// 旅人のコンパス × 2
					DeckCardList.push_back(std::make_shared<Card_Item_TravelerCompass>());
					DeckCardList.push_back(std::make_shared<Card_Item_TravelerCompass>());
					// 装備強化スペル × 1
					DeckCardList.push_back(std::make_shared<Card_Spell_ArmsEnhancement>());
					break;
			}
			break;

		// 剣士
		case DataList_SaveData::CLASS_SWORDSMAN:
			switch (Level_Deck)
			{
				case 1:
					// 木の剣×2
					DeckCardList.push_back(std::make_shared<Card_Arms_WoodenSword>());
					DeckCardList.push_back(std::make_shared<Card_Arms_WoodenSword>());
					// 王家の剣×1
					DeckCardList.push_back(std::make_shared<Card_Arms_RoyalSword>());
					// 旅人の剣×1
					DeckCardList.push_back(std::make_shared<Card_Arms_TravelerSword>());
					// 木の盾×2
					DeckCardList.push_back(std::make_shared<Card_Arms_WoodenShield>());
					DeckCardList.push_back(std::make_shared<Card_Arms_WoodenShield>());
					// 王家の盾×1
					DeckCardList.push_back(std::make_shared<Card_Arms_RoyalShield>());
					// 木の弓×2
					DeckCardList.push_back(std::make_shared<Card_Arms_WoodenBow>());
					DeckCardList.push_back(std::make_shared<Card_Arms_WoodenBow>());
					// 装備強化スペル × 1
					DeckCardList.push_back(std::make_shared<Card_Spell_ArmsEnhancement>());
					break;

				case 2:
					// 王家の剣×2
					DeckCardList.push_back(std::make_shared<Card_Arms_RoyalSword>());
					DeckCardList.push_back(std::make_shared<Card_Arms_RoyalSword>());
					// 旅人の剣×1
					DeckCardList.push_back(std::make_shared<Card_Arms_TravelerSword>());
					// ガラスの剣×1
					DeckCardList.push_back(std::make_shared<Card_Arms_GrassSword>());
					// 骨の剣 × 1
					DeckCardList.push_back(std::make_shared<Card_Arms_BoneSword>());
					// 王家の盾×3
					DeckCardList.push_back(std::make_shared<Card_Arms_RoyalShield>());
					DeckCardList.push_back(std::make_shared<Card_Arms_RoyalShield>());
					DeckCardList.push_back(std::make_shared<Card_Arms_RoyalShield>());
					// 王家の弓×2
					DeckCardList.push_back(std::make_shared<Card_Arms_RoyalBow>());
					DeckCardList.push_back(std::make_shared<Card_Arms_RoyalBow>());
					// 装備強化スペル × 1
					DeckCardList.push_back(std::make_shared<Card_Spell_ArmsEnhancement>());
					break;

				case 3:
					// 王家の剣×2
					DeckCardList.push_back(std::make_shared<Card_Arms_RoyalSword>());
					DeckCardList.push_back(std::make_shared<Card_Arms_RoyalSword>());
					// 旅人の剣×1
					DeckCardList.push_back(std::make_shared<Card_Arms_TravelerSword>());
					// 骨の剣 × 1
					DeckCardList.push_back(std::make_shared<Card_Arms_BoneSword>());
					// 王家の盾×3
					DeckCardList.push_back(std::make_shared<Card_Arms_RoyalShield>());
					DeckCardList.push_back(std::make_shared<Card_Arms_RoyalShield>());
					DeckCardList.push_back(std::make_shared<Card_Arms_RoyalShield>());
					// 王家の弓×2
					DeckCardList.push_back(std::make_shared<Card_Arms_RoyalBow>());
					DeckCardList.push_back(std::make_shared<Card_Arms_RoyalBow>());
					DeckCardList.push_back(std::make_shared<Card_Arms_RoyalBow>());
					// 剣の加護 × 1
					DeckCardList.push_back(std::make_shared<Card_Spell_BlessingSword>());
					// 装備強化スペル × 1
					DeckCardList.push_back(std::make_shared<Card_Spell_ArmsEnhancement>());
					break;
			}
			break;

		// 魔法使い
		case DataList_SaveData::CLASS_WIZARD:
			switch (Level_Deck)
			{
				case 1:
					// 木の剣×2
					DeckCardList.push_back(std::make_shared<Card_Arms_WoodenSword>());
					DeckCardList.push_back(std::make_shared<Card_Arms_WoodenSword>());
					// 木の盾×2
					DeckCardList.push_back(std::make_shared<Card_Arms_WoodenShield>());
					DeckCardList.push_back(std::make_shared<Card_Arms_WoodenShield>());
					// 木の弓×2
					DeckCardList.push_back(std::make_shared<Card_Arms_WoodenBow>());
					DeckCardList.push_back(std::make_shared<Card_Arms_WoodenBow>());
					// 火の魔法 × 1
					DeckCardList.push_back(std::make_shared<Card_Spell_FireMagic>());
					// 風の魔法 × 1
					DeckCardList.push_back(std::make_shared<Card_Spell_WindMagic>());
					// 水の魔法 × 1
					DeckCardList.push_back(std::make_shared<Card_Spell_WaterMagic>());
					// 土の魔法 × 1
					DeckCardList.push_back(std::make_shared<Card_Spell_EarthMagic>());
					break;

				case 2:
					// 回復ポーション × 2
					DeckCardList.push_back(std::make_shared<Card_Item_HealingPotion>());
					DeckCardList.push_back(std::make_shared<Card_Item_HealingPotion>());
					// 火の魔法 × 2
					DeckCardList.push_back(std::make_shared<Card_Spell_FireMagic>());
					DeckCardList.push_back(std::make_shared<Card_Spell_FireMagic>());
					// 風の魔法 × 2
					DeckCardList.push_back(std::make_shared<Card_Spell_WindMagic>());
					DeckCardList.push_back(std::make_shared<Card_Spell_WindMagic>());
					// 水の魔法 × 2
					DeckCardList.push_back(std::make_shared<Card_Spell_WaterMagic>());
					DeckCardList.push_back(std::make_shared<Card_Spell_WaterMagic>());
					// 土の魔法 × 2
					DeckCardList.push_back(std::make_shared<Card_Spell_EarthMagic>());
					DeckCardList.push_back(std::make_shared<Card_Spell_EarthMagic>());
					// 装備強化スペル × 2
					DeckCardList.push_back(std::make_shared<Card_Spell_ArmsEnhancement>());
					break;

				case 3:
					// 回復ポーション × 1
					DeckCardList.push_back(std::make_shared<Card_Item_HealingPotion>());
					// 守護のポーション × 1
					DeckCardList.push_back(std::make_shared<Card_Item_GuardianPotion>());
					// 力のポーション × 1
					DeckCardList.push_back(std::make_shared<Card_Item_PowerPotion>());
					// 火の魔法 × 3
					DeckCardList.push_back(std::make_shared<Card_Spell_FireMagic>());
					DeckCardList.push_back(std::make_shared<Card_Spell_FireMagic>());
					DeckCardList.push_back(std::make_shared<Card_Spell_FireMagic>());
					// 風の魔法 × 3
					DeckCardList.push_back(std::make_shared<Card_Spell_WindMagic>());
					DeckCardList.push_back(std::make_shared<Card_Spell_WindMagic>());
					DeckCardList.push_back(std::make_shared<Card_Spell_WindMagic>());
					// 水の魔法 × 3
					DeckCardList.push_back(std::make_shared<Card_Spell_WaterMagic>());
					DeckCardList.push_back(std::make_shared<Card_Spell_WaterMagic>());
					DeckCardList.push_back(std::make_shared<Card_Spell_WaterMagic>());
					// 土の魔法 × 3
					DeckCardList.push_back(std::make_shared<Card_Spell_EarthMagic>());
					DeckCardList.push_back(std::make_shared<Card_Spell_EarthMagic>());
					DeckCardList.push_back(std::make_shared<Card_Spell_EarthMagic>());
					break;
			}
			break;

		// 盗賊
		case DataList_SaveData::CLASS_ROGUE:
			switch (Level_Deck)
			{
				case 1:
					// 木の剣×2
					DeckCardList.push_back(std::make_shared<Card_Arms_WoodenSword>());
					DeckCardList.push_back(std::make_shared<Card_Arms_WoodenSword>());
					// 毒のナイフ×2
					DeckCardList.push_back(std::make_shared<Card_Arms_PoisonDagger>());
					DeckCardList.push_back(std::make_shared<Card_Arms_PoisonDagger>());
					// 木の盾×3
					DeckCardList.push_back(std::make_shared<Card_Arms_WoodenShield>());
					DeckCardList.push_back(std::make_shared<Card_Arms_WoodenShield>());
					DeckCardList.push_back(std::make_shared<Card_Arms_WoodenShield>());
					// 木の弓×2
					DeckCardList.push_back(std::make_shared<Card_Arms_WoodenBow>());
					DeckCardList.push_back(std::make_shared<Card_Arms_WoodenBow>());
					// 回復のポーション × 1
					DeckCardList.push_back(std::make_shared<Card_Item_HealingPotion>());
					// 装備強化スペル × 1
					DeckCardList.push_back(std::make_shared<Card_Spell_ArmsEnhancement>());
					break;

				case 2:
					// 毒のナイフ×3
					DeckCardList.push_back(std::make_shared<Card_Arms_PoisonDagger>());
					DeckCardList.push_back(std::make_shared<Card_Arms_PoisonDagger>());
					DeckCardList.push_back(std::make_shared<Card_Arms_PoisonDagger>());
					// 王国の盾×3
					DeckCardList.push_back(std::make_shared<Card_Arms_RoyalShield>());
					DeckCardList.push_back(std::make_shared<Card_Arms_RoyalShield>());
					DeckCardList.push_back(std::make_shared<Card_Arms_RoyalShield>());
					// 王国の弓×2
					DeckCardList.push_back(std::make_shared<Card_Arms_RoyalBow>());
					DeckCardList.push_back(std::make_shared<Card_Arms_RoyalBow>());
					// 回復のポーション × 1
					DeckCardList.push_back(std::make_shared<Card_Item_HealingPotion>());
					// 毒のポーション × 1
					DeckCardList.push_back(std::make_shared<Card_Item_PoisonPotion>());
					// 装備強化スペル
					DeckCardList.push_back(std::make_shared<Card_Spell_ArmsEnhancement>());
					break;

				case 3:
					// 毒のナイフ×3
					DeckCardList.push_back(std::make_shared<Card_Arms_PoisonDagger>());
					DeckCardList.push_back(std::make_shared<Card_Arms_PoisonDagger>());
					DeckCardList.push_back(std::make_shared<Card_Arms_PoisonDagger>());
					// ガラスのナイフ×3
					DeckCardList.push_back(std::make_shared<Card_Arms_GrassDagger>());
					DeckCardList.push_back(std::make_shared<Card_Arms_GrassDagger>());
					DeckCardList.push_back(std::make_shared<Card_Arms_GrassDagger>());
					// 王国の盾×3
					DeckCardList.push_back(std::make_shared<Card_Arms_RoyalShield>());
					DeckCardList.push_back(std::make_shared<Card_Arms_RoyalShield>());
					DeckCardList.push_back(std::make_shared<Card_Arms_RoyalShield>());
					// 回復のポーション × 1
					DeckCardList.push_back(std::make_shared<Card_Item_HealingPotion>());
					// 毒のポーション × 3
					DeckCardList.push_back(std::make_shared<Card_Item_PoisonPotion>());
					DeckCardList.push_back(std::make_shared<Card_Item_PoisonPotion>());
					DeckCardList.push_back(std::make_shared<Card_Item_PoisonPotion>());
					// 装備強化スペル
					DeckCardList.push_back(std::make_shared<Card_Spell_ArmsEnhancement>());
					break;
			}
			break;

		// 商人
		case DataList_SaveData::CLASS_MARCHANT:
			switch (Level_Deck)
			{
				case 1:
					// 木の剣×2
					DeckCardList.push_back(std::make_shared<Card_Arms_WoodenSword>());
					DeckCardList.push_back(std::make_shared<Card_Arms_WoodenSword>());
					// 金の剣×1
					DeckCardList.push_back(std::make_shared<Card_Arms_GoldSword>());
					// 木の盾×2
					DeckCardList.push_back(std::make_shared<Card_Arms_WoodenShield>());
					DeckCardList.push_back(std::make_shared<Card_Arms_WoodenShield>());
					// 王家の盾×1
					DeckCardList.push_back(std::make_shared<Card_Arms_TravelerShield>());
					// 回復のポーション × 1
					DeckCardList.push_back(std::make_shared<Card_Item_HealingPotion>());
					// 守護のポーション × 1
					DeckCardList.push_back(std::make_shared<Card_Item_GuardianPotion>());
					// 力のポーション × 1
					DeckCardList.push_back(std::make_shared<Card_Item_PowerPotion>());
					break;

				case 2:
					// 金の剣×3
					DeckCardList.push_back(std::make_shared<Card_Arms_GoldSword>());
					DeckCardList.push_back(std::make_shared<Card_Arms_GoldSword>());
					DeckCardList.push_back(std::make_shared<Card_Arms_GoldSword>());
					// 金のナイフ×3
					DeckCardList.push_back(std::make_shared<Card_Arms_GoldDagger>());
					DeckCardList.push_back(std::make_shared<Card_Arms_GoldDagger>());
					DeckCardList.push_back(std::make_shared<Card_Arms_GoldDagger>());
					// 王家の盾×2
					DeckCardList.push_back(std::make_shared<Card_Arms_TravelerShield>());
					DeckCardList.push_back(std::make_shared<Card_Arms_TravelerShield>());
					// 王家の弓×2
					DeckCardList.push_back(std::make_shared<Card_Arms_RoyalBow>());
					DeckCardList.push_back(std::make_shared<Card_Arms_TravelerBow>());
					// 攻撃のポーション × 1
					DeckCardList.push_back(std::make_shared<Card_Item_HealingPotion>());
					// 守護のポーション × 1
					DeckCardList.push_back(std::make_shared<Card_Item_GuardianPotion>());
					// 力のポーション × 1
					DeckCardList.push_back(std::make_shared<Card_Item_PowerPotion>());
					break;

				case 3:
					// 金の剣×3
					DeckCardList.push_back(std::make_shared<Card_Arms_GoldSword>());
					DeckCardList.push_back(std::make_shared<Card_Arms_GoldSword>());
					DeckCardList.push_back(std::make_shared<Card_Arms_GoldSword>());
					// 金のナイフ×3
					DeckCardList.push_back(std::make_shared<Card_Arms_GoldDagger>());
					DeckCardList.push_back(std::make_shared<Card_Arms_GoldDagger>());
					DeckCardList.push_back(std::make_shared<Card_Arms_GoldDagger>());
					// 王家の盾×2
					DeckCardList.push_back(std::make_shared<Card_Arms_TravelerShield>());
					DeckCardList.push_back(std::make_shared<Card_Arms_TravelerShield>());
					// 回復のポーション × 2
					DeckCardList.push_back(std::make_shared<Card_Item_HealingPotion>());
					DeckCardList.push_back(std::make_shared<Card_Item_HealingPotion>());
					// 守護のポーション × 2
					DeckCardList.push_back(std::make_shared<Card_Item_GuardianPotion>());
					DeckCardList.push_back(std::make_shared<Card_Item_GuardianPotion>());
					// 力のポーション × 2
					DeckCardList.push_back(std::make_shared<Card_Item_PowerPotion>());
					DeckCardList.push_back(std::make_shared<Card_Item_PowerPotion>());
					// 王家の旗 × 2
					DeckCardList.push_back(std::make_shared<Card_Item_RoyalFlag>());
					DeckCardList.push_back(std::make_shared<Card_Item_RoyalFlag>());
					break;
			}
			break;
	}
}