/* "金のナイフ"カードの定義 */

/* 使用する要素のインクルード */
// ヘッダファイル
#include "Card_Arms_GoldDagger.h"
// 関連クラス
#include "DataList_GameResource.h"

// コンストラクタ
Card_Arms_GoldDagger::Card_Arms_GoldDagger() : Card_Arms_Base()
{
	/* カード情報の設定 */
	// レアリティ
	this->iRarity = RARITY_RARE;
	// カード名
	this->Name = "金の剣";
	// スートリスト
	this->Suite_List.push_back(SUITE_DAGGER);	// 短剣
	this->Suite_List.push_back(SUITE_GOLD);		// 金
	// ステータス
	this->Strength	= 8;					// 攻撃力
	this->Diffence	= 0;					// 防御力
	this->AttackRange = ATTACKRANGE_FRONT;	// 攻撃範囲
	// 画像の名前
	this->ImageName = "GoldDagger";		// 画像の名前
	// 説明文
	this->ExplanationText = "いちばんまえのてきをこうげきする/n/cysターンかいしじ/ce/nこうげきりょくをXにする\nXはしょじコインのまいすう+８";
	// 行動前後の効果を実行するかのフラグ
	this->Effect_Action_Before_Flg	= false;
	this->Effect_Action_After_Flg	= false;
}

// 効果発動:ターン開始時
void Card_Arms_GoldDagger::Effect_StartTurn()
{
	/* データリストを取得 */
	std::shared_ptr<DataList_GameResource> GameResource = std::dynamic_pointer_cast<DataList_GameResource>(gpDataListServer->GetDataList("DataList_GameResource"));

	/* 攻撃力を所持しているコイン数にする */
	this->Strength =  8 + GameResource->GetHaveCoin();	// 攻撃力 = コイン数

	/* 永続的に強化されているなら、ステータスを水色で描写するようにする */
	if (this->Strength > 8)
	{
		this->StatusDrawBaseColor[0] = GetColor(0, 255, 255);	// 水色
	}

	/* 画像の更新 */
	this->UpdateImage();
}
