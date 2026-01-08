/* データリスト"ゲームリソース管理"クラスの定義 */

/* 使用する要素のインクルード */
// ヘッダファイル
#include "DataList_GameResource.h"

// コンストラクタ
DataList_GameResource::DataList_GameResource() : DataList_Base("DataList_GameResource")
{
	/* 初期化 */
	this->GameState				= STATE_STAGE;	// ゲームの状態
	this->StageEndFlg			= true;			// ステージ終了フラグ
	this->WoldMapActiveFlg		= false;		// ワールドマップが有効であるかのフラグ
	this->NowMapNode			= nullptr;		// 現在地点のノード
	this->NextStageSelectFlg	= false;		// 次のステージの選択が完了したかのフラグ
	this->GameEndFlg			= false;		// ゲーム終了フラグ
}
