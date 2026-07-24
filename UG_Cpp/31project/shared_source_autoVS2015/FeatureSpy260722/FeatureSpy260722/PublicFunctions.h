#undef CreateDialog
#ifndef NXOpen_PublicFunctions_H_INCLUDED
#define NXOpen_PublicFunctions_H_INCLUDED
//----------------------------------------------------------------------------
// 
//
// Vertex.h
//
// Description:
//     Header definition of Vertex.
//
//----------------------------------------------------------------------------
//#include "UserMachiningPublicModule.h"

#include <NXOpen/Session.hxx>
#include <NXOpen/UI.hxx>
#include <NXOpen/TaggedObject.hxx>
#include <NXOpen/Face.hxx>
#include <NXOpen/Edge.hxx>
#include <vector>
#include <uf_modl.h>
#include <uf_modl_curves.h>
#include <uf_defs.h>
#include <uf_vec.h>
#include <NXOpen/NXObjectManager.hxx>
#include <NXOpen/MeasureFaces.hxx>
#include <NXOpen/MeasureFaceBuilder.hxx>
#include <NXOpen/MeasureManager.hxx>
#include <NXOpen/SelectDisplayableObjectList.hxx>
#include <NXOpen/Unit.hxx>
#include <NXOpen/Part.hxx>
#include <NXOpen/IParameterizedSurface.hxx>
#include <NXOpen/UnitCollection.hxx>

#include <NXOpen/GeometricAnalysis_GeometricProperties.hxx>
#include <NXOpen/GeometricAnalysis_AnalysisManager.hxx>
#include <NXOpen/SmartObject.hxx>
#include <NXOpen/Features_DatumPlaneBuilder.hxx>
#include <NXOpen/Features_FeatureCollection.hxx>
#include <NXOpen/PointCollection.hxx>
#include <NXOpen/DirectionCollection.hxx>
#include <NXOpen/Plane.hxx>
#include <NXOpen/Point.hxx>
#include <NXOpen/PlaneCollection.hxx>
#include <NXOpen/Curve.hxx>
#include <NXOpen\Curve.hxx>
#include <NXOpen\Arc.hxx>

#include <math.h>
#include <uf_obj.h>
#include <uf_eval.h>
#include <uf_modl_dfo.h>
#include <uf_sf.h>
#include <uf_ui.h>
#include <uf.h>
//#include "afx.h"
#include <uf_curve.h>
#include <uf_csys.h>
#include <uf_mtx.h>
//#include <afxtempl.h>
//#include <NXOpen/libnxopencpp_exports.hxx>
#include <time.h> 
#include <stdio.h> 
#include <iosfwd>
#include <fstream>

#include <math.h>
#include <iostream>

#include <Windows.h>
#include <WinBase.h>
#include <strsafe.h>

using namespace NXOpen;

struct DeadlineInfo
{
	static const int Deadline_year = 2018;
	static const int Deadline_month = 5;
	static const int Deadline_day = 31;
	static const int Deadline_hour = 23;
	static const int Deadline_minute = 59;
	static const int Deadline_second = 59;

	static const int Deadline_Date = 20230831;
	static const int Deadline_Time = 235959;

};

////用户自定义函数，系统公共函数
DllExport bool   get_pEdge_fr_fafa(NXOpen::Face* pFace_This, NXOpen::Face* pFace_Other, std::vector<NXOpen::Edge*>& plEdge_fafa);
DllExport int  Is_Edge_in_lpEdge(NXOpen::Edge* ipEdge, std::vector<NXOpen::Edge*> ilpEdge);


////取一个面的主法向
////若是曲面　取均匀９点　算法向的平均值
////MainNormal  　输出的值
DllExport void  get_MainNormal_fr_face(NXOpen::Face* face, NXOpen::Vector3d& MainNormal);

//获得一个平面的法矢
DllExport void get_MainNormal_fr_plane(NXOpen::Face* ipFace, NXOpen::Vector3d& MainNormal);

////get two vector3d angle
DllExport double get_Angle_Two_vector3d(NXOpen::Vector3d vector3d1, NXOpen::Vector3d vector3d2);

