/* データリスト"セーブデータ管理"クラスの宣言 */

#pragma once

/* 使用する要素のインクルード */
// 共通定義
#include "AppFrame.h"

// データリスト"セーブデータ管理"クラス
class DataList_SaveData : public DataList_Base
{
	public:
		DataList_SaveData();			// コンストラクタ
		virtual ~DataList_SaveData();	// デストラクタ

		/* ゲッター */
		int GetHaveExp()					{ return HaveExp; }					// 所持経験値
		int GetPlayerClassNo()				{ return PlayerClassNo; }			// プレイヤークラス番号
		int GetLevel_Hp(int classNo)		{ return Level_Hp[classNo]; }		// クラスごとのHPレベル
		int GetLevel_Ability(int classNo)	{ return Level_Ability[classNo]; }	// クラスごとのアビリティレベル
		int GetLevel_Deck(int classNo)		{ return Level_Deck[classNo]; }		// クラスごとのデッキレベル

		/* セッター */
		void SetHaveExp(int exp)						{ HaveExp					= exp; }		// 所持経験値
		void SetPlayerClassNo(int classNo)				{ PlayerClassNo				= classNo; }	// プレイヤークラス番号
		void SetLevel_Hp(int classNo, int level)		{ Level_Hp[classNo]			= level; }		// クラスごとのHPレベル
		void SetLevel_Ability(int classNo, int level)	{ Level_Ability[classNo]	= level; }		// クラスごとのアビリティレベル
		void SetLevel_Deck(int classNo, int level)		{ Level_Deck[classNo]		= level; }		// クラスごとのデッキレベル

		/* 定数 */
		// プレイヤークラスの番号
		static const int CLASS_TRAVELER		= 0;	// 旅人
		static const int CLASS_SWORDSMAN	= 1;	// 剣士
		static const int CLASS_WIZARD		= 2;	// 魔法使い
		static const int CLASS_ROGUE		= 3;	// 盗賊
		static const int CLASS_MARCHANT		= 4;	// 商人
		static const int CLASS_MAX			= 5;	// クラスの最大数

	private:
		/* 変数 */
		int HaveExp;					// 所持経験値
		int PlayerClassNo;				// プレイヤークラス番号
		int Level_Hp[CLASS_MAX];		// クラスごとのHPレベル
		int Level_Ability[CLASS_MAX];	// クラスごとのアビリティレベル
		int Level_Deck[CLASS_MAX];		// クラスごとのデッキレベル

		/* 関数 */
		void SaveData_Load();	// セーブデータ読み込み
		void SaveData_Save();	// セーブデータ保存
};
