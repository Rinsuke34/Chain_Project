/* シーン"UI-ボタン"の定義 */

/* 使用する要素のインクルード */
// ヘッダファイル
#include "Scene_UI_Button.h"
// 関連クラス
#include "DataList_Image.h"

// コンストラクタ
Scene_UI_Button::Scene_UI_Button(const std::string& cName, const int iLayer) : Scene_Base(cName, iLayer, false, false)
{
	/* 初期化 */
	this->iFontHandle		= 0;		// フォントハンドル
	this->ButtonText		= "";		// ボタンテキスト
	this->CenterPos			= { 0, 0 };	// 位置座標
	this->bMouseOverFlg		= false;	// カーソルが重なっている状態であるかのフラグ
	this->Anim_ImageNo		= 0;		// アニメーションの画像番号
	this->Anim_ChangeDelay	= 0;		// アニメーションの変更までの待機時間

	/* 画像読み込み */
	// 画像管理データリスト取得
	std::shared_ptr<DataList_Image> pDataList_Image = std::dynamic_pointer_cast<DataList_Image>(gpDataListServer->GetDataList("DataList_Image"));
	// 角
	std::string ImageFilePath = "UI/Button/Button_Frame_Corner";
	this->Image_Frame_Corner[0] = pDataList_Image->iGetImageHandle(ImageFilePath);
	ImageFilePath = "UI/Button/Button_Frame_Corner_Over";
	this->Image_Frame_Corner[1] = pDataList_Image->iGetImageHandle(ImageFilePath);
	// 線
	ImageFilePath = "UI/Button/Button_Frame_Line";
	this->Image_Frame_Line[0] = pDataList_Image->iGetImageHandle(ImageFilePath);
	ImageFilePath = "UI/Button/Button_Frame_Line_Over";
	this->Image_Frame_Line[1] = pDataList_Image->iGetImageHandle(ImageFilePath);
	// 内側
	ImageFilePath = "UI/Button/Button_Frame_Inside";
	this->Image_Frame_Inside[0] = pDataList_Image->iGetImageHandle(ImageFilePath);;
	ImageFilePath = "UI/Button/Button_Frame_Inside_Over";
	this->Image_Frame_Inside[1] = pDataList_Image->iGetImageHandle(ImageFilePath);;

	// 強調表示アニメーション(角)
	for(int i = 0; i < EMPHASIS_ANIMATION_MAX; i++)
	{
		ImageFilePath = "UI/Button/Emphasis_Anim/Button_Emphasis_Corner_" + std::to_string(i + 1);
		this->Image_Frame_Emphasis[0][i] = pDataList_Image->iGetImageHandle(ImageFilePath);
	}
	// 強調表示アニメーション(線)
	for(int i = 0; i < EMPHASIS_ANIMATION_MAX; i++)
	{
		ImageFilePath = "UI/Button/Emphasis_Anim/Button_Emphasis_Line_" + std::to_string(i + 1);
		this->Image_Frame_Emphasis[1][i] = pDataList_Image->iGetImageHandle(ImageFilePath);
	}
}

// 更新
void Scene_UI_Button::Update()
{
	/* マウスカーソルがボタンの範囲内にあるか判定 */
	// ボタンの範囲を定義
	Struct_2D::RANGE Range = {
		this->CenterPos.iX - GetDrawStringWidthToHandle(this->ButtonText.c_str(), static_cast<int>(strlenDx(this->ButtonText.c_str())), this->iFontHandle) / 2 - FRAME_THICKNESS,
		this->CenterPos.iY - GetFontSizeToHandle(this->iFontHandle) / 2 - FRAME_THICKNESS,
		this->CenterPos.iX + GetDrawStringWidthToHandle(this->ButtonText.c_str(), static_cast<int>(strlenDx(this->ButtonText.c_str())), this->iFontHandle) / 2 + FRAME_THICKNESS,
		this->CenterPos.iY + GetFontSizeToHandle(this->iFontHandle) / 2 + FRAME_THICKNESS
	};
	// マウスの位置を定義
	Struct_2D::POSITION MousePosition = {
		gstKeyboardInputData.iMouseX,
		gstKeyboardInputData.iMouseY
	};

	/* ボタンの範囲内にマウスがあるか確認 */
	if (PUBLIC_PROCESS::bPositionIn2DRangeCheck(MousePosition, Range))
	{
		// マウスカーソルがボタンの範囲内にある場合
		/* 初めて重なった場合であるか確認 */
		if (this->bMouseOverFlg == false)
		{
			// 初めて重なった場合
			/* アニメーションをリセット */
			this->Anim_ImageNo		= 0;
			this->Anim_ChangeDelay	= EMPHASIS_ANIMATION_SPEED;
		}
		else
		{
			// 初めてではない場合
			/* アニメーションを更新 */
			if (this->Anim_ChangeDelay <= 0)
			{
				this->Anim_ChangeDelay = EMPHASIS_ANIMATION_SPEED;

				if (this->Anim_ImageNo >= EMPHASIS_ANIMATION_MAX - 1)
				{
					this->Anim_ImageNo = 0;
				}
				else
				{
					this->Anim_ImageNo++;
				}
			}
			else
			{
				this->Anim_ChangeDelay--;
			}

		}
		this->bMouseOverFlg = true;
	}
	else
	{
		// マウスカーソルがボタンの範囲内にない場合
		this->bMouseOverFlg = false;
	}
}

