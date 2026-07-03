/*****************************************************************************
**
** PublicFunctions.cpp
**
** Description:
**     This Create the PublicFunctions.
**
*****************************************************************************/

#include "PublicFunctions.h"
#include <NXOpen/TaggedObject.hxx>
#include <NXOpen/Edge.hxx>
#include <NXOpen/Face.hxx>
#include <vector>
#include <NXOpen/NXObjectManager.hxx>

bool   get_pEdge_fr_fafa(NXOpen::Face* pFace_This, NXOpen::Face* pFace_Other,
	std::vector<NXOpen::Edge*>& plEdge_fafa)//判断该平面是否有邻接面
{
	if (pFace_This->Tag() == pFace_Other->Tag())
		return false;

	plEdge_fafa.clear();
	std::vector<NXOpen::Edge*> plEdge_ThisEach;
	std::vector<NXOpen::Edge*>  plEdge_OtherEach;
	plEdge_ThisEach = pFace_This->GetEdges();//存入当前平面的所有边
	plEdge_OtherEach = pFace_Other->GetEdges();//存入相邻平面的所有边

	for (int i = 0; i<plEdge_ThisEach.size(); i++) //循环1
	{
		NXOpen::Edge* pEdge_Each = plEdge_ThisEach[i];
		tag_t Order_EachThis = pEdge_Each->Tag();	//当前平面中一个边的tag	 
		for (int j = 0; j<plEdge_OtherEach.size(); j++) //循环2
		{
			NXOpen::Edge* pEdge_Other = plEdge_OtherEach[j];//相邻平面中一个边
			tag_t Order_EachOther = pEdge_Other->Tag();//相邻平面中一个边的tag
			if (Order_EachThis == Order_EachOther)
			{
				int order = Is_Edge_in_lpEdge(pEdge_Other, plEdge_fafa);
				if (order == -1)
				{
					plEdge_fafa.push_back(pEdge_Other);
				}
			}
		}
	}
	if (plEdge_fafa.size()>0)
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}

}

int  Is_Edge_in_lpEdge(NXOpen::Edge* ipEdge, std::vector<NXOpen::Edge*> ilpEdge)
{
	int num_Edges = ilpEdge.size();
	tag_t Order1 = ipEdge->Tag();
	int i, j, k;
	i = j = k = 0;
	for (i = 0; i<num_Edges; i++)
	{
		NXOpen::Edge* pEdge = ilpEdge[i];
		tag_t Order2 = pEdge->Tag();
		if (Order1 == Order2)
		{
			return i;
		}

	}
	return -1;
}

//////////////////////////////////////////////////////////////////////////
////获得一个面的主法矢，此处采用在面上离散出9个点，得到这9个点的法矢的平均值
//////////////////////////////////////////////////////////////////////////
void  get_MainNormal_fr_face(NXOpen::Face* ipFace, NXOpen::Vector3d& MainNormal)
{
	tag_t facetag = ipFace->Tag();
	double param[2];  //parameter(u,v)on face
	double point[3];  //Point at parameter 
	double u1[3];     //First derivative in U
	double v1[3];     //First derivative in V
	double u2[3];     //Second derivative in U
	double v2[3];     //Second derivative in V
	double unit_norm[3];  //Unit face normal 
	double radii[2];  //Principal radii of curvature 
	logical issmoothness;
	char msg[132];
	double uv_min_max[4];  //u,v min and max

	UF_MODL_ask_face_smoothness(facetag, &issmoothness);

	UF_MODL_ask_face_uv_minmax(facetag, uv_min_max);

	double normalX, normalY, normalZ;
	normalX = normalY = normalZ = 0;

	double ustep = (uv_min_max[1] - uv_min_max[0]) / 4;
	double vstep = (uv_min_max[3] - uv_min_max[2]) / 4;

	for (int i = 1; i <= 3; i++)
	{
		param[0] = uv_min_max[0] + i*ustep;

		for (int j = 1; j <= 3; j++)
		{
			param[1] = uv_min_max[2] + j*vstep;

			UF_MODL_ask_face_props(facetag, param, point, u1, v1, u2, v2, unit_norm, radii);

			normalX = normalX + unit_norm[0] / 9;
			normalY = normalY + unit_norm[1] / 9;
			normalZ = normalZ + unit_norm[2] / 9;
		}
	}
	NXOpen::Vector3d vectormid(normalX, normalY, normalZ);
	MainNormal = vectormid;
}

//获得一个平面的法矢
void get_MainNormal_fr_plane(NXOpen::Face* ipFace, NXOpen::Vector3d& MainNormal)
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

	NXOpen::Vector3d unitnorm(unit_norm[0], unit_norm[1], unit_norm[2]);

	MainNormal = unitnorm;
};


//////////////////////////////////////////////////////////////////////////
////获得一个向量到一个向量的角度，弧度制
//////////////////////////////////////////////////////////////////////////
double get_Angle_Two_vector3d(NXOpen::Vector3d vector3d1, NXOpen::Vector3d vector3d2)
{
	double X1 = vector3d1.X;
	double Y1 = vector3d1.Y;
	double Z1 = vector3d1.Z;
	double X2 = vector3d2.X;
	double Y2 = vector3d2.Y;
	double Z2 = vector3d2.Z;
	/*if (X1 == -0.0000)
	{
	X1 = 0.000;
	}*/

	double AA = X1*X2 + Y1*Y2 + Z1*Z2;
	double B1 = sqrt(X1*X1 + Y1*Y1 + Z1*Z1);
	double B2 = sqrt(X2*X2 + Y2*Y2 + Z2*Z2);
	double BB = B1*B2;

	double cos_value = AA / BB;

	if (cos_value < 1.00001 && cos_value > 0.99999)
	{
		cos_value = 1;
	}

	double angle = acos(cos_value);

	if (angle < 0.000000001)
	{
		angle = 0;
	}

	return angle;
}

//获取中心点和中心点法矢，实体默认法矢向外
void get_Centre_fr_Face(NXOpen::Face* ipFace, NXOpen::Point3d& oCentre, NXOpen::Vector3d& iCenterNormal)
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

	NXOpen::Point3d centerpoint(point[0], point[1], point[2]);
	NXOpen::Vector3d centernormal(unit_norm[0], unit_norm[1], unit_norm[2]);

	oCentre = centerpoint;
	iCenterNormal = centernormal;
}

double get_distance_point_to_face(NXOpen::Face* ipFace, NXOpen::Point3d PointOutofFace)
//获得点point1到平面的距离,返回值为点point1到平面的距离.有正负之分。
{
	tag_t plane_tag = ipFace->Tag();

	int face1_type;
	double face1_point[3];
	double face1_dir[3];
	double face1_box[6];
	double face1_radius;
	double face1_rad_data;
	int face1_norm_dir;
	UF_MODL_ask_face_data(plane_tag, &face1_type, face1_point, face1_dir,
		face1_box, &face1_radius, &face1_rad_data, &face1_norm_dir);
	double point1[3];
	point1[0] = PointOutofFace.X;
	point1[1] = PointOutofFace.Y;
	point1[2] = PointOutofFace.Z;

	double fenzi = fabs((face1_dir[0] * point1[0] + face1_dir[1] * point1[1] + face1_dir[2] * point1[2]) - (face1_dir[0] * face1_point[0] + face1_dir[1] * face1_point[1] + face1_dir[2] * face1_point[2]));
	double fenmu = sqrt(face1_dir[0] * face1_dir[0] + face1_dir[1] * face1_dir[1] + face1_dir[2] * face1_dir[2]);
	double d = fenzi / fenmu;
	double distance = d;
	return distance;
}

//////////////////////////////////////////////////////////////////////////
////根据交线中点，计算两个面的夹角
////ipFace_This ， ipFace_Other 两个面  类型 CATFace*
////ipEdge 交线        类型 CATEdge*
////ipVolume 体积      类型 CATVolume* 
////od_MainAngle 输出的夹角    类型 double&
//////////////////////////////////////////////////////////////////////////
void  get_Mainangle_fr_MidPt(NXOpen::Face* ipFace_This, NXOpen::Face* ipFace_Other, NXOpen::Edge* ipEdge, double& od_MainAngle)
{
	////同一个面,不算角度
	if (ipFace_This->Tag() == ipFace_Other->Tag())
	{
		od_MainAngle = 0;
	}

	NXOpen::Point3d pPoint_Start;
	NXOpen::Point3d pPoint_End;
	ipEdge->GetVertices(&pPoint_Start, &pPoint_End);

	tag_t Edge_tag = ipEdge->Tag();
	tag_t Curve_tag;
	UF_MODL_create_curve_from_edge(Edge_tag, &Curve_tag);

	double param_start;
	double ref_point_start[3];
	ref_point_start[0] = pPoint_Start.X;
	ref_point_start[1] = pPoint_Start.Y;
	ref_point_start[2] = pPoint_Start.Z;
	UF_MODL_ask_curve_parm_no_ext(Curve_tag, ref_point_start, &param_start);
	double StartParam = param_start;

	double param_end;
	double ref_point_end[3];
	ref_point_end[0] = pPoint_End.X;
	ref_point_end[1] = pPoint_End.Y;
	ref_point_end[2] = pPoint_End.Z;
	UF_MODL_ask_curve_parm_no_ext(Curve_tag, ref_point_end, &param_end);
	double EndParam = param_end;

	double MidParam = (StartParam + EndParam) / 2;

	//double parm;
	double point[3];
	double tangent[3];
	double p_norm[3];
	double b_norm[3];
	double torsion;
	double rad_of_cur;

	UF_MODL_ask_curve_props(Curve_tag, MidParam, point, tangent, p_norm, b_norm, &torsion, &rad_of_cur);

	tag_t facetag_This = ipFace_This->Tag();
	double faceparam_this[2];
	double outpoint_this[3];
	UF_MODL_ask_face_parm(facetag_This, point, faceparam_this, outpoint_this);

	tag_t facetag_Other = ipFace_Other->Tag();
	double faceparam_other[2];
	double outpoint_other[3];
	UF_MODL_ask_face_parm(facetag_Other, point, faceparam_other, outpoint_other);

	double point_this[3];
	double u1_this[3];
	double v1_this[3];
	double u2_this[3];
	double v2_this[3];
	double unit_norm_this[3];
	double radii_this[3];
	UF_MODL_ask_face_props(facetag_This, faceparam_this, point_this, u1_this, v1_this, u2_this, v2_this, unit_norm_this, radii_this);

	double point_other[3];
	double u1_other[3];
	double v1_other[3];
	double u2_other[3];
	double v2_other[3];
	double unit_norm_other[3];
	double radii_other[3];
	UF_MODL_ask_face_props(facetag_Other, faceparam_other, point_other, u1_other, v1_other, u2_other, v2_other, unit_norm_other, radii_other);

	NXOpen::Vector3d Normal_point_this(unit_norm_this[0], unit_norm_this[1], unit_norm_this[2]);
	NXOpen::Vector3d Normal_point_other(unit_norm_other[0], unit_norm_other[1], unit_norm_other[2]);

	//此处的角度计算可能存在问题，需要注意***********************暂时采用这种方法
	double angle_Edge = get_Angle_Two_vector3d(Normal_point_this, Normal_point_other);
	od_MainAngle = angle_Edge;

	//////计算两个法向量相对切向量的角度
	//double angle_Edge = MathVector_InThisFace.GetAngleTo(MathVector_InOtherFace,MathVector_Tangent);

	//////将法向量的夹角换算成面的夹角
	//angle_Edge = angle_Edge + 3.14;
	//while (angle_Edge>6.28) 
	//{
	//	angle_Edge = angle_Edge - 6.28;
	//}
	//od_MainAngle = angle_Edge;	
	//return TRUE;

	UF_OBJ_set_blank_status(Curve_tag, UF_OBJ_BLANKED);

}

//////////////////////////////////////////////////////////////////////////
////判断一个面是否在一个面的列表中
//////////////////////////////////////////////////////////////////////////
int  Is_Face_in_lpFace(NXOpen::Face* ipFace, std::vector<NXOpen::Face*> ilpFace)
{
	if (ipFace == NULL)
	{
		return -1;
	}
	int num_Faces = ilpFace.size();
	tag_t Order1 = ipFace->Tag();
	int i = 0;
	for (i = 0; i<num_Faces; i++)
	{
		NXOpen::Face* pFace = ilpFace[i];
		tag_t Order2 = pFace->Tag();
		if (Order1 == Order2)
		{
			return i;
		}

	}
	return -1;
}

bool Is_Have_Same_Face(std::vector<Face*> lpFace1, std::vector<Face*> lpFace2)
{
	for (std::vector<Face*>::size_type i = 0; i != lpFace1.size(); i++)
	{
		Face* EachlpFace1 = lpFace1[i];

		if (Is_Face_in_lpFace(EachlpFace1, lpFace2) != -1) return true;
	}

	for (std::vector<Face*>::size_type i = 0; i != lpFace2.size(); i++)
	{
		Face* EachlpFace2 = lpFace2[i];

		if (Is_Face_in_lpFace(EachlpFace2, lpFace1) != -1) return true;
	}
	return false;
}

//////////////////////////////////////////////////////////////////////////
////结合两个面列表中的所有面，并放在一个列表中
//////////////////////////////////////////////////////////////////////////
void ConnectFaceLists(std::vector<NXOpen::Face*> iplFace1, std::vector<NXOpen::Face*> iplFace2, std::vector<NXOpen::Face*>& oplFace)
{
	std::vector<NXOpen::Face*> iplFaceOut;
	int numlpFace1 = iplFace1.size();
	int numlpFace2 = iplFace2.size();

	for (int i = 0; i<numlpFace1; i++)
	{
		NXOpen::Face* pFace = iplFace1[i];
		if (Is_Face_in_lpFace(pFace, iplFaceOut) == -1)
			iplFaceOut.push_back(pFace);
	}
	for (int i = 0; i<numlpFace2; i++)
	{
		NXOpen::Face*pFace = iplFace2[i];
		if (Is_Face_in_lpFace(pFace, iplFaceOut) == -1)
			iplFaceOut.push_back(pFace);
	}
	oplFace.clear();
	oplFace = iplFaceOut;
}

//////////////////////////////////////////////////////////////////////////
////得到一个边的高度,相对于分型面
//////////////////////////////////////////////////////////////////////////
double get_EdgeHigh_fr_Face(NXOpen::Edge* ipEdge, NXOpen::Face* ipFace)
{
	tag_t EdgeTag_Each = ipEdge->Tag();
	double point1[3];
	double point2[3];
	int vertex_count;
	double MidPoint[3];

	UF_MODL_ask_edge_verts(EdgeTag_Each, point1, point2, &vertex_count);
	if (vertex_count<1)
	{
		return FALSE;
	}

	if (vertex_count == 1)
	{
		MidPoint[0] = point1[0];
		MidPoint[1] = point1[1];
		MidPoint[2] = point1[2];
	}
	else
	{
		MidPoint[0] = (point1[0] + point2[0]) / 2;
		MidPoint[1] = (point1[1] + point2[1]) / 2;
		MidPoint[2] = (point1[2] + point2[2]) / 2;
	}

	NXOpen::Point3d MidPoint_EachEdge(MidPoint[0], MidPoint[1], MidPoint[2]);
	double d_high = get_distance_point_to_face(ipFace, MidPoint_EachEdge);
	return d_high;
}
//////////////////////////////////////////////////////////////////////////
////将三维的坐标点投影为二维坐标点，原点不变
////iMathPoint 原三为坐标点
////MathVector_u 目标坐标系的u向
////MathVector_v 目标坐标系的v向
////oMathPoint 转换后的二维点 注意该点只有前两个坐标值有效。
//////////////////////////////////////////////////////////////////////////
void Projection_3axes_to_2axes(NXOpen::Point3d iMathPoint, NXOpen::Vector3d MathVector_u,
	NXOpen::Vector3d MathVector_v, NXOpen::Point3d& oMathPoint)
{
	NormalizeVector(MathVector_u);
	NormalizeVector(MathVector_v);
	double u = iMathPoint.X*MathVector_u.X
		+ iMathPoint.Y*MathVector_u.Y
		+ iMathPoint.Z*MathVector_u.Z;
	double v = iMathPoint.X*MathVector_v.X
		+ iMathPoint.Y*MathVector_v.Y
		+ iMathPoint.Z*MathVector_v.Z;
	NXOpen::Point3d Mathpoint(u, v, 0);
	oMathPoint = Mathpoint;
}
//////////////////////////////////////////////////////////////////////////
////标准化或单位化一个三维矢量
//////////////////////////////////////////////////////////////////////////
void NormalizeVector(NXOpen::Vector3d& VectorNorm)
{
	double vec[3];
	double tolerance = 0.01;
	double magnitude;
	double unit_vec[3];

	vec[0] = VectorNorm.X;
	vec[1] = VectorNorm.Y;
	vec[2] = VectorNorm.Z;

	UF_VEC3_unitize(vec, tolerance, &magnitude, unit_vec);

	NXOpen::Vector3d NormalizedVec(unit_vec[0], unit_vec[1], unit_vec[2]);
	VectorNorm = NormalizedVec;
}

bool Cover_lpPoint_NewCoordinate(NXOpen::Vector3d MathVector_u, NXOpen::Vector3d MathVector_v,
	std::vector<NXOpen::Point3d> lpMathPoint_Out,
	std::vector<NXOpen::Point3d> &lpMathPoint_Projected)
{
	for (int i = 1; i <= lpMathPoint_Out.size(); i++)
	{
		NXOpen::Point3d MathPoint_Each = lpMathPoint_Out[i];
		NXOpen::Point3d MathPoint_Projected;
		Projection_3axes_to_2axes(MathPoint_Each, MathVector_v, MathVector_u, MathPoint_Projected);
		lpMathPoint_Projected.push_back(MathPoint_Projected);
	}
	if (lpMathPoint_Projected.size() <= 0) return FALSE;
	if (lpMathPoint_Projected.size() != lpMathPoint_Out.size()) return FALSE;
	return TRUE;
}

//////////////////////////////////////////////////////////////////////////
////判断两个同平面的直线是否有覆盖关系
////iMathPoint_StartThis，iMathPoint_EndThis第一条直线的首末点
////iMathPoint_StartOther，iMathPoint_EndOther第二条直线的首末点
////关系有数种：
////－1，两条线相离 0，两条线相互覆盖  1,前者完全在后者中 
////2,后者完全在前者中 3，两条线一部分相互覆盖;4,在容差范围内覆盖
//////////////////////////////////////////////////////////////////////////
int Is_Cover_fr_edges(NXOpen::Point3d iMathPoint_StartThis, NXOpen::Point3d iMathPoint_EndThis,
	NXOpen::Point3d iMathPoint_StartOther, NXOpen::Point3d iMathPoint_EndOther,
	double id_Min)
{
	std::vector<double> lD_Each;
	double dStartThis = iMathPoint_StartThis.X;
	double dEndThis = iMathPoint_EndThis.X;
	double dStartOther = iMathPoint_StartOther.X;
	double dEndOther = iMathPoint_EndOther.X;

	int tag = id_Min / fabs(id_Min);
	if (fabs(id_Min)>fabs((dStartThis - dEndThis) / 5))
		id_Min = tag*fabs(dStartThis - dEndThis) / 10;
	if (fabs(id_Min)>fabs((dStartOther - dEndOther) / 5))
		id_Min = tag*fabs(dStartOther - dEndOther) / 10;

	if (dStartThis>dEndThis)
	{
		double dEach = dStartThis;
		dStartThis = dEndThis;
		dEndThis = dEach;
	}

	if (dStartOther>dEndOther)
	{
		double dEach = dStartOther;
		dStartOther = dEndOther;
		dEndOther = dEach;
	}


	lD_Each.push_back(dStartThis - id_Min);
	lD_Each.push_back(dEndThis + id_Min);
	lD_Each.push_back(dStartOther - id_Min);
	lD_Each.push_back(dEndOther + id_Min);
	std::vector<int> Order_Each;
	Order_Each.push_back(1);
	Order_Each.push_back(2);
	Order_Each.push_back(3);
	Order_Each.push_back(4);
	for (int i = 0; i<3; i++)
	{
		double dThis = lD_Each[i];
		for (int j = i + 1; j <= 4; j++)
		{
			double dOther = lD_Each[j];
			if (dThis>dOther)
			{
				lD_Each[j] = dThis;
				lD_Each[i] = dOther;
				int Order_Mid = Order_Each[i];
				Order_Each[i] = Order_Each[j];
				Order_Each[j] = Order_Mid;
			}
		}
	}
	std::vector<double> Order_EdgeEach;
	for (int i = 0; i<4; i++) Order_EdgeEach.push_back(int((Order_Each[i] + 1) / 2));

	if (Order_EdgeEach[1] == Order_EdgeEach[4]) ////包含关系
		if (Order_EdgeEach[1] == 1) return 1;////前者完全在后者中
		else return 2;////后者完全在前者中

		if (Order_EdgeEach[1] == Order_EdgeEach[2]) return -1;
		return 3;
}

