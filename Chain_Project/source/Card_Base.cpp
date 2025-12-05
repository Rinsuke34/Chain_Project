/* カードのベースクラスの定義 */

/* 使用する要素のインクルード */
// ヘッダファイル
#include "Card_Base.h"
// 関連クラス
#include "DataList_Image.h"
#include "DataList_Battle.h"
#include "Character_Base.h"
// 共通定義
#include "VariableDefine.h"
#include "Action_Effect.h"

// コンストラクタ
Card_Base::Card_Base()
{
	/* 初期化 */
	// カード情報
	this->iRarity		= 0;					// レアリティ
	this->iCardType		= 0;					// カードの種類
	this->Name			= "";					// カード名
	this->Strength		= 0;					// 攻撃力
	this->Diffence		= 0;					// 防御力
	this->Strength_Buff	= 0;					// 攻撃力バフ
	this->Diffence_Buff	= 0;					// 防御力バフ
	this->AttackRange	= ATTACKRANGE_FRONT;	// 攻撃範囲
	// 画像
	this->Image = MakeScreen(IMAGE_SIZE_WIDTH, IMAGE_SIZE_HEIGHT, TRUE);
	// その他
	this->Now_Position		= { SCREEN_SIZE_WIDE, SCREEN_SIZE_HEIGHT / 2 };	// 現在座標(ドローしてる感を出すため山札の位置を初期値に設定)
	this->Setting_Position	= { 0, 0 };										// 設定座標(ホールドが解除された際に自動で補正される座標)
	this->bLostFlag			= false;										// 削除フラグ
	this->iNowChainCount	= 0;											// 現在のチェイン数(ターン開始時に設定)
	this->pPlayer			= nullptr;										// プレイヤーキャラクターのポインタ

	/* データリスト取得 */
	// バトル用データリスト
	this->pDataList_Battle = std::dynamic_pointer_cast<DataList_Battle>(gpDataListServer->GetDataList("DataList_Battle"));
}

// デストラクタ
Card_Base::~Card_Base()
{
	/* 画像を削除 */
	DeleteGraph(this->Image);
}

// 戦闘行動
void Card_Base::BattleAction()
{
	/* データリストが無効であるならば処理を行わない */
	if (this->pDataList_Battle == nullptr)
	{
		return;
	}

	/* プレイヤーを取得できていないならプレイヤーを取得する */
	CheckHavePlayer();

	/* 攻撃力が1以上であるか確認 */
	if (this->Strength + this->Strength_Buff > 0)
	{
		/* 攻撃範囲に応じて処理を変更 */
		switch (this->AttackRange)
		{
			// 先頭から
			case ATTACKRANGE_FRONT:
				{
					/* 前衛から順に敵が存在するか確認 */
					for (int i = 0; i < DataList_Battle::POSITION_MAX; i++)
					{
						/* 敵キャラクターが存在するか確認 */
						std::shared_ptr<Character_Base> pEnemyCharacter = this->pDataList_Battle->GetEnemyCharacter(i);
						if (pEnemyCharacter != nullptr)
						{
							/* 存在するなら */
							// 攻撃行動を設定する
							std::shared_ptr<Action_Effect_Attack> addEffect = std::make_shared<Action_Effect_Attack>();
							addEffect->Target_Camp		= Character_Base::CAMP_ENEMY;			// 効果対象の陣営:敵
							addEffect->Target_Position	= i;									// 効果対象の立ち位置:確認した敵キャラクターの位置
							addEffect->DamageAmount		= this->Strength + this->Strength_Buff;	// ダメージ量:カードの攻撃力
							addEffect->AllRange			= false;								// 全体攻撃でない
							addEffect->EffectUser		= this->pPlayer;						// 効果の使用者:プレイヤーキャラクター
							addEffect->Priority			= 5;									// 敵よりは高め
							this->pDataList_Battle->AddEffect(addEffect, GetMyAreaNo());
							break;
						}
					}
				}
				break;

			// ランダム
			case ATTACKRANGE_RANDOM:
				{
					/* ランダムな相手に攻撃処理を行う */
					while (true)
					{
						/* 0～2のランダムな数値を出す */
						int positionNo = GetRand(DataList_Battle::POSITION_MAX - 1);

						/* 敵キャラクターが存在するか確認 */
						std::shared_ptr<Character_Base> pEnemyCharacter = this->pDataList_Battle->GetEnemyCharacter(positionNo);
						if (pEnemyCharacter != nullptr)
						{
							/* 存在するなら */
							// 攻撃行動を設定する
							std::shared_ptr<Action_Effect_Attack> addEffect = std::make_shared<Action_Effect_Attack>();
							addEffect->Target_Camp		= Character_Base::CAMP_ENEMY;			// 効果対象の陣営:敵
							addEffect->Target_Position	= positionNo;							// 効果対象の立ち位置:確認した敵キャラクターの位置
							addEffect->DamageAmount		= this->Strength + this->Strength_Buff;	// ダメージ量:カードの攻撃力
							addEffect->AllRange			= false;								// 全体攻撃でない
							addEffect->EffectUser		= this->pPlayer;						// 効果の使用者:プレイヤーキャラクター
							addEffect->Priority			= 5;									// 敵よりは高め
							this->pDataList_Battle->AddEffect(addEffect, GetMyAreaNo());
							break;
						}
					}
				}
				break;

			// 全体
			case ATTACKRANGE_ALL:
				{
					/* 全体攻撃を行う */
					// 攻撃行動を設定する
					std::shared_ptr<Action_Effect_Attack> addEffect = std::make_shared<Action_Effect_Attack>();
					addEffect->Target_Camp		= Character_Base::CAMP_ENEMY;			// 効果対象の陣営:敵
					addEffect->Target_Position	= 0;									// 効果対象の立ち位置:無し
					addEffect->DamageAmount		= this->Strength + this->Strength_Buff;	// ダメージ量:カードの攻撃力
					addEffect->AllRange			= true;									// 全体攻撃である
					addEffect->EffectUser		= this->pPlayer;						// 効果の使用者:プレイヤーキャラクター
					addEffect->Priority			= 5;									// 敵よりは高め
					this->pDataList_Battle->AddEffect(addEffect, GetMyAreaNo());
				}
				break;
		}
	}

	/* 防御力が1以上であるか確認 */
	if (this->Diffence + this->Diffence_Buff > 0)
	{
		/* 自陣営の全キャラクターに防御力分のシールドを付与する */
		// シールド付与を設定する
		std::shared_ptr<Action_Effect_Defence> addEffect = std::make_shared<Action_Effect_Defence>();
		addEffect->Target_Camp		= Character_Base::CAMP_FRIEND;			// 効果対象の陣営:仲間
		addEffect->Target_Position	= 0;									// 効果対象の立ち位置:無し
		addEffect->ShieldAmount		= this->Diffence + this->Diffence_Buff;	// シールド付与量:カードの防御力
		addEffect->EffectUser		= this->pPlayer;						// 効果の使用者:プレイヤーキャラクター
		addEffect->AllRange			= true;									// 全体付与である
		addEffect->Priority			= 100;									// そうとう優先度高め
		this->pDataList_Battle->AddEffect(addEffect, GetMyAreaNo());
	}
}

