#undef CreateDialog
#ifndef PlanerBlindGrooveMillingCmd_H
#define PlanerBlindGrooveMillingCmd_H

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
#include <NXOpen/CAM_NcmScBuilder.hxx>
#include <NXOpen/CAM_NcmScEngRetBuilder.hxx>
#include <NXOpen/CAM_NcmTransferBuilder.hxx>
#include <NXOpen/DirectionCollection.hxx>
#include <NXOpen/PointCollection.hxx>
#include <NXOpen/CAM_NcmAvoidancePointBuilder.hxx>

#include <NXOpen/CAM_ToolAxisFixed.hxx>
#include <NXOpen/CAM_PlanarOperationBuilder.hxx>
#include <NXOpen/CAM_CutDirection.hxx>
#include <NXOpen/CAM_CutAngle.hxx>
#include <NXOpen/CAM_CutAngle.hxx>
#include <NXOpen/CAM_CutLevelPlanar.hxx>

#include <NXOpen/Session.hxx>
#include <NXOpen/PartCollection.hxx>

#include <uf_camgeom.h>
#include <uf_cambnd.h>
#include <uf_ui.h>

#include <vector>

#include "PublicFunctions.h"
#include "FeaturePublicClass.h"
#include "ProcessPublicClass.h"
#include <string>

#include <NXOpen/CAM_PlanarMilling.hxx>
#include <NXOpen/CAM_PlanarMillingBuilder.hxx>
#include <NXOpen/CAM_PlanarOperationBuilder.hxx>

using namespace NXOpen;

class PlanerBlindGrooveMillingCmd :
	public TaggedObject
{
public:
	PlanerBlindGrooveMillingCmd();
	PlanerBlindGrooveMillingCmd(std::vector<PlanerBlindGroove*> ListOfPlanerBlindGroove, PlanerBlindGrooveProcessParameter ParameterSet, Part* WorkPart, Body* WorkBody, Body* WorkBlank, std::vector<Face*>ListOfSelectedFace);
	virtual ~PlanerBlindGrooveMillingCmd();

public:
	std::vector<PlanerBlindGroove*> PlanerBlindGrooveResultList;
	PlanerBlindGrooveProcessParameter PlanerBlindGrooveParameterSet;
	Part* workPart;
	Body* selectedBody;
	Body* selectedBlank;
	std::vector<Face*> SelectedFaceList;
	NXOpen::Vector3d _Normal;  

	void PlanerBlindGrooveMillingDecision(std::vector<Face*> PlanerBlindGrooveBottomFaceList);

	Point3d GetCenterPoint(Face *face);
	tag_t SelectPart();
	tag_t SelectBlank();
	Vector3d get_Centre_fr_Face(Face* ipFace);
	double DotProduct(double a[3], double b[3]);
};
#endif