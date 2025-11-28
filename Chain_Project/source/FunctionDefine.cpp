/* メインのプロジェクトで使用する関数の定義 */

/* 使用する要素のインクルード */
// ヘッダファイル
#include "FunctionDefine.h"
// 標準ライブラリ
#include <algorithm>
// 関連クラス
#include "Scene_Load_Checker.h"
#include "Scene_Load_Draw.h"
#include "DataList_Image.h"

// ロード関係
namespace LOAD_FUNCTION
{
	// ロード画面シーンの追加
	void	AddLoadScene()
	{
		gpSceneServer->AddSceneReservation(std::make_shared<Scene_Load_Checker>());
		gpSceneServer->AddSceneReservation(std::make_shared<Scene_Load_Draw>());
	}
}

// 描写関係
namespace DRAW_FUNCTION
{
	// 鎖の描写
	void DrawChain(Struct_2D::POSITION StartPos, Struct_2D::POSITION EndPos)
	{
		// 引数
		// StartPos	<- 鎖の開始位置
		// EndPos	<- 鎖の終了位置

		/* データリスト取得 */
		std::shared_ptr<DataList_Image> pDataList_Image = std::dynamic_pointer_cast<DataList_Image>(gpDataListServer->GetDataList("DataList_Image"));
		if (!pDataList_Image) return;

		/* 鎖の画像を取得 */
		std::string ImageFilePath = "Card_Commoon/Chain";
		std::shared_ptr<int> Image = pDataList_Image->iGetImageHandle(ImageFilePath);
		if (!Image || *Image < 0) return;

		// 画像サイズ取得
		int SizeX = 0, SizeY = 0;
		GetGraphSize(*Image, &SizeX, &SizeY);
		if (SizeX <= 0 || SizeY <= 0) return;

		// ベクトルと長さ
		double dx = static_cast<double>(EndPos.iX - StartPos.iX);
		double dy = static_cast<double>(EndPos.iY - StartPos.iY);
		double dChainLength = std::sqrt(dx * dx + dy * dy);
		if (dChainLength <= 0.00001) return;

		// 単位方向ベクトル
		double ux = dx / dChainLength;
		double uy = dy / dChainLength;

		// 回転角（ラジアン）
		double rad = std::atan2(dy, dx);
		double cosA = std::cos(rad);
		double sinA = std::sin(rad);

		// 画像サイズをステップに使う（幅と高さが同じならどちらでも可）
		double step = static_cast<double>(SizeX);
		// 中央寄せするため半ステップ分ずらして開始
		double offset = step * 0.5;

		const double halfW = static_cast<double>(SizeX) * 0.5;
		const double halfH = static_cast<double>(SizeY) * 0.5;

		// 鎖の描写（中心から等間隔に配置）
		for (double d = offset; d < dChainLength; d += step)
		{
			// 中心座標
			double cx = static_cast<double>(StartPos.iX) + ux * d;
			double cy = static_cast<double>(StartPos.iY) + uy * d;

			// 矩形の4頂点（回転前のオフセット）
			// 左上, 右上, 右下, 左下 の順に回転して描画
			double ox1 = -halfW; double oy1 = -halfH; // 左上
			double ox2 = halfW; double oy2 = -halfH; // 右上
			double ox3 = halfW; double oy3 = halfH; // 右下
			double ox4 = -halfW; double oy4 = halfH; // 左下

			// 回転して座標を算出
			int x1 = static_cast<int>(std::round(cx + (ox1 * cosA - oy1 * sinA)));
			int y1 = static_cast<int>(std::round(cy + (ox1 * sinA + oy1 * cosA)));
			int x2 = static_cast<int>(std::round(cx + (ox2 * cosA - oy2 * sinA)));
			int y2 = static_cast<int>(std::round(cy + (ox2 * sinA + oy2 * cosA)));
			int x3 = static_cast<int>(std::round(cx + (ox3 * cosA - oy3 * sinA)));
			int y3 = static_cast<int>(std::round(cy + (ox3 * sinA + oy3 * cosA)));
			int x4 = static_cast<int>(std::round(cx + (ox4 * cosA - oy4 * sinA)));
			int y4 = static_cast<int>(std::round(cy + (ox4 * sinA + oy4 * cosA)));

			DrawModiGraph(
				x1, y1,
				x2, y2,
				x3, y3,
				x4, y4,
				(*Image),
				TRUE
			);
		}
	}
}