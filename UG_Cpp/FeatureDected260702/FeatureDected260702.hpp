//==============================================================================
//  FeatureDected260702.hpp
//  Feature Recognition Dialog for Casing Parts
//
//  Based on FBM_Win32_NX11_NUAA_finalversion project.
//  Strictly follows original data structures, recognition algorithms,
//  and NX Open API usage patterns.
//
//  Features: Boss / Groove (4 types) / Hole (3 types)
//            Highlight display, external file export
//
//  NX Block Styler generated, NX 12
//==============================================================================

#ifndef FEATUREDECTED260702_H_INCLUDED
#define FEATUREDECTED260702_H_INCLUDED

//------------------------------------------------------------------------------
// NX Block Styler required headers
//------------------------------------------------------------------------------
#include <uf_defs.h>
#include <uf_ui_types.h>
#include <iostream>
#include <vector>
#include <math.h>
#include <fstream>
#include <string>
#include <time.h>

#include <NXOpen/Session.hxx>
#include <NXOpen/UI.hxx>
#include <NXOpen/NXMessageBox.hxx>
#include <NXOpen/Callback.hxx>
#include <NXOpen/NXException.hxx>
#include <NXOpen/BlockStyler_UIBlock.hxx>
#include <NXOpen/BlockStyler_BlockDialog.hxx>
#include <NXOpen/BlockStyler_PropertyList.hxx>
#include <NXOpen/BlockStyler_Group.hxx>
#include <NXOpen/BlockStyler_SelectObject.hxx>
#include <NXOpen/BlockStyler_Enumeration.hxx>
#include <NXOpen/BlockStyler_Button.hxx>
#include <NXOpen/BlockStyler_MultilineString.hxx>

// NX geometry object headers
#include <NXOpen/Face.hxx>
#include <NXOpen/Edge.hxx>
#include <NXOpen/Body.hxx>
#include <NXOpen/Part.hxx>
#include <NXOpen/PartCollection.hxx>
#include <NXOpen/NXObjectManager.hxx>
#include <NXOpen/TaggedObject.hxx>
#include <NXOpen/MeasureFaces.hxx>
#include <NXOpen/MeasureFaceBuilder.hxx>
#include <NXOpen/MeasureManager.hxx>
#include <NXOpen/Unit.hxx>
#include <NXOpen/UnitCollection.hxx>

// UF (User Function) API headers
#include <uf.h>
#include <uf_modl.h>
#include <uf_ui.h>
#include <uf_obj.h>
#include <uf_disp.h>
#include <uf_eval.h>
#include <uf_vec.h>
#include <uf_curve.h>
#include <uf_csys.h>
#include <uf_mtx.h>
#include <uf_sf.h>

//------------------------------------------------------------------------------
// Namespaces
//------------------------------------------------------------------------------
using namespace std;
using namespace NXOpen;
using namespace NXOpen::BlockStyler;

//------------------------------------------------------------------------------
// PI constant
//------------------------------------------------------------------------------
#ifndef PI
#define PI 3.14159265358979323846
#endif

//=============================================================================
// Feature Data Structures
// Strictly follow FeaturePublicClass.h from FBM project
//=============================================================================

// ---- Boss features ----
struct NormalBoss
{
    std::vector<NXOpen::Face*> TopFaceVector;
    std::vector<NXOpen::Face*> SideFaceVector;
    std::vector<NXOpen::Face*> BottomCornerFaceVector;
    std::vector<NXOpen::Face*> BottomFaceVector;
    bool OutOrIn;
    bool ifselected;
    char* name;
};

struct NoSideBoss
{
    std::vector<NXOpen::Face*> TopFaceVector;
    std::vector<NXOpen::Face*> BottomCornerFaceVector;
    std::vector<NXOpen::Face*> BottomFaceVector;
    bool OutOrIn;
    bool ifselected;
    char* name;
};

struct DoubleLayerBoss
{
    std::vector<NXOpen::Face*> TopFaceVector;
    std::vector<NXOpen::Face*> SideFaceVector;
    std::vector<NXOpen::Face*> NTopFaceVector;
    std::vector<NXOpen::Face*> NSideFaceVector;
    std::vector<NXOpen::Face*> ConnectFaceVector;
    std::vector<NXOpen::Face*> BottomCornerFaceVector;
    std::vector<NXOpen::Face*> BottomFaceVector;
    bool OutOrIn;
    bool ifselected;
    char* name;
};

// ---- Groove features ----
struct CircularGroove
{
    std::vector<NXOpen::Face*> TopFaceVector;
    std::vector<NXOpen::Face*> BottomCornerFaceVector;
    std::vector<NXOpen::Face*> LimitFaceVector;
    bool ifselected;
};

