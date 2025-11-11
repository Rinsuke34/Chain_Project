/* プレイヤーの入力を取得するクラスの宣言 */

#pragma once
/* 使用する要素のインクルード */
// 標準ライブラリ
#include <cstring>
// 外部ライブラリ
#include <DxLib.h>

// 入力取得クラス
class PlayerInput
{
	public:
		PlayerInput();					// コンストラクタ
		virtual ~PlayerInput() {};		// デストラクタ

		/* 関数 */
		void	Input();				// プレイヤーの入力取得

	private:
		/* 関数 */
		void	InputJoypad();			// ジョイパッド
		void	InputKeyboard();		// キーボード＆マウス

	protected:
};
