#ifndef SLHOLEDRILLINGCMD_H
#define SLHOLEDRILLINGCMD_H

#include <NXOpen/TaggedObject.hxx>
#include <NXOpen/Part.hxx>
#include <NXOpen/Body.hxx>
#include <NXOpen/Plane.hxx>
#include <NXOpen/PlaneCollection.hxx>

#include <uf_camgeom.h>
#include <uf_defs.h>
#include <uf_cam.h>
#include <uf_drpos.h>
#include <uf_modl.h>
#include <uf_hmop.h>
#include <uf_obj.h>
#include <uf_ude.h>
#include <uf_ui_ugopen.h>
#include <NXOpen/NXException.hxx>
#include <NXOpen/NXObjectManager.hxx>
#include <NXOpen/Session.hxx>
#include <NXOpen/Builder.hxx>
#include <NXOpen/CAM_AcrossVoids.hxx>
#include <NXOpen/CAM_ArcOutputTypeCiBuilder.hxx>
#include <NXOpen/CAM_CAMObject.hxx>
#include <NXOpen/CAM_CAMSetup.hxx>
#include <NXOpen/CAM_CleanupOutput.hxx>
#include <NXOpen/CAM_CleanupSettings.hxx>
#include <NXOpen/CAM_CornerControlBuilder.hxx>
#include <NXOpen/CAM_CutAngle.hxx>
#include <NXOpen/CAM_CutDirection.hxx>
#include <NXOpen/CAM_CutParameters.hxx>
#include <NXOpen/CAM_DisplayPaint.hxx>
#include <NXOpen/CAM_DisplayPath.hxx>
#include <NXOpen/CAM_DisplaySilhouette.hxx>
#include <NXOpen/CAM_DisplayTool.hxx>
#include <NXOpen/CAM_DrillSurfaceBuilder.hxx>
#include <NXOpen/CAM_DrillGeomBuilder.hxx>//
#include <NXOpen/CAM_ExtendAtEdges.hxx>
#include <NXOpen/CAM_FBM_Feature.hxx>
#include <NXOpen/CAM_FBM_FeatureGeometry.hxx>
#include <NXOpen/CAM_FBM_FeatureSet.hxx>
#include <NXOpen/CAM_FBM_ThreadFeatureGeometry.hxx>
#include <NXOpen/CAM_FeedsBuilder.hxx>
#include <NXOpen/CAM_FeedsOptimizationData.hxx>
#include <NXOpen/CAM_FinishPassesBuilder.hxx>
#include <NXOpen/CAM_FeatureGeometry.hxx>
#include <NXOpen/CAM_GeometryCiBuilder.hxx>
#include <NXOpen/CAM_Geometry.hxx>
#include <NXOpen/CAM_GeometrySet.hxx>
#include <NXOpen/CAM_GeometrySetList.hxx>
#include <NXOpen/CAM_HoleBossGeom.hxx>
#include <NXOpen/CAM_HoleMakingBuilder.hxx>
#include <NXOpen/CAM_HoleDrilling.hxx>//
#include <NXOpen/CAM_HoleDrillingBuilder.hxx>//
#include <NXOpen/CAM_Inheritable2dLength.hxx>
#include <NXOpen/CAM_InheritableDoubleBuilder.hxx>
#include <NXOpen/CAM_InheritableFeedBuilder.hxx>
#include <NXOpen/CAM_InheritableIntBuilder.hxx>
#include <NXOpen/CAM_InheritableTextBuilder.hxx>
#include <NXOpen/CAM_InheritableToolDepBuilder.hxx>
#include <NXOpen/CAM_LevelToLevel.hxx>
#include <NXOpen/CAM_LowerLimit.hxx>
#include <NXOpen/CAM_MaxCutTraverse.hxx>
#include <NXOpen/CAM_Method.hxx>
#include <NXOpen/CAM_MillCutParameters.hxx>
#include <NXOpen/CAM_MillOperationBuilder.hxx>
#include <NXOpen/CAM_MinCutVolume.hxx>
#include <NXOpen/CAM_MultiBladeBaseGeometry.hxx>
#include <NXOpen/CAM_MultiBladeSplittersGeometry.hxx>
#include <NXOpen/CAM_MultiDepthCut.hxx>
#include <NXOpen/CAM_MultipleStepoverBuilder.hxx>
#include <NXOpen/CAM_NCGroup.hxx>
#include <NXOpen/CAM_NCGroupCollection.hxx>
#include <NXOpen/CAM_NcmAvoidancePointBuilder.hxx>
#include <NXOpen/CAM_NcmClearanceBuilder.hxx>
#include <NXOpen/CAM_NcmHoleMachining.hxx>
#include <NXOpen/CAM_NcmHoleMachiningEngRet.hxx>
#include <NXOpen/CAM_Operation.hxx>
#include <NXOpen/CAM_OperationBuilder.hxx>
#include <NXOpen/CAM_OperationCollection.hxx>
#include <NXOpen/CAM_OperationDisplayOptionsBuilder.hxx>
//#include <NXOpen/CAM_OperationPathDisplayColorsBuilder.hxx>
#include <NXOpen/CAM_OrientGeometry.hxx>
#include <NXOpen/CAM_OutputUncutRegions.hxx>
#include <NXOpen/CAM_ParamBuilder.hxx>
#include <NXOpen/CAM_PathDisplayColors.hxx>
#include <NXOpen/CAM_PointToPointBuilder.hxx>
#include <NXOpen/CAM_DrillSurfaceBuilder.hxx>
#include <NXOpen/CAM_PostEventsCiBuilder.hxx>
#include <NXOpen/CAM_SmallAreaAvoidance.hxx>
#include <NXOpen/CAM_SplitterList.hxx>
#include <NXOpen/CAM_SteepContainment.hxx>
#include <NXOpen/CAM_StepoverBuilder.hxx>
#include <NXOpen/CAM_StockPerPassBuilder.hxx>
#include <NXOpen/CAM_Tool.hxx>
#include <NXOpen/CAM_ToolAxisChange.hxx>
#include <NXOpen/CAM_ToolAxisFixed.hxx>
#include <NXOpen/CAM_ToolAxisVariable.hxx>
#include <NXOpen/CAM_ToolAxisTilt.hxx>
#include <NXOpen/CAM_ToolChangeCiBuilder.hxx>
#include <NXOpen/CAM_TrochoidalSettings.hxx>
#include <NXOpen/CAM_ThreadMilling.hxx>
#include <NXOpen/CAM_ThreadMillingBuilder.hxx>
#include <NXOpen/CAM_ThreadMillingCutParameters.hxx>
#include <NXOpen/CAM_ThreadedBossSet.hxx>
#include <NXOpen/CAM_ThreadedBossSetList.hxx>
#include <NXOpen/CAM_ThreadedHoleSet.hxx>
#include <NXOpen/CAM_ThreadedHoleSetList.hxx>
#include <NXOpen/CAM_Ude.hxx>
#include <NXOpen/CAM_UdeList.hxx>
#include <NXOpen/CAM_UdeSet.hxx>
#include <NXOpen/CAM_UpDownCutting.hxx>
#include <NXOpen/CAM_ZAxisSmoothing.hxx>
#include <NXOpen/Direction.hxx>
#include <NXOpen/DirectionCollection.hxx>
#include <NXOpen/Expression.hxx>
#include <NXOpen/ExpressionCollection.hxx>
#include <NXOpen/Face.hxx>
#include <NXOpen/Features_FeatureCollection.hxx>
#include <NXOpen/Features_Revolve.hxx>
#include <NXOpen/IPlane.hxx>
#include <NXOpen/Line.hxx>
#include <NXOpen/LineCollection.hxx>
#include <NXOpen/NXObject.hxx>
#include <NXOpen/Part.hxx>
#include <NXOpen/PartCollection.hxx>
#include <NXOpen/Plane.hxx>
#include <NXOpen/PlaneCollection.hxx>
#include <NXOpen/Point.hxx>
#include <NXOpen/Section.hxx>
#include <NXOpen/SectionCollection.hxx>
#include <NXOpen/Session.hxx>
#include <NXOpen/Sketch.hxx>
#include <NXOpen/SketchCollection.hxx>
#include <NXOpen/Unit.hxx>
#include <NXOpen/UnitCollection.hxx>
#include <NXOpen/Update.hxx>
#include <NXOpen/View.hxx>
#include <NXOpen/Line.hxx>
#include <NXOpen/CurveCollection.hxx>
#include <NXOpen/Edge.hxx>
#include <NXOpen/Arc.hxx>
#include <vector>
#include <math.h>
#include <NXOpen/CAM_DrillGeomBuilder.hxx>
#include <NXOpen/CAM_NCGroupCollection.hxx>
#include <NXOpen/CAM_OperationBuilder.hxx>
#include <NXOpen/CAM_HoleDrillingBuilder.hxx>
#include <NXOpen/CAM_DrillMethodBuilder.hxx>
#include <NXOpen/CAM_CylinderMilling.hxx>
#include <NXOpen/CAM_CylinderMillingBuilder.hxx>
#include <NXOpen/CAM_CylinderMillingCutParameters.hxx>
#include <NXOpen/CAM_CylindricalMillingBuilder.hxx>
#include <NXOpen/CAM_CylindricalMillingCutParameters.hxx>
#include <windows.h>
#include "uf_ui_ont.h"
#include <uf_modl.h>

