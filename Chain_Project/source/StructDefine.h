/* 構造体の宣言 */
// ※structでなく汎用的に使うclassも定義

#pragma once

/* カード効果 */
// ベース
class Card_Effect_Base
{
	public:
		int Target_Camp;		// 効果対象の陣営
		int Target_Position;	// 効果対象の立ち位置
};

// 攻撃
class Card_Effect_Attack : public Card_Effect_Base
{
	public:
		int Damage;				// ダメージ量
};

// 防御
class Card_Effect_Defence : public Card_Effect_Base
{
	public:
		int Shield;				// シールド量
};

// 回復
class Card_Effect_Heal : public Card_Effect_Base
{
	public:
		int Heal;				// 回復量
};

// 状態以上付与
class Card_Effect_StatusAilment : public Card_Effect_Base
{
	public:
		int StatusAilment_Type;		// 状態異常の種類
		int StatusAilment_Level;	// 状態異常のレベル
};
