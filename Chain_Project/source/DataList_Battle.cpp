/* データリスト"バトル用データ管理"クラスの定義 */

/* 使用する要素のインクルード */
// ヘッダファイル
#include "DataList_Battle.h"
// 関連クラス
#include "Card_Base.h"

// コンストラクタ
DataList_Battle::DataList_Battle() : DataList_Base("DataList_Battle")
{
	
}

// カード関係(総括用)
// カードリストにカードを追加
void DataList_Battle::AddCard(const std::shared_ptr<Card_Base>& pCard)
{
	// 引数
	// pCard <- 追加するカードのポインタ
	this->AllDeckCardList.push_back(pCard);
}

// ロストカードリストにカードを追加
void DataList_Battle::AddLostCard(const std::shared_ptr<Card_Base>& pCard)
{
	// 引数
	// pCard <- 追加するカードのポインタ
	this->AllLostCardList.push_back(pCard);
}

// 全カードリストからカードを削除
void DataList_Battle::RemoveAllCard(const std::shared_ptr<Card_Base>& pCard)
{
	// 引数
	// pCard <- 追加するカードのポインタ
	this->AllDeckCardList.erase(std::remove(AllDeckCardList.begin(), AllDeckCardList.end(), pCard), AllDeckCardList.end());
}

// ロストカードリストからカードを削除
void DataList_Battle::RemoveLostCard(const std::shared_ptr<Card_Base>& pCard)
{
	// 引数
	// pCard <- 追加するカードのポインタ
	this->AllLostCardList.erase(std::remove(AllLostCardList.begin(), AllLostCardList.end(), pCard), AllLostCardList.end());
}

// カード関連(戦闘用)
// デッキにカードを追加
void DataList_Battle::AddDeckCard(const std::shared_ptr<Card_Base>& pCard)
{
	// 引数
	// pCard <- 追加するカードのポインタ
	this->DeckCardList.push_back(pCard);
}

// 手札にカードを追加
void DataList_Battle::AddHandCard(const std::shared_ptr<Card_Base>& pCard)
{
	// 引数
	// pCard <- 追加するカードのポインタ
	this->HandCardList.push_back(pCard);
}

// 捨て札にカードを追加
void DataList_Battle::AddTrashCard(const std::shared_ptr<Card_Base>& pCard)
{
	// 引数
	// pCard <- 追加するカードのポインタ

	/* 対象のカードのロストフラグが無効であるなら追加する */
	if (pCard->GetLostFlg() == false)
	{
		this->TrashCardList.push_back(pCard);
	}
}

// デッキからカードを削除
void DataList_Battle::RemoveDeckCard(const std::shared_ptr<Card_Base>& pCard)
{
	// 引数
	// pCard <- 追加するカードのポインタ
	this->DeckCardList.erase(std::remove(DeckCardList.begin(), DeckCardList.end(), pCard),DeckCardList.end());
}

// 手札からカードを削除
void DataList_Battle::RemoveHandCard(const std::shared_ptr<Card_Base>& pCard)
{
	// 引数
	// pCard <- 追加するカードのポインタ
	this->HandCardList.erase(std::remove(HandCardList.begin(), HandCardList.end(), pCard), HandCardList.end());
}

// 捨て札からカードを削除
void DataList_Battle::RemoveTrashCard(const std::shared_ptr<Card_Base>& pCard)
{
	// 引数
	// pCard <- 追加するカードのポインタ
	this->TrashCardList.erase(std::remove(TrashCardList.begin(), TrashCardList.end(), pCard), TrashCardList.end());
}

// バトルエリアからカードを削除
void DataList_Battle::RemoveBattleAreaCard(int AreaNo)
{
	// 引数
	// AreaNo <- バトルエリア番号
	this->BattleAreaCardList[AreaNo] = nullptr;
}

// チェイン状態の確認
void DataList_Battle::CheckChain()
{
	/* バトルエリアにセットされたカード同士で同じスートが連続しているか確認 */
	for (int i = 0; i < CHAIN_SUTE_AREA_MAX; i++)
	{
		/* 現在登録されているスートリストを初期化する */
		this->Chain_Suite_List[i].clear();

		/* いずれかのバトルエリアにカードが設定されていないならスキップ */
		if (this->BattleAreaCardList[i] == nullptr || this->BattleAreaCardList[i + 1] == nullptr)
		{
			continue;
		}

		/* 値が低い方のエリアにセットされたカードのスートリストを取得 */
		std::vector<std::string> LowerNumberSuiteList = this->BattleAreaCardList[i]->GetSuiteList();

		/* 値が高い方のエリアにセットされたカードのスートリストを取得 */
		std::vector<std::string> HigherNumberSuiteList = this->BattleAreaCardList[i + 1]->GetSuiteList();
	
		/* スートリスト同士で共通するスートがあるか確認 */
		for (const std::string& lowerSuite : LowerNumberSuiteList)
		{
			for (const std::string& higherSuite : HigherNumberSuiteList)
			{
				/* 同名のスートが設定されているなら登録する */
				if (lowerSuite == higherSuite)
				{
					this->Chain_Suite_List[i].push_back(lowerSuite);
					break;
				}
			}
		}
	}
}

// チェイン状態を初期化する
void DataList_Battle::ResetChain()
{
	/* チェイン中のスートリストを初期化する */
	for (int i = 0; i < CHAIN_SUTE_AREA_MAX; i++)
	{
		this->Chain_Suite_List[i].clear();
	}
}

// 与効果を追加
void DataList_Battle::AddEffect(const std::shared_ptr<Action_Effect_Base>& effect, int AreaNo)
{
	// 引数
	// effect	<- 追加する与効果
	// AreaNo	<- 与効果を追加するバトルエリア

	this->ActionEffectList[AreaNo].push_back(effect);
}

// 与効果を削除
void DataList_Battle::RemoveEffect(const std::shared_ptr<Action_Effect_Base>& effect, int AreaNo)
{
	// 引数
	// effect	<- 削除する与効果
	// AreaNo	<- 与効果を削除するバトルエリア

	this->ActionEffectList[AreaNo].erase(std::remove(ActionEffectList[AreaNo].begin(), ActionEffectList[AreaNo].end(), effect), ActionEffectList[AreaNo].end());
}
