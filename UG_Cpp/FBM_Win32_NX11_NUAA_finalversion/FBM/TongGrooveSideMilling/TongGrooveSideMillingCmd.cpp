#include "TongGrooveSideMillingCmd.h"
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

static void set_camgeom_app_data(tag_t item, int geom_type, int num_app_data, UF_CAMGEOM_app_data_p_t *app_data);

static void CAM_init_camgeom_app_data(UF_CAMGEOM_app_data_p_t app_data)
{
	if (app_data)
	{
		app_data->has_stock = 0;
		app_data->has_cut_stock = 0;
		app_data->has_tolerances = 0;
		app_data->has_feedrate = 0;
		app_data->has_offset = 0;
		app_data->has_avoidance_type = 0;

		app_data->stock = 0.0;
		app_data->cut_stock[0] = 0;
		app_data->cut_stock[1] = 0;
		app_data->cut_stock[2] = 0;
		app_data->tolerances[0] = 0;
		app_data->tolerances[1] = 0;
		app_data->feedrate_unit = UF_CAM_feedrate_unit_per_minute;
		app_data->feedrate_value = 1;
		app_data->offset = 0;
		app_data->avoidance_type = UF_CAM_avoidance_type_warning;
	}
}

PassGrooveSideMillingCmd::PassGrooveSideMillingCmd() :
TaggedObject()
{
}

PassGrooveSideMillingCmd::PassGrooveSideMillingCmd(std::vector<PassGroove*> ListOfPassGroove, PassGrooveSideProcessParameter ParameterSet, Part* WorkPart, Body* WorkBody, Body* WorkBlank, std::vector<Face*>ListOfSelectedFace) :
TaggedObject()
{
	PassGrooveResultList = ListOfPassGroove;       
	PassGrooveParameterSet = ParameterSet;   
	workPart = WorkPart;             
	selectedBody = WorkBody; 
	selectedBlank = WorkBlank;
	SelectedPassGrooveTopFaceList = ListOfSelectedFace;
	_Normal = PassGrooveParameterSet.Approch_Normal;

	UF_initialize();

	if (PassGrooveParameterSet.FaceSelect&&PassGrooveParameterSet.syspart==1)
	{
		SelectedPassGroove.clear();
		Index.clear();
		for (std::vector<Face*>::size_type i = 0; i != SelectedPassGrooveTopFaceList.size(); i++)
		{
			Face* EachFace = SelectedPassGrooveTopFaceList[i];
			tag_t EachFaceTag = EachFace->Tag();
			for (std::vector<PassGroove*>::size_type j = 0; j != PassGrooveResultList.size(); j++)
			{
				PassGroove* EachPassGroove = PassGrooveResultList[j];
				Face* EachPassGrooveTopFace = EachPassGroove->TopFaceVector[0];
				if (EachFaceTag == EachPassGrooveTopFace->Tag() && strstr(EachPassGroove->name, "通槽") != NULL)
				{
					SelectedPassGroove.push_back(EachPassGroove);
					Index.push_back(j);
				}
			}
		}

		PassGrooveSideMillingDecision1(SelectedPassGroove);  
	}
	else if (PassGrooveParameterSet.AllGroove&&PassGrooveParameterSet.syspart == 1)
	{
		Index.clear();
		for (std::vector<PassGroove*>::size_type i = 0; i != PassGrooveResultList.size(); i++)
		{
			Index.push_back(i);
		}
		PassGrooveSideMillingDecision1(PassGrooveResultList);
	}
	else
	{
		if (PassGrooveParameterSet.syspart==0)
		{
			uc1601("请设置坐标系和几何体",1);
		}
		if (PassGrooveParameterSet.AllGroove==0&&PassGrooveParameterSet.FaceSelect==0)
		{
			uc1601("请选择加工区域",1);
		}
	}

	UF_terminate();
}

//-----------------------------------------------------------------------------
// PassGrooveSideMillingCmd : destructor
//-----------------------------------------------------------------------------

PassGrooveSideMillingCmd::~PassGrooveSideMillingCmd()
{
}

Point3d PassGrooveSideMillingCmd::GetCenterPoint(Face *face)
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

tag_t PassGrooveSideMillingCmd::SelectPart()
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

tag_t PassGrooveSideMillingCmd::SelectBlank()
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

Vector3d PassGrooveSideMillingCmd::get_Centre_fr_Face(Face* ipFace)
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

double PassGrooveSideMillingCmd::DotProduct(double a[3], double b[3])
{
	double result;
	result = a[0] * b[0] + a[1] * b[1] + a[2] * b[2];
	return result;

}