////***get center point on a face (this function maybe need to modify ,this can't calculate the accurate center point)
DllExport void get_Centre_fr_Face(NXOpen::Face* ipFace, NXOpen::Point3d& oCentre, NXOpen::Vector3d& iCenterNormal);

////get distance ,a point to face(Is a plane)
DllExport double get_distance_point_to_face(NXOpen::Face* ipFace, NXOpen::Point3d PointOutofFace);

DllExport void  get_Mainangle_fr_MidPt(NXOpen::Face* ipFace_This, NXOpen::Face* ipFace_Other,
	NXOpen::Edge* ipEdge, double& od_MainAngle);

DllExport int  Is_Face_in_lpFace(NXOpen::Face* ipFace, std::vector<NXOpen::Face*> ilpFace);
DllExport void ConnectFaceLists(std::vector<NXOpen::Face*> iplFace1, std::vector<NXOpen::Face*> iplFace2, std::vector<NXOpen::Face*>& oplFace);
DllExport double get_EdgeHigh_fr_Face(NXOpen::Edge* ipEdge, NXOpen::Face* ipFace);
DllExport void NormalizeVector(NXOpen::Vector3d& VectorNorm);
DllExport void Projection_3axes_to_2axes(NXOpen::Point3d iMathPoint, NXOpen::Vector3d MathVector_u,
	NXOpen::Vector3d MathVector_v, NXOpen::Point3d& oMathPoint);
DllExport bool Cover_lpPoint_NewCoordinate(NXOpen::Vector3d MathVector_u, NXOpen::Vector3d MathVector_v,
	std::vector<NXOpen::Point3d> lpMathPoint_Out,
	std::vector<NXOpen::Point3d> &lpMathPoint_Projected);
DllExport int Is_Cover_fr_edges(NXOpen::Point3d iMathPoint_StartThis, NXOpen::Point3d iMathPoint_EndThis,
	NXOpen::Point3d iMathPoint_StartOther, NXOpen::Point3d iMathPoint_EndOther,
	double id_Min);

DllExport bool get_MidMathPoint_fr_Edge(NXOpen::Edge* ipEdge, const double Midparam, NXOpen::Point3d &oMathPointMid);
DllExport double get_FaceHigh_fr_Face(NXOpen::Part* workPart, NXOpen::Face* ipFace_Each, NXOpen::Face* ipFace);
DllExport double calcArea(NXOpen::Part* workPart, NXOpen::Face* face);

DllExport void RemoveFaceVectorInFaceVector(std::vector<NXOpen::Face*> FaceVector1, std::vector<NXOpen::Face*> &FaceVector2);

//得到一个面所有的相邻面
DllExport void get_Adjacent_Faces(NXOpen::Face* ipFace, std::vector<NXOpen::Face*> &AdjacentFaces);
//得到一个面的内环相邻面
DllExport void get_Inner_Adjacent_Faces(NXOpen::Face* ipFace, std::vector<NXOpen::Face*> &InnerAdjacentFaces);

//得到一条边中点的切线
DllExport void get_Tangent_of_Centerpoint(Edge* iEdge, double &tangent);

DllExport void get_FaceAngle(Face* pFaceThis, Face* pFaceOther, Edge* pEdge, double& pMainAngle);
DllExport void get_FaceAngle_old(Face* pFaceThis, Face* pFaceOther, Edge* pEdge, double& pMainAngle);

DllExport bool Is_Have_Same_Face(std::vector<Face*> lpFace1, std::vector<Face*> lpFace2);

DllExport void get_surface_angle(NXOpen::Face* iFace, double& MaxAngle, double& Radius, NXOpen::Point3d& MidVertex, Vector3d _Normal);

//ExportedByUserMachiningPublicModule

DllExport void get_face_maxRad(Face* iFace, double & MaxFaceRad);

DllExport void get_face_maxRad2(Face* iFace, double & MaxFaceRad);

//去重
DllExport void RemoveDplicateFace(std::vector<Face*> &FacesVector);
DllExport void RemoveDplicateEdge(std::vector<Edge*> &EdgeVector);
DllExport void RemoveDplicatePoint3d(std::vector<Point3d> &Point3dVector);

//得到面列所有边和顶点
DllExport void Get_Edges_Vertexs_FaceVector(std::vector<Face*> FaceVector, std::vector<Edge*> &AllEdges, std::vector<Point3d> &AllVertexPoints);

