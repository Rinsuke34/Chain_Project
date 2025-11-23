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
	this->bDeleteFlag		= false;										// 削除フラグ
	this->iNowChainCount	= 0;											// 現在のチェイン数(ターン開始時に設定)

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
							Card_Effect_Attack addEffect;
							addEffect.Target_Camp		= Character_Base::CAMP_ENEMY;	// 効果対象の陣営:敵
							addEffect.Target_Position	= i;							// 効果対象の立ち位置:確認した敵キャラクターの位置
							addEffect.Damage			= this->Strength;				// ダメージ量:カードの攻撃力
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
							Card_Effect_Attack addEffect;
							addEffect.Target_Camp		= Character_Base::CAMP_ENEMY;	// 効果対象の陣営:敵
							addEffect.Target_Position	= positionNo;					// 効果対象の立ち位置:確認した敵キャラクターの位置
							addEffect.Damage			= this->Strength;				// ダメージ量:カードの攻撃力
							this->pDataList_Battle->AddEffect(addEffect);
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
							Card_Effect_Attack addEffect;
							addEffect.Target_Camp = Character_Base::CAMP_ENEMY;	// 効果対象の陣営:敵
							addEffect.Target_Position = i;						// 効果対象の立ち位置:確認した敵キャラクターの位置
							addEffect.Damage = this->Strength;					// ダメージ量:カードの攻撃力
							this->pDataList_Battle->AddEffect(addEffect);
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
				Card_Effect_Defence addEffect;
				addEffect.Target_Camp		= Character_Base::CAMP_FRIEND;	// 効果対象の陣営:仲間
				addEffect.Target_Position	= i;							// 効果対象の立ち位置:確認した仲間キャラクターの位置
				addEffect.Shield			= this->Diffence;				// シールド付与量:カードの防御力
				this->pDataList_Battle->AddEffect(addEffect);
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
	/* 要素からカード画像を生成する */
	// 0:背景, 1:イラスト, 2:フレーム
	std::shared_ptr<int> Image[3];

	/* 画像管理データリストを取得 */
	std::shared_ptr<DataList_Image> pDataList_Image = std::dynamic_pointer_cast<DataList_Image>(gpDataListServer->GetDataList("DataList_Image"));

	/* 背景の画像を取得 */
	std::string ImageFilePath = "Card_Commoon/BackGround_Test";
	Image[0] = pDataList_Image->iGetImageHandle(ImageFilePath);

	/* イラストの画像を取得 */
	ImageFilePath = "Card_Ilust/" + this->ImageName;
	Image[1] = pDataList_Image->iGetImageHandle(ImageFilePath);

	/* フレームの画像を取得 */
	ImageFilePath = "Card_Commoon/Frame_Test";
	Image[2] = pDataList_Image->iGetImageHandle(ImageFilePath);

	/* 描画先をカード画像に設定 */
	SetDrawScreen(this->Image);

	/* 画像をクリア */
	ClearDrawScreen();

	/* 背景描写 */
	{
		/* 背景描写 */
		DrawModiGraph(
			(IMAGE_SIZE_WIDTH / 2) - (CARD_WIDTH / 2), (IMAGE_SIZE_HEIGHT / 2) - (CARD_HEIGHT / 2),
			(IMAGE_SIZE_WIDTH / 2) + (CARD_WIDTH / 2), (IMAGE_SIZE_HEIGHT / 2) - (CARD_HEIGHT / 2),
			(IMAGE_SIZE_WIDTH / 2) + (CARD_WIDTH / 2), (IMAGE_SIZE_HEIGHT / 2) + (CARD_HEIGHT / 2),
			(IMAGE_SIZE_WIDTH / 2) - (CARD_WIDTH / 2), (IMAGE_SIZE_HEIGHT / 2) + (CARD_HEIGHT / 2),
			*(Image[0]),
			TRUE
		);
	}


	/* イラスト描写 */
	{
		/* イラスト描写 */
		DrawModiGraph(
			(IMAGE_SIZE_WIDTH / 2) - (CARD_WIDTH / 2), (IMAGE_SIZE_HEIGHT / 2) - (CARD_WIDTH / 2),
			(IMAGE_SIZE_WIDTH / 2) + (CARD_WIDTH / 2), (IMAGE_SIZE_HEIGHT / 2) - (CARD_WIDTH / 2),
			(IMAGE_SIZE_WIDTH / 2) + (CARD_WIDTH / 2), (IMAGE_SIZE_HEIGHT / 2) + (CARD_WIDTH / 2),
			(IMAGE_SIZE_WIDTH / 2) - (CARD_WIDTH / 2), (IMAGE_SIZE_HEIGHT / 2) + (CARD_WIDTH / 2),
			*(Image[1]),
			TRUE
		);
	}

	/* フレーム描写 */
	{
		/* フレームサイズ取得 */
		int SizeX, SizeY;
		GetGraphSize(*(Image[0]), &SizeX, &SizeY);

		/* フレーム描写 */
		DrawModiGraph(
			(IMAGE_SIZE_WIDTH / 2) - (CARD_WIDTH / 2), (IMAGE_SIZE_HEIGHT / 2) - (CARD_HEIGHT / 2),
			(IMAGE_SIZE_WIDTH / 2) + (CARD_WIDTH / 2), (IMAGE_SIZE_HEIGHT / 2) - (CARD_HEIGHT / 2),
			(IMAGE_SIZE_WIDTH / 2) + (CARD_WIDTH / 2), (IMAGE_SIZE_HEIGHT / 2) + (CARD_HEIGHT / 2),
			(IMAGE_SIZE_WIDTH / 2) - (CARD_WIDTH / 2), (IMAGE_SIZE_HEIGHT / 2) + (CARD_HEIGHT / 2),
			*(Image[2]),
			TRUE
		);
	}

	/* カード名のフレーム描写 */
	DrawBox(
		(IMAGE_SIZE_WIDTH / 2)	- (NAMEPLATE_WIDTH / 2),
		(IMAGE_SIZE_HEIGHT / 2)	- (CARD_HEIGHT / 2) + NAMEPLATE_POSITION_Y + (NAMEPLATE_HEIGHT / 2),
		(IMAGE_SIZE_WIDTH / 2)	+ (NAMEPLATE_WIDTH / 2),
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

	/* 描画先を裏画面に戻す */
	SetDrawScreen(DX_SCREEN_BACK);
}