#include <iostream>




#include <NXOpen/Scalar.hxx>
#include <NXOpen/ScalarCollection.hxx>

#include "PublicFunctions.h"
#include "FeaturePublicClass.h"
#include "ProcessPublicClass.h"

using namespace NXOpen;

class SLHoleDrillingcmd : public TaggedObject
{
public: SLHoleDrillingcmd();

public:  SLHoleDrillingcmd(AllFeatureHole allFeatureHolelist, HoleProcessParameter ParameterSet, Part* WorkPart, Body* WorkBody, std::vector<NXOpen::Face*> SelectedHoleInWallFaceList);
		 virtual ~SLHoleDrillingcmd();

public:
	AllFeatureHole allFeatureHole;
	std::vector<DirectionNormalHole*> HoleDNList;
	std::vector<DirectionRightHole*> HoleDRList;
	std::vector<WallNormalHole*> HoleWNList;
	std::vector<BossNormalHole*> HoleBNList;
	std::vector<BossOneRightHole*> HoleBORList;
	std::vector<BossOneCircleHole*> HoleBOCList;
	std::vector<BossDoubleRightHole*> HoleBDRList;
	std::vector<BossDoubleCircleHole*> HoleBDCList;
	std::vector<BossOneRightOneCircleHole*> HoleBOROCRList;
	std::vector<Other3Hole*> HoleBOList;
	
