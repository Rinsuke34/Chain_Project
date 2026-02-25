/* シーン"ショップ画面"の定義 */

/* 使用する要素のインクルード */
// ヘッダファイル
#include "Scene_Shop.h"
// 共通定義
#include "VariableDefine.h"
#include "FunctionDefine.h"
// 関連クラス
#include "DataList_GameResource.h"
#include "DataList_Image.h"
#include "Scene_UI_Button.h"
#include "Card_Base.h"
#include "Scene_UI_ExplanationText.h"
#include "DataList_SaveData.h"

/* 商品クラス */
// コンストラクタ
Shop_Item::Shop_Item(std::shared_ptr<Card_Base> card, int price)
{
	/* 初期化 */
	this->ItemCard				= card;		// 商品カード
	this->ItemPrice				= price;	// 商品価格
	this->SoldOutFlg			= false;	// 売り切れフラグ
	this->CardCenterPosition	= { 0, 0 }; // カード中心位置

	/* プレイヤーが商人であるなら商品価格を半額にする */
	std::shared_ptr<DataList_SaveData> pDataList_SaveData = std::dynamic_pointer_cast<DataList_SaveData>(gpDataListServer->GetDataList("DataList_SaveData"));
	if (pDataList_SaveData->GetPlayerClassNo() == DataList_SaveData::CLASS_MARCHANT)
	{
		this->ItemPrice /= 2;
	}
}

/* ショップクラス */
// コンストラクタ
Scene_Shop::Scene_Shop() : Scene_Base("Scene_Shop", 10, false, false)
{
	/* 初期化 */
	this->ShopEndFlg	= false;		// ショップ終了フラグ

	/* データリスト取得 */
	// ゲームリソース管理用データリスト
	this->pDataList_GameResource = std::dynamic_pointer_cast<DataList_GameResource>(gpDataListServer->GetDataList("DataList_GameResource"));

	/* UI作成 */
	{
		/* "決定"ボタンの作成 */
		this->UI_DecisionButton = std::make_shared<Scene_UI_Button>("Battle_DecisionButton", this->iLayerOrder + 1);
		this->UI_DecisionButton->SetButtonText("しゅうりょう");
		this->UI_DecisionButton->SetCenterPos({1550, 950 });
		this->UI_DecisionButton->SetFontHandle(giFont_DonguriDuel_32);
		gpSceneServer->AddSceneReservation(this->UI_DecisionButton);
	}

	/* 画像読み込み */
	{
		// 画像管理データリスト
		std::shared_ptr<DataList_Image> pDataList_Image = std::dynamic_pointer_cast<DataList_Image>(gpDataListServer->GetDataList("DataList_Image"));

		// 背景画像
		std::string ImageFilePath = "BackGround/Shop_BackGround";
		this->Image_BackGround = pDataList_Image->iGetImageHandle(ImageFilePath);
		// 背景の枠の画像
		ImageFilePath = "UI/Button/Button_Frame_Corner";
		this->Image_Frame_Corner = pDataList_Image->iGetImageHandle(ImageFilePath);
		ImageFilePath = "UI/Button/Button_Frame_Line";
		this->Image_Frame_Line = pDataList_Image->iGetImageHandle(ImageFilePath);
		ImageFilePath = "UI/Button/Button_Frame_Inside";
		this->Image_Frame_Inside = pDataList_Image->iGetImageHandle(ImageFilePath);
		// コイン
		ImageFilePath = "Common_Icon/Icon_Coin";
		this->Image_Coin = pDataList_Image->iGetImageHandle(ImageFilePath);
	}
}

// デストラクタ
Scene_Shop::~Scene_Shop()
{
	/* 紐づいたUIを削除 */
	if (this->UI_DecisionButton)
	{
		this->UI_DecisionButton->SetDeleteFlg(true);
		this->UI_DecisionButton = nullptr;
	}
	if (this->UI_ExplanationText)
	{
		this->UI_ExplanationText->SetDeleteFlg(true);
		this->UI_ExplanationText = nullptr;
	}
}

// 更新
void Scene_Shop::Update()
{
	/* ショップ終了確認 */
	ShopEnd_Check();

	/* ショップ商品購入確認 */
	ShopItem_BuyCheck();

	/* 説明文設定 */
	Update_Explanation();
}

