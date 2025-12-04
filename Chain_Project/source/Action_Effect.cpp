/* ゲーム内でのキャラクターの行動内容クラスの定義 */

/* 使用する要素のインクルード */
// ヘッダファイル
#include "Action_Effect.h"
// 共通定義
#include "VariableDefine.h"
// 関連クラス
#include "Character_Base.h"
#include "Card_Base.h"
#include "DataList_Battle.h"
#include "DataList_Image.h"

/* ベース */
// コンストラクタ
Action_Effect_Base::Action_Effect_Base()
{
	/* 初期化 */
	this->Target_Camp		= -1;		// 効果対象の陣営
	this->Target_Position	= -1;		// 効果対象の立ち位置
	this->EffectUser		= nullptr;	// 効果の使用者
	this->AllRange			= false;	// 全体に効果を与えるか
	this->Now_Position		= { 0, 0 };	// 現在座標
	this->Setting_Position	= { 0, 0 };	// 設定座標(自動で補完される座標)
	this->Priority			= 0;		// 優先順位(高いほど先に実行)

	/* 画像作成 */
	this->Image = MakeScreen(IMAGE_SIZE_WIDE, IMAGE_SIZE_HEIGHT, TRUE);

	/* データリスト取得 */
	// バトル用データリスト
	this->pDataList_Battle = std::dynamic_pointer_cast<DataList_Battle>(gpDataListServer->GetDataList("DataList_Battle"));
}

// デストラクタ
Action_Effect_Base::~Action_Effect_Base()
{
	/* 画像を削除 */
	DeleteGraph(this->Image);
}

// 更新
void Action_Effect_Base::Update()
{
	/* 座標の補完処理 */
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

	/* 画像の更新処理 */
	Update_Image();
}

// 描写
void Action_Effect_Base::Draw()
{
	/* 画像描写 */
	DrawGraph(
		this->Now_Position.iX - (IMAGE_SIZE_WIDE / 2),
		this->Now_Position.iY - (IMAGE_SIZE_HEIGHT / 2),
		this->Image,
		TRUE);
}

// 画像更新
void Action_Effect_Base::Update_Image()
{
	/* 描写先をカード画像に設定 */
	SetDrawScreen(this->Image);

	/* 画像をクリア */
	ClearDrawScreen();

	/* 背景描写 */
	Draw_BackGround();

	/* 効果描写 */
	Draw_Effect();

	/* フレーム描写 */
	Draw_Frame();

	/* 描写先を裏画面に戻す */
	SetDrawScreen(DX_SCREEN_BACK);
}

// 背景作成
void Action_Effect_Base::Draw_BackGround()
{
	/* 仮で真っ白にする */
	DrawBox(
		0, 0,
		IMAGE_SIZE_WIDE, IMAGE_SIZE_HEIGHT,
		GetColor(255, 255, 255),
		TRUE);

	/* 使用者の顔部分を描写する */
	if (this->EffectUser != nullptr)
	{
		// 効果使用者が存在する場合
		/* 画像管理データリストを取得 */
		std::shared_ptr<DataList_Image> pDataList_Image = std::dynamic_pointer_cast<DataList_Image>(gpDataListServer->GetDataList("DataList_Image"));

		/* 効果使用者の画像を取得 */
		std::shared_ptr<int> ExecutorImage = this->EffectUser->GetImage();

		/* キャラクター画像の描画 */
		DrawModiGraph(
			0,								this->EffectUser->GetEyeHeight(),
			this->EffectUser->GetSizeX(),	this->EffectUser->GetEyeHeight(),
			this->EffectUser->GetSizeX(),	this->EffectUser->GetSizeY() + this->EffectUser->GetEyeHeight(),
			0,								this->EffectUser->GetSizeY() + this->EffectUser->GetEyeHeight(),
			*(ExecutorImage),
			TRUE
		);
	}
}

