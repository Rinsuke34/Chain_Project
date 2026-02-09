/* キャラクターのベースクラスの定義 */

/* 使用する要素のインクルード */
// ヘッダファイル
#include "Character_Base.h"
// 共通定義
#include "VariableDefine.h"
// 関連クラス
#include "Scene_Particles_Text.h"
#include "DataList_Image.h"
#include "DataList_Battle.h"
#include "Buff_Debuff.h"
#include "Action_Effect.h"

// コンストラクタ
Character_Base::Character_Base()
{
	/* 初期化 */
	this->iHP_Max						= 0;								// 体力(最大値)
	this->iHP_Now						= 0;								// 体力(現在値)
	this->iShield_Now					= 0;								// シールド(現在値)
	this->Image							= nullptr;							// 画像
	this->BasePos						= { 0, 0 };							// 基準座標
	this->Camp							= -1;								// 陣営
	this->SizeX							= -1;								// キャラクターの幅
	this->SizeY							= -1;								// キャラクターの高さ
	this->CorrectionPos					= { 0, 0 };							// 補正座標
	this->AddBuffReaction				= 0;								// バフ付与時のリアクション
	this->DamageReaction				= 0;								// 被ダメージ時のリアクション
	this->AttackReaction				= 0;								// 攻撃時のリアクション
	this->Death_DeleteFlg				= false;							// 死亡により盤面上から削除するかのフラグ
	this->FlattenPercent				= 100;								// 平たくする量(%単位 / 100が標準)
	this->StandbyFlatten_Count			= GetRand(STANDBY_FLATTEN_PERIOD);	// 待機時の平たくする量のカウント
	this->StandbyFlatten_Percent		= 0;								// 待機児の平たくする量の実数値
	this->ActionEffect_Scale_Count		= 0;								// 行動内容のスケールカウント
	this->ActionEffect_Scale_Size		= 0;								// 行動内容のスケールサイズ

	/* 行動内容用フレーム画像取得 */
	// 画像管理データリストを取得
	std::shared_ptr<DataList_Image> pDataList_Image = std::dynamic_pointer_cast<DataList_Image>(gpDataListServer->GetDataList("DataList_Image"));
	// 角
	std::string Image_Name = "UI/Button/Button_Frame_Corner";
	this->Image_Frame_Corner = pDataList_Image->iGetImageHandle(Image_Name);
	// 線
	Image_Name = "UI/Button/Button_Frame_Line";
	this->Image_Frame_Line = pDataList_Image->iGetImageHandle(Image_Name);
	// 内側
	Image_Name = "UI/Button/Button_Frame_Inside";
	this->Image_Frame_Inside = pDataList_Image->iGetImageHandle(Image_Name);

	/* データリスト取得 */
	// バトル用データリスト
	this->pDataList_Battle = std::dynamic_pointer_cast<DataList_Battle>(gpDataListServer->GetDataList("DataList_Battle"));
}

// 描画
void Character_Base::Draw()
{
	/* 各リアクションによる座標補正 */
	Correction_Reaction();

	/* 行動内容の拡大、縮小処理 */
	ActionEffect_ScaleUpdate();

	/* キャラクターのサイズが設定されていないなら画像サイズを算出 */
	if (this->SizeX == -1 || this->SizeY == -1)
	{
		GetGraphSize(*(this->Image), &this->SizeX, &this->SizeY);
	}

	/* キャラクター画像の描画 */
	if (this->Camp == CAMP_FRIEND)
	{
		// 味方陣営なら右向き(画像のまま)
		DrawModiGraph(
			this->BasePos.iX + CorrectionPos.iX - (this->SizeX / 2), this->BasePos.iY + CorrectionPos.iY - ((this->SizeY) * static_cast<float>(this->FlattenPercent / 100.f)),
			this->BasePos.iX + CorrectionPos.iX + (this->SizeX / 2), this->BasePos.iY + CorrectionPos.iY - ((this->SizeY) * static_cast<float>(this->FlattenPercent / 100.f)),
			this->BasePos.iX + CorrectionPos.iX + (this->SizeX / 2), this->BasePos.iY + CorrectionPos.iY,
			this->BasePos.iX + CorrectionPos.iX - (this->SizeX / 2), this->BasePos.iY + CorrectionPos.iY,
			*(this->Image),
			TRUE
		);
	}
	else
	{
		// 敵陣営なら左右反転
		DrawModiGraph(
			this->BasePos.iX + CorrectionPos.iX + (this->SizeX / 2), this->BasePos.iY + CorrectionPos.iY - ((this->SizeY) * static_cast<float>(this->FlattenPercent / 100.f)),
			this->BasePos.iX + CorrectionPos.iX - (this->SizeX / 2), this->BasePos.iY + CorrectionPos.iY - ((this->SizeY) * static_cast<float>(this->FlattenPercent / 100.f)),
			this->BasePos.iX + CorrectionPos.iX - (this->SizeX / 2), this->BasePos.iY + CorrectionPos.iY,
			this->BasePos.iX + CorrectionPos.iX + (this->SizeX / 2), this->BasePos.iY + CorrectionPos.iY,
			*(this->Image),
			TRUE
		);
	}

	/* ステータスバーの描写 */
	Draw_StatusBar();
}

