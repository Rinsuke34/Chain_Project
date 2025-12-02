/* キャラクターのベースクラスの宣言 */

#pragma once

/* 使用する要素のインクルード */
// 標準ライブラリ
#include <memory>
// 共通定義
#include "AppFrame.h"

// キャラクターのベースクラス
class Character_Base : public std::enable_shared_from_this<Character_Base>
{
	public:
		Character_Base();				// コンストラクタ
		virtual ~Character_Base()	{};	// デストラクタ

		/* 関数 */
		virtual void Draw();					// 描画
		virtual void Draw_HPBar();				// 体力バー描画
		virtual void Draw_ShieldBar();			// シールドバー描写
		virtual void Action()		{};			// 行動
		virtual void Action_Attack();			// 攻撃アクション
		virtual void Action_AddBuff();			// バフ付与アクション
		virtual void Damage(int DamageAmount);	// ダメージ処理
		virtual void AddShield(int Shield);		// シールド追加処理
		virtual void ShieldReset_EndTurn();		// シールドリセット(ターン終了時)
		virtual void Heal(int Heal);			// 回復処理

		/* ゲッター */
		int 					GetHP_Max()		{ return iHP_Max; }		// 体力(最大値)の取得
		int 					GetHP_Now()		{ return iHP_Now; }		// 体力(現在値)の取得
		int 					GetShield_Now() { return iShield_Now; }	// シールド(現在値)の取得
		Struct_2D::POSITION		GetBasePos()	{ return BasePos; }		// 基準座標の取得
		int						GetCamp()		{ return Camp; }		// 陣営の取得

		/* セッター */
		void	SetHp_Max(int MaxHP)					{ this->iHP_Max			= MaxHP; }		// 体力(最大値)の設定
		void	SetHp_Now(int NowHP)					{ this->iHP_Now			= NowHP; }		// 体力(現在地)の設定
		void	SetShield_Now(int NowShield)			{ this->iShield_Now		= NowShield; }	// シールド(現在値)の設定
		void	SetBasePos(Struct_2D::POSITION Pos)		{ this->BasePos			= Pos; }		// 基準座標の設定
		void	SetCamp(int camp)						{ this->Camp			= camp; }		// 陣営の設定

		/* 定数 */
		// 描写系
		static const int	HPBAR_WIDE			= 128;	// HPバーの幅
		static const int	HPBAR_HEIGHT		= 20;	// HPバーの高さ
		static const int	HPBAR_UPPER			= 20;	// HPバーの上端位置補正値
		static const int	SHIELDBAR_HEIGHT	= 10;	// シールドバーの高さ
		static const int	SHIELDBAR_UPPER		= 10;	// シールドバーの上端位置補正値
		// 陣営
		static const int	CAMP_FRIEND		= 0;	// 仲間陣営
		static const int	CAMP_ENEMY		= 1;	// 敵陣営

	protected:
		/* 変数 */
		// キャラクター情報
		int						iHP_Max;		// 体力(最大値)
		int						iHP_Now;		// 体力(現在値)
		int						iShield_Now;	// シールド(現在値)
		std::shared_ptr<int>	Image;			// 画像
		int						Camp;			// 陣営
		// その他
		Struct_2D::POSITION		BasePos;			// 基準座標(足元)
		int						SizeX;				// キャラクターの幅
		int						SizeY;				// キャラクターの高さ
		Struct_2D::POSITION		CorrectionPos;		// 補正座標
		int						AddBuffReaction;	// バフ付与時のリアクション
		int						DamageReaction;		// 被ダメージ時のリアクション
		int						AttackReaction;		// 攻撃時のリアクション

		/* 関数 */
		void SetUpImage(std::string ImageName);		// 指定の名称の画像を設定する
		void Correction_Reaction();					// 各リアクションによる座標補正
};
