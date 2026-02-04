/* カードのベースクラスの定義 */

/* 使用する要素のインクルード */
// ヘッダファイル
#include "Card_Base.h"
// 共通定義
#include "VariableDefine.h"
#include "Action_Effect.h"
// 関連クラス
#include "Scene_Particles_Text.h"
#include "DataList_Image.h"
#include "DataList_Battle.h"
#include "Character_Base.h"

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
	this->CardState		= CARDSTATE_NONE;		// カード状態
	// 画像
	this->Image = MakeScreen(IMAGE_SIZE_WIDTH, IMAGE_SIZE_HEIGHT, TRUE);
	this->RotateAngle = 0.f;
	// その他
	this->Now_Position		= { 0, 0 };		// 現在座標(ドローしてる感を出すため山札の位置を初期値に設定)
	this->Setting_Position	= { 0, 0 };		// 設定座標(ホールドが解除された際に自動で補正される座標)
	this->bLostFlag			= false;		// 削除フラグ
	this->iNowChainCount	= 0;			// 現在のチェイン数(ターン開始時に設定)
	this->pPlayer			= nullptr;		// プレイヤーキャラクターのポインタ

	/* 画像取得 */
	/* 画像管理データリストを取得 */
	std::shared_ptr<DataList_Image> pDataList_Image = std::dynamic_pointer_cast<DataList_Image>(gpDataListServer->GetDataList("DataList_Image"));

	/* 裏面画像を取得 */
	std::string ImageFilePath = "Card_Commoon/Card_BackSide";
	Image_BackSide = pDataList_Image->iGetImageHandle(ImageFilePath);
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
							addEffect->Priority			= 100 - this->GetMyAreaNo();			// 100-設定されたエリア番号を優先順位に設定
							addEffect->EffectCard		= shared_from_this();					// 効果を使用するカード:このカード
							this->pDataList_Battle->AddEffect(addEffect);
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
							addEffect->Priority			= 100 - this->GetMyAreaNo();			// 100-設定されたエリア番号を優先順位に設定
							addEffect->EffectCard		= shared_from_this();					// 効果を使用するカード:このカード
							this->pDataList_Battle->AddEffect(addEffect);
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
					addEffect->Priority			= 100 - this->GetMyAreaNo();			// 100-設定されたエリア番号を優先順位に設定
					addEffect->EffectCard		= shared_from_this();					// 効果を使用するカード:このカード
					this->pDataList_Battle->AddEffect(addEffect);
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
		addEffect->Priority			= 100 - this->GetMyAreaNo();			// 100-設定されたエリア番号を優先順位に設定
		addEffect->EffectCard		= shared_from_this();					// 効果を使用するカード:このカード
		this->pDataList_Battle->AddEffect(addEffect);
	}
}

// 更新処理
void Card_Base::Update()
{
	/* 状態に応じて設定を変更する */
	bool Complement_SkipFlg = false;	// 補完処理をスキップするか
	switch (this->CardState)
	{
		case CARDSTATE_DECK:	// デッキ
			/* 設定座標の設定 */
			this->Setting_Position.iX = SCREEN_SIZE_WIDE - 50;
			this->Setting_Position.iY = SCREEN_SIZE_HEIGHT / 2 + 100;

			/* 回転角度の設定 */
			this->RotateAngle = -0.7853f;	// -45度
			break;

		case CARDSTATE_PICKED:	// ピックアップ中
			/* 補完処理をスキップ */
			Complement_SkipFlg = true;
			break;

		case CARDSTATE_HAND:					// 手札
		case CARDSTATE_SETTING:					// 設定中
		case CARDSTATE_GETLIST_BACK:			// 取得一覧:裏面
		case CARDSTATE_GETLIST_FRONT:			// 取得一覧:表面
		case CARDSTATE_GETLIST_FRONT_DELETE:	// 取得一覧:表面(削除選択)
		case CARDSTATE_SHOP_DISPLAY:			// ショップ:陳列中
		case CARDSTATE_SHOP_SELLED:				// ショップ:購入済み
			/* 特殊な処理は無し */
			break;

		case CARDSTATE_TRASH:	// 捨て札
		case CARDSTATE_LOST:	// ロスト
			/* 設定座標の設定 */
			this->Setting_Position.iX = 100;
			this->Setting_Position.iY = SCREEN_SIZE_HEIGHT / 2 + 100;
			break;
	}

	/* スキップフラグが無効であるなら補完処理を実施 */
	if (Complement_SkipFlg == false)
	{
		/* 座標補完処理 */
		Complement_Position();

		/* 回転補完処理 */
		Complement_Rotate();
	}
}

