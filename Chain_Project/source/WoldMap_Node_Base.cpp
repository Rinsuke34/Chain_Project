/* ワールドマップノードのベースクラスの定義 */

/* 使用する要素のインクルード */
// ヘッダファイル
#include "WoldMap_Node_Base.h"
// 関連クラス
#include "DataList_Image.h"

// コンストラクタ
WoldMap_Node_Base::WoldMap_Node_Base()
{
	/* 初期化 */
	this->NodeType							= "";					// ノードの種類
	this->Position_Map						= {0, 0};				// マップ上での座標
	this->Position_Now						= {0, 0};				// 画面上での中心座標
	this->Move_Node_List.clear();									// 移動可能ノードリスト
	this->Image_Icon_Size					= WOLDMAP_ICON_SIZE;	// アイコン画像サイズ
	this->Image_Icon[ICON_INDEX_NODE_ICON]	= nullptr;				// アイコン画像[1:固有のアイコン]

	/* 画像管理データリストを取得 */
	std::shared_ptr<DataList_Image> pDataList_Image = std::dynamic_pointer_cast<DataList_Image>(gpDataListServer->GetDataList("DataList_Image"));

	/* 画像の取得 */
	// アイコン画像[0:プレイヤー現在位置]
	std::string ImageFilePath = "WoldMap_Node/PlayerPoint";
	this->Image_Icon[ICON_INDEX_PLAYER_POS]	= pDataList_Image->iGetImageHandle(ImageFilePath);
	// 角
	ImageFilePath = "UI/Button/Button_Frame_Corner_Over";
	this->Image_Frame_Corner = pDataList_Image->iGetImageHandle(ImageFilePath);;
	// 線
	ImageFilePath = "UI/Button/Button_Frame_Line_Over";
	this->Image_Frame_Line = pDataList_Image->iGetImageHandle(ImageFilePath);;
	// 内側
	ImageFilePath = "UI/Button/Button_Frame_Inside_Over";
	this->Image_Frame_Inside = pDataList_Image->iGetImageHandle(ImageFilePath);
}

// 更新処理
void WoldMap_Node_Base::Update()
{

}

