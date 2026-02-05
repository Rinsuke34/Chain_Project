/* シーン"UI - 説明テキスト"の定義 */

/* 使用する要素のインクルード */
// ヘッダファイル
#include "Scene_UI_ExplanationText.h"
// 関連クラス
#include "DataList_Image.h"
// 共通定義
#include "VariableDefine.h"

// コンストラクタ
Scene_UI_ExplanationText::Scene_UI_ExplanationText(const int iLayer) : Scene_Base("Scene_UI_ExplanationText", iLayer, false, false)
{
	/* 初期化 */
	this->Base_Pos			= { 0, 0 };		// 基準座標
	this->UpwardDisplayFlg	= false;		// 上方向表示フラグ(有効なら基準座標から上側へむけて描写する)

	/* 画像読み込み */
	// 画像管理データリスト取得
	std::shared_ptr<DataList_Image> pDataList_Image = std::dynamic_pointer_cast<DataList_Image>(gpDataListServer->GetDataList("DataList_Image"));
	// 角
	std::string ImageFilePath = "UI/Button/Button_Frame_Corner";
	this->Image_Frame_Corner = pDataList_Image->iGetImageHandle(ImageFilePath);
	// 線
	ImageFilePath = "UI/Button/Button_Frame_Line";
	this->Image_Frame_Line = pDataList_Image->iGetImageHandle(ImageFilePath);
	// 内側
	ImageFilePath = "UI/Button/Button_Frame_Inside";
	this->Image_Frame_Inside = pDataList_Image->iGetImageHandle(ImageFilePath);
}

// 更新
void Scene_UI_ExplanationText::Update()
{
	/* 基準座標に応じて描写方向を設定 */
	if (this->Base_Pos.iY <= SCREEN_SIZE_HEIGHT / 2)
	{
		// 基準座標が画面上半分の場合、上方向へ描写
		this->UpwardDisplayFlg = true;
	}
	else
	{
		// 基準座標が画面下半分の場合、下方向へ描写
		this->UpwardDisplayFlg = false;
	}
}

// 描画
void Scene_UI_ExplanationText::Draw()
{
	// 元文字列（UTF-8 を想定）
	const std::string& text = this->ExplanationText;

	// マルチバイト(プロジェクト設定) -> wstring
	std::wstring wtext = PUBLIC_PROCESS::MByteToWstring(text);

	// 描画開始位置（Base_Pos を基準）
	int baseX = this->Base_Pos.iX;
	int baseY = this->Base_Pos.iY;
	int x = baseX;
	int y = baseY;

	// 行高（フォントサイズに合わせて調整）
	const int lineHeight = 20;

	// 描画色
	const unsigned int colorWhite = GetColor(255, 255, 255);
	const unsigned int colorYellow = GetColor(255, 255, 0);
	unsigned int curColor = colorWhite;

	// 1行あたりの文字カウント（表示カウント）。8文字で改行。
	int countInLine = 0;

	// バッファ（同じ色の文字をまとめる）
	std::wstring buffer;

	auto flushBuffer = [&](void) {
		if (buffer.empty()) return;
		// 共通関数で Shift_JIS(CP932) に変換して描画
		std::string bytes = PUBLIC_PROCESS::WstringToShiftJIS(buffer);
		DrawStringToHandle(x, y, bytes.c_str(), curColor, giFont_JF_Dot_MPlus10_20);
		// 幅進行：GetDrawStringWidthToHandle が使える場合はそちらを使うのが望ましいが、
		// 簡易的な進行で足りる場合は下記で対応
		int approxAdvance = 0;
		for (wchar_t wc : buffer) {
			if (wc <= 0x007F) approxAdvance += 8;
			else approxAdvance += 16;
		}
		x += approxAdvance;
		buffer.clear();
		};

	// マーカーは "/cys" "/ce"、改行は "/n" に対応
	const std::wstring markerStart = L"/cys";
	const std::wstring markerEnd = L"/ce";
	const std::wstring slashN = L"/n";

	for (size_t i = 0; i < wtext.size(); )
	{
		// "/n" を改行として扱う
		if (i + slashN.size() <= wtext.size() && wtext.substr(i, slashN.size()) == slashN)
		{
			flushBuffer();
			// 次行位置の計算（上方向なら減算、下方向なら加算）
			if (this->UpwardDisplayFlg) y -= lineHeight; else y += lineHeight;
			x = baseX;
			countInLine = 0;
			i += slashN.size();
			continue;
		}

		// マーカー検出（/cys /ce）
		if (i + markerStart.size() <= wtext.size() && wtext.substr(i, markerStart.size()) == markerStart)
		{
			flushBuffer();
			curColor = colorYellow;
			i += markerStart.size();
			continue;
		}
		if (i + markerEnd.size() <= wtext.size() && wtext.substr(i, markerEnd.size()) == markerEnd)
		{
			flushBuffer();
			curColor = colorWhite;
			i += markerEnd.size();
			continue;
		}

		// 既存の改行文字 '\n' もサポート
		if (wtext[i] == L'\n')
		{
			flushBuffer();
			if (this->UpwardDisplayFlg) y -= lineHeight; else y += lineHeight;
			x = baseX;
			countInLine = 0;
			++i;
			continue;
		}

		// 通常文字をバッファへ追加
		buffer.push_back(wtext[i]);
		++i;
		++countInLine;

		// 8文字で改行
		if (countInLine >= 8)
		{
			flushBuffer();
			if (this->UpwardDisplayFlg) y -= lineHeight; else y += lineHeight;
			x = baseX;
			countInLine = 0;
		}
	}

	// 残りを描画
	flushBuffer();
}

// 背景描写
void Scene_UI_ExplanationText::Draw_BackGround()
{

}
