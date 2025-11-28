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
#include "StructDefine.h"

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

	/* 攻撃力が1以上であるか確認 */
	if (this->Strength > 0)
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
							std::shared_ptr<Card_Effect_Attack> addEffect = std::make_shared<Card_Effect_Attack>();
							addEffect->Target_Camp		= Character_Base::CAMP_ENEMY;	// 効果対象の陣営:敵
							addEffect->Target_Position	= i;							// 効果対象の立ち位置:確認した敵キャラクターの位置
							addEffect->DamageAmount		= this->Strength;				// ダメージ量:カードの攻撃力
							addEffect->EffectUser		= this->pPlayer;				// 効果の使用者:プレイヤーキャラクター
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
							std::shared_ptr<Card_Effect_Attack> addEffect = std::make_shared<Card_Effect_Attack>();
							addEffect->Target_Camp		= Character_Base::CAMP_ENEMY;	// 効果対象の陣営:敵
							addEffect->Target_Position	= positionNo;					// 効果対象の立ち位置:確認した敵キャラクターの位置
							addEffect->DamageAmount		= this->Strength;				// ダメージ量:カードの攻撃力
							addEffect->EffectUser		= this->pPlayer;				// 効果の使用者:プレイヤーキャラクター
							this->pDataList_Battle->AddEffect(addEffect, GetMyAreaNo());
							break;
						}
					}
				}
				break;

			// 全体
			case ATTACKRANGE_ALL:
				{
					/* 全体攻撃処理を行う */
					for (int i = 0; i < DataList_Battle::POSITION_MAX; i++)
					{
						/* 敵キャラクターが存在するか確認 */
						std::shared_ptr<Character_Base> pEnemyCharacter = this->pDataList_Battle->GetEnemyCharacter(i);
						if (pEnemyCharacter != nullptr)
						{
							/* 存在するなら */
							// 攻撃行動を設定する
							std::shared_ptr<Card_Effect_Attack> addEffect = std::make_shared<Card_Effect_Attack>();
							addEffect->Target_Camp		= Character_Base::CAMP_ENEMY;	// 効果対象の陣営:敵
							addEffect->Target_Position	= i;							// 効果対象の立ち位置:確認した敵キャラクターの位置
							addEffect->DamageAmount		= this->Strength;				// ダメージ量:カードの攻撃力
							addEffect->EffectUser		= this->pPlayer;				// 効果の使用者:プレイヤーキャラクター
							this->pDataList_Battle->AddEffect(addEffect, GetMyAreaNo());
						}
					}
				}
				break;
		}
	}

	/* 防御力が1以上であるか確認 */
	if (this->Diffence > 0)
	{
		/* 自陣営の全キャラクターに防御力分のシールドを付与する */
		for (int i = 0; i < DataList_Battle::POSITION_MAX; i++)
		{
			/* 仲間キャラクターが存在するか確認 */
			std::shared_ptr<Character_Base> pFriendCharacter = this->pDataList_Battle->GetFriendCharacter(i);
			if (pFriendCharacter != nullptr)
			{
				/* 存在するなら */
				// シールド付与を設定する
				std::shared_ptr<Card_Effect_Defence> addEffect = std::make_shared<Card_Effect_Defence>();
				addEffect->Target_Camp		= Character_Base::CAMP_FRIEND;	// 効果対象の陣営:仲間
				addEffect->Target_Position	= i;							// 効果対象の立ち位置:確認した仲間キャラクターの位置
				addEffect->ShieldAmount		= this->Diffence;				// シールド付与量:カードの防御力
				addEffect->EffectUser		= this->pPlayer;				// 効果の使用者:プレイヤーキャラクター
				this->pDataList_Battle->AddEffect(addEffect, GetMyAreaNo());
			}
		}		
	}
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