// ステータスバーを描写
void Character_Base::Draw_StatusBar()
{
	/* 画像管理データリストを取得 */
	std::shared_ptr<DataList_Image> pDataList_Image = std::dynamic_pointer_cast<DataList_Image>(gpDataListServer->GetDataList("DataList_Image"));

	/* 画像を取得 */
	// HPバー背景(シールド無し)
	std::string StatusBar_BackGround_ImageName = "UI/StatusBar/StatusBar_BackGround";
	std::shared_ptr<int> StatusBar_BackGround_Image = pDataList_Image->iGetImageHandle(StatusBar_BackGround_ImageName);
	// HPバー背景(シールド有り)
	StatusBar_BackGround_ImageName = "UI/StatusBar/StatusBar_BackGround_Shield";
	std::shared_ptr<int> StartusBar_BackGround_Shield_Image = pDataList_Image->iGetImageHandle(StatusBar_BackGround_ImageName);
	// シールドアイコン
	std::string Shield_Icon_ImageName = "UI/StatusBar/Icon_Shield";
	std::shared_ptr<int> Shield_Icon_Image = pDataList_Image->iGetImageHandle(Shield_Icon_ImageName);

	/* 背景を描写 */
	DrawGraph(
		this->BasePos.iX - (HPBAR_WIDE / 2),
		this->BasePos.iY - this->SizeY - HPBAR_UPPER,
		this->iShield_Now > 0 ? *(StartusBar_BackGround_Shield_Image) : *(StatusBar_BackGround_Image),
		TRUE
	);

	/* HPを描写 */
	DrawBox(
		this->BasePos.iX - (HPBAR_WIDE / 2) + HPBAR_FRAME_WIDE,
		this->BasePos.iY - this->SizeY - HPBAR_UPPER + 2,
		this->BasePos.iX - (HPBAR_WIDE / 2) + HPBAR_FRAME_WIDE + ((HPBAR_WIDE - (HPBAR_FRAME_WIDE * 2)) * this->iHP_Now) / this->iHP_Max,
		this->BasePos.iY - this->SizeY - HPBAR_UPPER + 18,
		GetColor(255, 0, 0),
		TRUE
	);

	/* 残り体力を文字列で描写 */
	std::string HPText = std::to_string(this->iHP_Now) + "/" + std::to_string(this->iHP_Max);

	/* 文字列の高さ、幅を取得 */
	int iSizeX = GetDrawStringWidthToHandle(HPText.c_str(), static_cast<int>(strlenDx(HPText.c_str())), giFont_JF_Dot_MPlus10_20);
	int iSizeY = GetFontSizeToHandle(giFont_JF_Dot_MPlus10_20);

	/* 残り体力の文字列描写 */
	DrawStringToHandle(
		this->BasePos.iX - (iSizeX / 2),
		this->BasePos.iY - this->SizeY - HPBAR_UPPER + ((HPBAR_HEIGHT - iSizeY) / 2),
		HPText.c_str(),
		GetColor(255, 255, 255),
		giFont_JF_Dot_MPlus10_20
	);

	/* シールドが付与されているならシールドのアイコンを描写 */
	if (this->iShield_Now > 0)
	{
		/* シールドアイコンのサイズ取得 */
		int Shield_Icon_SizeX, Shield_Icon_SizeY;
		GetGraphSize(*(Shield_Icon_Image), &Shield_Icon_SizeX, &Shield_Icon_SizeY);

		/* シールドアイコン描写 */
		DrawGraph(
			(this->BasePos.iX - (HPBAR_WIDE / 2)) - (Shield_Icon_SizeX / 2),
			(this->BasePos.iY - this->SizeY - HPBAR_UPPER) + (HPBAR_HEIGHT / 2) - (Shield_Icon_SizeY / 2),
			*(Shield_Icon_Image),
			TRUE
		);

		/* 残りシールドを文字列で描写 */
		std::string ShieldText = std::to_string(this->iShield_Now);

		/* 文字列の高さ、幅を取得 */
		int iShield_Text_SizeX = GetDrawStringWidthToHandle(ShieldText.c_str(), static_cast<int>(strlenDx(ShieldText.c_str())), giFont_JF_Dot_MPlus10_16);
		int iShield_Text_SizeY = GetFontSizeToHandle(giFont_JF_Dot_MPlus10_16);

		/* 残りシールドの文字列描写 */
		DrawStringToHandle(
			(this->BasePos.iX - (HPBAR_WIDE / 2)) - (iShield_Text_SizeX / 2),
			(this->BasePos.iY - this->SizeY - HPBAR_UPPER) + (HPBAR_HEIGHT / 2) - (iShield_Text_SizeY / 2),
			ShieldText.c_str(),
			GetColor(255, 255, 255),
			giFont_JF_Dot_MPlus10_16
		);
	}

	/* バフ、デバフの描写 */
	// 味方であるなら左側、敵であるなら右側に描写
	for (size_t i = 0; i < this->Buff_Debuff_List.size(); i++)
	{
		/* バフ、デバフの描写 */
		DrawModiGraph(
			this->BasePos.iX - (this->SizeX / 2) + (i * 32),		this->BasePos.iY - (this->SizeY) - 64,
			this->BasePos.iX - (this->SizeX / 2) + ((i + 1) * 32),	this->BasePos.iY - (this->SizeY) - 64,
			this->BasePos.iX - (this->SizeX / 2) + ((i + 1) * 32),	this->BasePos.iY - (this->SizeY) - 64 + 32,
			this->BasePos.iX - (this->SizeX / 2) + (i * 32),		this->BasePos.iY - (this->SizeY) - 64 + 32,
			*(this->Buff_Debuff_List[i]->Image),
			TRUE
		);

		/* 残りターン数の描写 */
		std::string TurnText = std::to_string(this->Buff_Debuff_List[i]->Buff_Debuff_Time);
		DrawStringToHandle(
			this->BasePos.iX - (this->SizeX / 2) + (i * 32) + 4,
			this->BasePos.iY - (this->SizeY) - 64 + 16,
			TurnText.c_str(),
			GetColor(255, 255, 255),
			giFont_JF_Dot_MPlus10_16
		);
	}
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
	this->AttackReaction	= 15;
}

