/* シーン"タイトル画面"の宣言 */

#pragma once

/* 使用する要素のインクルード */
// 共通定義
#include "AppFrame.h"

/* 前方宣言 */
class Scene_UI_Button;

// シーン"タイトル画面"
class Scene_Title : public Scene_Base
{
	public:
		Scene_Title(bool AnimFlg);	// コンストラクタ
		virtual ~Scene_Title();		// デストラクタ

		/* 関数 */
		virtual void	Update()			override;	// 更新
		virtual void	Draw()				override;	// 描画

		/* 定数 */
		// フェーズ
		static const int PHASE_TEAMLOGO		= 0;	// チームロゴ描写
		static const int PHASE_BLACKOUT		= 1;	// ブラックアウト
		static const int PHASE_LOGO_WHITE	= 2;	// タイトルロゴ(白)表示
		static const int PHASE_COMPASS_FALL = 3;	// コンパス落下
		static const int PHASE_LOGO_COLOR	= 4;	// タイトルロゴ(カラー)表示
		static const int PHASE_ADD_BUTTON	= 5;	// ボタン表示
		static const int PHASE_MAX			= 6;	// フェーズの最大数

	private:
		/* 関数 */
		void AdvanceImageLoad();		// 画像リソースの先行ロード
		void AdvanceFontLoad();			// フォントリソースの先行ロード
		void AddButton();				// UI(ボタン)の作成

		/* 変数 */
		// 画像
		std::shared_ptr<int> Image_TitleLogo[4];	// タイトルロゴ[0:文字, 1:コンパス, 2:魔法陣, 3:文字(白)]
		std::shared_ptr<int> Image_BackGround[2];	// タイトル背景[0:空, 1:建物]
		std::shared_ptr<int> Image_TeamLogo;		// チームロゴ
		// UIのハンドル
		std::shared_ptr<Scene_UI_Button> UI_Button[3];	// ボタンUI[0:はじめから, 1:ビルド設定, 2:終了]
		// 描写用変数
		double	RotationAngle_MagicalCircle;	// 魔法陣の回転角度		
		int		LogoAlpha_White;				// タイトルロゴ(白)のアルファ値
		int		LogoAlpha;						// タイトルロゴのアルファ値
		int		CompassPosY;					// コンパスの描写座標
		float	MagicalCircleScale;				// 魔法陣の拡大率
		int		BuildingPosY;					// 建物の描写座標Y
		int		SkyAlpha;						// 空のアルファ値
		// その他
		int		iPhase;							// フェーズ
		int		Counter_Phase;					// フェーズ用カウンター
};
