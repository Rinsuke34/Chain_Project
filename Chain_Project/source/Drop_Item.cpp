/* ゲーム内でのドロップアイテムクラスの定義 */

/* 使用する要素のインクルード */
// ヘッダファイル
#include "Drop_Item.h"
// 関連クラス
#include "DataList_GameResource.h"
#include "DataList_Image.h"

/* ベース */
// コンストラクタ
Drop_Item_Base::Drop_Item_Base()
{
	/* 初期化 */
	this->Position			= { 0, 0 };
	this->Image				= nullptr;
	this->MovePhase			= PHASE_FALLING;
	this->FallingSpeed		= FALLING_SPEED_START;
	this->WaitTimer			= 10;
	this->UnderPosY			= GetRand(40) + 255;
	this->DeleteFlg			= false;
	this->FallMoveSpeed_X	= GetRand(5) - 2;

	/* データリスト取得 */
	// ゲームリソース管理用データリスト
	this->pDataList_GameResource = std::dynamic_pointer_cast<DataList_GameResource>(gpDataListServer->GetDataList("DataList_GameResource"));
}

// 更新
void Drop_Item_Base::Update()
{
	switch (this->MovePhase)
	{
		// 落下フェーズ
		case PHASE_FALLING:
			// 落下フェーズ更新
			Update_Falling();
			break;

		// 待機フェーズ
		case PHASE_WAITING:
			// 待機フェーズ更新
			Update_Waiting();
			break;

		// 移動フェーズ
		case PHASE_MOVEING:
			// 移動フェーズ更新
			Update_Moveing();
			break;
	}
}

// 落下フェーズ更新
void Drop_Item_Base::Update_Falling()
{
	/* 落下速度を加算 */
	this->FallingSpeed += 1;

	/* 位置座標を更新 */
	this->Position.iY += this->FallingSpeed;
	this->Position.iX += this->FallMoveSpeed_X;

	/* 地面に到達したか確認 */
	if (this->FallingSpeed > 0 && this->Position.iY >= this->UnderPosY)
	{
		// 地面に到達したなら位置座標を地面に合わせ、フェーズを待機フェーズに変更
		this->Position.iY	= this->UnderPosY;
		this->MovePhase		= PHASE_WAITING;
	}
}

// 待機フェーズ更新
void Drop_Item_Base::Update_Waiting()
{
	this->WaitTimer--;
	if (this->WaitTimer <= 0)
	{
		// 待機時間が終了したなら移動フェーズに変更
		this->MovePhase = PHASE_MOVEING;
	}
}

// 描画
void Drop_Item_Base::Draw()
{
	if (this->Image != nullptr)
	{
		DrawGraph(this->Position.iX - (DRAW_SIZE / 2), this->Position.iY - (DRAW_SIZE / 2), *(this->Image), TRUE);
	}
}

/* コイン */
Drop_Item_Coin::Drop_Item_Coin()
{
	/* 画像設定 */
	// 画像管理データリストを取得
	std::shared_ptr<DataList_Image> pDataList_Image = std::dynamic_pointer_cast<DataList_Image>(gpDataListServer->GetDataList("DataList_Image"));
	// 画像名称
	std::string ImageName = "Common_Icon/Icon_Coin";
	/* 指定の画像を読み込む */
	this->Image = pDataList_Image->iGetImageHandle(ImageName);
}

// 移動フェーズ更新
void Drop_Item_Coin::Update_Moveing()
{
	Struct_2D::POSITION GoalPos = { 78, 30 };
	Struct_2D::POSITION Direction;
	Direction.iX = GoalPos.iX - this->Position.iX;
	Direction.iY = GoalPos.iY - this->Position.iY;
	float Distance = sqrtf(static_cast<float>(Direction.iX * Direction.iX + Direction.iY * Direction.iY));

	if (Distance < MOVE_SPEED)
	{
		// ゴールに到達したならコインを所持数に加算し、アイテムを消去
		this->pDataList_GameResource->AddCoin(1);
		this->DeleteFlg = true;
	}
	else
	{
		// ゴールに向かって移動
		Direction.iX = static_cast<int>(Direction.iX / Distance * MOVE_SPEED);
		Direction.iY = static_cast<int>(Direction.iY / Distance * MOVE_SPEED);
		this->Position.iX += Direction.iX;
		this->Position.iY += Direction.iY;
	}	
}