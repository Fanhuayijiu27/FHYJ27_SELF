#undef CreateDialog
#ifndef ROUGHPLUNGINGCMD_H
#define ROUGHPLUNGINGCMD_H

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

class RoughPlungingCmd: public TaggedObject
{
public:
	// Standard constructors and destructors
	// -------------------------------------
	RoughPlungingCmd ();
	RoughPlungingCmd (std::vector<CircularGroove*> ListOfCavity, std::vector<SubsidenceCavity*> ListOfSubsidenceCavity, std::vector<ArcCavity*> ListOfArcCavity, 
		std::vector<Boss*> ListOfBoss, std::vector<Rib*> ListOfRib, RoughPlungeProcessParameter ParameterSet, Part* WorkPart, Body* WorkBody, Body* WorkBlank, std::vector<Face*> ListOfSelectedFace
					);
	virtual ~RoughPlungingCmd ();

public:
	std::vector<CircularGroove*> CircularGrooveResultList;
	std::vector<SubsidenceCavity*> SubsidenceCavityResultList;
	std::vector<ArcCavity*> ArcCavityResultList;                
	std::vector<Rib*> RibResultList;                            
	std::vector<Boss*> BossResultList;                          

	RoughPlungeProcessParameter RoughPlungeParameterSet;
	Part* workPart;
	Body* selectedBody;
	Body* selectedBlank;
	std::vector<Face*> SelectedFaceList;
	tag_t partTag;
	tag_t blankTag;
	NXString _Geomtry;

	std::vector<CircularGroove*> SelectedCavity;
	std::vector<SubsidenceCavity*> SelectedSubsidenceCavity;
	std::vector<ArcCavity*> SelectedArcCavity;                
	std::vector<Rib*> SelectedRib; 

	double PlungingMaxAngle;
	NXOpen::Vector3d _Normal;

	void ACRoughPlungingDecision(std::vector<CircularGroove*> ListOfCavity);
	void SCRoughPlungingDecision(std::vector<SubsidenceCavity*> ListOfSubsidenceCavity);
	void ArcCRoughPlungingDecision(std::vector<ArcCavity*> ListOfArcCavity);
	void RTRoughPlungingDecision(std::vector<Rib*> ListOfRib);
	
	void RoughPlungingOperation(std::vector<NXOpen::Face*> iFaceList, NXOpen::Direction *iDirection, NXString iOperationName, double PointArray[15]);

	void GeneralRoughPlungingDecision(std::vector<Face*> ListOfSelectedFace);
	Vector3d get_Centre_fr_Face(Face* ipFace);
	Point3d GetCenterPoint(Face *face);

	double DotProduct(double a[3], double b[3]);

	static UF_CAMBND_app_data_p_t create_and_init_bnd_app_data(void);

};

#endif