// バフ付与アクション
void Character_Base::Action_AddBuff()
{
	/* バフ取得時のリアクションを設定 */
	this->AddBuffReaction = 15;
}

// ダメージ処理
void Character_Base::Damage(int DamageAmount)
{
	// 引数
	// DamageAmount : ダメージ量

	/* 計算前の体力、シールドを保存 */
	int OldHP		= this->iHP_Now;
	int OldShield	= this->iShield_Now;

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

		/* シールド減少量分のテキストを描写 */
		std::shared_ptr<Scene_Particles_Text> AddText = std::make_shared<Scene_Particles_Text>();
		std::string DamageText = std::to_string(OldShield - this->iShield_Now);
		AddText->SetText(DamageText);
		AddText->SetPosition({ this->BasePos.iX - (HPBAR_WIDE / 2), this->BasePos.iY - this->SizeY - HPBAR_UPPER + (HPBAR_HEIGHT / 2) });
		AddText->SetFontHandle(giFont_JF_Dot_MPlus10_16_Edge);
		AddText->SetMove({ 0, -1 });
		AddText->SetAlpha(0);
		AddText->SetColor(GetColor(0, 191, 255));
		gpSceneServer->AddSceneReservation(AddText);
	}

	/* 体力からダメージを減算 */
	this->iHP_Now -= DamageAmount;

	/* 体力が0以下になった場合、0に設定 */
	if (this->iHP_Now < 0)
	{
		this->iHP_Now = 0;
	}

	/* 体力が1以上減少しているなら体力減少量分のテキストを描写 */
	if ((OldHP - this->iHP_Now) > 0)
	{
		std::shared_ptr<Scene_Particles_Text> AddText = std::make_shared<Scene_Particles_Text>();
		std::string DamageText = std::to_string(OldHP - this->iHP_Now);
		AddText->SetText(DamageText);
		AddText->SetPosition({ this->BasePos.iX, this->BasePos.iY - this->SizeY - HPBAR_UPPER + (HPBAR_HEIGHT / 2) });
		AddText->SetFontHandle(giFont_JF_Dot_MPlus10_20_Edge);
		AddText->SetMove({ 0, -1 });
		AddText->SetAlpha(0);
		AddText->SetColor(GetColor(255, 0, 0));
		gpSceneServer->AddSceneReservation(AddText);
	}

	/* 被ダメージ時のリアクションを設定 */
	this->DamageReaction = 15;
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