struct SubsidenceGroove
{
    std::vector<NXOpen::Face*> TopFaceVector;
    std::vector<NXOpen::Face*> BottomCornerFaceVector;
    bool ifselected;
    int deter;
};

struct PlanerBlindGroove
{
    std::vector<NXOpen::Face*> BottomFaceVector;
    std::vector<NXOpen::Face*> BottomCornerFaceVector;
    std::vector<NXOpen::Face*> SideFaceVector;
    bool ifselected;
    char* name;
};

struct PassGroove
{
    std::vector<NXOpen::Face*> TopFaceVector;
    std::vector<NXOpen::Face*> RightFaceVector;
    std::vector<NXOpen::Face*> SideFaceVector;
    std::vector<NXOpen::Face*> BottomFaceVector;
    bool ifselected;
    char* name;
};

// ---- Hole features ----
struct DirectionNormalHole
{
    std::vector<NXOpen::Face*> TopFaceVector;
    std::vector<NXOpen::Face*> InWallVector;
    std::vector<NXOpen::Face*> BottomFaceVector;
    char* name;
};

struct DirectionRightHole
{
    std::vector<NXOpen::Face*> TopFaceVector;
    std::vector<NXOpen::Face*> RightVector;
    std::vector<NXOpen::Face*> InWallVector;
    std::vector<NXOpen::Face*> BottomFaceVector;
    char* name;
};

struct DirectionBlindHole
{
    std::vector<NXOpen::Face*> TopFaceVector;
    std::vector<NXOpen::Face*> InWallVector;
    std::vector<NXOpen::Face*> BottomFaceVector;
    char* name;
};

//=============================================================================
// FeatureDected260702 Dialog Class
//=============================================================================
class DllExport FeatureDected260702
{
public:
    static Session *theSession;
    static UI *theUI;
    FeatureDected260702();
    ~FeatureDected260702();
    int Show();

    // BlockStyler callbacks
    void initialize_cb();
    void dialogShown_cb();
    int apply_cb();
    int ok_cb();
    int update_cb(NXOpen::BlockStyler::UIBlock* block);
    PropertyList* GetBlockProperties(const char *blockID);

private:
    const char* theDlxFileName;
    NXOpen::BlockStyler::BlockDialog* theDialog;

    // ---- UI controls ----
    NXOpen::BlockStyler::Group* group;
    NXOpen::BlockStyler::SelectObject* selection0;
    NXOpen::BlockStyler::SelectObject* selection01;
    NXOpen::BlockStyler::Group* group0;
    NXOpen::BlockStyler::Enumeration* enum0;
    NXOpen::BlockStyler::Button* button0;
    NXOpen::BlockStyler::Group* group1;
    NXOpen::BlockStyler::MultilineString* multiline_string0;
    NXOpen::BlockStyler::Button* button01;
    NXOpen::BlockStyler::Group* group2;
    NXOpen::BlockStyler::Enumeration* enum01;
    NXOpen::BlockStyler::Button* button02;

    // ---- Selection state ----
    NXOpen::Body* pSelectedBody;
    NXOpen::Face* pSelectedFace;
    NXOpen::Vector3d _Approach_Normal;
    std::vector<NXOpen::Face*> AllFaces;

    // ---- Boss feature results ----
    std::vector<NormalBoss*> Feature_NormalBoss;
    std::vector<NoSideBoss*> Feature_NoSideBoss;
    std::vector<DoubleLayerBoss*> Feature_DoubleLayerBoss;

    // ---- Groove feature results ----
    std::vector<CircularGroove*> Feature_CircularGroove0;
    std::vector<CircularGroove*> Feature_CircularGroove1;
    std::vector<SubsidenceGroove*> Feature_SubsidenceGroove;
    std::vector<PlanerBlindGroove*> Feature_PlanerBlindGroove;
    std::vector<PassGroove*> Feature_PassGroove;

    // ---- Hole feature results ----
    std::vector<DirectionNormalHole*> Feature_DirectionNormalHole;
    std::vector<DirectionRightHole*> Feature_DirectionRightHole;
    std::vector<DirectionBlindHole*> Feature_DirectionBlindHole;

    // ---- Plane classification ----
    NXOpen::Face* TopPlaneFace;
    NXOpen::Face* BottomPlaneFace;
    std::vector<NXOpen::Face*> ParallelPlaneFace;

    // ---- Recognition state ----
    bool hasRecognized;

    //=========================================================================
    // Utility Functions
    // Strictly follow PublicFunctions.cpp from FBM project
    //=========================================================================

    void get_MainNormal_fr_plane(NXOpen::Face* ipFace, NXOpen::Vector3d& MainNormal);

    double get_Angle_Two_vector3d(NXOpen::Vector3d v1, NXOpen::Vector3d v2);

    void get_Adjacent_Faces(NXOpen::Face* ipFace, std::vector<NXOpen::Face*>& AdjacentFaces);