bool get_MidMathPoint_fr_Edge(NXOpen::Edge* ipEdge, const double Midparam, NXOpen::Point3d &oMathPointMid)
{
	/*double pPoint_Start[3];
	double pPoint_End[3];
	int Vertex_Count;*/
	tag_t Curve_tag = ipEdge->Tag();

	double parm = Midparam;
	double point[3];
	double tangent[3];
	double p_norm[3];
	double b_norm[3];
	double torsion;
	double rad_of_cur;

	UF_MODL_ask_curve_props(Curve_tag, parm, point, tangent, p_norm, b_norm, &torsion, &rad_of_cur);

	NXOpen::Point3d MathPoint(point[0], point[1], point[2]);
	oMathPointMid = MathPoint;
	return TRUE;
}

////得到一个边的高度,相对于分型面
double get_FaceHigh_fr_Face(NXOpen::Part* workPart, NXOpen::Face* ipFace_Each, NXOpen::Face* ipFace)
{
	NXOpen::Point3d MidPoint_EachFace;
	NXOpen::Vector3d MidNormal_EachFace;
	get_Centre_fr_Face(ipFace_Each, MidPoint_EachFace, MidNormal_EachFace);
	double d_high = get_distance_point_to_face(ipFace, MidPoint_EachFace);
	return d_high;
}

/////求面积
double calcArea(NXOpen::Part* workPart, NXOpen::Face* face)
{

	NXOpen::Unit *unit1(dynamic_cast<NXOpen::Unit *>(workPart->UnitCollection()->FindObject("SquareMilliMeter")));
	NXOpen::Unit *unit2(dynamic_cast<NXOpen::Unit *>(workPart->UnitCollection()->FindObject("MilliMeter")));
	std::vector<NXOpen::IParameterizedSurface *> objects1(1);
	objects1[0] = face;
	NXOpen::MeasureFaces *measureFaces1;
	if (face->Tag())
	{
		measureFaces1 = workPart->MeasureManager()->NewFaceProperties(unit1, unit2, 0.99, objects1);

		double AreaResult = measureFaces1->Area();
		delete measureFaces1;
		return AreaResult;
	}
	else return -1;


}

void RemoveFaceVectorInFaceVector(std::vector<NXOpen::Face*> FaceVector1, std::vector<NXOpen::Face*> &FaceVector2)
{
	for (std::vector<NXOpen::Face*>::size_type i = 0; i != FaceVector1.size(); i++)
	{
		NXOpen::Face* pFace_Each1 = FaceVector1[i];
		for (std::vector<NXOpen::Face*>::size_type j = 0; j != FaceVector2.size(); j++)
		{
			NXOpen::Face* pFace_Each2 = FaceVector2[j];
			if (pFace_Each1->Tag() == pFace_Each2->Tag())
			{
				std::vector<NXOpen::Face*>::iterator iter = FaceVector2.begin();
				iter += j;
				FaceVector2.erase(iter);
				break;
			}
		}
	}
}

void get_Adjacent_Faces(NXOpen::Face* ipFace, std::vector<NXOpen::Face*> &AdjacentFaces)//得到ipFace的相邻面
{

	std::vector<NXOpen::Edge*> pFace_Each_Edge = ipFace->GetEdges();

	for (std::vector<NXOpen::Edge*>::size_type i = 0; i != pFace_Each_Edge.size(); i++)
	{
		NXOpen::Edge* pEdge_Each = pFace_Each_Edge[i];
		std::vector<NXOpen::Face*> pEdge_Each_Faces = pEdge_Each->GetFaces();
		for (std::vector<NXOpen::Face*>::size_type j = 0; j != pEdge_Each_Faces.size(); j++)
		{
			NXOpen::Face* pFace_Each = pEdge_Each_Faces[j];

			if (pFace_Each->Tag() != ipFace->Tag())
				AdjacentFaces.push_back(pFace_Each);
		}
	}
}

//得到一条边中点的切线
void get_Tangent_of_Centerpoint(Edge* iEdge, double centerpoint[3], double tangent[3])
{
	tag_t iEdge_tag = iEdge->Tag();
	Point3d Vertex1;
	Point3d Vertex2;
	iEdge->GetVertices(&Vertex1, &Vertex2);
	double Vertex1Point[3] = { Vertex1.X, Vertex1.Y, Vertex1.Z };
	double Vertex2Point[3] = { Vertex2.X, Vertex2.Y, Vertex2.Z };

	/*int CurveDirection;
	UF_MODL_get_curve_edge_direction(Vertex1Point, iEdge_tag, &CurveDirection);
	if (CurveDirection == UF_MODL_CURVE_START_FROM_BEGIN)
	{
	for (int i = 0; i < 3; i++)
	{
	StartPoint[i] = Vertex1Point[i];
	EndPoint[i] = Vertex2Point[i];
	}
	}
	else
	{
	for (int i = 0; i < 3; i++)
	{
	EndPoint[i] = Vertex1Point[i];
	StartPoint[i] = Vertex2Point[i];
	}
	}*/

	double uMin, uMax;
	tag_t iCurve_tag;
	UF_MODL_create_curve_from_edge(iEdge_tag, &iCurve_tag);
	UF_MODL_ask_curve_parm_no_ext(iCurve_tag, Vertex1Point, &uMin);
	UF_MODL_ask_curve_parm_no_ext(iCurve_tag, Vertex2Point, &uMax);

	double uMid = (uMin + uMax) / 2;

	double pnorm[3];
	double bnorm[3];
	double torsion;
	double rad_of_cur;

	UF_MODL_ask_curve_props(iCurve_tag, uMid, centerpoint, tangent, pnorm, bnorm, &torsion, &rad_of_cur);

	UF_OBJ_delete_object(iCurve_tag);
}

