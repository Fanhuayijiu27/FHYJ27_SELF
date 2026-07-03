#include "AnnulusCavityBottomMillingCmd.h"
#include <uf.h>
#include <uf_ui_ont.h>
#include <uf_part.h>
#include <uf_obj.h>
#include <uf_modl.h>
#include <stdio.h>
#include <uf_ui.h>
#include <uf_ui_ugopen.h>
#include <uf_ncgroup.h>
#include <uf_setup.h>
#include <uf_param.h>
#include <uf_param_indices.h>
#include <uf_csys.h>
#include <uf_evalsf.h>
#include <uf_disp.h>
#include <NXOpen/Face.hxx>
#include <NXOpen/Body.hxx>
#include <uf_mtx.h>
#include <NXOpen/CAM_Geometry.hxx>
#include <NXOpen/CAM_GeometryCiBuilder.hxx>
#include <NXOpen/CAM_GeometrySet.hxx>
#include <NXOpen/CAM_GeometrySetList.hxx>
#include <NXOpen/Part.hxx>
#include <NXOpen/PartCollection.hxx>
#include <NXOpen/PartLoadStatus.hxx>
#include <NXOpen/Plane.hxx>
#include <NXOpen/Point.hxx>

#include <uf_defs.h>
#include <NXOpen/NXException.hxx>
#include <NXOpen/Session.hxx>
#include <NXOpen/Axis.hxx>
#include <NXOpen/BasePart.hxx>
#include <NXOpen/Body.hxx>
#include <NXOpen/BodyCollection.hxx>
#include <NXOpen/BodyDumbRule.hxx>
#include <NXOpen/Builder.hxx>
#include <NXOpen/CAM_AcrossVoids.hxx>
#include <NXOpen/CAM_ArcOutputTypeCiBuilder.hxx>
#include <NXOpen/CAM_AreaMillingContainment.hxx>
#include <NXOpen/CAM_AreaMillingNonSteepContainment.hxx>
#include <NXOpen/CAM_AreaMillingSteepContainment.hxx>
#include <NXOpen/CAM_BladeCutLevels.hxx>
#include <NXOpen/CAM_BladeFinish.hxx>
#include <NXOpen/CAM_BladeRough.hxx>
#include <NXOpen/CAM_BlendFinish.hxx>
#include <NXOpen/CAM_Boundary.hxx>
#include <NXOpen/CAM_BoundarySet.hxx>
#include <NXOpen/CAM_BoundarySetList.hxx>
#include <NXOpen/CAM_CAMSession.hxx>
#include <NXOpen/CAM_CAMSetup.hxx>
#include <NXOpen/CAM_CleanupOutput.hxx>
#include <NXOpen/CAM_CleanupSettings.hxx>
#include <NXOpen/CAM_CornerControlBuilder.hxx>
#include <NXOpen/CAM_CustomLoopSet.hxx>
#include <NXOpen/CAM_CustomLoopSetList.hxx>
#include <NXOpen/CAM_CustomLoops.hxx>
#include <NXOpen/CAM_CutAngle.hxx>
#include <NXOpen/CAM_CutDirection.hxx>
#include <NXOpen/CAM_CutParameters.hxx>
#include <NXOpen/CAM_CutPatternBuilder.hxx>
#include <NXOpen/CAM_CutVolumeGeom.hxx>
#include <NXOpen/CAM_Cycle.hxx>
#include <NXOpen/CAM_DisplayPaint.hxx>
#include <NXOpen/CAM_DisplayPath.hxx>
#include <NXOpen/CAM_DisplaySilhouette.hxx>
#include <NXOpen/CAM_DisplayTool.hxx>
#include <NXOpen/CAM_DmAmBuilder.hxx>
#include <NXOpen/CAM_DmBndBuilder.hxx>
#include <NXOpen/CAM_DmCmBuilder.hxx>
#include <NXOpen/CAM_DmCmMultiplePassesCiBuilder.hxx>
#include <NXOpen/CAM_DmCurveBuilder.hxx>
#include <NXOpen/CAM_DmRadBuilder.hxx>
#include <NXOpen/CAM_DmRotaryFloorFinishBuilder.hxx>
#include <NXOpen/CAM_DmSpiralBuilder.hxx>
#include <NXOpen/CAM_DmStreamlineCutStep.hxx>
#include <NXOpen/CAM_DmSurfBuilder.hxx>
#include <NXOpen/CAM_DmToolPosition.hxx>
#include <NXOpen/CAM_DmTpBuilder.hxx>
#include <NXOpen/CAM_DmTrimExtend.hxx>
#include <NXOpen/CAM_DmUserExitCiBuilder.hxx>
#include <NXOpen/CAM_DriveChainItemBuilder.hxx>
#include <NXOpen/CAM_DriveChainItemBuilderList.hxx>
#include <NXOpen/CAM_EdgeFinish.hxx>
#include <NXOpen/CAM_ExtendAtEdges.hxx>
#include <NXOpen/CAM_FeedsBuilder.hxx>
#include <NXOpen/CAM_FeedsOptimizationData.hxx>
#include <NXOpen/CAM_FinishPassesBuilder.hxx>
#include <NXOpen/CAM_FlowBuilder.hxx>
#include <NXOpen/CAM_FlowCutSteepContainment.hxx>
#include <NXOpen/CAM_Geometry.hxx>
#include <NXOpen/CAM_GeometryCiBuilder.hxx>
#include <NXOpen/CAM_GeometrySet.hxx>
#include <NXOpen/CAM_GeometrySetList.hxx>
#include <NXOpen/CAM_HoleBossGeom.hxx>
#include <NXOpen/CAM_HubFinish.hxx>
#include <NXOpen/CAM_Inheritable2dLength.hxx>
#include <NXOpen/CAM_InheritableDoubleBuilder.hxx>
#include <NXOpen/CAM_InheritableFeedBuilder.hxx>
#include <NXOpen/CAM_InheritableFeedModeBuilder.hxx>
#include <NXOpen/CAM_InheritableIntBuilder.hxx>
#include <NXOpen/CAM_InheritableTextBuilder.hxx>
#include <NXOpen/CAM_InheritableToolDepBuilder.hxx>
#include <NXOpen/CAM_InterpolateAngle.hxx>
#include <NXOpen/CAM_InterpolateVector.hxx>
#include <NXOpen/CAM_LayoutCiBuilder.hxx>
#include <NXOpen/CAM_LevelToLevel.hxx>
#include <NXOpen/CAM_LowerLimit.hxx>
#include <NXOpen/CAM_MaxCutTraverse.hxx>
#include <NXOpen/CAM_MillCutParameters.hxx>
#include <NXOpen/CAM_MillOperationBuilder.hxx>
#include <NXOpen/CAM_MinCutVolume.hxx>
#include <NXOpen/CAM_MultiBladeBaseGeometry.hxx>
#include <NXOpen/CAM_MultiBladeSplittersGeometry.hxx>
#include <NXOpen/CAM_MultiDepthCut.hxx>
#include <NXOpen/CAM_MultipleStepoverBuilder.hxx>
#include <NXOpen/CAM_NcmAvoidancePointBuilder.hxx>
#include <NXOpen/CAM_NcmClearanceBuilder.hxx>
#include <NXOpen/CAM_NcmScBuilder.hxx>
#include <NXOpen/CAM_NcmScEngRetBuilder.hxx>
#include <NXOpen/CAM_NcmSmoothingBuilder.hxx>
#include <NXOpen/CAM_NcmSubopBuilder.hxx>
#include <NXOpen/CAM_NcmTransferBuilder.hxx>
#include <NXOpen/CAM_Notes.hxx>
#include <NXOpen/CAM_OperationBuilder.hxx>
#include <NXOpen/CAM_OperationCollection.hxx>
#include <NXOpen/CAM_OperationDisplayOptionsBuilder.hxx>
#include <NXOpen/CAM_OutputUncutRegions.hxx>
#include <NXOpen/CAM_ParamBuilder.hxx>
#include <NXOpen/CAM_PartAxisBuilder.hxx>
#include <NXOpen/CAM_PathDisplay.hxx>
#include <NXOpen/CAM_PathDisplayColors.hxx>
#include <NXOpen/CAM_PostEventsCiBuilder.hxx>
#include <NXOpen/CAM_ProjVecCiBuilder.hxx>
#include <NXOpen/CAM_Sequence.hxx>
#include <NXOpen/CAM_SmallAreaAvoidance.hxx>
#include <NXOpen/CAM_Splitter.hxx>
#include <NXOpen/CAM_SplitterList.hxx>
#include <NXOpen/CAM_SpunOutlineGeom.hxx>
#include <NXOpen/CAM_SteepContainment.hxx>
#include <NXOpen/CAM_StepoverBuilder.hxx>
#include <NXOpen/CAM_StockPerPassBuilder.hxx>
#include <NXOpen/CAM_SurfaceContour.hxx>
#include <NXOpen/CAM_SurfaceContourBuilder.hxx>
#include <NXOpen/CAM_SurfaceDriveGeometry.hxx>
#include <NXOpen/CAM_SurfaceDriveGeometrySet.hxx>
#include <NXOpen/CAM_SurfaceDriveGeometrySetList.hxx>
#include <NXOpen/CAM_Tool.hxx>
#include <NXOpen/CAM_ToolAxisAdvanced.hxx>
#include <NXOpen/CAM_ToolAxisBase.hxx>
#include <NXOpen/CAM_ToolAxisChange.hxx>
#include <NXOpen/CAM_ToolAxisCiBuilder.hxx>
#include <NXOpen/CAM_ToolAxisFixed.hxx>
#include <NXOpen/CAM_ToolAxisTilt.hxx>
#include <NXOpen/CAM_ToolAxisVariable.hxx>
#include <NXOpen/CAM_ToolChangeCiBuilder.hxx>
#include <NXOpen/CAM_ToolPathEditorBuilder.hxx>
#include <NXOpen/CAM_ToolPathSplitParametersBuilder.hxx>
#include <NXOpen/CAM_Topology.hxx>
#include <NXOpen/CAM_TrochoidalSettings.hxx>
#include <NXOpen/CAM_Ude.hxx>
#include <NXOpen/CAM_UdeList.hxx>
#include <NXOpen/CAM_UdeSet.hxx>
#include <NXOpen/CAM_UpDownCutting.hxx>
#include <NXOpen/CAM_ZAxisSmoothing.hxx>
#include <NXOpen/Direction.hxx>
#include <NXOpen/Face.hxx>
#include <NXOpen/NXObject.hxx>
#include <NXOpen/ObjectList.hxx>
#include <NXOpen/Part.hxx>
#include <NXOpen/PartCollection.hxx>
#include <NXOpen/PartLoadStatus.hxx>
#include <NXOpen/Plane.hxx>
#include <NXOpen/Point.hxx>
#include <NXOpen/ScCollector.hxx>
#include <NXOpen/ScRuleFactory.hxx>
#include <NXOpen/Section.hxx>
#include <NXOpen/SectionList.hxx>
#include <NXOpen/SelectTaggedObjectList.hxx>
#include <NXOpen/SelectionIntentRule.hxx>
#include <NXOpen/Session.hxx>
#include <NXOpen/SmartObject.hxx>
#include <NXOpen/TaggedObject.hxx>
#include <uf_defs.h>
#include <NXOpen/NXException.hxx>
#include <NXOpen/Session.hxx>
#include <NXOpen/Axis.hxx>
#include <NXOpen/BasePart.hxx>
#include <NXOpen/Body.hxx>
#include <NXOpen/BodyCollection.hxx>
#include <NXOpen/BodyDumbRule.hxx>
#include <NXOpen/Builder.hxx>
#include <NXOpen/CAM_AcrossVoids.hxx>
#include <NXOpen/CAM_ArcOutputTypeCiBuilder.hxx>
#include <NXOpen/CAM_AreaMillingContainment.hxx>
#include <NXOpen/CAM_AreaMillingNonSteepContainment.hxx>
#include <NXOpen/CAM_AreaMillingSteepContainment.hxx>
#include <NXOpen/CAM_BladeCutLevels.hxx>
#include <NXOpen/CAM_BladeFinish.hxx>
#include <NXOpen/CAM_BladeRough.hxx>
#include <NXOpen/CAM_BlendFinish.hxx>
#include <NXOpen/CAM_Boundary.hxx>
#include <NXOpen/CAM_BoundarySet.hxx>
#include <NXOpen/CAM_BoundarySetList.hxx>
#include <NXOpen/CAM_CAMSession.hxx>
#include <NXOpen/CAM_CAMSetup.hxx>
#include <NXOpen/CAM_CleanupOutput.hxx>
#include <NXOpen/CAM_CleanupSettings.hxx>
#include <NXOpen/CAM_CornerControlBuilder.hxx>
#include <NXOpen/CAM_CustomLoopSet.hxx>
#include <NXOpen/CAM_CustomLoopSetList.hxx>
#include <NXOpen/CAM_CustomLoops.hxx>
#include <NXOpen/CAM_CutAngle.hxx>
#include <NXOpen/CAM_CutDirection.hxx>
#include <NXOpen/CAM_CutParameters.hxx>
#include <NXOpen/CAM_CutPatternBuilder.hxx>
#include <NXOpen/CAM_CutVolumeGeom.hxx>
#include <NXOpen/CAM_Cycle.hxx>
#include <NXOpen/CAM_DisplayPaint.hxx>
#include <NXOpen/CAM_DisplayPath.hxx>
#include <NXOpen/CAM_DisplaySilhouette.hxx>
#include <NXOpen/CAM_DisplayTool.hxx>
#include <NXOpen/CAM_DmAmBuilder.hxx>
#include <NXOpen/CAM_DmBndBuilder.hxx>
#include <NXOpen/CAM_DmCmBuilder.hxx>
#include <NXOpen/CAM_DmCmMultiplePassesCiBuilder.hxx>
#include <NXOpen/CAM_DmCurveBuilder.hxx>
#include <NXOpen/CAM_DmRadBuilder.hxx>
#include <NXOpen/CAM_DmRotaryFloorFinishBuilder.hxx>
#include <NXOpen/CAM_DmSpiralBuilder.hxx>
#include <NXOpen/CAM_DmStreamlineCutStep.hxx>
#include <NXOpen/CAM_DmSurfBuilder.hxx>
#include <NXOpen/CAM_DmToolPosition.hxx>
#include <NXOpen/CAM_DmTpBuilder.hxx>
#include <NXOpen/CAM_DmTrimExtend.hxx>
#include <NXOpen/CAM_DmUserExitCiBuilder.hxx>
#include <NXOpen/CAM_DriveChainItemBuilder.hxx>
#include <NXOpen/CAM_DriveChainItemBuilderList.hxx>
#include <NXOpen/CAM_EdgeFinish.hxx>
#include <NXOpen/CAM_ExtendAtEdges.hxx>
#include <NXOpen/CAM_FeedsBuilder.hxx>
#include <NXOpen/CAM_FeedsOptimizationData.hxx>
#include <NXOpen/CAM_FinishPassesBuilder.hxx>
#include <NXOpen/CAM_FlowBuilder.hxx>
#include <NXOpen/CAM_FlowCutSteepContainment.hxx>
#include <NXOpen/CAM_Geometry.hxx>
#include <NXOpen/CAM_GeometryCiBuilder.hxx>
#include <NXOpen/CAM_GeometrySet.hxx>
#include <NXOpen/CAM_GeometrySetList.hxx>
#include <NXOpen/CAM_HoleBossGeom.hxx>
#include <NXOpen/CAM_HubFinish.hxx>
#include <NXOpen/CAM_Inheritable2dLength.hxx>
#include <NXOpen/CAM_InheritableDoubleBuilder.hxx>
#include <NXOpen/CAM_InheritableFeedBuilder.hxx>
#include <NXOpen/CAM_InheritableFeedModeBuilder.hxx>
#include <NXOpen/CAM_InheritableIntBuilder.hxx>
#include <NXOpen/CAM_InheritableTextBuilder.hxx>
#include <NXOpen/CAM_InheritableToolDepBuilder.hxx>
#include <NXOpen/CAM_InterpolateAngle.hxx>
#include <NXOpen/CAM_InterpolateVector.hxx>
#include <NXOpen/CAM_LayoutCiBuilder.hxx>
#include <NXOpen/CAM_LevelToLevel.hxx>
#include <NXOpen/CAM_LowerLimit.hxx>
#include <NXOpen/CAM_MaxCutTraverse.hxx>
#include <NXOpen/CAM_MillCutParameters.hxx>
#include <NXOpen/CAM_MillOperationBuilder.hxx>
#include <NXOpen/CAM_MinCutVolume.hxx>
#include <NXOpen/CAM_MultiBladeBaseGeometry.hxx>
#include <NXOpen/CAM_MultiBladeSplittersGeometry.hxx>
#include <NXOpen/CAM_MultiDepthCut.hxx>
#include <NXOpen/CAM_MultipleStepoverBuilder.hxx>
#include <NXOpen/CAM_NcmAvoidancePointBuilder.hxx>
#include <NXOpen/CAM_NcmClearanceBuilder.hxx>
#include <NXOpen/CAM_NcmScBuilder.hxx>
#include <NXOpen/CAM_NcmScEngRetBuilder.hxx>
#include <NXOpen/CAM_NcmSmoothingBuilder.hxx>
#include <NXOpen/CAM_NcmSubopBuilder.hxx>
#include <NXOpen/CAM_NcmTransferBuilder.hxx>
#include <NXOpen/CAM_Notes.hxx>
#include <NXOpen/CAM_OperationBuilder.hxx>
#include <NXOpen/CAM_OperationCollection.hxx>
#include <NXOpen/CAM_OperationDisplayOptionsBuilder.hxx>
#include <NXOpen/CAM_OutputUncutRegions.hxx>
#include <NXOpen/CAM_ParamBuilder.hxx>
#include <NXOpen/CAM_PartAxisBuilder.hxx>
#include <NXOpen/CAM_PathDisplay.hxx>
#include <NXOpen/CAM_PathDisplayColors.hxx>
#include <NXOpen/CAM_PostEventsCiBuilder.hxx>
#include <NXOpen/CAM_ProjVecCiBuilder.hxx>
#include <NXOpen/CAM_Sequence.hxx>
#include <NXOpen/CAM_SmallAreaAvoidance.hxx>
#include <NXOpen/CAM_Splitter.hxx>
#include <NXOpen/CAM_SplitterList.hxx>
#include <NXOpen/CAM_SpunOutlineGeom.hxx>
#include <NXOpen/CAM_SteepContainment.hxx>
#include <NXOpen/CAM_StepoverBuilder.hxx>
#include <NXOpen/CAM_StockPerPassBuilder.hxx>
#include <NXOpen/CAM_SurfaceContour.hxx>
#include <NXOpen/CAM_SurfaceContourBuilder.hxx>
#include <NXOpen/CAM_SurfaceDriveGeometry.hxx>
#include <NXOpen/CAM_SurfaceDriveGeometrySet.hxx>
#include <NXOpen/CAM_SurfaceDriveGeometrySetList.hxx>
#include <NXOpen/CAM_Tool.hxx>
#include <NXOpen/CAM_ToolAxisAdvanced.hxx>
#include <NXOpen/CAM_ToolAxisBase.hxx>
#include <NXOpen/CAM_ToolAxisChange.hxx>
#include <NXOpen/CAM_ToolAxisCiBuilder.hxx>
#include <NXOpen/CAM_ToolAxisFixed.hxx>
#include <NXOpen/CAM_ToolAxisTilt.hxx>
#include <NXOpen/CAM_ToolAxisVariable.hxx>
#include <NXOpen/CAM_ToolChangeCiBuilder.hxx>
#include <NXOpen/CAM_ToolPathEditorBuilder.hxx>
#include <NXOpen/CAM_ToolPathSplitParametersBuilder.hxx>
#include <NXOpen/CAM_Topology.hxx>
#include <NXOpen/CAM_TrochoidalSettings.hxx>
#include <NXOpen/CAM_Ude.hxx>
#include <NXOpen/CAM_UdeList.hxx>
#include <NXOpen/CAM_UdeSet.hxx>
#include <NXOpen/CAM_UpDownCutting.hxx>
#include <NXOpen/CAM_ZAxisSmoothing.hxx>
#include <NXOpen/Direction.hxx>
#include <NXOpen/Face.hxx>
#include <NXOpen/NXObject.hxx>
#include <NXOpen/ObjectList.hxx>
#include <NXOpen/Part.hxx>
#include <NXOpen/PartCollection.hxx>
#include <NXOpen/PartLoadStatus.hxx>
#include <NXOpen/Plane.hxx>
#include <NXOpen/Point.hxx>
#include <NXOpen/ScCollector.hxx>
#include <NXOpen/ScRuleFactory.hxx>
#include <NXOpen/Section.hxx>
#include <NXOpen/SectionList.hxx>
#include <NXOpen/SelectTaggedObjectList.hxx>
#include <NXOpen/SelectionIntentRule.hxx>
#include <NXOpen/Session.hxx>
#include <NXOpen/SmartObject.hxx>
#include <NXOpen/TaggedObject.hxx>
#define Epslion 1e-8
using namespace NXOpen;