    void get_External_Adjacent_Faces(NXOpen::Face* ipFace, std::vector<NXOpen::Face*>& ExternalAdjacentFaces);

    void get_Inner_Adjacent_Faces(NXOpen::Face* ipFace, std::vector<NXOpen::Face*>& InnerAdjacentFaces);

    int Is_Face_in_lpFace(NXOpen::Face* ipFace, std::vector<NXOpen::Face*> ilpFace);

    void RemoveFaceVectorInFaceVector(std::vector<NXOpen::Face*> FaceVector1,
                                       std::vector<NXOpen::Face*>& FaceVector2);

    void PutFaceVectorIntoFaceVector(std::vector<NXOpen::Face*> FaceVector1,
                                      std::vector<NXOpen::Face*>& FaceVector2);

    logical IsTwoFaceSmooth(NXOpen::Face* EachFace, NXOpen::Face* OtherFace);

    void GetEdge_From_TwoFaces(NXOpen::Face* EachFace, NXOpen::Face* OtherFace,
                                std::vector<NXOpen::Edge*>& Corporate_EdgeVector);

    void get_FaceAngle(NXOpen::Face* iFaceThis, NXOpen::Face* iFaceOther,
                        NXOpen::Edge* iEdge, double& pMainAngle);

    double calcArea(NXOpen::Part* workPart, NXOpen::Face* face);

    void get_face_maxRad(NXOpen::Face* iFace, double& MaxFaceRad);
    void get_face_maxRad2(NXOpen::Face* iFace, double& MaxFaceRad);

    int Get_FullCircle_Num(NXOpen::Face* inputFace, NXOpen::Vector3d _Normal);
    int Get_FullCircle_Num1(NXOpen::Face* inputFace);
    int Get_FullCircle_Num2(NXOpen::Face* inputFace);

    void get_Inner_Edge(NXOpen::Face* ipFace, std::vector<NXOpen::Edge*>& Inner_EdgeVector);

    bool IsCircleEdgeComplete(NXOpen::Edge* EachEdge);

    void GetOneAdjacentFace(NXOpen::Edge* BeforeEdge, NXOpen::Face* BeforeFace,
                             NXOpen::Face*& AdjacentFace);

    void Get_Next_Face(NXOpen::Face* EachFace, NXOpen::Face* UselessFace,
                        NXOpen::Face*& UsefulFace);

    bool Get_RightAndWrong_Edge_FromCylinder(NXOpen::Face* EachFace,
                                              NXOpen::Edge*& RightEdge,
                                              NXOpen::Edge*& WrongEdge);

    void GetBossBottomCornerFace(std::vector<NXOpen::Face*> TopFaceVector,
                                  std::vector<NXOpen::Face*> SideFaceVector,
                                  std::vector<NXOpen::Face*>& BottomCornerFaceVector,
                                  std::vector<NXOpen::Face*>& BottomFaceVector);

    void GetBossBottomFace(std::vector<NXOpen::Face*> TopFaceVector,
                            std::vector<NXOpen::Face*> SideFaceVector,
                            std::vector<NXOpen::Face*> BottomCornerFaceVector,
                            std::vector<NXOpen::Face*>& BottomFaceVector);

    double get_Two_Point_Distance(double p1[3], double p2[3]);

    void RemoveDplicateFace(std::vector<NXOpen::Face*>& FacesVector);

    void OrderingFace(std::vector<NXOpen::Face*>& FacesVector, NXOpen::Vector3d _Normal);
    void OrderDirectionHole(std::vector<NXOpen::Face*>& CylinderFaces, NXOpen::Face* _ApproachFace);
    void OrderBodyHole(std::vector<NXOpen::Face*>& CylinderFaces, NXOpen::Face* _ApproachFace);
    void OrderCircularGroove(std::vector<NXOpen::Face*>& CylinderFaces, NXOpen::Face* _ApproachFace);
    void OrderSingleFeature(std::vector<NXOpen::Face*>& FacesVector);

    //=========================================================================
    // Feature Recognition Functions
    // Strictly follow SpyFunctions.cpp from FBM project
    //=========================================================================

    // ---- Boss recognition ----
    void BossRecognitionAndCreate(std::vector<NXOpen::Face*> AllFaces,
                                   std::vector<NXOpen::Face*> SealFace_NormalBoss,
                                   std::vector<NormalBoss*>& Feature_NormalBoss,
                                   std::vector<NXOpen::Face*> SealFace_NoSideBoss,
                                   std::vector<NoSideBoss*>& Feature_NoSideBoss,
                                   std::vector<NXOpen::Face*> SealFace_DoubleLayerBoss,
                                   std::vector<DoubleLayerBoss*>& Feature_DoubleLayerBoss,
                                   NXOpen::Vector3d _Approach_Normal,
                                   NXOpen::Part* workPart);