// 描画
void Scene_Shop::Draw()
{
	/* 背景画像 */
	Draw_BackGround();

	/* ショップ商品描写 */
	Draw_ShopItems();
}

// 背景描写
void Scene_Shop::Draw_BackGround()
{
	/* 背景画像の描写 */
	DrawModiGraph(
		0, 0,
		SCREEN_SIZE_WIDE, 0,
		SCREEN_SIZE_WIDE, SCREEN_SIZE_HEIGHT,
		0, SCREEN_SIZE_HEIGHT,
		*(this->Image_BackGround),
		TRUE
	);

	/* 背景、フレームの描写 */
	DRAW_FUNCTION::DrawFrame_Image(
		{ SCREEN_SIZE_WIDE / 2, (SCREEN_SIZE_HEIGHT / 2) + 200 },
		{ 1500 - (32 * 2), 600 - (32 * 2) },
		32,
		*(this->Image_Frame_Corner),
		*(this->Image_Frame_Line),
		*(this->Image_Frame_Inside)
	);
}

// ショップ商品描写
void Scene_Shop::Draw_ShopItems()
{
	/* 商品リストを描写 */
	for (auto& ShopItem : this->ShopItemList)
	{
		/* リスト内のカードを描写 */
		ShopItem->GetCard()->Draw();
		
		/* 売り切れであるか確認 */
		if (ShopItem->GetSoldOutFlg())
		{
			// 売り切れである場合
			/* 売り切れテキスト描写 */
			std::string PriceText = "うりきれ";
			int TextSize = GetFontSizeToHandle(giFont_DonguriDuel_32) * 4;
			int StartPosX = ShopItem->GetCardCenterPosition().iX - (TextSize / 2);
			DrawStringToHandle(
				StartPosX,
				ShopItem->GetCardCenterPosition().iY + 150 - 16,
				PriceText.c_str(),
				GetColor(255, 255, 0),
				giFont_DonguriDuel_32
			);
		}
		else
		{
			// 売り切れでない場合
			/* 描写座標算出 */
			std::string PriceText = " × " + std::to_string(ShopItem->GetPrice());
			int CoinSize = 20;
			int TextSize = GetDrawStringWidth(PriceText.c_str(), static_cast<int>(strlenDx(PriceText.c_str())), giFont_DonguriDuel_32);
			int StartPosX = ShopItem->GetCardCenterPosition().iX - ((TextSize + CoinSize) / 2);

			/* コイン描写 */
			int CoinCenterX = StartPosX;
			int CoinCenterY = ShopItem->GetCardCenterPosition().iY + 150;
			DrawModiGraph(
				CoinCenterX - CoinSize, CoinCenterY - CoinSize,
				CoinCenterX + CoinSize, CoinCenterY - CoinSize,
				CoinCenterX + CoinSize, CoinCenterY + CoinSize,
				CoinCenterX - CoinSize, CoinCenterY + CoinSize,
				*(this->Image_Coin),
				TRUE
			);

			/* 価格描写 */
			DrawStringToHandle(
				StartPosX + CoinSize,
				ShopItem->GetCardCenterPosition().iY + 150 - 16,
				PriceText.c_str(),
				GetColor(255, 255, 0),
				giFont_DonguriDuel_32
			);
		}		
	}
}

// カード位置セットアップ
void Scene_Shop::CardPosition_Setup()
{
	/* 商品リスト内のカードの設定座標を算出し、設定する */
	int ItemCount = static_cast<int>(this->ShopItemList.size());
	for (int i = 0; i < ItemCount; i++)
	{
		auto& ShopItem = this->ShopItemList[i];

		Struct_2D::POSITION SettingPos = {
			(SCREEN_SIZE_WIDE / 2) - ((SHOP_ITEM_INTERVAL * (ItemCount - 1)) / 2) + (SHOP_ITEM_INTERVAL * i),
			(SCREEN_SIZE_HEIGHT / 2) + 150
		};

		/* カードに設定座標を設定 */
		ShopItem->GetCard()->SetSettingPos(SettingPos);
		ShopItem->GetCard()->SetNowPos(SettingPos);

		/* ショップ商品にもカード中心位置を設定 */
		ShopItem->SetCardCenterPosition(SettingPos);
	}
}

