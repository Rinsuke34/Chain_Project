/* ゲーム内でのキャラクターのバフ、デバフの宣言 */

#pragma once

/* 使用する要素のインクルード */
// 共通定義
#include "AppFrame.h"

/* バフ、デバフ関連 */
// ベース
class Character_Buff_Debuff_Base
{
	public:
		Character_Buff_Debuff_Base();				// コンストラクタ
		virtual ~Character_Buff_Debuff_Base() {};	// デストラクタ

		/* 関数 */
		virtual void Update();			// 更新処理

		/* 変数 */
		std::string				Name;					// バフ、デバフの名称
		int						Buff_Debuff_Time;		// バフ、デバフの残りターン数
		bool					DeleteFlag;				// 削除フラグ
		std::shared_ptr<int>	Image;					// 画像
};

// 攻撃力バフ
class Character_Buff_Strength : public Character_Buff_Debuff_Base
{
	public:
		Character_Buff_Strength();				// コンストラクタ
		virtual ~Character_Buff_Strength() {};	// デストラクタ

		/* 変数 */
		int Strength_Buff_Amount;	// 攻撃力バフ量

};

// 防御力バフ
class Character_Buff_Diffence : public Character_Buff_Debuff_Base
{
	public:
		Character_Buff_Diffence();				// コンストラクタ
		virtual ~Character_Buff_Diffence() {};	// デストラクタ

		/* 変数 */
		int Diffence_Buff_Amount;	// 防御力バフ量
};

// 毒状態
class Character_Debuff_Poison : public Character_Buff_Debuff_Base
{
	public:
		Character_Debuff_Poison();				// コンストラクタ
		virtual ~Character_Debuff_Poison() {};	// デストラクタ
};