void get_FaceAngle(Face* iFaceThis, Face* iFaceOther, Edge* iEdge, double& pMainAngle)
{
	int deleteFlag = -1;             //删除对象的标签值
	double curve_Threshold = 100;    //判断交点是否多等分的阈值
	bool Acurve_Flag = false;
	bool Bcurve_Flag = false;        //是否重新找点判断判断凹凸
	double OffsetDisMin = 1;

	//过交线中点作法平面
	double tangent_iEdge[3];
	double centerpoint_iEdge[3];
	get_Tangent_of_Centerpoint(iEdge, centerpoint_iEdge, tangent_iEdge);

	tag_t iDatumPlane_tag;
	UF_MODL_create_plane(centerpoint_iEdge, tangent_iEdge, &iDatumPlane_tag);
	UF_OBJ_set_blank_status(iDatumPlane_tag, UF_OBJ_BLANKED);


	//=============1===============求法平面与A面的交线
	UF_MODL_intersect_info_p_t* intersectInfo;
	int NumIntersections = -1;
	int InterSectionFlag = UF_MODL_intersect_objects(iFaceThis->Tag(), iDatumPlane_tag, 0, &NumIntersections, &intersectInfo);
	if (InterSectionFlag != 0)
	{
		deleteFlag = UF_OBJ_delete_object(iDatumPlane_tag);
		return;
	}

	Curve* ThisCurve;
	bool FlagThisCurve = false;
	for (int j = 1; j <= NumIntersections; j++)
	{
		if ((*intersectInfo)->intersect_type != UF_MODL_INTERSECT_CURVE)	continue;

		UF_MODL_intersect_info_intersect_t intersect = (*intersectInfo)->intersect;
		UF_MODL_intersect_info_curve_t curve = intersect.curve;
		tag_t intersectCurveTag = curve.identifier;
		UF_OBJ_set_blank_status(intersectCurveTag, UF_OBJ_BLANKED);

		if (!!FlagThisCurve)
		{
			//若 已经找到该点
			UF_OBJ_delete_object(intersectCurveTag);
			intersectInfo++;
			continue;
		}

		NXObjectManager intersectCurveManager;
		NXOpen::TaggedObject* pTaggedObject_IntersectCurve = intersectCurveManager.Get(intersectCurveTag);
		Curve* jIntersectCurve = dynamic_cast<Curve*>(pTaggedObject_IntersectCurve);
		UF_OBJ_set_blank_status(jIntersectCurve->Tag(), UF_OBJ_BLANKED);

		UF_EVAL_p_t evaluator;
		UF_EVAL_initialize(jIntersectCurve->Tag(), &evaluator);
		double limits[2];
		UF_EVAL_ask_limits(evaluator, limits);
		int n_derivatives = 2;
		double derivatives[6];

		double curvePoint[3];
		UF_EVAL_evaluate(evaluator, n_derivatives, limits[0], curvePoint, derivatives);
		double curvePoint2[3];
		UF_EVAL_evaluate(evaluator, n_derivatives, limits[1], curvePoint2, derivatives);


		double ErrorPoint = abs(curvePoint[0] - centerpoint_iEdge[0]) + abs(curvePoint[1] - centerpoint_iEdge[1]) + abs(curvePoint[2] - centerpoint_iEdge[2]);
		double ErrorPoint2 = abs(curvePoint2[0] - centerpoint_iEdge[0]) + abs(curvePoint2[1] - centerpoint_iEdge[1]) + abs(curvePoint2[2] - centerpoint_iEdge[2]);
		if (ErrorPoint < 0.03 || ErrorPoint2 < 0.03)
		{
			ThisCurve = jIntersectCurve;
			FlagThisCurve = true;

			//for (int k = j+1; k <= NumIntersections; k ++)
			//{
			//	intersectInfo++;
			//	if ((*intersectInfo)->intersect_type != UF_MODL_INTERSECT_CURVE)	continue;
			//	UF_MODL_intersect_info_intersect_t intersect = (*intersectInfo)->intersect;
			//	UF_MODL_intersect_info_curve_t curve = intersect.curve;
			//	tag_t intersectCurveTag = curve.identifier;
			//	UF_OBJ_delete_object(jIntersectCurve->Tag());
			//}

			intersectInfo++;
			continue;
		}
		UF_OBJ_delete_object(intersectCurveTag);
		UF_OBJ_delete_object(jIntersectCurve->Tag());
		intersectInfo++;
	}
	if (!FlagThisCurve)
	{
		deleteFlag = UF_OBJ_delete_object(iDatumPlane_tag);
		//deleteFlag = UF_OBJ_delete_object(ThisCurve->Tag());
		return;
	}


	//=============2===============求法平面与B面的交线
	InterSectionFlag = UF_MODL_intersect_objects(iFaceOther->Tag(), iDatumPlane_tag, 0, &NumIntersections, &intersectInfo);
	if (InterSectionFlag != 0)
	{
		deleteFlag = UF_OBJ_delete_object(iDatumPlane_tag);
		return;
	}

	Curve* OtherCurve;
	bool FlagOtherCurve = false;
	for (int j = 1; j <= NumIntersections; j++)
	{
		if ((*intersectInfo)->intersect_type != UF_MODL_INTERSECT_CURVE)	continue;

		UF_MODL_intersect_info_intersect_t intersect = (*intersectInfo)->intersect;
		UF_MODL_intersect_info_curve_t curve = intersect.curve;
		tag_t intersectCurveTag = curve.identifier;
		UF_OBJ_set_blank_status(intersectCurveTag, UF_OBJ_BLANKED);

		if (!!FlagOtherCurve)
		{
			//若 已经找到该点
			UF_OBJ_delete_object(intersectCurveTag);
			intersectInfo++;
			continue;
		}

		NXObjectManager intersectCurveManager;
		NXOpen::TaggedObject* pTaggedObject_IntersectCurve = intersectCurveManager.Get(intersectCurveTag);
		Curve* jIntersectCurve = dynamic_cast<Curve*>(pTaggedObject_IntersectCurve);
		UF_OBJ_set_blank_status(jIntersectCurve->Tag(), UF_OBJ_BLANKED);
		int err_code;
		UF_CURVE_line_p_t tempLine;
		tempLine = (UF_CURVE_line_p_t)UF_allocate_memory(sizeof(UF_CURVE_line_t), &err_code);
		int FlagLineData = UF_CURVE_ask_line_data(jIntersectCurve->Tag(), tempLine);
		if (FlagLineData == 0)//交线是直线
		{
			NXOpen::Point3d AStartPoint;
			NXOpen::Point3d AEndPoint;
			AStartPoint.X = tempLine->start_point[0];
			AStartPoint.Y = tempLine->start_point[1];
			AStartPoint.Z = tempLine->start_point[2];
			AEndPoint.X = tempLine->end_point[0];
			AEndPoint.Y = tempLine->end_point[1];
			AEndPoint.Z = tempLine->end_point[2];
			double ErrorStart = abs(AStartPoint.X - centerpoint_iEdge[0]) + abs(AStartPoint.Y - centerpoint_iEdge[1]) + abs(AStartPoint.Z - centerpoint_iEdge[2]);
			double ErrorEnd = abs(AEndPoint.X - centerpoint_iEdge[0]) + abs(AEndPoint.Y - centerpoint_iEdge[1]) + abs(AEndPoint.Z - centerpoint_iEdge[2]);
			if (ErrorStart < 0.03 || ErrorEnd < 0.03)
			{
				OtherCurve = jIntersectCurve;
				FlagOtherCurve = true;

				//for (int k = j+1; k <= NumIntersections; k ++)
				//{
				//	intersectInfo++;
				//	if ((*intersectInfo)->intersect_type != UF_MODL_INTERSECT_CURVE)	continue;

				//	UF_MODL_intersect_info_intersect_t intersect = (*intersectInfo)->intersect;
				//	UF_MODL_intersect_info_curve_t curve = intersect.curve;
				//	tag_t intersectCurveTag = curve.identifier;
				//	UF_OBJ_delete_object(jIntersectCurve->Tag());
				//}

				intersectInfo++;
				continue;
			}
			//intersectInfo++;
			//UF_OBJ_delete_object(intersectCurveTag);
			//UF_OBJ_delete_object(jIntersectCurve->Tag());			
		}
		else //交线是 非直线
		{
			UF_EVAL_p_t evaluator;
			UF_EVAL_initialize(jIntersectCurve->Tag(), &evaluator);
			double limits[2];
			UF_EVAL_ask_limits(evaluator, limits);
			int n_derivatives = 2;
			double derivatives[6];

			double curvePoint[3];
			UF_EVAL_evaluate(evaluator, n_derivatives, limits[0], curvePoint, derivatives);
			double curvePoint2[3];
			UF_EVAL_evaluate(evaluator, n_derivatives, limits[1], curvePoint2, derivatives);

			double ErrorPoint = abs(curvePoint[0] - centerpoint_iEdge[0]) + abs(curvePoint[1] - centerpoint_iEdge[1]) + abs(curvePoint[2] - centerpoint_iEdge[2]);
			double ErrorPoint2 = abs(curvePoint2[0] - centerpoint_iEdge[0]) + abs(curvePoint2[1] - centerpoint_iEdge[1]) + abs(curvePoint2[2] - centerpoint_iEdge[2]);
			if (ErrorPoint < 0.03 || ErrorPoint2 < 0.03)
			{
				OtherCurve = jIntersectCurve;
				FlagOtherCurve = true;

				//for (int k = j+1; k <= NumIntersections; k ++)
				//{
				//	intersectInfo++;
				//	if ((*intersectInfo)->intersect_type != UF_MODL_INTERSECT_CURVE)	continue;

				//	UF_MODL_intersect_info_intersect_t intersect = (*intersectInfo)->intersect;
				//	UF_MODL_intersect_info_curve_t curve = intersect.curve;
				//	tag_t intersectCurveTag = curve.identifier;
				//	UF_OBJ_delete_object(jIntersectCurve->Tag());
				//}

				intersectInfo++;
				continue;
			}
		}
		UF_OBJ_delete_object(intersectCurveTag);
		UF_OBJ_delete_object(jIntersectCurve->Tag());
		intersectInfo++;
	}
	if (!FlagOtherCurve)
	{
		deleteFlag = UF_OBJ_delete_object(iDatumPlane_tag);
		//deleteFlag = UF_OBJ_delete_object(OtherCurve->Tag());
		return;
	}//UF_free(intersectInfo);
	deleteFlag = UF_OBJ_delete_object(iDatumPlane_tag);


	//===========3============求法平面与A面交线的中点(用于求角度) AThisPoint，若交线过长需得到多等分后的“中点”
	double AThisPoint[3];
	//double new_point_parameter;
	//OffsetDisMin = ThisCurve->GetLength()/2;
	//int newPointFlag = UF_MODL_ask_point_along_curve_2(centerpoint_iEdge, ThisCurve->Tag(), OffsetDisMin, 1, 0.001, AThisPoint, &new_point_parameter);
	//if (newPointFlag != 0) return;

	////方法二，暂时注释
	//	double Para(0);
	//	double curPoint[3];
	//	int FlagUnlineData = UF_MODL_ask_curve_parm(ThisCurve->Tag(),centerpoint_iEdge,&Para,curPoint);
	//	if (FlagUnlineData != 0)	return;
	//
	//	double tangent[3]; 
	//	double p_norm[3];
	//	double b_norm[3];
	//	double torsion;
	//	double rad_of_cur;
	//	UF_MODL_ask_curve_props(ThisCurve->Tag(),Para+0.5,AThisPoint,tangent,p_norm,b_norm,&torsion,&rad_of_cur);
	////方法二，暂时注释


	//方法三
	UF_EVAL_p_t evaluator;
	UF_EVAL_initialize(ThisCurve->Tag(), &evaluator);
	double limits[2];
	UF_EVAL_ask_limits(evaluator, limits);
	double parm = (limits[1] + limits[0]) / 2;
	int n_derivatives = 2;
	double derivatives[6];
	int EvaFlag = UF_EVAL_evaluate(evaluator, n_derivatives, parm, AThisPoint, derivatives);
	if (EvaFlag != 0)
	{
		deleteFlag = UF_OBJ_delete_object(ThisCurve->Tag());
		return;
	}

	// 得到名义上靠近交线的“起点”
	double derivatives_StartEnd[6];
	double StartPoint[3];
	UF_EVAL_evaluate(evaluator, n_derivatives, limits[0], StartPoint, derivatives_StartEnd);
	double EndPoint[3];
	UF_EVAL_evaluate(evaluator, n_derivatives, limits[1], EndPoint, derivatives_StartEnd);

	//double param_start;  
	//UF_MODL_ask_curve_parm_no_ext(ThisCurve->Tag(),centerpoint_iEdge,&param_start);
	//double abs0 = abs(param_start - limits[0]);
	//double abs1 = abs(param_start - limits[1]);
	//if (abs0 > 0.01 && abs1 < 0.01)
	//{
	//	uc1601("A, 1", 1);
	//}
	//if (abs0 < 0.01 && abs1 > 0.01)
	//{
	//	uc1601("A,  0", 1);
	//}

	double absStart = abs(centerpoint_iEdge[0] - StartPoint[0]) + abs(centerpoint_iEdge[1] - StartPoint[1]) + abs(centerpoint_iEdge[2] - StartPoint[2]);
	double absEnd = abs(centerpoint_iEdge[0] - EndPoint[0]) + abs(centerpoint_iEdge[1] - EndPoint[1]) + abs(centerpoint_iEdge[2] - EndPoint[2]);

	double start_param_evaluate = limits[0];
	double end_param_evaluater = limits[1];
	if (absStart > 0.01 && absEnd < 0.01)
	{
		//uc1601("E, 1", 1);
		start_param_evaluate = limits[1];
		end_param_evaluater = limits[0];
	}


	// 若交线太长（弯曲角度太大），需重新找点作为求交的点，
	// A线是用来作相交矢量的，需要可能需要再次调整，但是存在无交点却线很长的情况
	double AThisPoint_intersect[3];
	double ThisCurveLength = ThisCurve->GetLength();
	if (ThisCurveLength > 2 * curve_Threshold)
	{
		int shang = ceil(ThisCurveLength / curve_Threshold);
		parm = ((shang - 1)*start_param_evaluate + end_param_evaluater) / shang;

		EvaFlag = UF_EVAL_evaluate(evaluator, n_derivatives, parm, AThisPoint_intersect, derivatives);  //此时的切矢量已经不再是中点处的了
		if (EvaFlag != 0)
		{
			deleteFlag = UF_OBJ_delete_object(ThisCurve->Tag());
			return;
		}

		Acurve_Flag = true;
	}
	//deleteFlag = UF_OBJ_delete_object(ThisCurve->Tag());
	double tangent_this[3] = { derivatives[0], derivatives[1], derivatives[2] };



	//==========4============求偏置后的交线中点附近处的新点 BOtherPoint
	double BOtherPoint[3];
	double new_point_parameter2;
	//newPointFlag = UF_MODL_ask_point_along_curve_2(centerpoint_iEdge, OtherCurve->Tag(), OffsetDisMin, 1, 0.0, BOtherPoint, &new_point_parameter2);
	//if (newPointFlag != 0) return;
	//FlagUnlineData = UF_MODL_ask_curve_parm(OtherCurve->Tag(),centerpoint_iEdge,&Para,curPoint);
	//if (FlagUnlineData != 0)	return;
	//UF_MODL_ask_curve_props(OtherCurve->Tag(),Para+0.5,BOtherPoint,tangent,p_norm,b_norm,&torsion,&rad_of_cur);

	UF_EVAL_p_t evaluator22;
	UF_EVAL_initialize(OtherCurve->Tag(), &evaluator22);
	double limits22[2];
	UF_EVAL_ask_limits(evaluator22, limits22);
	double parm22 = (limits22[1] + limits22[0]) / 2;
	//int n_derivatives = 2;
	double derivatives22[6];
	EvaFlag = UF_EVAL_evaluate(evaluator22, n_derivatives, parm22, BOtherPoint, derivatives22);  //也可以用此得到中点
	if (EvaFlag != 0)
	{
		deleteFlag = UF_OBJ_delete_object(OtherCurve->Tag());
		return;
	}

	// 得到名义上靠近交线的“起点”
	double StartPoint22[3];
	UF_EVAL_evaluate(evaluator22, n_derivatives, limits22[0], StartPoint22, derivatives_StartEnd);
	double EndPoint22[3];
	UF_EVAL_evaluate(evaluator22, n_derivatives, limits22[1], EndPoint22, derivatives_StartEnd);

	double absStart2 = abs(centerpoint_iEdge[0] - StartPoint22[0]) + abs(centerpoint_iEdge[1] - StartPoint22[1]) + abs(centerpoint_iEdge[2] - StartPoint22[2]);
	double absEnd2 = abs(centerpoint_iEdge[0] - EndPoint22[0]) + abs(centerpoint_iEdge[1] - EndPoint22[1]) + abs(centerpoint_iEdge[2] - EndPoint22[2]);

	double start_param_evaluate2 = limits22[0];
	double end_param_evaluater2 = limits22[1];
	if (absStart2 > 0.01 && absEnd2 < 0.01)
	{
		//uc1601("E, 2", 1);
		start_param_evaluate2 = limits22[1];
		end_param_evaluater2 = limits22[0];
	}

	// 若交线太长（弯曲角度太大），需重新找点作为求交的点
	double BOtherPoint_intersect[3];
	double OtherCurveLength = OtherCurve->GetLength();
	if (OtherCurveLength > 2 * curve_Threshold)
	{
		int shang = ceil(OtherCurveLength / curve_Threshold);
		double parm222 = ((shang - 1)*start_param_evaluate2 + end_param_evaluater2) / shang;
		EvaFlag = UF_EVAL_evaluate(evaluator22, n_derivatives, parm222, BOtherPoint_intersect, derivatives22);  //此时的切矢量已经不再是中点处的了
		if (EvaFlag != 0)
		{
			deleteFlag = UF_OBJ_delete_object(OtherCurve->Tag());
			return;
		}

		Bcurve_Flag = true;
	}
	deleteFlag = UF_OBJ_delete_object(OtherCurve->Tag());
	//得到两个新点（可能是几等分之后的求交点）处， 最终的曲线切矢量
	double tangent_other[3] = { derivatives22[0], derivatives22[1], derivatives22[2] };


	//=============5===============根据计算得到的两个新点，计算两个面在该点处的法向量。
	double point_this[3];
	double u1_this[3];
	double v1_this[3];
	double u2_this[3];
	double v2_this[3];
	double radii_this[3];

	//得到求角度的矢量A等信息
	double faceparam_this[2];
	double outpoint_this[3];
	UF_MODL_ask_face_parm(iFaceThis->Tag(), AThisPoint, faceparam_this, outpoint_this);
	double unit_norm_this[3];
	UF_MODL_ask_face_props(iFaceThis->Tag(), faceparam_this, point_this, u1_this, v1_this, u2_this, v2_this, unit_norm_this, radii_this);

	//得到求角度的矢量B等信息
	double faceparam_other[2];
	double outpoint_other[3];
	UF_MODL_ask_face_parm(iFaceOther->Tag(), BOtherPoint, faceparam_other, outpoint_other);
	double unit_norm_other[3];
	UF_MODL_ask_face_props(iFaceOther->Tag(), faceparam_other, point_this, u1_this, v1_this, u2_this, v2_this, unit_norm_other, radii_this);

	//=============6==============得到判断凹凸的矢量信息，，B面只用求得求交点附近的 B线的法失就可以构建求交平面
	if (!!Bcurve_Flag)
	{
		BOtherPoint[0] = BOtherPoint_intersect[0];
		BOtherPoint[1] = BOtherPoint_intersect[1];
		BOtherPoint[2] = BOtherPoint_intersect[2];
	}
	if (!!Acurve_Flag)
	{
		AThisPoint[0] = AThisPoint_intersect[0];
		AThisPoint[1] = AThisPoint_intersect[1];
		AThisPoint[2] = AThisPoint_intersect[2];
	}

	//创建过另外一点的基准平面(过面B上该求交点，交线otherCurve的法平面,Other)
	tag_t OtherDatum;
	UF_MODL_create_plane(BOtherPoint, tangent_other, &OtherDatum);  //unit_norm_this
	UF_OBJ_set_blank_status(OtherDatum, UF_OBJ_BLANKED);


	//====判断有无交点和交点位置，进而调整A处法失量的位置
	double crossover_props_start;
	UF_MODL_ask_curve_parm_no_ext(ThisCurve->Tag(), centerpoint_iEdge, &crossover_props_start);

	double propsPoint[3];
	double intersectMinPara_props;   //可能的最小点的parameter
	UF_MODL_ask_curve_parm(ThisCurve->Tag(), AThisPoint, &intersectMinPara_props, propsPoint);

	bool   MinParaFlag = false;
	double tempCoords[3];
	UF_MODL_intersect_objects(ThisCurve->Tag(), OtherDatum, 0, &NumIntersections, &intersectInfo);
	for (int j = 1; j <= NumIntersections; j++)
	{
		if ((*intersectInfo)->intersect_type != UF_MODL_INTERSECT_POINT)	continue;

		UF_MODL_intersect_info_intersect_t intersect = (*intersectInfo)->intersect;
		UF_MODL_intersect_info_point_t intersectPoint = intersect.point;
		tempCoords[0] = intersectPoint.coords[0];
		tempCoords[1] = intersectPoint.coords[1];
		tempCoords[2] = intersectPoint.coords[2];

		double Para(0);
		double curPoint[3];
		int FlagUnlineData = UF_MODL_ask_curve_parm(ThisCurve->Tag(), tempCoords, &Para, curPoint);
		if (FlagUnlineData != 0)	continue;

		if (abs(crossover_props_start - intersectMinPara_props) > abs(crossover_props_start - Para))
		{
			AThisPoint[0] = curPoint[0];
			AThisPoint[1] = curPoint[1];
			AThisPoint[2] = curPoint[2];
			intersectMinPara_props = Para;
			MinParaFlag = true;
		}
	}


	//============7===============得到判断凹凸的矢量信息,  A面求相交用的法矢量
	double final_norm_this_intersect[3] = { unit_norm_this[0], unit_norm_this[1], unit_norm_this[2] };
	double intersectParam = (crossover_props_start + intersectMinPara_props) / 2;
	double tangent[3];
	double p_norm[3];
	double b_norm[3];
	double torsion;
	double rad_of_cur;
	UF_MODL_ask_curve_props(ThisCurve->Tag(), intersectParam, AThisPoint, tangent, p_norm, b_norm, &torsion, &rad_of_cur);
	deleteFlag = UF_OBJ_delete_object(ThisCurve->Tag());

	double faceparam_this_intersect[2];
	UF_MODL_ask_face_parm(iFaceThis->Tag(), AThisPoint, faceparam_this_intersect, outpoint_this);
	UF_MODL_ask_face_props(iFaceThis->Tag(), faceparam_this_intersect, point_this, u1_this, v1_this, u2_this, v2_this, final_norm_this_intersect, radii_this);



	//根据法向量，创建一条线(沿着面A该点处的法向量，this)
	double APointOffset[3];
	APointOffset[0] = AThisPoint[0] + 10000 * final_norm_this_intersect[0];
	APointOffset[1] = AThisPoint[1] + 10000 * final_norm_this_intersect[1];
	APointOffset[2] = AThisPoint[2] + 10000 * final_norm_this_intersect[2];

	int err_code1;
	tag_t line1;
	UF_CURVE_line_p_t tempLine;
	tempLine = (UF_CURVE_line_p_t)UF_allocate_memory(sizeof(UF_CURVE_line_t), &err_code1);
	tempLine->start_point[0] = AThisPoint[0];
	tempLine->start_point[1] = AThisPoint[1];
	tempLine->start_point[2] = AThisPoint[2];
	tempLine->end_point[0] = APointOffset[0];
	tempLine->end_point[1] = APointOffset[1];
	tempLine->end_point[2] = APointOffset[2];
	int Flag1 = UF_CURVE_create_line(tempLine, &line1);
	if (Flag1 != 0)
	{
		deleteFlag = UF_OBJ_delete_object(OtherDatum);
		return;
	}

	//===============8=================求基准面与新创建的线的交点，并且判断凹凸关系
	int concavityFlag = 0;
	InterSectionFlag = UF_MODL_intersect_objects(line1, OtherDatum, 0, &NumIntersections, &intersectInfo);
	if (InterSectionFlag != 0)
	{
		deleteFlag = UF_OBJ_delete_object(OtherDatum);
		deleteFlag = UF_OBJ_delete_object(line1);
		return;
	}

	deleteFlag = UF_OBJ_delete_object(line1);
	if (NumIntersections > 0)//正方向有交点
	{
		//凹关系
		concavityFlag = 1;
		//uc1601("凹关系",1);
	}
	else if (NumIntersections == 0)
	{
		double APointOffset_rev[3];
		APointOffset_rev[0] = AThisPoint[0] - 10000 * final_norm_this_intersect[0];
		APointOffset_rev[1] = AThisPoint[1] - 10000 * final_norm_this_intersect[1];
		APointOffset_rev[2] = AThisPoint[2] - 10000 * final_norm_this_intersect[2];

		//tempLine = (UF_CURVE_line_p_t) UF_allocate_memory(sizeof(UF_CURVE_line_t), &err_code);
		//tempLine->start_point[0] = AThisPoint[0];
		//tempLine->start_point[1] = AThisPoint[1];
		//tempLine->start_point[2] = AThisPoint[2];
		tempLine->end_point[0] = APointOffset_rev[0];
		tempLine->end_point[1] = APointOffset_rev[1];
		tempLine->end_point[2] = APointOffset_rev[2];
		Flag1 = UF_CURVE_create_line(tempLine, &line1);
		if (Flag1 != 0)
		{
			deleteFlag = UF_OBJ_delete_object(OtherDatum);
			return;
		}

		int NumIntersections_rev = -1;
		InterSectionFlag = UF_MODL_intersect_objects(line1, OtherDatum, 0, &NumIntersections_rev, &intersectInfo);
		if (InterSectionFlag != 0)
		{
			deleteFlag = UF_OBJ_delete_object(OtherDatum);
			deleteFlag = UF_OBJ_delete_object(line1);
			return;
		}

		deleteFlag = UF_OBJ_delete_object(line1);
		if (NumIntersections_rev > 0)//反方向有交点
		{
			//凸关系
			concavityFlag = 2;
			//uc1601("凸关系",1);
		}
		else
		{
			//错误关系
			concavityFlag = 3;
			uc1601(" Warning!:\n 可能存在两平行相交面，已使用紫色与棕色标识。", 1);
			UF_OBJ_set_color(iFaceThis->Tag(), 163);
			UF_OBJ_set_color(iFaceOther->Tag(), 164);
		}
	}
	else
	{
		//错误关系
		concavityFlag = 4;
		uc1601(" Warning!:\n 可能存在异常的面边关系,已使用黄色标出。", 1);
		UF_OBJ_set_color(iFaceThis->Tag(), 6);
		UF_OBJ_set_color(iFaceOther->Tag(), 6);
	}
	deleteFlag = UF_OBJ_delete_object(OtherDatum);



	////分析投影位置的方法二，计算交点，在分析
	//	double  coordsPoint[3];
	//	bool FlagIntersecPoint = false;
	//	for (int j = 1; j <= NumIntersections; j ++)
	//	{
	//		if ((*intersectInfo)->intersect_type != UF_MODL_INTERSECT_POINT)	continue;
	//
	//		UF_MODL_intersect_info_intersect_t intersect = (*intersectInfo)->intersect;
	//		UF_MODL_intersect_info_point_t intersecPoint = intersect.point;
	//		coordsPoint[0] = intersecPoint.coords[0];
	//		coordsPoint[1] = intersecPoint.coords[1];
	//		coordsPoint[2] = intersecPoint.coords[2];
	//		FlagIntersecPoint = true;
	//	}
	//
	//	//UF_OBJ_delete_object();
	//
	//	if (!FlagIntersecPoint) return;
	//
	//	//比较交点在法矢量上投影的位置，分析凹凸关系
	//	double newVector[3];
	//	newVector[0] = coordsPoint[0] - AThisPoint[0];
	//	newVector[1] = coordsPoint[1] - AThisPoint[1];
	//	newVector[2] = coordsPoint[2] - AThisPoint[2];
	//
	//	double newVectorLength = sqrt(newVector[0]*newVector[0] + newVector[1]*newVector[1] + newVector[2]*newVector[2]);
	//	double TargetVector[3];
	//	TargetVector[0] = newVector[0] / newVectorLength;
	//	TargetVector[1] = newVector[1] / newVectorLength;
	//	TargetVector[2] = newVector[2] / newVectorLength;
	//
	//	double ErrorNormal = abs(unit_norm_this[0]-TargetVector[0]) + abs(unit_norm_this[1]- TargetVector[1]) + abs(unit_norm_this[2]-TargetVector[2]);
	//	if (ErrorNormal < 0.1)
	//	{
	//		//凹关系
	//		concavityFlag = 1;
	//	}
	//
	//	else if (ErrorNormal > 1.9 && ErrorNormal < 3)//最小2，最大两倍根号2
	//	{
	//		//凸关系
	//		concavityFlag = 2;
	//	}
	//	else
	//	{
	//		//错误关系
	//		concavityFlag = 3;
	//	}
	////计算交点在分析

	//============9============根据凹凸关系，计算出最终的面面夹角值
	NXOpen::Vector3d vector1(unit_norm_this[0], unit_norm_this[1], unit_norm_this[2]);
	NXOpen::Vector3d vector2(unit_norm_other[0], unit_norm_other[1], unit_norm_other[2]);

	//直接计算两个矢量的夹角
	double angle1 = get_Angle_Two_vector3d(vector1, vector2);
	if (concavityFlag == 1)
	{
		pMainAngle = PI - angle1;
	}
	else if (concavityFlag == 2)
	{
		pMainAngle = PI + angle1;
	}
	else if (concavityFlag == 3)
	{
		pMainAngle = PI;
	}
	else if (concavityFlag == 4)
	{
		return;
	}
}
void get_FaceAngle_old(Face* iFaceThis, Face* iFaceOther, Edge* iEdge, double& pMainAngle)
{
	double tangent_iEdge[3];
	double centerpoint_iEdge[3];
	get_Tangent_of_Centerpoint(iEdge, centerpoint_iEdge, tangent_iEdge);

	tag_t iDatumPlane_tag;
	UF_MODL_create_plane(centerpoint_iEdge, tangent_iEdge, &iDatumPlane_tag);

	int interCurveCount;
	tag_t iEdge_tag = iEdge->Tag();
	Point3d Vertex1;
	Point3d Vertex2;
	iEdge->GetVertices(&Vertex1, &Vertex2);
	if (Vertex1.X == Vertex2.X && Vertex1.Y == Vertex2.Y && Vertex1.Z == Vertex2.Z) interCurveCount = 2;
	else interCurveCount = 1;

	double tangent[3];
	double p_norm[3];
	double b_norm[3];
	double torsion;
	double rad_of_cur;
	//得到This面的法矢
	UF_MODL_intersect_info_p_t* intersectInfo_This;
	tag_t intersectCurveThis_tag = 0;
	int num_intersections = 0;
	UF_MODL_intersect_objects(iFaceThis->Tag(), iDatumPlane_tag, 0, &num_intersections, &intersectInfo_This);
	if (num_intersections == 1)
	{
		if ((*intersectInfo_This)->intersect_type == UF_MODL_INTERSECT_CURVE)
		{
			UF_MODL_intersect_info_intersect_t intersect = (*intersectInfo_This)->intersect;
			UF_MODL_intersect_info_curve_t curve = intersect.curve;
			intersectCurveThis_tag = curve.identifier;
		}
	}
	else if (num_intersections == 2)
	{
		tag_t intersectCurveThis_tag_temp = 0;
		if ((*intersectInfo_This)->intersect_type == UF_MODL_INTERSECT_CURVE)
		{
			UF_MODL_intersect_info_intersect_t intersect = (*intersectInfo_This)->intersect;
			UF_MODL_intersect_info_curve_t curve = intersect.curve;
			intersectCurveThis_tag_temp = curve.identifier;
		}
		double StartPoint[3];
		double EndPoint[3];
		UF_MODL_ask_curve_props(intersectCurveThis_tag_temp, 0, StartPoint, tangent, p_norm, b_norm, &torsion, &rad_of_cur);
		UF_MODL_ask_curve_props(intersectCurveThis_tag_temp, 1, EndPoint, tangent, p_norm, b_norm, &torsion, &rad_of_cur);
		if (((StartPoint[0] - centerpoint_iEdge[0]) > -0.001 && (StartPoint[0] - centerpoint_iEdge[0]) < 0.001 &&
			(StartPoint[1] - centerpoint_iEdge[1]) > -0.001 && (StartPoint[1] - centerpoint_iEdge[1]) < 0.001 &&
			(StartPoint[2] - centerpoint_iEdge[2]) > -0.001 && (StartPoint[2] - centerpoint_iEdge[2]) < 0.001) ||
			((EndPoint[0] - centerpoint_iEdge[0]) > -0.001 && (EndPoint[0] - centerpoint_iEdge[0]) < 0.001 &&
			(EndPoint[1] - centerpoint_iEdge[1]) > -0.001 && (EndPoint[1] - centerpoint_iEdge[1]) < 0.001 &&
			(EndPoint[2] - centerpoint_iEdge[2]) > -0.001 && (EndPoint[2] - centerpoint_iEdge[2]) < 0.001))
		{
			intersectCurveThis_tag = intersectCurveThis_tag_temp;
			intersectInfo_This++;
			UF_MODL_intersect_info_intersect_t intersect = (*intersectInfo_This)->intersect;
			UF_MODL_intersect_info_curve_t curve = intersect.curve;
			UF_OBJ_delete_object(curve.identifier);
		}
		else
		{
			UF_OBJ_delete_object(intersectCurveThis_tag_temp);
			intersectInfo_This++;
			if ((*intersectInfo_This)->intersect_type == UF_MODL_INTERSECT_CURVE)
			{
				UF_MODL_intersect_info_intersect_t intersect = (*intersectInfo_This)->intersect;
				UF_MODL_intersect_info_curve_t curve = intersect.curve;
				intersectCurveThis_tag = curve.identifier;
			}
		}
	}
	else if (num_intersections == 3)
	{
		tag_t intersectCurveThis_tag_temp = 0;
		if ((*intersectInfo_This)->intersect_type == UF_MODL_INTERSECT_CURVE)
		{
			UF_MODL_intersect_info_intersect_t intersect = (*intersectInfo_This)->intersect;
			UF_MODL_intersect_info_curve_t curve = intersect.curve;
			intersectCurveThis_tag_temp = curve.identifier;
		}
		double StartPoint[3];
		double EndPoint[3];
		UF_MODL_ask_curve_props(intersectCurveThis_tag_temp, 0, StartPoint, tangent, p_norm, b_norm, &torsion, &rad_of_cur);
		UF_MODL_ask_curve_props(intersectCurveThis_tag_temp, 1, EndPoint, tangent, p_norm, b_norm, &torsion, &rad_of_cur);
		if (((StartPoint[0] - centerpoint_iEdge[0]) > -0.001 && (StartPoint[0] - centerpoint_iEdge[0]) < 0.001 &&
			(StartPoint[1] - centerpoint_iEdge[1]) > -0.001 && (StartPoint[1] - centerpoint_iEdge[1]) < 0.001 &&
			(StartPoint[2] - centerpoint_iEdge[2]) > -0.001 && (StartPoint[2] - centerpoint_iEdge[2]) < 0.001) ||
			((EndPoint[0] - centerpoint_iEdge[0]) > -0.001 && (EndPoint[0] - centerpoint_iEdge[0]) < 0.001 &&
			(EndPoint[1] - centerpoint_iEdge[1]) > -0.001 && (EndPoint[1] - centerpoint_iEdge[1]) < 0.001 &&
			(EndPoint[2] - centerpoint_iEdge[2]) > -0.001 && (EndPoint[2] - centerpoint_iEdge[2]) < 0.001))
		{
			intersectCurveThis_tag = intersectCurveThis_tag_temp;
			intersectInfo_This++;
			UF_MODL_intersect_info_intersect_t intersect = (*intersectInfo_This)->intersect;
			UF_MODL_intersect_info_curve_t curve = intersect.curve;
			UF_OBJ_delete_object(curve.identifier);

			intersectInfo_This++;
			intersect = (*intersectInfo_This)->intersect;
			curve = intersect.curve;
			UF_OBJ_delete_object(curve.identifier);
		}
		else
		{
			UF_OBJ_delete_object(intersectCurveThis_tag_temp);
			intersectInfo_This++;
			if ((*intersectInfo_This)->intersect_type == UF_MODL_INTERSECT_CURVE)
			{
				UF_MODL_intersect_info_intersect_t intersect = (*intersectInfo_This)->intersect;
				UF_MODL_intersect_info_curve_t curve = intersect.curve;
				intersectCurveThis_tag_temp = curve.identifier;
			}
			double StartPoint[3];
			double EndPoint[3];
			UF_MODL_ask_curve_props(intersectCurveThis_tag_temp, 0, StartPoint, tangent, p_norm, b_norm, &torsion, &rad_of_cur);
			UF_MODL_ask_curve_props(intersectCurveThis_tag_temp, 1, EndPoint, tangent, p_norm, b_norm, &torsion, &rad_of_cur);
			if (((StartPoint[0] - centerpoint_iEdge[0]) > -0.001 && (StartPoint[0] - centerpoint_iEdge[0]) < 0.001 &&
				(StartPoint[1] - centerpoint_iEdge[1]) > -0.001 && (StartPoint[1] - centerpoint_iEdge[1]) < 0.001 &&
				(StartPoint[2] - centerpoint_iEdge[2]) > -0.001 && (StartPoint[2] - centerpoint_iEdge[2]) < 0.001) ||
				((EndPoint[0] - centerpoint_iEdge[0]) > -0.001 && (EndPoint[0] - centerpoint_iEdge[0]) < 0.001 &&
				(EndPoint[1] - centerpoint_iEdge[1]) > -0.001 && (EndPoint[1] - centerpoint_iEdge[1]) < 0.001 &&
				(EndPoint[2] - centerpoint_iEdge[2]) > -0.001 && (EndPoint[2] - centerpoint_iEdge[2]) < 0.001))
			{
				intersectCurveThis_tag = intersectCurveThis_tag_temp;
				intersectInfo_This++;
				UF_MODL_intersect_info_intersect_t intersect = (*intersectInfo_This)->intersect;
				UF_MODL_intersect_info_curve_t curve = intersect.curve;
				UF_OBJ_delete_object(curve.identifier);
			}
			else
			{
				UF_OBJ_delete_object(intersectCurveThis_tag_temp);
				intersectInfo_This++;
				if ((*intersectInfo_This)->intersect_type == UF_MODL_INTERSECT_CURVE)
				{
					UF_MODL_intersect_info_intersect_t intersect = (*intersectInfo_This)->intersect;
					UF_MODL_intersect_info_curve_t curve = intersect.curve;
					intersectCurveThis_tag = curve.identifier;
				}
			}

		}
	}

	double parm = 0.5;
	double centerpointThis_interEdge[3];

	UF_MODL_ask_curve_props(intersectCurveThis_tag, parm, centerpointThis_interEdge, tangent, p_norm, b_norm, &torsion, &rad_of_cur);

	double parmThis_interEdge[2];
	double face_pnt[3];
	UF_MODL_ask_face_parm(iFaceThis->Tag(), centerpointThis_interEdge, parmThis_interEdge, face_pnt);

	double point[3];
	double u1[3];
	double v1[3];
	double u2[3];
	double v2[3];
	double unit_norm_this[3];
	double radii[2];
	UF_MODL_ask_face_props(iFaceThis->Tag(), parmThis_interEdge, point, u1, v1, u2, v2, unit_norm_this, radii);

	//得到Other面的法矢
	UF_MODL_intersect_info_p_t* intersectInfo_Other;
	tag_t intersectCurveOther_tag = 0;
	UF_MODL_intersect_objects(iFaceOther->Tag(), iDatumPlane_tag, 0, &num_intersections, &intersectInfo_Other);
	if (num_intersections == 1)
	{
		if ((*intersectInfo_Other)->intersect_type == UF_MODL_INTERSECT_CURVE)
		{
			UF_MODL_intersect_info_intersect_t intersect = (*intersectInfo_Other)->intersect;
			UF_MODL_intersect_info_curve_t curve = intersect.curve;
			intersectCurveOther_tag = curve.identifier;
		}
	}
	else if (num_intersections == 2)
	{
		tag_t intersectCurveOther_tag_temp = 0;
		if ((*intersectInfo_Other)->intersect_type == UF_MODL_INTERSECT_CURVE)
		{
			UF_MODL_intersect_info_intersect_t intersect = (*intersectInfo_Other)->intersect;
			UF_MODL_intersect_info_curve_t curve = intersect.curve;
			intersectCurveOther_tag_temp = curve.identifier;
		}
		double StartPoint[3];
		double EndPoint[3];
		UF_MODL_ask_curve_props(intersectCurveOther_tag_temp, 0, StartPoint, tangent, p_norm, b_norm, &torsion, &rad_of_cur);
		UF_MODL_ask_curve_props(intersectCurveOther_tag_temp, 1, EndPoint, tangent, p_norm, b_norm, &torsion, &rad_of_cur);
		if (((StartPoint[0] - centerpoint_iEdge[0]) > -0.001 && (StartPoint[0] - centerpoint_iEdge[0]) < 0.001 &&
			(StartPoint[1] - centerpoint_iEdge[1]) > -0.001 && (StartPoint[1] - centerpoint_iEdge[1]) < 0.001 &&
			(StartPoint[2] - centerpoint_iEdge[2]) > -0.001 && (StartPoint[2] - centerpoint_iEdge[2]) < 0.001) ||
			((EndPoint[0] - centerpoint_iEdge[0]) > -0.001 && (EndPoint[0] - centerpoint_iEdge[0]) < 0.001 &&
			(EndPoint[1] - centerpoint_iEdge[1]) > -0.001 && (EndPoint[1] - centerpoint_iEdge[1]) < 0.001 &&
			(EndPoint[2] - centerpoint_iEdge[2]) > -0.001 && (EndPoint[2] - centerpoint_iEdge[2]) < 0.001))
		{
			intersectCurveOther_tag = intersectCurveOther_tag_temp;
			intersectInfo_Other++;
			UF_MODL_intersect_info_intersect_t intersect = (*intersectInfo_Other)->intersect;
			UF_MODL_intersect_info_curve_t curve = intersect.curve;
			UF_OBJ_delete_object(curve.identifier);
		}
		else
		{
			UF_OBJ_delete_object(intersectCurveOther_tag_temp);
			intersectInfo_Other++;
			if ((*intersectInfo_Other)->intersect_type == UF_MODL_INTERSECT_CURVE)
			{
				UF_MODL_intersect_info_intersect_t intersect = (*intersectInfo_Other)->intersect;
				UF_MODL_intersect_info_curve_t curve = intersect.curve;
				intersectCurveOther_tag = curve.identifier;
			}
		}
	}
	else if (num_intersections == 3)
	{
		tag_t intersectCurveOther_tag_temp = 0;
		if ((*intersectInfo_Other)->intersect_type == UF_MODL_INTERSECT_CURVE)
		{
			UF_MODL_intersect_info_intersect_t intersect = (*intersectInfo_Other)->intersect;
			UF_MODL_intersect_info_curve_t curve = intersect.curve;
			intersectCurveOther_tag_temp = curve.identifier;
		}
		double StartPoint[3];
		double EndPoint[3];
		UF_MODL_ask_curve_props(intersectCurveOther_tag_temp, 0, StartPoint, tangent, p_norm, b_norm, &torsion, &rad_of_cur);
		UF_MODL_ask_curve_props(intersectCurveOther_tag_temp, 1, EndPoint, tangent, p_norm, b_norm, &torsion, &rad_of_cur);
		if (((StartPoint[0] - centerpoint_iEdge[0]) > -0.001 && (StartPoint[0] - centerpoint_iEdge[0]) < 0.001 &&
			(StartPoint[1] - centerpoint_iEdge[1]) > -0.001 && (StartPoint[1] - centerpoint_iEdge[1]) < 0.001 &&
			(StartPoint[2] - centerpoint_iEdge[2]) > -0.001 && (StartPoint[2] - centerpoint_iEdge[2]) < 0.001) ||
			((EndPoint[0] - centerpoint_iEdge[0]) > -0.001 && (EndPoint[0] - centerpoint_iEdge[0]) < 0.001 &&
			(EndPoint[1] - centerpoint_iEdge[1]) > -0.001 && (EndPoint[1] - centerpoint_iEdge[1]) < 0.001 &&
			(EndPoint[2] - centerpoint_iEdge[2]) > -0.001 && (EndPoint[2] - centerpoint_iEdge[2]) < 0.001))
		{
			intersectCurveOther_tag = intersectCurveOther_tag_temp;
			intersectInfo_Other++;
			UF_MODL_intersect_info_intersect_t intersect = (*intersectInfo_Other)->intersect;
			UF_MODL_intersect_info_curve_t curve = intersect.curve;
			UF_OBJ_delete_object(curve.identifier);

			intersectInfo_Other++;
			intersect = (*intersectInfo_Other)->intersect;
			curve = intersect.curve;
			UF_OBJ_delete_object(curve.identifier);
		}
		else
		{
			UF_OBJ_delete_object(intersectCurveOther_tag_temp);
			intersectInfo_Other++;
			if ((*intersectInfo_Other)->intersect_type == UF_MODL_INTERSECT_CURVE)
			{
				UF_MODL_intersect_info_intersect_t intersect = (*intersectInfo_Other)->intersect;
				UF_MODL_intersect_info_curve_t curve = intersect.curve;
				intersectCurveOther_tag_temp = curve.identifier;
			}
			double StartPoint[3];
			double EndPoint[3];
			UF_MODL_ask_curve_props(intersectCurveOther_tag_temp, 0, StartPoint, tangent, p_norm, b_norm, &torsion, &rad_of_cur);
			UF_MODL_ask_curve_props(intersectCurveOther_tag_temp, 1, EndPoint, tangent, p_norm, b_norm, &torsion, &rad_of_cur);
			if (((StartPoint[0] - centerpoint_iEdge[0]) > -0.001 && (StartPoint[0] - centerpoint_iEdge[0]) < 0.001 &&
				(StartPoint[1] - centerpoint_iEdge[1]) > -0.001 && (StartPoint[1] - centerpoint_iEdge[1]) < 0.001 &&
				(StartPoint[2] - centerpoint_iEdge[2]) > -0.001 && (StartPoint[2] - centerpoint_iEdge[2]) < 0.001) ||
				((EndPoint[0] - centerpoint_iEdge[0]) > -0.001 && (EndPoint[0] - centerpoint_iEdge[0]) < 0.001 &&
				(EndPoint[1] - centerpoint_iEdge[1]) > -0.001 && (EndPoint[1] - centerpoint_iEdge[1]) < 0.001 &&
				(EndPoint[2] - centerpoint_iEdge[2]) > -0.001 && (EndPoint[2] - centerpoint_iEdge[2]) < 0.001))
			{
				intersectCurveOther_tag = intersectCurveOther_tag_temp;
				intersectInfo_Other++;
				UF_MODL_intersect_info_intersect_t intersect = (*intersectInfo_Other)->intersect;
				UF_MODL_intersect_info_curve_t curve = intersect.curve;
				UF_OBJ_delete_object(curve.identifier);
			}
			else
			{
				UF_OBJ_delete_object(intersectCurveOther_tag_temp);
				intersectInfo_Other++;
				if ((*intersectInfo_Other)->intersect_type == UF_MODL_INTERSECT_CURVE)
				{
					UF_MODL_intersect_info_intersect_t intersect = (*intersectInfo_Other)->intersect;
					UF_MODL_intersect_info_curve_t curve = intersect.curve;
					intersectCurveOther_tag = curve.identifier;
				}
			}

		}
	}

	parm = 0.5;
	double centerpointOther_interEdge[3];

	UF_MODL_ask_curve_props(intersectCurveOther_tag, parm, centerpointOther_interEdge, tangent, p_norm, b_norm, &torsion, &rad_of_cur);

	double parmOther_interEdge[2];
	UF_MODL_ask_face_parm(iFaceOther->Tag(), centerpointOther_interEdge, parmOther_interEdge, face_pnt);

	double unit_norm_other[3];
	UF_MODL_ask_face_props(iFaceOther->Tag(), parmOther_interEdge, point, u1, v1, u2, v2, unit_norm_other, radii);

	//得到This面的侧
	double StartPoint[3];
	double EndPoint[3];
	UF_MODL_ask_curve_props(intersectCurveThis_tag, 0, StartPoint, tangent, p_norm, b_norm, &torsion, &rad_of_cur);
	UF_MODL_ask_curve_props(intersectCurveThis_tag, 1, EndPoint, tangent, p_norm, b_norm, &torsion, &rad_of_cur);

	double tangent2[3];
	if ((StartPoint[0] - centerpoint_iEdge[0]) > -0.001 &&
		(StartPoint[0] - centerpoint_iEdge[0]) < 0.001 &&
		(StartPoint[1] - centerpoint_iEdge[1]) > -0.001 &&
		(StartPoint[1] - centerpoint_iEdge[1]) < 0.001 &&
		(StartPoint[2] - centerpoint_iEdge[2]) > -0.001 &&
		(StartPoint[2] - centerpoint_iEdge[2]) < 0.001)
	{
		double vertex[3];
		double uPara;
		UF_MODL_ask_curve_parm_no_ext(intersectCurveThis_tag, StartPoint, &uPara);
		UF_MODL_ask_curve_props(intersectCurveThis_tag, uPara, vertex, tangent2, p_norm, b_norm, &torsion, &rad_of_cur);
	}
	else
	{
		double vertex[3];
		double uPara;
		UF_MODL_ask_curve_parm_no_ext(intersectCurveThis_tag, EndPoint, &uPara);
		UF_MODL_ask_curve_props(intersectCurveThis_tag, uPara, vertex, tangent2, p_norm, b_norm, &torsion, &rad_of_cur);
		tangent2[0] = -tangent2[0];
		tangent2[1] = -tangent2[1];
		tangent2[2] = -tangent2[2];
	}

	double ref_pnt[3];
	double param[2];
	UF_MODL_ask_face_parm(iFaceThis->Tag(), centerpoint_iEdge, param, face_pnt);

	double unit_norm[3];
	UF_MODL_ask_face_props(iFaceThis->Tag(), param, point, u1, v1, u2, v2, unit_norm, radii);

	double angle;
	UF_VEC3_angle_between(tangent_iEdge, tangent2, unit_norm, &angle);

	if (angle > PI)
	{
		tangent_iEdge[0] = -tangent_iEdge[0];
		tangent_iEdge[1] = -tangent_iEdge[1];
		tangent_iEdge[2] = -tangent_iEdge[2];
	}

	//计算面角度
	double angle_Edge;
	UF_VEC3_angle_between(unit_norm_this, unit_norm_other, tangent_iEdge, &angle_Edge);

	angle_Edge += PI;
	if (angle_Edge > TWOPI) angle_Edge -= TWOPI;

	UF_OBJ_delete_object(iDatumPlane_tag);
	UF_OBJ_delete_object(intersectCurveThis_tag);
	UF_OBJ_delete_object(intersectCurveOther_tag);

	pMainAngle = angle_Edge;
}


