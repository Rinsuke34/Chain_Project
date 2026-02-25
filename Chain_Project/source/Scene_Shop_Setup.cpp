/* シーン"ショップ画面"の定義(ラインナップのセットアップ部分) */

/* 使用する要素のインクルード */
// ヘッダファイル
#include "Scene_Shop.h"
// 関連クラス
#include "Card_Include.h"

// ショップ設定
void Scene_Shop::ShopSetup(int Level)
{
	// 引数
	// Level <- ショップのラインナップの種類の判別に使用

	/* ラインナップ設定処理 */
	std::shared_ptr<Card_Base> AddCard		= nullptr;
	std::shared_ptr<Shop_Item> pShopItem	= nullptr;
	switch (Level)
	{
		// ステージ1
		case 0:
			AddCard		= std::make_shared<Card_Arms_TravelerSword>();
			pShopItem	= std::make_shared<Shop_Item>(AddCard, 10);
			this->ShopItemList.push_back(pShopItem);

			AddCard		= std::make_shared<Card_Arms_RoyalSword>();
			pShopItem	= std::make_shared<Shop_Item>(AddCard, 10);
			this->ShopItemList.push_back(pShopItem);

			AddCard		= std::make_shared<Card_Arms_GrassSword>();
			pShopItem	= std::make_shared<Shop_Item>(AddCard, 10);
			this->ShopItemList.push_back(pShopItem);
			break;

		case 1:
			AddCard		= std::make_shared<Card_Arms_TravelerSword>();
			pShopItem	= std::make_shared<Shop_Item>(AddCard, 10);
			this->ShopItemList.push_back(pShopItem);

			AddCard = std::make_shared<Card_Arms_TravelerShield>();
			pShopItem = std::make_shared<Shop_Item>(AddCard, 10);
			this->ShopItemList.push_back(pShopItem);

			AddCard = std::make_shared<Card_Item_HealingPotion>();
			pShopItem = std::make_shared<Shop_Item>(AddCard, 15);
			this->ShopItemList.push_back(pShopItem);

			AddCard = std::make_shared<Card_Arms_GrassSword>();
			pShopItem = std::make_shared<Shop_Item>(AddCard, 20);
			this->ShopItemList.push_back(pShopItem);
			break;

		// ステージ2
		case 10:
			AddCard = std::make_shared<Card_Item_HealingPotion>();
			pShopItem = std::make_shared<Shop_Item>(AddCard, 15);
			this->ShopItemList.push_back(pShopItem);

			AddCard = std::make_shared<Card_Arms_GrassSword>();
			pShopItem = std::make_shared<Shop_Item>(AddCard, 20);
			this->ShopItemList.push_back(pShopItem);

			AddCard = std::make_shared<Card_Item_TravelerCompass>();
			pShopItem = std::make_shared<Shop_Item>(AddCard, 30);
			this->ShopItemList.push_back(pShopItem);
			break;

		case 11:
			AddCard = std::make_shared<Card_Item_HealingPotion>();
			pShopItem = std::make_shared<Shop_Item>(AddCard, 15);
			this->ShopItemList.push_back(pShopItem);

			AddCard = std::make_shared<Card_Arms_GrassSword>();
			pShopItem = std::make_shared<Shop_Item>(AddCard, 20);
			this->ShopItemList.push_back(pShopItem);

			AddCard = std::make_shared<Card_Item_RoyalFlag>();
			pShopItem = std::make_shared<Shop_Item>(AddCard, 30);
			this->ShopItemList.push_back(pShopItem);
			break;

		// ステージ3
		case 20:
			AddCard = std::make_shared<Card_Spell_ArmsEnhancement>();
			pShopItem = std::make_shared<Shop_Item>(AddCard, 30);
			this->ShopItemList.push_back(pShopItem);

			AddCard = std::make_shared<Card_Item_RoyalFlag>();
			pShopItem = std::make_shared<Shop_Item>(AddCard, 30);
			this->ShopItemList.push_back(pShopItem);

			AddCard = std::make_shared<Card_Item_TravelerCompass>();
			pShopItem = std::make_shared<Shop_Item>(AddCard, 30);
			this->ShopItemList.push_back(pShopItem);
			break;

		case 21:
			AddCard = std::make_shared<Card_Item_HealingPotion>();
			pShopItem = std::make_shared<Shop_Item>(AddCard, 15);
			this->ShopItemList.push_back(pShopItem);

			AddCard = std::make_shared<Card_Item_PoisonPotion>();
			pShopItem = std::make_shared<Shop_Item>(AddCard, 15);
			this->ShopItemList.push_back(pShopItem);

			AddCard = std::make_shared<Card_Item_PowerPotion>();
			pShopItem = std::make_shared<Shop_Item>(AddCard, 15);
			this->ShopItemList.push_back(pShopItem);

			AddCard = std::make_shared<Card_Item_GuardianPotion>();
			pShopItem = std::make_shared<Shop_Item>(AddCard, 15);
			this->ShopItemList.push_back(pShopItem);
			break;
	}

	/* カードの初期設定 */
	for (auto& Item : this->ShopItemList)
	{
		std::shared_ptr<Card_Base> AddCard = Item->GetCard();

		/* 画像更新 */
		AddCard->UpdateImage();

		/* 陳列中状態に設定 */
		AddCard->SetCardState(Card_Base::CARDSTATE_SHOP_DISPLAY);
	}

	/* 描写座標の更新 */
	CardPosition_Setup();
}
