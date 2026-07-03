#undef CreateDialog
#ifndef ANNULUSCAVITYBOTTOMMILLINGCMD_H
#define ANNULUSCAVITYBOTTOMMILLINGCMD_H

#include <NXOpen/TaggedObject.hxx>
#include <NXOpen/Part.hxx>
#include <NXOpen/Body.hxx>

#include <NXOpen/CAM_NCGroup.hxx>
#include <NXOpen/CAM_Method.hxx>
#include <NXOpen/CAM_Tool.hxx>
#include <NXOpen/CAM_FeatureGeometry.hxx>
#include <NXOpen/CAM_CAMSetup.hxx>
#include <NXOpen/CAM_NCGroupCollection.hxx>
#include <NXOpen/CAM_Operation.hxx>
#include <NXOpen/CAM_SurfaceContour.hxx>
#include <NXOpen/CAM_SurfaceContourBuilder.hxx>
#include <NXOpen/CAM_OperationCollection.hxx>
#include <NXOpen/CAM_SurfaceDriveGeometry.hxx>
#include <NXOpen/CAM_ToolAxisVariable.hxx>
#include <NXOpen/CAM_ToolAxisFixed.hxx>
#include <NXOpen/CAM_DmAmBuilder.hxx>
#include <NXOpen/CAM_CutAngle.hxx>
#include <NXOpen/CAM_DmSurfBuilder.hxx>
#include <NXOpen/CAM_DmcurveBuilder.hxx>
#include <NXOpen/CAM_DriveChainItemBuilderList.hxx>
#include <NXOpen/Section.hxx>
#include <NXOpen/SectionCollection.hxx>
#include <NXOpen/Curve.hxx>
#include <NXOpen/CurveCollection.hxx>
#include <NXOpen/CurveDumbRule.hxx>
#include <NXOpen/Arc.hxx>
#include <NXOpen/Session.hxx>
#include <NXOpen/Update.hxx>
#include <NXOpen/Preferences_SessionModeling.hxx>
#include <NXOpen/PartCollection.hxx>
#include <NXOpen/ScRuleFactory.hxx>
#include <NXOpen\CAM_DmSurfBuilder.hxx>

#include <NXOpen/CAM_SurfaceDriveGeometrySet.hxx>
#include <NXOpen/CAM_SurfaceDriveGeometry.hxx>
#include <NXOpen/CAM_SurfaceDriveGeometrySetList.hxx>
#include <NXOpen/CAM_Inheritable2dLength.hxx>
#include <NXOpen/CAM_MillCutParameters.hxx>
#include <NXOpen/CAM_NcmScBuilder.hxx>
#include <NXOpen/CAM_NcmTransferBuilder.hxx>
#include <NXOpen/CAM_NcmClearanceBuilder.hxx>
#include <NXOpen/CAM_CutPatternBuilder.hxx>
#include <NXOpen/CAM_StepoverBuilder.hxx>
#include <NXOpen/CAM_FeedsBuilder.hxx>
#include <NXOpen/CAM_InheritableFeedBuilder.hxx>
#include <NXOpen/CAM_InheritableDoubleBuilder.hxx>
#include <NXOpen/CAM_NcmScEngRetBuilder.hxx>
#include <NXOpen/CAM_InheritableToolDepBuilder.hxx>
#include <NXOpen/Direction.hxx>
#include <NXOpen/DirectionCollection.hxx>
#include <uf_param.h>
#include <uf_camgeom.h>
#include <uf_cambnd.h>
#include <uf_vec.h>
#include <uf_ui.h>
#include <uf_defs.h>

#include <vector>
#include <math.h>

#include "PublicFunctions.h"
#include "FeaturePublicClass.h"
#include "ProcessPublicClass.h"
#include "uf_ui_ont.h"
#include <NXOpen/TaggedObject.hxx>
#include <NXOpen/Part.hxx>
#include <NXOpen/Body.hxx>

#include <NXOpen/CAM_NCGroup.hxx>
#include <NXOpen/CAM_Method.hxx>
#include <NXOpen/CAM_Tool.hxx>
#include <NXOpen/CAM_FeatureGeometry.hxx>
#include <NXOpen/CAM_CAMSetup.hxx>
#include <NXOpen/CAM_NCGroupCollection.hxx>
#include <NXOpen/CAM_Operation.hxx>
#include <NXOpen/CAM_SurfaceContour.hxx>
#include <NXOpen/CAM_SurfaceContourBuilder.hxx>
#include <NXOpen/CAM_OperationCollection.hxx>
#include <NXOpen/CAM_DmSurfBuilder.hxx>
#include <NXOpen/CAM_ToolAxisVariable.hxx>
#include <NXOpen/CAM_ToolAxisFixed.hxx>
#include <NXOpen/CAM_DmAmBuilder.hxx>
#include <NXOpen/CAM_PlungeMillingBuilder.hxx>
#include <NXOpen/CAM_PlungeMilling.hxx>
#include <NXOpen/CAM_StepUpBuilder.hxx>
#include <NXOpen/CAM_OperationBuilder.hxx>
#include <NXOpen/CAM_CutLevel.hxx>
#include <NXOpen/CAM_CutDirection.hxx>
#include <NXOpen/CAM_CutAngle.hxx>

