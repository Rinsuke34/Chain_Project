/* キャラクターのベースクラスの宣言 */

#pragma once

/* 使用する要素のインクルード */
// 共通定義
#include "AppFrame.h"

// キャラクターのベースクラス
class Character_Base
{
	public:
		Character_Base();				// コンストラクタ
		virtual ~Character_Base()	{};	// デストラクタ

		/* 関数 */
		virtual void Draw();			// 描画
		virtual void Draw_HPBar();		// 体力バー描画
		virtual void Action()		{};	// 行動

		/* ゲッター */
		int 					GetHP_Max()		{ return iHP_Max; }			// 体力(最大値)の取得
		int 					GetHP_Now()		{ return iHP_Now; }			// 体力(現在値)の取得
		Struct_2D::POSITION		GetCenterPos()	{ return Center_Position; }	// 中心座標の取得

		/* セッター */
		void	SetHp_Max(int MaxHP)					{ this->iHP_Max			= MaxHP; }	// 体力(最大値)の設定
		void	SetHp_Now(int NowHP)					{ this->iHP_Now			= NowHP; }	// 体力(現在地)の設定
		void	SetCenterPos(Struct_2D::POSITION Pos)	{ this->Center_Position	= Pos; }	// 中心座標の設定

	protected:
		/* 変数 */
		// キャラクター情報
		int						iHP_Max;			// 体力(最大値)
		int						iHP_Now;			// 体力(現在値)
		std::shared_ptr<int>	Image;				// 画像
		// その他
		Struct_2D::POSITION		Center_Position;	// 中心座標

		/* 関数 */
		void SetUpImage(std::string ImageName);		// 指定の名称の画像を設定する
};