// 背景を描写
void Card_Base::DrawBackGround()
{
	/* 画像管理データリストを取得 */
	std::shared_ptr<DataList_Image> pDataList_Image = std::dynamic_pointer_cast<DataList_Image>(gpDataListServer->GetDataList("DataList_Image"));

	/* 背景の画像を取得 */
	std::string ImageFilePath = "Card_Commoon/BackGround_Test";
	std::shared_ptr<int> Image = pDataList_Image->iGetImageHandle(ImageFilePath);

	/* 背景描写 */
	DrawModiGraph(
		(IMAGE_SIZE_WIDTH / 2) - (CARD_WIDTH / 2), (IMAGE_SIZE_HEIGHT / 2) - (CARD_HEIGHT / 2),
		(IMAGE_SIZE_WIDTH / 2) + (CARD_WIDTH / 2), (IMAGE_SIZE_HEIGHT / 2) - (CARD_HEIGHT / 2),
		(IMAGE_SIZE_WIDTH / 2) + (CARD_WIDTH / 2), (IMAGE_SIZE_HEIGHT / 2) + (CARD_HEIGHT / 2),
		(IMAGE_SIZE_WIDTH / 2) - (CARD_WIDTH / 2), (IMAGE_SIZE_HEIGHT / 2) + (CARD_HEIGHT / 2),
		*(Image),
		TRUE
	);
}

// イラストを描写
void Card_Base::DrawImage()
{
	/* 画像管理データリストを取得 */
	std::shared_ptr<DataList_Image> pDataList_Image = std::dynamic_pointer_cast<DataList_Image>(gpDataListServer->GetDataList("DataList_Image"));

	/* イラストの画像を取得 */
	std::string ImageFilePath = "Card_Ilust/" + this->ImageName;
	std::shared_ptr<int> Image = pDataList_Image->iGetImageHandle(ImageFilePath);

	/* イラスト描写 */
	DrawModiGraph(
		(IMAGE_SIZE_WIDTH / 2) - (CARD_WIDTH / 2), (IMAGE_SIZE_HEIGHT / 2) - (CARD_WIDTH / 2) + ILLUSTRATION_POS_Y,
		(IMAGE_SIZE_WIDTH / 2) + (CARD_WIDTH / 2), (IMAGE_SIZE_HEIGHT / 2) - (CARD_WIDTH / 2) + ILLUSTRATION_POS_Y,
		(IMAGE_SIZE_WIDTH / 2) + (CARD_WIDTH / 2), (IMAGE_SIZE_HEIGHT / 2) + (CARD_WIDTH / 2) + ILLUSTRATION_POS_Y,
		(IMAGE_SIZE_WIDTH / 2) - (CARD_WIDTH / 2), (IMAGE_SIZE_HEIGHT / 2) + (CARD_WIDTH / 2) + ILLUSTRATION_POS_Y,
		*(Image),
		TRUE
	);
}

// スートを描写
void Card_Base::DrawSuite()
{
	/* 画像管理データリストを取得 */
	std::shared_ptr<DataList_Image> pDataList_Image = std::dynamic_pointer_cast<DataList_Image>(gpDataListServer->GetDataList("DataList_Image"));

	// 画像[0:フレーム, 1:スート]
	std::shared_ptr<int> Image[2];

	/* スートのフレームの画像を取得 */
	std::string ImageFilePath = "Card_Commoon/SuitFrame_Test";
	Image[0] = pDataList_Image->iGetImageHandle(ImageFilePath);

	/* スートの画像を取得し、描写する */
	for (int i = 0; i < this->Suite_List.size(); i++)
	{
		/* スート画像を取得 */
		ImageFilePath = "Card_Suit/" + this->Suite_List[i];
		Image[1] = pDataList_Image->iGetImageHandle(ImageFilePath);

		/* 描写 */
		for (int x = 0; x < 2; x++)
		{
			DrawModiGraph(
				(IMAGE_SIZE_WIDTH / 2) - (CARD_WIDTH / 2) + (i * SUTE_SIZE_WIDTH) + SUTE_START_POS_X,		(IMAGE_SIZE_HEIGHT / 2) - (CARD_HEIGHT / 2) + SUTE_POS_Y,
				(IMAGE_SIZE_WIDTH / 2) - (CARD_WIDTH / 2) + ((i + 1) * SUTE_SIZE_WIDTH) + SUTE_START_POS_X,	(IMAGE_SIZE_HEIGHT / 2) - (CARD_HEIGHT / 2) + SUTE_POS_Y,
				(IMAGE_SIZE_WIDTH / 2) - (CARD_WIDTH / 2) + ((i + 1) * SUTE_SIZE_WIDTH) + SUTE_START_POS_X,	(IMAGE_SIZE_HEIGHT / 2) - (CARD_HEIGHT / 2) + SUTE_POS_Y + SUTE_SIZE_HEIGHT,
				(IMAGE_SIZE_WIDTH / 2) - (CARD_WIDTH / 2) + (i * SUTE_SIZE_WIDTH) + SUTE_START_POS_X,		(IMAGE_SIZE_HEIGHT / 2) - (CARD_HEIGHT / 2) + SUTE_POS_Y + SUTE_SIZE_HEIGHT,
				*(Image[x]),
				TRUE
			);
		}		
	}
}

