/* すべてのシーンのベースとなるクラスの宣言 */

#pragma once

/* 使用する要素のインクルード */
// 標準ライブラリ
#include <string>
// 外部ライブラリ
#include <DxLib.h>

// シーンベースクラス
class Scene_Base
{
	public:
		Scene_Base(const std::string& cName, const int iLayer, const bool bLowerLayerUpdataStopFlg);	// コンストラクタ
		virtual ~Scene_Base() {};	// デストラクタ

		/* 関数 */
		virtual void	Update()			{};	// 更新
		virtual void	Draw()				{};	// 描画

		/* セッター */
		void	SetDeleteFlg(bool bDeleteFlg)	{ this->bDeleteFlg = bDeleteFlg; }		// 削除フラグを設定(有効ならばシーンを削除する)

		/* ゲッター */
		bool	bGetDeleteFlg()					{ return this->bDeleteFlg; };				// 削除フラグを取得
		int		iGetSceneLayerOrder()			{ return this->iLayerOrder; };				// レイヤー順序を取得
		bool	bGetLowerLayerUpdateStopFlg()	{ return this->bLowerLayerUpdataStopFlg; }	// 下層レイヤー計算停止フラグを取得
		std::string	stGetSceneName()			{ return this->stSceneName; }				// シーン名称を取得

	protected:
		/* 変数 */
		// SceneServerでの管理用データ
		std::string		stSceneName;					// シーン名称
		int				iLayerOrder;					// レイヤー順序(値が大きいほど手前に描写)
		bool			bLowerLayerUpdataStopFlg;		// 下層レイヤー計算停止フラグ(有効なら下層レイヤーで計算を行わない)
		bool			bDeleteFlg;						// 削除フラグ(有効ならシーンを削除)
};
