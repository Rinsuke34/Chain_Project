/* 構造体の宣言 */
// ※structでなく汎用的に使うclassも定義

#pragma once

/* 前方宣言 */
class Character_Base;

/* カード効果 */
// ベース
class Card_Effect_Base
{
	public:
		virtual ~Card_Effect_Base() = default;			// デストラクタ

		int								Target_Camp		= 0;		// 効果対象の陣営
		int								Target_Position	= 0;		// 効果対象の立ち位置
		std::shared_ptr<Character_Base>	EffectUser		= nullptr;	// 効果の使用者
		bool							AllRange		= false;	// 全体に効果を与えるか
};

// 攻撃
class Card_Effect_Attack : public Card_Effect_Base
{
	public:
		int DamageAmount	= 0;	// ダメージ量
};

// 防御
class Card_Effect_Defence : public Card_Effect_Base
{
	public:
		int ShieldAmount	= 0;	// シールド量
};

// 回復
class Card_Effect_Heal : public Card_Effect_Base
{
	public:
		int HealAmount	= 0;		// 回復量
};

// 状態異常付与
class Card_Effect_StatusAilment : public Card_Effect_Base
{
	public:
		int StatusAilment_Type	= 0;	// 状態異常の種類
		int StatusAilment_Level	= 0;	// 状態異常のレベル
};
