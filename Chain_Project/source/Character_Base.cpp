/* キャラクターのベースクラスの定義 */

/* 使用する要素のインクルード */
// ヘッダファイル
#include "Character_Base.h"
// 共通定義
#include "VariableDefine.h"
// 関連クラス
#include "DataList_Image.h"

// コンストラクタ
Character_Base::Character_Base()
{
	/* 初期化 */
	this->iHP_Max			= 0;			// 体力(最大値)
	this->iHP_Now			= 0;			// 体力(現在値)
	this->iShield_Now		= 0;			// シールド(現在値)
	this->Image				= nullptr;		// 画像
	this->BasePos			= { 0, 0 };		// 基準座標
	this->Camp				= -1;			// 陣営
	this->SizeX				= -1;			// キャラクターの幅
	this->SizeY				= -1;			// キャラクターの高さ
	this->CorrectionPos		= { 0, 0 };		// 補正座標
	this->AddBuffReaction	= 0;			// バフ付与時のリアクション
	this->DamageReaction	= 0;			// 被ダメージ時のリアクション
	this->AttackReaction	= 0;			// 攻撃時のリアクション
	this->EyeHeight			= 0;			// 目線の高さ(行動内容アイコンの描写位置)
}

// 描画
void Character_Base::Draw()
{
	/* 各リアクションによる座標補正 */
	Correction_Reaction();

	/* キャラクターのサイズが設定されていないなら画像サイズを算出 */
	if (this->SizeX == -1 || this->SizeY == -1)
	{
		GetGraphSize(*(this->Image), &this->SizeX, &this->SizeY);
	}

	/* キャラクター画像の描画 */
	DrawModiGraph(
		this->BasePos.iX + CorrectionPos.iX - (this->SizeX / 2), this->BasePos.iY + CorrectionPos.iY - (this->SizeY),
		this->BasePos.iX + CorrectionPos.iX + (this->SizeX / 2), this->BasePos.iY + CorrectionPos.iY - (this->SizeY),
		this->BasePos.iX + CorrectionPos.iX + (this->SizeX / 2), this->BasePos.iY + CorrectionPos.iY,
		this->BasePos.iX + CorrectionPos.iX - (this->SizeX / 2), this->BasePos.iY + CorrectionPos.iY,
		*(this->Image),
		TRUE
	);

	/* 体力バー描画 */
	Draw_HPBar();

	/* シールドバー描写 */
	Draw_ShieldBar();
}

// 体力バー描画
void Character_Base::Draw_HPBar()
{
	/* 体力バーの描写中心座標を設定 */
	Struct_2D::POSITION HPBar_CenterPos = {
		this->BasePos.iX,
		this->BasePos.iY - this->SizeY - HPBAR_UPPER
	};

	/* 体力バーの背景描写 */
	DrawBox(
		HPBar_CenterPos.iX - (HPBAR_WIDE / 2),
		HPBar_CenterPos.iY - (HPBAR_HEIGHT / 2),
		HPBar_CenterPos.iX + (HPBAR_WIDE / 2),
		HPBar_CenterPos.iY + (HPBAR_HEIGHT / 2),
		GetColor(0, 0, 0),
		TRUE
	);

	/* 体力バーの残り体力描写 */
	DrawBox(
		HPBar_CenterPos.iX - (HPBAR_WIDE / 2),
		HPBar_CenterPos.iY - (HPBAR_HEIGHT / 2),
		HPBar_CenterPos.iX - (HPBAR_WIDE / 2) + (HPBAR_WIDE * this->iHP_Now / this->iHP_Max),
		HPBar_CenterPos.iY + (HPBAR_HEIGHT / 2),
		GetColor(255, 0, 0),
		TRUE
	);

	/* 残り体力を文字列に設定 */
	std::string HPText = std::to_string(this->iHP_Now) + " / " + std::to_string(this->iHP_Max);

	/* 文字列の高さ、幅を取得 */
	int iSizeX = GetDrawStringWidthToHandle(HPText.c_str(), static_cast<int>(strlenDx(HPText.c_str())), giFont_Cp_Period_16);
	int iSizeY = GetFontSizeToHandle(giFont_Cp_Period_16);

	/* 残り体力の文字列描写 */
	DrawStringToHandle(
		HPBar_CenterPos.iX - iSizeX / 2,
		HPBar_CenterPos.iY - iSizeY / 2,
		HPText.c_str(),
		GetColor(255, 255, 255),
		giFont_Cp_Period_16
	);
}

