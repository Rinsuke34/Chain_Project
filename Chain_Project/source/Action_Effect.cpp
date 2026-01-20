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
	this->Priority			= 0;		// 優先順位(高いほど先に実行)
	this->EffectCard		= nullptr;	// 効果を使用するカード
	this->IconType			= -1;		// アイコンのタイプ

	/* データリスト取得 */
	// バトル用データリスト
	this->pDataList_Battle = std::dynamic_pointer_cast<DataList_Battle>(gpDataListServer->GetDataList("DataList_Battle"));
}

// 画像設定
void Action_Effect_Base::Setup_Image()
{
	/* 画像名称取得 */
	std::string ImageName;
	switch (IconType)
	{
		// アイコンタイプ:攻撃
		case ICON_TYPE_ATTACK:
			ImageName = "Common_Icon/Icon_Attack";
			break;

		// アイコンタイプ:防御
		case ICON_TYPE_DEFENCE:
			ImageName = "Common_Icon/Icon_Defence";
			break;

		// アイコンタイプ:回復
		case ICON_TYPE_HEAL:
			ImageName = "Common_Icon/Icon_Heal";
			break;

		// アイコンタイプ:特殊効果
		case ICON_TYPE_EXTRA:
			ImageName = "Common_Icon/Icon_Extra";
			break;
	}

	/* 画像管理データリストを取得 */
	std::shared_ptr<DataList_Image> pDataList_Image = std::dynamic_pointer_cast<DataList_Image>(gpDataListServer->GetDataList("DataList_Image"));

	/* 指定の画像を読み込む */
	this->Image = pDataList_Image->iGetImageHandle(ImageName);
}

/* 攻撃 */
// コンストラクタ
Action_Effect_Attack::Action_Effect_Attack()
{
	/* 初期化 */
	this->DamageAmount	= 0;				// ダメージ量
	this->IconType		= ICON_TYPE_ATTACK;	// アイコンタイプ:攻撃

	/* 画像設定 */
	Setup_Image();
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
					/* 行動時効果(行動直前)を実行 */
					if (this->EffectCard)
					{
						this->EffectCard->Effect_Action_Before(TargetEnemyCharacter);
					}

					/* ダメージ処理を実行 */
					TargetEnemyCharacter->Damage(this->DamageAmount);

					/* 行動時効果(行動直後)を実行 */
					if (this->EffectCard)
					{
						this->EffectCard->Effect_Action_After(TargetEnemyCharacter);
					}
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
			std::shared_ptr<Character_Base> TargetEnemyCharacter = nullptr;

			/* 対象の敵が存在していて、HPが残っているか確認 */
			for(int i = 0; i < DataList_Battle::POSITION_MAX; i++)
			{
				/* ターゲットの設定 */
				this->Target_Position = (this->Target_Position + i) % DataList_Battle::POSITION_MAX;

				/* ターゲットの敵を取得 */
				TargetEnemyCharacter = this->pDataList_Battle->GetEnemyCharacter(this->Target_Position);

				/* ターゲットの敵が存在していて、HPが1以上であるならループを抜ける */
				if (TargetEnemyCharacter != nullptr && TargetEnemyCharacter->GetHP_Now() > 0)
				{
					break;
				}
			}

			if (TargetEnemyCharacter != nullptr)
			{
				// 対象の敵キャラクターが存在する場合
				/* 行動時効果(行動直前)を実行 */
				if (this->EffectCard)
				{
					this->EffectCard->Effect_Action_Before(TargetEnemyCharacter);
				}

				/* ダメージ処理を実行 */
				TargetEnemyCharacter->Damage(this->DamageAmount);

				/* 行動時効果を実行 */
				if (this->EffectCard)
				{
					this->EffectCard->Effect_Action_After(TargetEnemyCharacter);
				}

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
					/* 行動時効果(行動直前)を実行 */
					if (this->EffectCard)
					{
						this->EffectCard->Effect_Action_Before(TargetFriendCharacter);
					}

					/* ダメージ処理を実行 */
					TargetFriendCharacter->Damage(this->DamageAmount);

					/* 行動時効果を実行 */
					if (this->EffectCard)
					{
						this->EffectCard->Effect_Action_After(TargetFriendCharacter);
					}
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
			std::shared_ptr<Character_Base> TargetFriendCharacter = nullptr;

			/* 対象の仲間が存在していて、HPが残っているか確認 */
			for (int i = 0; i < DataList_Battle::POSITION_MAX; i++)
			{
				/* ターゲットの設定 */
				this->Target_Position = (this->Target_Position + i) % DataList_Battle::POSITION_MAX;

				/* ターゲットの仲間を取得 */
				TargetFriendCharacter = this->pDataList_Battle->GetFriendCharacter(this->Target_Position);

				/* ターゲットの仲間キャラクターが存在していて、HPが1以上であるならループを抜ける */
				if (TargetFriendCharacter != nullptr && TargetFriendCharacter->GetHP_Now() > 0)
				{
					break;
				}
			}

			if (TargetFriendCharacter != nullptr)
			{
				// 対象の仲間キャラクターが存在する場合
				/* 行動時効果(行動直前)を実行 */
				if (this->EffectCard)
				{
					this->EffectCard->Effect_Action_Before(TargetFriendCharacter);
				}

				/* ダメージ処理を実行 */
				TargetFriendCharacter->Damage(this->DamageAmount);

				/* 行動時効果を実行 */
				if (this->EffectCard)
				{
					this->EffectCard->Effect_Action_After(TargetFriendCharacter);
				}

				/* 攻撃リアクションを設定 */
				if (this->EffectUser)
				{
					this->EffectUser->Action_Attack();
				}
			}
		}
	}
}