//得到回转面的角度   角度制
//输入：需要计算的面 iFace
//输出：面最大的回转角度 MaxAngle； 最长回转边的半径 ；最长回转边的中点 MidVertex；
void get_surface_angle(NXOpen::Face* iFace, double& MaxAngle, double& Radius, NXOpen::Point3d& MidVertex, Vector3d _Normal)
{
	std::vector<NXOpen::Edge*>  EdgeList = iFace->GetEdges();

	//找到最长边界线(严格来讲，是找到绕中心轴旋转的弧线的最长)
	double MaxLength = 0;
	int FlagLength = NULL;
	for (std::vector<NXOpen::Edge*>::size_type i = 0; i != EdgeList.size(); i++)
	{
		NXOpen::Edge* iEdge = EdgeList[i];

		int typeN;
		UF_MODL_ask_edge_type(iEdge->Tag(), &typeN);
		//非 圆弧，排除
		if (typeN != UF_MODL_CIRCULAR_EDGE)   continue;

		tag_t iCurve_tag;
		UF_MODL_create_curve_from_edge(iEdge->Tag(), &iCurve_tag);
		UF_OBJ_set_blank_status(iCurve_tag, UF_OBJ_BLANKED);

		NXObjectManager CurveManager;
		NXOpen::TaggedObject* pTaggedObject_Curve = CurveManager.Get(iCurve_tag);
		NXOpen::Curve* pCurve = dynamic_cast<Curve*>(pTaggedObject_Curve);
		NXOpen::Arc * iEdgeArc(dynamic_cast<NXOpen::Arc *>(pCurve));
		NXOpen::Point3d iCenterPoint = iEdgeArc->CenterPoint();
		NXOpen::Vector3d vector3d1(iCenterPoint.X, iCenterPoint.Y, iCenterPoint.Z);
		UF_OBJ_delete_object(iEdgeArc->Tag());

		double angleVector3ds = get_Angle_Two_vector3d(vector3d1, _Normal);
		angleVector3ds = angleVector3ds / PI * 180;
		//圆心不过中心轴线， 排除
		if (0.01 < angleVector3ds && angleVector3ds < 179.99)  continue;


		double iLength = iEdge->GetLength();
		if (iLength > MaxLength)
		{
			MaxLength = iLength;
			FlagLength = i;
		}
	}

	//计算最大角度
	NXOpen::Edge* LongestEdge = EdgeList[FlagLength];

	//tag_t elem_tag = LongestEdge->Tag(); 
	//int* num_edges ; 
	//tag_t* start_nodes ; 
	//tag_t* end_nodes ; 
	//tag_t* mid_nodes;
	//int Result = UF_SF_ask_element_edges(elem_tag, num_edges, &start_nodes, &end_nodes, &mid_nodes);

	//NXObjectManager *Manager = new NXObjectManager;
	//TaggedObject* pTaggedObject = Manager->Get(*mid_nodes);
	//NXOpen::Point3d* pFace_Each = dynamic_cast<Point3d*>(pTaggedObject[1]);

	//UF_free(start_nodes);
	//UF_free(end_nodes);
	//UF_free(mid_nodes);

	NXOpen::Point3d vertex1; /** First vertex in the edge */
	NXOpen::Point3d vertex2; /** Second vertex in the edge */
	LongestEdge->GetVertices(&vertex1, &vertex2);


	//得到中点 MidPoint
	tag_t iEdge_tag = LongestEdge->Tag();
	tag_t iCurve_tag;
	UF_MODL_create_curve_from_edge(iEdge_tag, &iCurve_tag);

	double Point1[3] = { vertex1.X, vertex1.Y, vertex1.Z };
	double Point2[3] = { vertex2.X, vertex2.Y, vertex2.Z };
	double uMin, uMax;
	UF_MODL_ask_curve_parm_no_ext(iCurve_tag, Point1, &uMin);
	UF_MODL_ask_curve_parm_no_ext(iCurve_tag, Point2, &uMax);

	double uMid = (uMin + uMax) / 2;
	double MidPoint[3];
	double MidTangent[3];
	double pnorm[3];
	double bnorm[3];
	double torsion;
	double rad_of_cur;
	UF_MODL_ask_curve_props(iCurve_tag, uMid, MidPoint, MidTangent, pnorm, bnorm, &torsion, &rad_of_cur);
	Radius = rad_of_cur;
	MidVertex.X = MidPoint[0];
	MidVertex.Y = MidPoint[1];
	MidVertex.Z = MidPoint[2];

	int iCurveDeleteFlag = UF_OBJ_delete_object(iCurve_tag);

	//起点等于终点，边界线是封闭的,角度2π
	double subx = fabs(vertex1.X - vertex2.X);
	double suby = fabs(vertex1.Y - vertex2.Y);
	double subz = fabs(vertex1.Z - vertex2.Z);
	if (subx<0.0001 && suby<0.0001 && subz<0.1)
	{
		MaxAngle = 360;//2π
	}
	//不到2π，计算实际角度
	else
	{
		double point1X = 0;
		double point1Y = 0;
		double point2X = 0;
		double point2Y = 0;
		if (_Normal.X == 0 && _Normal.Y == 0 && _Normal.Z == 1)
		{
			point1X = vertex1.X;
			point1Y = vertex1.Y;
			point2X = MidPoint[0];
			point2Y = MidPoint[1];
		}
		else if (_Normal.X == 0 && _Normal.Y == 1 && _Normal.Z == 0)
		{
			point1X = vertex1.X;
			point1Y = vertex1.Z;
			point2X = MidPoint[0];
			point2Y = MidPoint[2];
		}
		else if (_Normal.X == 1 && _Normal.Y == 0 && _Normal.Z == 0)
		{
			point1X = vertex1.Y;
			point1Y = vertex1.Z;
			point2X = MidPoint[1];
			point2Y = MidPoint[2];
		}
		NXOpen::Vector3d vector1(point1X, point1Y, 0);
		//NXOpen::Vector3d vector2 (vertex2.X, vertex2.Y, 0);
		NXOpen::Vector3d Midvector(point2X, point2Y, 0);

		//直接计算两个矢量的夹角
		double angle1 = get_Angle_Two_vector3d(vector1, Midvector);
		//double angle2 = get_Angle_Two_vector3d(Midvector,vector2);
		MaxAngle = (angle1 + angle1) * 180 / PI;

		//double* small_angle;
		//double* large_angle;
		//int angle = UF_MODL_ask_vector_angle(vector1,vector2,small_angle,large_angle);
	}
}

