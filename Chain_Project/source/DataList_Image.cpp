/* データリスト"画像データ管理"クラスの定義 */

/* 使用する要素のインクルード */
// ヘッダファイル
#include "DataList_Image.h"

// コンストラクタ
DataList_Image::DataList_Image() : DataList_Base("DataList_Image")
{
	/* 初期化 */
	this->ImageHandleList.clear();	// 画像データリスト
}

// デストラクタ
DataList_Image::~DataList_Image()
{
	/* リスト内の画像データを削除 */
	for (auto& ImageHandle : this->ImageHandleList)
	{
		/* 画像データの削除 */
		DeleteGraph(ImageHandle.second);
	}
}

// 画像データ読み込み
void DataList_Image::LoadImageHandle(std::string& cFilePath)
{
	// 引数
	// cFilePath	<- 読み込む画像データのファイル名

	/* 対象の画像データが読み込まれていないか確認 */
	if (bCheckImage(cFilePath) == false)
	{
		// 読み込まれていない場合
		/* 画像のファイルパスを取得 */
		std::string FileName = "resource/ImageData/" + cFilePath + ".png";

		/* 画像データ読み込み */
		int GrHandle = LoadGraph(FileName.c_str());

		/* 画像データをリストに追加 */
		this->ImageHandleList[cFilePath] = GrHandle;
	}
}

// 画像データ読み込み(非同期読み込み対応)
void DataList_Image::LoadImageHandle_ASync(std::string& cFilePath)
{
	// 引数
	// cFilePath	<- 読み込む画像データのファイル名

	/* 非同期読み込みフラグを有効にする */
	SetUseASyncLoadFlag(TRUE);

	/* 対象の画像データを読み込む */
	LoadImageHandle(cFilePath);

	/* 非同期読み込みフラグを無効にする */
	SetUseASyncLoadFlag(FALSE);
}

// 画像データハンドル取得
std::shared_ptr<int> DataList_Image::iGetImageHandle(std::string& cFilePath)
{
	// 引数
	// cFilePath	<- 取得する画像データのファイル名
	// 戻り値
	// std::shared_ptr<int>	<- 画像データハンドル(存在しない場合はnullptr)

	/* 対象の画像を取得する */
	// ※読み込まれていないなら読み込み

	/* 対象の画像データが読み込まれていないか確認 */
	if (bCheckImage(cFilePath) == false)
	{
		// 読み込まれていない場合
		/* 画像データを読み込む */
		LoadImageHandle(cFilePath);
	}

	/* 画像ハンドルのポインタをスマートポインタで返す */
	auto list = ImageHandleList.find(cFilePath);
	if (list != ImageHandleList.end())
	{
		/* 対象の画像ハンドルのポインタを戻り値で返却 */
		return std::make_shared<int>(list->second);
	}

	/* 見つからなかった場合はnullptrを返す */
	// ※通常はここに来ることはない想定
	return nullptr;
}

// 該当画像データ存在確認
bool DataList_Image::bCheckImage(std::string& cFileName)
{
	// 引数
	// cFileName	: 読み込む画像データのファイル名
	// 戻り値
	// bool			: 画像データが登録されているか(true:登録されている / false:登録されていない)

	/* 対象の画像がリストに登録されているか確認 */

	bool bReturn = false;

	/* 対象の画像が登録されているか */
	if (this->ImageHandleList.count(cFileName) != 0)
	{
		// 登録されている場合
		bReturn = true;
	}

	return bReturn;
}