//特征排序
DllExport void OrderingFace(std::vector<Face*> &FacesVector, Vector3d _Normal);

//得到整环数
DllExport int Get_FullCircle_Num(Face* inputFace, Vector3d _Normal);

//得到中间角度
DllExport void get_MidAngle(Face* inputFace, double& inputAngle, Vector3d _Normal, double& OutputAngle /*,NXOpen::Point3d &MidVertex*/);

//
DllExport int  Is_Num_in_lpNum(int pNum, std::vector<int> lpNum);

//得到最后有效时间
DllExport void GetDeadlineInfo(double &ddd);

//得到实时时间
DllExport void GetCurrentDate(double &CurrentInfoDouble);

//加密解密文件
DllExport double EncryptDecryptInfo(char *SecretFilePath, double currentInfo, double deadInfo);

//读写最新的使用时间
DllExport double readWriteLatestInfo(std::string NormalFilePath, double currentInfo, double deadInfo);


//----------------------------------------1222----------------------------------------------//
DllExport void get_External_Adjacent_Faces(NXOpen::Face* ipFace, std::vector<NXOpen::Face*> &ExternalAdjacentFaces);

DllExport logical IsTwoFaceSmooth(Face*EachFace, Face*OtherFace);

DllExport void GetEdge_From_TwoFaces(NXOpen::Face* EachFace, NXOpen::Face* OtherFace, std::vector<NXOpen::Edge*> &CorporateEdge);

DllExport void PutFaceVectorIntoFaceVector(std::vector<NXOpen::Face*>FaceVector1, std::vector<NXOpen::Face*>&FaceVector2);

DllExport void get_Inner_Edge(NXOpen::Face* ipFace, std::vector<NXOpen::Edge*> &Inner_EdgeVector);

//自定义一个对话框
DllExport int DifineDialogBox(char *sPromptStr);

//将分布在机匣上的孔进行排序=========进行三等分划
DllExport void OrderBodyHole(std::vector<Face*> &CylinderFaces, Face *_ApproachFace);

//特征排序

//弧形槽按高度排序
DllExport void OrderCircularGroove(std::vector<Face*> &CylinderFaces, Face *_ApproachFace);

//孔按高度排序
DllExport void OrderDirectionHole(std::vector<Face*> &CylinderFaces, Face *_ApproachFace);



DllExport bool IsCircleEdgeComplete(NXOpen::Edge* EachEdge);

DllExport void GetOneAdjacentFace(NXOpen::Edge*BeforeEdge, NXOpen::Face*BeforeFace, NXOpen::Face* &AdjacentFace);

DllExport void Get_Next_Face(NXOpen::Face *EachFace, NXOpen::Face *UselessFace, NXOpen::Face * &UsefulFace);

DllExport bool Get_RightAndWrong_Edge_FromCylinder(NXOpen::Face*EachFace, NXOpen::Edge* &RightEdge, NXOpen::Edge* &WrongEdge);
//----------------------------------------1222----------------------------------------------//

//加密解密子函数
DllExport void encryptFile(std::ifstream& inFile, std::ofstream& outFile, int key);

DllExport char encryptChar(char c, int key);

DllExport void declassifyFile(std::ifstream& inFile, std::ofstream& outFile, int key);

DllExport char declassifyChar(char decChar, int key);

DllExport void EncryptOriginalInfo();
//================AddFunction==============
DllExport int Get_FullCircle_Num1(Face* inputFace);//得到面的整圆数

												   //--------------------------------string get number------------------------------------------//
DllExport int to_int(std::string s);
//--------------------------------得到含侧面凸台底面------------------------------------------//
DllExport void GetBossBottomCornerFace(std::vector<Face*> TopFaceVector, std::vector<Face*> SideFaceVector, std::vector<Face*>& BottomCornerFaceVector, std::vector<Face*>& BottomFaceVector);
//判断该NXString是否在FeatureSpyResultDisplayStringList中-------------------------------------//
DllExport bool isNXStringrepeat(NXString string, std::vector<NXString> FeatureSpyResultDisplayStringList);

DllExport int Get_FullCircle_Num2(Face* inputFace);//针对筋找凸台侧面做约束面


#endif