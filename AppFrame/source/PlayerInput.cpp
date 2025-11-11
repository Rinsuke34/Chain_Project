/* プレイヤーの入力取得クラスの定義 */

/* 使用する要素のインクルード */
// ヘッダファイル
#include "PlayerInput.h"
// 共通定義
#include "AppConstantDefine.h"
#include "AppVariableDefine.h"
#include "AppFunctionDefine.h"

// コンストラクタ
PlayerInput::PlayerInput()
{
	/* 初期化 */
	gstJoypadInputData		= {};						// ジョイパッド
	gstKeyboardInputData	= {};						// キーボード＆マウス
	gucTriggerThreshold		= INIT_TRIGGER_THRESHOLD;	// トリガー入力の閾値
}

// プレイヤーの入力取得
void PlayerInput::Input()
{
	/* ジョイパッドの入力取得 */
	InputJoypad();

	/* キーボード＆マウスの入力取得 */
	InputKeyboard();
}

// 入力取得_ジョイパッド
void PlayerInput::InputJoypad()
{
	/* ジョイパッド用の入力取得 */
	// ※XInputを前提としているため、DirectInputは非対応

	/* 前回の入力を保存 */
	Struct_Input::PLAYER_INPUT_JOYPAD pOldJoypadInput = gstJoypadInputData;

	/* ジョイパッド(1番)からの入力内容を取得(XInput) */
	XINPUT_STATE stXInputState;
	GetJoypadXInputState(DX_INPUT_PAD1, &stXInputState);

	/* ボタンの入力処理 */
	{
		/* ホールド入力情報を取得 */
		for (int i = 0; i < 16; i++)
		{
			gstJoypadInputData.cgInput[INPUT_HOLD][i] = stXInputState.Buttons[i];
		}

		/* トリガ入力情報を取得 */
		for (int i = 0; i < 16; i++)
		{
			gstJoypadInputData.cgInput[INPUT_TRG][i] = (gstJoypadInputData.cgInput[INPUT_HOLD][i] ^ pOldJoypadInput.cgInput[INPUT_HOLD][i]) & gstJoypadInputData.cgInput[INPUT_HOLD][i];
		}

		/* リリース入力情報を取得 */
		for (int i = 0; i < 16; i++)
		{
			gstJoypadInputData.cgInput[INPUT_REL][i] = (gstJoypadInputData.cgInput[INPUT_HOLD][i] ^ pOldJoypadInput.cgInput[INPUT_HOLD][i]) & pOldJoypadInput.cgInput[INPUT_HOLD][i];
		}
	}
	
	/* トリガーの入力処理 */
	{
		/* ホールド入力情報を取得 */
		{
			/* 入力値が閾値以上であるか判定 */
			gstJoypadInputData.bgTrigger[INPUT_HOLD][INPUT_LEFT]	= PUBLIC_PROCESS::bCheckAboveThreshold(stXInputState.LeftTrigger, gucTriggerThreshold);
			gstJoypadInputData.bgTrigger[INPUT_HOLD][INPUT_RIGHT]	= PUBLIC_PROCESS::bCheckAboveThreshold(stXInputState.RightTrigger, gucTriggerThreshold);
		}

		/* トリガ入力情報を取得 */
		{
			/* トリガーの入力が変化したか判定 */
			gstJoypadInputData.bgTrigger[INPUT_TRG][INPUT_LEFT]		= gstJoypadInputData.bgTrigger[INPUT_TRG][INPUT_LEFT]	^ pOldJoypadInput.bgTrigger[INPUT_HOLD][INPUT_LEFT];
			gstJoypadInputData.bgTrigger[INPUT_TRG][INPUT_RIGHT]	= gstJoypadInputData.bgTrigger[INPUT_TRG][INPUT_RIGHT]	^ pOldJoypadInput.bgTrigger[INPUT_HOLD][INPUT_RIGHT];
		}
		
		/* リリース入力情報を取得 */
		{
			/* トリガーの入力が変化したか判定 */
			gstJoypadInputData.bgTrigger[INPUT_REL][INPUT_LEFT]		= gstJoypadInputData.bgTrigger[INPUT_TRG][INPUT_LEFT]	^ pOldJoypadInput.bgTrigger[INPUT_HOLD][INPUT_LEFT];
			gstJoypadInputData.bgTrigger[INPUT_REL][INPUT_RIGHT]	= gstJoypadInputData.bgTrigger[INPUT_TRG][INPUT_RIGHT]	^ pOldJoypadInput.bgTrigger[INPUT_HOLD][INPUT_RIGHT];
		}
	}

	/* アナログスティックの入力量を取得 */
	{
		gstJoypadInputData.sAnalogStickX[INPUT_LEFT]	= stXInputState.ThumbLX;	// 左スティックX軸
		gstJoypadInputData.sAnalogStickY[INPUT_LEFT]	= stXInputState.ThumbLY;	// 左スティックY軸
		gstJoypadInputData.sAnalogStickX[INPUT_RIGHT]	= stXInputState.ThumbRX;	// 右スティックX軸
		gstJoypadInputData.sAnalogStickY[INPUT_RIGHT]	= stXInputState.ThumbRY;	// 右スティックY軸
	}
}

