//#include "stdafx.h"
#include "ArcCavityBottomMillingCmd.h"
using namespace NXOpen;

//-----------------------------------------------------------------------------
// BossTopMillingCmd : constructor
//-----------------------------------------------------------------------------
ArcCavityBottomMillingCmd::ArcCavityBottomMillingCmd():
TaggedObject()
{
}

ArcCavityBottomMillingCmd::ArcCavityBottomMillingCmd(std::vector<ArcCavity*> ListOfCavity, /*std::vector<Boss*> ListOfBoss,*/
													 ArcCavityProcessParameter ParameterSet, Part* WorkPart, Body* WorkBody,
													 std::vector<Face*> ListOfSelectedFace):
TaggedObject()
{
	ArcCavityResultList = ListOfCavity;       //环形槽识别结果
	//BossResultList          = ListOfBoss;         //凸台识别结果
	ArcCavityParameterSet = ParameterSet;     //工艺决策参数
	workPart     = WorkPart;               //工作部件
	selectedBody = WorkBody;               //零件体
	SelectedFaceList = ListOfSelectedFace;
	_Normal = ArcCavityParameterSet.Approch_Normal;

	UF_initialize();

	if (ArcCavityParameterSet.surfaceDriveFlag)
	{
		/*char* str ("曲面驱动模式还未开发完善，暂时无法生成刀轨，请选择曲线驱动方式") ;

		(str,1);*/
	}
	if (ArcCavityParameterSet.curveDriveFlag)
	{
		if (ArcCavityParameterSet.FaceSelected)
		{
			SelectedArcCavity.clear();
			ArcCavityIndex.clear();
			for (std::vector<Face*>::size_type i = 0; i != SelectedFaceList.size(); i++)
			{
				Face* EachFace = SelectedFaceList[i] ;
				tag_t EachFaceTag = EachFace->Tag() ;
				for (std::vector<ArcCavity*>::size_type j = 0 ; j != ArcCavityResultList.size() ; j++)
				{
					ArcCavity* EachArcCavity = ArcCavityResultList[j];
					Face* EachArcCavityBottomFace = EachArcCavity->BottomFaceVector[0];
					if (EachFaceTag == EachArcCavityBottomFace->Tag())
					{
						SelectedArcCavity.push_back(EachArcCavity);
						ArcCavityIndex.push_back(j) ;
					}
				}
			}
			ArcCavityBottomMillingDecision1(SelectedArcCavity);
		}
		else
		{
			ArcCavityBottomMillingDecision();
		}
	}
	else
	{
		char* str ("请勾选曲线驱动，再生成刀轨") ;
		uc1601(str,1);
	}

	UF_terminate();
}

//-----------------------------------------------------------------------------
// BossTopMillingCmd : destructor
//-----------------------------------------------------------------------------
ArcCavityBottomMillingCmd::~ArcCavityBottomMillingCmd()
{
}

