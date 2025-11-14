/* 広域変数の宣言 */

#pragma once
/* 使用する要素のインクルード */
// 標準ライブラリ
#include <vector>
#include <memory>
// 共通定義
#include "AppStructDefine.h"

/* 前方宣言 */
class Fps;
class PlayerInput;
class SceneServer;
class DataListServer;

/* プログラム状態 */
extern bool gbEndFlg;	// プログラム終了フラグ

/* プレイヤーの入力情報 */
extern Struct_Input::PLAYER_INPUT_JOYPAD			gstJoypadInputData;				// ジョイパッド
extern Struct_Input::PLAYER_INPUT_KEYBOARD_MOUSE	gstKeyboardInputData;			// キーボード＆マウス
extern unsigned char								gucTriggerThreshold;			// トリガー入力の閾値
extern bool											gbMouseCursorCenterFixedFlg;	// マウスの中心固定フラグ(有効であるならばマウスカーソルを画面中央に固定する)
extern bool											gbMouseCursorNotDepictedFlg;	// マウスカーソル描写無効フラグ(有効であるならばマウスカーソルを描写しない)


/* 実行中クラス */
extern std::unique_ptr<Fps>				gpFps;					// fps固定用クラス
extern std::unique_ptr<PlayerInput>		gpPlayerInput;			// プレイヤー入力取得クラス
extern std::unique_ptr<SceneServer>		gpSceneServer;			// シーンサーバークラス
extern std::unique_ptr<DataListServer>	gpDataListServer;		// データリストサーバークラス

/* フレームレート関連 */
extern int giNowFps;							// 現在のフレームレート

/* デバッグ用描写管理フラグ */
extern bool gbDrawSceneListFlg;					// シーンリストの描写
extern bool	gbDrawDatalistFlg;					// プレイヤーステータスの描写
extern bool gbDrawFpsFlg;						// フレームレートの描写
