#undef CreateDialog
#ifndef DLBOSSSIDEMILLINGCMD_H
#define DLBOSSSIDEMILLINGCMD_H

#include <NXOpen/TaggedObject.hxx>
#include <NXOpen/Part.hxx>
#include <NXOpen/Body.hxx>
#include <NXOpen/Plane.hxx>
#include <NXOpen/PlaneCollection.hxx>

#include <NXOpen/CAM_NCGroup.hxx>
#include <NXOpen/CAM_Method.hxx>
#include <NXOpen/CAM_Tool.hxx>
#include <NXOpen/CAM_FeatureGeometry.hxx>
#include <NXOpen/CAM_CAMSetup.hxx>
#include <NXOpen/CAM_NCGroupCollection.hxx>
#include <NXOpen/CAM_Operation.hxx>
#include <NXOpen/CAM_FaceMilling.hxx>
#include <NXOpen/CAM_FaceMillingBuilder.hxx>
#include <NXOpen/CAM_OperationCollection.hxx>
#include <NXOpen/CAM_GeometryCiBuilder.hxx>
#include <NXOpen/CAM_MultiBladeBaseGeometry.hxx>
#include <NXOpen/CAM_CutPatternBuilder.hxx>
#include <NXOpen/CAM_MillCutParameters.hxx>
#include <NXOpen/CAM_CornerControlBuilder.hxx>
#include <NXOpen/CAM_Inheritable2dLength.hxx>
#include <NXOpen/CAM_StepoverBuilder.hxx>
#include <NXOpen/CAM_InheritableToolDepBuilder.hxx>
#include <NXOpen/CAM_NcmClearanceBuilder.hxx>
#include <NXOpen/CAM_NcmPlanarBuilder.hxx>
#include <NXOpen/CAM_FeedsBuilder.hxx>
#include <NXOpen/CAM_InheritableFeedBuilder.hxx>
#include <NXOpen/CAM_NcmPlanarEngRetBuilder.hxx>
#include <NXOpen/CAM_ProgramOrderGroupBuilder.hxx>

#include <NXOpen/CAM_SurfaceContour.hxx>
#include <NXOpen/CAM_SurfaceContourBuilder.hxx>
#include <NXOpen/CAM_FlowBuilder.hxx>
#include <NXOpen/CAM_ToolAxisFixed.hxx>
#include <NXOpen/CAM_DmBndBuilder.hxx>
#include <NXOpen/CAM_ZLevelMilling.hxx>
#include <NXOpen/CAM_ZLevelMillingBuilder.hxx>
#include <NXOpen/CAM_CutLevel.hxx>
#include <NXOpen/CAM_CutDirection.hxx>
#include <NXOpen/CAM_ExtendAtEdges.hxx>
#include <NXOpen/CAM_PlanarOperationBuilder.hxx>
#include <NXOpen/CAM_CutAngle.hxx>
#include <NXOpen/CAM_NcmScEngRetBuilder.hxx>
#include <NXOpen/CAM_NcmTransferBuilder.hxx>
#include <NXOpen/CAM_NcmScBuilder.hxx>

#include <NXOpen/Scalar.hxx>
#include <NXOpen/ScalarCollection.hxx>

#include <NXOpen/Session.hxx>
#include <NXOpen/PartCollection.hxx>

#include <uf_camgeom.h>
#include <uf_cambnd.h>
#include <uf_obj.h>
#include <uf_ui.h>
#include <uf_bound.h>
#include <uf_wave.h>
#include <uf_setup.h>
#include <uf_param.h>

#include <vector>

#include "PublicFunctions.h"
#include "FeaturePublicClass.h"
#include "ProcessPublicClass.h"