// 入力取得_キーボード＆マウス
void PlayerInput::InputKeyboard()
{
	/* キーボード＆マウス用の入力取得 */

	/* 前回の入力を保存 */
	Struct_Input::PLAYER_INPUT_KEYBOARD_MOUSE pOldKeyboardInput = gstKeyboardInputData;

	/* キーボードの入力処理 */
	{
		/* キーボードからのホールド入力情報を得る */
		GetHitKeyStateAll(gstKeyboardInputData.cgInput[INPUT_HOLD]);

		for (int i = 0; i < 256; i++)
		{
			/* トリガ入力情報の生成 */
			gstKeyboardInputData.cgInput[INPUT_TRG][i] = (gstKeyboardInputData.cgInput[INPUT_HOLD][i] ^ pOldKeyboardInput.cgInput[INPUT_HOLD][i]) & gstKeyboardInputData.cgInput[INPUT_HOLD][i];

			/* リリース入力情報の生成 */
			gstKeyboardInputData.cgInput[INPUT_REL][i] = (gstKeyboardInputData.cgInput[INPUT_HOLD][i] ^ pOldKeyboardInput.cgInput[INPUT_HOLD][i]) & pOldKeyboardInput.cgInput[INPUT_HOLD][i];
		}
	}

	/* マウスの入力処理 */
	{
		/* マウスのホールド入力情報を得る */
		gstKeyboardInputData.igInput[INPUT_HOLD] = GetMouseInput();

		/* マウスのトリガ入力情報の生成 */
		gstKeyboardInputData.igInput[INPUT_TRG] = (gstKeyboardInputData.igInput[INPUT_HOLD] ^ pOldKeyboardInput.igInput[INPUT_HOLD]) & gstKeyboardInputData.igInput[INPUT_HOLD];

		/* マウスのリリース入力情報の生成 */
		gstKeyboardInputData.igInput[INPUT_REL] = (gstKeyboardInputData.igInput[INPUT_HOLD] ^ pOldKeyboardInput.igInput[INPUT_HOLD]) & pOldKeyboardInput.igInput[INPUT_HOLD];

		/* マウス座標取得 */
		int iMouseX, iMouseY;	// マウス座標
		GetMousePoint(&iMouseX, &iMouseY);

		/* マウス座標設定 */
		gstKeyboardInputData.iMouseX = iMouseX;
		gstKeyboardInputData.iMouseY = iMouseY;

		/* マウス固定処理 */
		if (gbMouseCursorCenterFixedFlg == true)
		{
			// マウスの中心固定フラグが有効であるならば
			/* マウス原点設定 */
			// ※マウス座標の中心を原点として移動量を取得する
			int iMouseOriginX = SCREEN_SIZE_WIDE / 2;
			int iMouseOriginY = SCREEN_SIZE_HEIGHT / 2;

			/* マウス移動量測定 */
			gstKeyboardInputData.iMouseMoveX = iMouseX - iMouseOriginX;
			gstKeyboardInputData.iMouseMoveY = iMouseY - iMouseOriginY;

			/* マウス座標リセット */
			SetMousePoint(iMouseOriginX, iMouseOriginY);
		}
		else
		{
			// マウスの中心固定フラグが無効であるならば
			/* マウス移動量測定 */
			gstKeyboardInputData.iMouseMoveX = iMouseX - pOldKeyboardInput.iMouseX;
			gstKeyboardInputData.iMouseMoveY = iMouseY - pOldKeyboardInput.iMouseY;
		}

		/* マウスカーソルの描写設定 */
		if (gbMouseCursorNotDepictedFlg == true)
		{
			// マウスカーソル描写無効フラグが有効であるならば
			/* マウスカーソル表示を無効化 */
			SetMouseDispFlag(FALSE);
		}
		else
		{
			// マウスカーソル描写無効フラグが無効であるならば
			/* マウスカーソル表示を有効化 */
			SetMouseDispFlag(TRUE);
		}
	}
}
