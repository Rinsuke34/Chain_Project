/* データリスト"フォントデータ管理"クラスの宣言 */

#pragma once

/* 使用する要素のインクルード */
// 共通定義
#include "AppFrame.h"

// データリスト"フォントデータ管理"クラス
class DataList_Font : public DataList_Base
{
	public:
		DataList_Font();			// コンストラクタ
		virtual ~DataList_Font();	// デストラクタ

		/* 関数 */
		void	LoadFontHandle(std::string& cFilePath, int iEdgeSize);			// フォントデータ読み込み
		void	LoadFontHandle_ASync(std::string& cFilePath, int iEdgeSize);	// フォントデータ読み込み(非同期読み込み対応)
		int		iGetFontHnadle(std::string& cFilePath);							// フォントデータ取得

	private:
		/* 変数 */
		// リスト
		std::map<std::string, int>	FontHandleList;		// フォントデータリスト

		/* 関数 */
		bool	bCheckFont(std::string& cFileName);		// 該当フォントデータ存在確認
};
