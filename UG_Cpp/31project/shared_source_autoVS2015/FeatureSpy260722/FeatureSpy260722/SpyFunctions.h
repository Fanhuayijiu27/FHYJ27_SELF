#ifndef NXOpen_SpyFunctions_H_INCLUDED
#define NXOpen_SpyFunctions_H_INCLUDED

//#include "afxdlgs.h"
//#include <uf_evalsf.h>
//#include <uf_facet.h>
//#include <uf.h>
//#include "afx.h"
//#include "afxwin.h"
//#undef CreateDialog

//////////

//-----

#include <uf_defs.h>
#include <uf_ui_types.h>
#include <iostream>
#include <NXOpen/Session.hxx>
#include <NXOpen/UI.hxx>
#include <NXOpen/NXMessageBox.hxx>
#include <NXOpen/Callback.hxx>
#include <NXOpen/NXException.hxx>
#include <NXOpen/BlockStyler_UIBlock.hxx>
#include <NXOpen/BlockStyler_BlockDialog.hxx>
#include <NXOpen/BlockStyler_PropertyList.hxx>
#include <NXOpen/BlockStyler_ListBox.hxx>
#include <math.h>
#include "HuNXOpen.h"
#include "uf_all.h"
#include "PublicFunctions.h"
#include "FeaturePublicClass.h"

#include "xml.h"



using namespace NXOpen;
//-----
//凸台特征识别与种子面创建---------------------------------------------------------------------------------
DllExport void BossRecognitionAndCreate(std::vector<Face*>AllFaces,std::vector<Face*>SealFace_NormalBoss,std::vector<NormalBoss*>&Feature_NormalBoss,
										std::vector<Face*>SealFace_NoSideBoss,std::vector<NoSideBoss*>&Feature_NoSideBoss,
										std::vector<Face*>SealFace_DoubleLayerBoss, std::vector<DoubleLayerBoss*>&Feature_DoubleLayerBoss, NXOpen::Vector3d _Approach_Normal, NXOpen::Part* workPart);

DllExport void DistinguishBossOutOrIn(std::vector<NormalBoss*>&Feature_NormalBoss,std::vector<NoSideBoss*>&Feature_NoSideBoss,std::vector<DoubleLayerBoss*>&Feature_DoubleLayerBoss,double _Approach_Point[3]);

//凸台特征识别与种子面创建---------------------------------------------------------------------------------

//轴向孔识别
DllExport void DirectionMillRecognition(std::vector<Face*>AllFaces,Face *BeginFace,Face* BottomPlaneFace,Face* TopPlaneFace,
										std::vector<Face*> ParallelPlaneFace, std::vector<Face*>&LeftSealHole_FaceVector
										,std::vector<Face*>&SealFace_DirectionNormalHole,std::vector<Face*>&SealFace_DirectionRightHole
										,std::vector<Face*>&SealFace_DirectionBlindHole);
//构建轴向孔特征
DllExport void CreateDirectionMillFeature(Face* BottomPlaneFace,Face* TopPlaneFace,
										  std::vector<Face*> ParallelPlaneFace,std::vector<Face*>SealFace_DirectionNormalHole,std::vector<Face*>SealFace_DirectionRightHole,
										  std::vector<Face*>SealFace_DirectionBlindHole,
										  std::vector<DirectionNormalHole*>&Feature_DirectionNormalHole,//普通通孔
										  std::vector<DirectionRightHole*>&Feature_DirectionRightHole,//倒斜角通孔
										  std::vector<DirectionBlindHole*>&Feature_DirectionBlindHole);//盲孔

//31新模型轴向孔种子面识别
DllExport void DirectionMillRecognition_31(
    const std::vector<NXOpen::Face*>& AllFaces,
    NXOpen::Vector3d _Approach_Normal,
    std::vector<NXOpen::Face*>& LeftSealHole_FaceVector,
    std::vector<NXOpen::Face*>& SealFace_DirectionStepThroughHole,
    std::vector<NXOpen::Face*>& SealFace_DirectionLongRoundThroughHole,
    std::vector<NXOpen::Face*>& SealFace_DirectionBlindHole
);

//31新模型轴向孔特征创建
DllExport void CreateDirectionMillFeature_31(
    NXOpen::Vector3d _Approach_Normal,
    const std::vector<NXOpen::Face*>& SealFace_DirectionStepThroughHole,
    const std::vector<NXOpen::Face*>& SealFace_DirectionLongRoundThroughHole,
    const std::vector<NXOpen::Face*>& SealFace_DirectionBlindHole,
    std::vector<DirectionStepThroughHole_31*>& Feature_DirectionStepThroughHole,
    std::vector<DirectionLongRoundThroughHole_31*>& Feature_DirectionLongRoundThroughHole,
    std::vector<DirectionBlindHole_31*>& Feature_DirectionBlindHole
);

