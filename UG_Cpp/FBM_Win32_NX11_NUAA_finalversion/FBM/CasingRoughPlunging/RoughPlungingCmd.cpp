#include "RoughPlungingCmd.h"
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
#define Epslion 1e-8
using namespace NXOpen;

static UF_CAMBND_boundary_data_p_t create_and_init_bnd_data
(
 UF_CAM_boundary_type_t   type,
 int                      plane_type,
 double                   *origin,
 double                   *matrix,
 UF_CAM_material_side_t   material_side,
 int                      ignore_holes,
 int                      ignore_islands,
 int                      ignore_chamfers,
 UF_CAMBND_app_data_p_t   app_data
 );

//-----------------------------------------------------------------------------
// RoughPlungingCmd : constructor
//-----------------------------------------------------------------------------
RoughPlungingCmd::RoughPlungingCmd():
TaggedObject()
{
}

RoughPlungingCmd::RoughPlungingCmd(std::vector<CircularGroove*> ListOfCavity, std::vector<SubsidenceCavity*> ListOfSubsidenceCavity, std::vector<ArcCavity*> ListOfArcCavity, 
								   std::vector<Boss*> ListOfBoss, std::vector<Rib*> ListOfRib, RoughPlungeProcessParameter ParameterSet, Part* WorkPart, Body* WorkBody, 
								   Body* WorkBlank, std::vector<Face*> ListOfSelectedFace) :
TaggedObject()
{
	CircularGrooveResultList = ListOfCavity;              //环形槽识别结果
	SubsidenceCavityResultList = ListOfSubsidenceCavity;  //下陷槽识别结果
	ArcCavityResultList = ListOfArcCavity;				  //弧形槽识别结果
	BossResultList = ListOfBoss;						  //凸台识别结果
	RibResultList = ListOfRib;                            //筋识别结果

	RoughPlungeParameterSet = ParameterSet;     //工艺决策参数
	workPart = WorkPart;						//工作部件
	selectedBody = WorkBody;					//零件体
	selectedBlank = WorkBlank;//毛坯

	SelectedFaceList = ListOfSelectedFace;
	_Geomtry = RoughPlungeParameterSet.GeomtryMCS ;

	UF_initialize();

	//uc1601("开始编程所选的特征面", 1);
	GeneralRoughPlungingDecision(SelectedFaceList);
	UF_terminate();
}

//-----------------------------------------------------------------------------
// ACRoughPlungingCmd : destructor
//-----------------------------------------------------------------------------
RoughPlungingCmd::~RoughPlungingCmd()
{
}

//获取面中心点法矢，实体默认法矢向外
Vector3d RoughPlungingCmd::get_Centre_fr_Face(Face* ipFace)
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

Point3d RoughPlungingCmd::GetCenterPoint(Face *face)//小满——获得中心点
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

double RoughPlungingCmd::DotProduct(double a[3], double b[3])
{
	double result;
	result = a[0] * b[0] + a[1] * b[1] + a[2] * b[2];
	return result;

}

