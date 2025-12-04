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
		Action_Effect_Base();			// コンストラクタ
		virtual ~Action_Effect_Base();	// デストラクタ

		/* 関数 */
		virtual void Update();				// 更新
		virtual void ExecuteEffect() {};	// 効果実行
		virtual void Draw();				// 描写

		/* 変数 */
		int									Target_Camp;		// 効果対象の陣営
		int									Target_Position;	// 効果対象の立ち位置
		std::shared_ptr<Character_Base>		EffectUser;			// 効果の使用者
		bool								AllRange;			// 全体に効果を与えるか
		Struct_2D::POSITION					Now_Position;		// 現在座標
		Struct_2D::POSITION					Setting_Position;	// 設定座標(自動で補完される座標)
		int									Priority;			// 優先順位(高いほど先に実行)
		int									Image;				// 画像

		/* 定数 */
		static const int INTERPOLATION_SPEED	= 5;	// 補間速度
		static const int IMAGE_SIZE_HEIGHT		= 32;	// 画像高さ
		static const int IMAGE_SIZE_WIDE		= 218;	// 画像幅
		static const int IMAGE_FRAME_WIDE		= 4;	// 画像フレーム幅

	protected:
		/* 関数 */
		virtual void Update_Image();		// 画像更新
		virtual void Draw_BackGround();		// 背景作成
		virtual void Draw_Effect()	{};		// 効果部分作成
		virtual void Draw_Frame();			// フレーム作成

		/* 変数 */
		std::shared_ptr<DataList_Battle>	pDataList_Battle;	// 戦闘用データリスト
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

	protected:
		/* 関数 */
		virtual void Draw_Effect()	override;	// 効果部分作成
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

	protected:
		/* 関数 */
		virtual void Draw_Effect()	override;	// 効果部分作成
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

	protected:
		/* 関数 */
		virtual void Draw_Effect()	override;	// 効果部分作成
};

// 状態異常付与
class Action_Effect_StatusAliment : public Action_Effect_Base
{
	public:
		Action_Effect_StatusAliment();				// コンストラクタ
		virtual ~Action_Effect_StatusAliment()	{};	// デストラクタ

		/* 関数 */
		void ExecuteEffect()	override;	// 効果実行

		/* 変数 */
		int StatusAilment_Type;		// 状態異常の種類
		int StatusAilment_Level;	// 状態異常のレベル

	protected:
		/* 関数 */
		virtual void Draw_Effect()	override;	// 効果部分作成
};

// 特殊効果
class Action_Effect_Extra : public Action_Effect_Base
{
	public:
		Action_Effect_Extra();				// コンストラクタ
		virtual ~Action_Effect_Extra()	{};	// デストラクタ

		/* 関数 */
		void ExecuteEffect()	override;	// 効果実行

		/* 変数 */
		std::shared_ptr<Card_Base> ExEffectCard;	// 特殊効果を使用するカード

	protected:
		/* 関数 */
		virtual void Draw_Effect()	override;	// 効果部分作成
};
