#include "DLBossSideMillingCmd.h"
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


DLBossSideMillingCmd::DLBossSideMillingCmd():
TaggedObject()
{
}

DLBossSideMillingCmd::DLBossSideMillingCmd(std::vector<DoubleLayerBoss*> ListOfNBoss, BossSideProcessParameter ParameterSet, Part* WorkPart, Body* WorkBody, Body* WorkBlank, std::vector<Face*>ListOfSelectedFace, std::vector<Face*>ListOfSelectedZhiDingFace) :
TaggedObject()
{
	BossResultList = ListOfNBoss;      
	BossParameterSet = ParameterSet;  
	workPart = WorkPart;              
	selectedBody = WorkBody;      
	selectedBlank = WorkBlank;
	SelectedBossTopFaceList = ListOfSelectedFace;
	SelectedZhiDingFaceList = ListOfSelectedZhiDingFace;
	_Normal = BossParameterSet.Approch_Normal;

	UF_initialize();

	if (BossParameterSet.FaceSelect&&BossParameterSet.syspart != 0)
	{
		std::vector<Face*> SelectedFaceList2;
		for (std::vector<Face*>::size_type i = 0; i != SelectedBossTopFaceList.size(); i++)
		{
			Face* EachFace = SelectedBossTopFaceList[i];
			tag_t EachFaceTag = EachFace->Tag();
			for (std::vector<DoubleLayerBoss*>::size_type j = 0; j != BossResultList.size(); j++)
			{
				DoubleLayerBoss* EachBoss = BossResultList[j];
				Face* EachBossTopFace = EachBoss->TopFaceVector[0];
				Face* EachBossnTopFace = EachBoss->NTopFaceVector[0];
				if (EachFaceTag == EachBossTopFace->Tag() && strstr(EachBoss->name, "多层凸台") != NULL)
				{
					SelectedFaceList2.push_back(EachBoss->TopFaceVector[0]);
					SelectedFaceList2.push_back(EachBoss->NTopFaceVector[0]);
				}
				else if (EachFaceTag == EachBossnTopFace->Tag() && strstr(EachBoss->name, "多层凸台") != NULL)
				{
					SelectedFaceList2.push_back(EachBoss->TopFaceVector[0]);
					SelectedFaceList2.push_back(EachBoss->NTopFaceVector[0]);
				}
			}
		}
		int k = 0;
		if (k != SelectedFaceList2.size())
		{
			BossSideMillingDecision1(SelectedFaceList2,SelectedZhiDingFaceList);
		}
	}
	else if (BossParameterSet.MOREboss&&BossParameterSet.syspart != 0)
	{
		std::vector<Face*> AllBossTopFaceList;
		for (std::vector<DoubleLayerBoss*>::size_type i = 0; i != ListOfNBoss.size(); i++)//取出所有凸台顶面
		{
			DoubleLayerBoss* EachBoss = ListOfNBoss[i];
			Face* BossTopFace1 = EachBoss->TopFaceVector[0];

			AllBossTopFaceList.push_back(BossTopFace1);

			Face* BossTopFace2 = EachBoss->NTopFaceVector[0];

			AllBossTopFaceList.push_back(BossTopFace2);

		}

		BossSideMillingDecision1(AllBossTopFaceList,SelectedZhiDingFaceList);
	}
	
	UF_terminate();
}

//-----------------------------------------------------------------------------
// BossSideMillingCmd : destructor
//-----------------------------------------------------------------------------
DLBossSideMillingCmd::~DLBossSideMillingCmd()
{
}

Point3d DLBossSideMillingCmd::GetCenterPoint(Face *face)
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

static int select_filter_proc_fn(tag_t object, int type[3], void* user_data, UF_UI_selection_p_t select)
{
	if (object == NULL)
	{
		return UF_UI_SEL_REJECT;
	}
	else
	{
		return UF_UI_SEL_ACCEPT;
	}
}

static int init_proc(UF_UI_selection_p_t select, void* user_data)
{
	int num_triples = 3;
	UF_UI_mask_t mask_triples[] =
	{ UF_point_type, UF_point_subtype, UF_UI_SEL_NOT_A_FEATURE,
	UF_line_type, UF_line_normal_subtype, UF_UI_SEL_NOT_A_FEATURE,
	UF_solid_type, UF_solid_body_subtype, UF_UI_SEL_FEATURE_BODY
	};
	UF_UI_set_sel_mask(select, UF_UI_SEL_MASK_CLEAR_AND_ENABLE_SPECIFIC, num_triples, mask_triples);
	if ((UF_UI_set_sel_procs(select, select_filter_proc_fn, NULL, user_data)) == 0)
	{
		return UF_UI_SEL_SUCCESS;
	}
	else
	{
		return UF_UI_SEL_FAILURE;
	}
}

tag_t DLBossSideMillingCmd::SelectPart()
{
	int count = 0, resp = 0;
	double cursor[3] = { 0 };
	tag_t objTag = NULL_TAG, view = NULL_TAG;
	UF_UI_select_with_single_dialog("请选择工件", "工件选择对话框", UF_UI_SEL_SCOPE_WORK_PART, init_proc, NULL, &resp, &objTag, cursor, &view);
	if (resp >= UF_UI_OK && objTag != NULL_TAG)
	{
		UF_DISP_set_highlight(objTag, 0);
	}
	else
		objTag = NULL_TAG;

	return objTag;
}

tag_t DLBossSideMillingCmd::SelectBlank()
{
	int count = 0, resp = 0;
	double cursor[3] = { 0 };
	tag_t objTag = NULL_TAG, view = NULL_TAG;
	UF_UI_select_with_single_dialog("请选择毛坯", "毛坯选择对话框", UF_UI_SEL_SCOPE_WORK_PART, init_proc, NULL, &resp, &objTag, cursor, &view);
	if (resp >= UF_UI_OK && objTag != NULL_TAG)
	{
		UF_DISP_set_highlight(objTag, 0);
	}
	else
		objTag = NULL_TAG;

	return objTag;
}


Vector3d DLBossSideMillingCmd::get_Centre_fr_Face(Face* ipFace)
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


double DLBossSideMillingCmd::DotProduct(double a[3], double b[3])
{
	double result;
	result = a[0] * b[0] + a[1] * b[1] + a[2] * b[2];
	return result;

}