void ArcCavityBottomMillingCmd::ArcCavityBottomMillingDecision()
{
	int Index = 0;
	for (std::vector<ArcCavity*>::size_type i = 0; i != ArcCavityResultList.size(); i++)
	{
		ArcCavity* EachAnnulusCavity = ArcCavityResultList[i];

		for (std::vector<NXOpen::Face*>::size_type k = 0; k !=  EachAnnulusCavity->BottomFaceVector.size(); k++)
		{
			NXOpen::Face* EachBottomFace = EachAnnulusCavity->BottomFaceVector[k];//取出每一个环形槽底面

			//限制面积太小的环形槽（可能就是一根线那种）
			double area = calcArea(workPart, EachBottomFace);
			if (area < 1000)
			{
				continue;
			}

			Index ++;
			ArcCavityBottomMillOperation2(EachBottomFace, Index);

			//环形槽现在都是整环面，所以下面的计算先全部注释
			//double MaxAngle;
			//double MaxRadius;
			//NXOpen::Point3d MidVertex;
			//get_surface_angle(EachBottomFace, MaxAngle, MaxRadius, MidVertex);
			//
			////NXOpen::Vector3d iMidVector (MidVertex.X, MidVertex.Y, 0);
			////double VectorLength = sqrt(MidVertex.X*MidVertex.X+MidVertex.Y*MidVertex.Y);
			////NXOpen::Vector3d MidVector (MidVertex.X/VectorLength, MidVertex.Y/VectorLength, 0);

			////NXOpen::Vector3d OriginalVector (1 , 0 , 0);
			////double vector1[3] = {MidVector.X , MidVector.Y , MidVector.Z};// First vector
			////double vector2[3] = {1 , 0 , 0};// Second vector
			////double small_angle ;// Small angle (in radians)
			////double large_angle ;// Large angle (in radians)              
			////int flag = UF_MODL_ask_vector_angle(vector1, vector2, &small_angle, &large_angle);
			////double MidAngle;//得到初始角度，采用角度制
			////if(MidVector.Y >= 0)
			////	MidAngle = small_angle*180/PI ;
			////if(MidVector.Y < 0)
			////	MidAngle = large_angle*180/PI ;
			//
			////int NumMill = 0;
			////if(MaxAngle > 300 && MaxAngle <= 360)//360°完整环形槽(严格来讲，还应该考虑凸台的位置及相对高度)
			////{
			////	NumMill = 6;
			////	//for(int i = 0; i < NumMill; i++)
			////	//{
			////	//	NXOpen::Point3d iCenterPoint (0, 0, StartVertex.Z);
			////	//	NXOpen::Vector3d iCenterNormal (cos(60*i*PI/180), sin(60*i*PI/180), StartVertex.Z);
			////	//	AnnulusCavityBottomMillOperation(EachBottomFace,  iCenterPoint, iCenterNormal);
			////	//}
			////}
			////if(MaxAngle <= 60)
			////{
			////	NumMill = 1;
			////	//NXOpen::Point3d iCenterPoint;
			////	//NXOpen::Vector3d iCenterNormal;
			////	//get_Centre_fr_Face(EachBottomFace, iCenterPoint, iCenterNormal);
			////	////取用了面中心点的法矢作为刀轴方向
			////	//AnnulusCavityBottomMillOperation(EachBottomFace,  iCenterPoint, iCenterNormal);
			////}					
			////if(MaxAngle > 60 && MaxAngle <= 120)
			////	NumMill = 2;
			////if(MaxAngle > 120 && MaxAngle <= 180)
			////	NumMill = 3;
			////if(MaxAngle > 180 && MaxAngle <= 240)
			////	NumMill = 4;
			////if(MaxAngle > 240 && MaxAngle <= 300)
			////	NumMill = 5;

			//int NumMill = 6;
			//for(int n = 0; n < 1/*NumMill*/; n++)
			//{
			//	double iAngle ;
			//	iAngle = (60*n - 30*NumMill + 30) *PI /180 ;
			//	NXOpen::Point3d iCenterPoint ;
			//	iCenterPoint.X = MaxRadius * cos(iAngle);
			//	iCenterPoint.Y = MaxRadius * (-1) * sin(iAngle);
			//	iCenterPoint.Z = MidVertex.Z;

			//	//get_Centre_fr_Face(EachBottomFace, iCenterPoint, iCenterNormal);

			//	AnnulusCavityBottomMillOperation(EachBottomFace,  iCenterPoint, k, n);
			//}
		}
	}
}

void ArcCavityBottomMillingCmd::ArcCavityBottomMillingDecision1(std::vector<ArcCavity*> ArcCavityList)
{

	for (std::vector<ArcCavity*>::size_type i = 0; i != ArcCavityList.size(); i++)
	{
		int Index = ArcCavityIndex[i];
		ArcCavity* EachAnnulusCavity = ArcCavityList[i];

		for (std::vector<NXOpen::Face*>::size_type k = 0; k !=  EachAnnulusCavity->BottomFaceVector.size(); k++)
		{
			NXOpen::Face* EachBottomFace = EachAnnulusCavity->BottomFaceVector[k];//取出每一个环形槽底面

			//限制面积太小的环形槽（可能就是一根线那种）
			double area = calcArea(workPart, EachBottomFace);
			if (area < 1000)
			{
				continue;
			}

			//Index ++;
			ArcCavityBottomMillOperation2(EachBottomFace, Index);

		}
	}
}

