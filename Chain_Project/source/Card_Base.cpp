/* カードのベースクラスの定義 */

/* 使用する要素のインクルード */
// ヘッダファイル
#include "Card_Base.h"
// 関連クラス
#include "DataList_Image.h"
// 共通定義
#include "VariableDefine.h"

// コンストラクタ
Card_Base::Card_Base()
{
	/* 初期化 */
	// カード情報
	this->iRarity	= 0;		// レアリティ
	this->iCardType	= 0;		// カードの種類
	this->Name		= "てすとよう";		// カード名
	// 画像
	this->Image = MakeScreen(IMAGE_SIZE_WIDE, IMAGE_SIZE_HEIGHT, TRUE);
	// その他
	this->Now_Position		= { SCREEN_SIZE_WIDE, SCREEN_SIZE_HEIGHT / 2 };	// 現在座標(ドローしてる感を出すため山札の位置を初期値に設定)
	this->Setting_Position	= { 0, 0 };										// 設定座標(ホールドが解除された際に自動で補正される座標)

	UpdateImage();
}

// デストラクタ
Card_Base::~Card_Base()
{
	/* 画像を削除 */
	DeleteGraph(this->Image);
}

// 位置座標補間処理
void Card_Base::Position_Interpolation()
{
	/* X座標補間 */
	if (std::abs(this->Setting_Position.iX - this->Now_Position.iX) < INTERPOLATION_SPEED)
	{
		this->Now_Position.iX = this->Setting_Position.iX;
	}
	else
	{
		this->Now_Position.iX += (this->Setting_Position.iX - this->Now_Position.iX) / INTERPOLATION_SPEED;
	}

	/* Y座標補間 */
	if (std::abs(this->Setting_Position.iY - this->Now_Position.iY) < INTERPOLATION_SPEED)
	{
		this->Now_Position.iY = this->Setting_Position.iY;
	}
	else
	{
		this->Now_Position.iY += (this->Setting_Position.iY - this->Now_Position.iY) / INTERPOLATION_SPEED;
	}
}

// 描画
void Card_Base::Draw()
{
	/* 画像描写 */
	DrawGraph(
		this->Now_Position.iX - (IMAGE_SIZE_WIDE / 2),
		this->Now_Position.iY - (IMAGE_SIZE_HEIGHT / 2),
		this->Image,
		TRUE
	);
}

// 画像更新
// ※ カードの要素に応じて画像を更新する
void Card_Base::UpdateImage()
{
	/* 要素からカード画像を生成する */
	// 0:背景, 1:イラスト, 2:フレーム
	std::shared_ptr<int> Image[3];

	/* 画像管理データリストを取得 */
	std::shared_ptr<DataList_Image> pDataList_Image = std::dynamic_pointer_cast<DataList_Image>(gpDataListServer->GetDataList("DataList_Image"));

	/* 背景の画像を取得 */
	std::string ImageFilePath = "Card_Commoon/BackGround_Test";
	Image[0] = pDataList_Image->iGetImageHandle(ImageFilePath);

	/* イラストの画像を取得 */
//	ImageFilePath = "Card_Ilust/" + GetImageName();
	ImageFilePath = "Card_Ilust/Test_Image";
	Image[1] = pDataList_Image->iGetImageHandle(ImageFilePath);

	/* フレームの画像を取得 */
	ImageFilePath = "Card_Commoon/Frame_Test";
	Image[2] = pDataList_Image->iGetImageHandle(ImageFilePath);

	/* 描画先をカード画像に設定 */
	SetDrawScreen(this->Image);

	/* 画像をクリア */
	ClearDrawScreen();

	/* 背景→イラスト→フレームの順で描写 */
	// 画像の中心を基準に描写する
	for (int i = 0; i < 3; i++)
	{
		/* 画像サイズ取得 */
		int SizeX, SizeY;
		GetGraphSize(*(Image[i]), &SizeX, &SizeY);

		/* 画像描写 */
		DrawGraph(
			(IMAGE_SIZE_WIDE / 2)	- (SizeX / 2),
			(IMAGE_SIZE_HEIGHT / 2) - (SizeY / 2),
			*(Image[i]),
			TRUE
		);
	}

	/* カード名描写 */
	// 文字列の高さ、幅を取得
	int iSizeX = GetDrawStringWidthToHandle(this->Name.c_str(), static_cast<int>(strlenDx(this->Name.c_str())), giFont_DonguriDuel_32);
	int iSizeY = GetFontSizeToHandle(giFont_DonguriDuel_32);

	// 文字列描写
	DrawStringToHandle(
		(IMAGE_SIZE_WIDE / 2)	- (iSizeX / 2),
		(IMAGE_SIZE_HEIGHT / 2)	- (CARD_HEIGHT / 2) + (iSizeY / 2),
		this->Name.c_str(),
		GetColor(0, 0, 0),
		giFont_DonguriDuel_32
	);

	/* 描画先を裏画面に戻す */
	SetDrawScreen(DX_SCREEN_BACK);
}