//识别壁面和筋面的孔
DllExport void WallAndRibMillRecognition(std::vector<Face*>AllFaces,std::vector<Face*>&LeftSealHole_FaceVector
										 ,std::vector<Face*>&SealFace_WallNormalHole,std::vector<Face*>&SealFace_WallCircleCombineHole
										 , std::vector<Face*>&SealFace_RibCombineHole);

//构建识别壁面和筋面的孔特征
DllExport void CreateWallAndRibMillFeature(std::vector<NXOpen::Face*>SealFace_WallNormalHole,std::vector<NXOpen::Face*>SealFace_WallCircleCombineHole,
										   std::vector<NXOpen::Face*>SealFace_RibCombineHole,
										   std::vector<WallNormalHole*>&Feature_WallNormalHole,
										   std::vector<WallCircleCombineHole *>&Feature_WallCircleCombineHole,
										   std::vector<RibCombineHole*>&Feature_RibCombineHole);

//凸台非复合孔的识别
DllExport void BossHoleRecognition1(std::vector<Face*>AllFaces,std::vector<Face*>&LeftSealHole_FaceVector,
									std::vector<NXOpen::Face*>&SealFace_BossNormalHole,//凸台通孔
									std::vector<NXOpen::Face*>&SealFace_BossOneRightHole,//凸台单侧直角通孔
									std::vector<NXOpen::Face*>&SealFace_BossOneCircleHole,//凸台单侧圆角通孔
									std::vector<NXOpen::Face*>&SealFace_BossDoubleRightHole,//凸台双侧直角通孔
									std::vector<NXOpen::Face*>&SealFace_BossDoubleCircleHole,//凸台双侧圆角通孔
									std::vector<NXOpen::Face*>&SealFace_BossOneRightOneCircleHole,//凸台单侧直角单侧圆角通孔
									std::vector<NXOpen::Face*>&SealFace_BossNormalBlindHole,//凸台无倒角盲孔);
									std::vector<NXOpen::Face*>&SealFace_BossRightBlindHole);//凸台倒斜角盲孔)

//其它孔识别
DllExport void OtherHoleRecognition1(std::vector<Face*>AllFaces, std::vector<NXOpen::Face*>&SealFace_otherHole1,
									 std::vector<NXOpen::Face*>&SealFace_otherHole2,
									 std::vector<NXOpen::Face*>&SealFace_otherHole3);//其它孔)

//其它孔识别
DllExport void CreatOtherHoleFeature(std::vector<NXOpen::Face*>&SealFace_otherHole1,
									 std::vector<NXOpen::Face*>&SealFace_otherHole2,
									 std::vector<NXOpen::Face*>&SealFace_otherHole3,
									 std::vector<Other1Hole*>&Feature_otherHole1,
									 std::vector<Other2Hole*>&Feature_otherHole2,
									 std::vector<Other3Hole*>&Feature_otherHole3);//其它孔)




//凸台复合孔识别
DllExport void BossCombineHoleRecognition(std::vector<Face*>AllFaces, std::vector<Face*>&LeftSealHole_FaceVector4,
										  std::vector<NXOpen::Face*>&SealFace_BossManyLayerHole,
										  std::vector<NXOpen::Face*>&SealFace_BossOutsideWideCombineHole,
										  std::vector<NXOpen::Face*>&SealFace_BossInsideWideCombineHole,
										  std::vector<NXOpen::Face*>&SealFace_BossMidCricleCombineHole);

//构建凸台孔特征
DllExport void CreateBossHoleFeature(std::vector<NXOpen::Face*>SealFace_BossNormalHole,//凸台通孔
									 std::vector<NXOpen::Face*>SealFace_BossOneRightHole,//凸台单侧直角通孔
									 std::vector<NXOpen::Face*>SealFace_BossOneCircleHole,//凸台单侧圆角通孔
									 std::vector<NXOpen::Face*>SealFace_BossDoubleRightHole,//凸台双侧直角通孔
									 std::vector<NXOpen::Face*>SealFace_BossDoubleCircleHole,//凸台双侧圆角通孔
									 std::vector<NXOpen::Face*>SealFace_BossOneRightOneCircleHole,//凸台单侧直角单侧圆角通孔
									 std::vector<NXOpen::Face*>SealFace_BossNormalBlindHole,//凸台无倒角盲孔);
									 std::vector<NXOpen::Face*>SealFace_BossRightBlindHole,//凸台倒斜角盲孔))
									 std::vector<NXOpen::Face*>SealFace_BossManyLayerHole,
									 std::vector<NXOpen::Face*>SealFace_BossOutsideWideCombineHole,
									 std::vector<NXOpen::Face*>SealFace_BossInsideWideCombineHole,
									 std::vector<NXOpen::Face*>SealFace_BossMidCricleCombineHole,
									 std::vector<BossNormalHole*>&Feature_BossNormalHole,
									 std::vector<BossOneRightHole*>&Feature_BossOneRightHole,
									 std::vector<BossOneCircleHole*>&Feature_BossOneCircleHole,
									 std::vector<BossDoubleRightHole*>&Feature_BossDoubleRightHole,
									 std::vector<BossDoubleCircleHole*>&Feature_BossDoubleCircleHole,
									 std::vector<BossOneRightOneCircleHole*>&Feature_BossOneRightOneCircleHole,
									 std::vector<BossManyLayerHole*>&Feature_BossManyLayerHole,
									 std::vector<BossOutsideWideCombineHole*>&Feature_BossOutsideWideCombineHole,
									 std::vector<BossInsideWideCombineHole*>&Feature_BossInsideWideCombineHole,
									 std::vector<BossMidCricleCombineHole*>&Feature_BossMidCricleCombineHole,
									 std::vector<BossNormalBlindHole*>&Feature_BossNormalBlindHole,
									 std::vector<BossRightBlindHole*>&Feature_BossRightBlindHole);

