/* ゲーム内でのキャラクターの行動内容クラスの宣言 */

#pragma once

/* 使用する要素のインクルード */
// 共通定義
#include "AppFrame.h"

/* 前方宣言 */
class Character_Base;
class Card_Base;
class DataList_Battle;

/* 行動内容クラス */
// ベース
class Action_Effect_Base
{
	public:
		Action_Effect_Base();				// コンストラクタ
		virtual ~Action_Effect_Base() {};	// デストラクタ

		/* 関数 */
		virtual void ExecuteEffect()	{};	// 効果実行
		virtual void Setup_Image();			// 画像設定

		/* 変数 */
		int									Target_Camp;		// 効果対象の陣営
		int									Target_Position;	// 効果対象の立ち位置
		std::shared_ptr<Character_Base>		EffectUser;			// 効果の使用者
		bool								AllRange;			// 全体に効果を与えるか
		int									Priority;			// 優先順位(高いほど先に実行)
		std::shared_ptr<Card_Base>			EffectCard;			// 効果を使用するカード
		int									IconType;			// アイコンのタイプ
		std::string							ExplanationText;	// 説明文

		/* 定数 */
		static const int ICON_TYPE_ATTACK	= 0;	// アイコンタイプ:攻撃
		static const int ICON_TYPE_DEFENCE	= 1;	// アイコンタイプ:防御
		static const int ICON_TYPE_HEAL		= 2;	// アイコンタイプ:回復
		static const int ICON_TYPE_EXTRA	= 3;	// アイコンタイプ:特殊効果

		/* ゲッター */
		std::shared_ptr<int> GetImage()		{ return Image; }	// 画像取得

	protected:

		/* 変数 */
		std::shared_ptr<DataList_Battle>	pDataList_Battle;	// 戦闘用データリスト
		std::shared_ptr<int>				Image;				// 画像
};

// 攻撃
class Action_Effect_Attack : public Action_Effect_Base
{
	public:
		Action_Effect_Attack();				// コンストラクタ
		virtual ~Action_Effect_Attack()	{};	// デストラクタ

		/* 関数 */
		void ExecuteEffect()	override;	// 効果実行

		/* 変数 */
		int DamageAmount;	// ダメージ量
};

// 防御
class Action_Effect_Defence : public Action_Effect_Base
{
	public:
		Action_Effect_Defence();				// コンストラクタ
		virtual ~Action_Effect_Defence() {};	// デストラクタ

		/* 関数 */
		void ExecuteEffect()	override;	// 効果実行

		/* 変数 */
		int ShieldAmount;	// シールド量
};

// 回復
class Action_Effect_Heal : public Action_Effect_Base
{
	public:
		Action_Effect_Heal();				// コンストラクタ
		virtual ~Action_Effect_Heal()	{};	// デストラクタ

		/* 関数 */
		void ExecuteEffect()	override;	// 効果実行

		/* 変数 */
		int HealAmount;		// 回復量
};

// 特殊効果
class Action_Effect_Extra : public Action_Effect_Base
{
	public:
		Action_Effect_Extra();				// コンストラクタ
		virtual ~Action_Effect_Extra()	{};	// デストラクタ

		/* 関数 */
		void ExecuteEffect()	override;	// 効果実行
};
