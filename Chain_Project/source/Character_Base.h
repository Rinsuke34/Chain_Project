/* キャラクターのベースクラスの宣言 */

#pragma once

/* 使用する要素のインクルード */
// 標準ライブラリ
#include <memory>
// 共通定義
#include "AppFrame.h"

// 前方宣言
class DataList_Battle;
class Character_Buff_Debuff_Base;
class Action_Effect_Base;
class Card_Base;

// キャラクターのベースクラス
class Character_Base : public std::enable_shared_from_this<Character_Base>
{
	public:
		Character_Base();				// コンストラクタ
		virtual ~Character_Base()	{};	// デストラクタ

		/* 関数 */
		virtual void Draw();					// 描画
		virtual void Draw_StatusBar();			// ステータスバーを描写
		virtual void Action()		{};			// 行動
		virtual void Action_Attack();			// 攻撃アクション
		virtual void Action_AddBuff();			// バフ付与アクション
		virtual void Damage(int DamageAmount);	// ダメージ処理
		virtual void AddShield(int Shield);		// シールド追加処理
		virtual void ShieldReset_EndTurn();		// シールドリセット(ターン終了時)
		virtual void Heal(int Heal);			// 回復処理
		virtual void Update_Buff_Debuff();		// バフ、デバフの更新
		virtual void Add_Buff_Debuff(const std::shared_ptr<Character_Buff_Debuff_Base>& Buff_Debuff);							// バフ、デバフの追加
		virtual std::vector<std::shared_ptr<Character_Buff_Debuff_Base>> CheckGet_Buff_Debuff(std::string Buff_Debuff_Name);	// 対象の名称のバフ、デバフを取得
		virtual void Draw_Action_Effect();																						// 行動内容の描写
		virtual void Delete_Action_Effect(std::shared_ptr<Action_Effect_Base> ActionEffect);									// 該当の行動内容の削除
		virtual bool MouseInCharacter();																						// マウスカーソルがキャラクター上にあるか確認

		/* ゲッター */
		int 												GetHP_Max()				{ return iHP_Max; }				// 体力(最大値)の取得
		int 												GetHP_Now()				{ return iHP_Now; }				// 体力(現在値)の取得
		int 												GetShield_Now()			{ return iShield_Now; }			// シールド(現在値)の取得
		Struct_2D::POSITION									GetBasePos()			{ return BasePos; }				// 基準座標の取得
		int													GetCamp()				{ return Camp; }				// 陣営の取得
		std::shared_ptr<int>								GetImage()				{ return Image; }				// 画像取得
		int													GetSizeX()				{ return SizeX; }				// キャラクターの幅
		int													GetSizeY()				{ return SizeY; }				// キャラクターの高さ
		std::vector<std::shared_ptr<Action_Effect_Base>>	GetActionEffectList()	{ return ActionEffectList; }	// 行動内容一覧の取得
		bool												GetDeathDeleteFlg()		{ return Death_DeleteFlg; }		// 死亡により盤面上から削除するかのフラグ
		int													GetDropCoin()			{ return DropCoin; }			// ドロップするコインの枚数
		std::vector<std::shared_ptr<Card_Base>>				GetDropCardList()		{ return DropCardList; }		// ドロップするカード一覧の取得

		/* セッター */
		void	SetHp_Max(int MaxHP)														{ this->iHP_Max			= MaxHP; }		// 体力(最大値)の設定
		void	SetHp_Now(int NowHP)														{ this->iHP_Now			= NowHP; }		// 体力(現在地)の設定
		void	SetShield_Now(int NowShield)												{ this->iShield_Now		= NowShield; }	// シールド(現在値)の設定
		void	SetBasePos(Struct_2D::POSITION Pos)											{ this->BasePos			= Pos; }		// 基準座標の設定
		void	SetCamp(int camp)															{ this->Camp			= camp; }		// 陣営の設定
		void	SetDropCoin(int dropcoin)													{ this->DropCoin		= dropcoin; }	// ドロップするコインの枚数
		void	SetDropCardList(const std::vector<std::shared_ptr<Card_Base>>& cardList)	{ this->DropCardList	= cardList; }	// ドロップするカード一覧の設定

