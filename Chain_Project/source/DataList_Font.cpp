/* データリスト"フォントデータ管理"クラスの定義 */

/* 使用する要素のインクルード */
// ヘッダファイル
#include "DataList_Font.h"

// コンストラクタ
DataList_Font::DataList_Font() : DataList_Base("DataList_Font")
{
	/* 初期化 */
	this->FontHandleList.clear();	// 画像データリスト
}

// デストラクタ
DataList_Font::~DataList_Font()
{
	/* リスト内のフォントデータを削除 */
	for (auto& FontHandle : this->FontHandleList)
	{
		/* フォントデータの削除 */
		DeleteFontToHandle(FontHandle.second);
	}
}

// フォントデータ読み込み
void DataList_Font::LoadFontHandle(std::string& cFilePath, int iEdgeSize)
{
	// 引数
	// cFilePath	<- 読み込むフォントデータのファイル名
	// iEdgeSize	<- フォントの縁取りサイズ

	/* 対象のフォントデータが読み込まれていないか確認 */
	if (bCheckFont(cFilePath) == false)
	{
		// 読み込まれていない場合
		/* フォントのファイルパスを取得 */
		std::string FileName = "resource/FontData/" + cFilePath + ".dft";

		/* フォントデータ読み込み */
		int GrHandle = LoadFontDataToHandle(FileName.c_str(), iEdgeSize);

		/* フォントデータをリストに追加 */
		this->FontHandleList[cFilePath] = GrHandle;
	}
}

// フォントデータ読み込み(非同期読み込み対応)
void DataList_Font::LoadFontHandle_ASync(std::string& cFilePath, int iEdgeSize)
{
	// 引数
	// cFilePath	<- 読み込む画像データのファイル名
	// iEdgeSize	<- フォントの縁取りサイズ

	/* 非同期読み込みフラグを有効にする */
	SetUseASyncLoadFlag(TRUE);

	/* 対象のフォントデータを読み込む */
	LoadFontHandle(cFilePath, iEdgeSize);

	/* 非同期読み込みフラグを無効にする */
	SetUseASyncLoadFlag(FALSE);
}

// フォントデータハンドル取得
int DataList_Font::iGetFontHnadle(std::string& cFilePath)
{
	// 引数
	// cFilePath	<- 取得するフォントデータのファイル名
	// 戻り値
	// int			<- フォントデータハンドル(存在しない場合は-1)

	/* 対象のフォントを取得する */
	// ※読み込まれていないなら読み込み

	/* 対象のフォントデータが読み込まれていないか確認 */
	if (bCheckFont(cFilePath) == false)
	{
		// 読み込まれていない場合
		/* フォントデータを読み込む */
		LoadFontHandle(cFilePath, 0);
	}

	/* フォントハンドルを返す */
	for (auto& FontHandleList : this->FontHandleList)
	{
		if (FontHandleList.first == cFilePath)
		{
			return FontHandleList.second;
		}
	}

	/* 見つからなかった場合は-1を返す */
	// ※通常はここに来ることはない想定
	return -1;
}

// 該当フォントデータ存在確認
bool DataList_Font::bCheckFont(std::string& cFileName)
{
	// 引数
	// cFileName	: 読み込むフォントデータのファイル名
	// 戻り値
	// bool			: フォントデータが登録されているか(true:登録されている / false:登録されていない)

	/* 対象のフォントがリストに登録されているか確認 */

	bool bReturn = false;

	/* 対象のフォントが登録されているか */
	if (this->FontHandleList.count(cFileName) != 0)
	{
		// 登録されている場合
		bReturn = true;
	}

	return bReturn;
}