// 描画
void Scene_UI_Button::Draw()
{
	/* カーソルの状態に応じてインデックスを変更 */
	int iImageIndex = this->bMouseOverFlg ? 1 : 0;

	/* 文字列の高さ、幅を取得 */
	int iSizeX = GetDrawStringWidthToHandle(this->ButtonText.c_str(), static_cast<int>(strlenDx(this->ButtonText.c_str())), this->iFontHandle);
	int iSizeY = GetFontSizeToHandle(this->iFontHandle);
	// 改行が含まれている場合は高さを調整
	int iLineCount = 1;
	for (size_t i = 0; i < this->ButtonText.length(); i++)
	{
		if (this->ButtonText[i] == '\n')
		{
			iLineCount++;
		}
	}
	iSizeY *= iLineCount;

	/* 枠を描写 */
	// 角(左上)
	DrawModiGraph(
		this->CenterPos.iX - iSizeX / 2 - FRAME_THICKNESS,	this->CenterPos.iY - iSizeY / 2 - FRAME_THICKNESS,
		this->CenterPos.iX - iSizeX / 2,					this->CenterPos.iY - iSizeY / 2 - FRAME_THICKNESS,
		this->CenterPos.iX - iSizeX / 2,					this->CenterPos.iY - iSizeY / 2,
		this->CenterPos.iX - iSizeX / 2 - FRAME_THICKNESS,	this->CenterPos.iY - iSizeY / 2,
		*(this->Image_Frame_Corner[iImageIndex]), TRUE
	);
	// 角(右上)
	DrawModiGraph(
		this->CenterPos.iX + iSizeX / 2 + FRAME_THICKNESS,	this->CenterPos.iY - iSizeY / 2 - FRAME_THICKNESS,
		this->CenterPos.iX + iSizeX / 2 + FRAME_THICKNESS,	this->CenterPos.iY - iSizeY / 2,
		this->CenterPos.iX + iSizeX / 2,					this->CenterPos.iY - iSizeY / 2,
		this->CenterPos.iX + iSizeX / 2,					this->CenterPos.iY - iSizeY / 2 - FRAME_THICKNESS,
		*(this->Image_Frame_Corner[iImageIndex]), TRUE
	);
	// 角(右下)
	DrawModiGraph(
		this->CenterPos.iX + iSizeX / 2 + FRAME_THICKNESS,	this->CenterPos.iY + iSizeY / 2 + FRAME_THICKNESS,
		this->CenterPos.iX + iSizeX / 2,					this->CenterPos.iY + iSizeY / 2 + FRAME_THICKNESS,
		this->CenterPos.iX + iSizeX / 2,					this->CenterPos.iY + iSizeY / 2,
		this->CenterPos.iX + iSizeX / 2 + FRAME_THICKNESS,	this->CenterPos.iY + iSizeY / 2,
		*(this->Image_Frame_Corner[iImageIndex]), TRUE
	);
	// 角(左下)
	DrawModiGraph(
		this->CenterPos.iX - iSizeX / 2 - FRAME_THICKNESS,	this->CenterPos.iY + iSizeY / 2 + FRAME_THICKNESS,
		this->CenterPos.iX - iSizeX / 2 - FRAME_THICKNESS,	this->CenterPos.iY + iSizeY / 2,
		this->CenterPos.iX - iSizeX / 2,					this->CenterPos.iY + iSizeY / 2,
		this->CenterPos.iX - iSizeX / 2,					this->CenterPos.iY + iSizeY / 2 + FRAME_THICKNESS,
		*(this->Image_Frame_Corner[iImageIndex]), TRUE
	);
	// 線(上)
	DrawModiGraph(
		this->CenterPos.iX - iSizeX / 2, this->CenterPos.iY - iSizeY / 2 - FRAME_THICKNESS,
		this->CenterPos.iX + iSizeX / 2, this->CenterPos.iY - iSizeY / 2 - FRAME_THICKNESS,
		this->CenterPos.iX + iSizeX / 2, this->CenterPos.iY - iSizeY / 2,
		this->CenterPos.iX - iSizeX / 2, this->CenterPos.iY - iSizeY / 2,
		*(this->Image_Frame_Line[iImageIndex]), TRUE
	);
	// 線(右)
	DrawModiGraph(
		this->CenterPos.iX + iSizeX / 2 + FRAME_THICKNESS,	this->CenterPos.iY + iSizeY / 2,
		this->CenterPos.iX + iSizeX / 2 + FRAME_THICKNESS,	this->CenterPos.iY - iSizeY / 2,
		this->CenterPos.iX + iSizeX / 2,					this->CenterPos.iY - iSizeY / 2,
		this->CenterPos.iX + iSizeX / 2,					this->CenterPos.iY + iSizeY / 2,
		*(this->Image_Frame_Line[iImageIndex]), TRUE
	);
	// 線(下)
	DrawModiGraph(
		this->CenterPos.iX + iSizeX / 2, this->CenterPos.iY + iSizeY / 2 + FRAME_THICKNESS,
		this->CenterPos.iX - iSizeX / 2, this->CenterPos.iY + iSizeY / 2 + FRAME_THICKNESS,
		this->CenterPos.iX - iSizeX / 2, this->CenterPos.iY + iSizeY / 2,
		this->CenterPos.iX + iSizeX / 2, this->CenterPos.iY + iSizeY / 2,
		*(this->Image_Frame_Line[iImageIndex]), TRUE
	);
	// 線(左)
	DrawModiGraph(
		this->CenterPos.iX - iSizeX / 2 - FRAME_THICKNESS,	this->CenterPos.iY - iSizeY / 2,
		this->CenterPos.iX - iSizeX / 2 - FRAME_THICKNESS,	this->CenterPos.iY + iSizeY / 2,
		this->CenterPos.iX - iSizeX / 2,					this->CenterPos.iY + iSizeY / 2,
		this->CenterPos.iX - iSizeX / 2,					this->CenterPos.iY - iSizeY / 2,
		*(this->Image_Frame_Line[iImageIndex]), TRUE
	);
	// 内側
	DrawModiGraph(
		this->CenterPos.iX - iSizeX / 2,	this->CenterPos.iY - iSizeY / 2,
		this->CenterPos.iX + iSizeX / 2,	this->CenterPos.iY - iSizeY / 2,
		this->CenterPos.iX + iSizeX / 2,	this->CenterPos.iY + iSizeY / 2,
		this->CenterPos.iX - iSizeX / 2,	this->CenterPos.iY + iSizeY / 2,
		*(this->Image_Frame_Inside[iImageIndex]), TRUE
	);

	/* 強調表示アニメーションを描写 */
	if (this->bMouseOverFlg)
	{
		// 角(左上)
		DrawModiGraph(
			this->CenterPos.iX - iSizeX / 2 - EMPHASIS_ANIMATION_THICKNESS,	this->CenterPos.iY - iSizeY / 2 - EMPHASIS_ANIMATION_THICKNESS,
			this->CenterPos.iX - iSizeX / 2,								this->CenterPos.iY - iSizeY / 2 - EMPHASIS_ANIMATION_THICKNESS,
			this->CenterPos.iX - iSizeX / 2,								this->CenterPos.iY - iSizeY / 2,
			this->CenterPos.iX - iSizeX / 2 - EMPHASIS_ANIMATION_THICKNESS,	this->CenterPos.iY - iSizeY / 2,
			*(this->Image_Frame_Emphasis[0][this->Anim_ImageNo]), TRUE
		);
		// 角(右上)
		DrawModiGraph(
			this->CenterPos.iX + iSizeX / 2 + EMPHASIS_ANIMATION_THICKNESS, this->CenterPos.iY - iSizeY / 2 - EMPHASIS_ANIMATION_THICKNESS,
			this->CenterPos.iX + iSizeX / 2 + EMPHASIS_ANIMATION_THICKNESS, this->CenterPos.iY - iSizeY / 2,
			this->CenterPos.iX + iSizeX / 2,								this->CenterPos.iY - iSizeY / 2,
			this->CenterPos.iX + iSizeX / 2,								this->CenterPos.iY - iSizeY / 2 - EMPHASIS_ANIMATION_THICKNESS,
			*(this->Image_Frame_Emphasis[0][this->Anim_ImageNo]), TRUE
		);
		// 角(右下)
		DrawModiGraph(
			this->CenterPos.iX + iSizeX / 2 + EMPHASIS_ANIMATION_THICKNESS,	this->CenterPos.iY + iSizeY / 2 + EMPHASIS_ANIMATION_THICKNESS,
			this->CenterPos.iX + iSizeX / 2,								this->CenterPos.iY + iSizeY / 2 + EMPHASIS_ANIMATION_THICKNESS,
			this->CenterPos.iX + iSizeX / 2,								this->CenterPos.iY + iSizeY / 2,
			this->CenterPos.iX + iSizeX / 2 + EMPHASIS_ANIMATION_THICKNESS, this->CenterPos.iY + iSizeY / 2,
			*(this->Image_Frame_Emphasis[0][this->Anim_ImageNo]), TRUE
		);
		// 角(左下)
		DrawModiGraph(
			this->CenterPos.iX - iSizeX / 2 - EMPHASIS_ANIMATION_THICKNESS,	this->CenterPos.iY + iSizeY / 2 + EMPHASIS_ANIMATION_THICKNESS,
			this->CenterPos.iX - iSizeX / 2 - EMPHASIS_ANIMATION_THICKNESS,	this->CenterPos.iY + iSizeY / 2,
			this->CenterPos.iX - iSizeX / 2,								this->CenterPos.iY + iSizeY / 2,
			this->CenterPos.iX - iSizeX / 2,								this->CenterPos.iY + iSizeY / 2 + EMPHASIS_ANIMATION_THICKNESS,
			*(this->Image_Frame_Emphasis[0][this->Anim_ImageNo]), TRUE
		);
		// 線(上)
		DrawModiGraph(
			this->CenterPos.iX - iSizeX / 2, this->CenterPos.iY - iSizeY / 2 - EMPHASIS_ANIMATION_THICKNESS,
			this->CenterPos.iX + iSizeX / 2, this->CenterPos.iY - iSizeY / 2 - EMPHASIS_ANIMATION_THICKNESS,
			this->CenterPos.iX + iSizeX / 2, this->CenterPos.iY - iSizeY / 2,
			this->CenterPos.iX - iSizeX / 2, this->CenterPos.iY - iSizeY / 2,
			*(this->Image_Frame_Emphasis[1][this->Anim_ImageNo]), TRUE
		);
		// 線(右)
		DrawModiGraph(
			this->CenterPos.iX + iSizeX / 2 + EMPHASIS_ANIMATION_THICKNESS, this->CenterPos.iY + iSizeY / 2,
			this->CenterPos.iX + iSizeX / 2 + EMPHASIS_ANIMATION_THICKNESS, this->CenterPos.iY - iSizeY / 2,
			this->CenterPos.iX + iSizeX / 2,								this->CenterPos.iY - iSizeY / 2,
			this->CenterPos.iX + iSizeX / 2,								this->CenterPos.iY + iSizeY / 2,
			*(this->Image_Frame_Emphasis[1][this->Anim_ImageNo]), TRUE
		);
		// 線(下)
		DrawModiGraph(
			this->CenterPos.iX + iSizeX / 2, this->CenterPos.iY + iSizeY / 2 + EMPHASIS_ANIMATION_THICKNESS,
			this->CenterPos.iX - iSizeX / 2, this->CenterPos.iY + iSizeY / 2 + EMPHASIS_ANIMATION_THICKNESS,
			this->CenterPos.iX - iSizeX / 2, this->CenterPos.iY + iSizeY / 2,
			this->CenterPos.iX + iSizeX / 2, this->CenterPos.iY + iSizeY / 2,
			*(this->Image_Frame_Emphasis[1][this->Anim_ImageNo]), TRUE
		);
		// 線(左)
		DrawModiGraph(
			this->CenterPos.iX - iSizeX / 2 - EMPHASIS_ANIMATION_THICKNESS, this->CenterPos.iY - iSizeY / 2,
			this->CenterPos.iX - iSizeX / 2 - EMPHASIS_ANIMATION_THICKNESS, this->CenterPos.iY + iSizeY / 2,
			this->CenterPos.iX - iSizeX / 2,								this->CenterPos.iY + iSizeY / 2,
			this->CenterPos.iX - iSizeX / 2,								this->CenterPos.iY - iSizeY / 2,
			*(this->Image_Frame_Emphasis[1][this->Anim_ImageNo]), TRUE
		);
	}

	/* ボタンテキスト描写 */
	DrawStringToHandle(
		this->CenterPos.iX - iSizeX / 2,
		this->CenterPos.iY - iSizeY / 2,
		this->ButtonText.c_str(),
		this->bMouseOverFlg ? GetColor(0, 0, 0) : GetColor(255, 255, 255),
		this->iFontHandle
	);
}