//计算一个面的内环相邻面
//输入：顶面
//输出：内环相邻面
void get_Inner_Adjacent_Faces(NXOpen::Face* ipFace, std::vector<NXOpen::Face*> &InnerAdjacentFaces)
{
	tag_t ipFace_t = ipFace->Tag();

	std::vector<Edge*> ipFaceInnerEdge;
	uf_loop_p_t loop_list = NULL;
	UF_MODL_ask_face_loops(ipFace_t, &loop_list);
	int loopscount = 0;
	UF_MODL_ask_loop_list_count(loop_list, &loopscount);

	for (int i = 0; i < loopscount; i++)
	{
		int loopstype = 0;
		uf_list_p_t EdgeList = NULL;
		UF_MODL_ask_loop_list_item(loop_list, i, &loopstype, &EdgeList);
		if (loopstype != 1)
		{
			int EdgeListCount = 0;
			UF_MODL_ask_list_count(EdgeList, &EdgeListCount);
			for (int j = 0; j < EdgeListCount; j++)
			{
				tag_t Edge_Tag = NULL;
				UF_MODL_ask_list_item(EdgeList, j, &Edge_Tag);

				//通过Tag值得到对应的边
				NXObjectManager EdgeManager;
				TaggedObject* pTaggedObject_Edge_Each = EdgeManager.Get(Edge_Tag);
				Edge* pEdge_Each = dynamic_cast<Edge*>(pTaggedObject_Edge_Each);

				if (pEdge_Each->GetLength() >0)  //可以采用一个长度值来限定部分其他的太小的内环。
				{
					ipFaceInnerEdge.push_back(pEdge_Each);//存入内环列表中
				}
			}
		}
	}

	for (std::vector<Edge*>::size_type i = 0; i != ipFaceInnerEdge.size(); i++)
	{
		Edge* pEdge_Each = ipFaceInnerEdge[i];
		std::vector<Face*> pEdge_Each_Faces = pEdge_Each->GetFaces();

		for (std::vector<Face*>::size_type j = 0; j != pEdge_Each_Faces.size(); j++)
		{
			Face* pFace_Each = pEdge_Each_Faces[j];
			tag_t pFace_Each_tag = pFace_Each->Tag();

			if (pFace_Each_tag != ipFace_t && Is_Face_in_lpFace(pFace_Each, InnerAdjacentFaces) == -1)
				InnerAdjacentFaces.push_back(pFace_Each);
		}
	}
}

//计算一个面的最大曲率半径
void get_face_maxRad(Face* iFace, double & MaxFaceRad)
{
	//tag_t faceTag = iFace->Tag();
	//double param [2] = {0.5, 0.5};  //parameter(u,v)on face
	//double point [3];  //Point at parameter 
	//double u1 [3];     //First derivative in U
	//double v1 [3];     //First derivative in V
	//double u2 [3];     //Second derivative in U
	//double v2 [3];     //Second derivative in V
	//double unit_norm [3];  //Unit face normal 
	//double radii [2];  //Principal radii of curvature 
	//UF_MODL_ask_face_props(faceTag,param,point,u1,v1,u2,v2,unit_norm,radii);

	tag_t facetag = iFace->Tag();
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

	double tempRad = 0;
	tempRad = abs(radii[0]);
	radii[0] = tempRad;
	tempRad = abs(radii[1]);
	radii[1] = tempRad;

	double MaxRad(0);
	Face::FaceType pFace_Type = iFace->SolidFaceType();
	if (pFace_Type == Face::FaceType::FaceTypeCylindrical || pFace_Type == Face::FaceType::FaceTypeConical || pFace_Type == Face::FaceType::FaceTypeSurfaceOfRevolution || pFace_Type == Face::FaceType::FaceTypeBlending)
	{
		if (radii[0] >= radii[1])
		{
			MaxRad = radii[1];
		}
		if (radii[0] <  radii[1])
		{
			MaxRad = radii[0];
		}
	}
	else
	{
		if (radii[0] >= radii[1])
		{
			MaxRad = radii[0];
		}
		if (radii[0] <  radii[1])
		{
			MaxRad = radii[1];
		}
	}
	MaxFaceRad = MaxRad;
}
void get_face_maxRad2(Face* iFace, double & MaxFaceRad)
{
	//tag_t faceTag = iFace->Tag();
	//double param [2] = {0.5, 0.5};  //parameter(u,v)on face
	//double point [3];  //Point at parameter 
	//double u1 [3];     //First derivative in U
	//double v1 [3];     //First derivative in V
	//double u2 [3];     //Second derivative in U
	//double v2 [3];     //Second derivative in V
	//double unit_norm [3];  //Unit face normal 
	//double radii [2];  //Principal radii of curvature 
	//UF_MODL_ask_face_props(faceTag,param,point,u1,v1,u2,v2,unit_norm,radii);

	tag_t facetag = iFace->Tag();
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

	//double tempRad = 0;
	//tempRad = abs(radii[0]);
	//radii[0] = tempRad;
	//tempRad = abs(radii[1]);
	//radii[1] = tempRad;

	double MaxRad(0);
	Face::FaceType pFace_Type = iFace->SolidFaceType();
	if (pFace_Type == Face::FaceType::FaceTypeCylindrical || pFace_Type == Face::FaceType::FaceTypeConical || pFace_Type == Face::FaceType::FaceTypeSwept)
	{
		if (radii[0] >= radii[1])
		{
			MaxRad = radii[1];
		}
		if (radii[0] <  radii[1])
		{
			MaxRad = radii[0];
		}
	}
	else
	{
		if (radii[0] >= radii[1])
		{
			MaxRad = radii[0];
		}
		if (radii[0] <  radii[1])
		{
			MaxRad = radii[1];
		}
	}
	MaxFaceRad = MaxRad;
}
//去除重复面
void RemoveDplicateFace(std::vector<Face*> &FacesVector)
{
	std::vector<Face*> TempFacesVector;
	for (std::vector<Face*>::size_type ii = 0; ii != FacesVector.size(); ii++)
	{
		Face* iiFace = FacesVector[ii];
		int DuplicatinFlag = Is_Face_in_lpFace(iiFace, TempFacesVector);
		if (DuplicatinFlag == -1)
		{
			TempFacesVector.push_back(iiFace);
		}
	}

	FacesVector.clear();
	FacesVector.insert(FacesVector.begin(), TempFacesVector.begin(), TempFacesVector.end());
}
//去除重复边
void RemoveDplicateEdge(std::vector<Edge*> &EdgeVector)
{
	std::vector<Edge*> TempVector;
	for (std::vector<Edge*>::size_type ii = 0; ii != EdgeVector.size(); ii++)
	{
		Edge* iiTemp = EdgeVector[ii];
		bool equalFlag = false;
		for (std::vector<Edge*>::size_type jj = ii + 1; jj != EdgeVector.size(); jj++)
		{
			Edge* jjTemp = EdgeVector[jj];

			if (jjTemp->Tag() == iiTemp->Tag())
			{
				equalFlag = true;
				break;
			}
		}
		if (!equalFlag)
		{
			TempVector.push_back(iiTemp);
		}
	}

	EdgeVector.clear();
	EdgeVector.insert(EdgeVector.begin(), TempVector.begin(), TempVector.end());
}
//去除重复点
void RemoveDplicatePoint3d(std::vector<Point3d> &Point3dVector)
{
	std::vector<Point3d> TempVector;
	for (std::vector<Point3d*>::size_type ii = 0; ii != Point3dVector.size(); ii++)
	{
		Point3d iiTemp = Point3dVector[ii];
		bool equalFlag = false;
		for (std::vector<Point3d*>::size_type jj = ii + 1; jj != Point3dVector.size(); jj++)
		{
			Point3d jjTemp = Point3dVector[jj];

			if (iiTemp.X == jjTemp.X  && iiTemp.Y == jjTemp.Y && iiTemp.Z == jjTemp.Z)
			{
				equalFlag = true;
				break;
			}
		}
		if (!equalFlag)
		{
			TempVector.push_back(iiTemp);
		}
	}

	Point3dVector.clear();
	Point3dVector.insert(Point3dVector.begin(), TempVector.begin(), TempVector.end());
}

//得到面列中所有顶点
void Get_Edges_Vertexs_FaceVector(std::vector<Face*> FaceVector, std::vector<Edge*> &AllEdges, std::vector<Point3d> &AllVertexPoints)
{
	for (std::vector<Face*>::size_type j = 0; j != FaceVector.size(); j++)
	{
		Face* EachFace = FaceVector[j];
		std::vector<Edge*> FaceEdges = EachFace->GetEdges();
		for (std::vector<Edge*>::size_type k = 0; k != FaceEdges.size(); k++)
		{
			Edge* EachEdge = FaceEdges[k];

			AllEdges.push_back(EachEdge);

			NXOpen::Point3d vertex1;
			NXOpen::Point3d vertex2;
			EachEdge->GetVertices(&vertex1, &vertex2);
			AllVertexPoints.push_back(vertex1);
			AllVertexPoints.push_back(vertex2);
		}
	}

	RemoveDplicateEdge(AllEdges);
	RemoveDplicatePoint3d(AllVertexPoints);
}

//特征种子面排序
void OrderingFace(std::vector<Face*> &FacesVector, Vector3d _Normal)
{
	std::vector<Point3d> CenterPoint3dVector;
	CenterPoint3dVector.clear();
	//得到中点
	for (std::vector<Face*>::size_type ii = 0; ii != FacesVector.size(); ii++)//遍历所有凸台平面
	{
		Face* iiFace = FacesVector[ii];

		double uv_min_max[4];
		double param[2];
		UF_MODL_ask_face_uv_minmax(iiFace->Tag(), uv_min_max);//uv平均值
		double uaverage = (uv_min_max[0] + uv_min_max[1]) / 2;
		double vaverage = (uv_min_max[2] + uv_min_max[3]) / 2;

		param[0] = uaverage;
		param[1] = vaverage;
		double centerPoint[3];  //中点坐标
		double u1[3];
		double v1[3];
		double u2[3];
		double v2[3];
		double unit_norm[3];
		double radii[2];
		UF_MODL_ask_face_props(iiFace->Tag(), param, centerPoint, u1, v1, u2, v2, unit_norm, radii);
		Point3d CenterPoint3d(centerPoint[0], centerPoint[1], centerPoint[2]);

		CenterPoint3dVector.push_back(CenterPoint3d);
	}

	//得到中点角度
	std::vector<double> PlusAngle;
	std::vector<double> MinusAngle;
	std::vector<Face*> PlusFace;
	std::vector<Face*> MinusFace;
	PlusAngle.clear();
	MinusAngle.clear();
	PlusFace.clear();
	MinusFace.clear();
	for (std::vector<Point3d>::size_type ii = 0; ii != CenterPoint3dVector.size(); ii++)//遍历所有凸台平面中点
	{
		double _name_X = 0;
		double _name_Y = 0;
		if (_Normal.X == 0 && _Normal.Y == 0 && _Normal.Z == 1)
		{
			_name_X = CenterPoint3dVector[ii].X;
			_name_Y = CenterPoint3dVector[ii].Y;
		}
		else if (_Normal.X == 0 && _Normal.Y == 1 && _Normal.Z == 0)
		{
			_name_X = CenterPoint3dVector[ii].Z;
			_name_Y = CenterPoint3dVector[ii].X;
		}
		else if (_Normal.X == 1 && _Normal.Y == 0 && _Normal.Z == 0)
		{
			_name_X = CenterPoint3dVector[ii].Y;
			_name_Y = CenterPoint3dVector[ii].Z;
		}
		else  return;

		Vector3d OriginalNormal(1, 0, 0);
		Vector3d CalculatNormal(_name_X, _name_Y, 0);
		double angle = get_Angle_Two_vector3d(OriginalNormal, CalculatNormal);
		angle = angle * 180 / PI;

		if (_name_Y >= 0)
		{
			PlusAngle.push_back(angle);
			PlusFace.push_back(FacesVector[ii]);
		}
		else
		{
			MinusAngle.push_back(angle);
			MinusFace.push_back(FacesVector[ii]);
		}
	}

	//中点角度排序
	int num = PlusAngle.size();  //正排序，结果从小到大
	if (num > 2)
	{
		for (std::vector<double>::size_type ii = 0; ii != num - 1; ii++)
		{
			for (std::vector<double>::size_type jj = 0; jj != num - ii - 1; jj++)
			{
				if (PlusAngle[jj] > PlusAngle[jj + 1])
				{
					double temp = PlusAngle[jj];
					PlusAngle[jj] = PlusAngle[jj + 1];
					PlusAngle[jj + 1] = temp;

					Face* tempFace = PlusFace[jj];
					PlusFace[jj] = PlusFace[jj + 1];
					PlusFace[jj + 1] = tempFace;
				}
			}
		}
	}


	num = MinusAngle.size();   //负排序，结果从大到小
	if (num > 2)
	{
		for (std::vector<double>::size_type ii = 0; ii != num - 1; ii++)
		{
			for (std::vector<double>::size_type jj = 0; jj != num - ii - 1; jj++)
			{
				if (MinusAngle[jj] < MinusAngle[jj + 1])
				{
					double temp = MinusAngle[jj];
					MinusAngle[jj] = MinusAngle[jj + 1];
					MinusAngle[jj + 1] = temp;

					Face* tempFace = MinusFace[jj];
					MinusFace[jj] = MinusFace[jj + 1];
					MinusFace[jj + 1] = tempFace;
				}
			}
		}
	}

	//重新输出排序后的特征种子面列表
	FacesVector.clear();
	FacesVector.insert(FacesVector.end(), PlusFace.begin(), PlusFace.end());
	FacesVector.insert(FacesVector.end(), MinusFace.begin(), MinusFace.end());
}

