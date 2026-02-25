/* カードのインクルードまとめ */
// このクラスをインクルードすることで、全てのカードクラスを使用可能にする

#pragma once

/* 親クラス */
#include "Card_Base.h"

/* 武具カード */
// 剣のみ(刀)
#include "Card_Arms_AsuraKatana.h"
#include "Card_Arms_CursedKatana.h"
#include "Card_Arms_Katana.h"
// 木製 
#include "Card_Arms_WoodenShield.h"
#include "Card_Arms_WoodenSword.h"
#include "Card_Arms_WoodenBow.h"
// 旅人
#include "Card_Arms_TravelerSword.h"
#include "Card_Arms_TravelerShield.h"
#include "Card_Arms_TravelerBow.h"
// 王国
#include "Card_Arms_RoyalSword.h"
#include "Card_Arms_RoyalShield.h"
#include "Card_Arms_RoyalBow.h"
// 毒
#include "Card_Arms_PoisonDagger.h"
// 死者
#include "Card_Arms_ExecutorSword.h"
#include "Card_Arms_BoneSword.h"
#include "Card_Arms_BoneDagger.h"
#include "Card_Arms_BoneBow.h"
// ガラス
#include "Card_Arms_GrassSword.h"
#include "Card_Arms_GrassDagger.h"
// エレメント
#include "Card_Arms_StormBow.h"
// 金
#include "Card_Arms_GoldDagger.h"
#include "Card_Arms_GoldSword.h"

/* 道具カード */
// 薬
#include "Card_Item_PoisonPotion.h"
#include "Card_Item_HealingPotion.h"
#include "Card_Item_PowerPotion.h"
#include "Card_Item_GuardianPotion.h"
// 旅人
#include "Card_Item_TravelerCompass.h"
// 王国
#include "Card_Item_RoyalFlag.h"

// 魔法カード
// 武具系
#include "Card_Spell_ArmsEnhancement.h"
#include "Card_Spell_ShieldBash.h"
#include "Card_Spell_BlessingSword.h"
// エレメント系
#include "Card_Spell_FireMagic.h"
#include "Card_Spell_WaterMagic.h"
#include "Card_Spell_EarthMagic.h"
#include "Card_Spell_WindMagic.h"

