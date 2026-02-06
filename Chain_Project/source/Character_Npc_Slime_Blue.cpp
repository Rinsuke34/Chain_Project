/* キャラクター(スライム(青))の定義 */

/* 使用する要素のインクルード */
// ヘッダファイル
#include "Character_Npc_Slime_Blue.h"
// 関連クラス
#include "DataList_Battle.h"
#include "Card_Include.h"

// コンストラクタ
Character_Slime_Blue::Character_Slime_Blue() : Character_Base()
{
	/* 初期化 */
	this->iHP_Max	= 15;			// 体力(最大値)
	this->iHP_Now	= 15;			// 体力(現在値)
	this->Camp		= Character_Base::CAMP_ENEMY;	// 陣営:敵陣営
	this->SizeX		= 150;			// キャラクターの幅
	this->SizeY		= 150;			// キャラクターの高さ
	this->DropCoin	= 3;			// ドロップするコインの枚数
	SetUpImage("Character_Ilust/Npc/Slime_Blue");

	/* ドロップするカード一覧の設定 */
	std::vector<std::shared_ptr<Card_Base>> dropCardList;
	if ((rand() % 100) < 80) { dropCardList.push_back(std::make_shared<Card_Arms_TravelerShield>()); }
	SetDropCardList(dropCardList);
}

// 行動
void Character_Slime_Blue::Action()
{
	/* 自分自身の立ち位置を取得 */
	int MyPosition = -1;
	for (int i = DataList_Battle::POSITION_FRONT; i < DataList_Battle::POSITION_MAX; i++)
	{
		std::shared_ptr<Character_Base> pCheckCharacter = this->pDataList_Battle->GetEnemyCharacter(i);
		if (pCheckCharacter != nullptr)
		{
			if (pCheckCharacter.get() == this)
			{
				MyPosition = i;
				break;
			}
		}
	}

	/* 自分の位置が見つからない場合は行動しない */
	if (MyPosition == -1)	{ return; }

	/* 防御行動を設定する */
	std::shared_ptr<Action_Effect_Defence> defenceEffect = std::make_shared<Action_Effect_Defence>();
	defenceEffect->Target_Camp		= Character_Base::CAMP_ENEMY;	// 効果対象の陣営:敵
	defenceEffect->Target_Position	= MyPosition;					// 効果対象の立ち位置:自分自身
	defenceEffect->ShieldAmount		= 5;							// シールド量
	defenceEffect->EffectUser		= shared_from_this();			// 効果の使用者:自分自身
	defenceEffect->Priority			= 0;							// 優先順位：最遅
	defenceEffect->EffectCard		= nullptr;						// 効果を使用するカード:無し
	this->pDataList_Battle->AddEffect(defenceEffect);
	this->ActionEffectList.push_back(defenceEffect);
}
