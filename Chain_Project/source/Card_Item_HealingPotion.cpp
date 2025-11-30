/* "回復のポーション"カードの定義 */

/* 使用する要素のインクルード */
// ヘッダファイル
#include "Card_Item_HealingPotion.h"
// 共通定義
#include "StructDefine.h"
// 関連クラス	
#include "DataList_Battle.h"
#include "Character_Base.h"

// コンストラクタ
Card_Item_HealingPotion::Card_Item_HealingPotion() : Card_Item_Base()
{
	/* カード情報の設定 */
	// レアリティ
	this->iRarity = RARITY_COMMON;
	// カード名
	this->Name = "回復のポーション";
	// スートリスト
	this->Suite_List.push_back(SUITE_POTION);	// 薬(ポーション)
	// 画像の名前
	this->ImageName = "HealingPotion";			// 画像の名前
}

// 効果発動:行動開始時
void Card_Item_HealingPotion::Effect_StartAction()
{
	/* プレイヤーを取得できていないならプレイヤーを取得する */
	CheckHavePlayer();

	/* 自陣営の全キャラクターに回復を行う */
	for (int i = 0; i < DataList_Battle::POSITION_MAX; i++)
	{
		/* 仲間キャラクターが存在するか確認 */
		std::shared_ptr<Character_Base> pFriendCharacter = this->pDataList_Battle->GetFriendCharacter(i);
		if (pFriendCharacter != nullptr)
		{
			/* 存在するなら */
			// 回復を設定する
			std::shared_ptr<Card_Effect_Heal> addEffect = std::make_shared<Card_Effect_Heal>();
			addEffect->Target_Camp		= Character_Base::CAMP_FRIEND;		// 効果対象の陣営:仲間
			addEffect->Target_Position	= i;								// 効果対象の立ち位置:確認した仲間キャラクターの位置
			addEffect->HealAmount		= 20 + this->iNowChainCount * 10;	// 回復量:20 + (チェイン数 * 10)
			addEffect->EffectUser		= this->pPlayer;					// 効果の使用者:プレイヤーキャラクター
			this->pDataList_Battle->AddEffect(addEffect, GetMyAreaNo());
		}
	}

	/* ロストフラグを有効化 */
	this->bLostFlag = true;
}