// バフ、デバフの更新
void Character_Base::Update_Buff_Debuff()
{
	/* 毒ダメージ処理 */
	// 毒デバフを取得
	std::vector<std::shared_ptr<Character_Buff_Debuff_Base>> Poison = this->CheckGet_Buff_Debuff("Debuff_Poison");
	if (Poison.size() > 0)
	{
		// 所持しているなら
		/* 残りターン数分のダメージを受ける */
		for (auto& debuff : Poison)
		{
			Damage(debuff->Buff_Debuff_Time);
		}
	}

	/* すべてのバフ、デバフのカウントを進める */
	for (auto& BuffDebuff : this->Buff_Debuff_List)
	{
		BuffDebuff->Update();
	}

	/* 削除フラグが立っているバフ、デバフを削除 */
	this->Buff_Debuff_List.erase(
		std::remove_if(
			this->Buff_Debuff_List.begin(),
			this->Buff_Debuff_List.end(),
			[](const std::shared_ptr<Character_Buff_Debuff_Base>& BuffDebuff)
			{
				return BuffDebuff->DeleteFlag;
			}
		),
		this->Buff_Debuff_List.end()
	);
}

// バフ、デバフの追加
void Character_Base::Add_Buff_Debuff(const std::shared_ptr<Character_Buff_Debuff_Base>& Buff_Debuff)
{
	// 引数
	// Buff_Debuff : 追加するバフ、デバフ

	/* バフ、デバフを追加 */
	this->Buff_Debuff_List.push_back(Buff_Debuff);
}

// 対象の名称のバフ、デバフを取得
std::vector<std::shared_ptr<Character_Buff_Debuff_Base>> Character_Base::CheckGet_Buff_Debuff(std::string Buff_Debuff_Name)
{
	// 引数
	// Buff_Debuff_Name : 対象の名称のバフ、デバフの名称

	/* 取得用配列 */
	std::vector<std::shared_ptr<Character_Buff_Debuff_Base>> ReturnList;

	/* すべてのバフ、デバフを確認 */
	for (auto& BuffDebuff : this->Buff_Debuff_List)
	{
		/* 対象の名称のバフ、デバフであるなら取得用配列に追加 */
		if (BuffDebuff->Name == Buff_Debuff_Name)
		{
			ReturnList.push_back(BuffDebuff);
		}
	}

	/* 取得用配列を返す */
	return ReturnList;
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

	/* 死亡時のリアクション補正 */
	if (this->iHP_Now <= 0)
	{
		// 死亡している場合
		/* 平たくしていく */
		if (this->FlattenPercent > 0)
		{
			// 平たくする量が0より大きい場合
			this->FlattenPercent -= 1;
			if (this->FlattenPercent < 0)
			{
				this->FlattenPercent = 0;
			}
		}
		else
		{
			// 平たくする量が0以下になった場合
			/* 削除フラグの有効化 */
			this->FlattenPercent = 0;
			this->Death_DeleteFlg = true;
		}		
	}
	else
	{
		// 生存している場合
		/* 待機時の平たくする量の更新 */
		this->StandbyFlatten_Count++;
		float value = (std::sin(2.f * PI * this->StandbyFlatten_Count / STANDBY_FLATTEN_PERIOD) + 1.f) / 2.f;
		this->StandbyFlatten_Percent = value * STANDBY_FLATTEN_MAX;
		if (this->StandbyFlatten_Count >= STANDBY_FLATTEN_PERIOD)
		{
			this->StandbyFlatten_Count = 0;
		}

		this->FlattenPercent = 100.f - this->StandbyFlatten_Percent;
	}
}