	//多层孔
	std::vector<BossOutsideWideCombineHole*> HoleBOWCList;
	std::vector<BossManyLayerHole*> HoleBMList;//1+2
	std::vector<BossMidCricleCombineHole*> HoleBMCCList;//1+2	
	std::vector<WallCircleCombineHole*> HoleWCCList;//2
	std::vector<RibCombineHole*> HoleRCList;//1+2
	std::vector<BossInsideWideCombineHole*> HoleBIWCList;
	
	
	//盲孔
	std::vector<DirectionBlindHole*> HoleDBList;
	std::vector<BossNormalBlindHole*> HoleBNBList;
	std::vector<BossRightBlindHole*> HoleBRBList;
	
	

	HoleProcessParameter HoleParameterSet;
	Part* workPart;
	Body* selectedBody;
	std::vector<Face*> selectedFaceList;//所选孔壁面
	std::vector<Face*> selectedFaceOtherList;//所选孔壁面的相邻面
	std::vector<Face*> selectedTopFaceList;//所选孔的顶面
	//std::vector<DirectionNormalHole*> SelectedHole;
	std::vector<char*> selectedHoleNameList;//所选孔的名称
	//多层
	std::vector<Face*> selectedFaceList1;//所选孔壁面
	std::vector<Face*> selectedFaceOtherList1;//所选孔壁面的相邻面
	std::vector<Face*> selectedTopFaceList1;//所选孔的顶面
	//std::vector<DirectionNormalHole*> SelectedHole;
	std::vector<char*> selectedHoleNameList1;//所选孔的名称