// フレーム作成
void Action_Effect_Base::Draw_Frame()
{
	/* 使用者の陣営に応じて色を決定する */
	if (this->EffectUser != nullptr)
	{
		switch (this->EffectUser->GetCamp())
		{
			case Character_Base::CAMP_FRIEND:
				// 仲間陣営である場合
				DrawBox(
					0, 0,
					IMAGE_SIZE_WIDE, IMAGE_FRAME_WIDE,
					GetColor(0, 0, 255),
					TRUE);

				DrawBox(
					IMAGE_SIZE_WIDE - IMAGE_FRAME_WIDE, 0,
					IMAGE_SIZE_WIDE, IMAGE_SIZE_HEIGHT,
					GetColor(0, 0, 255),
					TRUE);

				DrawBox(
					0, 0,
					IMAGE_FRAME_WIDE, IMAGE_SIZE_HEIGHT,
					GetColor(0, 0, 255),
					TRUE);

				DrawBox(
					0, IMAGE_SIZE_HEIGHT - IMAGE_FRAME_WIDE,
					IMAGE_SIZE_WIDE, IMAGE_SIZE_HEIGHT,
					GetColor(0, 0, 255),
					TRUE);
				break;

			case Character_Base::CAMP_ENEMY:
				// 敵陣営である場合
				DrawBox(
					0, 0,
					IMAGE_SIZE_WIDE, IMAGE_FRAME_WIDE,
					GetColor(255, 0, 0),
					TRUE);

				DrawBox(
					IMAGE_SIZE_WIDE - IMAGE_FRAME_WIDE, 0,
					IMAGE_SIZE_WIDE, IMAGE_SIZE_HEIGHT,
					GetColor(255, 0, 0),
					TRUE);

				DrawBox(
					0, 0,
					IMAGE_FRAME_WIDE, IMAGE_SIZE_HEIGHT,
					GetColor(255, 0, 0),
					TRUE);

				DrawBox(
					0, IMAGE_SIZE_HEIGHT - IMAGE_FRAME_WIDE,
					IMAGE_SIZE_WIDE, IMAGE_SIZE_HEIGHT,
					GetColor(255, 0, 0),
					TRUE);
		}
	}
}

/* 攻撃 */
// コンストラクタ
Action_Effect_Attack::Action_Effect_Attack()
{
	/* 初期化 */
	this->DamageAmount	= 0;	// ダメージ量
}

// 効果実行
void Action_Effect_Attack::ExecuteEffect()
{
	/* 効果の対象キャラクターを取得 */
	if (this->Target_Camp == Character_Base::CAMP_ENEMY)
	{
		// 敵キャラクターが対象である場合
		/* 全体攻撃であるか確認 */
		if (this->AllRange)
		{
			// 全体攻撃である場合
			/* 全ての敵キャラクターにダメージ処理を実行 */
			for (int i = 0; i < DataList_Battle::POSITION_MAX; i++)
			{
				std::shared_ptr<Character_Base> TargetEnemyCharacter = this->pDataList_Battle->GetEnemyCharacter(i);
				if (TargetEnemyCharacter != nullptr)
				{
					// 対象の敵キャラクターが存在する場合
					/* ダメージ処理を実行 */
					TargetEnemyCharacter->Damage(this->DamageAmount);
				}
			}

			/* 攻撃リアクションを設定 */
			if (this->EffectUser)
			{
				this->EffectUser->Action_Attack();
			}
		}
		else
		{
			// 単体攻撃である場合
			/* 対象の立ち位置の敵を取得 */
			std::shared_ptr<Character_Base> TargetEnemyCharacter = this->pDataList_Battle->GetEnemyCharacter(this->Target_Position);
			if (TargetEnemyCharacter != nullptr)
			{
				// 対象の敵キャラクターが存在する場合
				/* ダメージ処理を実行 */
				TargetEnemyCharacter->Damage(this->DamageAmount);

				/* 攻撃リアクションを設定 */
				if (this->EffectUser)
				{
					this->EffectUser->Action_Attack();
				}
			}
		}
	}
	else
	{
		// 仲間キャラクターが対象である場合
		/* 全体攻撃であるか確認 */
		if (this->AllRange)
		{
			// 全体攻撃である場合
			/* 全ての仲間キャラクターにダメージ処理を実行 */
			for (int i = 0; i < DataList_Battle::POSITION_MAX; i++)
			{
				std::shared_ptr<Character_Base> TargetFriendCharacter = this->pDataList_Battle->GetFriendCharacter(i);
				if (TargetFriendCharacter != nullptr)
				{
					// 対象の仲間キャラクターが存在する場合
					/* ダメージ処理を実行 */
					TargetFriendCharacter->Damage(this->DamageAmount);
				}
			}

			/* 攻撃リアクションを設定 */
			if (this->EffectUser)
			{
				this->EffectUser->Action_Attack();
			}
		}
		else
		{
			// 単体攻撃である場合
			/* 対象の立ち位置の仲間を取得 */
			std::shared_ptr<Character_Base> TargetFriendCharacter = this->pDataList_Battle->GetFriendCharacter(this->Target_Position);
			if (TargetFriendCharacter != nullptr)
			{
				// 対象の仲間キャラクターが存在する場合
				/* ダメージ処理を実行 */
				TargetFriendCharacter->Damage(this->DamageAmount);

				/* 攻撃リアクションを設定 */
				if (this->EffectUser)
				{
					this->EffectUser->Action_Attack();
				}
			}
		}
	}
}