void PassGrooveSideMillingCmd::PassGrooveSideMillingDecision1(std::vector<PassGroove*> PassGrooveList)
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

	for (std::vector<PassGroove*>::size_type i = 0; i != PassGrooveList.size(); i++)
	{
		PassGroove* EachPassGroove = PassGrooveList[i];
		NXOpen::Face* PassGrooveTopFace = EachPassGroove->TopFaceVector[0];
		tag_t TopFaceTag = PassGrooveTopFace->Tag();
		int EachIndex = Index[i];
		char *Eachname;

		for (std::vector<PassGroove*>::size_type j = 0; j != PassGrooveResultList.size(); j++)
		{
			PassGroove* EachPassGroove = PassGrooveResultList[j];
			Face* PassGrooveTopFace = EachPassGroove->TopFaceVector[0];
			if (TopFaceTag == PassGrooveTopFace->Tag() && (strstr(EachPassGroove->name, "通槽") != NULL))
			{
				Eachname = EachPassGroove->name;
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
		Vector3d iCenterNormal = get_Centre_fr_Face(PassGrooveTopFace);
		Point3d iCenterPoint = GetCenterPoint(PassGrooveTopFace);

		Point *midPoint;
		midPoint = workPart->Points()->CreatePoint(iCenterPoint);

		Direction * direction = workPart->Directions()->CreateDirection(PassGrooveTopFace, midPoint, SenseForward, SmartObject::UpdateOptionAfterModeling);
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


		Vector3d centernormal = get_Centre_fr_Face(PassGrooveTopFace);
		double Facenormal[3] = { centernormal.X, centernormal.Y, centernormal.Z };

		//2.2 获得NX自动捕获的顶面上的坐标原点、矩阵坐标
		//坐标原点
		Point3d centeripoint = GetCenterPoint(PassGrooveTopFace);
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
		if (PassGrooveParameterSet.LCsys)
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
				char*Eachname1 = "已存在同名坐标系:";
				int len = strlen(Eachname1);
				char* tempEachName = new char[len + 1];
				strcpy(tempEachName, Eachname1);

				int lenSuffix = strlen(MCSName);
				int lenB = strlen(tempEachName);
				char* newString = new char[lenB + lenSuffix + 1];
				strcpy(newString, tempEachName);
				char* WorkpieceName;
				WorkpieceName = strcat(newString, MCSName);

				uc1601(WorkpieceName, 1);
			}

			//在坐标系下创建新的几何体
			tag_t  newWorkpiece = NULL_TAG;
			UF_NCGEOM_create("mill_contour", "WORKPIECE", &newWorkpiece);
			UF_UI_ONT_refresh();

			char*Eachname1 = "PassGroove.";
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

		}
		else
		{

			WorkpieceName = "WORKPIECE";

		}
		

		CAM::NCGroup *nCGroup1(dynamic_cast<CAM::NCGroup *>(workPart->CAMSetup()->CAMGroupCollection()->FindObject(PassGrooveParameterSet.PROGRAM)));
		CAM::Method *method1(dynamic_cast<CAM::Method *>(workPart->CAMSetup()->CAMGroupCollection()->FindObject(PassGrooveParameterSet.METHOD)));

		CAM::Tool *tool1(dynamic_cast<CAM::Tool *>(workPart->CAMSetup()->CAMGroupCollection()->FindObject(PassGrooveParameterSet.ToolMsg.ToolName)));
		CAM::FeatureGeometry *featureGeometry1(dynamic_cast<CAM::FeatureGeometry *>(workPart->CAMSetup()->CAMGroupCollection()->FindObject(WorkpieceName)));
		CAM::Operation *operation1;

		NXString nxString(tagStr.c_str());
		NXString OperationName = "PassGrooveSideMilling.Cu" + nxString;
		try
		{
			operation1 = workPart->CAMSetup()->CAMOperationCollection()->Create(nCGroup1, method1, tool1, featureGeometry1, "mill_planar", "PLANAR_MILL", CAM::OperationCollection::UseDefaultNameFalse, OperationName);
		}
		catch (...)
		{
			operation1 = workPart->CAMSetup()->CAMOperationCollection()->Create(nCGroup1, method1, tool1, featureGeometry1, "mill_planar", "PLANAR_MILL", CAM::OperationCollection::UseDefaultNameTrue, OperationName);
		}

		NXOpen::CAM::PlanarMilling *planarMilling1(dynamic_cast<NXOpen::CAM::PlanarMilling *>(operation1));
		NXOpen::CAM::PlanarMillingBuilder *planarMillingBuilder1;
		planarMillingBuilder1 = workPart->CAMSetup()->CAMOperationCollection()->CreatePlanarMillingBuilder(planarMilling1);



		//获取边界
		std::vector<Face*> GrooveSideFace = EachPassGroove->SideFaceVector;
		std::vector<Face*> GrooveRightFace = EachPassGroove->RightFaceVector;
		std::vector<NXOpen::Edge*>edgeofTopface = PassGrooveTopFace->GetEdges();
		std::vector<NXOpen::Edge*>bianjieedgevector;
		bianjieedgevector.clear();
		if (GrooveRightFace.size()==0)
		{
			for (int i = 0; i < GrooveSideFace.size();i++)
			{
				std::vector<NXOpen::Edge*>edgeofSideface = GrooveSideFace[i]->GetEdges();
				for (int j = 0; j < edgeofSideface.size();j++)
				{
					for (int k = 0; k < edgeofTopface.size();k++)
					{
						if (edgeofSideface[j]->Tag()==edgeofTopface[k]->Tag())
						{
							bianjieedgevector.push_back(edgeofSideface[j]);
						}
					}
				}
			}
		}
		else
		{
			for (int i = 0; i < GrooveSideFace.size();i++)
			{
				Face*sideface = GrooveSideFace[i];
				std::vector<NXOpen::Edge*>edgeofSideface = sideface->GetEdges();
				for (int j = 0; j < edgeofSideface.size(); j++)
				{
					for (int k = 0; k < GrooveRightFace.size();k++)
					{
						std::vector<NXOpen::Edge*>edgeofrightface = GrooveRightFace[k]->GetEdges();
						for (int a = 0; a < edgeofrightface.size();a++)
						{
							if (edgeofSideface[j]->Tag()==edgeofrightface[a]->Tag())
							{
								bianjieedgevector.push_back(edgeofSideface[j]);
							}
						}
					}
				}
			}
		}
		/*switch (bianjieedgevector.size())
		{
		case 0:
			uc1601("0", 1);
			break;
		case 7:
			uc1601("7", 1);
			break;
		case 8:
			uc1601("8", 1);
			break;
		default:
			uc1601("999", 1);
			break;
		}*/


		std::vector<NXOpen::Edge*>bianjieedgevector1;
		bianjieedgevector1.push_back(bianjieedgevector[0]);
		NXOpen::Point3d point00;
		NXOpen::Point3d point01;
		bianjieedgevector[0]->GetVertices(&point00, &point01);
		
		for (int i = 0; i < bianjieedgevector.size() - 1;i++)
		{
			for (int j = 0; j < bianjieedgevector.size();j++)
			{
				NXOpen::Point3d point02;
				NXOpen::Point3d point03;
				bianjieedgevector[j]->GetVertices(&point02, &point03);
				if (point02.X == point00.X || point02.Y == point00.Y || point02.Z == point00.Z)
				{
					int a = 0;
					for (int k = 0; k < bianjieedgevector1.size();k++)
					{
						if (bianjieedgevector[j]->Tag() == bianjieedgevector1[k]->Tag())
						{
							a = 1;
							break;
						}
					}
					if (a==0)
					{
						bianjieedgevector1.push_back(bianjieedgevector[j]);
						point00 = point03;
						break;
					}
				}
				else if (point03.X == point00.X || point03.Y == point00.Y || point03.Z == point00.Z)
				{
					int a = 0;
					for (int k = 0; k < bianjieedgevector1.size(); k++)
					{
						if (bianjieedgevector[j]->Tag() == bianjieedgevector1[k]->Tag())
						{
							a = 1;
							break;
						}
					}
					if (a == 0)
					{
						bianjieedgevector1.push_back(bianjieedgevector[j]);
						point00 = point02;
						break;
					}
				}
			}
		}



		tag_t tagarr[100];
		for (int a = 0; a < bianjieedgevector1.size(); a++)
		{
			tagarr[a] = bianjieedgevector1[a]->Tag();
		}
		int bianjienum = bianjieedgevector1.size();

		//指定部件边界
		UF_CAMBND_boundary_data_t  cutdata;
		cutdata.boundary_type = UF_CAM_boundary_type_closed;
		cutdata.material_side = UF_CAM_material_side_out_right;
		cutdata.plane_type = 1;
		cutdata.app_data = nullptr;

		UF_CAMBND_append_bnd_from_curve(operation1->Tag(), UF_CAM_part, bianjienum, tagarr, &cutdata, NULL);

		//获取底面偏置距离
		double centerdistance;

		std::vector<NXOpen::Face*>GrooveSideFaceVector = EachPassGroove->SideFaceVector;
		int k = GrooveSideFaceVector.size();
		if (k==1)
		{
			Face *TongGrooveTopface = EachPassGroove->TopFaceVector[0];
			Face *TongGrooveBottomFace = EachPassGroove->BottomFaceVector[0];
			Point3d TopiCenterPoint = GetCenterPoint(TongGrooveTopface);
			Point3d BottomiCenterPoint = GetCenterPoint(TongGrooveBottomFace);
			centerdistance = sqrt((TopiCenterPoint.X - BottomiCenterPoint.X)*(TopiCenterPoint.X - BottomiCenterPoint.X) + (TopiCenterPoint.Y - BottomiCenterPoint.Y)*(TopiCenterPoint.Y - BottomiCenterPoint.Y) + (TopiCenterPoint.Z - BottomiCenterPoint.Z)*(TopiCenterPoint.Z - BottomiCenterPoint.Z));
			
		}
		else
		{
			//第一个面和第二个面未必有相交线
			std::vector<NXOpen::Edge*>edgeofface0 = GrooveSideFaceVector[0]->GetEdges();
			std::vector<NXOpen::Edge*>edgeofface1 = GrooveSideFaceVector[1]->GetEdges();
			//已解决
			/*for (int i = 0; i < edgeofface0.size(); i++)
			{
				int a = 0;
				for (int j = 1; j < GrooveSideFaceVector.size();j++)
				{
					std::vector<NXOpen::Edge*>edgeofface1 = GrooveSideFaceVector[j]->GetEdges();
					for (int k = 0; k < edgeofface1.size();k++)
					{
						if (edgeofface0[i]->Tag()==edgeofface1[k]->Tag())
						{
							a = 1;
							centerdistance = edgeofface1[k]->GetLength();
							break;
						}
					}
					if (a==1)
					{
						break;
					}
				}
				if (a==1)
				{
					break;
				}
			}*/
			for (int i = 0; i < edgeofface0.size(); i++)
			{
				int a = 0;
				for (int j = 0; j < edgeofface1.size(); j++)
				{
					if (edgeofface0[i]->Tag() == edgeofface1[j]->Tag())
					{
						a = 1;
						centerdistance = edgeofface1[j]->GetLength();
						break;
					}
				}
				if (a == 1)
				{
					break;
				}
				else
				{
					continue;
				}
			}
		
		}

		centerdistance = -centerdistance;
		char distancechar[256];
		sprintf(distancechar, "%.6lf\t", centerdistance);

		//指定底面
		NXOpen::Plane *plane1;
		plane1 = workPart->Planes()->CreatePlane(centeripoint, iCenterNormal, NXOpen::SmartObject::UpdateOptionAfterModeling);
		plane1->SetMethod(NXOpen::PlaneTypes::MethodTypeDistance);
		std::vector<NXOpen::NXObject *> geom1(1);
		geom1[0] = PassGrooveTopFace;
		plane1->SetGeometry(geom1);
		plane1->SetFlip(false);
		plane1->SetReverseSide(false);

		NXOpen::Expression *expression5;
		expression5 = plane1->Expression();

		/*expression5->SetRightHandSide("-20");*/
		expression5->SetRightHandSide(distancechar);
		plane1->SetAlternate(NXOpen::PlaneTypes::AlternateTypeOne);
		plane1->Evaluate();
		planarMillingBuilder1->Geometry()->SetFloorPlane(plane1);


		//设置刀轴方向
		planarMillingBuilder1->ToolAxisFix()->SetToolAxisType(CAM::ToolAxisFixed::TypesFixed);
		NXOpen::Direction *direction1;
		direction1 = workPart->Directions()->CreateDirection(PassGrooveTopFace, NXOpen::SenseForward, NXOpen::SmartObject::UpdateOptionAfterModeling);
		planarMillingBuilder1->ToolAxisFix()->SetVector(direction1);

		//切削方向
		switch (PassGrooveParameterSet.CuttingDirection)
		{
		case 0:
			planarMillingBuilder1->CutParameters()->CutDirection()->SetType(CAM::CutDirection::TypesClimb);
			break;
		case 1:
			planarMillingBuilder1->CutParameters()->CutDirection()->SetType(CAM::CutDirection::TypesConventional);
			break;
		}

		//切削模式
		planarMillingBuilder1->CutPattern()->SetCutPattern(CAM::CutPatternBuilder::TypesFollowPeriphery);
		planarMillingBuilder1->CutParameters()->SetPatternDirection(CAM::CutParametersPatternDirectionTypesInward);

		planarMillingBuilder1->BndStepover()->SetStepoverType(CAM::StepoverBuilder::StepoverTypesConstant);
		planarMillingBuilder1->BndStepover()->DistanceBuilder()->SetValue(PassGrooveParameterSet.StepDistance);
		switch (PassGrooveParameterSet.StepDistanceunit)
		{
		case 0:
			planarMillingBuilder1->BndStepover()->DistanceBuilder()->SetIntent(CAM::ParamValueIntentToolDep);
			break;
		case 1:
			planarMillingBuilder1->BndStepover()->DistanceBuilder()->SetIntent(CAM::ParamValueIntentPartUnits);
			break;
		}
		//每刀切削深度
		planarMillingBuilder1->CutLevel()->CommonDepth()->SetValue(PassGrooveParameterSet.depth);

		planarMillingBuilder1->CutParameters()->PartStock()->SetValue(PassGrooveParameterSet.PartAllowance);
		planarMillingBuilder1->CutParameters()->FloorStock()->SetValue(PassGrooveParameterSet.PartAllowance);

		CAM::Inheritable2dLength *inheritable2dLength1;
		inheritable2dLength1 = planarMillingBuilder1->CutParameters()->Tolerances();

		inheritable2dLength1->SetValue0(PassGrooveParameterSet.Tolerance);
		inheritable2dLength1->SetValue1(PassGrooveParameterSet.Tolerance);

		planarMillingBuilder1->FeedsBuilder()->SpindleRpmBuilder()->SetValue(PassGrooveParameterSet.SpindleSpeed);
		planarMillingBuilder1->FeedsBuilder()->FeedCutBuilder()->SetValue(PassGrooveParameterSet.Machining);
		planarMillingBuilder1->FeedsBuilder()->FeedEngageBuilder()->SetValue(PassGrooveParameterSet.Approach);
		planarMillingBuilder1->FeedsBuilder()->FeedRetractBuilder()->SetValue(PassGrooveParameterSet.Retract);

		//封闭区域进刀类型(螺旋)
		planarMillingBuilder1->NonCuttingBuilder()->EngageClosedAreaBuilder()->SetEngRetType(NXOpen::CAM::NcmPlanarEngRetBuilder::EngRetTypesHelical);
		planarMillingBuilder1->NonCuttingBuilder()->RetractAreaBuilder()->SetEngRetType(CAM::NcmPlanarEngRetBuilder::EngRetTypesPlungeLift);

		if (PassGrooveParameterSet.ToolMotion.ToolMotionType == 0)
		{
			planarMillingBuilder1->NonCuttingBuilder()->EngageOpenAreaBuilder()->SetEngRetType(CAM::NcmPlanarEngRetBuilder::EngRetTypesArc);

			planarMillingBuilder1->NonCuttingBuilder()->EngageOpenAreaBuilder()->RadiusBuilder()->SetValue(PassGrooveParameterSet.ToolMotion.CircleRadius);
			switch (PassGrooveParameterSet.ToolMotion.CircleRadiusUnit)
			{
			case 0:
				planarMillingBuilder1->NonCuttingBuilder()->EngageOpenAreaBuilder()->RadiusBuilder()->SetIntent(CAM::ParamValueIntentToolDep);
				break;
			case 1:
				planarMillingBuilder1->NonCuttingBuilder()->EngageOpenAreaBuilder()->RadiusBuilder()->SetIntent(CAM::ParamValueIntentPartUnits);
				break;
			}
			planarMillingBuilder1->NonCuttingBuilder()->EngageOpenAreaBuilder()->HeightBuilder()->SetValue(PassGrooveParameterSet.ToolMotion.LinearExtension);
			switch (PassGrooveParameterSet.ToolMotion.LinearExtensionUnit)
			{
			case 0:
				planarMillingBuilder1->NonCuttingBuilder()->EngageOpenAreaBuilder()->HeightBuilder()->SetIntent(CAM::ParamValueIntentToolDep);
				break;
			case 1:
				planarMillingBuilder1->NonCuttingBuilder()->EngageOpenAreaBuilder()->HeightBuilder()->SetIntent(CAM::ParamValueIntentPartUnits);
				break;
			}
			planarMillingBuilder1->NonCuttingBuilder()->EngageOpenAreaBuilder()->SetArcAngle(PassGrooveParameterSet.ToolMotion.CircleAngle);
		}
		else if (PassGrooveParameterSet.ToolMotion.ToolMotionType == 1)
		{
			planarMillingBuilder1->NonCuttingBuilder()->EngageOpenAreaBuilder()->SetEngRetType(CAM::NcmPlanarEngRetBuilder::EngRetTypesLinear);

			planarMillingBuilder1->NonCuttingBuilder()->EngageOpenAreaBuilder()->LengthBuilder()->SetValue(PassGrooveParameterSet.ToolMotion.ToolMotionDistance);
			switch (PassGrooveParameterSet.ToolMotion.ToolMotionDistanceUnit)
			{
			case 0:
				planarMillingBuilder1->NonCuttingBuilder()->EngageOpenAreaBuilder()->LengthBuilder()->SetIntent(CAM::ParamValueIntentToolDep);
				break;
			case 1:
				planarMillingBuilder1->NonCuttingBuilder()->EngageOpenAreaBuilder()->LengthBuilder()->SetIntent(CAM::ParamValueIntentPartUnits);
				break;
			}
			planarMillingBuilder1->NonCuttingBuilder()->EngageOpenAreaBuilder()->SetSwingAngle(PassGrooveParameterSet.ToolMotion.RotationAngle);

			planarMillingBuilder1->NonCuttingBuilder()->EngageOpenAreaBuilder()->SetRampAngle(PassGrooveParameterSet.ToolMotion.TiltAngle);

			planarMillingBuilder1->NonCuttingBuilder()->EngageOpenAreaBuilder()->HeightBuilder()->SetValue(PassGrooveParameterSet.ToolMotion.LinearExtension);
			switch (PassGrooveParameterSet.ToolMotion.LinearExtensionUnit)
			{
			case 0:
				planarMillingBuilder1->NonCuttingBuilder()->EngageOpenAreaBuilder()->HeightBuilder()->SetIntent(CAM::ParamValueIntentToolDep);
				break;
			case 1:
				planarMillingBuilder1->NonCuttingBuilder()->EngageOpenAreaBuilder()->HeightBuilder()->SetIntent(CAM::ParamValueIntentPartUnits);
				break;
			}
		}
		else if (PassGrooveParameterSet.ToolMotion.ToolMotionType == 2)
		{
			planarMillingBuilder1->NonCuttingBuilder()->EngageOpenAreaBuilder()->SetEngRetType(CAM::NcmPlanarEngRetBuilder::EngRetTypesLinearRelativeCut);

			planarMillingBuilder1->NonCuttingBuilder()->EngageOpenAreaBuilder()->LengthBuilder()->SetValue(PassGrooveParameterSet.ToolMotion.ToolMotionDistance);
			switch (PassGrooveParameterSet.ToolMotion.ToolMotionDistanceUnit)
			{
			case 0:
				planarMillingBuilder1->NonCuttingBuilder()->EngageOpenAreaBuilder()->LengthBuilder()->SetIntent(CAM::ParamValueIntentToolDep);
				break;
			case 1:
				planarMillingBuilder1->NonCuttingBuilder()->EngageOpenAreaBuilder()->LengthBuilder()->SetIntent(CAM::ParamValueIntentPartUnits);
				break;
			}
			planarMillingBuilder1->NonCuttingBuilder()->EngageOpenAreaBuilder()->SetSwingAngle(PassGrooveParameterSet.ToolMotion.RotationAngle);

			planarMillingBuilder1->NonCuttingBuilder()->EngageOpenAreaBuilder()->SetRampAngle(PassGrooveParameterSet.ToolMotion.TiltAngle);

			planarMillingBuilder1->NonCuttingBuilder()->EngageOpenAreaBuilder()->HeightBuilder()->SetValue(PassGrooveParameterSet.ToolMotion.LinearExtension);
			switch (PassGrooveParameterSet.ToolMotion.LinearExtensionUnit)
			{
			case 0:
				planarMillingBuilder1->NonCuttingBuilder()->EngageOpenAreaBuilder()->HeightBuilder()->SetIntent(CAM::ParamValueIntentToolDep);
				break;
			case 1:
				planarMillingBuilder1->NonCuttingBuilder()->EngageOpenAreaBuilder()->HeightBuilder()->SetIntent(CAM::ParamValueIntentPartUnits);
				break;
			}
		}
		planarMillingBuilder1->NonCuttingBuilder()->EngageOpenAreaBuilder()->MinClearanceBuilder()->SetValue(10.0);

		planarMillingBuilder1->NonCuttingBuilder()->EngageOpenAreaBuilder()->MinClearanceBuilder()->SetIntent(CAM::ParamValueIntentToolDep);

		if (PassGrooveParameterSet.SafeSetType == 1)
		{
			planarMillingBuilder1->NonCuttingBuilder()->ClearanceBuilder()->SetClearanceType(CAM::NcmClearanceBuilder::ClearanceTypesAutomatic);

		}
		else if (PassGrooveParameterSet.SafeSetType == 0)
		{
			planarMillingBuilder1->NonCuttingBuilder()->ClearanceBuilder()->SetClearanceType(CAM::NcmClearanceBuilder::ClearanceTypesCylinder);
			planarMillingBuilder1->NonCuttingBuilder()->ClearanceBuilder()->SetRadius(380.0 + PassGrooveParameterSet.SafeSetOffset);
			NXOpen::Point3d iCenterPoint(0.0, 0.0, 0.0);
			NXOpen::Point *point3;
			point3 = workPart->Points()->CreatePoint(iCenterPoint);

			NXOpen::NXObject * TPointObject = (NXOpen::NXObject *) point3;
			planarMillingBuilder1->NonCuttingBuilder()->ClearanceBuilder()->SetPoint(TPointObject);

			Direction *direction1 = workPart->Directions()->CreateDirection(point3, _Normal);
			planarMillingBuilder1->NonCuttingBuilder()->ClearanceBuilder()->SetAxis(direction1);

		}
		planarMillingBuilder1->NonCuttingBuilder()->ClearanceBuilder()->SetSafeDistance(PassGrooveParameterSet.SafeSetOffset);

		planarMillingBuilder1->CutParameters()->BlankOverhang()->SetIntent(CAM::ParamValueIntentToolDep);
		planarMillingBuilder1->CutParameters()->BlankOverhang()->SetValue(25.0);

		NXObject *nXObject1;
		nXObject1 = planarMillingBuilder1->Commit();
		planarMillingBuilder1->Destroy();

		std::vector<CAM::CAMObject *> objects2(1);
		objects2[0] = planarMilling1;
		workPart->CAMSetup()->GenerateToolPath(objects2);

		UF_UI_ONT_refresh();

		UF_CAM_update_single_object_customization(planarMillingBuilder1->Tag());
		
		CAM::NCGroup *nCGroup2(dynamic_cast<CAM::NCGroup *>(workPart->CAMSetup()->CAMGroupCollection()->FindObject(PassGrooveParameterSet.PROGRAM)));
		CAM::Method *method2(dynamic_cast<CAM::Method *>(workPart->CAMSetup()->CAMGroupCollection()->FindObject(PassGrooveParameterSet.METHOD)));
		CAM::Tool *tool2(dynamic_cast<CAM::Tool *>(workPart->CAMSetup()->CAMGroupCollection()->FindObject(PassGrooveParameterSet.ToolMsg.ToolName)));
		CAM::FeatureGeometry *featureGeometry2(dynamic_cast<CAM::FeatureGeometry *>(workPart->CAMSetup()->CAMGroupCollection()->FindObject(WorkpieceName)));
		CAM::Operation *operation2;

		char* StringIndex = new char;
		sprintf(StringIndex, "%d", EachIndex);
		NXString NXStringIndex = StringIndex;
		delete StringIndex;
		NXString nxString1(tagStr.c_str());
		NXString OperationName1 = "PassGrooveSideMilling." + nxString1;
		try
		{
			operation2 = workPart->CAMSetup()->CAMOperationCollection()->Create(nCGroup2, method2, tool2, featureGeometry2, "mill_contour", "ZLEVEL_PROFILE", CAM::OperationCollection::UseDefaultNameFalse, OperationName1);
		}
		catch (...)
		{
			operation2 = workPart->CAMSetup()->CAMOperationCollection()->Create(nCGroup2, method2, tool2, featureGeometry2, "mill_contour", "ZLEVEL_PROFILE", CAM::OperationCollection::UseDefaultNameTrue, OperationName1);
		}

		CAM::ZLevelMilling *zLevelMilling1(dynamic_cast<CAM::ZLevelMilling *>(operation2));
		CAM::ZLevelMillingBuilder *zLevelMillingBuilder1;
		zLevelMillingBuilder1 = workPart->CAMSetup()->CAMOperationCollection()->CreateZlevelMillingBuilder(zLevelMilling1);


		CAM::ToolAxisFixed *toolAxisFixed1;
		toolAxisFixed1 = zLevelMillingBuilder1->ToolAxisFix();
		toolAxisFixed1->SetToolAxisType(CAM::ToolAxisFixed::TypesFixed);


		NXOpen::Scalar * u;
		u = workPart->Scalars()->CreateScalar(0.5, Scalar::DimensionalityTypeNone, SmartObject::UpdateOptionAfterModeling);
		NXOpen::Scalar * v;
		v = workPart->Scalars()->CreateScalar(0.5, Scalar::DimensionalityTypeNone, SmartObject::UpdateOptionAfterModeling);


		NXOpen::IParameterizedSurface * face = PassGrooveTopFace;
		Point *midPoint1;
		midPoint1 = workPart->Points()->CreatePoint(face, u, v, SmartObject::UpdateOptionAfterModeling);

		Direction *direction2;
		direction2 = workPart->Directions()->CreateDirection(PassGrooveTopFace, midPoint1, SenseForward, SmartObject::UpdateOptionAfterModeling);
		
		toolAxisFixed1->SetVector(direction2);



		zLevelMillingBuilder1->CutLevel()->SetCutLevelType(CAM::CutLevel::TypesOptimized);
		zLevelMillingBuilder1->CutLevel()->GlobalDepthPerCut()->DistanceBuilder()->SetValue(1);  
		zLevelMillingBuilder1->CutLevel()->ApplyGlobalDepthPerCut();


		zLevelMillingBuilder1->CutParameters()->MergeDistance()->SetValue(30.0);


		switch (PassGrooveParameterSet.CuttingDirection)
		{
		case 0:
			zLevelMillingBuilder1->CutParameters()->CutDirection()->SetType(CAM::CutDirection::TypesClimb); 
			break;
		case 1:
			zLevelMillingBuilder1->CutParameters()->CutDirection()->SetType(CAM::CutDirection::TypesConventional);  
			break;
		case 2:
			zLevelMillingBuilder1->CutParameters()->CutDirection()->SetType(CAM::CutDirection::TypesMixed); 
			break;

		}


		switch (PassGrooveParameterSet.CuttingMode)
		{
		case 0:
			zLevelMillingBuilder1->CutPattern()->SetCutPattern(CAM::CutPatternBuilder::TypesZig);
			break;
		case 1:
			zLevelMillingBuilder1->CutPattern()->SetCutPattern(CAM::CutPatternBuilder::TypesZigZag);
			zLevelMillingBuilder1->CutParameters()->CutAngle()->SetType(CAM::CutAngle::TypesAuto);   
			break;
		case 2:
			zLevelMillingBuilder1->CutPattern()->SetCutPattern(CAM::CutPatternBuilder::TypesFollowPeriphery);
			break;
		case 3:
			zLevelMillingBuilder1->CutPattern()->SetCutPattern(CAM::CutPatternBuilder::TypesProfile);
			zLevelMillingBuilder1->CutParameters()->SetPatternDirection(CAM::CutParametersPatternDirectionTypesInward);  

			break;
		default:
			break;
		}

		zLevelMillingBuilder1->CutParameters()->ExtendAtEdges()->SetStatus(true);   
		zLevelMillingBuilder1->CutParameters()->ExtendAtEdges()->Distance()->SetValue(50.0);
		
		zLevelMillingBuilder1->CutParameters()->PartStock()->SetValue(PassGrooveParameterSet.PartAllowance);

		zLevelMillingBuilder1->NonCuttingBuilder()->OverlapDistanceBuilder()->SetValue(0.1); 

		zLevelMillingBuilder1->NonCuttingBuilder()->SetDefaultRegionStartType(CAM::NcmPlanarBuilder::DefaultRegionStartTypesCorner);  


		CAM::Inheritable2dLength *inheritable2dLength2;
		inheritable2dLength2 = zLevelMillingBuilder1->CutParameters()->Tolerances();

		inheritable2dLength2->SetValue0(PassGrooveParameterSet.Tolerance);
		inheritable2dLength2->SetValue1(PassGrooveParameterSet.Tolerance);

		zLevelMillingBuilder1->FeedsBuilder()->SpindleRpmBuilder()->SetValue(PassGrooveParameterSet.SpindleSpeed);
		zLevelMillingBuilder1->FeedsBuilder()->FeedCutBuilder()->SetValue(PassGrooveParameterSet.Machining);
		zLevelMillingBuilder1->FeedsBuilder()->FeedEngageBuilder()->SetValue(PassGrooveParameterSet.Approach);
		zLevelMillingBuilder1->FeedsBuilder()->FeedRetractBuilder()->SetValue(PassGrooveParameterSet.Retract);

		

		zLevelMillingBuilder1->NonCuttingBuilder()->EngageClosedAreaBuilder()->SetEngRetType(CAM::NcmPlanarEngRetBuilder::EngRetTypesSameAsEngage);
		zLevelMillingBuilder1->NonCuttingBuilder()->RetractAreaBuilder()->SetEngRetType(CAM::NcmPlanarEngRetBuilder::EngRetTypesPlungeLift);

		if (PassGrooveParameterSet.ToolMotion.ToolMotionType == 0)
		{
			zLevelMillingBuilder1->NonCuttingBuilder()->EngageOpenAreaBuilder()->SetEngRetType(CAM::NcmPlanarEngRetBuilder::EngRetTypesArc);
			
			zLevelMillingBuilder1->NonCuttingBuilder()->EngageOpenAreaBuilder()->RadiusBuilder()->SetValue(PassGrooveParameterSet.ToolMotion.CircleRadius);
			switch (PassGrooveParameterSet.ToolMotion.CircleRadiusUnit)
			{
			case 0:
				zLevelMillingBuilder1->NonCuttingBuilder()->EngageOpenAreaBuilder()->RadiusBuilder()->SetIntent(CAM::ParamValueIntentToolDep);
				break;
			case 1:
				zLevelMillingBuilder1->NonCuttingBuilder()->EngageOpenAreaBuilder()->RadiusBuilder()->SetIntent(CAM::ParamValueIntentPartUnits);
				break;
			}
			
			zLevelMillingBuilder1->NonCuttingBuilder()->EngageOpenAreaBuilder()->HeightBuilder()->SetValue(PassGrooveParameterSet.ToolMotion.LinearExtension);
			switch (PassGrooveParameterSet.ToolMotion.LinearExtensionUnit)
			{
			case 0:
				zLevelMillingBuilder1->NonCuttingBuilder()->EngageOpenAreaBuilder()->HeightBuilder()->SetIntent(CAM::ParamValueIntentToolDep);
				break;
			case 1:
				zLevelMillingBuilder1->NonCuttingBuilder()->EngageOpenAreaBuilder()->HeightBuilder()->SetIntent(CAM::ParamValueIntentPartUnits);
				break;
			}
			
			zLevelMillingBuilder1->NonCuttingBuilder()->EngageOpenAreaBuilder()->SetArcAngle(PassGrooveParameterSet.ToolMotion.CircleAngle);
		}
		else if (PassGrooveParameterSet.ToolMotion.ToolMotionType == 1)
		{
			zLevelMillingBuilder1->NonCuttingBuilder()->EngageOpenAreaBuilder()->SetEngRetType(CAM::NcmPlanarEngRetBuilder::EngRetTypesLinear);
			
			zLevelMillingBuilder1->NonCuttingBuilder()->EngageOpenAreaBuilder()->LengthBuilder()->SetValue(PassGrooveParameterSet.ToolMotion.ToolMotionDistance);
			switch (PassGrooveParameterSet.ToolMotion.ToolMotionDistanceUnit)
			{
			case 0:
				zLevelMillingBuilder1->NonCuttingBuilder()->EngageOpenAreaBuilder()->LengthBuilder()->SetIntent(CAM::ParamValueIntentToolDep);
				break;
			case 1:
				zLevelMillingBuilder1->NonCuttingBuilder()->EngageOpenAreaBuilder()->LengthBuilder()->SetIntent(CAM::ParamValueIntentPartUnits);
				break;
			}
			
			zLevelMillingBuilder1->NonCuttingBuilder()->EngageOpenAreaBuilder()->SetSwingAngle(PassGrooveParameterSet.ToolMotion.RotationAngle);

			zLevelMillingBuilder1->NonCuttingBuilder()->EngageOpenAreaBuilder()->SetRampAngle(PassGrooveParameterSet.ToolMotion.TiltAngle);
			
			zLevelMillingBuilder1->NonCuttingBuilder()->EngageOpenAreaBuilder()->HeightBuilder()->SetValue(PassGrooveParameterSet.ToolMotion.LinearExtension);
			switch (PassGrooveParameterSet.ToolMotion.LinearExtensionUnit)
			{
			case 0:
				zLevelMillingBuilder1->NonCuttingBuilder()->EngageOpenAreaBuilder()->HeightBuilder()->SetIntent(CAM::ParamValueIntentToolDep);
				break;
			case 1:
				zLevelMillingBuilder1->NonCuttingBuilder()->EngageOpenAreaBuilder()->HeightBuilder()->SetIntent(CAM::ParamValueIntentPartUnits);
				break;
			}
		}
		else if (PassGrooveParameterSet.ToolMotion.ToolMotionType == 2)
		{
			zLevelMillingBuilder1->NonCuttingBuilder()->EngageOpenAreaBuilder()->SetEngRetType(CAM::NcmPlanarEngRetBuilder::EngRetTypesLinearRelativeCut);
			
			zLevelMillingBuilder1->NonCuttingBuilder()->EngageOpenAreaBuilder()->LengthBuilder()->SetValue(PassGrooveParameterSet.ToolMotion.ToolMotionDistance);
			switch (PassGrooveParameterSet.ToolMotion.ToolMotionDistanceUnit)
			{
			case 0:
				zLevelMillingBuilder1->NonCuttingBuilder()->EngageOpenAreaBuilder()->LengthBuilder()->SetIntent(CAM::ParamValueIntentToolDep);
				break;
			case 1:
				zLevelMillingBuilder1->NonCuttingBuilder()->EngageOpenAreaBuilder()->LengthBuilder()->SetIntent(CAM::ParamValueIntentPartUnits);
				break;
			}
			
			zLevelMillingBuilder1->NonCuttingBuilder()->EngageOpenAreaBuilder()->SetSwingAngle(PassGrooveParameterSet.ToolMotion.RotationAngle);

			zLevelMillingBuilder1->NonCuttingBuilder()->EngageOpenAreaBuilder()->SetRampAngle(PassGrooveParameterSet.ToolMotion.TiltAngle);
			
			zLevelMillingBuilder1->NonCuttingBuilder()->EngageOpenAreaBuilder()->HeightBuilder()->SetValue(PassGrooveParameterSet.ToolMotion.LinearExtension);
			switch (PassGrooveParameterSet.ToolMotion.LinearExtensionUnit)
			{
			case 0:
				zLevelMillingBuilder1->NonCuttingBuilder()->EngageOpenAreaBuilder()->HeightBuilder()->SetIntent(CAM::ParamValueIntentToolDep);
				break;
			case 1:
				zLevelMillingBuilder1->NonCuttingBuilder()->EngageOpenAreaBuilder()->HeightBuilder()->SetIntent(CAM::ParamValueIntentPartUnits);
				break;
			}
		}

		zLevelMillingBuilder1->NonCuttingBuilder()->EngageOpenAreaBuilder()->MinClearanceBuilder()->SetValue(50.0);

		zLevelMillingBuilder1->NonCuttingBuilder()->EngageOpenAreaBuilder()->MinClearanceBuilder()->SetIntent(CAM::ParamValueIntentToolDep);


		if (PassGrooveParameterSet.SafeSetType == 1)
		{
			zLevelMillingBuilder1->NonCuttingBuilder()->ClearanceBuilder()->SetClearanceType(CAM::NcmClearanceBuilder::ClearanceTypesAutomatic);   

		}
		else if (PassGrooveParameterSet.SafeSetType == 0)
		{
			zLevelMillingBuilder1->NonCuttingBuilder()->ClearanceBuilder()->SetClearanceType(CAM::NcmClearanceBuilder::ClearanceTypesCylinder);
			zLevelMillingBuilder1->NonCuttingBuilder()->ClearanceBuilder()->SetRadius(380.0 + PassGrooveParameterSet.SafeSetOffset);
			NXOpen::Point3d iCenterPoint(0.0, 0.0, 0.0);
			NXOpen::Point *point3;
			point3 = workPart->Points()->CreatePoint(iCenterPoint);

			NXOpen::NXObject * TPointObject = (NXOpen::NXObject *) point3;
			zLevelMillingBuilder1->NonCuttingBuilder()->ClearanceBuilder()->SetPoint(TPointObject);

			Direction *direction1 = workPart->Directions()->CreateDirection(point3, _Normal);
			zLevelMillingBuilder1->NonCuttingBuilder()->ClearanceBuilder()->SetAxis(direction1);

		}
		
		zLevelMillingBuilder1->NonCuttingBuilder()->ClearanceBuilder()->SetSafeDistance(PassGrooveParameterSet.SafeSetOffset);

		zLevelMillingBuilder1->CutParameters()->BlankOverhang()->SetIntent(CAM::ParamValueIntentToolDep);
		zLevelMillingBuilder1->CutParameters()->BlankOverhang()->SetValue(20.0);
		
		int NumFaces = EachPassGroove->SideFaceVector.size();
		tag_t AllTag_t[30] = {};
		for (std::vector<NXOpen::Face*>::size_type i = 0; i != NumFaces; i++)
		{
			NXOpen::Face* EachFace = EachPassGroove->SideFaceVector[i];
			tag_t EachFaceTag = EachFace->Tag();
			AllTag_t[i] = EachFaceTag;
		}

		int result1 = UF_CAMGEOM_append_items(operation2->Tag(), UF_CAM_cut_area, NumFaces, AllTag_t, NULL);
		if (result1 != 0)
		{
			char* str("cut_area error");
			uc1601(str, 1);
		}
		
		NXObject *nXObject;
		nXObject = zLevelMillingBuilder1->Commit();
		zLevelMillingBuilder1->Destroy();

		std::vector<CAM::CAMObject *> objects1(1);
		CAM::ZLevelMilling *zLevelMilling2(dynamic_cast<CAM::ZLevelMilling *>(nXObject));
		objects1[0] = zLevelMilling2;
		workPart->CAMSetup()->GenerateToolPath(objects1);

		UF_UI_ONT_refresh();

		UF_CAM_update_single_object_customization(zLevelMillingBuilder1->Tag());

		

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

	bnd_data = (UF_CAMBND_boundary_data_p_t)UF_allocate_memory(sizeof(UF_CAMBND_boundary_data_t), &err_code);

	bnd_data->boundary_type = type;
	bnd_data->plane_type = plane_type;
	for (i = 0; i<3; i++)
		bnd_data->origin[i] = origin[i];
	for (i = 0; i<9; i++)
		bnd_data->matrix[i] = matrix[i];
	bnd_data->material_side = material_side;
	bnd_data->ignore_holes = ignore_holes;
	bnd_data->ignore_islands = ignore_islands;
	bnd_data->ignore_chamfers = ignore_chamfers;
	bnd_data->app_data = app_data;

	return(bnd_data);
}