// シールドバー描写
void Character_Base::Draw_ShieldBar()
{
	/* シールドバーの描写中心座標を設定 */
	Struct_2D::POSITION HPBar_CenterPos = {
		this->BasePos.iX,
		this->BasePos.iY - this->SizeY - SHIELDBAR_UPPER
	};

	/* シールドバーの残量描写 */
	DrawBox(
		HPBar_CenterPos.iX - (HPBAR_WIDE / 2),
		HPBar_CenterPos.iY - (SHIELDBAR_HEIGHT / 2),
		HPBar_CenterPos.iX - (HPBAR_WIDE / 2) + (HPBAR_WIDE * this->iShield_Now / this->iHP_Max),
		HPBar_CenterPos.iY + (SHIELDBAR_HEIGHT / 2),
		GetColor(0, 0, 255),
		TRUE
	);
}

// 指定の名称の画像を設定する
void Character_Base::SetUpImage(std::string ImageName)
{
	// 引数
	// ImageName : 画像名称

	/* 画像管理データリストを取得 */
	std::shared_ptr<DataList_Image> pDataList_Image = std::dynamic_pointer_cast<DataList_Image>(gpDataListServer->GetDataList("DataList_Image"));

	/* 指定の画像を読み込む */
	this->Image = pDataList_Image->iGetImageHandle(ImageName);
}

// 攻撃アクション
void Character_Base::Action_Attack()
{
	/* 攻撃時のリアクションを設定 */
	this->AttackReaction	= 30;
}

// バフ付与アクション
void Character_Base::Action_AddBuff()
{
	/* バフ取得時のリアクションを設定 */
	this->AddBuffReaction = 30;
}

// ダメージ処理
void Character_Base::Damage(int DamageAmount)
{
	// 引数
	// DamageAmount : ダメージ量

	/* シールドでダメージを軽減 */
	if (this->iShield_Now > 0)
	{
		// シールドがある場合
		/* シールドでダメージを軽減 */
		if (DamageAmount <= this->iShield_Now)
		{
			// ダメージ量がシールド以下の場合
			this->iShield_Now -= DamageAmount;
			DamageAmount = 0;
		}
		else
		{
			// ダメージ量がシールド以上の場合
			DamageAmount -= this->iShield_Now;
			this->iShield_Now = 0;
		}
	}

	/* 体力からダメージを減算 */
	this->iHP_Now -= DamageAmount;

	/* 体力が0以下になった場合、0に設定 */
	if (this->iHP_Now < 0)
	{
		this->iHP_Now = 0;
	}

	/* 被ダメージ時のリアクションを設定 */
	this->DamageReaction = 30;
}

// シールド追加処理
void Character_Base::AddShield(int Shield)
{
	// 引数
	// Shield	<-	追加するシールド量

	/* シールドを追加 */
	this->iShield_Now += Shield;
}

// シールドリセット(ターン終了時)
void Character_Base::ShieldReset_EndTurn()
{
	/* シールドを0に設定 */
	this->iShield_Now = 0;
}

// 回復処理
void Character_Base::Heal(int Heal)
{
	// 引数
	// Heal	<-	回復量

	/* 体力を回復 */
	this->iHP_Now += Heal;

	/* 体力が最大値を超えた場合、最大値に設定 */
	if (this->iHP_Now > this->iHP_Max)
	{
		this->iHP_Now = this->iHP_Max;
	}
}

// 各リアクションによる座標補正
void Character_Base::Correction_Reaction()
{
	/* 座標補正の初期化 */
	this->CorrectionPos = { 0, 0 };

	/* 敵陣営であるなら平行方向への揺れの向きを変更 */
	int iDirection = 1;
	if (this->Camp == CAMP_ENEMY)
	{
		iDirection = -1;
	}

	/* バフ付与時のリアクション補正 */
	if (this->AddBuffReaction > 0)
	{
		this->CorrectionPos.iY -= (this->AddBuffReaction * 2);
		this->AddBuffReaction--;
	}
	/* 被ダメージ時のリアクション補正 */
	if (this->DamageReaction > 0)
	{
		this->CorrectionPos.iX -= (this->DamageReaction * 2) * iDirection;
		this->DamageReaction--;
	}
	/* 攻撃時のリアクション補正 */
	if (this->AttackReaction > 0)
	{
		this->CorrectionPos.iX += (this->AttackReaction * 2) * iDirection;
		this->AttackReaction--;
	}
}