// 効果部分作成
void Action_Effect_Attack::Draw_Effect()
{
	/* 画像管理データリストを取得 */
	std::shared_ptr<DataList_Image> pDataList_Image = std::dynamic_pointer_cast<DataList_Image>(gpDataListServer->GetDataList("DataList_Image"));

	/* 攻撃アイコンを描写 */
	std::string AttackIconFileName = "Common_Icon/Icon_Attack";
	std::shared_ptr<int> AttackIconImage = pDataList_Image->iGetImageHandle(AttackIconFileName);

	DrawGraph(
		IMAGE_SIZE_WIDE / 2, 0,
		*(AttackIconImage),
		TRUE
	);

	/* 攻撃力を描写 */
	std::string DamageText = std::to_string(this->DamageAmount);
	DrawStringToHandle(
		(IMAGE_SIZE_WIDE / 2) + 30,
		IMAGE_SIZE_HEIGHT / 2 - 8,
		DamageText.c_str(),
		GetColor(0, 0, 0),
		giFont_Cp_Period_16
	);
}

/* 防御 */
// コンストラクタ
Action_Effect_Defence::Action_Effect_Defence()
{
	/* 初期化 */
	this->ShieldAmount	= 0;	// シールド量
}

// 効果実行
void Action_Effect_Defence::ExecuteEffect()
{
	/* 効果の対象キャラクターを取得 */
	if (this->Target_Camp == Character_Base::CAMP_ENEMY)
	{
		// 敵キャラクターが対象である場合
		/* 全体に付与されるか確認 */
		if (this->AllRange)
		{
			// 全体に付与される場合
			/* 全ての敵キャラクターにシールド付与処理を実行 */
			for (int i = 0; i < DataList_Battle::POSITION_MAX; i++)
			{
				std::shared_ptr<Character_Base> TargetEnemyCharacter = this->pDataList_Battle->GetEnemyCharacter(i);
				if (TargetEnemyCharacter != nullptr)
				{
					// 対象の敵キャラクターが存在する場合
					/* シールド付与処理を実行 */
					TargetEnemyCharacter->AddShield(this->ShieldAmount);
				}
			}

			/* バフ付与リアクションを設定 */
			if (this->EffectUser)
			{
				this->EffectUser->Action_AddBuff();
			}
		}
		else
		{
			// 単体に付与される場合
			/* 対象の立ち位置の敵を取得 */
			std::shared_ptr<Character_Base> TargetEnemyCharacter = this->pDataList_Battle->GetEnemyCharacter(this->Target_Position);
			if (TargetEnemyCharacter != nullptr)
			{
				// 対象の敵キャラクターが存在する場合
				/* シールド付与処理を実行 */
				TargetEnemyCharacter->AddShield(this->ShieldAmount);

				/* バフ付与リアクションを設定 */
				if (this->EffectUser)
				{
					this->EffectUser->Action_AddBuff();
				}
			}
		}
	}
	else
	{
		// 仲間キャラクターが対象である場合
		/* 全体に付与されるか確認 */
		if (this->AllRange)
		{
			// 全体に付与される場合
			/* 全ての仲間キャラクターにシールド付与処理を実行 */
			for (int i = 0; i < DataList_Battle::POSITION_MAX; i++)
			{
				std::shared_ptr<Character_Base> TargetFriendCharacter = this->pDataList_Battle->GetFriendCharacter(i);
				if (TargetFriendCharacter != nullptr)
				{
					// 対象の仲間キャラクターが存在する場合
					/* シールド付与処理を実行 */
					TargetFriendCharacter->AddShield(this->ShieldAmount);
				}
			}

			/* バフ付与リアクションを設定 */
			if (this->EffectUser)
			{
				this->EffectUser->Action_AddBuff();
			}
		}
		else
		{
			// 単体に付与される場合
			/* 対象の立ち位置の仲間を取得 */
			std::shared_ptr<Character_Base> TargetFriendCharacter = this->pDataList_Battle->GetFriendCharacter(this->Target_Position);
			if (TargetFriendCharacter != nullptr)
			{
				// 対象の仲間キャラクターが存在する場合
				/* シールド付与処理を実行 */
				TargetFriendCharacter->AddShield(this->ShieldAmount);

				/* バフ付与リアクションを設定 */
				if (this->EffectUser)
				{
					this->EffectUser->Action_AddBuff();
				}
			}
		}
	}
}

// 効果部分作成
void Action_Effect_Defence::Draw_Effect()
{
	/* 画像管理データリストを取得 */
	std::shared_ptr<DataList_Image> pDataList_Image = std::dynamic_pointer_cast<DataList_Image>(gpDataListServer->GetDataList("DataList_Image"));

	/* 防御アイコンを描写 */
	std::string AttackIconFileName = "Common_Icon/Icon_Shield";
	std::shared_ptr<int> AttackIconImage = pDataList_Image->iGetImageHandle(AttackIconFileName);

	DrawGraph(
		IMAGE_SIZE_WIDE / 2, 0,
		*(AttackIconImage),
		TRUE
	);

	/* 攻撃力を描写 */
	std::string DamageText = std::to_string(this->ShieldAmount);
	DrawStringToHandle(
		(IMAGE_SIZE_WIDE / 2) + 30,
		IMAGE_SIZE_HEIGHT / 2 - 8,
		DamageText.c_str(),
		GetColor(0, 0, 0),
		giFont_Cp_Period_16
	);
}

