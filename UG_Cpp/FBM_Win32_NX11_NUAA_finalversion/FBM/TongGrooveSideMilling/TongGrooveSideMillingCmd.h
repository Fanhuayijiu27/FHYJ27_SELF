#undef CreateDialog
#ifndef PassGrooveSideMillingCmd_H
#define PassGrooveSideMillingCmd_H

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
#include <NXOpen/CAM_CutLevelPlanar.hxx>

#include <NXOpen/Scalar.hxx>
#include <NXOpen/ScalarCollection.hxx>


#include <uf_camgeom.h>
#include <uf_cambnd.h>
#include <uf_obj.h>
#include <uf_ui.h>
#include <uf_bound.h>
#include <uf_wave.h>
#include <uf_setup.h>
#include <uf_param.h>

#include <NXOpen/Session.hxx>
#include <NXOpen/PartCollection.hxx>

#include <vector>

#include "PublicFunctions.h"
#include "FeaturePublicClass.h"
#include "ProcessPublicClass.h"

#include <NXOpen/CAM_PlanarMilling.hxx>
#include <NXOpen/CAM_PlanarMillingBuilder.hxx>
#include <NXOpen/CAM_PlanarOperationBuilder.hxx>

using namespace NXOpen;

class PassGrooveSideMillingCmd : public TaggedObject
{
public:
	PassGrooveSideMillingCmd();
	PassGrooveSideMillingCmd(std::vector<PassGroove*> ListOfPassGroove, PassGrooveSideProcessParameter ParameterSet, Part* WorkPart, Body* WorkBody, Body* WorkBlank, std::vector<Face*>ListOfSelectedFace);
	virtual ~PassGrooveSideMillingCmd();

public:
	std::vector<PassGroove*> PassGrooveResultList;
	PassGrooveSideProcessParameter PassGrooveParameterSet;
	std::vector<Face*> SelectedPassGrooveTopFaceList;
	std::vector<PassGroove*> SelectedPassGroove;
	Part* workPart;
	Body* selectedBody;
	Body* selectedBlank;
	std::vector<int> Index;
	NXOpen::Vector3d _Normal;

	void PassGrooveSideMillingDecision1(std::vector<PassGroove*> PassGrooveList);   

	Vector3d get_Centre_fr_Face(Face* ipFace);
	Point3d GetCenterPoint(Face *face);
	tag_t SelectPart();
	tag_t SelectBlank();
	double DotProduct(double a[3], double b[3]);

	UF_CAMBND_app_data_p_t create_and_init_bnd_app_data(void);
};

#endif