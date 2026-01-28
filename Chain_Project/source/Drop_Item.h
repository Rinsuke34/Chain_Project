/* ゲーム内でのドロップアイテムクラスの宣言 */

#pragma once

/* 使用する要素のインクルード */
// 共通定義
#include "AppFrame.h"

/* 前方宣言 */
class DataList_GameResource;
class Card_Base;

/* ドロップアイテムクラス */
// ベース
class Drop_Item_Base
{
	public:
		Drop_Item_Base();				// コンストラクタ
		virtual ~Drop_Item_Base() {};	// デストラクタ

		/* 関数 */
		virtual void	Update();	// 更新
		virtual void	Draw();		// 描画

		/* ゲッター */
		Struct_2D::POSITION	GetPosition()	{ return this->Position; }	// 位置座標
		int					GetUnderPosY()	{ return this->UnderPosY; }	// 地面のY座標
		bool				GetDeleteFlg()	{ return this->DeleteFlg; }	// 削除フラグ

		/* セッター */
		void SetPosition(Struct_2D::POSITION pos)	{ this->Position	= pos; }	// 位置座標
		void SetUnderPosY(int y)					{ this->UnderPosY	= y; }		// 地面のY座標
		void SetDeleteFlg(bool flg)					{ this->DeleteFlg	= flg; }	// 削除フラグ

		/* 定数 */
		// フェーズ
		static const int PHASE_FALLING	= 0;	// 落下フェーズ
		static const int PHASE_WAITING	= 1;	// 待機フェーズ
		static const int PHASE_MOVEING	= 2;	// 移動フェーズ
		// その他
		static const int	FALLING_SPEED_START	= -10;		// 落下速度(初速)
		const float			MOVE_SPEED			= 50.f;		// 移動速度
		static const int	DRAW_SIZE			= 32;		// 描写時のサイズ

	protected:
		/* 変数 */
		Struct_2D::POSITION						Position;				// 位置座標
		int										FallingSpeed;			// 落下速度
		int										UnderPosY;				// 地面のY座標
		std::shared_ptr<DataList_GameResource>	pDataList_GameResource;	// ゲームリソース管理用データリスト
		std::shared_ptr<int>					Image;					// 画像
		int										MovePhase;				// 移動フェーズ
		int										WaitTimer;				// 待機タイマー
		bool									DeleteFlg;				// 削除フラグ
		int										FallMoveSpeed_X;		// 落下中のX軸移動速度

		/* 関数 */
		virtual void Update_Falling();		// 落下フェーズ更新
		virtual void Update_Waiting();		// 待機フェーズ更新
		virtual void Update_Moveing() {};	// 移動フェーズ更新
};

// コイン
class Drop_Item_Coin : public Drop_Item_Base
{
	public:
		Drop_Item_Coin();				// コンストラクタ
		virtual ~Drop_Item_Coin() {};	// デストラクタ

	protected:
		/* 関数 */
		virtual void Update_Moveing()	override;	// 移動フェーズ更新
};

// カード
class Drop_Item_Card : public Drop_Item_Base
{
	public:
		Drop_Item_Card();				// コンストラクタ
		virtual ~Drop_Item_Card() {};	// デストラクタ

		/* セッター */
		void SetCard(std::shared_ptr<Card_Base> Card) { this->Card = Card; }	// ドロップカード設定

		/* ゲッター */
		std::shared_ptr<Card_Base> GetCard() { return this->Card; }		// ドロップカード取得

	protected:
		/* 変数 */
		std::shared_ptr<Card_Base> 	Card;	// ドロップカード

		/* 関数 */
		virtual void Update_Moveing()	override;	// 移動フェーズ更新
};
