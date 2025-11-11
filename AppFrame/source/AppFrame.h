/* AppFrameの基本参照ファイル */
// ※ソリューションでこのヘッダファイルで定義されたシステムを使用する場合、このファイルをincludeする

#pragma once

#pragma comment(lib,"AppFrame.lib")

/* ライブラリ */
#include <DxLib.h>
#include <nlohmann/json.hpp>

/* メインループ関連 */
#include "AppMain.h"

/* 定義 */
#include "AppConstantDefine.h"
#include "AppFunctionDefine.h"
#include "AppStructDefine.h"
#include "AppVariableDefine.h"

/* シーン系 */
#include "Scene_Base.h"
#include "SceneServer.h"

/* データリスト系 */
#include "DataList_Base.h"
#include "DataListServer.h"