//得到一个面的整环数
int Get_FullCircle_Num(Face* inputFace, Vector3d _Normal)
{
	int NumFullcircle(0);
	NXOpen::Point3d vertex1; /** First vertex in the edge */
	NXOpen::Point3d vertex2; /** Second vertex in the edge */
	std::vector<NXOpen::Edge*> SeedFaceEdgeList;
	SeedFaceEdgeList.clear();

	if (inputFace->Tag())
	{
		SeedFaceEdgeList = inputFace->GetEdges();
	}

	for (std::vector<NXOpen::Edge*>::size_type j = 0; j != SeedFaceEdgeList.size(); j++)
	{
		Edge* EachEdge = SeedFaceEdgeList[j];

		int typeN;
		UF_MODL_ask_edge_type(EachEdge->Tag(), &typeN);
		//非 圆弧，排除
		if (typeN != UF_MODL_CIRCULAR_EDGE)   continue;

		EachEdge->GetVertices(&vertex1, &vertex2);
		double subx = fabs(vertex1.X - vertex2.X);
		double suby = fabs(vertex1.Y - vertex2.Y);
		double subz = fabs(vertex1.Z - vertex2.Z);
		// 非 整环， 排除
		if (subx > 0.001 || suby > 0.001 || subz > 0.001)  continue;


		tag_t iCurve_tag;
		UF_MODL_create_curve_from_edge(EachEdge->Tag(), &iCurve_tag);
		UF_OBJ_set_blank_status(iCurve_tag, UF_OBJ_BLANKED);

		NXObjectManager CurveManager;
		NXOpen::TaggedObject* pTaggedObject_Curve = CurveManager.Get(iCurve_tag);
		NXOpen::Curve* pCurve = dynamic_cast<Curve*>(pTaggedObject_Curve);
		NXOpen::Arc * iEdgeArc(dynamic_cast<NXOpen::Arc *>(pCurve));
		NXOpen::Point3d iCenterPoint = iEdgeArc->CenterPoint();
		NXOpen::Vector3d vector3d1(iCenterPoint.X, iCenterPoint.Y, iCenterPoint.Z);
		UF_OBJ_delete_object(iEdgeArc->Tag());

		double angleVector3ds = get_Angle_Two_vector3d(vector3d1, _Normal);
		angleVector3ds = angleVector3ds / PI * 180;
		//圆心不过中心轴线， 排除
		if (0.01 < angleVector3ds && angleVector3ds < 179.99)  continue;

		NumFullcircle++;
	}

	return NumFullcircle;
}

void get_MidAngle(Face* inputFace, double& inputAngle, Vector3d _Normal, double& OutputAngle /*,NXOpen::Point3d &MidVertex*/)
{
	double MaxLength = 0;
	int FlagLength = 0;
	NXOpen::Arc* TopFaceArc;
	std::vector<NXOpen::Edge*> RibFaceEdges;
	RibFaceEdges = inputFace->GetEdges();
	for (std::vector<Edge*>::size_type j = 0; j != RibFaceEdges.size(); j++)
	{
		Edge* EachEdge = RibFaceEdges[j];
		int typeN;
		UF_MODL_ask_edge_type(EachEdge->Tag(), &typeN);

		if (typeN != UF_MODL_CIRCULAR_EDGE)   continue;	           //非圆弧，排除

		tag_t iCurve_tag;
		UF_MODL_create_curve_from_edge(EachEdge->Tag(), &iCurve_tag);
		UF_OBJ_set_blank_status(iCurve_tag, UF_OBJ_BLANKED);

		NXObjectManager CurveManager;
		NXOpen::TaggedObject* pTaggedObject_Curve = CurveManager.Get(iCurve_tag);
		NXOpen::Curve* pCurve = dynamic_cast<Curve*>(pTaggedObject_Curve);
		NXOpen::Arc * iEdgeArc(dynamic_cast<NXOpen::Arc *>(pCurve));
		NXOpen::Point3d iCenterPoint = iEdgeArc->CenterPoint();
		NXOpen::Vector3d vector3d1(iCenterPoint.X, iCenterPoint.Y, iCenterPoint.Z);
		UF_OBJ_delete_object(iEdgeArc->Tag());

		double angleVector3ds = get_Angle_Two_vector3d(vector3d1, _Normal);
		angleVector3ds = angleVector3ds / PI * 180;
		//圆心不过中心轴线， 排除
		if (0.01 < angleVector3ds && angleVector3ds < 179.99)  continue;

		double iLength = EachEdge->GetLength();
		if (iLength > MaxLength)
		{
			MaxLength = iLength;
			FlagLength = j;
		}
	}
	NXOpen::Edge* CurveEdge = RibFaceEdges[FlagLength];

	tag_t ICurve_tag;
	UF_MODL_create_curve_from_edge(CurveEdge->Tag(), &ICurve_tag);
	UF_OBJ_set_blank_status(ICurve_tag, UF_OBJ_BLANKED);

	NXObjectManager CurveManager;
	NXOpen::TaggedObject* pTaggedObject_Curve = CurveManager.Get(ICurve_tag);
	NXOpen::Curve* PCurve = dynamic_cast<Curve*>(pTaggedObject_Curve);
	NXOpen::Arc * IEdgeArc(dynamic_cast<NXOpen::Arc *>(PCurve));

	TopFaceArc = IEdgeArc;

	UF_CURVE_arc_p_t* ArcCoord = NULL;
	ArcCoord = new UF_CURVE_arc_p_t;

	int k = UF_CURVE_ask_arc_data(TopFaceArc->Tag(), *ArcCoord);
	//UF_CURVE_arc_p_t &ArcData = *ArcCoord;
	delete ArcCoord;
	//tag_t MatrixTag = ArcData->matrix_tag ;
	//double MatrixValue[9];
	//UF_CSYS_ask_matrix_values(MatrixTag , MatrixValue) ;
	//double X_Vector[3] ;
	//double Y_Vector[3] ;
	//UF_MTX3_x_vec(MatrixValue ,X_Vector);
	//UF_MTX3_y_vec(MatrixValue ,Y_Vector);
	//
	//NXOpen::Vector3d iMidVector (MidVertex.X, MidVertex.Y, MidVertex.Z);
	//double VectorLength = sqrt(MidVertex.X*MidVertex.X+MidVertex.Y*MidVertex.Y + MidVertex.Z*MidVertex.Z);
	//NXOpen::Vector3d MidVector (MidVertex.X/VectorLength, MidVertex.Y/VectorLength, MidVertex.Z/VectorLength);
	//
	//NXOpen::Vector3d X_V (X_Vector[1] ,X_Vector[2] ,X_Vector[3]);
	//NXOpen::Vector3d Y_V (Y_Vector[1] ,Y_Vector[2] ,Y_Vector[3]);
	//
	//double angle1 = get_Angle_Two_vector3d(MidVector, X_V);
	//double angle2 = get_Angle_Two_vector3d(MidVector, Y_V);
	//double Angle2 = angle2*180/PI;
	//if(Angle2 < 90)
	//	OutputAngle = angle1*180/PI ;
	//if(Angle2 >= 90)
	//	OutputAngle = 360 - angle1*180/PI ;


	//double ArcLength = TopFaceArc->GetLength();
	//double SAngle =  TopFaceArc->StartAngle();
	//double EAngle =  TopFaceArc->EndAngle() ;
	//double eangle = EAngle*180/PI;
	//double sangle = SAngle*180/PI;
	//double angle1 = (SAngle*180/PI) + (inputAngle/2); 
	//double angle2;
	//if (angle1 > 180)
	//{
	//	OutputAngle = 540 - angle1;
	//}
	//else
	//{
	//	OutputAngle = 180 - angle1;
	//}

	//OutputAngle =  angle2+ (inputAngle/2) ;

}
int  Is_Num_in_lpNum(int pNum, std::vector<int> lpNum)
{
	if (pNum == NULL)
	{
		return -1;
	}
	int num_Faces = lpNum.size();

	int i = 0;
	for (i = 0; i<num_Faces; i++)
	{
		int EachNum = lpNum[i];
		if (pNum == EachNum)
		{
			return i;
		}

	}
	return -1;
}

void GetDeadlineInfo(double &DeadInfoDouble)
{
	DeadlineInfo *deadline_infomation = new DeadlineInfo;
	int Dead_Year = deadline_infomation->Deadline_year;
	int Dead_Month = deadline_infomation->Deadline_month;
	int Dead_Day = deadline_infomation->Deadline_day;
	int Dead_Hour = deadline_infomation->Deadline_hour;
	int Dead_Minute = deadline_infomation->Deadline_minute;
	int Dead_Second = deadline_infomation->Deadline_second;

	int Dead_Date = deadline_infomation->Deadline_Date;
	int Dead_Time = deadline_infomation->Deadline_Time;

	//char* tmpDate = new char;
	char tmpDate[9] = { 0 };
	sprintf(tmpDate, "%d", Dead_Date);
	tmpDate[8] = '\0';

	//char* tmpTime = new char;
	char tmpTime[7] = { 0 };
	sprintf(tmpTime, "%d", Dead_Time);
	tmpTime[6] = '\0';

	char DeadInfo[15];
	sprintf(DeadInfo, "%s%s", tmpDate, tmpTime);
	DeadInfo[14] = '\0';
	std::string DeadInfoString = DeadInfo;

	double dd = atof(DeadInfoString.c_str());
	DeadInfoDouble = dd;
}


void GetCurrentDate(double &CurrentInfoDouble)
{
	SYSTEMTIME st;
	GetLocalTime(&st);
	int mYear = (int)st.wYear;
	int mMonth = (int)st.wMonth;
	int mDay = (int)st.wDay;
	int mHour = (int)st.wHour;
	int mMinute = (int)st.wMinute;
	int mSecond = (int)st.wSecond;

	time_t t = time(NULL);
	char tmp[64];
	strftime(tmp, sizeof(tmp), "%Y/%m/%d %X %A 本年第%j天 %z", localtime(&t));
	char tmpDate[9] = { tmp[0], tmp[1], tmp[2], tmp[3], tmp[5], tmp[6], tmp[8], tmp[9], '\0' };    //  日期，每一位已补零
	std::string DateString = tmpDate;


	char* hourChar = new char;
	sprintf(hourChar, "%d", mHour);
	if (0 <= mHour && mHour <= 9)
	{
		char temp[3] = { 0 };
		std::string zeroString = "0";
		temp[0] = *(zeroString.c_str());
		temp[1] = *hourChar;
		temp[2] = '\0';
		hourChar = &(temp[0]);
	}

	char* minuteChar = new char;
	sprintf(minuteChar, "%d", mMinute);
	if (0 <= mMinute && mMinute <= 9)
	{
		char temp[3] = { '0', };
		strcat(temp, minuteChar);
		minuteChar = &(temp[0]);
	}

	char* secondChar = new char;
	sprintf(secondChar, "%d", mSecond);
	if (0 <= mSecond && mSecond <= 9)
	{
		char temp[3] = { '0', };
		strcat(temp, secondChar);
		secondChar = &(temp[0]);
	}

	char tmpTime[7];            //= {tmp[11], tmp[12], tmp[14], tmp[15], tmp[17], tmp[18],'\0'};  //时间，未补零
	sprintf(tmpTime, "%s%s", hourChar, minuteChar);
	strcat(tmpTime, secondChar);
	tmpTime[6] = '\0';
	std::string TimeString = tmpTime;

	std::string CurrentDateTimeString = DateString + TimeString + '\0';

	double dd = atof(CurrentDateTimeString.c_str());
	CurrentInfoDouble = dd;
}

double EncryptDecryptInfo(char *SecretFilePath, double currentInfo, double deadInfo)
{
	int key = 11;      // rand()%20;
	double errorDouble = 20231231235959.000;

	std::ifstream inFile2;
	std::ofstream outFile2;
	try
	{
		//char *CStr = "string to convert";
		//size_t len = strlen(CStr) + 1;
		//size_t converted = 0;
		//wchar_t *WStr;
		//WStr=(wchar_t*)malloc(len*sizeof(wchar_t));
		//mbstowcs_s(&converted, WStr, len, CStr, _TRUNCATE);

		inFile2.open(SecretFilePath);
	}
	catch (...)
	{
		inFile2.open("D:\\FBM\\setup\\secretInfo400.sci");
	}

	if (inFile2.fail())
	{
		uc1601(" Error!:\n 打开系统文件异常，请检查系统文件。", 1);
		return errorDouble;
	}
	std::string NormalFilePath = "tempsecinfo.sci";
	outFile2.open(NormalFilePath.c_str());
	if (outFile2.fail())
	{
		return errorDouble;
	}

	declassifyFile(inFile2, outFile2, key);  // declassifyFile
	inFile2.close();
	outFile2.close();


	//加入本次打开软件日期时间
	double LatestInfo = readWriteLatestInfo(NormalFilePath, currentInfo, deadInfo);


	std::ifstream inFile3;
	std::ofstream outFile3;
	inFile3.open(NormalFilePath.c_str());
	if (inFile3.fail())
	{
		uc1601(" Error!:\n 打开系统文件异常，请检查系统文件。", 1);
		remove(NormalFilePath.c_str());
		return errorDouble;
	}
	outFile3.open(SecretFilePath);
	if (outFile3.fail())
	{
		uc1601(" Error!:\n 打开系统文件异常，请检查系统文件。", 1);
		remove(NormalFilePath.c_str());
		return errorDouble;
	}

	encryptFile(inFile3, outFile3, key);
	inFile3.close();
	outFile3.close();


	//清除内容
	std::ofstream cleanInfo(NormalFilePath.c_str());
	cleanInfo << " " << std::endl;
	cleanInfo.close();

	//删除文本文件
	remove(NormalFilePath.c_str());

	return LatestInfo;
}

void encryptFile(std::ifstream& inFile, std::ofstream& outFile, int key)
{
	char c, ec;
	while (!inFile.eof())
	{
		inFile.get(c);
		ec = encryptChar(c, key);
		outFile.put(ec);
	}
}

char encryptChar(char c, int key)
{
	char enChar;

	if (c + key > 126)
		enChar = 32 + ((c + key) - 127);
	else
		enChar = c + key;

	return enChar;
}

void declassifyFile(std::ifstream& inFile, std::ofstream& outFile, int key)
{
	char c, dec;
	while (!inFile.eof())
	{
		inFile.get(c);
		dec = declassifyChar(c, key);
		outFile.put(dec);
	}
}

char declassifyChar(char decChar, int key)
{
	decChar -= key;

	if ((decChar > 31 || decChar == 10) && decChar < 126)
		return decChar;
	else
		return decChar - 32 + 127;
}

double readWriteLatestInfo(std::string NormalFilePath, double currentInfo, double deadInfo)
{
	std::string LatestDateInfo;  //上次时间
	std::vector<std::string> LinesVector;
	LinesVector.clear();

	std::ifstream infoIn(NormalFilePath.c_str());
	if (infoIn) // 有该文件  
	{
		std::string line;
		while (getline(infoIn, line)) // line中不包括每行的换行符  
		{
			if (line.length() > 0)
			{
				LinesVector.push_back(line);
			}
		}
	}
	infoIn.close();

	int countLines = LinesVector.size();
	if (countLines < 2)
	{
		return NULL;
	}

	LatestDateInfo = LinesVector[countLines - 2];
	double LL = atof(LatestDateInfo.c_str());

	double dd = deadInfo;
	double mm = currentInfo;

	if (mm > LL && LL > 0 && mm <= dd)
	{
		char buffer[200];
		sprintf_s(buffer, " %f", currentInfo);
		std::string StringIndex = buffer;

		std::string::size_type position;
		position = StringIndex.find(".");                    //第一个 “.”
		std::string subStr = StringIndex.substr(0, position);

		LinesVector[countLines - 1] = subStr;  //写入当前时间
		std::string EndFlag = "Endline";
		LinesVector.push_back(EndFlag);

		countLines = LinesVector.size();
	}

	std::ofstream infoOut(NormalFilePath.c_str());
	//std::ofstream infoOutForSee("C:\\Users\\Bigcat\\Desktop\\forSee.txt");  // just for check

	for (std::vector<std::string>::size_type i = 0; i != countLines; i++)
	{
		if (i >= 14 && i <= countLines - 3)
			continue;

		std::string iString = LinesVector[i];
		infoOut << iString << std::endl;
		//infoOutForSee<<iString<<std::endl;
	}
	infoOut.close();
	//infoOutForSee.close();

	return LL;
}

void EncryptOriginalInfo()
{
	int key = 11;

	std::ifstream inFile;
	std::ofstream outFile;
	inFile.open("C:\\Users\\Bigcat\\Desktop\\secretInfo444.txt");
	if (inFile.fail())
	{
		std::cout << "inFile opening failed.";
		return;
	}
	outFile.open("C:\\Users\\Bigcat\\Desktop\\secretInfo400.txt");
	if (outFile.fail())
	{
		std::cout << "outFile opening failed.";
		return;
	}
	encryptFile(inFile, outFile, key);
	inFile.close();
	outFile.close();
}
//---------------------------------20221220新加------------------------------------------------
//计算一个面的外环相邻面
//输入：顶面
//输出：外环相邻面
void get_External_Adjacent_Faces(NXOpen::Face* ipFace, std::vector<NXOpen::Face*> &ExternalAdjacentFaces)
{
	tag_t ipFace_t = ipFace->Tag();

	std::vector<Edge*> ipFaceExternalEdge;
	uf_loop_p_t loop_list = NULL;
	UF_MODL_ask_face_loops(ipFace_t, &loop_list);
	int loopscount = 0;
	UF_MODL_ask_loop_list_count(loop_list, &loopscount);

	for (int i = 0; i < loopscount; i++)
	{
		int loopstype = 0;
		uf_list_p_t EdgeList = NULL;
		//判断边缘类型 外围=1；孔=2；其他=3.   边的类型都是完整的一圈！
		UF_MODL_ask_loop_list_item(loop_list, i, &loopstype, &EdgeList);
		if (loopstype != 2)
		{
			int EdgeListCount = 0;
			UF_MODL_ask_list_count(EdgeList, &EdgeListCount);
			for (int j = 0; j < EdgeListCount; j++)
			{
				tag_t Edge_Tag = NULL;
				UF_MODL_ask_list_item(EdgeList, j, &Edge_Tag);

				//通过Tag值得到对应的边
				NXObjectManager EdgeManager;
				TaggedObject* pTaggedObject_Edge_Each = EdgeManager.Get(Edge_Tag);
				Edge* pEdge_Each = dynamic_cast<Edge*>(pTaggedObject_Edge_Each);
				//-----------------------------------在筋机匣中无法使用pEdge_Each->GetLength()，会显示无法从边冲提取曲线
				ipFaceExternalEdge.push_back(pEdge_Each);
			}
		}
	}

	for (std::vector<Edge*>::size_type i = 0; i != ipFaceExternalEdge.size(); i++)
	{
		//把内边列表的所有面再取出来存放
		Edge* pEdge_Each = ipFaceExternalEdge[i];
		std::vector<Face*> pEdge_Each_Faces = pEdge_Each->GetFaces();

		for (std::vector<Face*>::size_type j = 0; j != pEdge_Each_Faces.size(); j++)
		{
			Face* pFace_Each = pEdge_Each_Faces[j];
			tag_t pFace_Each_tag = pFace_Each->Tag();
			//判断，首先这个面不能是原面ipFace，其次这个面在之前找的相邻面容器中不能重复
			if (pFace_Each_tag != ipFace_t && Is_Face_in_lpFace(pFace_Each, ExternalAdjacentFaces) == -1)
				ExternalAdjacentFaces.push_back(pFace_Each);
		}
	}
}