	std::vector<Face*> TopFaceList;//所选孔的顶面
	std::vector<char*> HoleNameList;//所选孔的名称
	std::vector<int> IfBlindList;//是否是盲孔


	std::vector<int> Index;

	struct Coordinate3D          //三维坐标
	{ 
		double x;
		double y;
		double z;
	};

	struct DrGeo         //驱动几何信息
	{
		Face* selectedHoleTop;
		Face* selectedHoleInWall;
		Face* selectedHoleOther;
		char* selectedHoleName;
	};
	std::vector<DrGeo> AllDrGeo1;//驱动几何列表1
	std::vector<DrGeo> AllDrGeo2;//驱动几何列表2，第二层
	//第一层
	std::vector<DrGeo> AxisDrGeo;//轴向孔
	std::vector<DrGeo> BossDrGeo;//凸台孔
	std::vector<DrGeo> WallRibDrGeo;//筋面和壁面孔
	//第二层
	std::vector<DrGeo> AxisDrGeo2;//轴向孔
	std::vector<DrGeo> BossDrGeo2;//凸台孔
	std::vector<DrGeo> WallRibDrGeo2;//筋面和壁面孔

	//void DrillDecision();
	double DotProduct(double a[3], double b[3]);
	Point3d get_Centre_fr_Face(Face* ipFace);
	tag_t SelectBlank();//毛坯单选对话框，每次选取一个对象，对象类型可设置;
	tag_t SelectPart();
	Point3d GetCenterPoint(Face *face);


	//void SLHoleDrillingcmd::CreateMCS(Face* MCSTopFace, char *Eachname, double wcsZ[3], double origin[3]);
	void SLHoleDrillingcmd::CreateMCS(tag_t TopFaceTag, double Facenormal[3], char *Eachname, double wcsZ[3], double origin[3]);
	double SLHoleDrillingcmd::Get_result_determinant(double D[9]);
	Point3d SLHoleDrillingcmd::Get_opposite_coordinate(Point3d centeripoint, double wcs_origin[3], double wcsMatrixValues[9]);//获取相对坐标
	Point3d SLHoleDrillingcmd::Get_absolute_coordinate(double opposite_point[3], double wcs_origin[3], double wcsMatrixValues[9]);//获取绝对坐标

	void SLHoleDrillingcmd::HoleSpotDrillingDecision(std::vector<Face*> selectedFaceList, std::vector<Face*> selectedFaceOtherList, std::vector<Face*> selectedTopFaceList, std::vector<char*> selectedHoleNameList);

	void SLHoleDrillingcmd::OperatorDecision(std::vector<DrGeo> axisDrGeo, std::vector<DrGeo> bossDrGeo, std::vector<DrGeo> wallRibDrGeo, int OperatorType);
	void SLHoleDrillingcmd::SpotDrillingDecisioncmd(char* mcsname, std::vector<Face*> InWallVectorList);
	void SLHoleDrillingcmd::DrillingDecisioncmd(char* mcsname, std::vector<Face*> InWallVectorList);
	void SLHoleDrillingcmd::SpotFacingDecisioncmd(char* mcsname, std::vector<Face*> InWallVectorList);
	void SLHoleDrillingcmd::ReamingDecisioncmd(char* mcsname, std::vector<Face*> InWallVectorList);
	void SLHoleDrillingcmd::BoringDecisioncmd(char* mcsname, std::vector<Face*> InWallVectorList);

	void SLHoleDrillingcmd::HoleMillingDecisioncmd(char* mcsname, std::vector<Face*> InWallVectorList);
	void SLHoleDrillingcmd::ThreadMillingDecisioncmd(char* mcsname, std::vector<Face*> InWallVectorList);
	void SLHoleDrillingcmd::holeClassification(std::vector<DrGeo> AllDrGeo);
	void SLHoleDrillingcmd::holeClassification2(std::vector<DrGeo> AllDrGeo);//针对第二层孔



};

#endif