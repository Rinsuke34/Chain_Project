/* 広域変数の初期化 */

/* 使用する要素のインクルード */
// ヘッダファイル
#include "AppVariableDefine.h"
// 関連クラス
#include "Fps.h"
#include "PlayerInput.h"
#include "SceneServer.h"
#include "DataListServer.h"

/* プログラム状態 */
bool gbEndFlg	= false;	// プログラム終了フラグ

/* プレイヤーの入力情報 */
Struct_Input::PLAYER_INPUT_JOYPAD			gstJoypadInputData;						// ジョイパッド
Struct_Input::PLAYER_INPUT_KEYBOARD_MOUSE	gstKeyboardInputData;					// キーボード＆マウス
unsigned char								gucTriggerThreshold			= 0;		// トリガー入力の閾値
bool										gbMouseCursorCenterFixedFlg	= false;	// マウスの中心固定フラグ(有効であるならばマウスカーソルを画面中央に固定する)
bool										gbMouseCursorNotDepictedFlg	= false;	// マウスカーソル描写無効フラグ(有効であるならばマウスカーソルを描写しない)

/* 実行中クラス */
std::unique_ptr<Fps>			gpFps				= nullptr;	// fps固定用クラス
std::unique_ptr<PlayerInput>	gpPlayerInput		= nullptr;	// プレイヤー入力取得クラス
std::unique_ptr<SceneServer>	gpSceneServer		= nullptr;	// シーンサーバークラス
std::unique_ptr<DataListServer> gpDataListServer	= nullptr;	// データリストサーバークラス

/* フレームレート関連 */
int	giNowFps	= 0;						// 現在のフレームレート

/* デバッグ用描写管理フラグ */
bool gbDrawSceneListFlg		= false;		// シーンリストの描写
bool gbDrawDatalistFlg		= false;		// データリストの描写
bool gbDrawFpsFlg			= false;		// フレームレートの描写
