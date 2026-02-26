/* "旅人の弓"カードの定義 */

/* 使用する要素のインクルード */
// ヘッダファイル
#include "Card_Arms_TravelerBow.h"

// コンストラクタ
Card_Arms_TravelerBow::Card_Arms_TravelerBow() : Card_Arms_Base()
{
	/* カード情報の設定 */
	// レアリティ
	this->iRarity = RARITY_COMMON;
	// カード名
	this->Name = "旅人の弓";
	// スートリスト
	this->Suite_List.push_back(SUITE_BOW);		// 弓
	this->Suite_List.push_back(SUITE_TRAVELER);	// 旅人
	// ステータス
	this->Strength = 5;						// 攻撃力
	this->Diffence = 0;						// 防御力
	this->AttackRange = ATTACKRANGE_RANDOM;	// 攻撃範囲
	// 画像の名前
	this->ImageName = "TravelerBow";		// 画像の名前
	// 説明文
	this->ExplanationText = "ランダムなあいてをこうげきする/n/cysこうどうまえ/ce/nこうげきりょくをえいぞくてきに+Xする/nXはチェイン";
	// 行動前後の効果を実行するかのフラグ
	this->Effect_Action_Before_Flg	= true;
	this->Effect_Action_After_Flg	= false;

	/* その他の変数の初期化 */
	this->PermanenceBuff_Atk	= 0;
}

// 効果発動:行動直前
void Card_Arms_TravelerBow::Effect_Action_Before(std::shared_ptr<Character_Base> Target)
{
	// 引数
	// Target : 効果対象のキャラクター

	/* チェイン数を取得 */
	int ChainCount = MyChainCountGet_Buff();

	/* 攻撃力を永続的に加算 */
	this->PermanenceBuff_Atk += ChainCount;

	/* 基本攻撃力を設定 */
	this->Strength += ChainCount;

	/* 永続的に強化されているなら、ステータスを水色で描写するようにする */
	if (this->PermanenceBuff_Atk > 0)
	{
		this->StatusDrawBaseColor[0] = GetColor(0, 255, 255);	// 水色
	}

	/* 画像の更新 */
	this->UpdateImage();
}