Vector3d AnnulusCavityBottomMillingCmd::get_Centre_fr_Face(Face* ipFace)
{
	tag_t facetag = ipFace->Tag();
	double param[2];
	double point[3];
	double u1[3];
	double v1[3];
	double u2[3];
	double v2[3];
	double unit_norm[3];
	double radii[2];
	char msg[132];
	double uv_min_max[4];

	UF_MODL_ask_face_uv_minmax(facetag, uv_min_max);

	double uaverage = (uv_min_max[0] + uv_min_max[1]) / 2;
	double vaverage = (uv_min_max[2] + uv_min_max[3]) / 2;

	param[0] = uaverage;
	param[1] = vaverage;

	UF_MODL_ask_face_props(facetag, param, point, u1, v1, u2, v2, unit_norm, radii);

	Point3d centerpoint(point[0], point[1], point[2]);
	Vector3d centernormal(unit_norm[0], unit_norm[1], unit_norm[2]);

	return centernormal;
}

AnnulusCavityBottomMillingCmd::AnnulusCavityBottomMillingCmd():
TaggedObject()
{
}

AnnulusCavityBottomMillingCmd::AnnulusCavityBottomMillingCmd(std::vector<AnnulusCavity*> ListOfCavity, std::vector<Boss*> ListOfBoss,
	AnnulusCavityProcessParameter ParameterSet, Part* WorkPart,
	Body* WorkBody, Body* WorkBlank, std::vector<Face*> ListOfSelectedFace) :