//环形槽种子面
DllExport void CircularGrooveRecognition(std::vector<NXOpen::Face*>AllFaces,Face *Begin_Face,NXOpen::Part*workPart,std::vector<NXOpen::Face*>&SealFace_CircularGroove0,std::vector<NXOpen::Face*>&SealFace_CircularGroove1,Face *_ApproachFace);
DllExport void CreateCircularGrooveFeature(Face *Begin_Face,std::vector<NXOpen::Face*>SealFace_CircularGroove0,std::vector<NXOpen::Face*>SealFace_CircularGroove1,std::vector<CircularGroove*>&Feature_CircularGroove0,std::vector<CircularGroove*>&Feature_CircularGroove1);
//下陷槽识别
DllExport void SubsidenceGrooveRecognition(std::vector<NXOpen::Face*>AllFaces,Face* BottomPlaneFace,Face* TopPlaneFace,
										   std::vector<Face*> ParallelPlaneFace,std::vector<NXOpen::Face*>&SealFace_SubsidenceGroove,std::vector<NXOpen::Face*>&SealFace_HookFaceRib);
DllExport void CreateSubsidenceGrooveFeature(std::vector<NXOpen::Face*>SealFace_SubsidenceGroove,std::vector<SubsidenceGroove*>&Feature_SubsidenceGroove,Face *_ApproachFace);
//平面浅槽种子面
DllExport void PlanerBlindGrooveRecognition(std::vector<NXOpen::Face*>AllFaces,Face* BottomPlaneFace,Face* TopPlaneFace,
											std::vector<Face*> ParallelPlaneFace,std::vector<NXOpen::Face*>&SealFace_PlanerBlindGroove);
DllExport void CreatePlanerBlindGrooveFeature(std::vector<NXOpen::Face*>SealFace_PlanerBlindGroove,std::vector<PlanerBlindGroove*>&Feature_PlanerBlindGroove);
//通槽种子面
DllExport void PassGrooveRecognition(std::vector<NXOpen::Face*>AllFaces,std::vector<NXOpen::Face*>&SealFace_PassGroove);
DllExport void CreatePassGrooveFeature(std::vector<NXOpen::Face*>SealFace_PassGroove,std::vector<PassGroove*>&Feature_PassGroove);

//轴向回转筋
DllExport void DirectionRibRecognition(std::vector<Face*>AllFaces,std::vector<Face*>&SealFace_DirectionRib);

//构建主轴回转规则筋
DllExport void CreateDirectionRibFeature(std::vector<Face*>SealFace_DirectionRib,std::vector<DirectionRib*>&Feature_DirectionRib);

//曲面筋特征识别 
DllExport void  CreateHookFaceRibFeature(std::vector<Face*>SealFace_HookFaceRib,std::vector<HookFaceRib*>&Feature_HookFaceRib,std::vector<DirectionRib*>&Feature_DirectionRib,Vector3d NormalVector);

//--------------------------------得到含侧面凸台底面------------------------------------------//
DllExport void GetBossBottomFace(std::vector<Face*> TopFaceVector, std::vector<Face*> SideFaceVector, std::vector<Face*> BottomCornerFaceVector, std::vector<Face*>& BottomFaceVector);

//------

//识别轮廓特征----------------------------------mql
DllExport void OutlineRrecognition(std::vector<Face*>AllFaces,Face *Begin_Face,Face* BottomPlaneFace,Face* TopPlaneFace,
								   std::vector<Face*>&SeelFaces_OutlineVector,std::vector<Face*>&SeelFaces_OutlineVector1);
DllExport void CreateOutlineFeature(Face *Begin_Face,std::vector<Face*>SeelFaces_OutlineVector,std::vector<Face*>&SeelFaces_OutlineVector1,
									std::vector<Outline*>&Feature_OutlineVector,std::vector<Outline*>&Feature_OutlineVector1);
//识别轮廓特征---------------------------------mql
#endif