void DLBossSideMillingCmd::BossSideMillingDecision1(std::vector<Face*> BossTopFaceList, std::vector<Face*> SelectedZhiDingFace)
{
	tag_t wcsMatrixTag = NULL_TAG;
	double wcsMatrixValues[9] = { 0.0 };

	tag_t wcsTag = NULL_TAG;
	UF_CSYS_ask_wcs(&wcsTag);
	UF_CSYS_ask_matrix_of_object(wcsTag, &wcsMatrixTag);
	UF_CSYS_ask_matrix_values(wcsMatrixTag, wcsMatrixValues);

	double wcsX[3] = { wcsMatrixValues[0], wcsMatrixValues[1], wcsMatrixValues[2] };
	double wcsY[3] = { wcsMatrixValues[3], wcsMatrixValues[4], wcsMatrixValues[5] };
	double wcsZ[3] = { wcsMatrixValues[6], wcsMatrixValues[7], wcsMatrixValues[8] };

	char msg[256];
	sprintf(msg, "工作坐标系WCS矩阵为：\n%f, %f, %f,\n%f, %f, %f, \n%f, %f, %f\n\n",
		wcsMatrixValues[0], wcsMatrixValues[1], wcsMatrixValues[2],
		wcsMatrixValues[3], wcsMatrixValues[4], wcsMatrixValues[5],
		wcsMatrixValues[6], wcsMatrixValues[7], wcsMatrixValues[8]);
	UF_UI_write_listing_window(msg);

	int index = 0;
	for (std::vector<Face*>::size_type i = 0; i != BossTopFaceList.size(); i++)
	{
		Face* BossTopFace = BossTopFaceList[i];
		tag_t TopFaceTag = BossTopFace->Tag();

		int iftopface = 0;
		int GaiTuTai;
		for (std::vector<DoubleLayerBoss*>::size_type j = 0; j != BossResultList.size(); j++)
		{
			DoubleLayerBoss* EachBoss = BossResultList[j];
			Face* EachBossTopFace = EachBoss->TopFaceVector[0];
			Face* EachBossNTopFace = EachBoss->NTopFaceVector[0];
			if (TopFaceTag == EachBossTopFace->Tag() && (strstr(EachBoss->name, "多层凸台") != NULL))
			{
				iftopface = 1;
				GaiTuTai = j;
			}
			if (TopFaceTag == EachBossNTopFace->Tag() && (strstr(EachBoss->name, "多层凸台") != NULL))
			{
				iftopface = 0;
				GaiTuTai = j;
			}
		}
		

		//判断一下面类型type
		int type = 0;       //判断面类型
		int norm_dir = 0;   //1=外R，-1=内R
		double point[3];  //轴中心 点
		double dir[3];    //轴方向
		double box[6];    //极点坐标
		double radius;    //半径 圆锥=最小半径 + 最大半径 /2
		double rad_data;
		UF_MODL_ask_face_data(TopFaceTag, &type, point, dir, box, &radius, &rad_data, &norm_dir);

		//得到面的中心法矢量
		Vector3d iCenterNormal = get_Centre_fr_Face(BossTopFace);
		Point3d iCenterPoint = GetCenterPoint(BossTopFace);

		Point *midPoint;
		midPoint = workPart->Points()->CreatePoint(iCenterPoint);

		Direction * direction = workPart->Directions()->CreateDirection(BossTopFace, midPoint, SenseForward, SmartObject::UpdateOptionAfterModeling);
		NXOpen::Vector3d Dir1 = direction->Vector();
		NXOpen::Vector3d MidDir;
		MidDir.X = Dir1.X;
		MidDir.Y = Dir1.Y;
		MidDir.Z = Dir1.Z;

		Direction *iDirection;
		iDirection = workPart->Directions()->CreateDirection(iCenterPoint, MidDir, SmartObject::UpdateOptionAfterModeling);

		//获取面的TAG值

		std::string tagStr = std::to_string(TopFaceTag);
		const char *MCSTagName = tagStr.c_str();


		Session *theSession = Session::GetSession();
		Part *workPart(theSession->Parts()->Work());
		Part *displayPart(theSession->Parts()->Display());


		const char * WorkpieceName = nullptr;
		NXString Geomtrypart;


		Vector3d centernormal = get_Centre_fr_Face(BossTopFace);
		double Facenormal[3] = { centernormal.X, centernormal.Y, centernormal.Z };

		//2.2 获得NX自动捕获的顶面上的坐标原点、矩阵坐标
		//坐标原点
		Point3d centeripoint = GetCenterPoint(BossTopFace);
		double origin[3] = { centeripoint.X, centeripoint.Y, centeripoint.Z };//origin是原点

		//坐标矩阵
		tag_t MatrixTag = NULL_TAG;//创建空的对象矩阵tag

		UF_CSYS_ask_matrix_of_object(TopFaceTag, &MatrixTag); //获得矩阵tag
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

			if (abs(wcsZ[0]) > 0.5)
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
		if (BossParameterSet.LCsys)
		{

			tag_t MatrixID = NULL_TAG;
			UF_CSYS_create_matrix(targetMTx, &MatrixID);

			double CsysOrigin[3] = { origin[0], origin[1], origin[2] };
			tag_t CsysId = NULL_TAG;

			const char *MCSName = nullptr;
			std::string tagStr = std::to_string(TopFaceTag);
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
				/*uc1601("已存在该坐标系", 1);*/
				/*char*Eachname1 = "已存在同名坐标系:";
				int len = strlen(Eachname1);
				char* tempEachName = new char[len + 1];
				strcpy(tempEachName, Eachname1);

				int lenSuffix = strlen(MCSName);
				int lenB = strlen(tempEachName);
				char* newString = new char[lenB + lenSuffix + 1];
				strcpy(newString, tempEachName);
				char* WorkpieceName;
				WorkpieceName = strcat(newString, MCSName);

				uc1601(WorkpieceName, 1);*/
			}

			//在坐标系下创建新的几何体
			tag_t  newWorkpiece = NULL_TAG;
			UF_NCGEOM_create("mill_contour", "WORKPIECE", &newWorkpiece);
			UF_UI_ONT_refresh();

			char*Eachname1 = "BossMilling.";
			int len = strlen(Eachname1);
			char* tempEachName = new char[len + 1];
			strcpy(tempEachName, Eachname1);

			const char* suffix = MCSTagName;
			int lenSuffix = strlen(suffix);
			int lenB = strlen(tempEachName);
			char* newString = new char[lenB + lenSuffix + 1];
			strcpy(newString, tempEachName);
			WorkpieceName = strcat(newString, suffix);

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

								/*tag_t entity_list[1] = { PartTag };
								UF_CAMGEOM_append_items(newWorkpiece, UF_CAM_part, 1, entity_list, NULL);*/

								//设置毛坯

								/*tag_t blank_list[1] = { blankTag };
								UF_CAMGEOM_append_items(newWorkpiece, UF_CAM_blank, 1, blank_list, NULL);*/

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
				/*uc1601("已存在同名称WORKPIECE", 1);*/
			}

		}
		else
		{

			WorkpieceName = "WORKPIECE";

		}
		

		CAM::NCGroup *nCGroup1(dynamic_cast<CAM::NCGroup *>(workPart->CAMSetup()->CAMGroupCollection()->FindObject(BossParameterSet.PROGRAM)));
		CAM::Method *method1(dynamic_cast<CAM::Method *>(workPart->CAMSetup()->CAMGroupCollection()->FindObject(BossParameterSet.METHOD)));
		CAM::Tool *tool1(dynamic_cast<CAM::Tool *>(workPart->CAMSetup()->CAMGroupCollection()->FindObject(BossParameterSet.ToolMsg.ToolName)));
		CAM::FeatureGeometry *featureGeometry1(dynamic_cast<CAM::FeatureGeometry *>(workPart->CAMSetup()->CAMGroupCollection()->FindObject(WorkpieceName)));
		CAM::Operation *operation1;
		char* StringIndex = new char;
		sprintf(StringIndex, "%d", i + 1);
		NXString NXStringIndex = StringIndex;
		delete StringIndex;
		NXString nxString(tagStr.c_str());
		NXString OperationName = "BossSideMilling." + nxString;
		try
		{
			operation1 = workPart->CAMSetup()->CAMOperationCollection()->Create(nCGroup1, method1, tool1, featureGeometry1, "mill_contour", "FIXED_CONTOUR", CAM::OperationCollection::UseDefaultNameFalse, OperationName);
		}
		catch (...)
		{
			operation1 = workPart->CAMSetup()->CAMOperationCollection()->Create(nCGroup1, method1, tool1, featureGeometry1, "mill_contour", "FIXED_CONTOUR", CAM::OperationCollection::UseDefaultNameTrue, OperationName);
		}
		CAM::SurfaceContour *surfaceContour1(dynamic_cast<CAM::SurfaceContour *>(operation1));
		CAM::SurfaceContourBuilder *surfaceContourBuilder1;
		surfaceContourBuilder1 = workPart->CAMSetup()->CAMOperationCollection()->CreateSurfaceContourBuilder(surfaceContour1);

		CAM::ToolAxisFixed *toolAxisFixed1;
		toolAxisFixed1 = surfaceContourBuilder1->ToolAxisFixed();
		toolAxisFixed1->SetToolAxisType(CAM::ToolAxisFixed::TypesFixed);

		Point *midPoint1;
		midPoint1 = workPart->Points()->CreatePoint(centeripoint);
		Direction * direction1;
		direction1 = workPart->Directions()->CreateDirection(BossTopFace, midPoint1, SenseForward, SmartObject::UpdateOptionAfterModeling);
		toolAxisFixed1->SetVector(direction1);

		surfaceContourBuilder1->SetDriveMethod(CAM::SurfaceContourBuilder::DriveMethodTypesCurve);
		surfaceContourBuilder1->SetCleanupType(CAM::SurfaceContourBuilder::CleanupTypesOff);

		//指定部件为底角面
	    /*上顶面加工:将底角面和下面一层凸台的顶面指定为部件
	     次顶面加工 : 将底角面和上面一层凸台的顶面和侧面指定为部件*/
		
		/*if (iftopface == 1)
		{
			std::vector<Face*> DijiaoFace = BossResultList[GaiTuTai]->BottomCornerFaceVector;
			std::vector<Face*> NtopFace = BossResultList[GaiTuTai]->NTopFaceVector;
			for (int i = 0; i < DijiaoFace.size(); i++)
			{
				tag_t DijiaoFacetag = DijiaoFace[i]->Tag();
				UF_CAMGEOM_append_items(operation1->Tag(), UF_CAM_part, 1, &DijiaoFacetag, NULL);
			}
			for (int j = 0; j < NtopFace.size(); j++)
			{
				tag_t NtopFacetag = NtopFace[j]->Tag();
				UF_CAMGEOM_append_items(operation1->Tag(), UF_CAM_part, 1, &NtopFacetag, NULL);
			}
		}
		else
		{
			std::vector<Face*> DijiaoFace = BossResultList[GaiTuTai]->BottomCornerFaceVector;
			std::vector<Face*> TopFace = BossResultList[GaiTuTai]->TopFaceVector;
			std::vector<Face*> SideFace = BossResultList[GaiTuTai]->SideFaceVector;
			for (int i = 0; i < DijiaoFace.size(); i++)
			{
				tag_t DijiaoFacetag = DijiaoFace[i]->Tag();
				UF_CAMGEOM_append_items(operation1->Tag(), UF_CAM_part, 1, &DijiaoFacetag, NULL);
			}
			for (int j = 0; j < TopFace.size(); j++)
			{
				tag_t TopFacetag = TopFace[j]->Tag();
				UF_CAMGEOM_append_items(operation1->Tag(), UF_CAM_part, 1, &TopFacetag, NULL);
			}
			for (int k = 0; k < SideFace.size(); k++)
			{
				tag_t SideFacetag = SideFace[k]->Tag();
				UF_CAMGEOM_append_items(operation1->Tag(), UF_CAM_part, 1, &SideFacetag, NULL);
			}
		}*/
		//指定部件为所选面

		/*if (BossParameterSet.PointPart)
		{
		}*/
		for (int i = 0; i < SelectedZhiDingFace.size(); i++)
		{
			Face *ZhiDing = SelectedZhiDingFace[i];
			tag_t ZhiDingfacetag = ZhiDing->Tag();
			UF_CAMGEOM_append_items(operation1->Tag(), UF_CAM_part, 1, &ZhiDingfacetag, NULL);
		}
		double ToolDiameter = BossParameterSet.ToolMsg.ToolDiameter;
		double toolradius = ToolDiameter / 2;
		//创建偏置曲线操作
		NXOpen::Features::Feature *nullNXOpen_Features_Feature(NULL);
		NXOpen::Features::OffsetCurveBuilder *offsetCurveBuilder1;
		offsetCurveBuilder1 = workPart->Features()->CreateOffsetCurveBuilder(nullNXOpen_Features_Feature);
		NXOpen::NXObject *nullNXOpen_NXObject(NULL);
		NXOpen::Unit *unit1;
		unit1 = offsetCurveBuilder1->OffsetDistance()->Units();

		NXOpen::Expression *expression1;
		expression1 = workPart->Expressions()->CreateSystemExpressionWithUnits("0", unit1);
		offsetCurveBuilder1->CurveFitData()->SetTolerance(0.025399999999999999);
		offsetCurveBuilder1->CurveFitData()->SetAngleTolerance(0.5);
		////设置偏置距离
		offsetCurveBuilder1->OffsetDistance()->SetValue(toolradius);
		offsetCurveBuilder1->DraftHeight()->SetRightHandSide("5");
		offsetCurveBuilder1->DraftAngle()->SetRightHandSide("0");
		offsetCurveBuilder1->LawControl()->Value()->SetRightHandSide("5");
		offsetCurveBuilder1->LawControl()->StartValue()->SetRightHandSide("5");
		offsetCurveBuilder1->LawControl()->EndValue()->SetRightHandSide("5");
		offsetCurveBuilder1->Offset3dDistance()->SetRightHandSide("5");
		offsetCurveBuilder1->InputCurvesOptions()->SetAssociative(false);
		offsetCurveBuilder1->InputCurvesOptions()->SetInputCurveOption(NXOpen::GeometricUtilities::CurveOptions::InputCurveReplace);
		offsetCurveBuilder1->InputCurvesOptions()->SetInputCurveOption(NXOpen::GeometricUtilities::CurveOptions::InputCurveBlank);
		offsetCurveBuilder1->SetTrimMethod(NXOpen::Features::OffsetCurveBuilder::TrimOptionExtendTangents);
		offsetCurveBuilder1->LawControl()->AlongSpineData()->SetFeatureSpine(offsetCurveBuilder1->CurvesToOffset());

		NXOpen::Expression *expression2;
		expression2 = workPart->Expressions()->CreateSystemExpressionWithUnits("0", unit1);
		offsetCurveBuilder1->CurvesToOffset()->SetDistanceTolerance(0.025399999999999999);
		offsetCurveBuilder1->CurvesToOffset()->SetChainingTolerance(0.024129999999999999);
		offsetCurveBuilder1->LawControl()->AlongSpineData()->Spine()->SetDistanceTolerance(0.025399999999999999);
		offsetCurveBuilder1->LawControl()->AlongSpineData()->Spine()->SetChainingTolerance(0.024129999999999999);
		offsetCurveBuilder1->LawControl()->LawCurve()->SetDistanceTolerance(0.025399999999999999);
		offsetCurveBuilder1->LawControl()->LawCurve()->SetChainingTolerance(0.024129999999999999);
		offsetCurveBuilder1->CurvesToOffset()->SetAngleTolerance(0.5);
		offsetCurveBuilder1->LawControl()->AlongSpineData()->Spine()->SetAngleTolerance(0.5);
		offsetCurveBuilder1->LawControl()->LawCurve()->SetAngleTolerance(0.5);
		offsetCurveBuilder1->CurvesToOffset()->SetAllowedEntityTypes(NXOpen::Section::AllowTypesOnlyCurves);
		


		//获取面的边
		std::vector<NXOpen::Edge*>edgevector;
		edgevector = BossTopFace->GetEdges();

		int circleedge;
		for (int k = 0; k < edgevector.size(); k++)
		{
			Edge *edge = edgevector[k];
			tag_t edgetag = edge->Tag();
			int edgetype;
			UF_MODL_ask_edge_type(edgetag, &edgetype);

			if (edgetype == UF_MODL_CIRCULAR_EDGE)
			{
				NXOpen::Point3d point1;
				NXOpen::Point3d point2;
				edge->GetVertices(&point1, &point2);

				int ifedge = 0;
				for (int j = 0; j < edgevector.size(); j++)
				{
					if (k == j)
					{
						continue;
					}
					else
					{
						Edge *edge = edgevector[j];
						NXOpen::Point3d point3;
						NXOpen::Point3d point4;
						edge->GetVertices(&point3, &point4);
						if (point1.X == point3.X&&point1.Y == point3.Y&&point1.Z == point3.Z)
						{
							ifedge = 1;
						}
						else if (point1.X == point4.X&&point1.Y == point4.Y&&point1.Z == point4.Z)
						{
							ifedge = 1;
						}
					}
				}

				if (ifedge == 1)
				{
					circleedge = k;
					break;
				}


			}
		}


		/*for (int k = 0; k < edgevector.size(); k++)
		{

			tag_t edgetag = edgevector[k]->Tag();
			int edgetype;
			UF_MODL_ask_edge_type(edgetag, &edgetype);

			switch (edgetype)
			{
			case 3001:
				uc1601("3001", 1);
				break;
			case 3002:
				uc1601("3002", 1);
				break;
			case 3003:
				uc1601("3003", 1);
				break;
			case 3004:
				uc1601("3004", 1);
				break;
			case 3005:
				uc1601("3005", 1);
				break;
			case 3006:
				uc1601("3006", 1);
				break;
			case 3007:
				uc1601("3007", 1);
				break;
			case 3008:
				uc1601("3008", 1);
				break;
			case 3009:
				uc1601("3009", 1);
				break;
			case 100007:
				uc1601("100007", 1);
				break;
			default:
				uc1601("zhaobudao", 1);
				break;
			}


		}*/



		int lineedge;
		for (int k = 0; k < edgevector.size(); k++)
		{

			tag_t edgetag = edgevector[k]->Tag();
			int edgetype;
			UF_MODL_ask_edge_type(edgetag, &edgetype);

			//如果有一个边不为圆和椭圆,说明不是圆形凸台
			if (edgetype != UF_MODL_CIRCULAR_EDGE&&edgetype != UF_MODL_ELLIPTICAL_EDGE&&edgetype != UF_MODL_INTERSECTION_EDGE)
			{
				lineedge = k;
				break;
			}
		}



		int type3004num = 0;
		int type3003num = 0;
		for (int k = 0; k < edgevector.size(); k++)
		{

			tag_t edgetag = edgevector[k]->Tag();
			int edgetype;
			UF_MODL_ask_edge_type(edgetag, &edgetype);

			if (edgetype == UF_MODL_INTERSECTION_EDGE)
			{
				type3004num = type3004num + 1;
			}
			if (edgetype == UF_MODL_ELLIPTICAL_EDGE)
			{
				type3003num = type3003num + 1;
			}
		}


		if (iftopface == 1)
		{
			if (type3004num==0||type3004num==1)
			{
				Edge *edge = edgevector[lineedge];
				tag_t initialtag = edge->Tag();
				std::vector<Edge*>edgevector1(1);
				edgevector1[0] = edge;
				NXOpen::EdgeDumbRule *edgeDumbRule1;
				edgeDumbRule1 = workPart->ScRuleFactory()->CreateRuleEdgeDumb(edgevector1);
				offsetCurveBuilder1->CurvesToOffset()->AllowSelfIntersection(true);
				std::vector<NXOpen::SelectionIntentRule *> rules1(1);
				rules1[0] = edgeDumbRule1;
				NXOpen::Point3d helpPoint1;
				NXOpen::Point3d helpPoint2;
				edge->GetVertices(&helpPoint1, &helpPoint2);
				offsetCurveBuilder1->CurvesToOffset()->AddToSection(rules1, edgevector1[0], nullNXOpen_NXObject, nullNXOpen_NXObject, helpPoint1, NXOpen::Section::ModeCreate, false);

				NXOpen::Point3d point1;
				NXOpen::Point3d point2;
				edge->GetVertices(&point1, &point2);
				tag_t temptag = edge->Tag();
				do
				{
					for (int i = 0; i < edgevector.size(); i++)
					{
						Edge *edge1 = edgevector[i];
						if (edge1->Tag() == temptag)
						{
							continue;
						}
						NXOpen::Point3d point3;
						NXOpen::Point3d point4;
						edge1->GetVertices(&point3, &point4);
						if (point1.X == point3.X&&point1.Y == point3.Y&&point1.Z == point3.Z)
						{
							point1 = point4;
							temptag = edge1->Tag();
							int edgetype;
							UF_MODL_ask_edge_type(temptag, &edgetype);

							if (temptag != initialtag)
							{
								std::vector<Edge*>edgevector1(1);
								edgevector1[0] = edge1;
								NXOpen::EdgeDumbRule *edgeDumbRule1;
								edgeDumbRule1 = workPart->ScRuleFactory()->CreateRuleEdgeDumb(edgevector1);
								offsetCurveBuilder1->CurvesToOffset()->AllowSelfIntersection(true);
								std::vector<NXOpen::SelectionIntentRule *> rules1(1);
								rules1[0] = edgeDumbRule1;
								NXOpen::Point3d helpPoint1;
								NXOpen::Point3d helpPoint2;
								edge1->GetVertices(&helpPoint1, &helpPoint2);
								offsetCurveBuilder1->CurvesToOffset()->AddToSection(rules1, edgevector1[0], nullNXOpen_NXObject, nullNXOpen_NXObject, helpPoint1, NXOpen::Section::ModeCreate, false);
							}
							break;
						}

						if (point1.X == point4.X&&point1.Y == point4.Y&&point1.Z == point4.Z)
						{

							point1 = point3;
							temptag = edge1->Tag();
							int edgetype;
							UF_MODL_ask_edge_type(temptag, &edgetype);
							if (temptag != initialtag)
							{

								std::vector<Edge*>edgevector1(1);
								edgevector1[0] = edge1;
								NXOpen::EdgeDumbRule *edgeDumbRule1;
								edgeDumbRule1 = workPart->ScRuleFactory()->CreateRuleEdgeDumb(edgevector1);
								offsetCurveBuilder1->CurvesToOffset()->AllowSelfIntersection(true);
								std::vector<NXOpen::SelectionIntentRule *> rules1(1);
								rules1[0] = edgeDumbRule1;
								NXOpen::Point3d helpPoint1;
								NXOpen::Point3d helpPoint2;
								edge1->GetVertices(&helpPoint1, &helpPoint2);
								offsetCurveBuilder1->CurvesToOffset()->AddToSection(rules1, edgevector1[0], nullNXOpen_NXObject, nullNXOpen_NXObject, helpPoint1, NXOpen::Section::ModeCreate, false);
							}

							break;
						}

					}
				} while (temptag != initialtag);

				if (type3003num!=0)
				{
					offsetCurveBuilder1->SetReverseDirection(1);
				}
			}
			else
			{
				for (int i = 0; i < 9;i++)
				{
					std::vector<Edge*>edgevector1(1);
					edgevector1[0] = edgevector[i];
					NXOpen::EdgeDumbRule *edgeDumbRule1;
					edgeDumbRule1 = workPart->ScRuleFactory()->CreateRuleEdgeDumb(edgevector1);
					offsetCurveBuilder1->CurvesToOffset()->AllowSelfIntersection(true);
					std::vector<NXOpen::SelectionIntentRule *> rules1(1);
					rules1[0] = edgeDumbRule1;
					NXOpen::Point3d helpPoint1;
					NXOpen::Point3d helpPoint2;
					edgevector[i]->GetVertices(&helpPoint1, &helpPoint2);
					offsetCurveBuilder1->CurvesToOffset()->AddToSection(rules1, edgevector1[0], nullNXOpen_NXObject, nullNXOpen_NXObject, helpPoint1, NXOpen::Section::ModeCreate, false);
				}

			}

		}

		else
		{
			if (type3004num==0)
			{
				for (int i = 0; i < 5;i++)
				{
					Edge *edge = edgevector[i];
					std::vector<Edge*>edgevector1(1);
					edgevector1[0] = edge;
					NXOpen::EdgeDumbRule *edgeDumbRule1;
					edgeDumbRule1 = workPart->ScRuleFactory()->CreateRuleEdgeDumb(edgevector1);
					offsetCurveBuilder1->CurvesToOffset()->AllowSelfIntersection(true);
					std::vector<NXOpen::SelectionIntentRule *> rules1(1);
					rules1[0] = edgeDumbRule1;
					NXOpen::Point3d helpPoint1;
					NXOpen::Point3d helpPoint2;
					edge->GetVertices(&helpPoint1, &helpPoint2);
					offsetCurveBuilder1->CurvesToOffset()->AddToSection(rules1, edgevector1[0], nullNXOpen_NXObject, nullNXOpen_NXObject, helpPoint1, NXOpen::Section::ModeCreate, false);
				}
				offsetCurveBuilder1->SetReverseDirection(1);
			}
			else if (type3004num == 2)
			{
				Edge *edge = edgevector[lineedge];
				tag_t initialtag = edge->Tag();
				std::vector<Edge*>edgevector1(1);
				edgevector1[0] = edge;
				NXOpen::EdgeDumbRule *edgeDumbRule1;
				edgeDumbRule1 = workPart->ScRuleFactory()->CreateRuleEdgeDumb(edgevector1);
				offsetCurveBuilder1->CurvesToOffset()->AllowSelfIntersection(true);
				std::vector<NXOpen::SelectionIntentRule *> rules1(1);
				rules1[0] = edgeDumbRule1;
				NXOpen::Point3d helpPoint1;
				NXOpen::Point3d helpPoint2;
				edge->GetVertices(&helpPoint1, &helpPoint2);
				offsetCurveBuilder1->CurvesToOffset()->AddToSection(rules1, edgevector1[0], nullNXOpen_NXObject, nullNXOpen_NXObject, helpPoint1, NXOpen::Section::ModeCreate, false);

				NXOpen::Point3d point1;
				NXOpen::Point3d point2;
				edge->GetVertices(&point1, &point2);
				tag_t temptag = edge->Tag();

				for (int a = 0; a < 4; a++)
				{
					for (int i = 0; i < edgevector.size(); i++)
					{
						Edge *edge1 = edgevector[i];
						if (edge1->Tag() == temptag)
						{
							continue;
						}
						NXOpen::Point3d point3;
						NXOpen::Point3d point4;
						edge1->GetVertices(&point3, &point4);
						if (point1.X == point3.X&&point1.Y == point3.Y&&point1.Z == point3.Z)
						{
							point1 = point4;
							temptag = edge1->Tag();
							int edgetype;
							UF_MODL_ask_edge_type(temptag, &edgetype);

							if (temptag != initialtag)
							{
								std::vector<Edge*>edgevector1(1);
								edgevector1[0] = edge1;
								NXOpen::EdgeDumbRule *edgeDumbRule1;
								edgeDumbRule1 = workPart->ScRuleFactory()->CreateRuleEdgeDumb(edgevector1);
								offsetCurveBuilder1->CurvesToOffset()->AllowSelfIntersection(true);
								std::vector<NXOpen::SelectionIntentRule *> rules1(1);
								rules1[0] = edgeDumbRule1;
								NXOpen::Point3d helpPoint1;
								NXOpen::Point3d helpPoint2;
								edge1->GetVertices(&helpPoint1, &helpPoint2);
								offsetCurveBuilder1->CurvesToOffset()->AddToSection(rules1, edgevector1[0], nullNXOpen_NXObject, nullNXOpen_NXObject, helpPoint1, NXOpen::Section::ModeCreate, false);
							}



							break;
						}

						if (point1.X == point4.X&&point1.Y == point4.Y&&point1.Z == point4.Z)
						{

							point1 = point3;
							temptag = edge1->Tag();
							int edgetype;
							UF_MODL_ask_edge_type(temptag, &edgetype);
							if (temptag != initialtag)
							{

								std::vector<Edge*>edgevector1(1);
								edgevector1[0] = edge1;
								NXOpen::EdgeDumbRule *edgeDumbRule1;
								edgeDumbRule1 = workPart->ScRuleFactory()->CreateRuleEdgeDumb(edgevector1);
								offsetCurveBuilder1->CurvesToOffset()->AllowSelfIntersection(true);
								std::vector<NXOpen::SelectionIntentRule *> rules1(1);
								rules1[0] = edgeDumbRule1;
								NXOpen::Point3d helpPoint1;
								NXOpen::Point3d helpPoint2;
								edge1->GetVertices(&helpPoint1, &helpPoint2);
								offsetCurveBuilder1->CurvesToOffset()->AddToSection(rules1, edgevector1[0], nullNXOpen_NXObject, nullNXOpen_NXObject, helpPoint1, NXOpen::Section::ModeCreate, false);
							}

							break;
						}

					}
				}

				std::vector<Edge*>edgevector2(1);
				edgevector2[0] = edgevector[5];
				NXOpen::EdgeDumbRule *edgeDumbRule2;
				edgeDumbRule2 = workPart->ScRuleFactory()->CreateRuleEdgeDumb(edgevector2);
				offsetCurveBuilder1->CurvesToOffset()->AllowSelfIntersection(true);
				std::vector<NXOpen::SelectionIntentRule *> rules2(1);
				rules2[0] = edgeDumbRule2;
				NXOpen::Point3d helpPoint3;
				NXOpen::Point3d helpPoint4;
				edgevector[5]->GetVertices(&helpPoint3, &helpPoint4);
				offsetCurveBuilder1->CurvesToOffset()->AddToSection(rules2, edgevector2[0], nullNXOpen_NXObject, nullNXOpen_NXObject, helpPoint3, NXOpen::Section::ModeCreate, false);

				std::vector<Edge*>edgevector3(1);
				edgevector3[0] = edgevector[7];
				NXOpen::EdgeDumbRule *edgeDumbRule3;
				edgeDumbRule3 = workPart->ScRuleFactory()->CreateRuleEdgeDumb(edgevector3);
				offsetCurveBuilder1->CurvesToOffset()->AllowSelfIntersection(true);
				std::vector<NXOpen::SelectionIntentRule *> rules3(1);
				rules3[0] = edgeDumbRule3;
				NXOpen::Point3d helpPoint5;
				NXOpen::Point3d helpPoint6;
				edgevector[7]->GetVertices(&helpPoint5, &helpPoint6);
				offsetCurveBuilder1->CurvesToOffset()->AddToSection(rules3, edgevector3[0], nullNXOpen_NXObject, nullNXOpen_NXObject, helpPoint5, NXOpen::Section::ModeCreate, false);
			}

			else
			{
				Edge *edge0 = edgevector[0];
				int edge0type;
				UF_MODL_ask_edge_type(edge0->Tag(), &edge0type);
				if (edge0type == UF_MODL_LINEAR_EDGE)
				{
					Edge *edge7 = edgevector[circleedge + 6];
					int edge7type;
					UF_MODL_ask_edge_type(edge7->Tag(), &edge7type);
					if (edge7type != UF_MODL_CIRCULAR_EDGE)
					{
						for (int k = 0; k < 12; k++)
						{
							std::vector<Edge*>edgevector2(1);
							edgevector2[0] = edgevector[k];
							NXOpen::EdgeDumbRule *edgeDumbRule1;
							edgeDumbRule1 = workPart->ScRuleFactory()->CreateRuleEdgeDumb(edgevector2);
							offsetCurveBuilder1->CurvesToOffset()->AllowSelfIntersection(true);
							std::vector<NXOpen::SelectionIntentRule *> rules1(1);
							rules1[0] = edgeDumbRule1;
							NXOpen::Point3d helpPoint1;
							NXOpen::Point3d helpPoint2;
							edgevector[k]->GetVertices(&helpPoint1, &helpPoint2);
							offsetCurveBuilder1->CurvesToOffset()->AddToSection(rules1, edgevector2[0], nullNXOpen_NXObject, nullNXOpen_NXObject, helpPoint1, NXOpen::Section::ModeCreate, false);
							circleedge = circleedge + 1;
						}

						for (int j = 26; j > 18; j--)
						{
							std::vector<Edge*>edgevector2(1);
							edgevector2[0] = edgevector[j];
							NXOpen::EdgeDumbRule *edgeDumbRule1;
							edgeDumbRule1 = workPart->ScRuleFactory()->CreateRuleEdgeDumb(edgevector2);
							offsetCurveBuilder1->CurvesToOffset()->AllowSelfIntersection(true);
							std::vector<NXOpen::SelectionIntentRule *> rules1(1);
							rules1[0] = edgeDumbRule1;
							NXOpen::Point3d helpPoint1;
							NXOpen::Point3d helpPoint2;
							edgevector[j]->GetVertices(&helpPoint1, &helpPoint2);
							offsetCurveBuilder1->CurvesToOffset()->AddToSection(rules1, edgevector2[0], nullNXOpen_NXObject, nullNXOpen_NXObject, helpPoint1, NXOpen::Section::ModeCreate, false);
						}
						Edge *edge17 = edgevector[17];
						NXOpen::Point3d startpoint1;
						NXOpen::Point3d endPoint1;
						edge17->GetVertices(&startpoint1,&endPoint1);
						if (startpoint1.X==endPoint1.X&&startpoint1.Y==endPoint1.Y&&startpoint1.Z==endPoint1.Z)
						{
							std::vector<Edge*>edgevector2(1);
							edgevector2[0] = edgevector[18];
							NXOpen::EdgeDumbRule *edgeDumbRule1;
							edgeDumbRule1 = workPart->ScRuleFactory()->CreateRuleEdgeDumb(edgevector2);
							offsetCurveBuilder1->CurvesToOffset()->AllowSelfIntersection(true);
							std::vector<NXOpen::SelectionIntentRule *> rules1(1);
							rules1[0] = edgeDumbRule1;
							NXOpen::Point3d helpPoint1;
							NXOpen::Point3d helpPoint2;
							edgevector[18]->GetVertices(&helpPoint1, &helpPoint2);
							offsetCurveBuilder1->CurvesToOffset()->AddToSection(rules1, edgevector2[0], nullNXOpen_NXObject, nullNXOpen_NXObject, helpPoint1, NXOpen::Section::ModeCreate, false);
						}
						else
						{
							std::vector<Edge*>edgevector2(1);
							edgevector2[0] = edgevector[17];
							NXOpen::EdgeDumbRule *edgeDumbRule1;
							edgeDumbRule1 = workPart->ScRuleFactory()->CreateRuleEdgeDumb(edgevector2);
							offsetCurveBuilder1->CurvesToOffset()->AllowSelfIntersection(true);
							std::vector<NXOpen::SelectionIntentRule *> rules1(1);
							rules1[0] = edgeDumbRule1;
							NXOpen::Point3d helpPoint1;
							NXOpen::Point3d helpPoint2;
							edgevector[17]->GetVertices(&helpPoint1, &helpPoint2);
							offsetCurveBuilder1->CurvesToOffset()->AddToSection(rules1, edgevector2[0], nullNXOpen_NXObject, nullNXOpen_NXObject, helpPoint1, NXOpen::Section::ModeCreate, false);
						}
						
					}

					else
					{
						for (int i = 26; i > 18; i--)
						{
							std::vector<Edge*>edgevector2(1);
							edgevector2[0] = edgevector[i];
							NXOpen::EdgeDumbRule *edgeDumbRule1;
							edgeDumbRule1 = workPart->ScRuleFactory()->CreateRuleEdgeDumb(edgevector2);
							offsetCurveBuilder1->CurvesToOffset()->AllowSelfIntersection(true);
							std::vector<NXOpen::SelectionIntentRule *> rules1(1);
							rules1[0] = edgeDumbRule1;
							NXOpen::Point3d helpPoint1;
							NXOpen::Point3d helpPoint2;
							edgevector[i]->GetVertices(&helpPoint1, &helpPoint2);
							offsetCurveBuilder1->CurvesToOffset()->AddToSection(rules1, edgevector2[0], nullNXOpen_NXObject, nullNXOpen_NXObject, helpPoint1, NXOpen::Section::ModeCreate, false);
						}
						for (int k = 0; k < 13; k++)
						{
							std::vector<Edge*>edgevector2(1);
							edgevector2[0] = edgevector[k];
							NXOpen::EdgeDumbRule *edgeDumbRule1;
							edgeDumbRule1 = workPart->ScRuleFactory()->CreateRuleEdgeDumb(edgevector2);
							offsetCurveBuilder1->CurvesToOffset()->AllowSelfIntersection(true);
							std::vector<NXOpen::SelectionIntentRule *> rules1(1);
							rules1[0] = edgeDumbRule1;
							NXOpen::Point3d helpPoint1;
							NXOpen::Point3d helpPoint2;
							edgevector[k]->GetVertices(&helpPoint1, &helpPoint2);
							offsetCurveBuilder1->CurvesToOffset()->AddToSection(rules1, edgevector2[0], nullNXOpen_NXObject, nullNXOpen_NXObject, helpPoint1, NXOpen::Section::ModeCreate, false);
							circleedge = circleedge + 1;
						}

					}
				}
				else
				{
					for (int l = 0; l < 8; l++)
					{
						std::vector<Edge*>edgevector2(1);
						edgevector2[0] = edgevector[l];
						NXOpen::EdgeDumbRule *edgeDumbRule1;
						edgeDumbRule1 = workPart->ScRuleFactory()->CreateRuleEdgeDumb(edgevector2);
						offsetCurveBuilder1->CurvesToOffset()->AllowSelfIntersection(true);
						std::vector<NXOpen::SelectionIntentRule *> rules1(1);
						rules1[0] = edgeDumbRule1;
						NXOpen::Point3d helpPoint1;
						NXOpen::Point3d helpPoint2;
						edgevector[l]->GetVertices(&helpPoint1, &helpPoint2);
						offsetCurveBuilder1->CurvesToOffset()->AddToSection(rules1, edgevector2[0], nullNXOpen_NXObject, nullNXOpen_NXObject, helpPoint1, NXOpen::Section::ModeCreate, false);
					}
					for (int ll = 24; ll >18; ll--)
					{
						std::vector<Edge*>edgevector2(1);
						edgevector2[0] = edgevector[ll];
						NXOpen::EdgeDumbRule *edgeDumbRule1;
						edgeDumbRule1 = workPart->ScRuleFactory()->CreateRuleEdgeDumb(edgevector2);
						offsetCurveBuilder1->CurvesToOffset()->AllowSelfIntersection(true);
						std::vector<NXOpen::SelectionIntentRule *> rules1(1);
						rules1[0] = edgeDumbRule1;
						NXOpen::Point3d helpPoint1;
						NXOpen::Point3d helpPoint2;
						edgevector[ll]->GetVertices(&helpPoint1, &helpPoint2);
						offsetCurveBuilder1->CurvesToOffset()->AddToSection(rules1, edgevector2[0], nullNXOpen_NXObject, nullNXOpen_NXObject, helpPoint1, NXOpen::Section::ModeCreate, false);
					}
					for (int lll = 9; lll < 12;lll++)
					{
						std::vector<Edge*>edgevector2(1);
						edgevector2[0] = edgevector[lll];
						NXOpen::EdgeDumbRule *edgeDumbRule1;
						edgeDumbRule1 = workPart->ScRuleFactory()->CreateRuleEdgeDumb(edgevector2);
						offsetCurveBuilder1->CurvesToOffset()->AllowSelfIntersection(true);
						std::vector<NXOpen::SelectionIntentRule *> rules1(1);
						rules1[0] = edgeDumbRule1;
						NXOpen::Point3d helpPoint1;
						NXOpen::Point3d helpPoint2;
						edgevector[lll]->GetVertices(&helpPoint1, &helpPoint2);
						offsetCurveBuilder1->CurvesToOffset()->AddToSection(rules1, edgevector2[0], nullNXOpen_NXObject, nullNXOpen_NXObject, helpPoint1, NXOpen::Section::ModeCreate, false);
					}
					std::vector<Edge*>edgevector2(1);
					edgevector2[0] = edgevector[26];
					NXOpen::EdgeDumbRule *edgeDumbRule1;
					edgeDumbRule1 = workPart->ScRuleFactory()->CreateRuleEdgeDumb(edgevector2);
					offsetCurveBuilder1->CurvesToOffset()->AllowSelfIntersection(true);
					std::vector<NXOpen::SelectionIntentRule *> rules1(1);
					rules1[0] = edgeDumbRule1;
					NXOpen::Point3d helpPoint1;
					NXOpen::Point3d helpPoint2;
					edgevector[26]->GetVertices(&helpPoint1, &helpPoint2);
					offsetCurveBuilder1->CurvesToOffset()->AddToSection(rules1, edgevector2[0], nullNXOpen_NXObject, nullNXOpen_NXObject, helpPoint1, NXOpen::Section::ModeCreate, false);
					
					
					
				}
				
			}

		}	


		offsetCurveBuilder1->InputCurvesOptions()->SetAssociative(false);
		offsetCurveBuilder1->InputCurvesOptions()->SetInputCurveOption(NXOpen::GeometricUtilities::CurveOptions::InputCurveBlank);

		//无延伸
		/*offsetCurveBuilder1->SetTrimMethod(NXOpen::Features::OffsetCurveBuilder::TrimOptionNone);*/


		NXOpen::NXObject *nXObject1;
		nXObject1 = offsetCurveBuilder1->Commit();

		//获取偏置后的曲线的tag
		////获取偏置后的曲线
		std::vector<NXOpen::NXObject *>NXoffsetObject = offsetCurveBuilder1->GetCommittedObjects();

		for (int i = 0; i < NXoffsetObject.size(); i++)
		{
			Section *section1 = workPart->Sections()->CreateSection(NXoffsetObject[i]);
			CAM::DriveChainItemBuilder *driveChainItemBuilder1;

			driveChainItemBuilder1 = surfaceContourBuilder1->DmcurveBuilder()->CreateDpmcvDriveCurve(section1);

			surfaceContourBuilder1->DmcurveBuilder()->DpmcvDriveCurves()->Append(driveChainItemBuilder1);

		}

		offsetCurveBuilder1->Destroy();

		surfaceContourBuilder1->CutParameters()->PartStock()->SetValue(BossParameterSet.PartAllowance);

		CAM::Inheritable2dLength *inheritable2dLength1;
		inheritable2dLength1 = surfaceContourBuilder1->CutParameters()->Tolerances();

		inheritable2dLength1->SetValue0(BossParameterSet.Tolerance);
		inheritable2dLength1->SetValue1(BossParameterSet.Tolerance);


		//最大步长
		if (BossParameterSet.CuttingStepdistanceunit == 0)
		{
			surfaceContourBuilder1->CutParameters()->MaxCutStep()->SetIntent(NXOpen::CAM::ParamValueIntentToolDep);
			surfaceContourBuilder1->CutParameters()->MaxCutStep()->SetValue(BossParameterSet.CuttingStepdistance);
		}
		else if (BossParameterSet.CuttingStepdistanceunit == 1)
		{
			surfaceContourBuilder1->CutParameters()->MaxCutStep()->SetIntent(NXOpen::CAM::ParamValueIntentPartUnits);
			surfaceContourBuilder1->CutParameters()->MaxCutStep()->SetValue(BossParameterSet.CuttingStepdistance);
		}

		surfaceContourBuilder1->FeedsBuilder()->SpindleRpmBuilder()->SetValue(BossParameterSet.SpindleSpeed);
		surfaceContourBuilder1->FeedsBuilder()->FeedCutBuilder()->SetValue(BossParameterSet.Machining);
		surfaceContourBuilder1->FeedsBuilder()->FeedEngageBuilder()->SetValue(BossParameterSet.Approach);
		surfaceContourBuilder1->FeedsBuilder()->FeedRetractBuilder()->SetValue(BossParameterSet.Retract);

		surfaceContourBuilder1->NonCuttingBuilder()->RetractOpenAreaBuilder()->SetEngRetType(CAM::NcmScEngRetBuilder::EngRetTypesPlungeLift);
		surfaceContourBuilder1->NonCuttingBuilder()->TransferWithinRegionsBuilder()->SetApproachMethod(CAM::NcmTransferBuilder::AppDepMethodTypesAlongToolAxis);
		surfaceContourBuilder1->NonCuttingBuilder()->TransferWithinRegionsBuilder()->SetDepartureMethod(CAM::NcmTransferBuilder::AppDepMethodTypesAlongToolAxis);

		if (BossParameterSet.ToolMotion.ToolMotionType == 0)
		{
			surfaceContourBuilder1->NonCuttingBuilder()->EngageOpenAreaBuilder()->SetEngRetType(CAM::NcmScEngRetBuilder::EngRetTypesArcParallelToToolAxis);
			surfaceContourBuilder1->NonCuttingBuilder()->EngageOpenAreaBuilder()->RadiusBuilder()->SetValue(BossParameterSet.ToolMotion.CircleRadius);
			switch (BossParameterSet.ToolMotion.CircleRadiusUnit)
			{
			case 0:
				surfaceContourBuilder1->NonCuttingBuilder()->EngageOpenAreaBuilder()->RadiusBuilder()->SetIntent(CAM::ParamValueIntentToolDep);
				break;
			case 1:
				surfaceContourBuilder1->NonCuttingBuilder()->EngageOpenAreaBuilder()->RadiusBuilder()->SetIntent(CAM::ParamValueIntentPartUnits);
				break;
			}
			surfaceContourBuilder1->NonCuttingBuilder()->EngageOpenAreaBuilder()->LinearExtensionBuilder()->SetValue(BossParameterSet.ToolMotion.LinearExtension);
			switch (BossParameterSet.ToolMotion.LinearExtensionUnit)
			{
			case 0:
				surfaceContourBuilder1->NonCuttingBuilder()->EngageOpenAreaBuilder()->LinearExtensionBuilder()->SetIntent(CAM::ParamValueIntentToolDep);
				break;
			case 1:
				surfaceContourBuilder1->NonCuttingBuilder()->EngageOpenAreaBuilder()->LinearExtensionBuilder()->SetIntent(CAM::ParamValueIntentPartUnits);
				break;
			}
			surfaceContourBuilder1->NonCuttingBuilder()->EngageOpenAreaBuilder()->SetArcAngle(BossParameterSet.ToolMotion.CircleAngle);
		}
		else if (BossParameterSet.ToolMotion.ToolMotionType == 1)
		{
			surfaceContourBuilder1->NonCuttingBuilder()->EngageOpenAreaBuilder()->SetEngRetType(CAM::NcmScEngRetBuilder::EngRetTypesArcNormalToToolAxis);
			surfaceContourBuilder1->NonCuttingBuilder()->EngageOpenAreaBuilder()->LengthBuilder()->SetValue(BossParameterSet.ToolMotion.ToolMotionDistance);
			switch (BossParameterSet.ToolMotion.ToolMotionDistanceUnit)
			{
			case 0:
				surfaceContourBuilder1->NonCuttingBuilder()->EngageOpenAreaBuilder()->LengthBuilder()->SetIntent(CAM::ParamValueIntentToolDep);
				break;
			case 1:
				surfaceContourBuilder1->NonCuttingBuilder()->EngageOpenAreaBuilder()->LengthBuilder()->SetIntent(CAM::ParamValueIntentPartUnits);
				break;
			}
			/*surfaceContourBuilder1->NonCuttingBuilder()->EngageOpenAreaBuilder()->LinearExtensionBuilder()->SetValue(BossParameterSet.ToolMotion.LinearExtension);
			switch (BossParameterSet.ToolMotion.LinearExtensionUnit)
			{
			case 0:
			surfaceContourBuilder1->NonCuttingBuilder()->EngageOpenAreaBuilder()->LinearExtensionBuilder()->SetIntent(CAM::ParamValueIntentToolDep);
			break;
			case 1:
			surfaceContourBuilder1->NonCuttingBuilder()->EngageOpenAreaBuilder()->LinearExtensionBuilder()->SetIntent(CAM::ParamValueIntentPartUnits);
			break;
			}*/
			surfaceContourBuilder1->NonCuttingBuilder()->EngageOpenAreaBuilder()->SetSwingAngle(BossParameterSet.ToolMotion.RotationAngle);
			surfaceContourBuilder1->NonCuttingBuilder()->EngageOpenAreaBuilder()->SetRampAngle(BossParameterSet.ToolMotion.TiltAngle);
		}
		else if (BossParameterSet.ToolMotion.ToolMotionType == 2)
		{
			surfaceContourBuilder1->NonCuttingBuilder()->EngageOpenAreaBuilder()->SetEngRetType(CAM::NcmScEngRetBuilder::EngRetTypesArcNormalToPart);
			surfaceContourBuilder1->NonCuttingBuilder()->EngageOpenAreaBuilder()->LengthBuilder()->SetValue(BossParameterSet.ToolMotion.ToolMotionDistance);
			switch (BossParameterSet.ToolMotion.ToolMotionDistanceUnit)
			{
			case 0:
				surfaceContourBuilder1->NonCuttingBuilder()->EngageOpenAreaBuilder()->LengthBuilder()->SetIntent(CAM::ParamValueIntentToolDep);
				break;
			case 1:
				surfaceContourBuilder1->NonCuttingBuilder()->EngageOpenAreaBuilder()->LengthBuilder()->SetIntent(CAM::ParamValueIntentPartUnits);
				break;
			}
			//surfaceContourBuilder1->NonCuttingBuilder()->EngageOpenAreaBuilder()->LinearExtensionBuilder()->SetValue(BossParameterSet.ToolMotion.LinearExtension);
			//switch (BossParameterSet.ToolMotion.LinearExtensionUnit)
			//{
			//case 0:
			//	surfaceContourBuilder1->NonCuttingBuilder()->EngageOpenAreaBuilder()->LinearExtensionBuilder()->SetIntent(CAM::ParamValueIntentToolDep);
			//	break;
			//case 1:
			//	surfaceContourBuilder1->NonCuttingBuilder()->EngageOpenAreaBuilder()->LinearExtensionBuilder()->SetIntent(CAM::ParamValueIntentPartUnits);    //HeightBuilder
			//	break;
			//}
			surfaceContourBuilder1->NonCuttingBuilder()->EngageOpenAreaBuilder()->SetSwingAngle(BossParameterSet.ToolMotion.RotationAngle);
			surfaceContourBuilder1->NonCuttingBuilder()->EngageOpenAreaBuilder()->SetRampAngle(BossParameterSet.ToolMotion.TiltAngle);
		}
		else if (BossParameterSet.ToolMotion.ToolMotionType == 3)
		{
			surfaceContourBuilder1->NonCuttingBuilder()->EngageOpenAreaBuilder()->SetEngRetType(CAM::NcmScEngRetBuilder::EngRetTypesLinearNormalToPart);

			surfaceContourBuilder1->NonCuttingBuilder()->EngageOpenAreaBuilder()->LengthBuilder()->SetValue(BossParameterSet.ToolMotion.ToolMotionDistance);
			switch (BossParameterSet.ToolMotion.ToolMotionDistanceUnit)
			{
			case 0:
				surfaceContourBuilder1->NonCuttingBuilder()->EngageOpenAreaBuilder()->LengthBuilder()->SetIntent(CAM::ParamValueIntentToolDep);
				break;
			case 1:
				surfaceContourBuilder1->NonCuttingBuilder()->EngageOpenAreaBuilder()->LengthBuilder()->SetIntent(CAM::ParamValueIntentPartUnits);
				break;
			}
			surfaceContourBuilder1->NonCuttingBuilder()->EngageOpenAreaBuilder()->SetSwingAngle(BossParameterSet.ToolMotion.RotationAngle);

			surfaceContourBuilder1->NonCuttingBuilder()->EngageOpenAreaBuilder()->SetRampAngle(BossParameterSet.ToolMotion.TiltAngle);
		}
		if (BossParameterSet.SafeSetType == 0)
		{
			surfaceContourBuilder1->NonCuttingBuilder()->TransferCommonClearanceBuilder()->SetClearanceType(CAM::NcmClearanceBuilder::ClearanceTypesCylinder);
			surfaceContourBuilder1->NonCuttingBuilder()->TransferCommonClearanceBuilder()->SetRadius(380.0 + BossParameterSet.SafeSetOffset);
			NXOpen::Point3d iCenterPoint(0.0, 0.0, 0.0);
			NXOpen::Point *point3;
			point3 = workPart->Points()->CreatePoint(iCenterPoint);

			NXOpen::NXObject * TPointObject = (NXOpen::NXObject *) point3;
			surfaceContourBuilder1->NonCuttingBuilder()->TransferCommonClearanceBuilder()->SetPoint(TPointObject);
			Direction *direction1 = workPart->Directions()->CreateDirection(point3, _Normal);
			surfaceContourBuilder1->NonCuttingBuilder()->TransferCommonClearanceBuilder()->SetAxis(direction1);
		}
		else if (BossParameterSet.SafeSetType == 1)
		{
			surfaceContourBuilder1->NonCuttingBuilder()->TransferCommonClearanceBuilder()->SetClearanceType(CAM::NcmClearanceBuilder::ClearanceTypesAutomatic);
		}
		else if (BossParameterSet.SafeSetType == 2)
		{
			surfaceContourBuilder1->NonCuttingBuilder()->TransferCommonClearanceBuilder()->SetClearanceType(CAM::NcmClearanceBuilder::ClearanceTypesUseCommon);
		}
		surfaceContourBuilder1->NonCuttingBuilder()->TransferCommonClearanceBuilder()->SetSafeDistance(BossParameterSet.SafeSetOffset);

		surfaceContourBuilder1->CutParameters()->BlankOverhang()->SetIntent(CAM::ParamValueIntentToolDep);
		surfaceContourBuilder1->CutParameters()->BlankOverhang()->SetValue(20.0);


		NXObject *nXObject;
		nXObject = surfaceContourBuilder1->Commit();
		surfaceContourBuilder1->Destroy();

		std::vector<CAM::CAMObject *> objects1(1);
		objects1[0] = surfaceContour1;
		workPart->CAMSetup()->GenerateToolPath(objects1);

		UF_UI_ONT_refresh();

		UF_CAM_update_single_object_customization(surfaceContourBuilder1->Tag());

	}
}