		/* 定数 */
		// 描写系
		static const int	HPBAR_WIDE				= 128;	// HPバーの幅
		static const int	HPBAR_HEIGHT			= 20;	// HPバーの高さ
		static const int	HPBAR_UPPER				= 30;	// HPバーの上端位置補正値
		static const int	HPBAR_FRAME_WIDE		= 2;	// HPバーのフレームの幅
		static const int	SHIELDBAR_HEIGHT		= 16;	// シールドバーの高さ
		static const int	SHIELDBAR_UPPER			= 17;	// シールドバーの上端位置補正値
		static const int	ACTION_WIDE				= 48;	// 行動予告の幅
		static const int	ACTION_HEIGHT			= 48;	// 行動内容の高さ
		static const int	ACTION_X				= -96;	// 行動予告のX座標補正値
		static const int	ACTION_Y				= -128;	// 行動予告のY座標補正値
		static const int	ACTION_INTERVAL			= 52;	// 行動内容の描写間隔
		static const int	FRAME_THICKNESS			= 16;	// 枠の太さ
		static const int	STAND_MOVESPEED_MAX		= 3;	// 待機時の平たくする量の変化速度(最大速度)
		// 陣営
		static const int	CAMP_NONE		= -1;	// 陣営無し
		static const int	CAMP_FRIEND		= 0;	// 仲間陣営
		static const int	CAMP_ENEMY		= 1;	// 敵陣営
		// その他
		static const int	STANDBY_FLATTEN_PERIOD	= 120;	// 待機時の平たくする量の変化周期(秒)
		static const int	STANDBY_FLATTEN_MAX		= 10;	// 待機時の平たくする量の最大値(%単位)

	protected:
		/* 変数 */
		// データリスト
		std::shared_ptr<DataList_Battle> pDataList_Battle;		// バトル用データリスト
		// キャラクター情報
		int						iHP_Max;						// 体力(最大値)
		int						iHP_Now;						// 体力(現在値)
		int						iShield_Now;					// シールド(現在値)
		int						Camp;							// 陣営
		bool					Death_DeleteFlg;				// 死亡により盤面上から削除するかのフラグ
		int						DropCoin;						// ドロップするコイン枚数
		// その他
		Struct_2D::POSITION		BasePos;						// 基準座標(足元)
		int						SizeX;							// キャラクターの幅
		int						SizeY;							// キャラクターの高さ
		Struct_2D::POSITION		CorrectionPos;					// 補正座標
		int						AddBuffReaction;				// バフ付与時のリアクション
		int						DamageReaction;					// 被ダメージ時のリアクション
		int						AttackReaction;					// 攻撃時のリアクション
		int						FlattenPercent;					// 平たくする量(%単位 / 100が標準)		
		int						StandbyFlatten_Count;			// 待機時の平たくする量のカウント
		int						StandbyFlatten_Percent;			// 待機児の平たくする量の実数値
		std::vector<std::shared_ptr<Card_Base>>	DropCardList;	// ドロップするカード一覧
		int						ActionEffect_Scale_Size;		// 行動内容のスケールサイズ
		int						ActionEffect_Scale_Count;		// 行動内容のスケールカウント

		// バフ、デバフ情報
		std::vector<std::shared_ptr<Character_Buff_Debuff_Base>> 	Buff_Debuff_List;	// バフ、デバフ一覧
		// 行動内容
		std::vector<std::shared_ptr<Action_Effect_Base>>			ActionEffectList;	// 所持している行動内容一覧
		// 画像
		std::shared_ptr<int>	Image;				// キャラクター画像
		std::shared_ptr<int>	Image_Frame_Corner;	// 角
		std::shared_ptr<int>	Image_Frame_Line;	// 線
		std::shared_ptr<int>	Image_Frame_Inside;	// 内側

		/* 関数 */
		void SetUpImage(std::string ImageName);		// 指定の名称の画像を設定する
		void Correction_Reaction();					// 各リアクションによる座標補正
		void ActionEffect_ScaleUpdate();			// 行動内容のスケール更新
};
