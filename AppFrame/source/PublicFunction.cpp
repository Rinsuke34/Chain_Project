/* 汎用的に使用できる機能の定義 */

/* 使用する要素のインクルード */
// 標準ライブラリ
#include <fstream>
#include <thread>
#include <chrono>
#include <cstdio>
#include <Windows.h>
#include <filesystem>
// 関連クラス
#include "SceneServer.h"
#include "DataListServer.h"
// 共通定義
#include "AppFunctionDefine.h"
#include "AppVariableDefine.h"

// 指定ファイルを完全に削除するまで待機
void PUBLIC_FUNCTION::FileDeletesAndStand(const std::string& filename)
{
	// 引数
	// filename	<- 削除するファイル名

	/* 削除完了フラグ */
	bool bDeleteCompleteFlg = false;

	/* 指定ファイルを削除 */
	std::remove(filename.c_str());

	/* ファイルが削除されるまで待機 */
	while (bDeleteCompleteFlg == false)
	{
		/* ファイルが開けるか確認 */
		std::ifstream file(filename);
		if (file.is_open())
		{
			// ファイルが開ける場合
			/* 待機 */
			std::this_thread::sleep_for(std::chrono::milliseconds(10));
		}
		else
		{
			// ファイルが開けない場合
			/* 削除完了フラグを立てる */
			bDeleteCompleteFlg = true;
		}
	}
}

// ファイル選択ダイアログを表示
std::string	PUBLIC_FUNCTION::aOpenFileDialog(std::string Filter)
{
	// 引数
	// Filter	<- ファイル選択ダイアログのフィルター文字列(例："*.png;*.jpg;*.bmp")
	// 戻り値
	// std::string	<- 選択されたファイルのパス(選択されなかった場合は空文字列)

	/* DXライブラリのウィンドウハンドルを取得 */
	HWND WindHwnd = GetMainWindowHandle();

	/* ダイアログ作成 */
	OPENFILENAME ofn;
	ZeroMemory(&ofn, sizeof(ofn));
	ofn.lStructSize		= sizeof(ofn);
	ofn.hwndOwner		= WindHwnd;		// DXライブラリのウィンドウを親に設定
	ofn.lpstrFile		= new char[MAX_PATH];
	ofn.nMaxFile		= MAX_PATH;
	ofn.lpstrFilter		= Filter.c_str();
	ofn.nFilterIndex	= 1;
	ofn.lpstrFile[0]	= '\0';
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_EXPLORER;

	/* AppResourceフォルダが開くように設定 */
	std::filesystem::path appResourceDirPath	= std::filesystem::current_path().parent_path() / "AppResource";
	std::string appResourceDir					= appResourceDirPath.string();
	ofn.lpstrInitialDir							= appResourceDir.c_str();

	/* ダイアログ表示 */
	if (GetOpenFileName(&ofn))
	{
		// ファイルが選択された場合
		std::string selectedFile = ofn.lpstrFile;
		delete[] ofn.lpstrFile;

		/* 選択されたファイルのパスを返す */
		return selectedFile;
	}

	return "";
}

// 立方体型の線を描画
void PUBLIC_FUNCTION::DrawCubeLine3D(VECTOR aVertex[8], unsigned int iColor)
{
	// 引数
	// aVertex	<- 立方体の頂点配列(8頂点分)
	// iColor	<- 立方体の色

	DrawLine3D(aVertex[0], aVertex[1], iColor);
	DrawLine3D(aVertex[1], aVertex[5], iColor);
	DrawLine3D(aVertex[5], aVertex[4], iColor);
	DrawLine3D(aVertex[4], aVertex[0], iColor);
	DrawLine3D(aVertex[2], aVertex[3], iColor);
	DrawLine3D(aVertex[3], aVertex[7], iColor);
	DrawLine3D(aVertex[7], aVertex[6], iColor);
	DrawLine3D(aVertex[6], aVertex[2], iColor);
	DrawLine3D(aVertex[0], aVertex[2], iColor);
	DrawLine3D(aVertex[1], aVertex[3], iColor);
	DrawLine3D(aVertex[4], aVertex[6], iColor);
	DrawLine3D(aVertex[5], aVertex[7], iColor);
}

// XZ平面上に円を描画
void PUBLIC_FUNCTION::DrawCircleXZ3D(VECTOR vecCenter, float fRadius, int iDiv, int iColor)
{
	// 引数
	// vecCenter	<- 円の中心座標
	// fRadius		<- 円の半径
	// iDiv			<- 円の分割数(多いほど滑らかになる)
	// iColor		<- 円の色

	float fAngleStep = 2.0f * DX_PI_F / iDiv;

	for (int i = 0; i < iDiv; i++)
	{
		float fAngle1 = i * fAngleStep;
		float fAngle2 = (i + 1) * fAngleStep;

		VECTOR vecP1 = VGet(vecCenter.x + cosf(fAngle1) * fRadius, vecCenter.y, vecCenter.z + sinf(fAngle1) * fRadius);
		VECTOR vecP2 = VGet(vecCenter.x + cosf(fAngle2) * fRadius, vecCenter.y, vecCenter.z + sinf(fAngle2) * fRadius);

		DrawLine3D(vecP1, vecP2, iColor);
	}
}