/* 回復 */
// コンストラクタ
Action_Effect_Heal::Action_Effect_Heal()
{
	/* 初期化 */
	this->HealAmount	= 0;		// 回復量
}

// 効果実行
void Action_Effect_Heal::ExecuteEffect()
{
	/* 効果の対象キャラクターを取得 */
	if (this->Target_Camp == Character_Base::CAMP_ENEMY)
	{
		// 敵キャラクターが対象である場合
		/* 全体回復であるか確認 */
		if (this->AllRange)
		{
			// 全体回復である場合
			/* 全ての敵キャラクターに回復処理を実行 */
			for (int i = 0; i < DataList_Battle::POSITION_MAX; i++)
			{
				std::shared_ptr<Character_Base> TargetEnemyCharacter = this->pDataList_Battle->GetEnemyCharacter(i);
				if (TargetEnemyCharacter != nullptr)
				{
					// 対象の敵キャラクターが存在する場合
					/* 回復処理を実行 */
					TargetEnemyCharacter->Heal(this->HealAmount);
				}
			}

			/* バフ付与リアクションを設定 */
			if (this->EffectUser)
			{
				this->EffectUser->Action_AddBuff();
			}
		}
		else
		{
			// 単体回復である場合
			/* 対象の立ち位置の敵を取得 */
			std::shared_ptr<Character_Base> TargetEnemyCharacter = this->pDataList_Battle->GetEnemyCharacter(this->Target_Position);
			if (TargetEnemyCharacter != nullptr)
			{
				// 対象の敵キャラクターが存在する場合
				/* 回復処理を実行 */
				TargetEnemyCharacter->Heal(this->HealAmount);

				/* バフ付与リアクションを設定 */
				if (this->EffectUser)
				{
					this->EffectUser->Action_AddBuff();
				}
			}
		}
	}
	else
	{
		// 仲間キャラクターが対象である場合
		/* 全体回復であるか確認 */
		if (this->AllRange)
		{
			// 全体回復である場合
			/* 全ての仲間キャラクターに回復処理を実行 */
			for (int i = 0; i < DataList_Battle::POSITION_MAX; i++)
			{
				std::shared_ptr<Character_Base> TargetFriendCharacter = this->pDataList_Battle->GetFriendCharacter(i);
				if (TargetFriendCharacter != nullptr)
				{
					// 対象の仲間キャラクターが存在する場合
					/* 回復処理を実行 */
					TargetFriendCharacter->Heal(this->HealAmount);
				}
			}
			/* バフ付与リアクションを設定 */
			if (this->EffectUser)
			{
				this->EffectUser->Action_AddBuff();
			}
		}
		else
		{
			// 単体回復である場合
			/* 対象の立ち位置の仲間を取得 */
			std::shared_ptr<Character_Base> TargetFriendCharacter = this->pDataList_Battle->GetFriendCharacter(this->Target_Position);
			if (TargetFriendCharacter != nullptr)
			{
				// 対象の仲間キャラクターが存在する場合
				/* 回復処理を実行 */
				TargetFriendCharacter->Heal(this->HealAmount);

				/* バフ付与リアクションを設定 */
				if (this->EffectUser)
				{
					this->EffectUser->Action_AddBuff();
				}
			}
		}
	}
}

// 効果部分作成
void Action_Effect_Heal::Draw_Effect()
{

}

/* 状態異常付与 */
// コンストラクタ
Action_Effect_StatusAliment::Action_Effect_StatusAliment()
{
	/* 初期化 */
	this->StatusAilment_Type	= 0;	// 状態異常の種類
	this->StatusAilment_Level	= 0;	// 状態異常のレベル
}

// 効果実行
void Action_Effect_StatusAliment::ExecuteEffect()
{
	
}

// 効果部分作成
void Action_Effect_StatusAliment::Draw_Effect()
{
	
}

/* 特殊効果 */
// コンストラクタ
Action_Effect_Extra::Action_Effect_Extra()
{
	/* 初期化 */
	this->ExEffectCard	= nullptr;	// 特殊効果を使用するカード
}

// 効果実行
void Action_Effect_Extra::ExecuteEffect()
{
	/* カード効果(特殊効果)を実行 */
	this->ExEffectCard->Card_Effect_Extra_Process();
}

// 効果部分作成
void Action_Effect_Extra::Draw_Effect()
{
	
}
