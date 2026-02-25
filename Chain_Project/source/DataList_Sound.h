/* データリスト"音声データ管理"クラスの宣言 */

#pragma once

/* 使用する要素のインクルード */
// 共通定義
#include "AppFrame.h"

// データリスト"音声データ管理"クラス
class DataList_Sound : public DataList_Base
{
	public:
		DataList_Sound();			// コンストラクタ
		virtual ~DataList_Sound();	// デストラクタ

		/* 関数 */
		void					LoadSound(std::string& cFilePath);			// 音声データ読み込み
		void					LoadSound_ASync(std::string& cFilePath);	// 音声データ読み込み(非同期読み込み対応)
		std::shared_ptr<int>	iGetSoundHandle(std::string& cFilePath);	// 音声データハンドル取得
		void					PlayMemSound(std::string& cFilePath);		// メモリ上の音声データ再生
		void					PlayBgmSound(std::string& cFilePath);		// BGM音声データ再生(読み込み＆再生)
		void					StopBgmSound();								// BGM音声データ停止
		void					LoadBgmSound(std::string& cFilePath);		// BGM音声データ読み込み
		void					PlayBgmSound_Now();							// 現在読み込まれているBGM音声データ再生
		bool					CheckPlayBgm();								// 現在再生中のBGMがあるか

	private:
		/* 変数 */
		// SE
		std::map<std::string, int>	SoundList;		// 音声データリスト
		// BGM
		int							PlayBgm;		// 再生中のBGM
		std::string					NowPlayBgmName;	// 現在再生中のBGM名

		/* 関数 */
		bool	bCheckSound(std::string& cFileName);	// 該当音声データ存在確認

};
