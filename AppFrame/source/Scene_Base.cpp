/* シーンのベースとなるクラスの定義 */

/* 使用する要素のインクルード */
// 標準ライブラリ
#include <cstring>
// ヘッダファイル
#include "Scene_Base.h"
// 共通定義
#include "AppVariableDefine.h"

// コンストラクタ
Scene_Base::Scene_Base(const std::string& cName, const int iLayer, const bool bLowerLayerUpdataStopFlg, const bool bUpperLayerDrawStopFlg)
{
	// 引数
	// iLayer					<- シーンのレイヤー順序(高いほど手前に描写される)
	// cName					<- シーンの名前
	// bLowerLayerUpdataStopFlg	<- 下層レイヤーの計算停止フラグ(有効でも描写はされます)
	// bUpperLayerDrawStopFlg	<- 上層レイヤーの描写停止フラグ(有効でも計算はされます)

	/* 初期化処理 */
	// SceneServerでの管理用データ
	this->stSceneName				= cName;						// シーン名を設定
	this->iLayerOrder				= iLayer;						// レイヤー順序を設定
	this->bDeleteFlg				= false;						// 削除フラグを無効化
	this->bLowerLayerUpdataStopFlg	= bLowerLayerUpdataStopFlg;		// 下位レイヤーの計算停止フラグを設定
	this->bUpperLayerDrawStopFlg	= bUpperLayerDrawStopFlg;		// 上位レイヤーの描写停止フラグを設定
}