#include <NXOpen/CAM_Inheritable2dLength.hxx>
#include <NXOpen/CAM_MillCutParameters.hxx>
#include <NXOpen/CAM_NcmScBuilder.hxx>
#include <NXOpen/CAM_NcmClearanceBuilder.hxx>
#include <NXOpen/CAM_CutPatternBuilder.hxx>
#include <NXOpen/CAM_StepoverBuilder.hxx>
#include <NXOpen/CAM_FeedsBuilder.hxx>
#include <NXOpen/CAM_InheritableFeedBuilder.hxx>
#include <NXOpen/CAM_InheritableDoubleBuilder.hxx>
#include <NXOpen/CAM_NcmScEngRetBuilder.hxx>
#include <NXOpen/CAM_InheritableToolDepBuilder.hxx>
#include <NXOpen/Direction.hxx>
#include <NXOpen/DirectionCollection.hxx>
#include <NXOpen/Session.hxx>
#include <NXOpen/Conic.hxx>
#include <NXOpen/Arc.hxx>
#include <NXOpen/ArcCollection.hxx>
#include <NXOpen/Scalar.hxx>
#include <NXOpen/ScalarCollection.hxx>
#include <NXOpen/Xform.hxx>
#include <NXOpen/XformCollection.hxx>
#include <NXOpen/Session.hxx>
#include <NXOpen/PartCollection.hxx>
#include <NXOpen/CurveCollection.hxx>
#include <NXOpen/Curve.hxx>

#include <uf_param.h>
#include <uf_camgeom.h>
#include <uf_cambnd.h>
#include <uf_ui.h>
#include <uf_cut_levels.h>
#include <uf_curve.h>

#include <vector>
#include <string>

#include "PublicFunctions.h"
#include "FeaturePublicClass.h"
#include "ProcessPublicClass.h"


using namespace NXOpen;

class AnnulusCavityBottomMillingCmd: public TaggedObject
{
public:
	// Standard constructors and destructors
	// -------------------------------------
	AnnulusCavityBottomMillingCmd ();
	AnnulusCavityBottomMillingCmd(std::vector<AnnulusCavity*> ListOfCavity, std::vector<Boss*> ListOfBoss,
		AnnulusCavityProcessParameter ParameterSet, Part* WorkPart, Body* WorkBody,
		Body* WorkBlank, std::vector<Face*> ListOfSelectedFace);
	virtual ~AnnulusCavityBottomMillingCmd ();

public:
	std::vector<AnnulusCavity*> AnnulusCavityResultList;
	std::vector<Boss*> BossResultList;
	AnnulusCavityProcessParameter AnnulusCavityParameterSet;
	Part* workPart;
	Body* selectedBody;
	Body* selectedBlank;
	std::vector<Face*> SelectedFaceList;
	std::vector<AnnulusCavity*> SelectedCavity ;
	std::vector<int> CavityIndex ;

	NXOpen::Vector3d _Normal;

	void AnnulusCavityBottomMillingDecision();
	void AnnulusCavityBottomMillingDecision1(std::vector<AnnulusCavity*> AnnulusCavityList);
	void AnnulusCavityBottomMillOperation(NXOpen::Face* iFace, int k);    //截面交线版本
	void AnnulusCavityBottomMillOperation1(NXOpen::Face* iFace, int k);   //自动画线版本
	void AnnulusCavityBottomMillOperation2(NXOpen::Face* iFace, NXOpen::Point3d MidPoint_EachFace, int k/*, int n*/);
	void AnnulusCavityBottomMillingDecision3();
	void AnnulusCavityBottomMillingDecision4(std::vector<Face*> ListOfSelectedFace);
	Vector3d AnnulusCavityBottomMillingCmd::get_Centre_fr_Face(Face* ipFace);


	
	Point3d GetCenterPoint(Face *face);

	tag_t SelectPart();
	tag_t SelectBlank();
	double DotProduct(double a[3], double b[3]);


};

#endif
//#include"PublicFunctions.cpp"