// 行動内容の描写
void Character_Base::Draw_Action_Effect()
{
	/* 所持している行動内容一覧を描写 */
	int Index = 0;
	for (auto& ActionEffect : this->ActionEffectList)
	{
		/* 枠を描写 */
		// 元のアイコン描画位置・サイズは変更しない
		const int iconX = this->BasePos.iX + ACTION_X;
		const int iconY = this->BasePos.iY + ACTION_Y + (Index * ACTION_INTERVAL);
		const int iconW = ACTION_WIDE;
		const int iconH = ACTION_HEIGHT;

		// 枠だけ小さくするためのパディング（ピクセル単位）
		const int framePadding = 24;
		const int frameW = iconW - framePadding;
		const int frameH = iconH - framePadding;

		// 枠の中心座標（アイコン中央と揃える）
		const int centerX = iconX + (iconW / 2);
		const int centerY = iconY + (iconH / 2);

		/* 枠を描写（Scene_UI_Button に合わせた方式） */
		// 角(左上)
		DrawModiGraph(
			centerX - frameW / 2 - FRAME_THICKNESS,	centerY - frameH / 2 - FRAME_THICKNESS,
			centerX - frameW / 2,					centerY - frameH / 2 - FRAME_THICKNESS,
			centerX - frameW / 2,					centerY - frameH / 2,
			centerX - frameW / 2 - FRAME_THICKNESS,	centerY - frameH / 2,
			*(this->Image_Frame_Corner),
			TRUE
		);
		// 角(右上)
		DrawModiGraph(
			centerX + frameW / 2 + FRAME_THICKNESS,	centerY - frameH / 2 - FRAME_THICKNESS,
			centerX + frameW / 2 + FRAME_THICKNESS,	centerY - frameH / 2,
			centerX + frameW / 2,					centerY - frameH / 2,
			centerX + frameW / 2,					centerY - frameH / 2 - FRAME_THICKNESS,
			*(this->Image_Frame_Corner),
			TRUE
		);
		// 角(右下)
		DrawModiGraph(
			centerX + frameW / 2 + FRAME_THICKNESS,	centerY + frameH / 2 + FRAME_THICKNESS,
			centerX + frameW / 2,					centerY + frameH / 2 + FRAME_THICKNESS,
			centerX + frameW / 2,					centerY + frameH / 2,
			centerX + frameW / 2 + FRAME_THICKNESS,	centerY + frameH / 2,
			*(this->Image_Frame_Corner),
			TRUE
		);
		// 角(左下)
		DrawModiGraph(
			centerX - frameW / 2 - FRAME_THICKNESS,	centerY + frameH / 2 + FRAME_THICKNESS,
			centerX - frameW / 2 - FRAME_THICKNESS,	centerY + frameH / 2,
			centerX - frameW / 2,					centerY + frameH / 2,
			centerX - frameW / 2,					centerY + frameH / 2 + FRAME_THICKNESS,
			*(this->Image_Frame_Corner),
			TRUE
		);
		// 線(上)
		DrawModiGraph(
			centerX - frameW / 2, centerY - frameH / 2 - FRAME_THICKNESS,
			centerX + frameW / 2, centerY - frameH / 2 - FRAME_THICKNESS,
			centerX + frameW / 2, centerY - frameH / 2,
			centerX - frameW / 2, centerY - frameH / 2,
			*(this->Image_Frame_Line),
			TRUE
		);
		// 線(右)
		DrawModiGraph(
			centerX + frameW / 2 + FRAME_THICKNESS, centerY + frameH / 2,
			centerX + frameW / 2 + FRAME_THICKNESS,	centerY - frameH / 2,
			centerX + frameW / 2,					centerY - frameH / 2,
			centerX + frameW / 2,					centerY + frameH / 2,
			*(this->Image_Frame_Line),
			TRUE
		);
		// 線(下)
		DrawModiGraph(
			centerX + frameW / 2, centerY + frameH / 2 + FRAME_THICKNESS,
			centerX - frameW / 2, centerY + frameH / 2 + FRAME_THICKNESS,
			centerX - frameW / 2, centerY + frameH / 2,
			centerX + frameW / 2, centerY + frameH / 2,
			*(this->Image_Frame_Line),
			TRUE
		);
		// 線(左)
		DrawModiGraph(
			centerX - frameW / 2 - FRAME_THICKNESS, centerY - frameH / 2,
			centerX - frameW / 2 - FRAME_THICKNESS, centerY + frameH / 2,
			centerX - frameW / 2,					centerY + frameH / 2,
			centerX - frameW / 2,					centerY - frameH / 2,
			*(this->Image_Frame_Line),
			TRUE
		);
		// 内側
		DrawModiGraph(
			centerX - frameW / 2, centerY - frameH / 2,
			centerX + frameW / 2, centerY - frameH / 2,
			centerX + frameW / 2, centerY + frameH / 2,
			centerX - frameW / 2, centerY + frameH / 2,
			*(this->Image_Frame_Inside),
			TRUE
		);

		/* 行動内容の描写 */
		DrawModiGraph(
			this->BasePos.iX + ACTION_X - this->ActionEffect_Scale_Size,				this->BasePos.iY + ACTION_Y + (Index * ACTION_INTERVAL) - this->ActionEffect_Scale_Size,
			this->BasePos.iX + ACTION_X + ACTION_WIDE + this->ActionEffect_Scale_Size,	this->BasePos.iY + ACTION_Y + (Index * ACTION_INTERVAL) - this->ActionEffect_Scale_Size,
			this->BasePos.iX + ACTION_X + ACTION_WIDE + this->ActionEffect_Scale_Size,	this->BasePos.iY + ACTION_Y + ACTION_HEIGHT + (Index * ACTION_INTERVAL) + this->ActionEffect_Scale_Size,
			this->BasePos.iX + ACTION_X - this->ActionEffect_Scale_Size,				this->BasePos.iY + ACTION_Y + ACTION_HEIGHT + (Index * ACTION_INTERVAL) + this->ActionEffect_Scale_Size,
			*(ActionEffect->GetImage()),
			TRUE
		);
	}
}

