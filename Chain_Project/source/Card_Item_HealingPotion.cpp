/* "回復のポーション"カードの定義 */

/* 使用する要素のインクルード */
// ヘッダファイル
#include "Card_Item_HealingPotion.h"
// 関連クラス	
#include "DataList_Battle.h"
#include "Character_Base.h"
#include "Action_Effect.h"

// コンストラクタ
Card_Item_HealingPotion::Card_Item_HealingPotion() : Card_Item_Base()
{
	/* カード情報の設定 */
	// レアリティ
	this->iRarity = RARITY_COMMON;
	// カード名
	this->Name = "回復薬";
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
	// 回復を設定する
	std::shared_ptr<Action_Effect_Heal> addEffect	= std::make_shared<Action_Effect_Heal>();
	addEffect->Target_Camp						= Character_Base::CAMP_FRIEND;		// 効果対象の陣営:仲間
	addEffect->Target_Position					= 0;								// 効果対象の立ち位置:無効
	addEffect->HealAmount						= 5 + this->iNowChainCount * 5;		// 回復量:5 + (チェイン数 * 5)
	addEffect->EffectUser						= this->pPlayer;					// 効果の使用者:プレイヤーキャラクター
	addEffect->AllRange							= true;								// 全体に効果を与える
	addEffect->Priority							= 30;								// 低め
	this->pDataList_Battle->AddEffect(addEffect, GetMyAreaNo());

	/* ロストフラグを有効化 */
	this->bLostFlag = true;
}
