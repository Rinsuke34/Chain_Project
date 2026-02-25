/* "修羅刀"カードの定義 */

/* 使用する要素のインクルード */
// ヘッダファイル
#include "Card_Arms_AsuraKatana.h"
// 関連クラス
#include "Character_Base.h"
#include "DataList_Battle.h"

// コンストラクタ
Card_Arms_AsuraKatana::Card_Arms_AsuraKatana() : Card_Arms_Base()
{
	/* カード情報の設定 */
	// レアリティ
	this->iRarity	= RARITY_EPIC;
	// カード名
	this->Name		= "修羅刀";
	// スートリスト
	this->Suite_List.push_back(SUITE_SWORD);	// 剣
	// ステータス
	this->Strength		= 0;					// 攻撃力
	this->Diffence		= 0;					// 防御力
	this->AttackRange	= ATTACKRANGE_FRONT;	// 攻撃範囲
	// 画像の名前
	this->ImageName		= "AsuraKatana";		// 画像の名前
	// 説明文
	this->ExplanationText = "/cysターンかいしじ/ce/nこうげきりょくをXにする\nXはたおしたてきのかず";
	// 行動前後の効果を実行するかのフラグ
	this->Effect_Action_Before_Flg	= false;
	this->Effect_Action_After_Flg	= true;
	// その他
	this->Kill_Count = 0;	// 撃破数
	for (int i = 0; i < DataList_Battle::POSITION_MAX; i++)	this->LiveFlg[i] = false;	// 生存フラグ
}

// 効果発動:戦闘開始時
void Card_Arms_AsuraKatana::Effect_BattleStart()
{
	/* 敵の生存フラグを初期化 */
	for (int i = 0; i < DataList_Battle::POSITION_MAX; i++)
	{
		if (this->pDataList_Battle->GetEnemyCharacter(i))
		{
			this->LiveFlg[i] = true;
		}
		else
		{
			this->LiveFlg[i] = false;
		}
	}
}

// 効果発動::敵の死亡確認時
void Card_Arms_AsuraKatana::Effect_EnemyDeathCheck()
{
	/* 死亡カウントを更新 */
	for (int i = 0; i < DataList_Battle::POSITION_MAX; i++)
	{
		if (!this->pDataList_Battle->GetEnemyCharacter(i))
		{
			// 敵が存在しないなら
			if (this->LiveFlg[i])
			{
				// 前のターンでは生存していたなら
				this->Kill_Count++;	// 撃破数を加算
				this->LiveFlg[i] = false;	// 生存フラグを更新
			}
		}
		else
		{
			// 敵が存在するなら
			this->LiveFlg[i] = true;	// 生存フラグを更新
		}
	}

	/* 基本攻撃力を撃破数の値に設定 */
	this->Strength = this->Kill_Count;

	/* 敵の撃破数が20を超えているか確認 */
	if (this->Kill_Count >= 20)
	{
		// 超えているなら
		/* 画像名を変更する */
		this->ImageName = "SuperAsuraKatana";	// 画像の名前

		/* 画像の更新 */
		this->UpdateImage();
	}
}