// 該当の行動内容の削除
void Character_Base::Delete_Action_Effect(std::shared_ptr<Action_Effect_Base> ActionEffect)
{
	// 引数
	// ActionEffect : 削除する行動内容

	/* 指定の行動内容を削除 */
	this->ActionEffectList.erase(
		std::remove_if(
			this->ActionEffectList.begin(),
			this->ActionEffectList.end(),
			[&](const std::shared_ptr<Action_Effect_Base>& Effect)
			{
				return Effect == ActionEffect;
			}
		),
		this->ActionEffectList.end()
	);
}

// 行動内容のスケール更新
void Character_Base::ActionEffect_ScaleUpdate()
{
	// カウントは現在の値で補間を行い、その後インクリメントして 120 到達で 0 に戻す
	int c = this->ActionEffect_Scale_Count; // 0..119 の想定

	// 線形補間ヘルパー（a -> b を t(0..1) で補完）
	auto lerp_int = [](int a, int b, double t) -> int {
		return static_cast<int>(std::lround(a + (b - a) * t));
		};

	int scale = 0;
	if (c <= 30)
	{
		// [0 .. 30] : 0 -> 3
		double t = (30.0 == 0.0) ? 0.0 : static_cast<double>(c) / 30.0;
		scale = lerp_int(0, 3, t);
	}
	else if (c <= 60)
	{
		// (30 .. 60] : 3 -> 0
		double t = static_cast<double>(c - 30) / 30.0;
		scale = lerp_int(3, 0, t);
	}
	else if (c <= 90)
	{
		// (60 .. 90] : 0 -> -3
		double t = static_cast<double>(c - 60) / 30.0;
		scale = lerp_int(0, -3, t);
	}
	else
	{
		// (90 .. 119] : -3 -> 0  (120 は 0 と等価)
		double t = static_cast<double>(c - 90) / 30.0;
		scale = lerp_int(-3, 0, t);
	}

	// 結果を設定
	this->ActionEffect_Scale_Size = scale;

	// カウントを進めて 120 到達で 0 に戻す
	this->ActionEffect_Scale_Count++;
	if (this->ActionEffect_Scale_Count >= 120)
	{
		this->ActionEffect_Scale_Count = 0;
	}
}
