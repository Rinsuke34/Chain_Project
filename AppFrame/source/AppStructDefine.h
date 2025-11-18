/* AppFrameで使用する構造体の宣言 */

#pragma once

/* 使用する要素のインクルード */
// 標準ライブラリ
#include <vector>
#include <string>
// 外部ライブラリ
#include <DxLib.h>
// 共通定義
#include "AppConstantDefine.h"

// 入力関連
namespace Struct_Input
{
	struct PLAYER_INPUT_JOYPAD	// プレイヤー入力(ジョイパッド)
	{
		/* ジョイパッド */
		char	cgInput[INPUT_MAX][16];						// ジョイパッド入力[0:ホールド、1:トリガ、2:リリース][入力内容]
		bool	bgTrigger[INPUT_MAX][INPUT_DIRECTION];		// ジョイパッドのトリガーの入力[0:ホールド、1:トリガ、2:リリース][0:左、1:右]
		short	sAnalogStickX[INPUT_DIRECTION];				// X軸アナログ入力[0:左、1:右]
		short	sAnalogStickY[INPUT_DIRECTION];				// Y軸アナログ入力[0:左、1:右]
	};
	
	struct PLAYER_INPUT_KEYBOARD_MOUSE	// プレイヤー入力(キーボード、マウス)
	{
		/* キーボード */
		char	cgInput[INPUT_MAX][256];	// キー入力[0:ホールド、1:トリガ、2:リリース][入力内容]

		/* マウス */
		int	    igInput[INPUT_MAX];		// 入力[0:ホールド、1:トリガ、2:リリース]

		int		iMouseX;		// マウスX座標
		int		iMouseY;		// マウスY座標
		int		iMouseMoveX;	// X移動量
		int		iMouseMoveY;	// Y移動量
	};
}

// コリジョン関連
namespace Struct_Collision
{
	struct COLLISION_CAPSULE	// カプセルコリジョン
	{
		VECTOR	vecCapsuleTop;		// カプセルの上部
		VECTOR	vecCapsuleBottom;	// カプセルの下部
		float	fCapsuleRadius;		// カプセルの半径
	};

	struct COLLISION_SQHERE		// 球体コリジョン
	{
		VECTOR	vecSqhere;			// 球体の中心
		float	fSqhereRadius;		// 球体の半径
	};

	struct COLLISION_LINE		// 線分コリジョン
	{
		VECTOR	vecLineStart;		// 線分の始点
		VECTOR	vecLineEnd;			// 線分の終点
	};

	struct COLLISION_BOX		// ボックスコリジョン
	{
		VECTOR	vecBoxCenter;		// 箱の中心
		VECTOR	vecBoxHalfSize;		// 箱のサイズ(各軸の半径)
	};
};

// 2D関係
namespace Struct_2D
{
	struct POSITION		// 2D座標
	{
		int	iX;		// X座標
		int	iY;		// Y座標
	};

	struct RANGE		// 2D範囲
	{
		int iTopX;		// 範囲の左上X座標
		int iTopY;		// 範囲の左上Y座標
		int iBottomX;	// 範囲の右下X座標
		int iBottomY;	// 範囲の右下Y座標
	};
};