// フレームを描写
void Card_Base::DrawFrame()
{
	/* 画像管理データリストを取得 */
	std::shared_ptr<DataList_Image> pDataList_Image = std::dynamic_pointer_cast<DataList_Image>(gpDataListServer->GetDataList("DataList_Image"));

	/* フレームの画像を取得 */
	std::string ImageFilePath = "Card_Commoon/Frame_Test";
	std::shared_ptr<int> Image = pDataList_Image->iGetImageHandle(ImageFilePath);

	/* フレームサイズ取得 */
	int SizeX, SizeY;
	GetGraphSize(*(Image), &SizeX, &SizeY);

	/* フレーム描写 */
	DrawModiGraph(
		(IMAGE_SIZE_WIDTH / 2) - (CARD_WIDTH / 2), (IMAGE_SIZE_HEIGHT / 2) - (CARD_HEIGHT / 2),
		(IMAGE_SIZE_WIDTH / 2) + (CARD_WIDTH / 2), (IMAGE_SIZE_HEIGHT / 2) - (CARD_HEIGHT / 2),
		(IMAGE_SIZE_WIDTH / 2) + (CARD_WIDTH / 2), (IMAGE_SIZE_HEIGHT / 2) + (CARD_HEIGHT / 2),
		(IMAGE_SIZE_WIDTH / 2) - (CARD_WIDTH / 2), (IMAGE_SIZE_HEIGHT / 2) + (CARD_HEIGHT / 2),
		*(Image),
		TRUE
	);
}

// 名前を描写
void Card_Base::DrawName()
{
	/* カード名のフレーム描写 */
	DrawBox(
		(IMAGE_SIZE_WIDTH / 2) - (NAMEPLATE_WIDTH / 2),
		(IMAGE_SIZE_HEIGHT / 2) - (CARD_HEIGHT / 2) + NAMEPLATE_POSITION_Y + (NAMEPLATE_HEIGHT / 2),
		(IMAGE_SIZE_WIDTH / 2) + (NAMEPLATE_WIDTH / 2),
		(IMAGE_SIZE_HEIGHT / 2) - (CARD_HEIGHT / 2) + NAMEPLATE_POSITION_Y - (NAMEPLATE_HEIGHT / 2),
		GetColor(255, 255, 255),
		TRUE
	);

	/* カード名描写 */
	{
		// 文字列の高さ、幅を取得
		int iSizeX = GetDrawStringWidthToHandle(this->Name.c_str(), static_cast<int>(strlenDx(this->Name.c_str())), giFont_Cp_Period_16);
		int iSizeY = GetFontSizeToHandle(giFont_Cp_Period_16);

		// 文字列描写
		DrawStringToHandle(
			(IMAGE_SIZE_WIDTH / 2) - (iSizeX / 2),
			(IMAGE_SIZE_HEIGHT / 2) - (CARD_HEIGHT / 2) + NAMEPLATE_POSITION_Y - (iSizeY / 2),
			this->Name.c_str(),
			GetColor(0, 0, 0),
			giFont_Cp_Period_16
		);
	}
}