// ショップ終了確認
void Scene_Shop::ShopEnd_Check()
{
	/* ショップ終了フラグを確認 */
	if (this->ShopEndFlg == false)
	{
		// 無効であるならば
		/* "決定"ボタンが入力されたならば */
		if (this->UI_DecisionButton->GetMouseOverFlg() && (gstKeyboardInputData.igInput[INPUT_TRG] & MOUSE_INPUT_LEFT))
		{
			/* ワールドマップを有効にする */
			this->pDataList_GameResource->SetWoldMapActiveFlg(true);

			/* ショップ終了フラグを有効化 */
			this->ShopEndFlg = true;
		}
	}
	else
	{
		// 有効であるならば
		/* 次のステージの選択が完了しているならこのシーンを削除 */
		if (this->pDataList_GameResource->GetNextStageSelectFlg())
		{
			/* このシーンの削除フラグを有効にする */
			this->bDeleteFlg = true;

			/* 次のステージ選択を未完了に戻す */
			this->pDataList_GameResource->SetNextStageSelectedFlg(false);
		}
	}
}

// ショップ商品購入確認
void Scene_Shop::ShopItem_BuyCheck()
{
	/* 商品リスト内の各商品について購入確認を行う */
	for (auto& ShopItem : this->ShopItemList)
	{
		/* マウスオーバーしているか確認 */
		if (ShopItem->GetCard()->MouseInCard())
		{
			// マウスオーバーしている場合
			/* 売り切れでないか確認 */
			if (!ShopItem->GetSoldOutFlg())
			{
				// 売り切れでない場合
				/* 所持コインが足りているか確認 */
				if (this->pDataList_GameResource->GetHaveCoin() >= ShopItem->GetPrice())
				{
					// 足りている場合
					/* マウス左クリックトリガーが入力されたか確認 */
					if (gstKeyboardInputData.igInput[INPUT_TRG] & MOUSE_INPUT_LEFT)
					{
						// 入力された場合
						/* 所持コインを減算 */
						int NewCoin = this->pDataList_GameResource->GetHaveCoin() - ShopItem->GetPrice();
						this->pDataList_GameResource->SetHaveCoin(NewCoin);

						/* 商品カードをデッキに追加 */
						auto DeckCardList = this->pDataList_GameResource->GetDeckCardList();
						DeckCardList.push_back(ShopItem->GetCard());
						this->pDataList_GameResource->SetDeckCardList(DeckCardList);

						/* 商品を売り切れにする */
						ShopItem->SetSoldOutFlg(true);

						/* カードを購入済みにする */
						ShopItem->GetCard()->SetCardState(Card_Base::CARDSTATE_SHOP_SELLED);
					}
				}
			}
		}
	}
}

// 説明文設定
void Scene_Shop::Update_Explanation()
{
	/* ショップ内のカードにカーソルが重なっているか確認 */
	bool ExplanationDrowFlg = false;
	for(auto& card : this->ShopItemList)
	{
		/* マウスオーバーしているか確認 */
		if (card->GetCard()->MouseInCard())
		{
			// マウスオーバーしている場合
			/* 現在説明UIがnullであるなら作成する */
			if (!this->UI_ExplanationText)
			{
				/* 説明UIを作成する */
				this->UI_ExplanationText = std::make_shared<Scene_UI_ExplanationText>(this->iLayerOrder + 1);
				gpSceneServer->AddSceneReservation(this->UI_ExplanationText);
			}

			/* 説明文を設定する */
			this->UI_ExplanationText->SetExplanationText(card->GetCard()->GetExplanationText());

			/* 設定座標を設定する */
			Struct_2D::POSITION ExplanationPos = card->GetCard()->GetNowPos();
			ExplanationPos.iY -= (Card_Base::CARD_HEIGHT / 2);
			this->UI_ExplanationText->SetBasePos(ExplanationPos);

			/* 上方向に描写するよう設定 */
			this->UI_ExplanationText->SetUpwardDisplayFlg(true);

			/* 描写フラグを有効化する */
			ExplanationDrowFlg = true;
		}
	}

	/* 描写フラグが無効であるなら説明UIを削除する */
	if (!ExplanationDrowFlg)
	{
		if (this->UI_ExplanationText)
		{
			this->UI_ExplanationText->SetDeleteFlg(true);
			this->UI_ExplanationText = nullptr;
		}
	}
}