//偏置曲线
#include <uf_defs.h>
#include <NXOpen/NXException.hxx>
#include <NXOpen/Session.hxx>
#include <NXOpen/BasePart.hxx>
#include <NXOpen/Builder.hxx>
#include <NXOpen/Direction.hxx>
#include <NXOpen/Edge.hxx>
#include <NXOpen/EdgeDumbRule.hxx>
#include <NXOpen/Expression.hxx>
#include <NXOpen/ExpressionCollection.hxx>
#include <NXOpen/Features_Extrude.hxx>
#include <NXOpen/Features_FeatureCollection.hxx>
#include <NXOpen/Features_OffsetCurveBuilder.hxx>
#include <NXOpen/GeometricUtilities_AlongSpineBuilder.hxx>
#include <NXOpen/GeometricUtilities_CurveFitData.hxx>
#include <NXOpen/GeometricUtilities_CurveFitOptions.hxx>
#include <NXOpen/GeometricUtilities_CurveOptions.hxx>
#include <NXOpen/GeometricUtilities_LawBuilder.hxx>
#include <NXOpen/GeometricUtilities_MultiTransitionLawBuilder.hxx>
#include <NXOpen/GeometricUtilities_NonInflectingLawBuilder.hxx>
#include <NXOpen/GeometricUtilities_SShapedLawBuilder.hxx>
#include <NXOpen/Line.hxx>
#include <NXOpen/NXObject.hxx>
#include <NXOpen/ObjectList.hxx>
#include <NXOpen/Part.hxx>
#include <NXOpen/PartCollection.hxx>
#include <NXOpen/Point.hxx>
#include <NXOpen/ScRuleFactory.hxx>
#include <NXOpen/Section.hxx>
#include <NXOpen/SelectLine.hxx>
#include <NXOpen/SelectObject.hxx>
#include <NXOpen/SelectionIntentRule.hxx>
#include <NXOpen/Session.hxx>
#include <NXOpen/SectionCollection.hxx>
#include <NXOpen/CAM_DriveChainItemBuilderList.hxx>
#include <NXOpen/TaggedObject.hxx>
#include <NXOpen/Unit.hxx>
#include <iostream>
#include <string>
#include <cstdio>


//设置驱动线
#include <NXOpen/CAM_NCGroup.hxx>
#include <NXOpen/CAM_Method.hxx>
#include <NXOpen/CAM_Tool.hxx>
#include <NXOpen/CAM_FeatureGeometry.hxx>
#include <NXOpen/CAM_CAMSetup.hxx>
#include <NXOpen/CAM_NCGroupCollection.hxx>
#include <NXOpen/CAM_Operation.hxx>
#include <NXOpen/CAM_FaceMilling.hxx>
#include <NXOpen/CAM_FaceMillingBuilder.hxx>
#include <NXOpen/CAM_OperationCollection.hxx>
#include <NXOpen/CAM_GeometryCiBuilder.hxx>
#include <NXOpen/CAM_MultiBladeBaseGeometry.hxx>
#include <NXOpen/CAM_CutPatternBuilder.hxx>
#include <NXOpen/CAM_MillCutParameters.hxx>
#include <NXOpen/CAM_CornerControlBuilder.hxx>
#include <NXOpen/CAM_Inheritable2dLength.hxx>
#include <NXOpen/CAM_StepoverBuilder.hxx>
#include <NXOpen/CAM_InheritableToolDepBuilder.hxx>
#include <NXOpen/CAM_NcmClearanceBuilder.hxx>
#include <NXOpen/CAM_NcmPlanarBuilder.hxx>
#include <NXOpen/CAM_FeedsBuilder.hxx>
#include <NXOpen/CAM_InheritableFeedBuilder.hxx>
#include <NXOpen/CAM_NcmPlanarEngRetBuilder.hxx>
#include <NXOpen/CAM_ProgramOrderGroupBuilder.hxx>
#include <NXOpen/CAM_NcmScBuilder.hxx>
#include <NXOpen/CAM_NcmScEngRetBuilder.hxx>
#include <NXOpen/CAM_NcmTransferBuilder.hxx>
#include <NXOpen/DirectionCollection.hxx>
#include <NXOpen/PointCollection.hxx>
#include <NXOpen/CAM_NcmAvoidancePointBuilder.hxx>
#include <NXOpen/CAM_SurfaceContourBuilder.hxx>
#include <NXOpen/CAM_SurfaceContour.hxx>
#include <NXOpen/CAM_DmBndBuilder.hxx>
#include <NXOpen/CAM_DriveChainItemBuilderList.hxx>
#include <NXOpen/CAM_DmcurveBuilder.hxx>
#include <NXOpen/Section.hxx>
#include <NXOpen/SectionCollection.hxx>