void RoughPlungingCmd::GeneralRoughPlungingDecision(std::vector<Face*> ListOfSelectedFace)
{
	
	//1 拿到WCS的坐标矩阵，这里要求zc轴向上
	tag_t wcsMatrixTag = NULL_TAG;
	double wcsMatrixValues[9] = { 0.0 };

	tag_t wcsTag = NULL_TAG;
	UF_CSYS_ask_wcs(&wcsTag); //拿到WCS的Tag
	UF_CSYS_ask_matrix_of_object(wcsTag, &wcsMatrixTag);
	UF_CSYS_ask_matrix_values(wcsMatrixTag, wcsMatrixValues);

	double wcsX[3] = { wcsMatrixValues[0], wcsMatrixValues[1], wcsMatrixValues[2] };
	double wcsY[3] = { wcsMatrixValues[3], wcsMatrixValues[4], wcsMatrixValues[5] };
	double wcsZ[3] = { wcsMatrixValues[6], wcsMatrixValues[7], wcsMatrixValues[8] };
	//获取轴向
	NXOpen::Vector3d _Normal(wcsMatrixValues[6], wcsMatrixValues[7], wcsMatrixValues[8]);

	//2. 开始遍历拿到的所有面
	int index = 0;
	for (std::vector<Face*>::size_type i = 0; i != SelectedFaceList.size(); i++)
	{
		Face* EachFace = SelectedFaceList[i];
		tag_t EachFaceTag = EachFace->Tag();

		//判断一下面类型type
		int type = 0;       //判断面类型
		int norm_dir = 0;   //1=外R，-1=内R
		double point[3];  //轴中心 点
		double dir[3];    //轴方向
		double box[6];    //极点坐标
		double radius;    //半径 圆锥=最小半径 + 最大半径 /2
		double rad_data;
		UF_MODL_ask_face_data(EachFace->Tag(), &type, point, dir, box, &radius, &rad_data, &norm_dir);

		//得到面的中心法矢量
		Vector3d iCenterNormal = get_Centre_fr_Face(EachFace);
		Point3d iCenterPoint = GetCenterPoint(EachFace);

		Point *midPoint;
		midPoint = workPart->Points()->CreatePoint(iCenterPoint);

		Direction * direction = workPart->Directions()->CreateDirection(EachFace, midPoint, SenseForward, SmartObject::UpdateOptionAfterModeling);
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

		//设置程序的名字:插铣+tag
		char* StringIndex = new char;
		StringIndex = const_cast<char*>(MCSTagName);
		NXString NXStringIndex = StringIndex;

		NXString iOperationName = "Plunging." + NXStringIndex;

		Session *theSession = Session::GetSession();
		Part *workPart(theSession->Parts()->Work());
		Part *displayPart(theSession->Parts()->Display());
		

		const char * WorkpieceName = nullptr;
		NXString Geomtrypart;
		//创建新的插铣粗加工操作
		CAM::NCGroup *nCGroup1(dynamic_cast<CAM::NCGroup *>(workPart->CAMSetup()->CAMGroupCollection()->FindObject(RoughPlungeParameterSet.PROGRAM)));
		CAM::Method *method1(dynamic_cast<CAM::Method *>(workPart->CAMSetup()->CAMGroupCollection()->FindObject(RoughPlungeParameterSet.METHOD)));
		CAM::Tool *tool1(dynamic_cast<CAM::Tool *>(workPart->CAMSetup()->CAMGroupCollection()->FindObject(RoughPlungeParameterSet.ToolMsg.ToolName)));
		

		if (RoughPlungeParameterSet.IfLCsys)
		{
			
			Vector3d centernormal = get_Centre_fr_Face(EachFace);
			double Facenormal[3] = { centernormal.X, centernormal.Y, centernormal.Z };

			//2.2 获得NX自动捕获的顶面上的坐标原点、矩阵坐标
			//坐标原点
			Point3d centeripoint = GetCenterPoint(EachFace);
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

				if (abs(wcsZ[0])>0.5)
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

			char*Eachname1 = "Plunging.";
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
		operation1 = workPart->CAMSetup()->CAMOperationCollection()->Create(nCGroup1, method1, tool1, featureGeometry1, "mill_contour", "PLUNGE_MILLING", CAM::OperationCollection::UseDefaultNameFalse, iOperationName);
		CAM::PlungeMilling *plungeMilling1(dynamic_cast<CAM::PlungeMilling *>(operation1));
		CAM::PlungeMillingBuilder *plungeMillingBuilder1;
		plungeMillingBuilder1 = workPart->CAMSetup()->CAMOperationCollection()->CreatePlungeMillingBuilder(plungeMilling1);

		//设置固定的刀具轴向
		//plungeMillingBuilder1->ToolAxisFix()->SetToolAxisType(CAM::ToolAxisFixed::TypesFixed);
		CAM::ToolAxisFixed * toolAxisFixed1 = plungeMillingBuilder1->ToolAxisFix();
		toolAxisFixed1->SetToolAxisType(CAM::ToolAxisFixed::TypesFixed);
		toolAxisFixed1->SetVector(iDirection);

		//插削区域
		int NumFaces = 1;
		tag_t AllTag_t[1] = { EachFaceTag };

		int result = UF_CAMGEOM_append_items(operation1->Tag(), UF_CAM_cut_area, NumFaces, AllTag_t, NULL);

		//更新
		NXObject *nXObject;
		nXObject = plungeMillingBuilder1->Commit();

		
		//插削层参数
		CAM::CutLevel* CutLevel1 = plungeMillingBuilder1->CutLevel();
		bool result1 = CutLevel1->InitializeData();   //初始化参数

		CutLevel1->SetRangeType(CAM::CutLevel::RangeTypesSingle);   //RangeTypesAutomatic   RangeTypesSingle     RangeTypesUserDefined
		CutLevel1->SetCutLevelType(CAM::CutLevel::TypesConstant);  //TypesRangeBottom   TypesOptimized

		//工艺参数

		//步距
		switch (RoughPlungeParameterSet.StepDisType)
		{
		case 0:
			plungeMillingBuilder1->BndStepover()->SetStepoverType(CAM::StepoverBuilder::StepoverTypesPercentToolFlat);
			plungeMillingBuilder1->BndStepover()->PercentToolFlatBuilder()->SetValue(RoughPlungeParameterSet.StepDisAmount);
			break;
		case 1:
			plungeMillingBuilder1->BndStepover()->SetStepoverType(CAM::StepoverBuilder::StepoverTypesConstant);
			plungeMillingBuilder1->BndStepover()->DistanceBuilder()->SetIntent(CAM::ParamValueIntentPartUnits);
			plungeMillingBuilder1->BndStepover()->DistanceBuilder()->SetValue(RoughPlungeParameterSet.StepDisAmount);
			break;
		}

		// 走刀模式
		switch (RoughPlungeParameterSet.CuttingMode)
		{
		case 0:
			plungeMillingBuilder1->CutPattern()->SetCutPattern(CAM::CutPatternBuilder::TypesZig);
			break;
		case 1:
			plungeMillingBuilder1->CutPattern()->SetCutPattern(CAM::CutPatternBuilder::TypesZigZag);
			break;
		case 2:
			plungeMillingBuilder1->CutPattern()->SetCutPattern(CAM::CutPatternBuilder::TypesFollowPeriphery); //TypesFollowPeriphery  TypesFollowPart
			break;
		case 3:
			plungeMillingBuilder1->CutPattern()->SetCutPattern(CAM::CutPatternBuilder::TypesProfile);
			break;
		}

		//向前步长
		switch (RoughPlungeParameterSet.ForwardStepType)
		{
		case 0:
			plungeMillingBuilder1->StepAhead()->SetIntent(CAM::ParamValueIntentPartUnits);
			break;
		case 1:
			plungeMillingBuilder1->StepAhead()->SetIntent(CAM::ParamValueIntentToolDep);
			break;
		}
		plungeMillingBuilder1->StepAhead()->SetValue(RoughPlungeParameterSet.ForwardStepAmount);

		//单步向上
		switch (RoughPlungeParameterSet.SingleStepType)
		{
		case 0:
			plungeMillingBuilder1->StepUp()->Distance()->SetIntent(CAM::ParamValueIntentPartUnits);
			break;
		case 1:
			plungeMillingBuilder1->StepUp()->Distance()->SetIntent(CAM::ParamValueIntentToolDep);
			break;
		}
		plungeMillingBuilder1->StepUp()->Distance()->SetValue(RoughPlungeParameterSet.SingleStepAmount);

		//最大切宽
		switch (RoughPlungeParameterSet.MaxCutDisType)
		{
		case 0:
			plungeMillingBuilder1->MaxCutWidth()->SetIntent(CAM::ParamValueIntentPartUnits);
			break;
		case 1:
			plungeMillingBuilder1->MaxCutWidth()->SetIntent(CAM::ParamValueIntentToolDep);
			break;
		}
		plungeMillingBuilder1->MaxCutWidth()->SetValue(RoughPlungeParameterSet.MaxCutDisAmount);

		//切削参数----切削方向----顺逆铣
		switch (RoughPlungeParameterSet.CuttingDirection)
		{
		case 0:
			plungeMillingBuilder1->CutParameters()->CutDirection()->SetType(CAM::CutDirection::TypesConventional);
			break;
		case 1:
			plungeMillingBuilder1->CutParameters()->CutDirection()->SetType(CAM::CutDirection::TypesClimb);
			break;
		}

		//切削参数----切削角
		plungeMillingBuilder1->CutParameters()->CutAngle()->SetType(CAM::CutAngle::TypesAuto);  //  TypesSpecify

		//切削参数----余量设置
		plungeMillingBuilder1->CutParameters()->SetFloorSameAsPartStock(true);
		plungeMillingBuilder1->CutParameters()->PartStock()->SetValue(RoughPlungeParameterSet.PartAllowance);

		plungeMillingBuilder1->CutParameters()->TrimStock()->SetValue(0.0);

		plungeMillingBuilder1->CutParameters()->CheckStock()->SetValue(0.0);

		plungeMillingBuilder1->CutParameters()->BlankStock()->SetValue(0.0);

		//切削参数----公差
		CAM::Inheritable2dLength *inheritable2dLength1 = plungeMillingBuilder1->CutParameters()->Tolerances();
		inheritable2dLength1->SetValue0(RoughPlungeParameterSet.Tolerance);
		inheritable2dLength1->SetValue1(0.03);  //外公差


		//安全平面设定  自动
		plungeMillingBuilder1->SetTransferMethod(CAM::PlungeMillingBuilder::TransferMethodTypesAutomaticPlane);

		//退刀
		plungeMillingBuilder1->RetractDistance()->SetValue(RoughPlungeParameterSet.RetracttingDis);
		plungeMillingBuilder1->RetractAngle()->SetValue(RoughPlungeParameterSet.RetracttingAngle);

		//更新操作
		//NXObject *nXObject;
		nXObject = plungeMillingBuilder1->Commit();
		plungeMillingBuilder1->Destroy();

		//生成刀轨
		std::vector<CAM::CAMObject *> objects1(1);
		CAM::PlungeMilling *plungeMilling2(dynamic_cast<CAM::PlungeMilling *>(nXObject));
		objects1[0] = plungeMilling2;
		workPart->CAMSetup()->GenerateToolPath(objects1);

		//刷新
		UF_UI_ONT_refresh();

	}


}


static UF_CAMBND_boundary_data_p_t create_and_init_bnd_data
(
 UF_CAM_boundary_type_t   type,
 int                      plane_type,
 double                   *origin,
 double                   *matrix,
 UF_CAM_material_side_t   material_side,
 int                      ignore_holes,
 int                      ignore_islands,
 int                      ignore_chamfers,
 UF_CAMBND_app_data_p_t   app_data
 )
{
	UF_CAMBND_boundary_data_p_t bnd_data;
	int                         i, err_code;

	bnd_data = (UF_CAMBND_boundary_data_p_t) UF_allocate_memory(
		sizeof(UF_CAMBND_boundary_data_t), &err_code);

	bnd_data->boundary_type = type;
	bnd_data->plane_type = plane_type ;
	for (i=0; i<3; i++)
		bnd_data->origin[i] = origin[i] ;
	for (i=0; i<9; i++)
		bnd_data->matrix[i] = matrix[i] ;
	bnd_data->material_side = material_side;
	bnd_data->ignore_holes = ignore_holes ;
	bnd_data->ignore_islands = ignore_islands ; 
	bnd_data->ignore_chamfers = ignore_chamfers ;    
	bnd_data->app_data = app_data;

	return(bnd_data);
}

UF_CAMBND_app_data_p_t RoughPlungingCmd::create_and_init_bnd_app_data(void)
{
	UF_CAMBND_app_data_p_t  app_data; 
	int                     err_code;

	app_data = (UF_CAMBND_app_data_p_t) UF_allocate_memory(
		sizeof(UF_CAMBND_app_data_t), &err_code);

	if (err_code)
		return(NULL);

	/* Set flags. */
	app_data->has_stock = 0;
	app_data->has_tolerances = 0 ;
	app_data->has_feedrate = 0 ;
	app_data->has_blank_distance = 0;
	app_data->has_tool_position = 0;

	/* Set values. */
	app_data->stock = 0.1 ;
	app_data->tolerances[0] = 0.1 ;
	app_data->tolerances[1] = 0.1 ;
	app_data->feedrate_unit = UF_CAM_feedrate_unit_per_minute ;
	app_data->feedrate_value = 0.1 ;
	app_data->blank_distance = 0.0 ;
	app_data->tool_position = UF_CAM_tool_position_on;

	return(app_data);
}