TaggedObject()
{
	//uc1601("Enter AnnulusCavityBottomMillingCmd!",1);

	AnnulusCavityResultList = ListOfCavity;       //槽识别结果
	
	AnnulusCavityParameterSet = ParameterSet;     //工艺决策参数
	workPart     = WorkPart;               //工作部件
	selectedBody = WorkBody;  //零件体 
	selectedBlank = WorkBlank;//毛胚
	SelectedFaceList = ListOfSelectedFace;//所选面


	UF_initialize();
	AnnulusCavityBottomMillingDecision4(SelectedFaceList);//加工所选槽,曲面驱动

	
	UF_terminate();
}

//-----------------------------------------------------------------------------
//  destructor
//-----------------------------------------------------------------------------
AnnulusCavityBottomMillingCmd::~AnnulusCavityBottomMillingCmd()
{
}

void AnnulusCavityBottomMillingCmd::AnnulusCavityBottomMillingDecision4(std::vector<Face*> SelectedFaceList)
{
	//uc1601("加工所选槽!",1);//曲面驱动 加工所选槽

	//1 拿到WCS的坐标矩阵，这里要求zc轴向上
	tag_t wcsMatrixTag = NULL_TAG;
	double wcsMatrixValues[9] = { 0.0 };

	tag_t wcsTag = NULL_TAG;
	UF_CSYS_ask_wcs(&wcsTag);
	UF_CSYS_ask_matrix_of_object(wcsTag, &wcsMatrixTag);
	UF_CSYS_ask_matrix_values(wcsMatrixTag, wcsMatrixValues);

	double wcsX[3] = { wcsMatrixValues[0], wcsMatrixValues[1], wcsMatrixValues[2] };
	double wcsY[3] = { wcsMatrixValues[3], wcsMatrixValues[4], wcsMatrixValues[5] };
	double wcsZ[3] = { wcsMatrixValues[6], wcsMatrixValues[7], wcsMatrixValues[8] };
	
	//设置进退刀的轴向
	NXOpen::Vector3d _Normal(wcsMatrixValues[6], wcsMatrixValues[7], wcsMatrixValues[8]);

	int Index = 0;
	for (std::vector<AnnulusCavity*>::size_type i = 0; i != SelectedFaceList.size(); i++)
	{
		Face* EachAnnulusCavity = SelectedFaceList[i];
		tag_t EachFaceTag = EachAnnulusCavity->Tag();
		
		//局部坐标系相关----------------------------------------------------------------------------------------------
		//判断一下面类型type
		int type = 0;       //判断面类型
		int norm_dir = 0;   //1=外R，-1=内R
		double point[3];  //轴中心 点
		double dir[3];    //轴方向
		double box[6];    //极点坐标
		double radius;    //半径 圆锥=最小半径 + 最大半径 /2
		double rad_data;
		UF_MODL_ask_face_data(EachAnnulusCavity->Tag(), &type, point, dir, box, &radius, &rad_data, &norm_dir);
		
		//得到面的中心法矢量
		Vector3d iCenterNormal = get_Centre_fr_Face(EachAnnulusCavity);
		Point3d iCenterPoint = GetCenterPoint(EachAnnulusCavity);

		Point *midPoint;
		midPoint = workPart->Points()->CreatePoint(iCenterPoint);

		Direction * direction = workPart->Directions()->CreateDirection(EachAnnulusCavity, midPoint, SenseForward, SmartObject::UpdateOptionAfterModeling);
		NXOpen::Vector3d Dir1 = direction->Vector();
		NXOpen::Vector3d MidDir;
		MidDir.X = Dir1.X;
		MidDir.Y = Dir1.Y;
		MidDir.Z = Dir1.Z;

		Direction *iDirection;
		iDirection = workPart->Directions()->CreateDirection(iCenterPoint, MidDir, SmartObject::UpdateOptionAfterModeling);

		//获取面的TAG值

		std::string tagStr = std::to_string(EachFaceTag);
		const char *MCSTagName = tagStr.c_str();

		//设置程序的名字:milling+tag
		char* StringIndex = new char;
		StringIndex = const_cast<char*>(MCSTagName);
		NXString NXStringIndex = StringIndex;

		NXString iOperationName = "Milling." + NXStringIndex;

		Session *theSession = Session::GetSession();
		Part *workPart(theSession->Parts()->Work());
		Part *displayPart(theSession->Parts()->Display());

		const char * WorkpieceName = nullptr;
		NXString Geomtrypart;

		//创建可变轴轮廓铣操作
		CAM::NCGroup *nCGroup1(dynamic_cast<CAM::NCGroup *>(workPart->CAMSetup()->CAMGroupCollection()->FindObject(AnnulusCavityParameterSet.PROGRAM)));
		
		CAM::Method *method1(dynamic_cast<CAM::Method *>(workPart->CAMSetup()->CAMGroupCollection()->FindObject(AnnulusCavityParameterSet.METHOD)));
	
		CAM::Tool *tool1(dynamic_cast<CAM::Tool *>(workPart->CAMSetup()->CAMGroupCollection()->FindObject(AnnulusCavityParameterSet.ToolMsg.ToolName)));
		
		if (AnnulusCavityParameterSet.IfLCsys)
		{

			Vector3d centernormal = get_Centre_fr_Face(EachAnnulusCavity);
			double Facenormal[3] = { centernormal.X, centernormal.Y, centernormal.Z };

			//2.2 获得NX自动捕获的顶面上的坐标原点、矩阵坐标
			//坐标原点
			Point3d centeripoint = GetCenterPoint(EachAnnulusCavity);
			double origin[3] = { centeripoint.X, centeripoint.Y, centeripoint.Z };//origin是原点

			//坐标矩阵
			tag_t MatrixTag = NULL_TAG;//创建空的对象矩阵tag

			UF_CSYS_ask_matrix_of_object(EachFaceTag, &MatrixTag); //获得矩阵tag
			double Mtx_3d[9] = { 0.0 };
			UF_CSYS_ask_matrix_values(MatrixTag, Mtx_3d);//根据矩阵tag值获得矩阵值

			double autoX[3] = { Mtx_3d[0], Mtx_3d[1], Mtx_3d[2] }; //三个方向分量
			double autoY[3] = { Mtx_3d[3], Mtx_3d[4], Mtx_3d[5] };
			double autoZ[3] = { Mtx_3d[6], Mtx_3d[7], Mtx_3d[8] };

			//开始构造符合要求的坐标矩阵
			double targetMTx[9] = { 0.0 }; //初始化一个目标矩阵
			double product1 = DotProduct(autoX, wcsZ);
			double product2 = DotProduct(autoY, wcsZ);
			double product3 = DotProduct(autoZ, Facenormal);

			bool boolbutton = 0;

			if (type != 22)
			{
				//uc1601("不是平面", 1);

				double normalVector[3] = { iCenterNormal.X, iCenterNormal.Y, iCenterNormal.Z };

				double tempvector[3] = { 0.0 };

				if (abs(wcsZ[0]) > 0.5)
				{
					tempvector[0] = 0;
					tempvector[1] = -0.5 * normalVector[2] / normalVector[1];
					tempvector[2] = 0.5;
				}

				if (abs(wcsZ[1]) > 0.5)
				{
					tempvector[0] = 0.5;
					tempvector[1] = 0;
					tempvector[2] = -0.5 * normalVector[0] / normalVector[2]; 0.5;
				}

				if (abs(wcsZ[2]) > 0.5)
				{
					tempvector[0] = 0.5;
					tempvector[1] = -0.5 * normalVector[0] / normalVector[1];
					tempvector[2] = 0;
				}

				double mode = sqrt(tempvector[0] * tempvector[0] + tempvector[1] * tempvector[1] + tempvector[2] * tempvector[2]);

				double tagentVector[3] = { tempvector[0] / mode, tempvector[1] / mode, tempvector[2] / mode };


				double TryMtx[9];
				UF_MTX3_initialize(tagentVector, normalVector, TryMtx);

				targetMTx[0] = TryMtx[0];
				targetMTx[1] = TryMtx[1];
				targetMTx[2] = TryMtx[2];
				targetMTx[3] = -1 * TryMtx[6];
				targetMTx[4] = -1 * TryMtx[7];
				targetMTx[5] = -1 * TryMtx[8];
				targetMTx[6] = TryMtx[3];
				targetMTx[7] = TryMtx[4];
				targetMTx[8] = TryMtx[5];


				//=================2024.04为修改局部坐标系而添加================
				//加这一段的原因是发现，在非平面生成局部坐标系时，有可能生成的YM是朝上的
				double firstX[3] = { targetMTx[0], targetMTx[1], targetMTx[2] }; //对新得到的坐标系进行二次判断,主要是为了让Xm和Ym的方向满足要求，否则有坐标系的Ym朝下
				double firstY[3] = { targetMTx[3], targetMTx[4], targetMTx[5] };
				double firstZ[3] = { targetMTx[6], targetMTx[7], targetMTx[8] };

				double productYZ = DotProduct(firstY, wcsZ);//得到Ym和Zc的内积，YM如果朝上那么内积应该大于0，如果内积小于0，那么YM需要反向

				if (productYZ > 0)
				{
					//uc1601("YM是朝上的，正常", 1);
				}
				if (productYZ < 0)
				{

					//uc1601("YM是朝下的，需要逆转", 1);

					targetMTx[0] = -1 * targetMTx[0];
					targetMTx[1] = -1 * targetMTx[1];
					targetMTx[2] = -1 * targetMTx[2];
					targetMTx[3] = -1 * targetMTx[3];
					targetMTx[4] = -1 * targetMTx[4];
					targetMTx[5] = -1 * targetMTx[5];
					targetMTx[6] = targetMTx[6];
					targetMTx[7] = targetMTx[7];
					targetMTx[8] = targetMTx[8];

				}
				//=================2024.04为修改局部坐标系而添加================
				boolbutton = 1;


			}
			if (type == 22 && abs(product1) < Epslion  && product2 > 0)
			{
				boolbutton = 1;
				//uc1601("自动生成的x,y方向是对的，且y向上", 1);
				if (product3 > 0)
				{
					targetMTx[0] = autoX[0];
					targetMTx[1] = autoX[1];
					targetMTx[2] = autoX[2];
					targetMTx[3] = autoY[0];
					targetMTx[4] = autoY[1];
					targetMTx[5] = autoY[2];
					targetMTx[6] = autoZ[0];
					targetMTx[7] = autoZ[1];
					targetMTx[8] = autoZ[2];
				}
				if (product3 < 0)
				{
					targetMTx[0] = -1 * autoX[0];
					targetMTx[1] = -1 * autoX[1];
					targetMTx[2] = -1 * autoX[2];
					targetMTx[3] = autoY[0];
					targetMTx[4] = autoY[1];
					targetMTx[5] = autoY[2];
					targetMTx[6] = -1 * autoZ[0];
					targetMTx[7] = -1 * autoZ[1];
					targetMTx[8] = -1 * autoZ[2];
				}
			}
			if (type == 22 && abs(product1) < Epslion  && product2 < 0) //首先反向autoY
			{
				boolbutton = 1;
				//uc1601("自动生成的x,y方向是对的，但y向下", 1);
				double TryMtx[9]; //利用现有的x和反向后的Y生成右手坐标系矩阵
				autoY[0] = -1 * autoY[0];
				autoY[1] = -1 * autoY[1];
				autoY[2] = -1 * autoY[2];
				UF_MTX3_initialize(autoX, autoY, TryMtx);

				double TryZ[3] = { TryMtx[6], TryMtx[7], TryMtx[8] };
				double product4 = DotProduct(Facenormal, TryZ);

				if (product4 > 0)//说明autoX没有问题，直接使用TryMtx即可
				{
					targetMTx[0] = autoX[0];
					targetMTx[1] = autoX[1];
					targetMTx[2] = autoX[2];
					targetMTx[3] = autoY[0];
					targetMTx[4] = autoY[1];
					targetMTx[5] = autoY[2];
					targetMTx[6] = TryMtx[6];
					targetMTx[7] = TryMtx[7];
					targetMTx[8] = TryMtx[8];

				}
				if (product4 < 0) //说明autoX有问题，需要反向
				{
					autoX[0] = -1 * autoX[0];
					autoX[1] = -1 * autoX[1];
					autoX[2] = -1 * autoX[2];

					double TryMtx2[9];
					UF_MTX3_initialize(autoX, autoY, TryMtx2);

					targetMTx[0] = TryMtx2[0];
					targetMTx[1] = TryMtx2[1];
					targetMTx[2] = TryMtx2[2];
					targetMTx[3] = TryMtx2[3];
					targetMTx[4] = TryMtx2[4];
					targetMTx[5] = TryMtx2[5];
					targetMTx[6] = TryMtx2[6];
					targetMTx[7] = TryMtx2[7];
					targetMTx[8] = TryMtx2[8];
				}

			}
			if (type == 22 && abs(product2) < Epslion  && product1 > 0) //说明Y方向占了我们所希望的X的方向
			{
				boolbutton = 1;
				//uc1601("自动生成的x,y方向是反的，且x是向上的", 1);

				double TryMtx[9]; //利用现有的x和反向后的Y生成右手坐标系矩阵
				UF_MTX3_initialize(autoY, autoX, TryMtx);

				double TryZ[3] = { TryMtx[6], TryMtx[7], TryMtx[8] };
				double product4 = DotProduct(Facenormal, TryZ);

				if (product4 > 0)//说明交换位置后就OK了
				{
					targetMTx[0] = autoY[0];
					targetMTx[1] = autoY[1];
					targetMTx[2] = autoY[2];
					targetMTx[3] = autoX[0];
					targetMTx[4] = autoX[1];
					targetMTx[5] = autoX[2];
					targetMTx[6] = TryMtx[6];
					targetMTx[7] = TryMtx[7];
					targetMTx[8] = TryMtx[8];

				}
				if (product4 < 0)
				{
					autoY[0] = -1 * autoY[0];
					autoY[1] = -1 * autoY[1];
					autoY[2] = -1 * autoY[2];

					double TryMtx3[9];
					UF_MTX3_initialize(autoY, autoX, TryMtx3);

					targetMTx[0] = TryMtx3[0];
					targetMTx[1] = TryMtx3[1];
					targetMTx[2] = TryMtx3[2];
					targetMTx[3] = TryMtx3[3];
					targetMTx[4] = TryMtx3[4];
					targetMTx[5] = TryMtx3[5];
					targetMTx[6] = TryMtx3[6];
					targetMTx[7] = TryMtx3[7];
					targetMTx[8] = TryMtx3[8];

				}
			}
			if (type == 22 && abs(product2) < Epslion && product1 < 0)//说明Y方向占了我们所希望的X的方向,autoX方向也是反的
			{
				boolbutton = 1;
				//uc1601("自动生成的x,y方向是反的，且x是向下的", 1);

				autoX[0] = -1 * autoX[0];
				autoX[1] = -1 * autoX[1];
				autoX[2] = -1 * autoX[2];

				double TryMtx[9]; //利用现有的x和反向后的Y生成右手坐标系矩阵
				UF_MTX3_initialize(autoY, autoX, TryMtx);

				double TryZ[3] = { TryMtx[6], TryMtx[7], TryMtx[8] };
				double product4 = DotProduct(Facenormal, TryZ);

				if (product4 > 0)//说明交换个位置就可以
				{
					targetMTx[0] = TryMtx[0];
					targetMTx[1] = TryMtx[1];
					targetMTx[2] = TryMtx[2];
					targetMTx[3] = TryMtx[3];
					targetMTx[4] = TryMtx[4];
					targetMTx[5] = TryMtx[5];
					targetMTx[6] = TryMtx[6];
					targetMTx[7] = TryMtx[7];
					targetMTx[8] = TryMtx[8];

				}
				if (product4 < 0) //要反向最后的x方向
				{
					autoY[0] = -1 * autoY[0];
					autoY[1] = -1 * autoY[1];
					autoY[2] = -1 * autoY[2];

					double TryMtx4[9];
					UF_MTX3_initialize(autoY, autoX, TryMtx4);

					targetMTx[0] = TryMtx4[0];
					targetMTx[1] = TryMtx4[1];
					targetMTx[2] = TryMtx4[2];
					targetMTx[3] = TryMtx4[3];
					targetMTx[4] = TryMtx4[4];
					targetMTx[5] = TryMtx4[5];
					targetMTx[6] = TryMtx4[6];
					targetMTx[7] = TryMtx4[7];
					targetMTx[8] = TryMtx4[8];

				}
			}

			tag_t MatrixID = NULL_TAG;
			UF_CSYS_create_matrix(targetMTx, &MatrixID);

			double CsysOrigin[3] = { origin[0], origin[1], origin[2] };
			tag_t CsysId = NULL_TAG;

			const char *MCSName = nullptr;
			std::string tagStr = std::to_string(EachFaceTag);
			const char *MCSTagName = tagStr.c_str();
			const char *prefix = "MCS_face";
			int lenPrefix = strlen(prefix);
			char* newSring = new char[lenPrefix + tagStr.length() + 1];
			strcpy(newSring, prefix);
			MCSName = strcat(newSring, MCSTagName);

			if (boolbutton == 0)
			{
				UF_CSYS_create_temp_csys(CsysOrigin, MatrixTag, &CsysId);

			}
			else
			{
				UF_CSYS_create_temp_csys(CsysOrigin, MatrixID, &CsysId);
			}
			tag_t  newMCSObject = NULL_TAG;
			UF_NCGEOM_create("mill_contour", "MCS", &newMCSObject);
			UF_UI_ONT_refresh();
			UF_OBJ_set_name(newMCSObject, MCSName);
			tag_t setupTag = NULL_TAG;
			UF_SETUP_ask_setup(&setupTag);
			if (setupTag == NULL_TAG)
			{
				uc1601("提示：请先初始化加工环境", 1);
				return;
			}
			//获取几何根组
			tag_t GeomRootGroup = NULL_TAG;
			UF_SETUP_ask_geom_root(setupTag, &GeomRootGroup);
			char *tempMCSname = nullptr;
			tempMCSname = const_cast<char*>(MCSName);
			tag_t ExistMCSTag = NULL_TAG;
			UF_NCGROUP_ask_object_of_name(GeomRootGroup, tempMCSname, &ExistMCSTag);
			if (ExistMCSTag == NULL_TAG)
			{
				logical answer = 0;
				char reason[UF_NCGROUP_MAX_REASON_LEN];
				int error = UF_NCGROUP_can_accept_member(GeomRootGroup, newMCSObject, &answer, reason);
				if (error)
				{
					uc1601("不接受新建的成员?", 1);
				}
				if (answer)
				{
					error = UF_NCGROUP_accept_member(GeomRootGroup, newMCSObject);
					if (error)
					{
						uc1601("在根组中添加失败", 1);
					}
					else
					{
						UF_PARAM_set_tag_value(newMCSObject, UF_PARAM_MCS, CsysId);
						UF_UI_ONT_refresh();

					}
				}
			}
			else
			{
				uc1601("已存在该坐标系", 1);
			}

			//在坐标系下创建新的几何体
			tag_t  newWorkpiece = NULL_TAG;
			UF_NCGEOM_create("mill_contour", "WORKPIECE", &newWorkpiece);
			UF_UI_ONT_refresh();

			char*Eachname1 = "Milling.";
			int len = strlen(Eachname1);
			char* tempEachName = new char[len + 1];
			strcpy(tempEachName, Eachname1);

			//const char * WorkpieceName = nullptr;
			const char* suffix = "MCS_";   //前缀
			int lenSuffix = strlen(suffix);
			char* newString1 = new char[lenSuffix + tagStr.length() + 1];
			strcpy(newString1, suffix);
			WorkpieceName = strcat(newString1, MCSTagName);

			UF_OBJ_set_name(newWorkpiece, WorkpieceName);
			UF_UI_ONT_refresh();

			//2. 根据名称查找工作坐标系的Tag
			//获取当前的UF_SETUP对象
			tag_t NewsetupTag = NULL_TAG;
			UF_SETUP_ask_setup(&NewsetupTag);

			//获取几何的根组，如果是刀具或者程序跟组，则函数名不同
			tag_t NewRootGroup = NULL_TAG;
			UF_SETUP_ask_geom_root(NewsetupTag, &NewRootGroup);

			//在几何根组下查询坐标系并得到其的TAG
			tag_t obj_with_name = NULL_TAG;
			char *tempname = nullptr;
			tempname = const_cast<char*>(MCSName);
			UF_NCGROUP_ask_object_of_name(NewRootGroup, tempname, &obj_with_name);

			tag_t ExistWPTag = NULL_TAG;
			UF_NCGROUP_ask_object_of_name(GeomRootGroup, const_cast<char*>(WorkpieceName), &ExistWPTag);

			if (ExistWPTag == NULL_TAG)
			{
				//3. 将workpiece几何体添加至查找到的坐标系下，进行逻辑判断
				logical answer2 = 0;
				char reason2[UF_NCGROUP_MAX_REASON_LEN];
				int error2 = UF_NCGROUP_can_accept_member(obj_with_name, newWorkpiece, &answer2, reason2);

				if (error2)
				{
					uc1601("不接受添加workpiece", 1);
				}
				else
				{

					if (answer2)
					{
						error2 = UF_NCGROUP_accept_member(obj_with_name, newWorkpiece);
						if (error2)
						{
							uc1601("添加失败", 1);
						}
						else
						{
							//uc1601("添加00000", 1);
							tag_t PartTag = selectedBody->Tag();
							tag_t blankTag = selectedBlank->Tag();

							if (PartTag == NULL_TAG || blankTag == NULL_TAG)
							{

								return;
							}
							else
							{
								//设置工件

								tag_t entity_list[1] = { PartTag };
								UF_CAMGEOM_append_items(newWorkpiece, UF_CAM_part, 1, entity_list, NULL);

								//设置毛坯

								tag_t blank_list[1] = { blankTag };
								UF_CAMGEOM_append_items(newWorkpiece, UF_CAM_blank, 1, blank_list, NULL);

								//刷新导航器
								UF_UI_ONT_refresh();
								//uc1601("设置工件和毛坯成功", 1);
							}

						}
					}
				}

			}
			else
			{
				uc1601("已存在同名称WORKPIECE", 1);
			}

			Geomtrypart = WorkpieceName;

		}
		else
		{

			Geomtrypart = "WORKPIECE";

		}
		CAM::FeatureGeometry *featureGeometry1(dynamic_cast<CAM::FeatureGeometry *>(workPart->CAMSetup()->CAMGroupCollection()->FindObject(Geomtrypart)));
		CAM::Operation *operation1;
	
		
		try
		{
			operation1 = workPart->CAMSetup()->CAMOperationCollection()->Create(nCGroup1, method1, tool1, featureGeometry1, "mill_multi-axis", "VARIABLE_CONTOUR", CAM::OperationCollection::UseDefaultNameFalse, iOperationName);

		}
		catch (...)
		{
			operation1 = workPart->CAMSetup()->CAMOperationCollection()->Create(nCGroup1, method1, tool1, featureGeometry1, "mill_multi-axis", "VARIABLE_CONTOUR", CAM::OperationCollection::UseDefaultNameTrue, iOperationName);
		}

		CAM::SurfaceContour *surfaceContour1(dynamic_cast<CAM::SurfaceContour *>(operation1));
		CAM::SurfaceContourBuilder *surfaceContourBuilder1;
		surfaceContourBuilder1 = workPart->CAMSetup()->CAMOperationCollection()->CreateSurfaceContourBuilder(surfaceContour1);

		//刀轴可变
		CAM::ToolAxisVariable *toolAxisVariable1;
		toolAxisVariable1 = surfaceContourBuilder1->ToolAxisVariable();

		//设置"指定部件"
		//tag_t entity_list2[1] = { partTag };
		//UF_CAMGEOM_append_items(operation1->Tag(), UF_CAM_part, 1, entity_list2, NULL);

		//设置"指定检查"--------------------------------------------------------------------------------

		//UF_CAMGEOM_append_items(operation1->Tag(), UF_CAM_check, 1, entity_list2, NULL);
			
		//设置检查体等--------------------------------------------------------------------------------

		surfaceContourBuilder1->CheckGeometry()->InitializeData(false);
		NXOpen::CAM::GeometrySetList *geometrySetList1;
		geometrySetList1 = surfaceContourBuilder1->CheckGeometry()->GeometryList();

		NXOpen::TaggedObject *taggedObject1;
		taggedObject1 = geometrySetList1->FindItem(0);

		NXOpen::CAM::GeometrySet *geometrySet1(dynamic_cast<NXOpen::CAM::GeometrySet *>(taggedObject1));

		std::vector<NXOpen::Body *> bodies1(1);
		/*NXOpen::Body *body1(dynamic_cast<NXOpen::Body *>(workPart->Bodies()->FindObject("partTag")));*/
		bodies1[0] = selectedBody;
		NXOpen::BodyDumbRule *bodyDumbRule1;
		bodyDumbRule1 = workPart->ScRuleFactory()->CreateRuleBodyDumb(bodies1, true);

		NXOpen::ScCollector *scCollector1;
		scCollector1 = geometrySet1->ScCollector();

		std::vector<NXOpen::SelectionIntentRule *> rules1(1);
		rules1[0] = bodyDumbRule1;
		scCollector1->ReplaceRules(rules1, false);

		geometrySet1->SetCustomStock(true);
		geometrySet1->SetSafeClearance(0.0);

		NXObject *nXObject2;
		nXObject2 = surfaceContourBuilder1->Commit();

		//设置垂直于驱动面
		toolAxisVariable1->SetToolAxisType(CAM::ToolAxisVariable::Types::TypesNormalToDrive);  //TypesNormalToPart

		//切削区域赋值
		tag_t iFaceTag = EachAnnulusCavity->Tag();
		char b[50];


		int result = UF_CAMGEOM_append_items(operation1->Tag(), UF_CAM_cut_area, 1, &iFaceTag, NULL);
		if (result != 0)
		{
			char* str("cut_area error");
			uc1601(str, 1);
		} 

		//设置 曲面驱动的驱动方式
		surfaceContourBuilder1->SetDriveMethod(CAM::SurfaceContourBuilder::DriveMethodTypesSurfaceArea);  //DriveMethodTypesSurfaceArea
		surfaceContourBuilder1->SetCleanupType(CAM::SurfaceContourBuilder::CleanupTypesOff);
		////更新
		NXObject *nXObject;
		nXObject = surfaceContourBuilder1->Commit();

		//驱动几何体
		CAM::SurfaceDriveGeometry *SurfaceDriveGeometry1;
		SurfaceDriveGeometry1 = surfaceContourBuilder1->DmSurfBuilder()->DriveGeometry();

		NXOpen::NXObject * TfaceObject = (NXOpen::NXObject *) EachAnnulusCavity;

		TaggedObject *taggedObject2;
		taggedObject2 = surfaceContourBuilder1->DmSurfBuilder()->DriveGeometry()->GeometryList()->FindItem(0);
		CAM::SurfaceDriveGeometrySet *surfaceDriveGeometrySet1(dynamic_cast<CAM::SurfaceDriveGeometrySet *>(taggedObject2));
		surfaceDriveGeometrySet1->SetSurface(TfaceObject);

		nXObject = surfaceContourBuilder1->Commit();

		surfaceContourBuilder1->DmSurfBuilder()->SetDmSurfCutArea(CAM::DmSurfBuilder::DmSurfCutAreaTypeSurfacePercent);
		nXObject = surfaceContourBuilder1->Commit();
		//uc1601("01",1);//设置切削区域
		surfaceContourBuilder1->DmSurfBuilder()->SetDmSurfToolPos(CAM::DmSurfBuilder::ToolPositionTypeTanto);
		nXObject = surfaceContourBuilder1->Commit();
		//uc1601("02",1);//设置刀具位置
		surfaceContourBuilder1->DmSurfBuilder()->SetCutDirection(CAM::DmSurfBuilder::CutDirectionCorner1U);
		nXObject = surfaceContourBuilder1->Commit();
		//uc1601("03",1);//设置切削方向
		surfaceContourBuilder1->DmSurfBuilder()->MatSideTypeOpp;
		nXObject = surfaceContourBuilder1->Commit();
		//uc1601("04",1);//设置材料反向

		//驱动设置
		//surfaceContourBuilder1->DmSurfBuilder()->CutPatternBuilder()->SetCutPattern(CAM::CutPatternBuilder::TypesProfile);
		//uc1601("05",1);//设置切削模式
		surfaceContourBuilder1->DmSurfBuilder()->StepoverBuilder()->SetStepoverType(CAM::StepoverBuilder::StepoverTypesNumber);
		//uc1601("06",1);//步距类型：数量
		surfaceContourBuilder1->DmSurfBuilder()->StepoverBuilder()->SetNumberOfStepovers(AnnulusCavityParameterSet.StepDisAmount);
		//uc1601("07",1);//设定步距数

		//更新
		/*NXObject *nXObject;*/
		nXObject = surfaceContourBuilder1->Commit();
		//uc1601("0222", 1);
		//uc1601("07.1", 1);//设定步距数

		//赋值加工参数
		//部件余量
		surfaceContourBuilder1->CutParameters()->PartStock()->SetValue(AnnulusCavityParameterSet.PartAllowance);

		//公差
		CAM::Inheritable2dLength *inheritable2dLength1;
		inheritable2dLength1 = surfaceContourBuilder1->CutParameters()->Tolerances();

		inheritable2dLength1->SetValue0(AnnulusCavityParameterSet.Tolerance);
		inheritable2dLength1->SetValue1(AnnulusCavityParameterSet.Tolerance);
		//uc1601("05", 1);//设定步距数
		//主轴转速以及三个速度
		surfaceContourBuilder1->FeedsBuilder()->SpindleRpmBuilder()->SetValue(AnnulusCavityParameterSet.SpindleSpeed);
		surfaceContourBuilder1->FeedsBuilder()->FeedCutBuilder()->SetValue(AnnulusCavityParameterSet.Machining);
		surfaceContourBuilder1->FeedsBuilder()->FeedEngageBuilder()->SetValue(AnnulusCavityParameterSet.Approach);
		surfaceContourBuilder1->FeedsBuilder()->FeedRetractBuilder()->SetValue(AnnulusCavityParameterSet.Retract);
		//uc1601("08", 1);//设定步距数
		//进退刀设置
		surfaceContourBuilder1->NonCuttingBuilder()->RetractOpenAreaBuilder()->SetEngRetType(CAM::NcmScEngRetBuilder::EngRetTypesPlungeLift);
		surfaceContourBuilder1->NonCuttingBuilder()->TransferWithinRegionsBuilder()->SetApproachMethod(CAM::NcmTransferBuilder::AppDepMethodTypesAlongToolAxis);
		surfaceContourBuilder1->NonCuttingBuilder()->TransferWithinRegionsBuilder()->SetDepartureMethod(CAM::NcmTransferBuilder::AppDepMethodTypesAlongToolAxis);
		//uc1601("06", 1);//设定步距数
		if (AnnulusCavityParameterSet.ToolMotion.ToolMotionType == 0)
		{
			surfaceContourBuilder1->NonCuttingBuilder()->EngageOpenAreaBuilder()->SetEngRetType(CAM::NcmScEngRetBuilder::EngRetTypesArcParallelToToolAxis);
			//圆弧半径
			surfaceContourBuilder1->NonCuttingBuilder()->EngageOpenAreaBuilder()->RadiusBuilder()->SetValue(AnnulusCavityParameterSet.ToolMotion.CircleRadius);
			switch (AnnulusCavityParameterSet.ToolMotion.CircleRadiusUnit)
			{
			case 0:
				surfaceContourBuilder1->NonCuttingBuilder()->EngageOpenAreaBuilder()->RadiusBuilder()->SetIntent(CAM::ParamValueIntentToolDep);
				break;
			case 1:
				surfaceContourBuilder1->NonCuttingBuilder()->EngageOpenAreaBuilder()->RadiusBuilder()->SetIntent(CAM::ParamValueIntentPartUnits);
				break;
			}
			//线性延伸
			surfaceContourBuilder1->NonCuttingBuilder()->EngageOpenAreaBuilder()->LinearExtensionBuilder()->SetValue(AnnulusCavityParameterSet.ToolMotion.LinearExtension);
			switch (AnnulusCavityParameterSet.ToolMotion.LinearExtensionUnit)
			{
			case 0:
				surfaceContourBuilder1->NonCuttingBuilder()->EngageOpenAreaBuilder()->LinearExtensionBuilder()->SetIntent(CAM::ParamValueIntentToolDep);
				break;
			case 1:
				surfaceContourBuilder1->NonCuttingBuilder()->EngageOpenAreaBuilder()->LinearExtensionBuilder()->SetIntent(CAM::ParamValueIntentPartUnits);    //HeightBuilder
				break;
			}
			//圆弧角度
			surfaceContourBuilder1->NonCuttingBuilder()->EngageOpenAreaBuilder()->SetArcAngle(AnnulusCavityParameterSet.ToolMotion.CircleAngle);
		}
		else if (AnnulusCavityParameterSet.ToolMotion.ToolMotionType == 1)
		{
			surfaceContourBuilder1->NonCuttingBuilder()->EngageOpenAreaBuilder()->SetEngRetType(CAM::NcmScEngRetBuilder::EngRetTypesArcNormalToToolAxis);
			//圆弧半径
			surfaceContourBuilder1->NonCuttingBuilder()->EngageOpenAreaBuilder()->RadiusBuilder()->SetValue(AnnulusCavityParameterSet.ToolMotion.CircleRadius);
			switch (AnnulusCavityParameterSet.ToolMotion.CircleRadiusUnit)
			{
			case 0:
				surfaceContourBuilder1->NonCuttingBuilder()->EngageOpenAreaBuilder()->RadiusBuilder()->SetIntent(CAM::ParamValueIntentToolDep);
				break;
			case 1:
				surfaceContourBuilder1->NonCuttingBuilder()->EngageOpenAreaBuilder()->RadiusBuilder()->SetIntent(CAM::ParamValueIntentPartUnits);
				break;
			}
			//线性延伸
			surfaceContourBuilder1->NonCuttingBuilder()->EngageOpenAreaBuilder()->LinearExtensionBuilder()->SetValue(AnnulusCavityParameterSet.ToolMotion.LinearExtension);
			switch (AnnulusCavityParameterSet.ToolMotion.LinearExtensionUnit)
			{
			case 0:
				surfaceContourBuilder1->NonCuttingBuilder()->EngageOpenAreaBuilder()->LinearExtensionBuilder()->SetIntent(CAM::ParamValueIntentToolDep);
				break;
			case 1:
				surfaceContourBuilder1->NonCuttingBuilder()->EngageOpenAreaBuilder()->LinearExtensionBuilder()->SetIntent(CAM::ParamValueIntentPartUnits);    //HeightBuilder
				break;
			}
			//圆弧角度
			surfaceContourBuilder1->NonCuttingBuilder()->EngageOpenAreaBuilder()->SetArcAngle(AnnulusCavityParameterSet.ToolMotion.CircleAngle);
			//倾斜角度
			surfaceContourBuilder1->NonCuttingBuilder()->EngageOpenAreaBuilder()->SetRampAngle(AnnulusCavityParameterSet.ToolMotion.TiltAngle);
		}
		
		else if (AnnulusCavityParameterSet.ToolMotion.ToolMotionType == 2)
		{
			//uc1601("07", 1);//设定步距数
			surfaceContourBuilder1->NonCuttingBuilder()->EngageOpenAreaBuilder()->SetEngRetType(CAM::NcmScEngRetBuilder::EngRetTypesArcNormalToPart);
			//圆弧半径
			surfaceContourBuilder1->NonCuttingBuilder()->EngageOpenAreaBuilder()->RadiusBuilder()->SetValue(AnnulusCavityParameterSet.ToolMotion.CircleRadius);
			switch (AnnulusCavityParameterSet.ToolMotion.CircleRadiusUnit)
			{
			case 0:
				surfaceContourBuilder1->NonCuttingBuilder()->EngageOpenAreaBuilder()->RadiusBuilder()->SetIntent(CAM::ParamValueIntentToolDep);
				break;
			case 1:
				surfaceContourBuilder1->NonCuttingBuilder()->EngageOpenAreaBuilder()->RadiusBuilder()->SetIntent(CAM::ParamValueIntentPartUnits);
				break;
			}
			//线性延伸
			surfaceContourBuilder1->NonCuttingBuilder()->EngageOpenAreaBuilder()->LinearExtensionBuilder()->SetValue(AnnulusCavityParameterSet.ToolMotion.LinearExtension);
			switch (AnnulusCavityParameterSet.ToolMotion.LinearExtensionUnit)
			{
			case 0:
				surfaceContourBuilder1->NonCuttingBuilder()->EngageOpenAreaBuilder()->LinearExtensionBuilder()->SetIntent(CAM::ParamValueIntentToolDep);
				break;
			case 1:
				surfaceContourBuilder1->NonCuttingBuilder()->EngageOpenAreaBuilder()->LinearExtensionBuilder()->SetIntent(CAM::ParamValueIntentPartUnits);    //HeightBuilder
				break;
			}
			//圆弧角度
			surfaceContourBuilder1->NonCuttingBuilder()->EngageOpenAreaBuilder()->SetArcAngle(AnnulusCavityParameterSet.ToolMotion.CircleAngle);
		}
		else if (AnnulusCavityParameterSet.ToolMotion.ToolMotionType == 3)
		{
			surfaceContourBuilder1->NonCuttingBuilder()->EngageOpenAreaBuilder()->SetEngRetType(CAM::NcmScEngRetBuilder::EngRetTypesLinearNormalToPart);

			//距离
			surfaceContourBuilder1->NonCuttingBuilder()->EngageOpenAreaBuilder()->LengthBuilder()->SetValue(AnnulusCavityParameterSet.ToolMotion.ToolMotionDistance);
			switch (AnnulusCavityParameterSet.ToolMotion.ToolMotionDistanceUnit)
			{
			case 0:
				surfaceContourBuilder1->NonCuttingBuilder()->EngageOpenAreaBuilder()->LengthBuilder()->SetIntent(CAM::ParamValueIntentToolDep);
				break;
			case 1:
				surfaceContourBuilder1->NonCuttingBuilder()->EngageOpenAreaBuilder()->LengthBuilder()->SetIntent(CAM::ParamValueIntentPartUnits);
				break;
			}
			//旋转角度
			surfaceContourBuilder1->NonCuttingBuilder()->EngageOpenAreaBuilder()->SetSwingAngle(AnnulusCavityParameterSet.ToolMotion.RotationAngle);
			//倾斜角度
			surfaceContourBuilder1->NonCuttingBuilder()->EngageOpenAreaBuilder()->SetRampAngle(AnnulusCavityParameterSet.ToolMotion.TiltAngle);
		}
		//uc1601("08", 1);//设定步距数

		//公共安全设置
		if (AnnulusCavityParameterSet.SafeSetType == 0)
		{
			//uc1601("0008", 1);//设定步距数
			surfaceContourBuilder1->NonCuttingBuilder()->TransferCommonClearanceBuilder()->SetClearanceType(CAM::NcmClearanceBuilder::ClearanceTypesCylinder);
			surfaceContourBuilder1->NonCuttingBuilder()->TransferCommonClearanceBuilder()->SetRadius(380.0 + AnnulusCavityParameterSet.SafeSetOffset);
			NXOpen::Point3d iCenterPoint(0.0, 0.0, 0.0);
			NXOpen::Point *point3;
			point3 = workPart->Points()->CreatePoint(iCenterPoint);
			//uc1601("001111", 1);//设定步距数

			NXOpen::NXObject * TPointObject = (NXOpen::NXObject *) point3;
			//uc1601("000000", 1);//设定步距数
			surfaceContourBuilder1->NonCuttingBuilder()->TransferCommonClearanceBuilder()->SetPoint(TPointObject);
			//uc1601("00001", 1);//设定步距数
			Direction *direction1 = workPart->Directions()->CreateDirection(point3, _Normal);
			//uc1601("0000021", 1);//设定步距数
			surfaceContourBuilder1->NonCuttingBuilder()->TransferCommonClearanceBuilder()->SetAxis(direction1);
			//uc1601("000032", 1);//设定步距数
		}
		else if (AnnulusCavityParameterSet.SafeSetType == 1)
		{
			//uc1601("0009", 1);//设定步距数
			surfaceContourBuilder1->NonCuttingBuilder()->TransferCommonClearanceBuilder()->SetClearanceType(CAM::NcmClearanceBuilder::ClearanceTypesAutomatic);
		}
		//设置安全偏置距离
		surfaceContourBuilder1->NonCuttingBuilder()->TransferCommonClearanceBuilder()->SetSafeDistance(AnnulusCavityParameterSet.SafeSetOffset);
		//uc1601("0900000", 1);//设定步距数
		surfaceContourBuilder1->CutParameters()->BlankOverhang()->SetIntent(CAM::ParamValueIntentToolDep);
		surfaceContourBuilder1->CutParameters()->BlankOverhang()->SetValue(20.0);
		//uc1601("09", 1);//设定步距数
		//更新操作
		//NXObject *nXObject;
		nXObject = surfaceContourBuilder1->Commit();
		surfaceContourBuilder1->Destroy();
		//uc1601("090", 1);//设定步距数
		//生成刀轨
		std::vector<CAM::CAMObject *> objects1(1);
		objects1[0] = surfaceContour1;
		workPart->CAMSetup()->GenerateToolPath(objects1);

		//uc1601("生成刀轨！", 1);
		UF_UI_ONT_refresh();


	}
}

Point3d AnnulusCavityBottomMillingCmd::GetCenterPoint(Face *face)//小满——获得中心点
{
	UF_EVALSF_p_t evaluator = NULL;

	double uv_min_max[4] = { 0.0, 1.0, 0.0, 1.0 };
	UF_EVALSF_initialize_2(face->Tag(), &evaluator);
	UF_EVALSF_ask_face_uv_minmax(evaluator, uv_min_max);

	double uv_pair[2] = { 0.5 * (uv_min_max[0] + uv_min_max[1]), 0.5 * (uv_min_max[2] + uv_min_max[3]) };
	UF_MODL_SRF_VALUE_t surf_eval;
	UF_EVALSF_evaluate(evaluator, UF_MODL_EVAL_ALL, uv_pair, &surf_eval);

	Point3d origin;
	origin.X = surf_eval.srf_pos[0];
	origin.Y = surf_eval.srf_pos[1];
	origin.Z = surf_eval.srf_pos[2];

	return origin;
}

double AnnulusCavityBottomMillingCmd::DotProduct(double a[3], double b[3])
{
	double result;
	result = a[0] * b[0] + a[1] * b[1] + a[2] * b[2];
	return result;

}