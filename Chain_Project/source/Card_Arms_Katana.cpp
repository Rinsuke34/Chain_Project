/* "打刀"カードの定義 */

/* 使用する要素のインクルード */
// ヘッダファイル
#include "Card_Arms_Katana.h"
// 関連クラス
#include "DataList_Battle.h"
#include "Card_Arms_CursedKatana.h"

// コンストラクタ
Card_Arms_Katana::Card_Arms_Katana() : Card_Arms_Base()
{
	/* カード情報の設定 */
	// レアリティ
	this->iRarity = RARITY_RARE;
	// カード名
	this->Name = "打刀";
	// スートリスト
	this->Suite_List.push_back(SUITE_SWORD);	// 剣
	// ステータス
	this->Strength		= 20;					// 攻撃力
	this->Diffence		= 0;					// 防御力
	this->AttackRange	= ATTACKRANGE_FRONT;	// 攻撃範囲
	// 画像の名前
	this->ImageName		= "Katana";				// 画像の名前
	// 説明文
	this->ExplanationText = "いちばんまえのてきをこうげきする/nてきをあと２０たいたおすとしんかする";
	// その他
	this->Kill_Count = 0;	// 撃破数
	for (int i = 0; i < DataList_Battle::POSITION_MAX; i++)	this->LiveFlg[i] = false;	// 生存フラグ
}

// 効果発動:戦闘開始時
void Card_Arms_Katana::Effect_BattleStart()
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
void Card_Arms_Katana::Effect_EnemyDeathCheck()
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

				/* テキスト更新 */
				this->ExplanationText = "てきをあと" + std::to_string(20 - this->Kill_Count) + "たいたおすとしんか";

				/* 画像更新 */
				this->UpdateImage();
			}
		}
		else
		{
			// 敵が存在するなら
			this->LiveFlg[i] = true;	// 生存フラグを更新
		}
	}

	/* 敵の撃破数が20を超えているか確認 */
	if (this->Kill_Count >= 20)
	{
		// 超えているなら
		/* このカードのロストフラグを有効化 */
		this->bLostFlag = true;

		/* 妖刀を手札に追加する */
		std::shared_ptr<Card_Arms_CursedKatana> New_Cursed_Katana = std::make_shared<Card_Arms_CursedKatana>();

		// 手札に加える
		this->pDataList_Battle->AddHandCard(New_Cursed_Katana);
		New_Cursed_Katana->SetCardState(Card_Base::CARDSTATE_HAND);

		/* 現在の全カードリストを取得 */
		std::vector<std::shared_ptr<Card_Base>> AllDeckCardList = this->pDataList_Battle->GetAllDeckCardList();

		/* 現在の全カードリストに妖刀を追加する */
		AllDeckCardList.push_back(New_Cursed_Katana);

		/* 更新した全カードリストを設定する */
		this->pDataList_Battle->SetAllDeckCardList(AllDeckCardList);
	}
}
