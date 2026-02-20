/* "旅人の剣"カードの定義 */

/* 使用する要素のインクルード */
// ヘッダファイル
#include "Card_Arms_TravelerSword.h"

// コンストラクタ
Card_Arms_TravelerSword::Card_Arms_TravelerSword() : Card_Arms_Base()
{
	/* カード情報の設定 */
	// レアリティ
	this->iRarity = RARITY_COMMON;
	// カード名
	this->Name = "旅人の剣";
	// スートリスト
	this->Suite_List.push_back(SUITE_SWORD);	// 剣
	this->Suite_List.push_back(SUITE_TRAVELER);	// 旅人
	// ステータス
	this->Strength		= 3;					// 攻撃力
	this->Diffence		= 0;					// 防御力
	this->AttackRange	= ATTACKRANGE_FRONT;	// 攻撃範囲
	// 画像の名前
	this->ImageName = "TravelerSword";		// 画像の名前
	// 説明文
	this->ExplanationText = "/cysこうどうまえ/ce/nこうげきりょくをえいぞくてきにきょうかする/n/cysこうかりょう/ce/nチェインすう";
	// 行動前後の効果を実行するかのフラグ
	this->Effect_Action_Before_Flg	= true;
	this->Effect_Action_After_Flg	= false;

	/* その他の変数の初期化 */
	this->PermanenceBuff_Atk = 0;
}

// 効果発動:行動直前
void Card_Arms_TravelerSword::Effect_Action_Before(std::shared_ptr<Character_Base> Target)
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
	UpdateImage();
}