// 更新処理
void Card_Base::Update()
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
		this->Now_Position.iX - (IMAGE_SIZE_WIDTH / 2),
		this->Now_Position.iY - (IMAGE_SIZE_HEIGHT / 2),
		this->Image,
		TRUE
	);
}

// 画像更新
// ※ カードの要素に応じて画像を更新する
void Card_Base::UpdateImage()
{
	/* 描画先をカード画像に設定 */
	SetDrawScreen(this->Image);

	/* 画像をクリア */
	ClearDrawScreen();

	/* 背景描写 */
	DrawBackGround();

	/* イラスト描写 */
	DrawImage();

	/* フレーム描写 */
	DrawFrame();

	/* スート描写 */
	DrawSuite();

	/* カード名描写 */
	DrawName();

	/* 描画先を裏画面に戻す */
	SetDrawScreen(DX_SCREEN_BACK);
}

// 自身のバトルエリア番号を取得
int Card_Base::GetMyAreaNo()
{
	// 戻り値
	// int <- 自身のバトルエリア番号(ないなら-1)

	/* バトルエリアを巡回し、自身と同一オブジェクトの shared_ptr を探す */
	for (int i = 0; i < DataList_Battle::BATTLE_AREA_MAX; ++i)
	{
		std::shared_ptr<Card_Base> pAreaCard = this->pDataList_Battle->GetBattleAreaCardList(i);
		if (pAreaCard != nullptr && pAreaCard.get() == this)
		{
			return i;
		}
	}

	/* 見つからなければ -1 を返す */
	return -1;
}

// プレイヤーを取得しているか確認
void Card_Base::CheckHavePlayer()
{
	if (this->pPlayer == nullptr)
	{
		// バトル用データリストからプレイヤーキャラクターを取得する
		for (int i = 0; i < DataList_Battle::POSITION_MAX; i++)
		{
			if (this->pDataList_Battle->GetFriendCharacter(i))
			{
				this->pPlayer = this->pDataList_Battle->GetFriendCharacter(i);
				break;
			}
		}
	}
}

// 攻撃力バフ追加
void Card_Base::Add_Strength_Buff(int amount)
{
	// 引数
	// amount	<- 加算するバフ量

	/* バフ量を加算 */
	this->Strength_Buff += amount;

	/* 画像更新 */
	UpdateImage();
}

// 防御力バフ追加
void Card_Base::Add_Diffence_Buff(int amount)
{
	// 引数
	// amount	<- 加算するバフ量

	/* バフ量を加算 */
	this->Diffence_Buff += amount;

	/* 画像更新 */
	UpdateImage();
}