/* シーン"パーティクル(文字)"の定義 */

/* 使用する要素のインクルード */
// ヘッダファイル
#include "Scene_Particles_Text.h"

// コンストラクタ
Scene_Particles_Text::Scene_Particles_Text() : Scene_Base("Particles_Text", 10, false, false)
{
	/* 初期化 */
	this->Text			= "";					// 表示テキスト
	this->Pos			= { 0, 0 };				// 位置座標
	this->iFontHandle	= -1;					// フォントハンドル
	this->Move			= { 0, -1 };			// 移動量
	this->iAlpha		= 200;					// 透明度
	this->Color			= GetColor(0, 0, 0);	// 色
}

// 更新
void Scene_Particles_Text::Update()
{
	/* 座標を移動させる */
	this->Pos.iX += this->Move.iX;
	this->Pos.iY += this->Move.iY;

	/* アルファ値を上昇させる */
	if (this->iAlpha < 255)
	{
		this->iAlpha += 10;
	}

	/* アルファ値が最大になったら削除フラグを立てる */
	if (this->iAlpha >= 255)
	{
		this->iAlpha = 255;
		this->bDeleteFlg = true;
	}
}

// 描画
void Scene_Particles_Text::Draw()
{
	/* アルファブレンド設定（文字の透明度）*/
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, this->iAlpha);

	/* 文字列の高さ、幅を取得 */
	int iSizeX = GetDrawStringWidthToHandle(this->Text.c_str(), static_cast<int>(strlenDx(this->Text.c_str())), iFontHandle);
	int iSizeY = GetFontSizeToHandle(iFontHandle);

	/* 文字列を描写 */
	DrawStringToHandle(
		this->Pos.iX - (iSizeX / 2), this->Pos.iY - (iSizeY / 2),
		this->Text.c_str(),
		this->Color,
		this->iFontHandle);

	/* ブレンドモードを通常に戻す */
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}
