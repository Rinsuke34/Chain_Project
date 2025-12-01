/* カードのベースクラスの宣言 */

#pragma once

/* 使用する要素のインクルード */
// 標準ライブラリ
#include <string_view>
// 共通定義
#include "AppFrame.h"

// 前方宣言
class DataList_Battle;
class Character_Base;

// カードのベースクラス
class Card_Base
{
	public:
		Card_Base();			// コンストラクタ
		virtual ~Card_Base();	// デストラクタ

		/* 関数 */
		virtual void Draw();						// 描画
		virtual void UpdateImage();					// 画像更新
		virtual void Effect_StartTurn()			{};	// 効果発動:ターン開始時
		virtual void Effect_StartAction()		{};	// 効果発動:行動開始時
		virtual void BattleAction();				// 戦闘行動
		virtual void Effect_EndTurn()			{};	// 効果発動:ターン終了時
		virtual void Position_Interpolation();		// 位置座標補間処理
		
		/* セッター */
		// カード情報
		void SetRarity(int rarity)					{ iRarity			= rarity; }		// レアリティの設定
		void SetCardType(int type)					{ iCardType			= type; }		// カードの種類の設定
		void SetName(std::string name)				{ Name				= name; }		// カード名の設定
		void AddSuite(std::string suite)			{ Suite_List.push_back(suite); }	// スートの追加
		void SetStrength(int strength)				{ Strength			= strength; }	// 攻撃力の設定
		void SetDiffence(int diffence)				{ Diffence			= diffence; }	// 防御力の設定
		void SetAttackRange(int range)				{ AttackRange		= range; }		// 攻撃範囲の設定
		// その他
		void SetNowPos(Struct_2D::POSITION pos)		{ Now_Position		= pos; }		// 現在座標の設定
		void SetSettingPos(Struct_2D::POSITION pos) { Setting_Position	= pos; }		// 設定座標の設定
		void SetLostFlag(bool flag)					{ bLostFlag			= flag; }		// ロストフラグの設定
		void SetNowChainCount(int count)			{ iNowChainCount	= count; }		// 現在のチェイン数の設定

		/* ゲッター */
		// カード情報
		int							GetRarity()			{ return iRarity; }			// レアリティの取得
		int							GetCardType()		{ return iCardType; }		// カードの種類の取得
		std::string					GetName()			{ return Name; }			// カード名の取得
		std::vector<std::string>	GetSuiteList()		{ return Suite_List; }		// スートリストの取得
		int							GetStrength()		{ return Strength; }		// 攻撃力の取得
		int							GetDiffence()		{ return Diffence; }		// 防御力の取得
		int							GetAttackRange()	{ return AttackRange; }		// 攻撃範囲の取得
		// その他
		Struct_2D::POSITION			GetNowPos()		{ return Now_Position; }		// 現在座標の取得
		Struct_2D::POSITION			GetSettingPos() { return Setting_Position; }	// 設定座標の取得
		bool						GetLostFlg()	{ return bLostFlag; }			// ロストフラグの取得

		/* 定数 */
		// 補間
		static const int INTERPOLATION_SPEED	= 5;	// 補間速度
		// サイズ
		static const int CARD_WIDTH		= 168;	// カード幅
		static const int CARD_HEIGHT	= 238;	// カード高さ
		// レアリティの種類
		static const int RARITY_COMMON	= 0;	// コモン
		static const int RARITY_RARE	= 1;	// レア
		static const int RARITY_EPIC	= 2;	// エピック
		// カードの種類
		static const int TYPE_ARMS		= 0;	// 武具
		static const int TYPE_SPELL		= 1;	// 魔法
		static const int TYPE_ITEM		= 2;	// アイテム
		// スートの種類
		inline static const	std::string	SUITE_SWORD		= "Sword";	// 剣
		inline static const	std::string	SUITE_SHIELD	= "Shield";	// 盾
		inline static const	std::string	SUITE_POTION	= "Potion";	// 薬(ポーション)
		// 画像のサイズ(実際のサイズより大きめにする)
		static const int IMAGE_SIZE_WIDTH	= 256;	// 画像幅
		static const int IMAGE_SIZE_HEIGHT	= 256;	// 画像高さ
		// ネームプレート
		static const int NAMEPLATE_WIDTH		= 150;	// ネームプレート幅
		static const int NAMEPLATE_HEIGHT		= 24;	// ネームプレート高さ
		static const int NAMEPLATE_POSITION_Y	= 20;	// カード名描写位置Y座標(カードの最上部から見て)
		// 攻撃範囲
		static const int ATTACKRANGE_FRONT	= 0;	// 先頭から
		static const int ATTACKRANGE_RANDOM	= 1;	// ランダム
		static const int ATTACKRANGE_ALL	= 2;	// 全体
		// スート(描写)
		static const int SUTE_SIZE_WIDTH	= 36;	// スート幅
		static const int SUTE_SIZE_HEIGHT	= 36;	// スート高さ
		static const int SUTE_START_POS_X	= 12;	// スート描写開始X座標
		static const int SUTE_POS_Y			= 35;	// スート描写Y座標
		// イラスト(描写)
		static const int ILLUSTRATION_POS_Y = 32;	// イラスト描写Y座標(中心から見たときの補正量)

	protected:
		/* 変数 */
		// データリスト
		std::shared_ptr<DataList_Battle> pDataList_Battle;		// バトル用データリスト(※取得できない場合もあるので使用時はnullptrかチェックすること)
		// カード情報
		int								iRarity;			// レアリティ
		int								iCardType;			// カードの種類
		std::string						Name;				// カード名
		std::vector<std::string>		Suite_List;			// スートリスト
		int 							Strength;			// 攻撃力
		int								Diffence;			// 防御力
		int								AttackRange;		// 攻撃範囲
		std::string						ImageName;			// 画像の名前
		// 画像
		int								Image;				// 画像
		// その他
		Struct_2D::POSITION				Now_Position;		// 現在座標
		Struct_2D::POSITION				Setting_Position;	// 設定座標(ホールドが解除された際に自動で補正される座標)
		bool							bLostFlag;			// ロストフラグ(捨て札置き場におかれず完全に削除される)
		int								iNowChainCount;		// 現在のチェイン数(ターン開始時に設定)
		std::shared_ptr<Character_Base>	pPlayer;			// プレイヤーキャラクターのポインタ

		/* 関数 */
		virtual int		GetMyAreaNo();			// 自身のバトルエリア番号を取得
		virtual void	DrawBackGround()	{};	// 背景を描写
		virtual void	DrawImage()			{};	// イラストを描写
		virtual void	DrawSuite()			{};	// スートを描写
		virtual void	DrawFrame()			{};	// フレームを描写
		virtual void	DrawName()			{};	// 名前を描写
		virtual void	CheckHavePlayer();		// プレイヤーを取得しているか確認
};
