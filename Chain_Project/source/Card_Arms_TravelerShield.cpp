/* "旅人の盾"カードの定義 */

/* 使用する要素のインクルード */
// ヘッダファイル
#include "Card_Arms_TravelerShield.h"

// コンストラクタ
Card_Arms_TravelerShield::Card_Arms_TravelerShield() : Card_Arms_Base()
{
	/* カード情報の設定 */
	// レアリティ
	this->iRarity = RARITY_COMMON;
	// カード名
	this->Name = "旅人の盾";
	// スートリスト
	this->Suite_List.push_back(SUITE_SHIELD);	// 盾
	this->Suite_List.push_back(SUITE_TRAVELER);	// 旅人
	// ステータス
	this->Strength	= 0;					// 攻撃力
	this->Diffence	= 3;					// 防御力
	this->AttackRange = ATTACKRANGE_FRONT;	// 攻撃範囲
	// 画像の名前
	this->ImageName = "TravelerShield";		// 画像の名前
	// 説明文
	this->ExplanationText = "/cysこうどうまえ/ce/nぼうぎょりょくをえいぞくてきにきょうかする/n/cysこうかりょう/ce/nチェインすう";
	// 行動前後の効果を実行するかのフラグ
	this->Effect_Action_Before_Flg	= true;
	this->Effect_Action_After_Flg	= false;

	/* その他の変数の初期化 */
	this->PermanenceBuff_Def	= 0;		// 防御力の永続バフ
}

// 効果発動:行動直前
void Card_Arms_TravelerShield::Effect_Action_Before(std::shared_ptr<Character_Base> Target)
{
	// 引数
	// Target : 効果対象のキャラクター

	/* チェイン数を取得 */
	int ChainCount = MyChainCountGet_Buff();

	/* 防御力を永続的に加算 */
	this->PermanenceBuff_Def += ChainCount;

	/* 基本防御力を設定 */
	this->Diffence += ChainCount;

	/* 永続的に強化されているなら、ステータスを水色で描写するようにする */
	if (this->PermanenceBuff_Def > 0)
	{
		this->StatusDrawBaseColor[1] = GetColor(0, 255, 255);	// 水色
	}

	/* 画像の更新 */
	this->UpdateImage();
}
