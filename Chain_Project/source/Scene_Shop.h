/* シーン"ショップ画面"の宣言 */

#pragma once

/* 使用する要素のインクルード */
// 共通定義
#include "AppFrame.h"

/* 前方宣言 */
class DataList_GameResource;
class Scene_UI_Button;
class Card_Base;

// 商品クラス
class Shop_Item
{
	public:
		Shop_Item(std::shared_ptr<Card_Base> card, int price);	// コンストラクタ
		virtual ~Shop_Item() {};								// デストラクタ

		/* セッター */
		void	SetSoldOutFlg(bool flg)							{ SoldOutFlg			= flg; }	// 売り切れフラグ設定
		void	SetCardCenterPosition(Struct_2D::POSITION pos)	{ CardCenterPosition	= pos; }	// カード中心位置設定

		/* ゲッター */
		std::shared_ptr<Card_Base>	GetCard()				{ return ItemCard; }			// 商品カード取得
		int							GetPrice()				{ return ItemPrice; }			// 商品価格取得
		bool						GetSoldOutFlg()			{ return SoldOutFlg; }			// 売り切れフラグ取得
		Struct_2D::POSITION			GetCardCenterPosition()	{ return CardCenterPosition; }	// カード中心位置取得

	private:
		/* 変数 */
		std::shared_ptr<Card_Base>	ItemCard;			// 商品カード
		int							ItemPrice;			// 商品価格
		bool						SoldOutFlg;			// 売り切れフラグ
		Struct_2D::POSITION			CardCenterPosition; // カード中心位置
};

// シーン"ショップ画面"
class Scene_Shop : public Scene_Base
{
	public:
		Scene_Shop();			// コンストラクタ
		virtual ~Scene_Shop();	// デストラクタ

		/* 関数 */
		virtual void	Update()			override;	// 更新
		virtual void	Draw()				override;	// 描画
		virtual void	ShopSetup(int Level);			// ショップ設定

		/* 定数 */
		static const int SHOP_ITEM_INTERVAL	= 255;					// ショップ商品の間隔

	private:
		/* 変数 */
		// データリスト
		std::shared_ptr<DataList_GameResource>	pDataList_GameResource;	// ゲームリソース管理用データリスト
		// UIのハンドル
		std::shared_ptr<Scene_UI_Button>		UI_DecisionButton;		// 決定ボタン
		// 画像
		std::shared_ptr<int> Image_BackGround;		// 背景画像
		std::shared_ptr<int> Image_Frame_Corner;	// 角
		std::shared_ptr<int> Image_Frame_Line;		// 線
		std::shared_ptr<int> Image_Frame_Inside;	// 中央
		std::shared_ptr<int> Image_Coin;			// コイン
		// その他
		bool									ShopEndFlg;		// ショップ終了フラグ
		std::vector<std::shared_ptr<Shop_Item>>	ShopItemList;	// ショップ商品リスト

		/* 関数 */
		void Draw_BackGround();		// 背景描写
		void Draw_ShopItems();		// ショップ商品描写
		void CardPosition_Setup();	// カード位置セットアップ
		void ShopEnd_Check();		// ショップ終了確認
		void ShopItem_BuyCheck();	// ショップ商品購入確認
};