logical IsTwoFaceSmooth(Face*EachFace, Face*OtherFace)
{
	std::vector<Edge*>CorEdges;

	GetEdge_From_TwoFaces(EachFace, OtherFace, CorEdges);
	if (CorEdges.size() == 0)
	{
		uc1601("判断面和面是否相切时没有公共边", 1);
		return false;
	}
	logical is_smooth;
	UF_MODL_ask_edge_smoothness(CorEdges[0]->Tag(), 0, &is_smooth);
	if (is_smooth == true)
	{
		return true;
	}
	else
		return false;
}

void GetEdge_From_TwoFaces(NXOpen::Face* EachFace, NXOpen::Face* OtherFace, std::vector<NXOpen::Edge*> &Corporate_EdgeVector)
{
	std::vector<Edge*>EachFace_EdgeVector;
	std::vector<Edge*>OtherFace_EdgeVector;
	EachFace_EdgeVector = EachFace->GetEdges();
	OtherFace_EdgeVector = OtherFace->GetEdges();

	for (std::vector<Edge*>::size_type i = 0; i != EachFace_EdgeVector.size(); i++)
	{
		Edge*EachEdge;
		EachEdge = EachFace_EdgeVector[i];
		for (std::vector<Edge*>::size_type j = 0; j != OtherFace_EdgeVector.size(); j++)
		{
			Edge*OtherEdge;
			OtherEdge = OtherFace_EdgeVector[j];
			if (EachEdge == OtherEdge)
			{
				Corporate_EdgeVector.push_back(EachEdge);
				continue;
			}

		}
	}

	if (Corporate_EdgeVector.size() == 0)
	{
		uc1601("没有公共边", 1);
	}
}

//把容器1塞入容器2
void PutFaceVectorIntoFaceVector(std::vector<NXOpen::Face*>FaceVector1, std::vector<NXOpen::Face*>&FaceVector2)
{
	for (std::vector<Face*>::size_type i = 0; i != FaceVector1.size(); i++)
	{
		FaceVector2.push_back(FaceVector1[i]);
	}
}

//自定义一个对话框
int DifineDialogBox(char *sPromptStr)
{
	//单选菜单对话框
	char *sPromptStr1 = "11111";
	int iDefault = 0;
	char asOptions[][38] = { "是", "否" };
	int iNumOfOtions = 2;
	int value = uc1603(sPromptStr1, iDefault, asOptions, iNumOfOtions);

	return value;

}

//将分布在机匣上的孔进行排序=========进行三等分划
void OrderBodyHole(std::vector<Face*> &CylinderFaces, Face *_ApproachFace)
{
	int Begin_Type;
	double Begin_Point[3];
	double Begin_Dir[3];
	double Begin_Box[6];
	double Begin_Radius[3];
	double Begin_RadData[3];
	int Begin_NormDir;
	UF_MODL_ask_face_data(_ApproachFace->Tag(), &Begin_Type, Begin_Point, Begin_Dir, Begin_Box, Begin_Radius, Begin_RadData, &Begin_NormDir);

	Vector3d _ApproachNormal;
	_ApproachNormal.X = abs(Begin_Dir[0]);
	_ApproachNormal.Y = abs(Begin_Dir[1]);
	_ApproachNormal.Z = abs(Begin_Dir[2]);

	//================================确定机匣是以哪条轴作为轴心线=====================================================
	double absHeight;
	int Flag = 0;
	if (_ApproachNormal.X == 0 && _ApproachNormal.Y == 0 && _ApproachNormal.Z == 1)
	{
		Flag = 1;
		absHeight = Begin_Point[2];
	}
	else if (_ApproachNormal.X == 1 && _ApproachNormal.Y == 0 && _ApproachNormal.Z == 0)
	{
		Flag = 2;
		absHeight = Begin_Point[0];
	}
	else if (_ApproachNormal.X == 0 && _ApproachNormal.Y == 1 && _ApproachNormal.Z == 0)
	{
		Flag = 3;
		absHeight = Begin_Point[1];
	}
	else return;



	class FaceAndVector
	{
	public:
		Face* Cylinder_Face;
		double Standard_Angle;
	};

	std::vector<FaceAndVector*>TopCylinderFaceAndVector_Plus;
	std::vector<FaceAndVector*>TopCylinderFaceAndVector_Minus;
	std::vector<FaceAndVector*>MidCylinderFaceAndVector_Plus;
	std::vector<FaceAndVector*>MidCylinderFaceAndVector_Minus;
	std::vector<FaceAndVector*>BottomCylinderFaceAndVector_Plus;
	std::vector<FaceAndVector*>BottomCylinderFaceAndVector_Minus;

	for (std::vector<Face*>::size_type i = 0; i != CylinderFaces.size(); i++)
	{
		Face *EachFace;
		EachFace = CylinderFaces[i];

		int Type;
		double Point[3];
		double Dir[3];
		double Box[6];
		double Radius[3];
		double RadData[3];
		int NormDir;
		UF_MODL_ask_face_data(EachFace->Tag(), &Type, Point, Dir, Box, Radius, RadData, &NormDir);

		//======以圆心确定矢量=======
		Vector3d CenterPoint;
		Vector3d StandardVector;
		StandardVector.X = 1; StandardVector.Y = 0; StandardVector.Z = 0;

		double EachHoleHeight;
		if (Flag == 1)
		{
			CenterPoint.X = Point[0];
			CenterPoint.Y = Point[1];
			CenterPoint.Z = 0;
			EachHoleHeight = Point[2];
		}
		else if (Flag == 2)
		{
			CenterPoint.X = Point[2];
			CenterPoint.Y = Point[1];
			CenterPoint.Z = 0;
			EachHoleHeight = Point[0];
		}
		else if (Flag == 3)
		{
			CenterPoint.X = Point[2];
			CenterPoint.Y = Point[0];
			CenterPoint.Z = 0;
			EachHoleHeight = Point[1];
		}

		double Angle = get_Angle_Two_vector3d(CenterPoint, StandardVector);
		Angle = Angle * 180 / PI;

		FaceAndVector *newFV = new FaceAndVector;


		if (abs(absHeight - EachHoleHeight)<150)
		{
			if (CenterPoint.Y>0)
			{
				newFV->Cylinder_Face = EachFace;
				newFV->Standard_Angle = Angle;
				TopCylinderFaceAndVector_Plus.push_back(newFV);
			}
			else
			{
				newFV->Cylinder_Face = EachFace;
				newFV->Standard_Angle = Angle;
				TopCylinderFaceAndVector_Minus.push_back(newFV);
			}
		}
		else if (abs(absHeight - EachHoleHeight)<300)
		{
			if (CenterPoint.Y>0)
			{
				newFV->Cylinder_Face = EachFace;
				newFV->Standard_Angle = Angle;
				MidCylinderFaceAndVector_Plus.push_back(newFV);
			}
			else
			{
				newFV->Cylinder_Face = EachFace;
				newFV->Standard_Angle = Angle;
				MidCylinderFaceAndVector_Minus.push_back(newFV);
			}
		}
		else
		{
			if (CenterPoint.Y>0)
			{
				newFV->Cylinder_Face = EachFace;
				newFV->Standard_Angle = Angle;
				BottomCylinderFaceAndVector_Plus.push_back(newFV);
			}
			else
			{
				newFV->Cylinder_Face = EachFace;
				newFV->Standard_Angle = Angle;
				BottomCylinderFaceAndVector_Minus.push_back(newFV);
			}
		}
	}

	//============排序============
	for (std::vector<FaceAndVector*>::size_type i = 0; i != TopCylinderFaceAndVector_Plus.size(); i++)
	{
		for (std::vector<FaceAndVector*>::size_type j = i + 1; j != TopCylinderFaceAndVector_Plus.size(); j++)
		{
			if (TopCylinderFaceAndVector_Plus[i]->Standard_Angle<TopCylinderFaceAndVector_Plus[j]->Standard_Angle)
			{
				FaceAndVector *Temp_FaceAndVector;
				Temp_FaceAndVector = TopCylinderFaceAndVector_Plus[i];
				TopCylinderFaceAndVector_Plus[i] = TopCylinderFaceAndVector_Plus[j];
				TopCylinderFaceAndVector_Plus[j] = Temp_FaceAndVector;
			}
		}
	}
	for (std::vector<FaceAndVector*>::size_type i = 0; i != TopCylinderFaceAndVector_Minus.size(); i++)
	{
		for (std::vector<FaceAndVector*>::size_type j = i + 1; j != TopCylinderFaceAndVector_Minus.size(); j++)
		{
			if (TopCylinderFaceAndVector_Minus[i]->Standard_Angle>TopCylinderFaceAndVector_Minus[j]->Standard_Angle)
			{
				FaceAndVector *Temp_FaceAndVector;
				Temp_FaceAndVector = TopCylinderFaceAndVector_Minus[i];
				TopCylinderFaceAndVector_Minus[i] = TopCylinderFaceAndVector_Minus[j];
				TopCylinderFaceAndVector_Minus[j] = Temp_FaceAndVector;
			}
		}
	}
	for (std::vector<FaceAndVector*>::size_type i = 0; i != MidCylinderFaceAndVector_Plus.size(); i++)
	{
		for (std::vector<FaceAndVector*>::size_type j = i + 1; j != MidCylinderFaceAndVector_Plus.size(); j++)
		{
			if (MidCylinderFaceAndVector_Plus[i]->Standard_Angle<MidCylinderFaceAndVector_Plus[j]->Standard_Angle)
			{
				FaceAndVector *Temp_FaceAndVector;
				Temp_FaceAndVector = MidCylinderFaceAndVector_Plus[i];
				MidCylinderFaceAndVector_Plus[i] = MidCylinderFaceAndVector_Plus[j];
				MidCylinderFaceAndVector_Plus[j] = Temp_FaceAndVector;
			}
		}
	}
	for (std::vector<FaceAndVector*>::size_type i = 0; i != MidCylinderFaceAndVector_Minus.size(); i++)
	{
		for (std::vector<FaceAndVector*>::size_type j = i + 1; j != MidCylinderFaceAndVector_Minus.size(); j++)
		{
			if (MidCylinderFaceAndVector_Minus[i]->Standard_Angle>MidCylinderFaceAndVector_Minus[j]->Standard_Angle)
			{
				FaceAndVector *Temp_FaceAndVector;
				Temp_FaceAndVector = MidCylinderFaceAndVector_Minus[i];
				MidCylinderFaceAndVector_Minus[i] = MidCylinderFaceAndVector_Minus[j];
				MidCylinderFaceAndVector_Minus[j] = Temp_FaceAndVector;
			}
		}
	}

	for (std::vector<FaceAndVector*>::size_type i = 0; i != BottomCylinderFaceAndVector_Plus.size(); i++)
	{
		for (std::vector<FaceAndVector*>::size_type j = i + 1; j != BottomCylinderFaceAndVector_Plus.size(); j++)
		{
			if (BottomCylinderFaceAndVector_Plus[i]->Standard_Angle<BottomCylinderFaceAndVector_Plus[j]->Standard_Angle)
			{
				FaceAndVector *Temp_FaceAndVector;
				Temp_FaceAndVector = BottomCylinderFaceAndVector_Plus[i];
				BottomCylinderFaceAndVector_Plus[i] = BottomCylinderFaceAndVector_Plus[j];
				BottomCylinderFaceAndVector_Plus[j] = Temp_FaceAndVector;
			}
		}
	}
	for (std::vector<FaceAndVector*>::size_type i = 0; i != BottomCylinderFaceAndVector_Minus.size(); i++)
	{
		for (std::vector<FaceAndVector*>::size_type j = i + 1; j != BottomCylinderFaceAndVector_Minus.size(); j++)
		{
			if (BottomCylinderFaceAndVector_Minus[i]->Standard_Angle>BottomCylinderFaceAndVector_Minus[j]->Standard_Angle)
			{
				FaceAndVector *Temp_FaceAndVector;
				Temp_FaceAndVector = BottomCylinderFaceAndVector_Minus[i];
				BottomCylinderFaceAndVector_Minus[i] = BottomCylinderFaceAndVector_Minus[j];
				BottomCylinderFaceAndVector_Minus[j] = Temp_FaceAndVector;
			}
		}
	}

	CylinderFaces.clear();
	for (std::vector<FaceAndVector*>::size_type i = 0; i != TopCylinderFaceAndVector_Plus.size(); i++)
	{
		CylinderFaces.push_back(TopCylinderFaceAndVector_Plus[i]->Cylinder_Face);
	}
	for (std::vector<FaceAndVector*>::size_type i = 0; i != TopCylinderFaceAndVector_Minus.size(); i++)
	{
		CylinderFaces.push_back(TopCylinderFaceAndVector_Minus[i]->Cylinder_Face);
	}
	for (std::vector<FaceAndVector*>::size_type i = 0; i != MidCylinderFaceAndVector_Plus.size(); i++)
	{
		CylinderFaces.push_back(MidCylinderFaceAndVector_Plus[i]->Cylinder_Face);
	}
	for (std::vector<FaceAndVector*>::size_type i = 0; i != MidCylinderFaceAndVector_Minus.size(); i++)
	{
		CylinderFaces.push_back(MidCylinderFaceAndVector_Minus[i]->Cylinder_Face);
	}
	for (std::vector<FaceAndVector*>::size_type i = 0; i != BottomCylinderFaceAndVector_Plus.size(); i++)
	{
		CylinderFaces.push_back(BottomCylinderFaceAndVector_Plus[i]->Cylinder_Face);
	}
	for (std::vector<FaceAndVector*>::size_type i = 0; i != BottomCylinderFaceAndVector_Minus.size(); i++)
	{
		CylinderFaces.push_back(BottomCylinderFaceAndVector_Minus[i]->Cylinder_Face);
	}


}

//把轴向孔的种子面进行排序,暂时把轴向孔分为上下两行吧
void OrderDirectionHole(std::vector<Face*> &CylinderFaces, Face *_ApproachFace)
{
	int Begin_Type;
	double Begin_Point[3];
	double Begin_Dir[3];
	double Begin_Box[6];
	double Begin_Radius[3];
	double Begin_RadData[3];
	int Begin_NormDir;
	UF_MODL_ask_face_data(_ApproachFace->Tag(), &Begin_Type, Begin_Point, Begin_Dir, Begin_Box, Begin_Radius, Begin_RadData, &Begin_NormDir);

	Vector3d _ApproachNormal;
	_ApproachNormal.X = abs(Begin_Dir[0]);
	_ApproachNormal.Y = abs(Begin_Dir[1]);
	_ApproachNormal.Z = abs(Begin_Dir[2]);

	//确定机匣是以哪条轴作为轴心线
	double absHeight;
	int Flag = 0;
	if (_ApproachNormal.X == 0 && _ApproachNormal.Y == 0 && _ApproachNormal.Z == 1)
	{
		Flag = 1;
		absHeight = Begin_Point[2];
	}
	else if (_ApproachNormal.X == 1 && _ApproachNormal.Y == 0 && _ApproachNormal.Z == 0)
	{
		Flag = 2;
		absHeight = Begin_Point[0];
	}
	else if (_ApproachNormal.X == 0 && _ApproachNormal.Y == 1 && _ApproachNormal.Z == 0)
	{
		Flag = 3;
		absHeight = Begin_Point[1];
	}
	else return;

	class FaceAndVector
	{
	public:
		Face* Cylinder_Face;
		double Standard_Angle;
	};

	std::vector<FaceAndVector*>TopCylinderFaceAndVector_Plus;
	std::vector<FaceAndVector*>TopCylinderFaceAndVector_Minus;
	std::vector<FaceAndVector*>BottomCylinderFaceAndVector_Plus;
	std::vector<FaceAndVector*>BottomCylinderFaceAndVector_Minus;

	for (std::vector<Face*>::size_type i = 0; i != CylinderFaces.size(); i++)
	{
		Face *EachFace;
		EachFace = CylinderFaces[i];

		int Type;
		double Point[3];
		double Dir[3];
		double Box[6];
		double Radius[3];
		double RadData[3];
		int NormDir;
		UF_MODL_ask_face_data(EachFace->Tag(), &Type, Point, Dir, Box, Radius, RadData, &NormDir);

		//======以圆心确定矢量=======
		Vector3d CenterPoint;
		Vector3d StandardVector;
		StandardVector.X = 1; StandardVector.Y = 0; StandardVector.Z = 0;

		if (Flag == 1)
		{
			CenterPoint.X = Point[0];
			CenterPoint.Y = Point[1];
			CenterPoint.Z = 0;
			uc1601("0", 1);
		}
		else if (Flag == 2)
		{
			CenterPoint.X = Point[2];
			CenterPoint.Y = Point[1];
			CenterPoint.Z = 0;
		}
		else if (Flag == 3)
		{
			CenterPoint.X = Point[2];
			CenterPoint.Y = Point[0];
			CenterPoint.Z = 0;
		}


		double Angle = get_Angle_Two_vector3d(CenterPoint, StandardVector);
		Angle = Angle * 180 / PI;

		FaceAndVector *newFV = new FaceAndVector;

		if (Flag == 1)
		{
			if (abs(absHeight - Point[2])<15)
			{
				if (CenterPoint.Y>0)
				{
					newFV->Cylinder_Face = EachFace;
					newFV->Standard_Angle = Angle;
					TopCylinderFaceAndVector_Plus.push_back(newFV);
				}
				else
				{
					newFV->Cylinder_Face = EachFace;
					newFV->Standard_Angle = Angle;
					TopCylinderFaceAndVector_Minus.push_back(newFV);
				}
			}
			else
			{
				if (CenterPoint.Y>0)
				{
					newFV->Cylinder_Face = EachFace;
					newFV->Standard_Angle = Angle;
					BottomCylinderFaceAndVector_Plus.push_back(newFV);
				}
				else
				{
					newFV->Cylinder_Face = EachFace;
					newFV->Standard_Angle = Angle;
					BottomCylinderFaceAndVector_Minus.push_back(newFV);
				}
			}
		}
		else if (Flag == 2)
		{
			if (abs(absHeight - Point[0])<15)
			{
				if (CenterPoint.Y>0)
				{
					newFV->Cylinder_Face = EachFace;
					newFV->Standard_Angle = Angle;
					TopCylinderFaceAndVector_Plus.push_back(newFV);
				}
				else
				{
					newFV->Cylinder_Face = EachFace;
					newFV->Standard_Angle = Angle;
					TopCylinderFaceAndVector_Minus.push_back(newFV);
				}
			}
			else
			{
				if (CenterPoint.Y>0)
				{
					newFV->Cylinder_Face = EachFace;
					newFV->Standard_Angle = Angle;
					BottomCylinderFaceAndVector_Plus.push_back(newFV);
				}
				else
				{
					newFV->Cylinder_Face = EachFace;
					newFV->Standard_Angle = Angle;
					BottomCylinderFaceAndVector_Minus.push_back(newFV);
				}
			}
		}
		else if (Flag == 3)
		{
			if (abs(absHeight - Point[1])<15)
			{
				if (CenterPoint.Y>0)
				{
					newFV->Cylinder_Face = EachFace;
					newFV->Standard_Angle = Angle;
					TopCylinderFaceAndVector_Plus.push_back(newFV);
				}
				else
				{
					newFV->Cylinder_Face = EachFace;
					newFV->Standard_Angle = Angle;
					TopCylinderFaceAndVector_Minus.push_back(newFV);
				}
			}
			else
			{
				if (CenterPoint.Y>0)
				{
					newFV->Cylinder_Face = EachFace;
					newFV->Standard_Angle = Angle;
					BottomCylinderFaceAndVector_Plus.push_back(newFV);
				}
				else
				{
					newFV->Cylinder_Face = EachFace;
					newFV->Standard_Angle = Angle;
					BottomCylinderFaceAndVector_Minus.push_back(newFV);
				}
			}
		}
	}

	//============排序============
	for (std::vector<FaceAndVector*>::size_type i = 0; i != TopCylinderFaceAndVector_Plus.size(); i++)
	{
		for (std::vector<FaceAndVector*>::size_type j = i + 1; j != TopCylinderFaceAndVector_Plus.size(); j++)
		{
			if (TopCylinderFaceAndVector_Plus[i]->Standard_Angle<TopCylinderFaceAndVector_Plus[j]->Standard_Angle)
			{
				FaceAndVector *Temp_FaceAndVector;
				Temp_FaceAndVector = TopCylinderFaceAndVector_Plus[i];
				TopCylinderFaceAndVector_Plus[i] = TopCylinderFaceAndVector_Plus[j];
				TopCylinderFaceAndVector_Plus[j] = Temp_FaceAndVector;
			}
		}
	}
	for (std::vector<FaceAndVector*>::size_type i = 0; i != TopCylinderFaceAndVector_Minus.size(); i++)
	{
		for (std::vector<FaceAndVector*>::size_type j = i + 1; j != TopCylinderFaceAndVector_Minus.size(); j++)
		{
			if (TopCylinderFaceAndVector_Minus[i]->Standard_Angle>TopCylinderFaceAndVector_Minus[j]->Standard_Angle)
			{
				FaceAndVector *Temp_FaceAndVector;
				Temp_FaceAndVector = TopCylinderFaceAndVector_Minus[i];
				TopCylinderFaceAndVector_Minus[i] = TopCylinderFaceAndVector_Minus[j];
				TopCylinderFaceAndVector_Minus[j] = Temp_FaceAndVector;
			}
		}
	}
	for (std::vector<FaceAndVector*>::size_type i = 0; i != BottomCylinderFaceAndVector_Plus.size(); i++)
	{
		for (std::vector<FaceAndVector*>::size_type j = i + 1; j != BottomCylinderFaceAndVector_Plus.size(); j++)
		{
			if (BottomCylinderFaceAndVector_Plus[i]->Standard_Angle<BottomCylinderFaceAndVector_Plus[j]->Standard_Angle)
			{
				FaceAndVector *Temp_FaceAndVector;
				Temp_FaceAndVector = BottomCylinderFaceAndVector_Plus[i];
				BottomCylinderFaceAndVector_Plus[i] = BottomCylinderFaceAndVector_Plus[j];
				BottomCylinderFaceAndVector_Plus[j] = Temp_FaceAndVector;
			}
		}
	}
	for (std::vector<FaceAndVector*>::size_type i = 0; i != BottomCylinderFaceAndVector_Minus.size(); i++)
	{
		for (std::vector<FaceAndVector*>::size_type j = i + 1; j != BottomCylinderFaceAndVector_Minus.size(); j++)
		{
			if (BottomCylinderFaceAndVector_Minus[i]->Standard_Angle>BottomCylinderFaceAndVector_Minus[j]->Standard_Angle)
			{
				FaceAndVector *Temp_FaceAndVector;
				Temp_FaceAndVector = BottomCylinderFaceAndVector_Minus[i];
				BottomCylinderFaceAndVector_Minus[i] = BottomCylinderFaceAndVector_Minus[j];
				BottomCylinderFaceAndVector_Minus[j] = Temp_FaceAndVector;
			}
		}
	}

	CylinderFaces.clear();
	for (std::vector<FaceAndVector*>::size_type i = 0; i != TopCylinderFaceAndVector_Plus.size(); i++)
	{
		CylinderFaces.push_back(TopCylinderFaceAndVector_Plus[i]->Cylinder_Face);
	}
	for (std::vector<FaceAndVector*>::size_type i = 0; i != TopCylinderFaceAndVector_Minus.size(); i++)
	{
		CylinderFaces.push_back(TopCylinderFaceAndVector_Minus[i]->Cylinder_Face);
	}
	for (std::vector<FaceAndVector*>::size_type i = 0; i != BottomCylinderFaceAndVector_Plus.size(); i++)
	{
		CylinderFaces.push_back(BottomCylinderFaceAndVector_Plus[i]->Cylinder_Face);
	}
	for (std::vector<FaceAndVector*>::size_type i = 0; i != BottomCylinderFaceAndVector_Minus.size(); i++)
	{
		CylinderFaces.push_back(BottomCylinderFaceAndVector_Minus[i]->Cylinder_Face);
	}
}

