/* シーン"ビルド設定画面"の宣言 */

#pragma once

/* 使用する要素のインクルード */
// 共通定義
#include "AppFrame.h"

// 前方宣言
class Card_Base;
class Scene_UI_Button;
class DataList_SaveData;

// シーン"ビルド設定画面"
class Scene_Build : public Scene_Base
{
	public:
		Scene_Build();				// コンストラクタ
		virtual ~Scene_Build();		// デストラクタ

		/* 関数 */
		virtual void	Update()			override;	// 更新
		virtual void	Draw()				override;	// 描画

		/* 定数 */
		static const int CARD_INTERVAL		= 170;	// カードの間隔

	private:
		/* 変数 */
		// データリスト
		std::shared_ptr<DataList_SaveData> pDataList_SaveData;	// セーブデータ管理データリスト
		// カードリスト
		std::vector<std::shared_ptr<Card_Base>> ClassCardList;	// クラスカードリスト
		std::vector<std::shared_ptr<Card_Base>> DeckCardList;	// デッキカードリスト
		// UIのハンドル
		std::shared_ptr<Scene_UI_Button> UI_Button[3];	// ボタンUI[0:HP強化, 1:アビリティ強化, 2:デッキ強化]
		// 画像
		std::shared_ptr<int> Image_BackGround;		// 背景画像
		std::shared_ptr<int> Image_Frame_Corner;	// 角
		std::shared_ptr<int> Image_Frame_Line;		// 線
		std::shared_ptr<int> Image_Frame_Inside;	// 中央

		/* 関数 */
		void AddButton();			// ボタン追加
		void SetCard_Class();		// クラスカードの設定
		void SetCard_Deck();		// デッキカードの設定
		void SetCardPosition();		// カードの位置を設定
		void Updaate_SelectClass();	// クラス選択の更新
		void Draw_Class();			// クラス関連の描写
		void Draw_Hp();				// HP関連の描写
		void Draw_Ability();		// アビリティ関連の描写
		void Draw_Deck();			// デッキ関連の描写
};