// 描画
void Card_Base::Draw()
{
	/* フラグ定義 */
	bool bBackSideFlg	= false;	// 裏面を描画するか
	bool bDarkToneFlg	= false;	// 暗く描画するか

	/* 状態に応じて設定を変更する */
	switch (this->CardState)
	{
		case CARDSTATE_DECK:			// デッキ
		case CARDSTATE_GETLIST_BACK:	// 取得一覧:裏面
			/* 裏面を描画 */
			bBackSideFlg = true;
			break;

		case CARDSTATE_HAND:			// 手札
		case CARDSTATE_PICKED:			// ピックアップ中
		case CARDSTATE_SETTING:			// 設定中
		case CARDSTATE_GETLIST_FRONT:	// 取得一覧:表面
		case CARDSTATE_SHOP_DISPLAY:	// ショップ:陳列中
			/* 特殊な処理は無し */
			break;

		case CARDSTATE_TRASH:					// 捨て札
		case CARDSTATE_LOST:					// ロスト
		case CARDSTATE_GETLIST_FRONT_DELETE:	// 取得一覧:表面(削除選択)
		case CARDSTATE_SHOP_SELLED:				// ショップ:購入済み
			/* 暗く描写 */
			bDarkToneFlg = true;
			break;
	}

	/* 描画対象ハンドル */
	int drawHandle = bBackSideFlg ? *(this->Image_BackSide) : this->Image;

	/* 拡大率は 1.0f（比率を変えない） */
	const float scale = 1.0f;

	// 暗く描写するなら設定を暗くする (既存処理を維持)
	if (bDarkToneFlg)
	{
		SetDrawBright(100, 100, 100);
	}

	/* 描写 */
	DrawRotaGraph(
		this->Now_Position.iX,		// 描画中心 X
		this->Now_Position.iY,		// 描画中心 Y
		static_cast<double>(scale),
		static_cast<double>(this->RotateAngle),
		drawHandle,
		TRUE
	);

	/* 描画輝度を元に戻す */
	SetDrawBright(255, 255, 255);
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

	/* 追加テキスト描写 */
	DrawExText();

	/* 描画先を裏画面に戻す */
	SetDrawScreen(DX_SCREEN_BACK);
}

// 自身のバトルエリア番号を取得
int Card_Base::GetMyAreaNo()
{
	// 戻り値
	// int <- 自身のバトルエリア番号(ないなら-1)

	if (this->pDataList_Battle != nullptr)
	{
		/* バトルエリアを巡回し、自身と同一オブジェクトの shared_ptr を探す */
		for (int i = 0; i < DataList_Battle::BATTLE_AREA_MAX; ++i)
		{
			std::shared_ptr<Card_Base> pAreaCard = this->pDataList_Battle->GetBattleAreaCardList(i);
			if (pAreaCard != nullptr && pAreaCard.get() == this)
			{
				return i;
			}
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

	/* 攻撃力 + バフ量が100以上にならないように設定する */
	if (this->Strength + this->Strength_Buff + amount > 99)
	{
		amount = 100 - (this->Strength + this->Strength_Buff);
	}

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

	/* 防御力 + バフ量が100以上にならないように設定する */
	if (this->Diffence + this->Diffence_Buff + amount > 99)
	{
		amount = 100 - (this->Diffence + this->Diffence_Buff);
	}

	/* バフ量を加算 */
	this->Diffence_Buff += amount;

	/* 画像更新 */
	UpdateImage();
}

// マウスカーソルがカード上にあるか確認
bool Card_Base::MouseInCard()
{
	// 戻り値
	// bool <- マウスカーソルがカード上にあるなら true、ないなら false

	/* マウス座標を取得 */
	Struct_2D::POSITION MousePosition = {
		gstKeyboardInputData.iMouseX,
		gstKeyboardInputData.iMouseY
	};

	/* カードの範囲を定義 */
	Struct_2D::RANGE CardRange = {
		this->Now_Position.iX - (CARD_WIDTH / 2),
		this->Now_Position.iY - (CARD_HEIGHT / 2),
		this->Now_Position.iX + (CARD_WIDTH / 2),
		this->Now_Position.iY + (CARD_HEIGHT / 2)
	};

	/* 2D範囲内にマウス座標が存在するか確認 */
	if (PUBLIC_PROCESS::bPositionIn2DRangeCheck(MousePosition, CardRange))
	{
		// 存在するならばtrueを返す
		return true;
	}
	else
	{
		// 存在しないならばfalseを返す
		return false;
	}
}

// データリストの取得処理を行う
void Card_Base::SetUp_DataList()
{
	/* バトル用データリスト */
	this->pDataList_Battle = std::dynamic_pointer_cast<DataList_Battle>(gpDataListServer->GetDataList("DataList_Battle"));
}

// バフのリセット処理
void Card_Base::Reset_Buff()
{
	/* バフをリセット */
	this->Strength_Buff = 0;
	this->Diffence_Buff = 0;

	/* 画像更新 */
	UpdateImage();
}

// 座標補完処理
void Card_Base::Complement_Position()
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

// 回転補完処理
void Card_Base::Complement_Rotate()
{
	/* 角度を0に補完する(+なら-、-なら+の値へ) */
	if (std::abs(this->RotateAngle) < ROTATE_INTERPOLATION_SPEED)
	{
		this->RotateAngle = 0.f;
	}
	else if (this->RotateAngle > 0.f)
	{
		this->RotateAngle -= ROTATE_INTERPOLATION_SPEED;
	}
	else if (this->RotateAngle < 0.f)
	{
		this->RotateAngle += ROTATE_INTERPOLATION_SPEED;
	}
}
