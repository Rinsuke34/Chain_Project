/* シーン"UI - 説明テキスト"の定義 */

/* 使用する要素のインクルード */
// ヘッダファイル
#include "Scene_UI_ExplanationText.h"
// 関連クラス
#include "DataList_Image.h"
// 共通定義
#include "VariableDefine.h"
#include "FunctionDefine.h"

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
	/* 文字列の描写 */
	// ※文字列にコマンドを埋め込んで対応
	// ・色変更：/cys (黄色開始)、/ce (色終了、白に戻す)
	// ・改行：/n

	// 元文字列（UTF-8 を想定）
	const std::string& text = this->ExplanationText;
	std::wstring wtext = PUBLIC_PROCESS::MByteToWstring(text);

	const int lineHeight		= 20;	// 行の高さ
	const int maxCharsPerLine	= 8;	// 1行あたりの最大文字数（全角換算、半角なら2文字分）

	/* 1. 行数を算出 */
	int totalLines = 1;
	int countInLine = 0;
	for (size_t i = 0; i < wtext.size(); )
	{
		// コマンド長
		const std::wstring markerStart	= L"/cys";
		const std::wstring markerEnd	= L"/ce";
		const std::wstring slashN		= L"/n";

		if (i + slashN.size() <= wtext.size() && wtext.substr(i, slashN.size()) == slashN)
		{
			totalLines++;
			countInLine = 0;
			i += slashN.size();
			continue;
		}
		if (i + markerStart.size() <= wtext.size() && wtext.substr(i, markerStart.size()) == markerStart)
		{
			i += markerStart.size();
			continue;
		}
		if (i + markerEnd.size() <= wtext.size() && wtext.substr(i, markerEnd.size()) == markerEnd)
		{
			i += markerEnd.size();
			continue;
		}
		if (wtext[i] == L'\n')
		{
			totalLines++;
			countInLine = 0;
			++i;
			continue;
		}
		++countInLine;
		++i;
		if (countInLine >= maxCharsPerLine)
		{
			totalLines++;
			countInLine = 0;
		}
	}

	/* 2. 描画開始Y座標を決定 */
	int baseX = this->Base_Pos.iX;
	int baseY = this->Base_Pos.iY;
	int startY = baseY;

	if (this->UpwardDisplayFlg)
	{
		// 上方向描写：基準位置から上方向へ描写
		startY = baseY - (totalLines) * lineHeight - FRAME_THICKNESS;
	}
	else
	{
		// 下方向描写：基準座標から下方向へ描写
		startY = baseY + FRAME_THICKNESS;
	}

	/* 3. 描写開始X座標を決定 */
	baseX = baseX - (maxCharsPerLine * 20) / 2;

	/* 4. 背景を描写 */
	// 横幅は最大8文字分（全角16px/半角8pxで128px程度）、高さはlineHeight*totalLines
	const int boxWidth	= maxCharsPerLine * 20;
	const int boxHeight	= lineHeight * totalLines;
	DRAW_FUNCTION::DrawFrame_Image(
		{ baseX + boxWidth / 2, startY + boxHeight / 2 },
		{ boxWidth, boxHeight },
		FRAME_THICKNESS,
		*(this->Image_Frame_Corner),
		*(this->Image_Frame_Line),
		*(this->Image_Frame_Inside)
	);

	/* 5. 文字列描写 */
	int x = baseX;
	int y = startY;
	unsigned int colorWhite		= GetColor(255, 255, 255);
	unsigned int colorYellow	= GetColor(255, 255, 0);
	unsigned int curColor		= colorWhite;
	countInLine					= 0;
	std::wstring buffer;

	auto flushBuffer = [&]()
	{
		if (buffer.empty()) return;
		std::string bytes = PUBLIC_PROCESS::WstringToShiftJIS(buffer);
		DrawStringToHandle(x, y, bytes.c_str(), curColor, giFont_JF_Dot_MPlus10_20);
		int approxAdvance = 0;
		for (wchar_t wc : buffer)
		{
			if (wc <= 0x007F) approxAdvance += 8;
			else approxAdvance += 16;
		}
		x += approxAdvance;
		buffer.clear();
	};

	const std::wstring markerStart	= L"/cys";
	const std::wstring markerEnd	= L"/ce";
	const std::wstring slashN		= L"/n";

	for (size_t i = 0; i < wtext.size(); )
	{
		if (i + slashN.size() <= wtext.size() && wtext.substr(i, slashN.size()) == slashN) {
			flushBuffer();
			y += lineHeight;
			x = baseX;
			countInLine = 0;
			i += slashN.size();
			continue;
		}
		if (i + markerStart.size() <= wtext.size() && wtext.substr(i, markerStart.size()) == markerStart) {
			flushBuffer();
			curColor = colorYellow;
			i += markerStart.size();
			continue;
		}
		if (i + markerEnd.size() <= wtext.size() && wtext.substr(i, markerEnd.size()) == markerEnd) {
			flushBuffer();
			curColor = colorWhite;
			i += markerEnd.size();
			continue;
		}
		if (wtext[i] == L'\n') {
			flushBuffer();
			y += lineHeight;
			x = baseX;
			countInLine = 0;
			++i;
			continue;
		}
		buffer.push_back(wtext[i]);
		++i;
		++countInLine;
		if (countInLine >= maxCharsPerLine) {
			flushBuffer();
			y += lineHeight;
			x = baseX;
			countInLine = 0;
		}
	}
	flushBuffer();
}

// 背景描写
void Scene_UI_ExplanationText::Draw_BackGround()
{

}
