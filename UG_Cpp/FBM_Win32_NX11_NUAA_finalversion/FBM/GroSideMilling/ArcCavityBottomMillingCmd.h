#undef CreateDialog
#ifndef ARCCAVITYBOTTOMMILLINGCMD_H
#define ARCCAVITYBOTTOMMILLINGCMD_H

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
#include <NXOpen\CAM_DmSurfBuilder.hxx>
#include <NXOpen/CAM_ToolAxisVariable.hxx>
#include <NXOpen/CAM_ToolAxisFixed.hxx>
#include <NXOpen/CAM_DmAmBuilder.hxx>
#include <NXOpen/CAM_CutAngle.hxx>
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


//#include <NXOpen/CAM_SurfaceDriveGeometrySet.hxx>
//#include <NXOpen/CAM_SurfaceDriveGeometry.hxx>
//#include <NXOpen/CAM_SurfaceDriveGeometrySetList.hxx>
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
#include <NXOpen/CAM_NcmTransferBuilder.hxx>
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

using namespace NXOpen;

class ArcCavityBottomMillingCmd: public TaggedObject
{
public:
	// Standard constructors and destructors
	// -------------------------------------
	ArcCavityBottomMillingCmd ();
	ArcCavityBottomMillingCmd (std::vector<ArcCavity*> ListOfCavity, /*std::vector<Boss*> ListOfBoss,*/ ArcCavityProcessParameter ParameterSet, Part* WorkPart, Body* WorkBody , std::vector<Face*> ListOfSelectedFace);
	virtual ~ArcCavityBottomMillingCmd ();

public:
	std::vector<ArcCavity*> ArcCavityResultList;
	//std::vector<Boss*> BossResultList;
	ArcCavityProcessParameter ArcCavityParameterSet;
	Part* workPart;
	Body* selectedBody;
	std::vector<Face*> SelectedFaceList;
	std::vector<ArcCavity*> SelectedArcCavity ;
	std::vector<int> ArcCavityIndex ;
	NXOpen::Vector3d _Normal;

	void ArcCavityBottomMillingDecision();
	void ArcCavityBottomMillingDecision1(std::vector<ArcCavity*> ArcCavityList);
	void ArcCavityBottomMillOperation(NXOpen::Face* iFace, int k);    //截面交线版本
	void ArcCavityBottomMillOperation1(NXOpen::Face* iFace, int k);   //自动画线版本

	void ArcCavityBottomMillOperation2(NXOpen::Face* iFace, int k);   //来自下陷槽

};

#endif