bool IsCircleEdgeComplete(Edge* EachEdge)
{
	UF_initialize();
	bool Flag = 0;
	UF_EVAL_arc_t arc_1;
	UF_EVAL_p_t evaluator_1 = NULL;
	UF_EVAL_initialize(EachEdge->Tag(), &evaluator_1);
	UF_EVAL_ask_arc(evaluator_1, &arc_1);
	if (fabs(arc_1.limits[0]) < 0.1 && fabs(arc_1.limits[1] - 2 * PI) < 0.1)
	{
		Flag = 1;
	}
	UF_EVAL_free(evaluator_1);
	UF_terminate();
	return Flag;
}

//获得圆柱面的边的零面  注：只能是简单的双侧单面
void GetOneAdjacentFace(NXOpen::Edge*BeforeEdge, NXOpen::Face*BeforeFace, NXOpen::Face* & AdjacentFace)
{
	std::vector<Face*> Each_FaceVector;
	Each_FaceVector = BeforeEdge->GetFaces();
	if (Each_FaceVector.size() == 2)
	{
		if (Each_FaceVector[0] == BeforeFace)
		{
			AdjacentFace = Each_FaceVector[1];
		}
		else
		{
			AdjacentFace = Each_FaceVector[0];
		}
	}

}

//逐次搜寻孔的相邻面时候用,如果没找到就返回EachFace
void Get_Next_Face(NXOpen::Face*EachFace, NXOpen::Face*UselessFace, NXOpen::Face* &UsefulFace)
{
	std::vector<Face*>Adjacent_FaceVector;
	get_Adjacent_Faces(EachFace, Adjacent_FaceVector);

	if (Adjacent_FaceVector[0] == UselessFace)
	{
		UsefulFace = Adjacent_FaceVector[1];
	}
	else if (Adjacent_FaceVector[1] == UselessFace)
	{
		UsefulFace = Adjacent_FaceVector[0];
	}
	else
	{
		UsefulFace = EachFace;
	}

}

//返回0说明没有找到正确的Edge
bool Get_RightAndWrong_Edge_FromCylinder(NXOpen::Face*EachFace, NXOpen::Edge* &RightEdge, NXOpen::Edge* &WrongEdge)
{
	bool Flag = 1;
	//这里是找寻外侧和内侧边
	std::vector<Edge*>EachFace_EdgeVector;
	EachFace_EdgeVector = EachFace->GetEdges();
	if (EachFace_EdgeVector.size() != 2)
	{
		Flag = 0;
		return Flag;
	}//凸台孔种子面的边必须是两个
	if (IsCircleEdgeComplete(EachFace_EdgeVector[0]) == 0 && IsCircleEdgeComplete(EachFace_EdgeVector[1]) == 0)//两侧均为不完整圆弧
	{
		Flag = 0;
		return Flag;
	}
	else if (IsCircleEdgeComplete(EachFace_EdgeVector[0]) && IsCircleEdgeComplete(EachFace_EdgeVector[1]))//两侧均为完整圆弧
	{
		double point[3]; point[0] = 0.0; point[1] = 0.0; point[2] = 0.0;
		UF_EVAL_arc_t arc_1;
		UF_EVAL_arc_t arc_2;//定义圆
		UF_EVAL_p_t evaluator_1 = NULL;
		UF_EVAL_p_t evaluator_2 = NULL;
		double distance1 = 0; double distance2 = 0;
		UF_EVAL_initialize(EachFace_EdgeVector[0]->Tag(), &evaluator_1);
		UF_EVAL_initialize(EachFace_EdgeVector[1]->Tag(), &evaluator_2);
		UF_EVAL_ask_arc(evaluator_1, &arc_1);
		UF_EVAL_ask_arc(evaluator_2, &arc_2);
		UF_VEC3_distance(arc_1.center, point, &distance1);//把两个点的连线距离赋值到cydistance1里面
		UF_VEC3_distance(arc_2.center, point, &distance2);
		if (distance1>distance2)
		{
			RightEdge = EachFace_EdgeVector[0];
			WrongEdge = EachFace_EdgeVector[1];
			return Flag;
		}
		else
		{
			RightEdge = EachFace_EdgeVector[1];
			WrongEdge = EachFace_EdgeVector[0];
			return Flag;
		}
	}
	else  if (IsCircleEdgeComplete(EachFace_EdgeVector[0]) == 0 && IsCircleEdgeComplete(EachFace_EdgeVector[1]) == 1)//单侧完整圆弧
	{
		RightEdge = EachFace_EdgeVector[1];
		WrongEdge = EachFace_EdgeVector[0];
		return Flag;
	}
	else  if (IsCircleEdgeComplete(EachFace_EdgeVector[0]) == 1 && IsCircleEdgeComplete(EachFace_EdgeVector[1]) == 0)//单侧完整圆弧
	{
		RightEdge = EachFace_EdgeVector[0];
		WrongEdge = EachFace_EdgeVector[1];
		return Flag;
	}
}
//int extractNum(char * str)
int to_int(std::string s)
{

	int i = 0;
	int result = 0;
	for (i = 0; i <= s.size() - 1; i++)
	{
		if (s[i] >= '0'&&s[i] <= '9')
		{
			result = result * 10 + s[i] - 48;
		}
	}
	return result;

	/********** End **********/
}
//弧形槽排序
void OrderCircularGroove(std::vector<Face*> &CylinderFaces, Face *_ApproachFace)
{
	int Begin_Type;
	double Begin_Point[3];
	double Begin_Dir[3];
	double Begin_Box[6];
	double Begin_Radius[3];
	double Begin_RadData[3];
	int Begin_NormDir;
	UF_MODL_ask_face_data(_ApproachFace->Tag(), &Begin_Type, Begin_Point, Begin_Dir, Begin_Box, Begin_Radius, Begin_RadData, &Begin_NormDir);

	Vector3d _ApproachNormal;
	_ApproachNormal.X = abs(Begin_Dir[0]);
	_ApproachNormal.Y = abs(Begin_Dir[1]);
	_ApproachNormal.Z = abs(Begin_Dir[2]);

	//确定机匣是以哪条轴作为轴心线
	double absHeight;
	int Flag = 0;
	if (_ApproachNormal.X == 0 && _ApproachNormal.Y == 0 && _ApproachNormal.Z == 1)
	{
		Flag = 1;
		absHeight = Begin_Point[2];
	}
	else if (_ApproachNormal.X == 1 && _ApproachNormal.Y == 0 && _ApproachNormal.Z == 0)
	{
		Flag = 2;
		absHeight = Begin_Point[0];
	}
	else if (_ApproachNormal.X == 0 && _ApproachNormal.Y == 1 && _ApproachNormal.Z == 0)
	{
		Flag = 3;
		absHeight = Begin_Point[1];
	}
	else return;

	class FaceAndHeight
	{
	public:
		Face* Cylinder_Face;
		double Height;
	};

	std::vector<FaceAndHeight*>CircularGroove_FaceAndHeight;

	for (std::vector<Face*>::size_type i = 0; i != CylinderFaces.size(); i++)
	{
		Face *EachFace;
		EachFace = CylinderFaces[i];

		int Type;
		double Point[3];
		double Dir[3];
		double Box[6];
		double Radius[3];
		double RadData[3];
		int NormDir;
		UF_MODL_ask_face_data(EachFace->Tag(), &Type, Point, Dir, Box, Radius, RadData, &NormDir);

		FaceAndHeight *EachFaceAndHeight = new FaceAndHeight;
		if (Flag == 1)
		{
			EachFaceAndHeight->Height = abs(absHeight - Point[2]);
			EachFaceAndHeight->Cylinder_Face = EachFace;
			CircularGroove_FaceAndHeight.push_back(EachFaceAndHeight);
		}
		else if (Flag == 2)
		{
			EachFaceAndHeight->Height = abs(absHeight - Point[0]);
			EachFaceAndHeight->Cylinder_Face = EachFace;
			CircularGroove_FaceAndHeight.push_back(EachFaceAndHeight);
		}
		else
		{
			EachFaceAndHeight->Height = abs(absHeight - Point[1]);
			EachFaceAndHeight->Cylinder_Face = EachFace;
			CircularGroove_FaceAndHeight.push_back(EachFaceAndHeight);
		}
	}

	//-========================排序=============================
	for (std::vector<FaceAndHeight*>::size_type i = 0; i != CircularGroove_FaceAndHeight.size(); i++)
	{
		for (std::vector<FaceAndHeight*>::size_type j = i + 1; j != CircularGroove_FaceAndHeight.size(); j++)
		{
			if (CircularGroove_FaceAndHeight[i]->Height>CircularGroove_FaceAndHeight[j]->Height)
			{
				FaceAndHeight *Temp_FaceAndVector;
				Temp_FaceAndVector = CircularGroove_FaceAndHeight[i];
				CircularGroove_FaceAndHeight[i] = CircularGroove_FaceAndHeight[j];
				CircularGroove_FaceAndHeight[j] = Temp_FaceAndVector;
			}
		}
	}

	CylinderFaces.clear();
	for (std::vector<FaceAndHeight*>::size_type i = 0; i != CircularGroove_FaceAndHeight.size(); i++)
	{
		CylinderFaces.push_back(CircularGroove_FaceAndHeight[i]->Cylinder_Face);
	}

}
//得到面的整圆数
int Get_FullCircle_Num1(Face* inputFace)
{
	int NumFullcircle(0);
	NXOpen::Point3d vertex1; /** First vertex in the edge */
	NXOpen::Point3d vertex2; /** Second vertex in the edge */
	std::vector<NXOpen::Edge*> SeedFaceEdgeList;
	SeedFaceEdgeList.clear();

	if (inputFace->Tag())
	{
		SeedFaceEdgeList = inputFace->GetEdges();
	}

	for (std::vector<NXOpen::Edge*>::size_type j = 0; j != SeedFaceEdgeList.size(); j++)
	{
		Edge* EachEdge = SeedFaceEdgeList[j];

		int typeN;
		UF_MODL_ask_edge_type(EachEdge->Tag(), &typeN);
		//非 圆弧，排除
		if (typeN != UF_MODL_CIRCULAR_EDGE)   continue;

		EachEdge->GetVertices(&vertex1, &vertex2);
		double subx = fabs(vertex1.X - vertex2.X);
		double suby = fabs(vertex1.Y - vertex2.Y);
		double subz = fabs(vertex1.Z - vertex2.Z);
		// 非 整环， 排除
		if (subx > 0.001 || suby > 0.001 || subz > 0.001)  continue;

		NumFullcircle++;
	}
	return NumFullcircle;
}
//计算一个面的内边
//输入：顶面
//输出：内边Edge
void get_Inner_Edge(NXOpen::Face* ipFace, std::vector<NXOpen::Edge*> &Inner_EdgeVector)//test OK
{
	tag_t ipFace_t = ipFace->Tag();

	uf_loop_p_t loop_list = NULL;
	UF_MODL_ask_face_loops(ipFace_t, &loop_list);
	int loopscount = 0;
	UF_MODL_ask_loop_list_count(loop_list, &loopscount);

	for (int i = 0; i < loopscount; i++)
	{
		int loopstype = 0;
		uf_list_p_t EdgeList = NULL;
		//判断边缘类型 外围=1；孔=2；其他=3.   边的类型都是完整的一圈！
		UF_MODL_ask_loop_list_item(loop_list, i, &loopstype, &EdgeList);
		if (loopstype == 2)
		{
			int EdgeListCount = 0;
			UF_MODL_ask_list_count(EdgeList, &EdgeListCount);
			for (int j = 0; j < EdgeListCount; j++)
			{
				tag_t Edge_Tag = NULL;
				UF_MODL_ask_list_item(EdgeList, j, &Edge_Tag);

				//通过Tag值得到对应的边
				NXObjectManager EdgeManager;
				TaggedObject* pTaggedObject_Edge_Each = EdgeManager.Get(Edge_Tag);
				Edge* pEdge_Each = dynamic_cast<Edge*>(pTaggedObject_Edge_Each);
				Inner_EdgeVector.push_back(pEdge_Each);//存入内边列表中

			}
		}
	}

}
//得到含侧面凸台底角面
//-------------------------基本思想是：与侧面接触的主曲率半径小于80的面，以及前面直接接触种子面的非侧面，，相邻面中主曲率半径太大的作为底面
void GetBossBottomCornerFace(std::vector<Face*> TopFaceVector, std::vector<Face*> SideFaceVector, std::vector<Face*>& BottomCornerFaceVector, std::vector<Face*>& BottomFaceVector)
{
	for (std::vector<Face*>::size_type i = 0; i != SideFaceVector.size(); i++)    //凸台底角面识别有偏差
	{
		Face* EachSideFace = SideFaceVector[i];

		std::vector<Face*> AdjacentFaces;
		get_Adjacent_Faces(EachSideFace, AdjacentFaces);

		for (std::vector<Face*>::size_type j = 0; j != AdjacentFaces.size(); j++)
		{
			Face* EachAdjacentFace = AdjacentFaces[j];

			double Rad(0);
			get_face_maxRad(EachAdjacentFace, Rad);

			//通过曲率来确定是回转面还是小半径的倒角面
			if (Rad <= 80 &&
				Is_Face_in_lpFace(EachAdjacentFace, TopFaceVector) == -1 &&
				Is_Face_in_lpFace(EachAdjacentFace, SideFaceVector) == -1 &&
				Is_Face_in_lpFace(EachAdjacentFace, BottomCornerFaceVector) == -1)
			{
				BottomCornerFaceVector.push_back(EachAdjacentFace);
			}

			if (Rad > 80 &&
				Is_Face_in_lpFace(EachAdjacentFace, TopFaceVector) == -1 &&
				Is_Face_in_lpFace(EachAdjacentFace, SideFaceVector) == -1 &&
				Is_Face_in_lpFace(EachAdjacentFace, BottomFaceVector) == -1)
			{
				BottomFaceVector.push_back(EachAdjacentFace);
			}
		}
	}
}

bool isNXStringrepeat(NXString string, std::vector<NXString> FeatureSpyResultDisplayStringList)
{
	for (std::vector<NXString>::size_type i = 0; i != FeatureSpyResultDisplayStringList.size(); i++)
	{
		if (string.GetUTF8Text() == FeatureSpyResultDisplayStringList[i].GetUTF8Text())
			return 0;
	}
	return 1;
}

//并非整圆
int Get_FullCircle_Num2(Face* inputFace)
{
	int NumFullcircle(0);
	NXOpen::Point3d vertex1; /** First vertex in the edge */
	NXOpen::Point3d vertex2; /** Second vertex in the edge */
	std::vector<NXOpen::Edge*> SeedFaceEdgeList;
	SeedFaceEdgeList.clear();

	if (inputFace->Tag())
	{
		SeedFaceEdgeList = inputFace->GetEdges();
	}

	for (std::vector<NXOpen::Edge*>::size_type j = 0; j != SeedFaceEdgeList.size(); j++)
	{
		Edge* EachEdge = SeedFaceEdgeList[j];

		//int typeN;
		//UF_MODL_ask_edge_type(EachEdge->Tag() , &typeN);
		//非 圆弧，排除
		//if (typeN != UF_MODL_CIRCULAR_EDGE)   continue;

		EachEdge->GetVertices(&vertex1, &vertex2);
		double subx = fabs(vertex1.X - vertex2.X);
		double suby = fabs(vertex1.Y - vertex2.Y);
		double subz = fabs(vertex1.Z - vertex2.Z);
		// 非 整环， 排除
		if (subx > 0.001 || suby > 0.001 || subz > 0.001)  continue;

		NumFullcircle++;
	}
	return NumFullcircle;
}