    void DistinguishBossOutOrIn(std::vector<NormalBoss*>& Feature_NormalBoss,
                                 std::vector<NoSideBoss*>& Feature_NoSideBoss,
                                 std::vector<DoubleLayerBoss*>& Feature_DoubleLayerBoss,
                                 double _Approach_Point[3]);

    // ---- Groove recognition ----
    void CircularGrooveRecognition(std::vector<NXOpen::Face*> AllFaces,
                                    NXOpen::Face* Begin_Face, NXOpen::Part* workPart,
                                    std::vector<NXOpen::Face*>& SealFace_CircularGroove0,
                                    std::vector<NXOpen::Face*>& SealFace_CircularGroove1,
                                    NXOpen::Face* _ApproachFace);
    void CreateCircularGrooveFeature(NXOpen::Face* Begin_Face,
                                      std::vector<NXOpen::Face*> SealFace_CircularGroove0,
                                      std::vector<NXOpen::Face*> SealFace_CircularGroove1,
                                      std::vector<CircularGroove*>& Feature_CircularGroove0,
                                      std::vector<CircularGroove*>& Feature_CircularGroove1);

    void SubsidenceGrooveRecognition(std::vector<NXOpen::Face*> AllFaces,
                                      NXOpen::Face* BottomPlaneFace,
                                      NXOpen::Face* TopPlaneFace,
                                      std::vector<NXOpen::Face*> ParallelPlaneFace,
                                      std::vector<NXOpen::Face*>& SealFace_SubsidenceGroove,
                                      std::vector<NXOpen::Face*>& SealFace_HookFaceRib);
    void CreateSubsidenceGrooveFeature(std::vector<NXOpen::Face*> SealFace_SubsidenceGroove,
                                        std::vector<SubsidenceGroove*>& Feature_SubsidenceGroove,
                                        NXOpen::Face* _ApproachFace);

    void PlanerBlindGrooveRecognition(std::vector<NXOpen::Face*> AllFaces,
                                       NXOpen::Face* BottomPlaneFace,
                                       NXOpen::Face* TopPlaneFace,
                                       std::vector<NXOpen::Face*> ParallelPlaneFace,
                                       std::vector<NXOpen::Face*>& SealFace_PlanerBlindGroove);
    void CreatePlanerBlindGrooveFeature(std::vector<NXOpen::Face*> SealFace_PlanerBlindGroove,
                                         std::vector<PlanerBlindGroove*>& Feature_PlanerBlindGroove);

    void PassGrooveRecognition(std::vector<NXOpen::Face*> AllFaces,
                                std::vector<NXOpen::Face*>& SealFace_PassGroove);
    void CreatePassGrooveFeature(std::vector<NXOpen::Face*> SealFace_PassGroove,
                                  std::vector<PassGroove*>& Feature_PassGroove);

    // ---- Hole recognition ----
    void DirectionMillRecognition(std::vector<NXOpen::Face*> AllFaces,
                                   NXOpen::Face* BeginFace,
                                   NXOpen::Face* BottomPlaneFace,
                                   NXOpen::Face* TopPlaneFace,
                                   std::vector<NXOpen::Face*> ParallelPlaneFace,
                                   std::vector<NXOpen::Face*>& LeftSealHole_FaceVector,
                                   std::vector<NXOpen::Face*>& SealFace_DirectionNormalHole,
                                   std::vector<NXOpen::Face*>& SealFace_DirectionRightHole,
                                   std::vector<NXOpen::Face*>& SealFace_DirectionBlindHole);
    void CreateDirectionMillFeature(NXOpen::Face* BottomPlaneFace,
                                     NXOpen::Face* TopPlaneFace,
                                     std::vector<NXOpen::Face*> ParallelPlaneFace,
                                     std::vector<NXOpen::Face*> SealFace_DirectionNormalHole,
                                     std::vector<NXOpen::Face*> SealFace_DirectionRightHole,
                                     std::vector<NXOpen::Face*> SealFace_DirectionBlindHole,
                                     std::vector<DirectionNormalHole*>& Feature_DirectionNormalHole,
                                     std::vector<DirectionRightHole*>& Feature_DirectionRightHole,
                                     std::vector<DirectionBlindHole*>& Feature_DirectionBlindHole);

    //=========================================================================
    // Helper Functions
    //=========================================================================

    void AnalyzePlanes(NXOpen::Face* pPartingFace,
                        NXOpen::Face*& outTopFace,
                        NXOpen::Face*& outBottomFace,
                        std::vector<NXOpen::Face*>& outParallelFaces);

    void ClearAllFeatures();
    void HighlightFeatures(const std::string& featureType);
    void UnhighlightAll();
    void ExportResultsToFile();
    void UpdateResultDisplay();
};

#endif // FEATUREDECTED260702_H_INCLUDED
