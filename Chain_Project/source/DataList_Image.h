/* データリスト"画像データ管理"クラスの宣言 */

#pragma once

/* 使用する要素のインクルード */
// 共通定義
#include "AppFrame.h"

// データリスト"画像データ管理"クラス
class DataList_Image : public DataList_Base
{
	public:
		DataList_Image();			// コンストラクタ
		virtual ~DataList_Image();	// デストラクタ

		/* 関数 */
		void					LoadImageHandle(std::string& cFilePath);		// 画像データ読み込み
		void					LoadImageHandle_ASync(std::string& cFilePath);	// 画像データ読み込み(非同期読み込み対応)
		std::shared_ptr<int>	iGetImageHandle(std::string& cFilePath);		// 画像データハンドル取得

	private:
		/* 変数 */
		// リスト
		std::map<std::string, int>	ImageHandleList;	// 画像データリスト

		/* 関数 */
		bool	bCheckImage(std::string& cFileName);	// 該当画像データ存在確認
};