//----------------------------------------弧形槽精加工，画驱动圆弧曲线做可变轴铣削
void ArcCavityBottomMillingCmd::ArcCavityBottomMillOperation2(NXOpen::Face* iFace, int k)
{
	//可变轴的方法  用于加工大弧度下陷槽（弧形槽）
	CAM::NCGroup *nCGroup1(dynamic_cast<CAM::NCGroup *>(workPart->CAMSetup()->CAMGroupCollection()->FindObject(ArcCavityParameterSet.PROGRAM)));
	CAM::Method *method1(dynamic_cast<CAM::Method *>(workPart->CAMSetup()->CAMGroupCollection()->FindObject(ArcCavityParameterSet.METHOD)));
	CAM::Tool *tool1(dynamic_cast<CAM::Tool *>(workPart->CAMSetup()->CAMGroupCollection()->FindObject(ArcCavityParameterSet.ToolMsg.ToolName)));
	CAM::FeatureGeometry *featureGeometry1(dynamic_cast<CAM::FeatureGeometry *>(workPart->CAMSetup()->CAMGroupCollection()->FindObject("WORKPIECE_GEOBLANK")));
	CAM::Operation *operation1;

	char* StringIndex = new char;
	sprintf(StringIndex, "%d", k);
	NXString NXStringIndex = StringIndex;
	NXString OperationName = "ArcCBottom." + NXStringIndex ;
	delete StringIndex;

	try
	{
		operation1 = workPart->CAMSetup()->CAMOperationCollection()->Create(nCGroup1, method1, tool1, featureGeometry1, 
			"mill_multi-axis", "VARIABLE_CONTOUR", CAM::OperationCollection::UseDefaultNameFalse, OperationName);
	}
	catch(...)
	{
		//uc1601("Read XML file failed!\n Error is:\n 操作名字重复了!",1);
		OperationName = "ArcCBottom." + NXStringIndex + "_s";

		operation1 = workPart->CAMSetup()->CAMOperationCollection()->Create(nCGroup1, method1, tool1, featureGeometry1, 
			"mill_multi-axis", "VARIABLE_CONTOUR", CAM::OperationCollection::UseDefaultNameTrue, OperationName);
	}

	CAM::SurfaceContour *surfaceContour1(dynamic_cast<CAM::SurfaceContour *>(operation1));
	CAM::SurfaceContourBuilder *surfaceContourBuilder1;
	surfaceContourBuilder1 = workPart->CAMSetup()->CAMOperationCollection()->CreateSurfaceContourBuilder(surfaceContour1);

	//刀轴可变
	CAM::ToolAxisVariable *toolAxisVariable1;
	toolAxisVariable1 = surfaceContourBuilder1->ToolAxisVariable();
	toolAxisVariable1->SetToolAxisType(CAM::ToolAxisVariable::Types::TypesNormalToDrive);  //TypesNormalToPart

	//切削区域
	tag_t iFaceTag = iFace->Tag();
	int cut_area_flag = UF_CAMGEOM_append_items(operation1->Tag(), UF_CAM_cut_area, 1, &iFaceTag, NULL);

	//int NumBottomFaces = EachSubsidenceCavity->BottomFaceVector.size();
	//tag_t AllTag_t[10] = {};
	//for(std::vector<NXOpen::Face*>::size_type i = 0; i != NumBottomFaces; i++)
	//{
	//	NXOpen::Face* EachBottomFace = EachSubsidenceCavity->BottomFaceVector[i];
	//	tag_t EachBottomFaceTag = EachBottomFace->Tag();
	//	AllTag_t[i] = EachBottomFaceTag;
	//}
	//int cut_area_flag = UF_CAMGEOM_append_items(operation1->Tag(), UF_CAM_cut_area, NumBottomFaces, AllTag_t, NULL);
	if (cut_area_flag != 0)
	{
		char* str ("cut area error") ;
		uc1601(str,1);
	}

	//驱动方式  - 曲线驱动
	surfaceContourBuilder1->SetDriveMethod(CAM::SurfaceContourBuilder::DriveMethodTypesCurve);  //DriveMethodTypesSurfaceArea

	//主法向
	double SqrtOfNormal = sqrt(pow(_Normal.X, 2) +pow(_Normal.Y, 2) +pow(_Normal.Z, 2)) ;                         //单位法向的模为1

	//构建驱动曲线
	//算圆锥面的倾斜角度
	double MfgToolDiameter = ArcCavityParameterSet.ToolMsg.ToolDiameter;

	double scallopValue = ArcCavityParameterSet.ScallopHeightAmount;
	double lowCornerRad = ArcCavityParameterSet.ToolMsg.ToolCorner;
	double MfgPercentage = 0;
	//通过残余高度得到步距的直径比
	if (lowCornerRad != 0)
	{
		double tempValue = sqrt(2*lowCornerRad*scallopValue - scallopValue*scallopValue);
		MfgPercentage = (MfgToolDiameter - 2*lowCornerRad + 2*tempValue)/MfgToolDiameter;  
	}
	else
	{
		MfgPercentage = 0.5;  
	}

	//double MfgPercentage  =  0.3;     //暂定插铣的每一层之间的刀位间隙为刀具半径的70%  ----------------后面通过参与高度计算切宽
	double GeneratrixAngle = 0;       //弧度制表示的圆锥面的母线与轴线的夹角
	int NumPath = 0;                  // 计算刀路数，即为有多少层？

	Face::FaceType EachFaceType = iFace->SolidFaceType();
	if (EachFaceType == Face::FaceType::FaceTypeConical)
	{
		int face1_type;
		double face1_point[3];
		double face1_dir[3];
		double face1_box[6];
		double face1_radius;
		double face1_rad_data;
		int face1_norm_dir;
		UF_MODL_ask_face_data(iFaceTag,&face1_type,face1_point,face1_dir, face1_box,&face1_radius,&face1_rad_data,&face1_norm_dir);

		if (face1_rad_data != 0)
		{
			GeneratrixAngle = face1_rad_data;
		}
	}
	//求出外环边界
	std::vector<Edge*> AllEdges; 
	AllEdges.clear();
	uf_loop_p_t loop_list = NULL;
	UF_MODL_ask_face_loops(iFaceTag, &loop_list);
	int loops_count = 0;
	UF_MODL_ask_loop_list_count(loop_list,&loops_count);
	for (int i = 0; i < loops_count; i++)
	{
		int loop_stype = 0;
		uf_list_p_t EdgeList = NULL;
		UF_MODL_ask_loop_list_item(loop_list, i, &loop_stype, &EdgeList);
		if (loop_stype != 2)   //Peripheral=1, Hole=2, Other=3
		{
			int EdgeListCount = 0;
			UF_MODL_ask_list_count(EdgeList, &EdgeListCount);
			for (int j = 0 ; j < EdgeListCount; j++)
			{
				tag_t Edge_Tag = NULL;
				UF_MODL_ask_list_item(EdgeList, j, &Edge_Tag);
				NXObjectManager EdgeManager;
				TaggedObject* pTaggedObject_Edge_Each = EdgeManager.Get(Edge_Tag);
				Edge* pEdge_Each = dynamic_cast<Edge*>(pTaggedObject_Edge_Each);

				AllEdges.push_back(pEdge_Each);//存入非孔边界列表中
			}
		}
	}
	//求出上下高度的极限位置
	std::vector<NXOpen::Arc*>  AxisArcs;
	AxisArcs.clear();
	std::vector<double> vectorZ;
	vectorZ.clear();
	for (std::vector<Edge*>::size_type i = 0; i != AllEdges.size(); i ++)
	{
		Edge* EveryEdge = AllEdges[i];

		int typeN;
		UF_MODL_ask_edge_type(EveryEdge->Tag() , &typeN);
		if (typeN == UF_MODL_CIRCULAR_EDGE) //  UF_MODL_CIRCULAR_EDGE — 3002
		{
			tag_t iEdge_tag = EveryEdge->Tag();
			tag_t iCurve_tag;
			UF_MODL_create_curve_from_edge(iEdge_tag, &iCurve_tag);

			NXObjectManager CurveManager;
			NXOpen::TaggedObject* pTaggedObject_Curve = CurveManager.Get(iCurve_tag);
			NXOpen::Curve* pCurve = dynamic_cast<Curve*>(pTaggedObject_Curve);

			//  注意arc也可以用来直接得到圆弧的一些参数，如起始角度，终末角度等等
			NXOpen::Arc * iEdgeArc(dynamic_cast<NXOpen::Arc *>(pCurve));
			NXOpen::Point3d iCenterPoint = iEdgeArc->CenterPoint();
			double CosFromNormal = (iCenterPoint.X*_Normal.X + iCenterPoint.Y*_Normal.Y + iCenterPoint.Z*_Normal.Z)/sqrt(pow(iCenterPoint.X, 2) +pow(iCenterPoint.Y, 2) +pow(iCenterPoint.Z, 2)) ;
			if (CosFromNormal = 1)  //圆心在中心轴线上
			{
				double z = 0;
				z = iCenterPoint.X*_Normal.X + iCenterPoint.Y*_Normal.Y + iCenterPoint.Z*_Normal.Z ;
				vectorZ.push_back(z);
				AxisArcs.push_back(iEdgeArc);
			}
			else
			{
				int ArcDeleteFlag = UF_OBJ_delete_object(iEdgeArc->Tag());
			}
		}
		else
		{
			int EdgeDeleteFlag = UF_OBJ_delete_object(EveryEdge->Tag());
		}
	}
	double maxZ = -10000;
	double minZ =  10000;
	for (int m = 0; m < vectorZ.size(); m ++)                      //?
	{
		double iZ = vectorZ[m];
		if(iZ > maxZ)
			maxZ = iZ;
		if (iZ < minZ)
			minZ = iZ;
	}
	//double aaa = (maxZ - minZ)/cos(GeneratrixAngle) - MfgToolDiameter;  //原来的
	double aaa = (maxZ - minZ)/cos(GeneratrixAngle) - MfgToolDiameter*MfgPercentage;    //新的，还没仔细分析
	if (aaa >= 0)
	{
		double TempPathNum = aaa/ (MfgToolDiameter*MfgPercentage);  
		NumPath = ceil(TempPathNum) + 1;
	}
	//求弧形槽的最大最小半径
	double maxRadius = -100000;
	double minRadius = 1000000;
	for (std::vector<NXOpen::Arc*>::size_type i = 0; i != AxisArcs.size(); i ++)
	{
		NXOpen::Arc * iArc = AxisArcs[i];
		NXOpen::Point3d iCenterPoint = iArc->CenterPoint();
		if (iCenterPoint.X*_Normal.X + iCenterPoint.Y*_Normal.Y + iCenterPoint.Z*_Normal.Z == maxZ)
		{
			double tempRadius1 = iArc->Radius();
			if (tempRadius1 < minRadius)
			{
				minRadius = tempRadius1;
			}
		}
		if (iCenterPoint.X*_Normal.X + iCenterPoint.Y*_Normal.Y + iCenterPoint.Z*_Normal.Z == minZ)
		{
			double tempRadius = iArc->Radius();
			if (tempRadius > maxRadius)
			{
				maxRadius = tempRadius;
			}
		}
		//UF_MODL_delete_feature 
		int iArcDeleteFlag = UF_OBJ_delete_object(iArc->Tag());
		int statusFlag = UF_OBJ_ask_status(iArc->Tag());
		if (statusFlag == 4)
		{
			char* str ("delete arc fail") ;
			uc1601(str,1);
		}
	}
	if (NumPath == 0)
	{
		int operationDeleteFlag = UF_OBJ_delete_object(operation1->Tag());
		return;
	}
	//求交得到驱动曲线
	std::vector<NXOpen::Curve *> vectorofcurves ;
	vectorofcurves.clear();
	for (int i = 0; i < NumPath; i ++) 
	{
		if (i == NumPath -1) //最后一刀，反向生成
		{
			double centerZ = maxZ-(MfgToolDiameter*0.5);            
			double centerPoint[3] = {_Normal.X*centerZ, _Normal.Y*centerZ, _Normal.Z*centerZ};  //Gai
			double PlaneNormal[3] = {_Normal.X, _Normal.Y, _Normal.Z};
			tag_t  plane_tag;
			int CreatePlaneFlag = UF_MODL_create_plane(centerPoint, PlaneNormal, &plane_tag);

			UF_MODL_intersect_info_p_t* intersectInfo;
			int NumIntersections = -1;
			UF_MODL_intersect_objects(iFaceTag, plane_tag, 0, &NumIntersections, &intersectInfo);

			for (int j = 1; j <= NumIntersections; j ++)
			{
				if ((*intersectInfo)->intersect_type == UF_MODL_INTERSECT_CURVE)
				{
					UF_MODL_intersect_info_intersect_t intersect = (*intersectInfo)->intersect;
					UF_MODL_intersect_info_curve_t curve = intersect.curve;
					tag_t intersectCurveTag = curve.identifier;

					NXObjectManager intersectCurveManager;
					NXOpen::TaggedObject* pTaggedObject_IntersectCurve = intersectCurveManager.Get(intersectCurveTag);
					NXOpen::Curve* jIntersectCurve = dynamic_cast<Curve*>(pTaggedObject_IntersectCurve);
					vectorofcurves.push_back(jIntersectCurve);
				}
			}		
		}
		else  //第一刀顺着开始
		{
			double BossAboveSideDis = 0;
			double centerZ = minZ + BossAboveSideDis + (MfgToolDiameter*0.5) + MfgToolDiameter*MfgPercentage*(i);
			double centerPoint[3] = {_Normal.X*centerZ, _Normal.Y*centerZ, _Normal.Z*centerZ};     //Gai
			double PlaneNormal[3] = {_Normal.X, _Normal.Y, _Normal.Z};
			tag_t  plane_tag;
			int CreatePlaneFlag = UF_MODL_create_plane(centerPoint, PlaneNormal, &plane_tag);

			UF_MODL_intersect_info_p_t* intersectInfo;
			int NumIntersections = -1;
			UF_MODL_intersect_objects(iFaceTag, plane_tag, 0, &NumIntersections, &intersectInfo);

			for (int j = 1; j <= NumIntersections; j ++)
			{
				if ((*intersectInfo)->intersect_type == UF_MODL_INTERSECT_CURVE)
				{
					UF_MODL_intersect_info_intersect_t intersect = (*intersectInfo)->intersect;
					UF_MODL_intersect_info_curve_t curve = intersect.curve;
					tag_t intersectCurveTag = curve.identifier;

					NXObjectManager intersectCurveManager;
					NXOpen::TaggedObject* pTaggedObject_IntersectCurve = intersectCurveManager.Get(intersectCurveTag);
					NXOpen::Curve* jIntersectCurve = dynamic_cast<Curve*>(pTaggedObject_IntersectCurve);
					vectorofcurves.push_back(jIntersectCurve);
				}
			}
		}
	}
	//调整驱动曲线边界角度
	std::vector<NXOpen::NXObject *> vectorOfResultCurves ;
	vectorOfResultCurves.clear();
	std::vector<NXOpen::Point3d> vectorOfCenterPoint;
	vectorOfCenterPoint.clear();
	for (std::vector<NXOpen::Curve *>::size_type i = 0; i != vectorofcurves.size(); i ++)
	{
		NXOpen::Curve* tempCurve = vectorofcurves[i];
		NXOpen::Arc * tempArc(dynamic_cast<NXOpen::Arc *>(tempCurve));

		double tempArcStartAngle = tempArc->StartAngle();
		double tempArcEndAngle = tempArc->EndAngle();
		NXOpen::Point3d tempArcCenterPoint = tempArc->CenterPoint();
		vectorOfCenterPoint.push_back(tempArcCenterPoint);
		double tempArcRadius = tempArc->Radius();
		double BossLeftSideDis = 2;
		double BossAroundAngle = (BossLeftSideDis + MfgToolDiameter*0.5) / tempArcRadius;

		tempArc->SetParameters(tempArcRadius, tempArcCenterPoint, tempArcStartAngle+BossAroundAngle, tempArcEndAngle-BossAroundAngle);
		NXOpen::Curve* resultCurve = /*(NXOpen::Curve*)*/ tempArc;
		vectorOfResultCurves.push_back(resultCurve);
		UF_OBJ_set_blank_status(resultCurve->Tag(), UF_OBJ_BLANKED);          //隐藏
	}

	//赋驱动曲线，构建驱动
	Section *section1;	
	NXOpen::SectionCollection::LoopOption loopOption = NXOpen::SectionCollection:: LoopOptionSeparate;
	std::vector<NXOpen::Section *>  vectorofsection;
	vectorofsection.clear();
	workPart->Sections()->CreateSectionsUsingCurves(vectorOfResultCurves, loopOption, 0.005, 0.005, 0.05, vectorofsection);

	double sectionPointZ ;
	if (_Normal.Z != 0)
	{
		sectionPointZ=vectorOfCenterPoint[0].Z;
	}
	else if (_Normal.X != 0)
	{
		sectionPointZ=vectorOfCenterPoint[0].X;
	}
	else if (_Normal.Y != 0)
	{
		sectionPointZ=vectorOfCenterPoint[0].Y;
	}

	int numP = 1;
	for (std::vector<NXOpen::Section>::size_type i = 0; i != vectorofsection.size(); i ++)
	{
		section1 = vectorofsection[i];
		NXOpen::Point3d sectionPoint = vectorOfCenterPoint[i];

		int yushu = numP % 2;
		if (_Normal.Z != 0)
		{
			if (sectionPointZ != sectionPoint.Z && yushu == 1)
			{
				numP ++;
				section1->ReverseDirectionOfLoop(0);
				sectionPointZ = sectionPoint.Z;
			}
		}
		else if (_Normal.X != 0)
		{
			if (sectionPointZ != sectionPoint.X && yushu == 1)
			{
				numP ++;
				section1->ReverseDirectionOfLoop(0);
				sectionPointZ = sectionPoint.X;
			}
		}
		else if (_Normal.Y != 0)
		{
			if (sectionPointZ != sectionPoint.Y && yushu == 1)
			{
				numP ++;
				section1->ReverseDirectionOfLoop(0);
				sectionPointZ = sectionPoint.Y;
			}
		}

		CAM::DriveChainItemBuilder *driveChainItemBuilder1;
		driveChainItemBuilder1 = surfaceContourBuilder1->DmcurveBuilder()->CreateDpmcvDriveCurve(section1);
		surfaceContourBuilder1->DmcurveBuilder()->DpmcvDriveCurves()->Append(driveChainItemBuilder1);
	}
	//for (std::vector<NXOpen::Curve *>::size_type i = 0; i != vectorOfResultCurves.size(); i ++)
	//{
	//	NXOpen::NXObject * NXObjectCurve = vectorOfResultCurves[i];

	//int NXObjectCurveDeleteFlag = UF_OBJ_delete_object(NXObjectCurve->Tag());
	//if (NXObjectCurveDeleteFlag != 0)
	//{
	//	char* str ("delete ResultCurves fail") ;
	//	uc1601(str,1);
	//}
	//}
	//-------------------------------------赋值加工参数
	//部件余量
	surfaceContourBuilder1->CutParameters()->PartStock()->SetValue(ArcCavityParameterSet.PartAllowance);

	//公差
	CAM::Inheritable2dLength *inheritable2dLength1;
	inheritable2dLength1 = surfaceContourBuilder1->CutParameters()->Tolerances();

	inheritable2dLength1->SetValue0(ArcCavityParameterSet.Tolerance);
	inheritable2dLength1->SetValue1(ArcCavityParameterSet.Tolerance);

	//主轴转速以及三个速度
	surfaceContourBuilder1->FeedsBuilder()->SpindleRpmBuilder()->SetValue(ArcCavityParameterSet.SpindleSpeed);
	surfaceContourBuilder1->FeedsBuilder()->FeedCutBuilder()->SetValue(ArcCavityParameterSet.Machining);
	surfaceContourBuilder1->FeedsBuilder()->FeedEngageBuilder()->SetValue(ArcCavityParameterSet.Approach);
	surfaceContourBuilder1->FeedsBuilder()->FeedRetractBuilder()->SetValue(ArcCavityParameterSet.Retract);

	//退刀设置
	surfaceContourBuilder1->NonCuttingBuilder()->RetractOpenAreaBuilder()->SetEngRetType(CAM::NcmScEngRetBuilder::EngRetTypesPlungeLift);
	//-------------区域内非切移动
	surfaceContourBuilder1->NonCuttingBuilder()->TransferWithinRegionsBuilder()->SetApproachMethod(CAM::NcmTransferBuilder::AppDepMethodTypesAlongToolAxis);
	surfaceContourBuilder1->NonCuttingBuilder()->TransferWithinRegionsBuilder()->SetDepartureMethod(CAM::NcmTransferBuilder::AppDepMethodTypesAlongToolAxis);


	//进刀设置
	if (ArcCavityParameterSet.ToolMotion.ToolMotionType == 0)
	{
		surfaceContourBuilder1->NonCuttingBuilder()->EngageOpenAreaBuilder()->SetEngRetType(CAM::NcmScEngRetBuilder::EngRetTypesArcParallelToToolAxis);
		//圆弧半径
		surfaceContourBuilder1->NonCuttingBuilder()->EngageOpenAreaBuilder()->RadiusBuilder()->SetValue(ArcCavityParameterSet.ToolMotion.CircleRadius);
		switch(ArcCavityParameterSet.ToolMotion.CircleRadiusUnit)
		{
		case 0:
			surfaceContourBuilder1->NonCuttingBuilder()->EngageOpenAreaBuilder()->RadiusBuilder()->SetIntent(CAM::ParamValueIntentToolDep);
			break;
		case 1:
			surfaceContourBuilder1->NonCuttingBuilder()->EngageOpenAreaBuilder()->RadiusBuilder()->SetIntent(CAM::ParamValueIntentPartUnits);
			break;
		}
		//线性延伸
		surfaceContourBuilder1->NonCuttingBuilder()->EngageOpenAreaBuilder()->LinearExtensionBuilder()->SetValue(ArcCavityParameterSet.ToolMotion.LinearExtension);
		switch(ArcCavityParameterSet.ToolMotion.LinearExtensionUnit)
		{
		case 0:
			surfaceContourBuilder1->NonCuttingBuilder()->EngageOpenAreaBuilder()->LinearExtensionBuilder()->SetIntent(CAM::ParamValueIntentToolDep);
			break;
		case 1:
			surfaceContourBuilder1->NonCuttingBuilder()->EngageOpenAreaBuilder()->LinearExtensionBuilder()->SetIntent(CAM::ParamValueIntentPartUnits);    //HeightBuilder
			break;
		}
		//圆弧角度
		surfaceContourBuilder1->NonCuttingBuilder()->EngageOpenAreaBuilder()->SetArcAngle(ArcCavityParameterSet.ToolMotion.CircleAngle);
	}
	else if (ArcCavityParameterSet.ToolMotion.ToolMotionType == 1)
	{
		surfaceContourBuilder1->NonCuttingBuilder()->EngageOpenAreaBuilder()->SetEngRetType(CAM::NcmScEngRetBuilder::EngRetTypesArcNormalToToolAxis);
		//圆弧半径
		surfaceContourBuilder1->NonCuttingBuilder()->EngageOpenAreaBuilder()->RadiusBuilder()->SetValue(ArcCavityParameterSet.ToolMotion.CircleRadius);
		switch(ArcCavityParameterSet.ToolMotion.CircleRadiusUnit)
		{
		case 0:
			surfaceContourBuilder1->NonCuttingBuilder()->EngageOpenAreaBuilder()->RadiusBuilder()->SetIntent(CAM::ParamValueIntentToolDep);
			break;
		case 1:
			surfaceContourBuilder1->NonCuttingBuilder()->EngageOpenAreaBuilder()->RadiusBuilder()->SetIntent(CAM::ParamValueIntentPartUnits);
			break;
		}
		//线性延伸
		surfaceContourBuilder1->NonCuttingBuilder()->EngageOpenAreaBuilder()->LinearExtensionBuilder()->SetValue(ArcCavityParameterSet.ToolMotion.LinearExtension);
		switch(ArcCavityParameterSet.ToolMotion.LinearExtensionUnit)
		{
		case 0:
			surfaceContourBuilder1->NonCuttingBuilder()->EngageOpenAreaBuilder()->LinearExtensionBuilder()->SetIntent(CAM::ParamValueIntentToolDep);
			break;
		case 1:
			surfaceContourBuilder1->NonCuttingBuilder()->EngageOpenAreaBuilder()->LinearExtensionBuilder()->SetIntent(CAM::ParamValueIntentPartUnits);    //HeightBuilder
			break;
		}
		//圆弧角度
		surfaceContourBuilder1->NonCuttingBuilder()->EngageOpenAreaBuilder()->SetArcAngle(ArcCavityParameterSet.ToolMotion.CircleAngle);
		//倾斜角度
		surfaceContourBuilder1->NonCuttingBuilder()->EngageOpenAreaBuilder()->SetRampAngle(ArcCavityParameterSet.ToolMotion.TiltAngle);
	}
	else if (ArcCavityParameterSet.ToolMotion.ToolMotionType == 2)
	{
		surfaceContourBuilder1->NonCuttingBuilder()->EngageOpenAreaBuilder()->SetEngRetType(CAM::NcmScEngRetBuilder::EngRetTypesArcNormalToPart);
		//圆弧半径
		surfaceContourBuilder1->NonCuttingBuilder()->EngageOpenAreaBuilder()->RadiusBuilder()->SetValue(ArcCavityParameterSet.ToolMotion.CircleRadius);
		switch(ArcCavityParameterSet.ToolMotion.CircleRadiusUnit)
		{
		case 0:
			surfaceContourBuilder1->NonCuttingBuilder()->EngageOpenAreaBuilder()->RadiusBuilder()->SetIntent(CAM::ParamValueIntentToolDep);
			break;
		case 1:
			surfaceContourBuilder1->NonCuttingBuilder()->EngageOpenAreaBuilder()->RadiusBuilder()->SetIntent(CAM::ParamValueIntentPartUnits);
			break;
		}
		//线性延伸
		surfaceContourBuilder1->NonCuttingBuilder()->EngageOpenAreaBuilder()->LinearExtensionBuilder()->SetValue(ArcCavityParameterSet.ToolMotion.LinearExtension);
		switch(ArcCavityParameterSet.ToolMotion.LinearExtensionUnit)
		{
		case 0:
			surfaceContourBuilder1->NonCuttingBuilder()->EngageOpenAreaBuilder()->LinearExtensionBuilder()->SetIntent(CAM::ParamValueIntentToolDep);
			break;
		case 1:
			surfaceContourBuilder1->NonCuttingBuilder()->EngageOpenAreaBuilder()->LinearExtensionBuilder()->SetIntent(CAM::ParamValueIntentPartUnits);    //HeightBuilder
			break;
		}
		//圆弧角度
		surfaceContourBuilder1->NonCuttingBuilder()->EngageOpenAreaBuilder()->SetArcAngle(ArcCavityParameterSet.ToolMotion.CircleAngle);
	}
	else if (ArcCavityParameterSet.ToolMotion.ToolMotionType == 3)
	{
		surfaceContourBuilder1->NonCuttingBuilder()->EngageOpenAreaBuilder()->SetEngRetType(CAM::NcmScEngRetBuilder::EngRetTypesLinearNormalToPart);

		//距离
		surfaceContourBuilder1->NonCuttingBuilder()->EngageOpenAreaBuilder()->LengthBuilder()->SetValue(ArcCavityParameterSet.ToolMotion.ToolMotionDistance);
		switch(ArcCavityParameterSet.ToolMotion.ToolMotionDistanceUnit)
		{
		case 0:
			surfaceContourBuilder1->NonCuttingBuilder()->EngageOpenAreaBuilder()->LengthBuilder()->SetIntent(CAM::ParamValueIntentToolDep);
			break;
		case 1:
			surfaceContourBuilder1->NonCuttingBuilder()->EngageOpenAreaBuilder()->LengthBuilder()->SetIntent(CAM::ParamValueIntentPartUnits);
			break;
		}
		//旋转角度
		surfaceContourBuilder1->NonCuttingBuilder()->EngageOpenAreaBuilder()->SetSwingAngle(ArcCavityParameterSet.ToolMotion.RotationAngle);
		//倾斜角度
		surfaceContourBuilder1->NonCuttingBuilder()->EngageOpenAreaBuilder()->SetRampAngle(ArcCavityParameterSet.ToolMotion.TiltAngle);
	}

	//公共安全设置
	if (ArcCavityParameterSet.SafeSetType == 0)
	{
		surfaceContourBuilder1->NonCuttingBuilder()->TransferCommonClearanceBuilder()->SetClearanceType(CAM::NcmClearanceBuilder::ClearanceTypesCylinder);
		surfaceContourBuilder1->NonCuttingBuilder()->TransferCommonClearanceBuilder()->SetRadius(400.0+ArcCavityParameterSet.SafeSetOffset);
		NXOpen::Point3d iCenterPoint(0.0, 0.0, 0.0);
		NXOpen::Point *point3;
		point3 = workPart->Points()->CreatePoint(iCenterPoint);

		NXOpen::NXObject * TPointObject = (NXOpen::NXObject *) point3;
		surfaceContourBuilder1->NonCuttingBuilder()->TransferCommonClearanceBuilder()->SetPoint(TPointObject);
		//NXOpen::Vector3d normal01(0,0,1);
		Direction *direction1 = workPart->Directions()->CreateDirection(point3,_Normal);
		surfaceContourBuilder1->NonCuttingBuilder()->TransferCommonClearanceBuilder()->SetAxis(direction1);	}
	else if (ArcCavityParameterSet.SafeSetType == 1)
	{
		surfaceContourBuilder1->NonCuttingBuilder()->TransferCommonClearanceBuilder()->SetClearanceType(CAM::NcmClearanceBuilder::ClearanceTypesAutomatic);
	}
	//设置安全偏置距离
	surfaceContourBuilder1->NonCuttingBuilder()->TransferCommonClearanceBuilder()->SetSafeDistance(ArcCavityParameterSet.SafeSetOffset);
	surfaceContourBuilder1->CutParameters()->BlankOverhang()->SetIntent(CAM::ParamValueIntentToolDep);
	surfaceContourBuilder1->CutParameters()->BlankOverhang()->SetValue(20.0);

	//-------------------------------------加工参数赋值

	//更新操作
	NXObject *nXObject;
	nXObject = surfaceContourBuilder1->Commit();
	surfaceContourBuilder1->Destroy();

	//生成刀轨
	std::vector<CAM::CAMObject *> objects1(1);
	objects1[0] = surfaceContour1;
	workPart->CAMSetup()->GenerateToolPath(objects1);
}