// 描画処理
void WoldMap_Node_Base::Draw()
{
	/* 背景、フレームの描写 */
	// ※ボタンUIのフレームを流用
	// 角(左上)
	DrawModiGraph(
		this->Position_Now.iX - (this->Image_Icon_Size / 2) - FRAME_THICKNESS,	this->Position_Now.iY - (this->Image_Icon_Size / 2) - FRAME_THICKNESS,
		this->Position_Now.iX - (this->Image_Icon_Size / 2),					this->Position_Now.iY - (this->Image_Icon_Size / 2) - FRAME_THICKNESS,
		this->Position_Now.iX - (this->Image_Icon_Size / 2),					this->Position_Now.iY - (this->Image_Icon_Size / 2),
		this->Position_Now.iX - (this->Image_Icon_Size / 2) - FRAME_THICKNESS,	this->Position_Now.iY - (this->Image_Icon_Size / 2),
		*(this->Image_Frame_Corner), TRUE
	);
	// 角(右上)
	DrawModiGraph(
		this->Position_Now.iX + (this->Image_Icon_Size / 2) + FRAME_THICKNESS,	this->Position_Now.iY - (this->Image_Icon_Size / 2) - FRAME_THICKNESS,
		this->Position_Now.iX + (this->Image_Icon_Size / 2) + FRAME_THICKNESS,	this->Position_Now.iY - (this->Image_Icon_Size / 2),
		this->Position_Now.iX + (this->Image_Icon_Size / 2),					this->Position_Now.iY - (this->Image_Icon_Size / 2),
		this->Position_Now.iX + (this->Image_Icon_Size / 2),					this->Position_Now.iY - (this->Image_Icon_Size / 2) - FRAME_THICKNESS,
		*(this->Image_Frame_Corner), TRUE
	);
	// 角(右下)
	DrawModiGraph(
		this->Position_Now.iX + (this->Image_Icon_Size / 2) + FRAME_THICKNESS,	this->Position_Now.iY + (this->Image_Icon_Size / 2) + FRAME_THICKNESS,
		this->Position_Now.iX + (this->Image_Icon_Size / 2),					this->Position_Now.iY + (this->Image_Icon_Size / 2) + FRAME_THICKNESS,
		this->Position_Now.iX + (this->Image_Icon_Size / 2),					this->Position_Now.iY + (this->Image_Icon_Size / 2),
		this->Position_Now.iX + (this->Image_Icon_Size / 2) + FRAME_THICKNESS,	this->Position_Now.iY + (this->Image_Icon_Size / 2),
		*(this->Image_Frame_Corner), TRUE
	);
	// 角(左下)
	DrawModiGraph(
		this->Position_Now.iX - (this->Image_Icon_Size / 2) - FRAME_THICKNESS,	this->Position_Now.iY + (this->Image_Icon_Size / 2) + FRAME_THICKNESS,
		this->Position_Now.iX - (this->Image_Icon_Size / 2) - FRAME_THICKNESS,	this->Position_Now.iY + (this->Image_Icon_Size / 2),
		this->Position_Now.iX - (this->Image_Icon_Size / 2),					this->Position_Now.iY + (this->Image_Icon_Size / 2),
		this->Position_Now.iX - (this->Image_Icon_Size / 2),					this->Position_Now.iY + (this->Image_Icon_Size / 2) + FRAME_THICKNESS,
		*(this->Image_Frame_Corner), TRUE
	);
	// 線(上)
	DrawModiGraph(
		this->Position_Now.iX - (this->Image_Icon_Size / 2), this->Position_Now.iY - (this->Image_Icon_Size / 2) - FRAME_THICKNESS,
		this->Position_Now.iX + (this->Image_Icon_Size / 2), this->Position_Now.iY - (this->Image_Icon_Size / 2) - FRAME_THICKNESS,
		this->Position_Now.iX + (this->Image_Icon_Size / 2), this->Position_Now.iY - (this->Image_Icon_Size / 2),
		this->Position_Now.iX - (this->Image_Icon_Size / 2), this->Position_Now.iY - (this->Image_Icon_Size / 2),
		*(this->Image_Frame_Line), TRUE
	);
	// 線(右)
	DrawModiGraph(
		this->Position_Now.iX + (this->Image_Icon_Size / 2) + FRAME_THICKNESS,	this->Position_Now.iY + (this->Image_Icon_Size / 2),
		this->Position_Now.iX + (this->Image_Icon_Size / 2) + FRAME_THICKNESS,	this->Position_Now.iY - (this->Image_Icon_Size / 2),
		this->Position_Now.iX + (this->Image_Icon_Size / 2),					this->Position_Now.iY - (this->Image_Icon_Size / 2),
		this->Position_Now.iX + (this->Image_Icon_Size / 2),					this->Position_Now.iY + (this->Image_Icon_Size / 2),
		*(this->Image_Frame_Line), TRUE
	);
	// 線(下)
	DrawModiGraph(
		this->Position_Now.iX + (this->Image_Icon_Size / 2), this->Position_Now.iY + (this->Image_Icon_Size / 2) + FRAME_THICKNESS,
		this->Position_Now.iX - (this->Image_Icon_Size / 2), this->Position_Now.iY + (this->Image_Icon_Size / 2) + FRAME_THICKNESS,
		this->Position_Now.iX - (this->Image_Icon_Size / 2), this->Position_Now.iY + (this->Image_Icon_Size / 2),
		this->Position_Now.iX + (this->Image_Icon_Size / 2), this->Position_Now.iY + (this->Image_Icon_Size / 2),
		*(this->Image_Frame_Line), TRUE
	);
	// 線(左)
	DrawModiGraph(
		this->Position_Now.iX - (this->Image_Icon_Size / 2) - FRAME_THICKNESS,	this->Position_Now.iY - (this->Image_Icon_Size / 2),
		this->Position_Now.iX - (this->Image_Icon_Size / 2) - FRAME_THICKNESS,	this->Position_Now.iY + (this->Image_Icon_Size / 2),
		this->Position_Now.iX - (this->Image_Icon_Size / 2),					this->Position_Now.iY + (this->Image_Icon_Size / 2),
		this->Position_Now.iX - (this->Image_Icon_Size / 2),					this->Position_Now.iY - (this->Image_Icon_Size / 2),
		*(this->Image_Frame_Line), TRUE
	);
	// 内側
	DrawModiGraph(
		this->Position_Now.iX - (this->Image_Icon_Size / 2), this->Position_Now.iY - (this->Image_Icon_Size / 2),
		this->Position_Now.iX + (this->Image_Icon_Size / 2), this->Position_Now.iY - (this->Image_Icon_Size / 2),
		this->Position_Now.iX + (this->Image_Icon_Size / 2), this->Position_Now.iY + (this->Image_Icon_Size / 2),
		this->Position_Now.iX - (this->Image_Icon_Size / 2), this->Position_Now.iY + (this->Image_Icon_Size / 2),
		*(this->Image_Frame_Inside), TRUE
	);

	/* アイコンを描写 */
	DrawExtendGraph(
		this->Position_Now.iX - (this->Image_Icon_Size / 2), this->Position_Now.iY - (this->Image_Icon_Size / 2),
		this->Position_Now.iX + (this->Image_Icon_Size / 2), this->Position_Now.iY + (this->Image_Icon_Size / 2),
		*(this->Image_Icon[ICON_INDEX_NODE_ICON]), TRUE);
}
