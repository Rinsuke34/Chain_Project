/* データリスト"音声データ管理"クラスの定義 */

/* 使用する要素のインクルード */
// ヘッダファイル
#include "DataList_Sound.h"

// コンストラクタ
DataList_Sound::DataList_Sound() : DataList_Base("DataList_Sound")
{
	this->SoundList.clear();	// 音声データリスト
	this->PlayBgm	= -1;		// 再生中のBGM
	this->NowPlayBgmName = "";	// 現在再生中のBGM名
}

// デストラクタ
DataList_Sound::~DataList_Sound()
{
	for (auto& Sound : SoundList)
	{
		/* 音声データの削除 */
		DeleteSoundMem(Sound.second);
	}
}

// 音声データ読み込み
void DataList_Sound::LoadSound(std::string& cFilePath)
{
	// 引数
	// cFilePath	<- 読み込む音声データのファイル名

	/* 対象の音声データが読み込まれていないか確認 */
	if (bCheckSound(cFilePath) == false)
	{
		// 読み込まれていない場合
		/* 音声のファイルパスを取得 */
		std::string FileName = "resource/SoundData/SE/" + cFilePath + ".wav";

		/* 音声データ読み込み */
		int SoundHandle = LoadSoundMem(FileName.c_str());

		/* 音声データをリストに追加 */
		this->SoundList[cFilePath] = SoundHandle;
	}
}

// 音声データ読み込み(非同期読み込み対応)
void DataList_Sound::LoadSound_ASync(std::string& cFilePath)
{
	// 引数
	// cFilePath	<- 読み込む音声データのファイル名

	/* 非同期読み込みフラグを有効にする */
	SetUseASyncLoadFlag(TRUE);

	/* 対象の音声データを読み込む */
	LoadSound(cFilePath);

	/* 非同期読み込みフラグを無効にする */
	SetUseASyncLoadFlag(FALSE);
}

// 音声データハンドル取得
std::shared_ptr<int> DataList_Sound::iGetSoundHandle(std::string& cFilePath)
{
	// 引数
	// cFilePath			<- 取得する音声データのファイル名
	// 戻り値
	// std::shared_ptr<int>	<- 音声データハンドル(存在しない場合はnullptr)

	/* 対象の音声を取得する */
	// ※読み込まれていないなら読み込み

	/* 対象の音声データが読み込まれていないか確認 */
	if (bCheckSound(cFilePath) == false)
	{
		// 読み込まれていない場合
		/* 音声データを読み込む */
		LoadSound(cFilePath);
	}

	/* 音声ハンドルのポインタをスマートポインタで返す */
	auto list = SoundList.find(cFilePath);
	if (list != SoundList.end())
	{
		/* 対象の音声ハンドルのポインタを戻り値で返却 */
		return std::make_shared<int>(list->second);
	}

	/* 見つからなかった場合はnullptrを返す */
	// ※通常はここに来ることはない想定
	return nullptr;
}

// 該当音声データ存在確認
bool DataList_Sound::bCheckSound(std::string& cFileName)
{
	// 引数
	// cFileName	: 読み込む音声データのファイル名
	// 戻り値
	// bool			: 音声データが登録されているか(true:登録されている / false:登録されていない)

	/* 対象の音声がリストに登録されているか確認 */

	bool bReturn = false;

	/* 対象の音声が登録されているか */
	if (this->SoundList.count(cFileName) != 0)
	{
		// 登録されている場合
		bReturn = true;
	}

	return bReturn;
}

// メモリ上の音声データ再生
void DataList_Sound::PlayMemSound(std::string& cFilePath)
{
	// 引数
	// cFilePath	<- 再生する音声データのファイル名

	/* 音声データハンドルを取得 */
	std::shared_ptr<int> pSoundHandle = iGetSoundHandle(cFilePath);

	/* 音声データを再生 */
	if (pSoundHandle && *pSoundHandle >= 0)
	{
		PlaySoundMem(*pSoundHandle, DX_PLAYTYPE_BACK);
	}
}

// BGM音声データ再生(読み込み＆再生)
void DataList_Sound::PlayBgmSound(std::string& cFilePath)
{
	// 引数
	// cFilePath	<- 再生する音声データのファイル名

	/* 現在再生しているBGMであるなら処理をスキップする */
	if (this->NowPlayBgmName == cFilePath)
	{
		// 同じBGMである場合、処理を抜ける
		return;
	}
	else
	{
		// 異なるBGMである場合、現在のBGM名を更新する
		this->NowPlayBgmName = cFilePath;
	}

	/* 現在BGMが再生されているか確認 */
	if (this->PlayBgm != -1)
	{
		if (CheckSoundMem(this->PlayBgm) == TRUE)
		{
			// 再生されている場合
			/* BGMを停止＆メモリ上から削除 */
			StopSoundMem(this->PlayBgm);
			DeleteSoundMem(this->PlayBgm);
		}
	}

	/* BGMを読み込む */
	std::string FileName = "resource/SoundData/BGM/" + cFilePath + ".ogg";
	this->PlayBgm = LoadSoundMem(FileName.c_str());

	/* BGMを再生 */
	if (this->PlayBgm >= 0)
	{
		ChangeNextPlayVolumeSoundMem(75, this->PlayBgm);
		PlaySoundMem(this->PlayBgm, DX_PLAYTYPE_LOOP);
	}
}

// BGM音声データ停止
void DataList_Sound::StopBgmSound()
{
	/* 現在BGMが再生されているか確認 */
	if (CheckSoundMem(this->PlayBgm) == TRUE)
	{
		// 再生されている場合
		/* BGMを停止＆メモリ上から削除 */
		StopSoundMem(this->PlayBgm);
		DeleteSoundMem(this->PlayBgm);

		/* 再生中BGM情報を初期化 */
		this->NowPlayBgmName = "";
	}
}

// BGM音声データ読み込み
void DataList_Sound::LoadBgmSound(std::string& cFilePath)
{
	// 引数
	// cFilePath	<- 再生する音声データのファイル名

	/* 現在再生しているBGMであるなら処理をスキップする */
	if (this->NowPlayBgmName == cFilePath)
	{
		// 同じBGMである場合、処理を抜ける
		return;
	}
	else
	{
		// 異なるBGMである場合、現在のBGM名を更新する
		this->NowPlayBgmName = cFilePath;
	}

	/* 現在BGMが再生されているか確認 */
	if (this->PlayBgm != -1)
	{
		if (CheckSoundMem(this->PlayBgm) == TRUE)
		{
			// 再生されている場合
			/* BGMを停止＆メモリ上から削除 */
			StopSoundMem(this->PlayBgm);
			DeleteSoundMem(this->PlayBgm);
		}
	}

	/* BGMを読み込む */
	std::string FileName = "resource/SoundData/BGM/" + cFilePath + ".ogg";
	this->PlayBgm = LoadSoundMem(FileName.c_str());
}

// 現在読み込まれているBGM音声データ再生
void DataList_Sound::PlayBgmSound_Now()
{
	/* BGMを読み込む */
	std::string FileName = "resource/SoundData/BGM/" + this->NowPlayBgmName + ".ogg";
	this->PlayBgm = LoadSoundMem(FileName.c_str());

	/* BGMを再生 */
	ChangeNextPlayVolumeSoundMem(75, this->PlayBgm);
	PlaySoundMem(this->PlayBgm, DX_PLAYTYPE_LOOP);
}
