/* "硝子の短剣"カードの定義 */

/* 使用する要素のインクルード */
// ヘッダファイル
#include "Card_Arms_GrassDagger.h"

// コンストラクタ
Card_Arms_GrassDagger::Card_Arms_GrassDagger() : Card_Arms_Base()
{
	/* カード情報の設定 */
	// レアリティ
	this->iRarity = RARITY_RARE;
	// カード名
	this->Name	= "硝子の短剣";
	// スートリスト
	this->Suite_List.push_back(SUITE_DAGGER);	// 短剣
	this->Suite_List.push_back(SUITE_GRASS);	// ガラス
	// ステータス
	this->Strength		= 20;					// 攻撃力
	this->Diffence		= 0;					// 防御力
	this->AttackRange	= ATTACKRANGE_FRONT;	// 攻撃範囲
	// 画像の名前
	this->ImageName		= "GrassDagger";		// 画像の名前
	// 説明文
	this->ExplanationText = "";
}

// 効果発動:行動直後
void Card_Arms_GrassDagger::Effect_Action_After(std::shared_ptr<Character_Base> Target)
{
	// 引数
	// Target : 効果対象のキャラクター

	/* 破壊確率を計算する */
	int Break_Probability = 25 - (GetNowChainCount() * 5);	// 破壊確率 = 25 - (チェイン数 * 5)

	/* 破壊確率に基づいてカードが破壊されるか確認する */
	int RandomValue = rand() % 100;	// 0から99の乱数を生成
	if (RandomValue < Break_Probability)
	{
		// 破壊されるなら
		this->SetLostFlag(true);	// ロストフラグを立てる
	}
}
