/* カードのベースクラスの宣言 */

#pragma once

/* 使用する要素のインクルード */
// 共通定義
#include "AppFrame.h"

// カードのベースクラス
class Card_Base
{
	public:
		Card_Base();				// コンストラクタ
		virtual ~Card_Base() {};	// デストラクタ

		/* 関数 */
		virtual void Draw();						// 描画
		virtual void Effect_StartTurn()		{};		// 効果発動:ターン開始時
		virtual void Effect_StartAction()	{};		// 効果発動:行動開始時
		virtual void Effect_Battle()		{};		// 効果発動:戦闘時
		virtual void Effect_EndTurn()		{};		// 効果発動:ターン終了時

		/* セッター */
		void SetRarity(int rarity)					{ iRarity	= rarity; }				// レアリティの設定
		void SetCardType(int type)					{ iCardType	= type; }				// カードの種類の設定
		void SetName(std::string name)				{ Name	= name; }					// カード名の設定
		void AddSuite(int suite)					{ Suite_List.push_back(suite); }	// スートの追加
		void SetImage(std::shared_ptr<int> image)	{ Image	= image; }					// 画像の設定
		void SetDrawPos(Struct_2D::POSITION pos)	{ Pos_Draw = pos; }					// 描画位置の設定

		/* ゲッター */
		int GetRarity()						{ return iRarity; }		// レアリティの取得
		int GetCardType()					{ return iCardType; }	// カードの種類の取得
		std::string GetName()				{ return Name; }		// カード名の取得
		std::vector<int> GetSuiteList()		{ return Suite_List; }	// スートリストの取得
		std::shared_ptr<int> GetImage()		{ return Image; }		// 画像の取得
		Struct_2D::POSITION GetDrawPos()	{ return Pos_Draw; }	// 描画位置の取得

		/* 定数 */
		// レアリティの種類
		static const int RARITY_COMMON	= 0;	// コモン
		static const int RARITY_RARE	= 1;	// レア
		static const int RARITY_EPIC	= 2;	// エピック
		// カードの種類
		static const int TYPE_ARMS		= 0;	// 武具
		static const int TYPE_SPELL		= 1;	// 魔法
		static const int TYPE_ITEM		= 2;	// アイテム
		// スートの種類
		static const int SUITE_SWORD	= 0;	// 剣

	protected:
		/* 変数 */
		int						iRarity;	// レアリティ
		int						iCardType;	// カードの種類
		std::string				Name;		// カード名
		std::vector<int>		Suite_List;	// スートリスト
		std::shared_ptr<int>	Image;		// 画像
		Struct_2D::POSITION		Pos_Draw;	// 描画位置
};