/* 防御 */
// コンストラクタ
Action_Effect_Defence::Action_Effect_Defence()
{
	/* 初期化 */
	this->ShieldAmount	= 0;					// シールド量
	this->IconType		= ICON_TYPE_DEFENCE;	// アイコンタイプ:防御

	/* 画像設定 */
	Setup_Image();
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
					/* 行動時効果(行動直前)を実行 */
					if (this->EffectCard)
					{
						this->EffectCard->Effect_Action_Before(TargetEnemyCharacter);
					}

					/* シールド付与処理を実行 */
					TargetEnemyCharacter->AddShield(this->ShieldAmount);

					/* 行動時効果を実行 */
					if (this->EffectCard)
					{
						this->EffectCard->Effect_Action_After(TargetEnemyCharacter);
					}
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
				/* 行動時効果(行動直前)を実行 */
				if (this->EffectCard)
				{
					this->EffectCard->Effect_Action_Before(TargetEnemyCharacter);
				}

				/* シールド付与処理を実行 */
				TargetEnemyCharacter->AddShield(this->ShieldAmount);

				/* 行動時効果を実行 */
				if (this->EffectCard)
				{
					this->EffectCard->Effect_Action_After(TargetEnemyCharacter);
				}

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
					/* 行動時効果(行動直前)を実行 */
					if (this->EffectCard)
					{
						this->EffectCard->Effect_Action_Before(TargetFriendCharacter);
					}

					/* シールド付与処理を実行 */
					TargetFriendCharacter->AddShield(this->ShieldAmount);

					/* 行動時効果を実行 */
					if (this->EffectCard)
					{
						this->EffectCard->Effect_Action_After(TargetFriendCharacter);
					}
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
				/* 行動時効果(行動直前)を実行 */
				if (this->EffectCard)
				{
					this->EffectCard->Effect_Action_Before(TargetFriendCharacter);
				}

				/* シールド付与処理を実行 */
				TargetFriendCharacter->AddShield(this->ShieldAmount);

				/* 行動時効果を実行 */
				if (this->EffectCard)
				{
					this->EffectCard->Effect_Action_After(TargetFriendCharacter);
				}

				/* バフ付与リアクションを設定 */
				if (this->EffectUser)
				{
					this->EffectUser->Action_AddBuff();
				}
			}
		}
	}
}

/* 回復 */
// コンストラクタ
Action_Effect_Heal::Action_Effect_Heal()
{
	/* 初期化 */
	this->HealAmount	= 0;				// 回復量
	this->IconType		= ICON_TYPE_HEAL;	// アイコンタイプ:回復

	/* 画像設定 */
	Setup_Image();
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
					/* 行動時効果(行動直前)を実行 */
					if (this->EffectCard)
					{
						this->EffectCard->Effect_Action_Before(TargetEnemyCharacter);
					}

					/* 回復処理を実行 */
					TargetEnemyCharacter->Heal(this->HealAmount);

					/* 行動時効果を実行 */
					if (this->EffectCard)
					{
						this->EffectCard->Effect_Action_After(TargetEnemyCharacter);
					}
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
				/* 行動時効果(行動直前)を実行 */
				if (this->EffectCard)
				{
					this->EffectCard->Effect_Action_Before(TargetEnemyCharacter);
				}

				/* 回復処理を実行 */
				TargetEnemyCharacter->Heal(this->HealAmount);

				/* 行動時効果を実行 */
				if (this->EffectCard)
				{
					this->EffectCard->Effect_Action_After(TargetEnemyCharacter);
				}

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
					/* 行動時効果(行動直前)を実行 */
					if (this->EffectCard)
					{
						this->EffectCard->Effect_Action_Before(TargetFriendCharacter);
					}

					/* 回復処理を実行 */
					TargetFriendCharacter->Heal(this->HealAmount);

					/* 行動時効果を実行 */
					if (this->EffectCard)
					{
						this->EffectCard->Effect_Action_After(TargetFriendCharacter);
					}
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
				/* 行動時効果(行動直前)を実行 */
				if (this->EffectCard)
				{
					this->EffectCard->Effect_Action_Before(TargetFriendCharacter);
				}

				/* 回復処理を実行 */
				TargetFriendCharacter->Heal(this->HealAmount);

				/* 行動時効果を実行 */
				if (this->EffectCard)
				{
					this->EffectCard->Effect_Action_After(TargetFriendCharacter);
				}

				/* バフ付与リアクションを設定 */
				if (this->EffectUser)
				{
					this->EffectUser->Action_AddBuff();
				}
			}
		}
	}
}

/* 特殊効果 */
// コンストラクタ
Action_Effect_Extra::Action_Effect_Extra()
{
	/* 初期化 */
	this->EffectCard	= nullptr;			// 特殊効果を使用するカード
	this->IconType		= ICON_TYPE_EXTRA;	// アイコンタイプ:特殊効果

	/* 画像設定 */
	Setup_Image();
}

// 効果実行
void Action_Effect_Extra::ExecuteEffect()
{
	/* カード効果(特殊効果)を実行 */
	this->EffectCard->Card_Effect_Extra_Process();
}