#include <uf_defs.h>
#include <NXOpen/NXException.hxx>
#include <NXOpen/Session.hxx>
#include <NXOpen/Annotations_Dimension.hxx>
#include <NXOpen/BasePart.hxx>
#include <NXOpen/Builder.hxx>
#include <NXOpen/CurveCollection.hxx>
#include <NXOpen/DatumAxis.hxx>
#include <NXOpen/DatumCollection.hxx>
#include <NXOpen/Edge.hxx>
#include <NXOpen/Expression.hxx>
#include <NXOpen/Features_EdgeBlend.hxx>
#include <NXOpen/Features_FeatureCollection.hxx>
#include <NXOpen/Features_PatternFace.hxx>
#include <NXOpen/InferSnapType.hxx>
#include <NXOpen/Line.hxx>
#include <NXOpen/ModelingView.hxx>
#include <NXOpen/ModelingViewCollection.hxx>
#include <NXOpen/NXObject.hxx>
#include <NXOpen/Part.hxx>
#include <NXOpen/PartCollection.hxx>
#include <NXOpen/Preferences_SessionPreferences.hxx>
#include <NXOpen/Preferences_SessionSketch.hxx>
#include <NXOpen/SelectNXObject.hxx>
#include <NXOpen/SelectNXObjectList.hxx>
#include <NXOpen/SelectObject.hxx>
#include <NXOpen/SelectObjectList.hxx>
#include <NXOpen/Session.hxx>
#include <NXOpen/Sketch.hxx>
#include <NXOpen/SketchCollection.hxx>
#include <NXOpen/SketchConstraintBuilder.hxx>
#include <NXOpen/SketchDimensionalConstraint.hxx>
#include <NXOpen/SketchGeometricConstraint.hxx>
#include <NXOpen/SketchHelpedDimensionalConstraint.hxx>
#include <NXOpen/SketchHelpedGeometricConstraint.hxx>
#include <NXOpen/TaggedObject.hxx>
#include <NXOpen/View.hxx>

#include <NXOpen/Line.hxx>
#include <NXOpen/LineCollection.hxx>

using namespace NXOpen;

class DLBossSideMillingCmd : public TaggedObject
{
public:
	DLBossSideMillingCmd();
	DLBossSideMillingCmd(std::vector<DoubleLayerBoss*> ListOfNBoss, BossSideProcessParameter ParameterSet, Part* WorkPart, Body* WorkBody, Body* WorkBlank, std::vector<Face*>ListOfSelectedFace, std::vector<Face*>ListOfSelectedZhiDingFace);
	virtual ~DLBossSideMillingCmd();

public:
	std::vector<DoubleLayerBoss*> BossResultList;
	BossSideProcessParameter BossParameterSet;
	std::vector<Face*> SelectedBossTopFaceList;
	std::vector<Face*> SelectedZhiDingFaceList;
	std::vector<DoubleLayerBoss*> SelectedBoss;
	Part* workPart;
	Body* selectedBody;
	Body* selectedBlank;
	std::vector<int> Index;
	NXOpen::Vector3d _Normal;
  
	void BossSideMillingDecision1(std::vector<Face*> BossTopFaceList, std::vector<Face*> SelectedZhiDingFaceList);

	Point3d GetCenterPoint(Face *face);
	tag_t SelectPart();
	tag_t SelectBlank();
	Vector3d get_Centre_fr_Face(Face* ipFace);
	double DotProduct(double a[3], double b[3]);

};

#endif
