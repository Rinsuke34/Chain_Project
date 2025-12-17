/* ワールドマップノード"ショップ"クラスの定義 */

/* 使用する要素のインクルード */
// ヘッダファイル
#include "WoldMap_Node_Shop.h"
// 関連クラス
#include "DataList_Image.h"

// コンストラクタ
WoldMap_Node_Shop::WoldMap_Node_Shop() : WoldMap_Node_Base()
{
	/* 初期化 */
	this->NodeType = NODE_TYPE_SHOP;	// ノードの種類

	/* 画像管理データリストを取得 */
	std::shared_ptr<DataList_Image> pDataList_Image = std::dynamic_pointer_cast<DataList_Image>(gpDataListServer->GetDataList("DataList_Image"));

	/* 画像の取得 */
	std::string ImageFilePath = "WoldMap_Node/Shop";
	this->Image_Icon[ICON_INDEX_NODE_ICON] = pDataList_Image->iGetImageHandle(ImageFilePath);
}