UF_CAMBND_app_data_p_t PassGrooveSideMillingCmd::create_and_init_bnd_app_data(void)
{
	UF_CAMBND_app_data_p_t  app_data;
	int                     err_code;

	app_data = (UF_CAMBND_app_data_p_t)UF_allocate_memory(sizeof(UF_CAMBND_app_data_t), &err_code);

	if (err_code)
		return(NULL);

	/* Set flags. */
	app_data->has_stock = 0;
	app_data->has_tolerances = 0;
	app_data->has_feedrate = 0;
	app_data->has_blank_distance = 0;
	app_data->has_tool_position = 0;

	/* Set values. */
	app_data->stock = 0.1;
	app_data->tolerances[0] = 0.1;
	app_data->tolerances[1] = 0.1;
	app_data->feedrate_unit = UF_CAM_feedrate_unit_per_minute;
	app_data->feedrate_value = 0.1;
	app_data->blank_distance = 0.0;
	app_data->tool_position = UF_CAM_tool_position_on;

	return(app_data);
}

static void set_camgeom_app_data(tag_t item, int geom_type, int num_app_data, UF_CAMGEOM_app_data_p_t *app_data_list)
{
	int type, subtype, i;
	UF_CAMGEOM_app_data_p_t app_data;

	UF_OBJ_ask_type_and_subtype(item, &type, &subtype);

	for (i = 0; i < num_app_data; i++)
	{
		app_data = app_data_list[i];

		app_data->has_stock = 0;
		app_data->has_cut_stock = 0;
		app_data->has_tolerances = 0;
		app_data->has_feedrate = 0;
		app_data->has_offset = 0;
		app_data->has_avoidance_type = 0;

		if (type == UF_machining_geometry_grp_type &&
			subtype == UF_mill_geom_featr_subtype &&
			geom_type == UF_CAM_part)
		{
			app_data->has_offset = 1;
			app_data->offset = 0.1;
		}

		else if (type == UF_machining_operation_type)
		{
			if (subtype == UF_mach_cavity_milling_subtype)
			{
				if (geom_type == UF_CAM_part || geom_type == UF_CAM_blank)
				{
					app_data->has_stock = 1;
					app_data->has_tolerances = 1;
					app_data->stock = 0.001;
					app_data->tolerances[0] = 0.001;
					app_data->tolerances[1] = 0.001;
				}
			}
			else if (subtype == UF_mach_zlevel_milling_subtype)
			{
				if (geom_type == UF_CAM_part)
				{
					app_data->has_stock = 1;
					app_data->has_tolerances = 1;
					app_data->stock = 0.001;
					app_data->tolerances[0] = 0.001;
					app_data->tolerances[1] = 0.001;
				}
			}
			else if (subtype == UF_mach_surface_contour_subtype ||
				subtype == UF_mach_vasc_subtype)
			{
				if (geom_type == UF_CAM_part || geom_type == UF_CAM_check)
				{
					app_data->has_cut_stock = 1;
					app_data->cut_stock[0] = 0.2;
					app_data->cut_stock[1] = 0.1;
					app_data->cut_stock[2] = 0.5;
				}
				if (geom_type == UF_CAM_part)
				{
					app_data->has_feedrate = 1;
					app_data->feedrate_unit = UF_CAM_feedrate_unit_per_minute;
					app_data->feedrate_value = 10.0;
				}
				else if (geom_type == UF_CAM_check)
				{
					app_data->has_avoidance_type = 1;
					app_data->avoidance_type = UF_CAM_avoidance_type_warning;
				}
			}
		}
	}
}
