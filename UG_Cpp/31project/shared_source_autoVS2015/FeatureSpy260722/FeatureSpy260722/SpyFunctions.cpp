#include "SpyFunctions.h"
#include <NXOpen/TaggedObject.hxx>
#include <NXOpen/Edge.hxx>
#include <NXOpen/Face.hxx>
#include <vector>
#include <NXOpen/NXObjectManager.hxx>


using namespace NXOpen;
//--------------------------20221220新增 凸台特征识别fxy--------------------------------------------------

void DistinguishBossOutOrIn(std::vector<NormalBoss*>&Feature_NormalBoss, std::vector<NoSideBoss*>&Feature_NoSideBoss, std::vector<DoubleLayerBoss*>&Feature_DoubleLayerBoss, double _Approach_Point[3])
{
	for (std::vector<NormalBoss*>::size_type i = 0; i != Feature_NormalBoss.size(); i++)
	{
		Face *EachFace;
		EachFace = Feature_NormalBoss[i]->TopFaceVector[0];

		int type = 0;       //判断面类型
		int norm_dir = 0;   //1=外R，-1=内R
		double point[3];  //轴中心 点
		double dir[3];    //轴方向
		double box[6];    //极点坐标
		double radius;    //半径 圆锥=最小半径 + 最大半径 /2
		double rad_data;
		UF_MODL_ask_face_data(EachFace->Tag(), &type, point, dir, box, &radius, &rad_data, &norm_dir);

		Vector3d EachFaceVector1;
		Vector3d PointToPointVector2;
		EachFaceVector1.X = dir[0]; EachFaceVector1.Y = dir[1]; EachFaceVector1.Z = dir[2];
		PointToPointVector2.X = point[0] - _Approach_Point[0]; PointToPointVector2.Y = point[1] - _Approach_Point[1]; PointToPointVector2.Z = point[2] - _Approach_Point[2];
		double Angle0 = get_Angle_Two_vector3d(EachFaceVector1, PointToPointVector2);

		if (Angle0<(PI / 2))
		{
			Feature_NormalBoss[i]->OutOrIn = 1;
		}
		else
		{
			Feature_NormalBoss[i]->OutOrIn = 0;
		}

	}

	for (std::vector<NoSideBoss*>::size_type i = 0; i != Feature_NoSideBoss.size(); i++)
	{
		Face *EachFace;
		EachFace = Feature_NoSideBoss[i]->TopFaceVector[0];

		int type = 0;       //判断面类型
		int norm_dir = 0;   //1=外R，-1=内R
		double point[3];  //轴中心 点
		double dir[3];    //轴方向
		double box[6];    //极点坐标
		double radius;    //半径 圆锥=最小半径 + 最大半径 /2
		double rad_data;
		UF_MODL_ask_face_data(EachFace->Tag(), &type, point, dir, box, &radius, &rad_data, &norm_dir);

		Vector3d EachFaceVector1;
		Vector3d PointToPointVector2;
		EachFaceVector1.X = dir[0]; EachFaceVector1.Y = dir[1]; EachFaceVector1.Z = dir[2];
		PointToPointVector2.X = point[0] - _Approach_Point[0]; PointToPointVector2.Y = point[1] - _Approach_Point[1]; PointToPointVector2.Z = point[2] - _Approach_Point[2];
		double Angle0 = get_Angle_Two_vector3d(EachFaceVector1, PointToPointVector2);

		if (Angle0<(PI / 2))
		{
			Feature_NoSideBoss[i]->OutOrIn = 1;
		}
		else
		{
			Feature_NoSideBoss[i]->OutOrIn = 0;
		}

	}

	for (std::vector<DoubleLayerBoss*>::size_type i = 0; i != Feature_DoubleLayerBoss.size(); i++)
	{
		Face *EachFace;
		EachFace = Feature_DoubleLayerBoss[i]->TopFaceVector[0];

		int type = 0;       //判断面类型
		int norm_dir = 0;   //1=外R，-1=内R
		double point[3];  //轴中心 点
		double dir[3];    //轴方向
		double box[6];    //极点坐标
		double radius;    //半径 圆锥=最小半径 + 最大半径 /2
		double rad_data;
		UF_MODL_ask_face_data(EachFace->Tag(), &type, point, dir, box, &radius, &rad_data, &norm_dir);

		Vector3d EachFaceVector1;
		Vector3d PointToPointVector2;
		EachFaceVector1.X = dir[0]; EachFaceVector1.Y = dir[1]; EachFaceVector1.Z = dir[2];
		PointToPointVector2.X = point[0] - _Approach_Point[0]; PointToPointVector2.Y = point[1] - _Approach_Point[1]; PointToPointVector2.Z = point[2] - _Approach_Point[2];
		double Angle0 = get_Angle_Two_vector3d(EachFaceVector1, PointToPointVector2);

		if (Angle0<(PI / 2))
		{
			Feature_DoubleLayerBoss[i]->OutOrIn = 1;
		}
		else
		{
			Feature_DoubleLayerBoss[i]->OutOrIn = 0;
		}
		
	}


}

void BossRecognitionAndCreate(std::vector<Face*>AllFaces, std::vector<Face*>SealFace_NormalBoss, std::vector<NormalBoss*>&Feature_NormalBoss,
	std::vector<Face*>SealFace_NoSideBoss, std::vector<NoSideBoss*>&Feature_NoSideBoss,
	std::vector<Face*>SealFace_DoubleLayerBoss, std::vector<DoubleLayerBoss*>&Feature_DoubleLayerBoss, NXOpen::Vector3d _Approach_Normal, NXOpen::Part* workPart)
{
	std::vector<Face*>PlanerFaces;
	/*std::vector<Face*>tibuFaces;*/
	/*tibuFaces.clear();*/

	for (std::vector<Face*>::size_type i = 0; i != AllFaces.size(); i++)//存入所有平面
	{
		Face *EachFace;
		EachFace = AllFaces[i];
		if (EachFace->SolidFaceType() == Face::FaceTypePlanar)
		{
			std::vector<Edge*>Edges;
			/*UF_UI_write_listing_window("测试0:\n");*/
			Edges.clear();
			Edges = EachFace->GetEdges();

			/*char message[13];
			UF_UI_open_listing_window();
			sprintf( message, "%d\n",Edges.size());
			UF_UI_write_listing_window("边界个数为:\n");
			UF_UI_write_listing_window(message);
			UF_UI_write_listing_window("测试1:\n");*/

			if (Edges.size()>50)
			{
				continue;
			}
			Vector3d PlanerVector;
			int Type;
			double Point[3];
			double Dir[3];
			double Box[6];
			double Radius[3];
			double RadData[3];
			int NormDir;
			UF_MODL_ask_face_data(EachFace->Tag(), &Type, Point, Dir, Box, Radius, RadData, &NormDir);//获取法向向量
			PlanerVector.X = Dir[0];
			PlanerVector.Y = Dir[1];
			PlanerVector.Z = Dir[2];
			double EachAngle;
			EachAngle = get_Angle_Two_vector3d(PlanerVector, _Approach_Normal);

			if (EachAngle<0.01 || EachAngle>3.13)
			{
				continue;
			}
			//------
			/*UF_OBJ_set_color(EachFace->Tag(),111);*/
			//------
			PlanerFaces.push_back(EachFace);
		}


	}//平面存储完毕
	

	/*Face* surfacemax = PlanerFaces[0];
	Face* surfacesecondmax = PlanerFaces[0];*/
	/*NXOpen::Part* workPart = NXOpen::Session::GetSession()->Parts()->Work();*/

	/*uc1601("这里没问题3", 1);*/

	/*for (std::vector<Face*>::size_type i = 0; i != PlanerFaces.size(); i++)
	{

	if (calcArea(workPart, PlanerFaces[i]) - calcArea(workPart, surfacemax) >= 0.01)
	{

	surfacesecondmax = surfacemax;
	surfacemax = PlanerFaces[i];
	}

	else if (calcArea(workPart, PlanerFaces[i]) - calcArea(workPart, surfacesecondmax) >= 0.01)
	{
	surfacesecondmax = PlanerFaces[i];
	}
	}
	uc1601("这里没问题3", 1);*/

	for (std::vector<Face*>::size_type i = 0; i != PlanerFaces.size(); i++)
	{
		Face *EachFace = PlanerFaces[i];
		std::vector<Face*>SideFaceVector;
		get_External_Adjacent_Faces(EachFace, SideFaceVector);




		bool RightFlag0 = false;
		int PlanerNum = 0;
		int BlendNum = 0;
		int CylinderNum = 0;
		int RevolutionNum = 0;
		int ConnectNum = 0;

		Face *ConnectFace;
		for (std::vector<Face*>::size_type j = 0; j != SideFaceVector.size(); j++)//顶面的邻面
		{
			
			Face *OtherFace = SideFaceVector[j];

			bool IsSmooth = IsTwoFaceSmooth(EachFace, OtherFace);
			//if (IsSmooth == true && (calcArea(workPart, surfacesecondmax) - calcArea(workPart, EachFace) >= 0.01))//有光滑的侧面就不行

			try
			{
				if (IsSmooth == true && abs(calcArea(workPart, EachFace) - 1644.37335) > 0.01)
				{

					/*tibuFaces.push_back(EachFace);*/
					RightFlag0 = 1;
					break;

				}
			}
			catch (exception& ex)
			{
				continue;
			}
			
			
			/*UF_OBJ_set_color(OtherFace->Tag(), 50);*/

			/*uc1601("这里没问题4", 1);*/
			if (OtherFace->SolidFaceType() == Face::FaceTypePlanar)//该邻面是平面（含侧面凸台）
			{
				PlanerNum++;

				/*UF_OBJ_set_color(OtherFace->Tag(), 99);*/

				std::vector<Face*>SideAdFaces;
				get_External_Adjacent_Faces(OtherFace, SideAdFaces);//邻面的邻面

				for (std::vector<Face*>::size_type t = 0; t != SideAdFaces.size(); t++)
				{

					if (SideAdFaces[t]->SolidFaceType() == Face::FaceTypeCylindrical)//二级邻面为柱面（顶角面）
					{
						int Begin_type = 0;       //判断面类型
						int Begin_norm_dir = 0;   //1=外R，-1=内R
						double Begin_point[3];  //轴中心 点
						double Begin_dir[3];    //轴方向
						double Begin_box[6];    //极点坐标
						double Begin_radius;    //半径 圆锥=最小半径 + 最大半径 /2
						double rad_data;
						UF_MODL_ask_face_data(SideAdFaces[t]->Tag(), &Begin_type, Begin_point, Begin_dir, Begin_box, &Begin_radius, &rad_data, &Begin_norm_dir);

						if (Begin_radius<1.5)
							//if(Begin_radius<2.5&&Is_Face_in_lpFace(SideAdFaces[t],SideFaceVector)==-1)
						{
							ConnectFace = SideAdFaces[t];
							ConnectNum++;
						}
					}
				}
			}
			else if (OtherFace->SolidFaceType() == Face::FaceTypeCylindrical)//该邻面是柱面
			{

				CylinderNum++;
			}
			else if (OtherFace->SolidFaceType() == Face::FaceTypeBlending)//该邻面是倒圆面
			{
				//------
				/*UF_OBJ_set_color(OtherFace->Tag(),79);*/
				//------
				BlendNum++;
			}
			else if (OtherFace->SolidFaceType() == Face::FaceTypeSurfaceOfRevolution)
			{
				//------
				/*UF_OBJ_set_color(OtherFace->Tag(),59);*/
				//------
				RevolutionNum++;
			}
			
		}



		if (RightFlag0)
		{

			continue;
		}


		/*uc1601("这里没问题5", 1);*/

		if (BlendNum == SideFaceVector.size())//第一次相邻面全部都是倒角面，无侧面凸台
		{
			
			NoSideBoss *Feature = new NoSideBoss;
			Feature->TopFaceVector.push_back(EachFace);
			PutFaceVectorIntoFaceVector(SideFaceVector, Feature->BottomCornerFaceVector);
			Feature_NoSideBoss.push_back(Feature);
			
		}

		else if (ConnectNum == 1)//多层凸台
		{

			
			DoubleLayerBoss *Feature = new DoubleLayerBoss;
			Feature->TopFaceVector.push_back(EachFace);
			Feature->ConnectFaceVector.push_back(ConnectFace);

			/*UF_OBJ_set_color(EachFace->Tag(), 79);*/

			bool DoubleRight = true;
			for (std::vector<Face*>::size_type j = 0; j != SideFaceVector.size(); j++)
			{
				Face *OtherFace = SideFaceVector[j];

				if (OtherFace->SolidFaceType() == Face::FaceTypeSurfaceOfRevolution)
				{
					DoubleRight = false;
					break;
				}
				else if (OtherFace->SolidFaceType() == Face::FaceTypeCylindrical || OtherFace->SolidFaceType() == Face::FaceTypePlanar)
				{
					if (OtherFace == ConnectFace)continue;
					Feature->SideFaceVector.push_back(OtherFace);
				}
				else if (OtherFace->SolidFaceType() == Face::FaceTypeBlending)
				{
					Feature->BottomCornerFaceVector.push_back(OtherFace);
				}
			}
			//这里从侧面开始搜索主凸台底角面
			for (std::vector<Face*>::size_type j = 0; j != Feature->SideFaceVector.size(); j++)
			{
				Face *EachSideFace = Feature->SideFaceVector[j];
				std::vector<Face*>CornerFaceVector;
				get_Adjacent_Faces(EachSideFace, CornerFaceVector);

				for (std::vector<Face*>::size_type t = 0; t != CornerFaceVector.size(); t++)
				{
					Face *EachCornerFace = CornerFaceVector[t];
					if (EachCornerFace->SolidFaceType() == Face::FaceTypeBlending || EachCornerFace->SolidFaceType() == Face::FaceTypeSurfaceOfRevolution)
					{
						if (Is_Face_in_lpFace(EachCornerFace, Feature->SideFaceVector) == -1 && Is_Face_in_lpFace(EachCornerFace, Feature->BottomCornerFaceVector) == -1
							&& EachCornerFace != EachFace)
						{
							Feature->BottomCornerFaceVector.push_back(EachCornerFace);
						}
					}
				}
			}
			//这里找次级凸台的顶面
			std::vector<Face*>ConnectAdFaceVector;
			get_Adjacent_Faces(ConnectFace, ConnectAdFaceVector);
			for (std::vector<Face*>::size_type j = 0; j != ConnectAdFaceVector.size(); j++)
			{
				Face *EachConnectAdFace = ConnectAdFaceVector[j];
				if (EachConnectAdFace != EachFace&&Is_Face_in_lpFace(EachConnectAdFace, Feature->SideFaceVector) == -1 && EachConnectAdFace->SolidFaceType() == Face::FaceTypePlanar)
				{
					Feature->NTopFaceVector.push_back(EachConnectAdFace);
				}
			}

			if (Feature->NTopFaceVector.size() != 1) continue;

			std::vector<Face*>NTopAdFaceVector;
			get_External_Adjacent_Faces(Feature->NTopFaceVector[0], NTopAdFaceVector);

			for (std::vector<Face*>::size_type j = 0; j != NTopAdFaceVector.size(); j++)
			{
				Face *OtherFace = NTopAdFaceVector[j];

				if (OtherFace == ConnectFace) continue;

				if (OtherFace->SolidFaceType() == Face::FaceTypeCylindrical || OtherFace->SolidFaceType() == Face::FaceTypePlanar || OtherFace->SolidFaceType() == Face::FaceTypeConical)
				{
					Feature->NSideFaceVector.push_back(OtherFace);
				}
				else if (OtherFace->SolidFaceType() == Face::FaceTypeBlending)
				{
					if (Is_Face_in_lpFace(OtherFace, Feature->BottomCornerFaceVector) != -1) continue;
					Feature->BottomCornerFaceVector.push_back(OtherFace);
				}
			}

			for (std::vector<Face*>::size_type j = 0; j != Feature->NSideFaceVector.size(); j++)
			{
				Face *EachNSideFace = Feature->NSideFaceVector[j];
				std::vector<Face*>CornerFaceVector;
				get_Adjacent_Faces(EachNSideFace, CornerFaceVector);

				for (std::vector<Face*>::size_type t = 0; t != CornerFaceVector.size(); t++)
				{
					Face *EachCornerFace = CornerFaceVector[t];
					if (EachCornerFace->SolidFaceType() == Face::FaceTypeBlending || EachCornerFace->SolidFaceType() == Face::FaceTypeSurfaceOfRevolution || EachCornerFace->SolidFaceType() == Face::FaceTypeSpherical)
					{
						if (Is_Face_in_lpFace(EachCornerFace, Feature->SideFaceVector) == -1 && Is_Face_in_lpFace(EachCornerFace, Feature->BottomCornerFaceVector) == -1
							&& EachCornerFace != EachFace&&EachCornerFace != Feature->NTopFaceVector[0] && EachCornerFace != ConnectFace&&
							Is_Face_in_lpFace(EachCornerFace, Feature->NSideFaceVector) == -1)
						{
							Feature->BottomCornerFaceVector.push_back(EachCornerFace);
						}
					}
				}
			}
			Feature_DoubleLayerBoss.push_back(Feature);//多层凸台识别完毕
			
		}
		

		else if (ConnectNum == 0)
		{
			
			//------
			/*UF_OBJ_set_color(EachFace->Tag(), 109);*/
			//------

			bool NorRight = true;
			//UF_OBJ_set_color(EachFace->Tag(),36);
			NormalBoss *Feature = new NormalBoss;
			Feature->TopFaceVector.push_back(EachFace);
			int SidefaceNum = 0;
			for (std::vector<Face*>::size_type j = 0; j != SideFaceVector.size(); j++)//顶面的邻面
			{
				Face *OtherFace = SideFaceVector[j];
				//------
				/*UF_OBJ_set_color(OtherFace->Tag(),99);*/
				//------

				try
				{
					if (OtherFace->SolidFaceType() == Face::FaceTypeSurfaceOfRevolution && abs(calcArea(workPart, EachFace) - 1644.37335) > 0.01)
					{

						/*UF_OBJ_set_color(OtherFace->Tag(), 40);*/
						NorRight = false;
						break;
					}
					/*else if(OtherFace->SolidFaceType()==Face::FaceTypeCylindrical)*/
					else if (OtherFace->SolidFaceType() == Face::FaceTypeCylindrical || OtherFace->SolidFaceType() == Face::FaceTypePlanar || OtherFace->SolidFaceType() == Face::FaceTypeConical)
					{
						//------
						/*UF_OBJ_set_color(OtherFace->Tag(),99);*/
						//------
						SidefaceNum++;
						Feature->SideFaceVector.push_back(OtherFace);
					}
					else if (OtherFace->SolidFaceType() == Face::FaceTypeBlending)//该方法使得第二层可能就出现底角面
					{
						Feature->BottomCornerFaceVector.push_back(OtherFace);
					}
				}
				catch (exception& ex)
				{
					//---- Enter your exception handling code here -----
					continue;
				}
				
			}

			if (NorRight == false)
			{
				continue;
			}

			

			//这里从侧面开始搜索底角面
			for (std::vector<Face*>::size_type j = 0; j != Feature->SideFaceVector.size(); j++)
			{
				Face *EachSideFace = Feature->SideFaceVector[j];
				std::vector<Face*>CornerFaceVector;
				get_Adjacent_Faces(EachSideFace, CornerFaceVector);//得到该片侧面的所有邻面

				for (std::vector<Face*>::size_type t = 0; t != CornerFaceVector.size(); t++)
				{
					Face *EachCornerFace = CornerFaceVector[t];
					if (EachCornerFace->SolidFaceType() == Face::FaceTypeBlending || EachCornerFace->SolidFaceType() == Face::FaceTypeSurfaceOfRevolution)
					{
						if (Is_Face_in_lpFace(EachCornerFace, Feature->SideFaceVector) == -1 && Is_Face_in_lpFace(EachCornerFace, Feature->BottomCornerFaceVector) == -1
							&& EachCornerFace != EachFace)//该邻面不在侧面列表中，也不能与已识别的底角面重复，更不能在顶面列表
						{
							//------
							/*UF_OBJ_set_color(EachCornerFace->Tag(),79);*/
							//------
							Feature->BottomCornerFaceVector.push_back(EachCornerFace);
						}
					}
				}

			}
			
			if (Feature->BottomCornerFaceVector.size() != 0 || Feature->SideFaceVector.size() == SidefaceNum)
			{
				////为含侧面凸台寻找底面
				//GetBossBottomFace(Feature->TopFaceVector, Feature->SideFaceVector, Feature->BottomCornerFaceVector, Feature->BottomFaceVector);

				Feature_NormalBoss.push_back(Feature);//含侧面凸台识别完毕
			}

			else
			{
				continue;
			}
			

		}


		//=============区分凸台内外======================
		double DirVector[3];
		DirVector[0] = _Approach_Normal.X; DirVector[1] = _Approach_Normal.Y; DirVector[2] = _Approach_Normal.Z;
		DistinguishBossOutOrIn(Feature_NormalBoss, Feature_NoSideBoss, Feature_DoubleLayerBoss, DirVector);


	}
	//for (std::vector<Face*>::size_type i = 0; i != tibuFaces.size(); i++)
	//{
	//	
	//	for (std::vector<DoubleLayerBoss*>::size_type t = 0; t != Feature_DoubleLayerBoss.size(); t++)
	//	{
	//		
	//		if (Is_Face_in_lpFace(tibuFaces[i], Feature_DoubleLayerBoss[t]->NTopFaceVector) == -1)
	//		{
	//			
	//			if (abs(calcArea(workPart, tibuFaces[i]) - 1645.3) < 0.01)
	//			{
	//				
	//				NormalBoss *Feature = new NormalBoss;
	//				Face *EachFace = tibuFaces[i];

	//				UF_OBJ_set_color(EachFace->Tag(), 110);

	//				std::vector<Face*>SideFaceVector;
	//				int SidefaceNum = 0;
	//				get_External_Adjacent_Faces(EachFace, SideFaceVector);
	//				for (std::vector<Face*>::size_type j = 0; j != SideFaceVector.size(); j++)//顶面的邻面
	//				{

	//					
	//					Face *OtherFace = SideFaceVector[j];
	//					//------
	//					/*UF_OBJ_set_color(OtherFace->Tag(),99);*/
	//					//------					
	//					/*else if(OtherFace->SolidFaceType()==Face::FaceTypeCylindrical)*/
	//					if (OtherFace->SolidFaceType() == Face::FaceTypeCylindrical || OtherFace->SolidFaceType() == Face::FaceTypePlanar || OtherFace->SolidFaceType() == Face::FaceTypeConical)
	//					{
	//						//------
	//						/*UF_OBJ_set_color(OtherFace->Tag(),99);*/
	//						//------
	//						
	//						Feature->SideFaceVector.push_back(OtherFace);
	//						SidefaceNum++;
	//					}
	//					else if (OtherFace->SolidFaceType() == Face::FaceTypeBlending)//该方法使得第二层可能就出现底角面
	//					{
	//						Feature->BottomCornerFaceVector.push_back(OtherFace);
	//					}
	//				}

	//				//for (std::vector<Face*>::size_type j = 0; j != Feature->SideFaceVector.size(); j++)
	//				//{
	//				//	
	//				//	Face *EachSideFace = Feature->SideFaceVector[j];
	//				//	std::vector<Face*>CornerFaceVector;
	//				//	get_Adjacent_Faces(EachSideFace, CornerFaceVector);//得到该片侧面的所有邻面

	//				//	for (std::vector<Face*>::size_type t = 0; t != CornerFaceVector.size(); t++)
	//				//	{
	//				//		Face *EachCornerFace = CornerFaceVector[t];
	//				//		if (EachCornerFace->SolidFaceType() == Face::FaceTypeBlending || EachCornerFace->SolidFaceType() == Face::FaceTypeSurfaceOfRevolution)
	//				//		{
	//				//			if (Is_Face_in_lpFace(EachCornerFace, Feature->SideFaceVector) == -1 && Is_Face_in_lpFace(EachCornerFace, Feature->BottomCornerFaceVector) == -1
	//				//				&& EachCornerFace != EachFace)//该邻面不在侧面列表中，也不能与已识别的底角面重复，更不能在顶面列表
	//				//			{
	//				//				//------
	//				//				/*UF_OBJ_set_color(EachCornerFace->Tag(),79);*/
	//				//				//------
	//				//				Feature->BottomCornerFaceVector.push_back(EachCornerFace);
	//				//			}
	//				//		}
	//				//	}

	//				//}
	//				//if (Feature->BottomCornerFaceVector.size() != 0 || Feature->SideFaceVector.size() == SidefaceNum)
	//				//{
	//				//	////为含侧面凸台寻找底面
	//				//	//GetBossBottomFace(Feature->TopFaceVector, Feature->SideFaceVector, Feature->BottomCornerFaceVector, Feature->BottomFaceVector);

	//				//	Feature_NormalBoss.push_back(Feature);//含侧面凸台识别完毕
	//				//}

	//			}
	//		}
	//	}

	//	

	//	double DirVector[3];
	//	DirVector[0] = _Approach_Normal.X; DirVector[1] = _Approach_Normal.Y; DirVector[2] = _Approach_Normal.Z;
	//	DistinguishBossOutOrIn(Feature_NormalBoss, Feature_NoSideBoss, Feature_DoubleLayerBoss, DirVector);
	//}

}

//得到凸台底面
//-------------------------基本思想是：与底角面接触的都是底面，以及前面直接接触侧面的非底角面。   //需改：有的凸台底角面会接触到非底面
void GetBossBottomFace(std::vector<Face*> TopFaceVector, std::vector<Face*> SideFaceVector, std::vector<Face*> BottomCornerFaceVector, std::vector<Face*>& BottomFaceVector)
{
	for (std::vector<Face*>::size_type i = 0; i != BottomCornerFaceVector.size(); i++)
	{
		Face* EachBottomCornerFace = BottomCornerFaceVector[i];

		std::vector<Face*> AdjacentFaces;
		get_Adjacent_Faces(EachBottomCornerFace, AdjacentFaces);

		for (std::vector<Face*>::size_type j = 0; j != AdjacentFaces.size(); j++)
		{
			Face* EachAdjacentFace = AdjacentFaces[j];
			if (Is_Face_in_lpFace(EachAdjacentFace, TopFaceVector) == -1 &&
				Is_Face_in_lpFace(EachAdjacentFace, SideFaceVector) == -1 &&
				Is_Face_in_lpFace(EachAdjacentFace, BottomCornerFaceVector) == -1 &&
				Is_Face_in_lpFace(EachAdjacentFace, BottomFaceVector) == -1)
			{
				BottomFaceVector.push_back(EachAdjacentFace);
			}
		}
	}
}


//轴向孔种子面识别
void DirectionMillRecognition(std::vector<Face*>AllFaces, Face *BeginFace, Face* BottomPlaneFace, Face* TopPlaneFace,
	std::vector<Face*> ParallelPlaneFace, std::vector<Face*>&LeftSealHole_FaceVector
	, std::vector<Face*>&SealFace_DirectionNormalHole, std::vector<Face*>&SealFace_DirectionRightHole
	, std::vector<Face*>&SealFace_DirectionBlindHole)

{
	//===============================DEFINE PROGRESS Begin===================================================================
	//char *library_name = "D:\\ug10\\UGII\\libdisp.dll";
	//typedef void(*create_progress_bar)(int v1, bool v2, long *v3);//定义传参
	//create_progress_bar MT_create_progress_bar = NULL;
	//UF_load_library(library_name, "?MT_create_progress_bar@@YA_NH_NPEAJ@Z", (UF_load_f_p_t *)&MT_create_progress_bar);
	//typedef void(*set_progress_pos)(long *v1, int v2);//定义传参
	//set_progress_pos MT_set_progress_pos = NULL;
	//UF_load_library(library_name, "?MT_set_progress_pos@@YAXPEAJH@Z", (UF_load_f_p_t *)&MT_set_progress_pos);
	//typedef void(*show_progress_bar)(long *v1);//定义传参
	//show_progress_bar MT_show_progress_bar = NULL;
	//UF_load_library(library_name, "?MT_show_progress_bar@@YAXPEAJ@Z", (UF_load_f_p_t *)&MT_show_progress_bar);
	//typedef void(*set_progress_range)(long *v1, int v2, int v3);//定义传参
	//set_progress_range MT_set_progress_range = NULL;
	//UF_load_library(library_name, "?MT_set_progress_range@@YAXPEAJHH@Z", (UF_load_f_p_t *)&MT_set_progress_range);
	//typedef __int64(*ask_progress_cue_context)(void);//定义传参
	//ask_progress_cue_context MT_ask_progress_cue_context = NULL;
	//UF_load_library(library_name, "?MT_ask_progress_cue_context@@YAHXZ", (UF_load_f_p_t *)&MT_ask_progress_cue_context);
	//typedef __int64(*delete_progress_bar)(long *v1);//定义传参
	//delete_progress_bar MT_delete_progress_bar = NULL;
	//UF_load_library(library_name, "?MT_delete_progress_bar@@YAXPEAJ@Z", (UF_load_f_p_t *)&MT_delete_progress_bar);
	//typedef __int64(*set_progress_text)(long *v1, char *v2, bool v3);//定义传参
	//set_progress_text MT_set_progress_text = NULL;
	//UF_load_library(library_name, "?MT_set_progress_text@@YAXPEAJPEAD_N@Z", (UF_load_f_p_t *)&MT_set_progress_text);
	//if (MT_ask_progress_cue_context == NULL || MT_create_progress_bar == NULL || MT_show_progress_bar == NULL
	//	|| MT_set_progress_text == NULL || MT_set_progress_range == NULL || MT_set_progress_pos == NULL
	//	|| MT_delete_progress_bar == NULL)
	//{
	//	uc1601("进度条报错提示", 1);
	//	return;
	//}

	//__int64 cc = MT_ask_progress_cue_context();
	//long aa = 0;
	//MT_create_progress_bar(cc, true, &aa);//true和false控制是否显示百分比数字
	//MT_show_progress_bar(&aa);
	//MT_set_progress_text(&aa, "Loading", true);//此处用中文好像不显示

	//int num0=AllFaces.size()-1;
	//MT_set_progress_range(&aa, 0, num0);
	//int NowNum=0;
	//===============================DEFINE PROGRESS END===================================================================
	//第一步筛选出合适的种子面
	std::vector<NXOpen::Face*> seal_FaceVector;
	std::vector<NXOpen::Face*> sealdirection_FaceVector;
	//int Begin_type=0;       //判断面类型
	//int Begin_norm_dir=0;   //1=外R，-1=内R
	//double Begin_point[3];  //轴中心 点
	//double Begin_dir[3];    //轴方向
	//double Begin_box[6];    //极点坐标
	//double Begin_radius;    //半径 圆锥=最小半径 + 最大半径 /2
	//double Begin_rad_data;
	//UF_MODL_ask_face_data (BeginFace->Tag(),&Begin_type,Begin_point,Begin_dir,Begin_box,&Begin_radius,&Begin_rad_data,&Begin_norm_dir);

	Vector3d Normal_Vector;
	//Normal_Vector.X=Begin_dir[0];
	//Normal_Vector.Y=Begin_dir[1];
	//Normal_Vector.Z=Begin_dir[2];

	

	get_MainNormal_fr_plane(BeginFace, Normal_Vector);
	for (std::vector<Face*>::size_type i = 0; i != AllFaces.size(); i++)
	{
		/*MT_set_progress_pos(&aa, NowNum++);*/
		/*UF_UI_set_status("轴向孔特征识别");*/
		Face *EachFace = AllFaces[i];

		if (EachFace->SolidFaceType() == Face::FaceTypeCylindrical)
		{
			int type = 0;       //判断面类型
			int norm_dir = 0;   //1=外R，-1=内R
			double point[3];  //轴中心 点
			double dir[3];    //轴方向
			double box[6];    //极点坐标
			double radius;    //半径 圆锥=最小半径 + 最大半径 /2
			double rad_data;

			UF_MODL_ask_face_data(EachFace->Tag(), &type, point, dir, box, &radius, &rad_data, &norm_dir);
			std::vector<Edge*>EachFace_EdgeVector;
			EachFace_EdgeVector = EachFace->GetEdges();

			if (radius<35 && norm_dir == -1 && EachFace_EdgeVector.size()<20) //筛选出了边少于4的种子面
			{
				seal_FaceVector.push_back(EachFace);
				//UF_DISP_set_highlight(EachFace->Tag(),1);//TEST OK
			}
			else
			{
				continue;
			}
			Vector3d Each_Vector(dir[0], dir[1], dir[2]);
			//Vector3d Each_Vector;			
			//Each_Vector.X=dir[0];Each_Vector.Y=dir[1];Each_Vector.Z=dir[2];

			double Angle = get_Angle_Two_vector3d(Normal_Vector, Each_Vector);
			Angle = Angle / PI * 180;
			if (Angle<1 || Angle>179)//选择正确主轴方向的孔
			{
				sealdirection_FaceVector.push_back(EachFace);
				//UF_DISP_set_highlight(EachFace->Tag(),1);//TEST OK 在其中一个零件中出现位置问题
			}
			else
			{
				LeftSealHole_FaceVector.push_back(EachFace);//剩余的面作为下一类型孔特征的种子面
			}
		}
	}



	for (std::vector<Face*>::size_type i = 0; i != sealdirection_FaceVector.size(); i++)
	{
		Face *Each_Face = sealdirection_FaceVector[i];

		std::vector<Face*>EachAd_FaceVector;
		get_Adjacent_Faces(Each_Face, EachAd_FaceVector);
		if (EachAd_FaceVector.size() != 2)
		{
			continue;
		}

		if (EachAd_FaceVector[0]->Tag() == TopPlaneFace->Tag())
		{
			if (Is_Face_in_lpFace(EachAd_FaceVector[1], ParallelPlaneFace) != -1)
			{
				SealFace_DirectionNormalHole.push_back(Each_Face);
			}
			else
			{
				SealFace_DirectionBlindHole.push_back(Each_Face);
			}

		}

		else if (EachAd_FaceVector[1]->Tag() == TopPlaneFace->Tag())
		{
			if (Is_Face_in_lpFace(EachAd_FaceVector[0], ParallelPlaneFace) != -1)
			{
				SealFace_DirectionNormalHole.push_back(Each_Face);
			}
			else
			{
				SealFace_DirectionBlindHole.push_back(Each_Face);
			}

		}
		else if (EachAd_FaceVector[0]->Tag() == BottomPlaneFace->Tag() && Is_Face_in_lpFace(EachAd_FaceVector[1], ParallelPlaneFace) != -1)
		{
			SealFace_DirectionNormalHole.push_back(Each_Face);
		}
		else if (EachAd_FaceVector[1]->Tag() == BottomPlaneFace->Tag() && Is_Face_in_lpFace(EachAd_FaceVector[0], ParallelPlaneFace) != -1)
		{
			SealFace_DirectionNormalHole.push_back(Each_Face);
		}
		else if (Is_Face_in_lpFace(EachAd_FaceVector[0], ParallelPlaneFace) != -1 && EachAd_FaceVector[1]->SolidFaceType() == Face::FaceTypeConical)
		{
			SealFace_DirectionRightHole.push_back(Each_Face);
		}
		else if (Is_Face_in_lpFace(EachAd_FaceVector[1], ParallelPlaneFace) != -1 && EachAd_FaceVector[0]->SolidFaceType() == Face::FaceTypeConical)
		{
			SealFace_DirectionRightHole.push_back(Each_Face);
		}

		//if(EachAd_FaceVector[0]->SolidFaceType()==Face::FaceTypePlanar&&EachAd_FaceVector[1]->SolidFaceType()==Face::FaceTypePlanar)
		//{
		//	int type=0,type1=0;       //判断面类型
		//	int norm_dir=0,norm_dir1=0;   //-1=外R，0=内R
		//	double point[3],point1[3];  //轴中心 点
		//	double dir[3],dir1[3];    //轴方向
		//	double box[6],box1[6];    //极点坐标
		//	double radius,radius1;    //半径 圆锥=最小半径 + 最大半径 /2
		//	double rad_data,rad_data1;
		//	Face*TopFace;Face*BottomFace;

		//	UF_MODL_ask_face_data (EachAd_FaceVector[0]->Tag(),&type,point,dir,box,&radius,&rad_data,&norm_dir);
		//	UF_MODL_ask_face_data (EachAd_FaceVector[1]->Tag(),&type1,point1,dir1,box1,&radius1,&rad_data1,&norm_dir1);
		//	if(fabs(point[0])>fabs(point1[0]))//根据X大小判断上下
		//	{
		//		TopFace=EachAd_FaceVector[1];
		//		BottomFace=EachAd_FaceVector[0];
		//	}
		//	else
		//	{
		//		TopFace=EachAd_FaceVector[0];
		//		BottomFace=EachAd_FaceVector[1];
		//	}

		//	std::vector<Edge*>EdgeVector,EdgeVector1;
		//	EdgeVector=BottomFace->GetEdges();
		//	EdgeVector1=TopFace->GetEdges();
		//	if(EdgeVector.size()==1&&EdgeVector1.size()>3)//底面边为1个，说明盲孔
		//	{
		//		//UF_DISP_set_highlight(Each_Face->Tag(),1);//TEST ok
		//		SealFace_DirectionBlindHole.push_back(Each_Face);
		//	}
		//	else if (EdgeVector.size()>3&&EdgeVector1.size()>3)//通孔
		//	{
		//		if(Is_Face_in_lpFace(Each_Face,SealFace_DirectionNromalHole)==-1)
		//		{
		//			//UF_DISP_set_highlight(Each_Face->Tag(),1);//TEST ok
		//			SealFace_DirectionNromalHole.push_back(Each_Face); 
		//		}
		//	}

		//}
		//else//这里只剩下了倒角通孔(这里以后需要修改，可以直接在这里添加限制条件)
		//{
		//	//UF_DISP_set_highlight(Each_Face->Tag(),1);
		//	if(Is_Face_in_lpFace(Each_Face,SealFace_DirectionRightHole)==-1)
		//	{
		//		SealFace_DirectionRightHole.push_back(Each_Face);
		//	}
		//}
	}
	//MT_delete_progress_bar(&aa);//删除进度条
	//UF_unload_library(library_name);

	OrderDirectionHole(SealFace_DirectionBlindHole, BeginFace);
	OrderDirectionHole(SealFace_DirectionNormalHole, BeginFace);
	OrderDirectionHole(SealFace_DirectionRightHole, BeginFace);
}

//构建轴向孔特征
void CreateDirectionMillFeature(Face* BottomPlaneFace, Face* TopPlaneFace,
	std::vector<Face*> ParallelPlaneFace, std::vector<Face*>SealFace_DirectionNormalHole, std::vector<Face*>SealFace_DirectionRightHole,
	std::vector<Face*>SealFace_DirectionBlindHole,
	std::vector<DirectionNormalHole*>&Feature_DirectionNormalHole,//普通通孔
	std::vector<DirectionRightHole*>&Feature_DirectionRightHole,//倒斜角通孔
	std::vector<DirectionBlindHole*>&Feature_DirectionBlindHole)//盲孔
{
	//===============================DEFINE PROGRESS Begin===================================================================
	//char *library_name = "D:\\ug10\\UGII\\libdisp.dll";
	//typedef void(*create_progress_bar)(int v1, bool v2, long *v3);//定义传参
	//create_progress_bar MT_create_progress_bar = NULL;
	//UF_load_library(library_name, "?MT_create_progress_bar@@YA_NH_NPEAJ@Z", (UF_load_f_p_t *)&MT_create_progress_bar);
	//typedef void(*set_progress_pos)(long *v1, int v2);//定义传参
	//set_progress_pos MT_set_progress_pos = NULL;
	//UF_load_library(library_name, "?MT_set_progress_pos@@YAXPEAJH@Z", (UF_load_f_p_t *)&MT_set_progress_pos);
	//typedef void(*show_progress_bar)(long *v1);//定义传参
	//show_progress_bar MT_show_progress_bar = NULL;
	//UF_load_library(library_name, "?MT_show_progress_bar@@YAXPEAJ@Z", (UF_load_f_p_t *)&MT_show_progress_bar);
	//typedef void(*set_progress_range)(long *v1, int v2, int v3);//定义传参
	//set_progress_range MT_set_progress_range = NULL;
	//UF_load_library(library_name, "?MT_set_progress_range@@YAXPEAJHH@Z", (UF_load_f_p_t *)&MT_set_progress_range);
	//typedef __int64(*ask_progress_cue_context)(void);//定义传参
	//ask_progress_cue_context MT_ask_progress_cue_context = NULL;
	//UF_load_library(library_name, "?MT_ask_progress_cue_context@@YAHXZ", (UF_load_f_p_t *)&MT_ask_progress_cue_context);
	//typedef __int64(*delete_progress_bar)(long *v1);//定义传参
	//delete_progress_bar MT_delete_progress_bar = NULL;
	//UF_load_library(library_name, "?MT_delete_progress_bar@@YAXPEAJ@Z", (UF_load_f_p_t *)&MT_delete_progress_bar);
	//typedef __int64(*set_progress_text)(long *v1, char *v2, bool v3);//定义传参
	//set_progress_text MT_set_progress_text = NULL;
	//UF_load_library(library_name, "?MT_set_progress_text@@YAXPEAJPEAD_N@Z", (UF_load_f_p_t *)&MT_set_progress_text);
	//if (MT_ask_progress_cue_context == NULL || MT_create_progress_bar == NULL || MT_show_progress_bar == NULL
	//	|| MT_set_progress_text == NULL || MT_set_progress_range == NULL || MT_set_progress_pos == NULL
	//	|| MT_delete_progress_bar == NULL)
	//{
	//	uc1601("进度条报错提示", 1);
	//	return;
	//}

	//__int64 cc = MT_ask_progress_cue_context();
	//long aa = 0;
	//MT_create_progress_bar(cc, true, &aa);//true和false控制是否显示百分比数字
	//MT_show_progress_bar(&aa);
	//MT_set_progress_text(&aa, "Loading", true);//此处用中文好像不显示
	//int num0=SealFace_DirectionNromalHole.size()+  SealFace_DirectionRightHole.size()+  SealFace_DirectionBlindHole.size()-3;
	//MT_set_progress_range(&aa, 0, num0);
	//===============================DEFINE PROGRESS END===================================================================

	//第一步构建普通通孔特征
	for (std::vector<Face*>::size_type i = 0; i != SealFace_DirectionNormalHole.size(); i++)
	{
		/*MT_set_progress_pos(&aa, i);*/
		/*UF_UI_set_status("孔特征");*/
		Face*EachFace;
		EachFace = SealFace_DirectionNormalHole[i];
		std::vector<Face*>EachAd_FaceVector;
		get_Adjacent_Faces(EachFace, EachAd_FaceVector);
		if (EachAd_FaceVector.size() != 2)
		{
			uc1601("轴向普通通孔创建失败", 1);
			continue;
		}
		//不能用BottomFace->Tag()，否则出错
		Face*TopFace; Face*BottomFace;
		if (EachAd_FaceVector[0]->Tag() == TopPlaneFace->Tag() && Is_Face_in_lpFace(EachAd_FaceVector[1], ParallelPlaneFace) != -1)
		{
			TopFace = EachAd_FaceVector[0];
			BottomFace = EachAd_FaceVector[1];
		}
		else
		{
			TopFace = EachAd_FaceVector[1];
			BottomFace = EachAd_FaceVector[0];
		}

		DirectionNormalHole*Feature = new DirectionNormalHole;
		Feature->TopFaceVector.push_back(TopFace);
		Feature->BottomFaceVector.push_back(BottomFace);
		Feature->InWallVector.push_back(EachFace);
		Feature_DirectionNormalHole.push_back(Feature);//test  ok
	}

	//第二步构建轴向倒角通孔特征
	for (std::vector<Face*>::size_type i = 0; i != SealFace_DirectionRightHole.size(); i++)
	{
		//MT_set_progress_pos(&aa, i+SealFace_DirectionNromalHole.size()-1);
		Face*EachFace;
		EachFace = SealFace_DirectionRightHole[i];
		std::vector<Face*>EachAd_FaceVector;
		get_Adjacent_Faces(EachFace, EachAd_FaceVector);
		if (EachAd_FaceVector.size() != 2)
		{
			uc1601("轴向倒斜角通孔创建失败", 1);
			continue;
		}
		Face*RightFace;
		Face*TopFace; Face*BottomFace;
		//没有底面 BottomPlaneFace
		if (Is_Face_in_lpFace(EachAd_FaceVector[0], ParallelPlaneFace) != -1 && EachAd_FaceVector[1]->SolidFaceType() == Face::FaceTypeConical)
		{
			RightFace = EachAd_FaceVector[1];
			BottomFace = EachAd_FaceVector[0];
		}
		else if (Is_Face_in_lpFace(EachAd_FaceVector[1], ParallelPlaneFace) != -1 && EachAd_FaceVector[0]->SolidFaceType() == Face::FaceTypeConical)
		{
			RightFace = EachAd_FaceVector[0];
			BottomFace = EachAd_FaceVector[1];
		}

		std::vector<Face*>RightAd_FaceVector;
		get_Adjacent_Faces(RightFace, RightAd_FaceVector);
		if (RightAd_FaceVector[0]->Tag() == TopPlaneFace->Tag())
		{
			TopFace = RightAd_FaceVector[0];
		}
		else if (RightAd_FaceVector[1]->Tag() == TopPlaneFace->Tag())
		{
			TopFace = RightAd_FaceVector[1];
		}

		DirectionRightHole *Feature = new DirectionRightHole;
		Feature->TopFaceVector.push_back(TopFace);
		Feature->RightVector.push_back(RightFace);
		Feature->InWallVector.push_back(EachFace);
		Feature->BottomFaceVector.push_back(BottomFace);
		Feature_DirectionRightHole.push_back(Feature);//test ok
	}

	//第三步构建轴向盲孔特征
	for (std::vector<Face*>::size_type i = 0; i != SealFace_DirectionBlindHole.size(); i++)
	{
		//MT_set_progress_pos(&aa, i+SealFace_DirectionNromalHole.size()+SealFace_DirectionRightHole.size()-2);
		Face*EachFace;
		EachFace = SealFace_DirectionBlindHole[i];
		std::vector<Face*>EachAd_FaceVector;
		get_Adjacent_Faces(EachFace, EachAd_FaceVector);
		if (EachAd_FaceVector.size() != 2)
		{
			uc1601("轴向盲孔创建失败", 1);
			continue;
		}

		Face*TopFace; Face*BottomFace;
		if (EachAd_FaceVector[0]->Tag() == TopPlaneFace->Tag() && Is_Face_in_lpFace(EachAd_FaceVector[1], ParallelPlaneFace) == -1)
		{
			TopFace = EachAd_FaceVector[0];
			BottomFace = EachAd_FaceVector[1];
		}
		else if (EachAd_FaceVector[1] == TopPlaneFace && Is_Face_in_lpFace(EachAd_FaceVector[0], ParallelPlaneFace) == -1)
		{
			TopFace = EachAd_FaceVector[1];
			BottomFace = EachAd_FaceVector[0];
		}

		DirectionBlindHole *Feature = new DirectionBlindHole;
		Feature->TopFaceVector.push_back(TopFace);
		Feature->InWallVector.push_back(EachFace);
		Feature->BottomFaceVector.push_back(BottomFace);
		Feature_DirectionBlindHole.push_back(Feature);//test ok
	}
	//MT_delete_progress_bar(&aa);//删除进度条
	//UF_unload_library(library_name);
}

//31新模型轴向孔种子面识别
void DirectionMillRecognition_31(
    const std::vector<NXOpen::Face*>& AllFaces,
    NXOpen::Vector3d _Approach_Normal,
    std::vector<NXOpen::Face*>& LeftSealHole_FaceVector,
    std::vector<NXOpen::Face*>& SealFace_DirectionStepThroughHole,
    std::vector<NXOpen::Face*>& SealFace_DirectionLongRoundThroughHole,
    std::vector<NXOpen::Face*>& SealFace_DirectionBlindHole
)
{
	//第一步筛选：找出所有符合条件的轴向内圆柱面
	std::vector<NXOpen::Face*> seal_FaceVector;
	std::vector<NXOpen::Face*> sealdirection_FaceVector;

	for (std::vector<Face*>::size_type i = 0; i != AllFaces.size(); i++)
	{
		Face *EachFace = AllFaces[i];

		if (EachFace->SolidFaceType() == Face::FaceTypeCylindrical)
		{
			int type = 0;
			int norm_dir = 0;
			double point[3];
			double dir[3];
			double box[6];
			double radius;
			double rad_data;

			UF_MODL_ask_face_data(EachFace->Tag(), &type, point, dir, box, &radius, &rad_data, &norm_dir);
			std::vector<Edge*>EachFace_EdgeVector;
			EachFace_EdgeVector = EachFace->GetEdges();

			if (radius < 35 && norm_dir == -1 && EachFace_EdgeVector.size() < 20)
			{
				seal_FaceVector.push_back(EachFace);
			}
			else
			{
				continue;
			}

			Vector3d Each_Vector(dir[0], dir[1], dir[2]);

			double Angle = get_Angle_Two_vector3d(_Approach_Normal, Each_Vector);
			Angle = Angle / PI * 180;
			if (Angle < 1 || Angle > 179)//轴向孔
			{
				sealdirection_FaceVector.push_back(EachFace);
			}
			else
			{
				LeftSealHole_FaceVector.push_back(EachFace);//非轴向孔留给旧流程
			}
		}
	}

	RemoveDplicateFace(sealdirection_FaceVector);
	RemoveDplicateFace(LeftSealHole_FaceVector);

	//第四步2：识别轴向两级阶梯通孔
	std::vector<NXOpen::Face*> UsedDirectionFaceVector;

	for (std::vector<Face*>::size_type i = 0; i != sealdirection_FaceVector.size(); i++)
	{
		Face *EachFace = sealdirection_FaceVector[i];

		//跳过已使用的圆柱面
		if (Is_Face_in_lpFace(EachFace, UsedDirectionFaceVector) != -1)
		{
			continue;
		}

		//必须是完整圆柱面（至少2个完整圆）
		if (Get_FullCircle_Num1(EachFace) < 2)
		{
			continue;
		}

		std::vector<Face*> EachAd_FaceVector;
		get_Adjacent_Faces(EachFace, EachAd_FaceVector);

		bool IsFindStepThroughHole = false;

		for (std::vector<Face*>::size_type j = 0; j != EachAd_FaceVector.size(); j++)
		{
			Face *EachPlanerFace = EachAd_FaceVector[j];

			if (EachPlanerFace->SolidFaceType() != Face::FaceTypePlanar)
			{
				continue;
			}

			//从该平面相邻面中寻找另一个完整圆柱面
			std::vector<Face*> PlanerAd_FaceVector;
			get_Adjacent_Faces(EachPlanerFace, PlanerAd_FaceVector);

			Face *OtherCylinderFace = NULL;

			for (std::vector<Face*>::size_type k = 0; k != PlanerAd_FaceVector.size(); k++)
			{
				Face *CandidateFace = PlanerAd_FaceVector[k];

				if (CandidateFace == EachFace)
				{
					continue;
				}

				if (CandidateFace->SolidFaceType() != Face::FaceTypeCylindrical)
				{
					continue;
				}

				if (Is_Face_in_lpFace(CandidateFace, sealdirection_FaceVector) == -1)
				{
					continue;
				}

				if (Is_Face_in_lpFace(CandidateFace, UsedDirectionFaceVector) != -1)
				{
					continue;
				}

				if (Get_FullCircle_Num1(CandidateFace) < 2)
				{
					continue;
				}

				//判断两个圆柱面同轴
				{
					int type1 = 0, norm_dir1 = 0;
					double point1[3], dir1[3], box1[6], radius1, rad_data1;
					UF_MODL_ask_face_data(EachFace->Tag(), &type1, point1, dir1, box1, &radius1, &rad_data1, &norm_dir1);

					int type2 = 0, norm_dir2 = 0;
					double point2[3], dir2[3], box2[6], radius2, rad_data2;
					UF_MODL_ask_face_data(CandidateFace->Tag(), &type2, point2, dir2, box2, &radius2, &rad_data2, &norm_dir2);

					Vector3d Dir1(dir1[0], dir1[1], dir1[2]);
					Vector3d Dir2(dir2[0], dir2[1], dir2[2]);

					double AxisAngle = get_Angle_Two_vector3d(Dir1, Dir2);
					AxisAngle = AxisAngle / PI * 180;

					if (AxisAngle < 1 || AxisAngle > 179)//同向或反向
					{
						//计算两条平行轴线之间的距离
						Vector3d Delta(point2[0] - point1[0], point2[1] - point1[1], point2[2] - point1[2]);
						Vector3d Cross(Delta.Y * Dir1.Z - Delta.Z * Dir1.Y,
						               Delta.Z * Dir1.X - Delta.X * Dir1.Z,
						               Delta.X * Dir1.Y - Delta.Y * Dir1.X);
						double DirLength = sqrt(Dir1.X * Dir1.X + Dir1.Y * Dir1.Y + Dir1.Z * Dir1.Z);
						double CrossLength = sqrt(Cross.X * Cross.X + Cross.Y * Cross.Y + Cross.Z * Cross.Z);
						double AxisDistance = CrossLength / DirLength;

						if (AxisDistance < 0.01 && fabs(radius1 - radius2) > 0.01)//同轴异径
						{
							//确认阶梯通孔具有两个端面
							std::vector<Face*> EachFaceAd_FaceVector;
							get_Adjacent_Faces(EachFace, EachFaceAd_FaceVector);

							std::vector<Face*> CandidateAd_FaceVector;
							get_Adjacent_Faces(CandidateFace, CandidateAd_FaceVector);

							Face *EndFace1 = NULL;
							Face *EndFace2 = NULL;

							for (std::vector<Face*>::size_type m = 0; m != EachFaceAd_FaceVector.size(); m++)
							{
								Face *AdjFace = EachFaceAd_FaceVector[m];
								if (AdjFace != EachPlanerFace && AdjFace->SolidFaceType() == Face::FaceTypePlanar)
								{
									EndFace1 = AdjFace;
									break;
								}
							}

							for (std::vector<Face*>::size_type m = 0; m != CandidateAd_FaceVector.size(); m++)
							{
								Face *AdjFace = CandidateAd_FaceVector[m];
								if (AdjFace != EachPlanerFace && AdjFace->SolidFaceType() == Face::FaceTypePlanar)
								{
									EndFace2 = AdjFace;
									break;
								}
							}

							if (EndFace1 != NULL && EndFace2 != NULL && EndFace1 != EndFace2 && EndFace1 != EachPlanerFace && EndFace2 != EachPlanerFace)
							{
								SealFace_DirectionStepThroughHole.push_back(EachFace);
								UsedDirectionFaceVector.push_back(EachFace);
								UsedDirectionFaceVector.push_back(CandidateFace);
								IsFindStepThroughHole = true;
								break;
							}
						}
					}
				}

				if (OtherCylinderFace == NULL)
				{
					OtherCylinderFace = CandidateFace;
				}
			}

			if (IsFindStepThroughHole)
			{
				break;
			}
		}
	}

	RemoveDplicateFace(SealFace_DirectionStepThroughHole);
	RemoveDplicateFace(UsedDirectionFaceVector);

	//第四步3：识别轴向长圆通孔
	for (std::vector<Face*>::size_type i = 0; i != sealdirection_FaceVector.size(); i++)
	{
		Face *EachFace = sealdirection_FaceVector[i];

		//跳过已使用的圆柱面
		if (Is_Face_in_lpFace(EachFace, UsedDirectionFaceVector) != -1)
		{
			continue;
		}

		//必须是非完整圆柱面
		if (Get_FullCircle_Num1(EachFace) != 0)
		{
			continue;
		}

		std::vector<Face*> EachAd_FaceVector;
		get_Adjacent_Faces(EachFace, EachAd_FaceVector);

		//查找与EachFace通过直线公共边连接的平面
		std::vector<NXOpen::Face*> CommonPlanerFaceVector;

		for (std::vector<Face*>::size_type j = 0; j != EachAd_FaceVector.size(); j++)
		{
			Face *AdjFace = EachAd_FaceVector[j];

			if (AdjFace->SolidFaceType() != Face::FaceTypePlanar)
			{
				continue;
			}

			//获取两个面的公共边
			std::vector<Edge*> Corporate_EdgeVector;
			GetEdge_From_TwoFaces(EachFace, AdjFace, Corporate_EdgeVector);

			//检查是否存在直线公共边
			bool HasLinearEdge = false;
			for (std::vector<Edge*>::size_type k = 0; k != Corporate_EdgeVector.size(); k++)
			{
				int edge_type;
				UF_MODL_ask_edge_type(Corporate_EdgeVector[k]->Tag(), &edge_type);
				if (edge_type == UF_MODL_LINEAR_EDGE)
				{
					HasLinearEdge = true;
					break;
				}
			}

			if (HasLinearEdge)
			{
				CommonPlanerFaceVector.push_back(AdjFace);
			}
		}

		RemoveDplicateFace(CommonPlanerFaceVector);

		//要求恰好2个平面侧壁
		if (CommonPlanerFaceVector.size() != 2)
		{
			continue;
		}

		Face *PlanerFace1 = CommonPlanerFaceVector[0];
		Face *PlanerFace2 = CommonPlanerFaceVector[1];

		//获取两个平面各自的相邻面
		std::vector<Face*> Planer1Ad_FaceVector;
		std::vector<Face*> Planer2Ad_FaceVector;
		get_Adjacent_Faces(PlanerFace1, Planer1Ad_FaceVector);
		get_Adjacent_Faces(PlanerFace2, Planer2Ad_FaceVector);

		//在共同相邻面中寻找第二个非完整圆柱面
		std::vector<NXOpen::Face*> OtherCylinderFaceVector;

		for (std::vector<Face*>::size_type j = 0; j != Planer1Ad_FaceVector.size(); j++)
		{
			Face *CandidateFace = Planer1Ad_FaceVector[j];

			//必须同时存在于两个平面的相邻面中
			if (Is_Face_in_lpFace(CandidateFace, Planer2Ad_FaceVector) == -1)
			{
				continue;
			}

			//不等于EachFace
			if (CandidateFace == EachFace)
			{
				continue;
			}

			//必须是圆柱面
			if (CandidateFace->SolidFaceType() != Face::FaceTypeCylindrical)
			{
				continue;
			}

			//存在于轴向圆柱面集合
			if (Is_Face_in_lpFace(CandidateFace, sealdirection_FaceVector) == -1)
			{
				continue;
			}

			//未被其他特征使用
			if (Is_Face_in_lpFace(CandidateFace, UsedDirectionFaceVector) != -1)
			{
				continue;
			}

			//非完整圆柱面
			if (Get_FullCircle_Num1(CandidateFace) != 0)
			{
				continue;
			}

			//与两个平面都存在直线公共边
			std::vector<Edge*> CorEdges1;
			std::vector<Edge*> CorEdges2;
			GetEdge_From_TwoFaces(CandidateFace, PlanerFace1, CorEdges1);
			GetEdge_From_TwoFaces(CandidateFace, PlanerFace2, CorEdges2);

			bool HasLinear1 = false;
			for (std::vector<Edge*>::size_type k = 0; k != CorEdges1.size(); k++)
			{
				int edge_type;
				UF_MODL_ask_edge_type(CorEdges1[k]->Tag(), &edge_type);
				if (edge_type == UF_MODL_LINEAR_EDGE)
				{
					HasLinear1 = true;
					break;
				}
			}

			bool HasLinear2 = false;
			for (std::vector<Edge*>::size_type k = 0; k != CorEdges2.size(); k++)
			{
				int edge_type;
				UF_MODL_ask_edge_type(CorEdges2[k]->Tag(), &edge_type);
				if (edge_type == UF_MODL_LINEAR_EDGE)
				{
					HasLinear2 = true;
					break;
				}
			}

			if (HasLinear1 && HasLinear2)
			{
				OtherCylinderFaceVector.push_back(CandidateFace);
			}
		}

		RemoveDplicateFace(OtherCylinderFaceVector);

		//要求恰好1个第二圆柱面
		if (OtherCylinderFaceVector.size() != 1)
		{
			continue;
		}

		Face *OtherCylinderFace = OtherCylinderFaceVector[0];

		SealFace_DirectionLongRoundThroughHole.push_back(EachFace);
		UsedDirectionFaceVector.push_back(EachFace);
		UsedDirectionFaceVector.push_back(OtherCylinderFace);
	}

	RemoveDplicateFace(SealFace_DirectionLongRoundThroughHole);
	RemoveDplicateFace(UsedDirectionFaceVector);

	//第四步4：将剩余轴向圆柱面归为轴向盲孔
	for (std::vector<Face*>::size_type i = 0; i != sealdirection_FaceVector.size(); i++)
	{
		Face *EachFace = sealdirection_FaceVector[i];

		if (Is_Face_in_lpFace(EachFace, UsedDirectionFaceVector) != -1)
		{
			continue;
		}

		SealFace_DirectionBlindHole.push_back(EachFace);
		UsedDirectionFaceVector.push_back(EachFace);
	}

	RemoveDplicateFace(SealFace_DirectionBlindHole);
	RemoveDplicateFace(UsedDirectionFaceVector);
}

//新模型轴向孔特征创建
void CreateDirectionMillFeature_31(
    NXOpen::Vector3d _Approach_Normal,
    const std::vector<NXOpen::Face*>& SealFace_DirectionStepThroughHole,
    const std::vector<NXOpen::Face*>& SealFace_DirectionLongRoundThroughHole,
    const std::vector<NXOpen::Face*>& SealFace_DirectionBlindHole,
    std::vector<DirectionStepThroughHole_31*>& Feature_DirectionStepThroughHole,
    std::vector<DirectionLongRoundThroughHole_31*>& Feature_DirectionLongRoundThroughHole,
    std::vector<DirectionBlindHole_31*>& Feature_DirectionBlindHole
)
{
	//========== 一、创建轴向两级阶梯通孔 ==========
	for (std::vector<Face*>::size_type i = 0; i != SealFace_DirectionStepThroughHole.size(); i++)
	{
		Face *EachFace = SealFace_DirectionStepThroughHole[i];

		std::vector<Face*> EachAd_FaceVector;
		get_Adjacent_Faces(EachFace, EachAd_FaceVector);

		bool IsCreated = false;

		for (std::vector<Face*>::size_type j = 0; j != EachAd_FaceVector.size(); j++)
		{
			Face *EachPlanerFace = EachAd_FaceVector[j];

			if (EachPlanerFace->SolidFaceType() != Face::FaceTypePlanar)
			{
				continue;
			}

			//从该平面相邻面中寻找第二个完整圆柱面
			std::vector<Face*> PlanerAd_FaceVector;
			get_Adjacent_Faces(EachPlanerFace, PlanerAd_FaceVector);

			for (std::vector<Face*>::size_type k = 0; k != PlanerAd_FaceVector.size(); k++)
			{
				Face *OtherCylinderFace = PlanerAd_FaceVector[k];

				if (OtherCylinderFace == EachFace)
				{
					continue;
				}

				if (OtherCylinderFace->SolidFaceType() != Face::FaceTypeCylindrical)
				{
					continue;
				}

				int type2 = 0, norm_dir2 = 0;
				double point2[3], dir2[3], box2[6], radius2, rad_data2;
				UF_MODL_ask_face_data(OtherCylinderFace->Tag(), &type2, point2, dir2, box2, &radius2, &rad_data2, &norm_dir2);
				std::vector<Edge*> OtherEdges = OtherCylinderFace->GetEdges();

				if (norm_dir2 != -1 || radius2 >= 35 || OtherEdges.size() >= 20)
				{
					continue;
				}

				if (Get_FullCircle_Num1(OtherCylinderFace) < 2)
				{
					continue;
				}

				//判断同轴
				int type1 = 0, norm_dir1 = 0;
				double point1[3], dir1[3], box1[6], radius1, rad_data1;
				UF_MODL_ask_face_data(EachFace->Tag(), &type1, point1, dir1, box1, &radius1, &rad_data1, &norm_dir1);

				Vector3d Dir1(dir1[0], dir1[1], dir1[2]);
				Vector3d Dir2(dir2[0], dir2[1], dir2[2]);

				double AxisAngle = get_Angle_Two_vector3d(Dir1, Dir2);
				AxisAngle = AxisAngle / PI * 180;

				if (AxisAngle < 1 || AxisAngle > 179)
				{
					Vector3d Delta(point2[0] - point1[0], point2[1] - point1[1], point2[2] - point1[2]);
					Vector3d Cross(Delta.Y * Dir1.Z - Delta.Z * Dir1.Y,
					               Delta.Z * Dir1.X - Delta.X * Dir1.Z,
					               Delta.X * Dir1.Y - Delta.Y * Dir1.X);
					double DirLength = sqrt(Dir1.X * Dir1.X + Dir1.Y * Dir1.Y + Dir1.Z * Dir1.Z);
					double CrossLength = sqrt(Cross.X * Cross.X + Cross.Y * Cross.Y + Cross.Z * Cross.Z);
					double AxisDistance = CrossLength / DirLength;

					if (AxisDistance < 0.01 && fabs(radius1 - radius2) > 0.01)
					{
						//找两端孔口平面
						std::vector<Face*> EachFaceAd_FaceVector;
						get_Adjacent_Faces(EachFace, EachFaceAd_FaceVector);
						std::vector<Face*> OtherAd_FaceVector;
						get_Adjacent_Faces(OtherCylinderFace, OtherAd_FaceVector);

						Face *EndFace1 = NULL;
						Face *EndFace2 = NULL;

						for (std::vector<Face*>::size_type m = 0; m != EachFaceAd_FaceVector.size(); m++)
						{
							Face *AdjFace = EachFaceAd_FaceVector[m];
							if (AdjFace != EachPlanerFace && AdjFace->SolidFaceType() == Face::FaceTypePlanar)
							{
								EndFace1 = AdjFace;
								break;
							}
						}

						for (std::vector<Face*>::size_type m = 0; m != OtherAd_FaceVector.size(); m++)
						{
							Face *AdjFace = OtherAd_FaceVector[m];
							if (AdjFace != EachPlanerFace && AdjFace->SolidFaceType() == Face::FaceTypePlanar)
							{
								EndFace2 = AdjFace;
								break;
							}
						}

						if (EndFace1 != NULL && EndFace2 != NULL && EndFace1 != EndFace2
							&& EndFace1 != EachPlanerFace && EndFace2 != EachPlanerFace)
						{
							//根据投影确定上下
							int t1 = 0, nd1 = 0;
							double pt1[3], d1[3], bx1[6], r1, rd1;
							UF_MODL_ask_face_data(EndFace1->Tag(), &t1, pt1, d1, bx1, &r1, &rd1, &nd1);

							int t2 = 0, nd2 = 0;
							double pt2[3], d2[3], bx2[6], r2, rd2;
							UF_MODL_ask_face_data(EndFace2->Tag(), &t2, pt2, d2, bx2, &r2, &rd2, &nd2);

							double Proj1 = pt1[0] * _Approach_Normal.X + pt1[1] * _Approach_Normal.Y + pt1[2] * _Approach_Normal.Z;
							double Proj2 = pt2[0] * _Approach_Normal.X + pt2[1] * _Approach_Normal.Y + pt2[2] * _Approach_Normal.Z;

							Face *TopFace = NULL;
							Face *BottomFace = NULL;
							Face *TopInWallFace = NULL;
							Face *BottomInWallFace = NULL;

							if (Proj1 >= Proj2)
							{
								TopFace = EndFace1;
								BottomFace = EndFace2;
							}
							else
							{
								TopFace = EndFace2;
								BottomFace = EndFace1;
							}

							//通过相邻关系确定哪个圆柱面与TopFace相连
							std::vector<Face*> TopAd_FaceVector;
							get_Adjacent_Faces(TopFace, TopAd_FaceVector);
							if (Is_Face_in_lpFace(EachFace, TopAd_FaceVector) != -1)
							{
								TopInWallFace = EachFace;
								BottomInWallFace = OtherCylinderFace;
							}
							else
							{
								TopInWallFace = OtherCylinderFace;
								BottomInWallFace = EachFace;
							}

							DirectionStepThroughHole_31* Feature = new DirectionStepThroughHole_31;
							Feature->TopFaceVector.push_back(TopFace);
							Feature->TopInWallVector.push_back(TopInWallFace);
							Feature->PlanerVector.push_back(EachPlanerFace);
							Feature->BottomInWallVector.push_back(BottomInWallFace);
							Feature->BottomFaceVector.push_back(BottomFace);
							Feature->name = NULL;

							Feature_DirectionStepThroughHole.push_back(Feature);
							IsCreated = true;
							break;
						}
					}
				}
			}

			if (IsCreated)
			{
				break;
			}
		}
	}

	//========== 二、创建轴向长圆通孔 ==========
	for (std::vector<Face*>::size_type i = 0; i != SealFace_DirectionLongRoundThroughHole.size(); i++)
	{
		Face *EachFace = SealFace_DirectionLongRoundThroughHole[i];

		std::vector<Face*> EachAd_FaceVector;
		get_Adjacent_Faces(EachFace, EachAd_FaceVector);

		//查找与EachFace通过直线公共边连接的平面
		std::vector<NXOpen::Face*> PlanerFaceVector;

		for (std::vector<Face*>::size_type j = 0; j != EachAd_FaceVector.size(); j++)
		{
			Face *AdjFace = EachAd_FaceVector[j];

			if (AdjFace->SolidFaceType() != Face::FaceTypePlanar)
			{
				continue;
			}

			std::vector<Edge*> Corporate_EdgeVector;
			GetEdge_From_TwoFaces(EachFace, AdjFace, Corporate_EdgeVector);

			bool HasLinearEdge = false;
			for (std::vector<Edge*>::size_type k = 0; k != Corporate_EdgeVector.size(); k++)
			{
				int edge_type;
				UF_MODL_ask_edge_type(Corporate_EdgeVector[k]->Tag(), &edge_type);
				if (edge_type == UF_MODL_LINEAR_EDGE)
				{
					HasLinearEdge = true;
					break;
				}
			}

			if (HasLinearEdge)
			{
				PlanerFaceVector.push_back(AdjFace);
			}
		}

		RemoveDplicateFace(PlanerFaceVector);

		if (PlanerFaceVector.size() != 2)
		{
			continue;
		}

		Face *PlanerFace1 = PlanerFaceVector[0];
		Face *PlanerFace2 = PlanerFaceVector[1];

		//寻找同时与两个平面相邻的第二圆柱面
		std::vector<Face*> Planer1Ad_FaceVector;
		std::vector<Face*> Planer2Ad_FaceVector;
		get_Adjacent_Faces(PlanerFace1, Planer1Ad_FaceVector);
		get_Adjacent_Faces(PlanerFace2, Planer2Ad_FaceVector);

		std::vector<NXOpen::Face*> OtherCylinderFaceVector;

		for (std::vector<Face*>::size_type j = 0; j != Planer1Ad_FaceVector.size(); j++)
		{
			Face *CandidateFace = Planer1Ad_FaceVector[j];

			if (Is_Face_in_lpFace(CandidateFace, Planer2Ad_FaceVector) == -1)
			{
				continue;
			}

			if (CandidateFace == EachFace)
			{
				continue;
			}

			if (CandidateFace->SolidFaceType() != Face::FaceTypeCylindrical)
			{
				continue;
			}

			int tc = 0, nc = 0;
			double pc[3], dc[3], bc[6], rc, rdc;
			UF_MODL_ask_face_data(CandidateFace->Tag(), &tc, pc, dc, bc, &rc, &rdc, &nc);
			std::vector<Edge*> CandidateEdges = CandidateFace->GetEdges();

			if (nc != -1 || rc >= 35 || CandidateEdges.size() >= 20)
			{
				continue;
			}

			if (Get_FullCircle_Num1(CandidateFace) != 0)
			{
				continue;
			}

			Vector3d CandDir(dc[0], dc[1], dc[2]);
			double CandAngle = get_Angle_Two_vector3d(_Approach_Normal, CandDir);
			CandAngle = CandAngle / PI * 180;
			if (!(CandAngle < 1 || CandAngle > 179))
			{
				continue;
			}

			//与两个平面都存在直线公共边
			std::vector<Edge*> CorEdges1;
			std::vector<Edge*> CorEdges2;
			GetEdge_From_TwoFaces(CandidateFace, PlanerFace1, CorEdges1);
			GetEdge_From_TwoFaces(CandidateFace, PlanerFace2, CorEdges2);

			bool HasLinear1 = false;
			for (std::vector<Edge*>::size_type k = 0; k != CorEdges1.size(); k++)
			{
				int edge_type;
				UF_MODL_ask_edge_type(CorEdges1[k]->Tag(), &edge_type);
				if (edge_type == UF_MODL_LINEAR_EDGE)
				{
					HasLinear1 = true;
					break;
				}
			}

			bool HasLinear2 = false;
			for (std::vector<Edge*>::size_type k = 0; k != CorEdges2.size(); k++)
			{
				int edge_type;
				UF_MODL_ask_edge_type(CorEdges2[k]->Tag(), &edge_type);
				if (edge_type == UF_MODL_LINEAR_EDGE)
				{
					HasLinear2 = true;
					break;
				}
			}

			if (HasLinear1 && HasLinear2)
			{
				OtherCylinderFaceVector.push_back(CandidateFace);
			}
		}

		RemoveDplicateFace(OtherCylinderFaceVector);

		if (OtherCylinderFaceVector.size() != 1)
		{
			continue;
		}

		Face *OtherCylinderFace = OtherCylinderFaceVector[0];

		//找两个端面
		std::vector<Face*> EachFaceAd2;
		std::vector<Face*> OtherAd2;
		get_Adjacent_Faces(EachFace, EachFaceAd2);
		get_Adjacent_Faces(OtherCylinderFace, OtherAd2);

		std::vector<NXOpen::Face*> EndFaceVector;

		for (std::vector<Face*>::size_type j = 0; j != EachFaceAd2.size(); j++)
		{
			Face *AdjFace = EachFaceAd2[j];
			if (AdjFace->SolidFaceType() == Face::FaceTypePlanar
				&& AdjFace != PlanerFace1 && AdjFace != PlanerFace2
				&& Is_Face_in_lpFace(AdjFace, OtherAd2) != -1)
			{
				EndFaceVector.push_back(AdjFace);
			}
		}

		RemoveDplicateFace(EndFaceVector);

		if (EndFaceVector.size() != 2)
		{
			continue;
		}

		//根据投影确定上下
		int te1 = 0, ne1 = 0;
		double pe1[3], de1[3], be1[6], re1, rde1;
		UF_MODL_ask_face_data(EndFaceVector[0]->Tag(), &te1, pe1, de1, be1, &re1, &rde1, &ne1);

		int te2 = 0, ne2 = 0;
		double pe2[3], de2[3], be2[6], re2, rde2;
		UF_MODL_ask_face_data(EndFaceVector[1]->Tag(), &te2, pe2, de2, be2, &re2, &rde2, &ne2);

		double ProjE1 = pe1[0] * _Approach_Normal.X + pe1[1] * _Approach_Normal.Y + pe1[2] * _Approach_Normal.Z;
		double ProjE2 = pe2[0] * _Approach_Normal.X + pe2[1] * _Approach_Normal.Y + pe2[2] * _Approach_Normal.Z;

		Face *TopFace = NULL;
		Face *BottomFace = NULL;

		if (ProjE1 >= ProjE2)
		{
			TopFace = EndFaceVector[0];
			BottomFace = EndFaceVector[1];
		}
		else
		{
			TopFace = EndFaceVector[1];
			BottomFace = EndFaceVector[0];
		}

		DirectionLongRoundThroughHole_31* Feature = new DirectionLongRoundThroughHole_31;
		Feature->TopFaceVector.push_back(TopFace);
		Feature->CylinderVector.push_back(EachFace);
		Feature->CylinderVector.push_back(OtherCylinderFace);
		Feature->PlanerVector.push_back(PlanerFace1);
		Feature->PlanerVector.push_back(PlanerFace2);
		Feature->BottomFaceVector.push_back(BottomFace);
		Feature->name = NULL;

		Feature_DirectionLongRoundThroughHole.push_back(Feature);
	}

	//========== 三、创建轴向盲孔 ==========
	for (std::vector<Face*>::size_type i = 0; i != SealFace_DirectionBlindHole.size(); i++)
	{
		Face *EachFace = SealFace_DirectionBlindHole[i];

		std::vector<Face*> EachAd_FaceVector;
		get_Adjacent_Faces(EachFace, EachAd_FaceVector);

		std::vector<NXOpen::Face*> PlanerFaceVector;

		for (std::vector<Face*>::size_type j = 0; j != EachAd_FaceVector.size(); j++)
		{
			Face *AdjFace = EachAd_FaceVector[j];

			if (AdjFace->SolidFaceType() == Face::FaceTypePlanar)
			{
				PlanerFaceVector.push_back(AdjFace);
			}
		}

		RemoveDplicateFace(PlanerFaceVector);

		if (PlanerFaceVector.size() != 2)
		{
			continue;
		}

		//根据投影确定上下
		int tp1 = 0, np1 = 0;
		double pp1[3], dp1[3], bp1[6], rp1, rdp1;
		UF_MODL_ask_face_data(PlanerFaceVector[0]->Tag(), &tp1, pp1, dp1, bp1, &rp1, &rdp1, &np1);

		int tp2 = 0, np2 = 0;
		double pp2[3], dp2[3], bp2[6], rp2, rdp2;
		UF_MODL_ask_face_data(PlanerFaceVector[1]->Tag(), &tp2, pp2, dp2, bp2, &rp2, &rdp2, &np2);

		double Proj1 = pp1[0] * _Approach_Normal.X + pp1[1] * _Approach_Normal.Y + pp1[2] * _Approach_Normal.Z;
		double Proj2 = pp2[0] * _Approach_Normal.X + pp2[1] * _Approach_Normal.Y + pp2[2] * _Approach_Normal.Z;

		Face *TopFace = NULL;
		Face *BottomFace = NULL;

		if (Proj1 >= Proj2)
		{
			TopFace = PlanerFaceVector[0];
			BottomFace = PlanerFaceVector[1];
		}
		else
		{
			TopFace = PlanerFaceVector[1];
			BottomFace = PlanerFaceVector[0];
		}

		DirectionBlindHole_31* Feature = new DirectionBlindHole_31;
		Feature->TopFaceVector.push_back(TopFace);
		Feature->InWallVector.push_back(EachFace);
		Feature->BottomFaceVector.push_back(BottomFace);
		Feature->name = NULL;

		Feature_DirectionBlindHole.push_back(Feature);
	}
}

//识别壁面和筋面的孔
void WallAndRibMillRecognition(std::vector<Face*>AllFaces, std::vector<Face*>&LeftSealHole_FaceVector
	, std::vector<Face*>&SealFace_WallNormalHole, std::vector<Face*>&SealFace_WallCircleCombineHole
	, std::vector<Face*>&SealFace_RibCombineHole)
{
	//===============================DEFINE PROGRESS Begin===================================================================
	//char *library_name = "D:\\ug10\\UGII\\libdisp.dll";
	//typedef void(*create_progress_bar)(int v1, bool v2, long *v3);//定义传参
	//create_progress_bar MT_create_progress_bar = NULL;
	//UF_load_library(library_name, "?MT_create_progress_bar@@YA_NH_NPEAJ@Z", (UF_load_f_p_t *)&MT_create_progress_bar);
	//typedef void(*set_progress_pos)(long *v1, int v2);//定义传参
	//set_progress_pos MT_set_progress_pos = NULL;
	//UF_load_library(library_name, "?MT_set_progress_pos@@YAXPEAJH@Z", (UF_load_f_p_t *)&MT_set_progress_pos);
	//typedef void(*show_progress_bar)(long *v1);//定义传参
	//show_progress_bar MT_show_progress_bar = NULL;
	//UF_load_library(library_name, "?MT_show_progress_bar@@YAXPEAJ@Z", (UF_load_f_p_t *)&MT_show_progress_bar);
	//typedef void(*set_progress_range)(long *v1, int v2, int v3);//定义传参
	//set_progress_range MT_set_progress_range = NULL;
	//UF_load_library(library_name, "?MT_set_progress_range@@YAXPEAJHH@Z", (UF_load_f_p_t *)&MT_set_progress_range);
	//typedef __int64(*ask_progress_cue_context)(void);//定义传参
	//ask_progress_cue_context MT_ask_progress_cue_context = NULL;
	//UF_load_library(library_name, "?MT_ask_progress_cue_context@@YAHXZ", (UF_load_f_p_t *)&MT_ask_progress_cue_context);
	//typedef __int64(*delete_progress_bar)(long *v1);//定义传参
	//delete_progress_bar MT_delete_progress_bar = NULL;
	//UF_load_library(library_name, "?MT_delete_progress_bar@@YAXPEAJ@Z", (UF_load_f_p_t *)&MT_delete_progress_bar);
	//typedef __int64(*set_progress_text)(long *v1, char *v2, bool v3);//定义传参
	//set_progress_text MT_set_progress_text = NULL;
	//UF_load_library(library_name, "?MT_set_progress_text@@YAXPEAJPEAD_N@Z", (UF_load_f_p_t *)&MT_set_progress_text);
	//if (MT_ask_progress_cue_context == NULL || MT_create_progress_bar == NULL || MT_show_progress_bar == NULL
	//	|| MT_set_progress_text == NULL || MT_set_progress_range == NULL || MT_set_progress_pos == NULL
	//	|| MT_delete_progress_bar == NULL)
	//{
	//	uc1601("进度条报错提示", 1);
	//	return;
	//}

	//__int64 cc = MT_ask_progress_cue_context();
	//long aa = 0;
	//MT_create_progress_bar(cc, true, &aa);//true和false控制是否显示百分比数字
	//MT_show_progress_bar(&aa);
	//MT_set_progress_text(&aa, "Loading", true);//此处用中文好像不显示

	//int num0=AllFaces.size()-1;
	//MT_set_progress_range(&aa, 0, num0);
	//int NowNum=0;
	//===============================DEFINE PROGRESS END===================================================================
	for (std::vector<Face*>::size_type i = 0; i != AllFaces.size(); i++)
	{
		/*MT_set_progress_pos(&aa, NowNum++);*/
		/*UF_UI_set_status("壁面和筋面孔特征识别");*/
		Face*EachFace;
		EachFace = AllFaces[i];

		

		std::vector<Face*>EachAd_FaceVector;
		get_Adjacent_Faces(EachFace, EachAd_FaceVector); //得到当前面的相邻面

		int type = 0, type1 = 0;       //判断面类型
		int norm_dir = 0, norm_dir1 = 0;   //-1=外R，0=内R
		double point[3], point1[3];  //轴中心 点
		double dir[3], dir1[3];    //轴方向
		double box[6], box1[6];    //极点坐标
		double radius, radius1;    //半径 圆锥=最小半径 + 最大半径 /2
		double rad_data, rad_data1;

		UF_MODL_ask_face_data(EachAd_FaceVector[0]->Tag(), &type, point, dir, box, &radius, &rad_data, &norm_dir);
		UF_MODL_ask_face_data(EachAd_FaceVector[1]->Tag(), &type1, point1, dir1, box1, &radius1, &rad_data1, &norm_dir1);

		Face*TopFace; Face*BottomFace;
		//判断壁面普通孔  //加上面 的类型圆柱圆锥面，去除两端平面

		if (radius>200 && radius1>200)
		{
			//UF_DISP_set_highlight(EachFace->Tag(),1);
			SealFace_WallNormalHole.push_back(EachFace);
			continue;
		}

		std::vector<Edge*>EachFace_EdgeVector;

			

		Edge*FullCircleEdge;//种子面的完整圆弧边
		EachFace_EdgeVector = EachFace->GetEdges();

		//if (EachFace_EdgeVector.size() != 2) //增大容错
		//{
		//	continue;
		//}


		int FullNum = 0;
		for (std::vector<Edge*>::size_type j = 0; j != EachFace_EdgeVector.size(); j++)
		{
			if (IsCircleEdgeComplete(EachFace_EdgeVector[j]))
			{
				FullNum++;
				FullCircleEdge = EachFace_EdgeVector[j];
			}
		}
		if (FullNum != 1)//这里要求必须是"单侧"完整圆弧
		{
			continue;
		}

		Face *MidPlanar;
		GetOneAdjacentFace(FullCircleEdge, EachFace, MidPlanar);

		if (MidPlanar->SolidFaceType() != Face::FaceTypePlanar)
		{
			continue;
		}

		Face*CircleFace;
		Get_Next_Face(MidPlanar, EachFace, CircleFace);
		if (CircleFace == MidPlanar)
		{
			continue;
		}

		double CircleRad;
		get_face_maxRad(CircleFace, CircleRad);
		if (CircleRad>3)
		{
			continue;
		}
		Face*BigCylinderFace;
		Get_Next_Face(CircleFace, MidPlanar, BigCylinderFace);
		if (BigCylinderFace->SolidFaceType() == Face::FaceTypeCylindrical)
		{
			SealFace_WallCircleCombineHole.push_back(EachFace);
		}
		else if (BigCylinderFace->SolidFaceType() == Face::FaceTypeConical)
		{
			double Rad;
			double Rad1;
			get_face_maxRad(CircleFace, Rad);
			get_face_maxRad(BigCylinderFace, Rad1);
			std::vector<Edge*>Big_EdgeVector;
			Big_EdgeVector = BigCylinderFace->GetEdges();
			if (Rad<3 && Rad1>100)//限制倒圆角面必须曲率小.同时筋顶面曲率一定要大
			{
				SealFace_RibCombineHole.push_back(EachFace);
			}
		}
	}
	RemoveFaceVectorInFaceVector(SealFace_RibCombineHole, AllFaces);
	RemoveFaceVectorInFaceVector(SealFace_WallCircleCombineHole, AllFaces);
	RemoveFaceVectorInFaceVector(SealFace_WallNormalHole, AllFaces);
	for (std::vector<Face*>::size_type i = 0; i != AllFaces.size(); i++)
	{
		LeftSealHole_FaceVector.push_back(AllFaces[i]);
	}

	//MT_delete_progress_bar(&aa);//删除进度条
	//UF_unload_library(library_name);
}

//创建壁面和筋面孔特征
void CreateWallAndRibMillFeature(std::vector<NXOpen::Face*>SealFace_WallNormalHole, std::vector<NXOpen::Face*>SealFace_WallCircleCombineHole,
	std::vector<NXOpen::Face*>SealFace_RibCombineHole,
	std::vector<WallNormalHole*>&Feature_WallNormalHole,
	std::vector<WallCircleCombineHole *>&Feature_WallCircleCombineHole,
	std::vector<RibCombineHole*>&Feature_RibCombineHole)
{
	//===============================DEFINE PROGRESS Begin===================================================================
	//char *library_name = "D:\\ug10\\UGII\\libdisp.dll";
	//typedef void(*create_progress_bar)(int v1, bool v2, long *v3);//定义传参
	//create_progress_bar MT_create_progress_bar = NULL;
	//UF_load_library(library_name, "?MT_create_progress_bar@@YA_NH_NPEAJ@Z", (UF_load_f_p_t *)&MT_create_progress_bar);
	//typedef void(*set_progress_pos)(long *v1, int v2);//定义传参
	//set_progress_pos MT_set_progress_pos = NULL;
	//UF_load_library(library_name, "?MT_set_progress_pos@@YAXPEAJH@Z", (UF_load_f_p_t *)&MT_set_progress_pos);
	//typedef void(*show_progress_bar)(long *v1);//定义传参
	//show_progress_bar MT_show_progress_bar = NULL;
	//UF_load_library(library_name, "?MT_show_progress_bar@@YAXPEAJ@Z", (UF_load_f_p_t *)&MT_show_progress_bar);
	//typedef void(*set_progress_range)(long *v1, int v2, int v3);//定义传参
	//set_progress_range MT_set_progress_range = NULL;
	//UF_load_library(library_name, "?MT_set_progress_range@@YAXPEAJHH@Z", (UF_load_f_p_t *)&MT_set_progress_range);
	//typedef __int64(*ask_progress_cue_context)(void);//定义传参
	//ask_progress_cue_context MT_ask_progress_cue_context = NULL;
	//UF_load_library(library_name, "?MT_ask_progress_cue_context@@YAHXZ", (UF_load_f_p_t *)&MT_ask_progress_cue_context);
	//typedef __int64(*delete_progress_bar)(long *v1);//定义传参
	//delete_progress_bar MT_delete_progress_bar = NULL;
	//UF_load_library(library_name, "?MT_delete_progress_bar@@YAXPEAJ@Z", (UF_load_f_p_t *)&MT_delete_progress_bar);
	//typedef __int64(*set_progress_text)(long *v1, char *v2, bool v3);//定义传参
	//set_progress_text MT_set_progress_text = NULL;
	//UF_load_library(library_name, "?MT_set_progress_text@@YAXPEAJPEAD_N@Z", (UF_load_f_p_t *)&MT_set_progress_text);
	//if (MT_ask_progress_cue_context == NULL || MT_create_progress_bar == NULL || MT_show_progress_bar == NULL
	//	|| MT_set_progress_text == NULL || MT_set_progress_range == NULL || MT_set_progress_pos == NULL
	//	|| MT_delete_progress_bar == NULL)
	//{
	//	uc1601("进度条报错提示", 1);
	//	return;
	//}

	//__int64 cc = MT_ask_progress_cue_context();
	//long aa = 0;
	//MT_create_progress_bar(cc, true, &aa);//true和false控制是否显示百分比数字
	//MT_show_progress_bar(&aa);
	//MT_set_progress_text(&aa, "Loading", true);//此处用中文好像不显示
	//int num0=SealFace_WallNromalHole.size()+SealFace_WallCircleCombineHole.size()+SealFace_RibCombineHole.size();
	//MT_set_progress_range(&aa, 0, num0);
	//int NowNum=0;
	//===============================DEFINE PROGRESS END===================================================================
	for (std::vector<Face*>::size_type i = 0; i != SealFace_WallNormalHole.size(); i++)
	{
		/*MT_set_progress_pos(&aa, NowNum++);*/
		/*UF_UI_set_status("孔特征");*/
		Face *EachFace;
		EachFace = SealFace_WallNormalHole[i];

		std::vector<Face*>EachAd_FaceVector;
		get_Adjacent_Faces(EachFace, EachAd_FaceVector);
		if (EachAd_FaceVector.size() != 2)
		{
			continue;
		}
		int type = 0, type1 = 0;       //判断面类型
		int norm_dir = 0, norm_dir1 = 0;   //-1=外R，0=内R
		double point[3], point1[3];  //轴中心 点
		double dir[3], dir1[3];    //轴方向
		double box[6], box1[6];    //极点坐标
		double radius, radius1;    //半径 圆锥=最小半径 + 最大半径 /2
		double rad_data, rad_data1;

		UF_MODL_ask_face_data(EachAd_FaceVector[0]->Tag(), &type, point, dir, box, &radius, &rad_data, &norm_dir);
		UF_MODL_ask_face_data(EachAd_FaceVector[1]->Tag(), &type1, point1, dir1, box1, &radius1, &rad_data1, &norm_dir1);

		Face *TopFace; Face *BottomFace;
		//判断壁面普通孔  //加上面 的类型圆柱圆锥面，去除两端平面

		if (radius>200 && radius1>200)
		{
			if (radius > radius1)
			{
				TopFace = EachAd_FaceVector[0];
				BottomFace = EachAd_FaceVector[1];
			}
			else
			{
				TopFace = EachAd_FaceVector[1];
				BottomFace = EachAd_FaceVector[0];
			}
		}
		WallNormalHole *Feature = new WallNormalHole;
		Feature->TopFaceVector.push_back(TopFace);
		Feature->InWallVector.push_back(EachFace);
		Feature->BottomFaceVector.push_back(BottomFace);
		Feature_WallNormalHole.push_back(Feature);
	}


	for (std::vector<Face*>::size_type i = 0; i != SealFace_WallCircleCombineHole.size(); i++)
	{
		//MT_set_progress_pos(&aa, NowNum++);
		Face *EachFace;
		Face *TopFace;
		Face *TopInWall;
		Face *Circle;
		Face *Planer;
		Face *BottomFace;
		EachFace = SealFace_WallCircleCombineHole[i];
		std::vector<Edge*>EachFace_EdgeVector;
		EachFace_EdgeVector = EachFace->GetEdges();
		if (EachFace_EdgeVector.size() != 2)//种子面必须时两个边
		{
			continue;
		}

		Edge *RightEdge; Edge *WrongEdge;
		if (IsCircleEdgeComplete(EachFace_EdgeVector[0]))
		{
			RightEdge = EachFace_EdgeVector[0]; WrongEdge = EachFace_EdgeVector[1];
		}
		else if (IsCircleEdgeComplete(EachFace_EdgeVector[1]))
		{
			RightEdge = EachFace_EdgeVector[1]; WrongEdge = EachFace_EdgeVector[0];
		}
		else
		{
			continue;//找到正确的边，否则就不构建特征了
		}

		GetOneAdjacentFace(WrongEdge, EachFace, BottomFace);
		GetOneAdjacentFace(RightEdge, EachFace, Planer);
		Get_Next_Face(Planer, EachFace, Circle);
		if (Circle == Planer) { continue; }
		Get_Next_Face(Circle, Planer, TopInWall);
		if (TopInWall == Circle){ continue; }
		Get_Next_Face(TopInWall, Circle, TopFace);
		if (TopFace == TopInWall){ continue; }

		WallCircleCombineHole *Feature = new WallCircleCombineHole;
		Feature->TopFaceVector.push_back(TopFace);
		Feature->TopInWallVector.push_back(TopInWall);
		Feature->CircleVector.push_back(Circle);
		Feature->PlanerVector.push_back(Planer);
		Feature->BottomInWallVector.push_back(EachFace);
		Feature->BottomFaceVector.push_back(BottomFace);
		Feature_WallCircleCombineHole.push_back(Feature);//test ok
	}

	for (std::vector<Face*>::size_type i = 0; i != SealFace_RibCombineHole.size(); i++)
	{
		//MT_set_progress_pos(&aa, NowNum++);
		Face*EachFace;
		EachFace = SealFace_RibCombineHole[i];
		Face* TopFace;
		Face* Circle;
		Face* Planer;
		Face* Right;
		Face* Right1;
		Face* Right2;
		Face* BottomFace;

		std::vector<Edge*>EachFace_EdgeVector;
		EachFace_EdgeVector = EachFace->GetEdges();
		Edge*RightEdge; std::vector<Edge*>WrongEdgeVector;
		if (EachFace_EdgeVector.size()<2)
		{
			continue;
		}

		for (std::vector<Edge*>::size_type j = 0; j != EachFace_EdgeVector.size(); j++)
		{
			if (IsCircleEdgeComplete(EachFace_EdgeVector[j]))
			{
				//UF_DISP_set_highlight(EachFace_EdgeVector[j]->Tag(),1);
				RightEdge = EachFace_EdgeVector[j];
			}
			else if (IsCircleEdgeComplete(EachFace_EdgeVector[j]) == 0)
			{
				//UF_DISP_set_highlight(EachFace_EdgeVector[j]->Tag(),1);
				WrongEdgeVector.push_back(EachFace_EdgeVector[j]);
			}
		}

		//这里开始往下找
		if (WrongEdgeVector.size() == 1)
		{
			//UF_DISP_set_highlight(WrongEdgeVector[0]->Tag(),1);
			//UF_DISP_set_highlight(EachFace->Tag(),1);
			GetOneAdjacentFace(WrongEdgeVector[0], EachFace, Right);//这里不太懂为什么Right不能使用容器？？？？？？？？
			Get_Next_Face(Right, EachFace, BottomFace);
			if (BottomFace == Right){ continue; }

			//这里继续向上找
			GetOneAdjacentFace(RightEdge, EachFace, Planer);
			Get_Next_Face(Planer, EachFace, Circle);
			if (Circle == Planer){ continue; }
			Get_Next_Face(Circle, Planer, TopFace);
			if (TopFace == Circle){ continue; }

			RibCombineHole *Feature = new RibCombineHole;
			Feature->TopFaceVector.push_back(TopFace);
			Feature->CircleVector.push_back(Circle);
			Feature->PlanerVector.push_back(Planer);
			Feature->InWallVector.push_back(EachFace);
			Feature->RightVector.push_back(Right);
			Feature->BottomFaceVector.push_back(BottomFace);
			Feature_RibCombineHole.push_back(Feature);
		}
		else if (WrongEdgeVector.size() == 2)
		{

			std::vector<Face*>TempFaceVector;
			GetOneAdjacentFace(WrongEdgeVector[0], EachFace, Right1);
			GetOneAdjacentFace(WrongEdgeVector[1], EachFace, Right2);
			get_Adjacent_Faces(Right1, TempFaceVector);
			for (std::vector<Face*>::size_type t = 0; t != TempFaceVector.size(); t++)
			{
				if (TempFaceVector[t] != EachFace&&TempFaceVector[t] != Right2)
				{
					BottomFace = TempFaceVector[t];
				}
			}
			//这里继续向上找
			GetOneAdjacentFace(RightEdge, EachFace, Planer);
			Get_Next_Face(Planer, EachFace, Circle);
			if (Circle == Planer){ continue; }
			Get_Next_Face(Circle, Planer, TopFace);
			if (TopFace == Circle){ continue; }

			RibCombineHole *Feature = new RibCombineHole;
			Feature->TopFaceVector.push_back(TopFace);
			Feature->CircleVector.push_back(Circle);
			Feature->PlanerVector.push_back(Planer);
			Feature->InWallVector.push_back(EachFace);
			Feature->RightVector.push_back(Right1);
			Feature->RightVector.push_back(Right2);
			Feature->BottomFaceVector.push_back(BottomFace);
			Feature_RibCombineHole.push_back(Feature);
		}
	}
	//MT_delete_progress_bar(&aa);//删除进度条
	//UF_unload_library(library_name);

}

//凸台非复合孔的识别
void BossHoleRecognition1(std::vector<Face*>AllFaces, std::vector<Face*>&LeftSealHole_FaceVector,
	std::vector<NXOpen::Face*>&SealFace_BossNormalHole,//凸台通孔
	std::vector<NXOpen::Face*>&SealFace_BossOneRightHole,//凸台单侧直角通孔
	std::vector<NXOpen::Face*>&SealFace_BossOneCircleHole,//凸台单侧圆角通孔
	std::vector<NXOpen::Face*>&SealFace_BossDoubleRightHole,//凸台双侧直角通孔
	std::vector<NXOpen::Face*>&SealFace_BossDoubleCircleHole,//凸台双侧圆角通孔
	std::vector<NXOpen::Face*>&SealFace_BossOneRightOneCircleHole,//凸台单侧直角单侧圆角通孔)
	std::vector<NXOpen::Face*>&SealFace_BossNormalBlindHole,//凸台无倒角盲孔
	std::vector<NXOpen::Face*>&SealFace_BossRightBlindHole)//凸台倒斜角盲孔)
{
	//===============================DEFINE PROGRESS Begin===================================================================
	//char *library_name = "D:\\ug10\\UGII\\libdisp.dll";
	//typedef void(*create_progress_bar)(int v1, bool v2, long *v3);//定义传参
	//create_progress_bar MT_create_progress_bar = NULL;
	//UF_load_library(library_name, "?MT_create_progress_bar@@YA_NH_NPEAJ@Z", (UF_load_f_p_t *)&MT_create_progress_bar);
	//typedef void(*set_progress_pos)(long *v1, int v2);//定义传参
	//set_progress_pos MT_set_progress_pos = NULL;
	//UF_load_library(library_name, "?MT_set_progress_pos@@YAXPEAJH@Z", (UF_load_f_p_t *)&MT_set_progress_pos);
	//typedef void(*show_progress_bar)(long *v1);//定义传参
	//show_progress_bar MT_show_progress_bar = NULL;
	//UF_load_library(library_name, "?MT_show_progress_bar@@YAXPEAJ@Z", (UF_load_f_p_t *)&MT_show_progress_bar);
	//typedef void(*set_progress_range)(long *v1, int v2, int v3);//定义传参
	//set_progress_range MT_set_progress_range = NULL;
	//UF_load_library(library_name, "?MT_set_progress_range@@YAXPEAJHH@Z", (UF_load_f_p_t *)&MT_set_progress_range);
	//typedef __int64(*ask_progress_cue_context)(void);//定义传参
	//ask_progress_cue_context MT_ask_progress_cue_context = NULL;
	//UF_load_library(library_name, "?MT_ask_progress_cue_context@@YAHXZ", (UF_load_f_p_t *)&MT_ask_progress_cue_context);
	//typedef __int64(*delete_progress_bar)(long *v1);//定义传参
	//delete_progress_bar MT_delete_progress_bar = NULL;
	//UF_load_library(library_name, "?MT_delete_progress_bar@@YAXPEAJ@Z", (UF_load_f_p_t *)&MT_delete_progress_bar);
	//typedef __int64(*set_progress_text)(long *v1, char *v2, bool v3);//定义传参
	//set_progress_text MT_set_progress_text = NULL;
	//UF_load_library(library_name, "?MT_set_progress_text@@YAXPEAJPEAD_N@Z", (UF_load_f_p_t *)&MT_set_progress_text);
	//if (MT_ask_progress_cue_context == NULL || MT_create_progress_bar == NULL || MT_show_progress_bar == NULL
	//	|| MT_set_progress_text == NULL || MT_set_progress_range == NULL || MT_set_progress_pos == NULL
	//	|| MT_delete_progress_bar == NULL)
	//{
	//	uc1601("进度条报错提示", 1);
	//	return;
	//}

	//__int64 cc = MT_ask_progress_cue_context();
	//long aa = 0;
	//MT_create_progress_bar(cc, true, &aa);//true和false控制是否显示百分比数字
	//MT_show_progress_bar(&aa);
	//MT_set_progress_text(&aa, "Loading", true);//此处用中文好像不显示

	//int num0= AllFaces.size();
	//MT_set_progress_range(&aa, 0, num0);
	//int NowNum=0;
	//===============================DEFINE PROGRESS END===================================================================
	for (std::vector<Face*>::size_type i = 0; i != AllFaces.size(); i++)
	{
		/*MT_set_progress_pos(&aa, NowNum++);*/
		/*UF_UI_set_status("凸台单侧孔特征识别");*/
		Face*EachFace;
		EachFace = AllFaces[i];

		std::vector<Edge*>EachFace_EdgeVector;
		EachFace_EdgeVector = EachFace->GetEdges();
		if (EachFace_EdgeVector.size() != 2)
		{
			/*LeftSealHole_FaceVector.push_back(EachFace);*/
			continue;
		}//凸台孔种子面的边必须是两个
		
		/*UF_OBJ_set_color(EachFace->Tag(), 50);*/


		Edge*RightEdge; Edge*WrongEdge;
		//这里是找寻外侧和内侧边   直角边有整圆，圆角边没有
		if (IsCircleEdgeComplete(EachFace_EdgeVector[0]) == 0 && IsCircleEdgeComplete(EachFace_EdgeVector[1]) == 0)//两侧均为不完整圆弧
		{
			/*UF_OBJ_set_color(EachFace->Tag(), 30);*/
			continue;
		}

		

		else if (IsCircleEdgeComplete(EachFace_EdgeVector[0]) && IsCircleEdgeComplete(EachFace_EdgeVector[1]))//两侧均为完整圆弧
		{
			

			//双侧直角，边中心点与坐标原点距离找寻外侧和内侧边 
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
			}
			else
			{
				RightEdge = EachFace_EdgeVector[1];
				WrongEdge = EachFace_EdgeVector[0];
			}
		}
		else  if (IsCircleEdgeComplete(EachFace_EdgeVector[0]) == 0 && IsCircleEdgeComplete(EachFace_EdgeVector[1]) == 1)//单侧完整圆弧
		{
			/*UF_OBJ_set_color(EachFace->Tag(), 70);*/
			RightEdge = EachFace_EdgeVector[1];
			WrongEdge = EachFace_EdgeVector[0];
		}
		else  if (IsCircleEdgeComplete(EachFace_EdgeVector[0]) == 1 && IsCircleEdgeComplete(EachFace_EdgeVector[1]) == 0)//单侧完整圆弧
		{
			/*UF_OBJ_set_color(EachFace->Tag(), 70);*/
			RightEdge = EachFace_EdgeVector[0];
			WrongEdge = EachFace_EdgeVector[1];
		}

		Face *InsideFace; Face *OutsideFace;//种子面的内外邻面
		GetOneAdjacentFace(WrongEdge, EachFace, InsideFace);
		GetOneAdjacentFace(RightEdge, EachFace, OutsideFace);

		if (OutsideFace->SolidFaceType() == Face::FaceTypeConical)
		{
			if (InsideFace->SolidFaceType() == Face::FaceTypePlanar)
			{
				SealFace_BossOneRightHole.push_back(EachFace);//凸台单侧直角通孔
			}
			else if (InsideFace->SolidFaceType() == Face::FaceTypeConical || InsideFace->SolidFaceType() == Face::FaceTypeCylindrical ||
				InsideFace->SolidFaceType() == Face::FaceTypeSurfaceOfRevolution ||
				InsideFace->SolidFaceType() == Face::FaceTypeParametric || InsideFace->SolidFaceType() == Face::FaceTypeBlending)
			{
				double Rad;
				get_face_maxRad(InsideFace, Rad);
				if (Rad>100)
				{
					SealFace_BossOneRightHole.push_back(EachFace);//凸台单侧直角通孔
				}
				else
				{
					if (InsideFace->SolidFaceType() == Face::FaceTypeConical)
					{
						std::vector<Edge*> BlindConcialEdge;
						BlindConcialEdge.clear();
						BlindConcialEdge = InsideFace->GetEdges();
						if (BlindConcialEdge.size() == 1)
						{
							SealFace_BossRightBlindHole.push_back(EachFace);//凸台倒斜角盲孔)
						}
						else
						{
							SealFace_BossDoubleRightHole.push_back(EachFace);//凸台双侧直角通孔
						}
					}
					else if (InsideFace->SolidFaceType() == Face::FaceTypeParametric)
					{
						SealFace_BossDoubleRightHole.push_back(EachFace);//凸台双侧直角通孔
					}
					else if (InsideFace->SolidFaceType() == Face::FaceTypeBlending)
					{
						SealFace_BossOneRightOneCircleHole.push_back(EachFace);//凸台单侧直角单侧圆角通孔)
					}
				}
			}
		}

		else if (OutsideFace->SolidFaceType() == Face::FaceTypeSurfaceOfRevolution)
		{
			if (InsideFace->SolidFaceType() == Face::FaceTypeSurfaceOfRevolution ||
				InsideFace->SolidFaceType() == Face::FaceTypeBlending)
			{
				SealFace_BossDoubleCircleHole.push_back(EachFace);//凸台双侧圆角通孔
			}
			else if (InsideFace->SolidFaceType() == Face::FaceTypeConical ||
				InsideFace->SolidFaceType() == Face::FaceTypeCylindrical)
			{//圆锥可能是直角面，构成单侧直角单侧圆角
				double rad;
				get_face_maxRad(InsideFace, rad);

				if (rad <100)
				{
					SealFace_BossOneRightOneCircleHole.push_back(EachFace);//凸台单侧直角单侧圆角通孔
				}
			}
		}
		else if (OutsideFace->SolidFaceType() == Face::FaceTypePlanar)
		{
			Face*TempFace;
			Get_Next_Face(OutsideFace, EachFace, TempFace);
			if (TempFace != OutsideFace)
			{
				int type = 0;       //判断面类型
				int norm_dir = 0;   //-1=外R，0=内R
				double point[3];  //轴中心 点
				double dir[3];    //轴方向
				double box[6];    //极点坐标
				double radius;    //半径 圆锥=最小半径 + 最大半径 /2
				double rad_data;

				UF_MODL_ask_face_data(TempFace->Tag(), &type, point, dir, box, &radius, &rad_data, &norm_dir);

				if (type == 16 && norm_dir == -1)
				{
					continue;
				}//排除复合无圆角孔
				if (type == 19)
				{
					continue;//多层孔
				}
			}
			if (InsideFace->SolidFaceType() == Face::FaceTypePlanar)
			{

				std::vector<Edge*> PlannerFaceEdge;
				PlannerFaceEdge.clear();
				PlannerFaceEdge = InsideFace->GetEdges();
				if (PlannerFaceEdge.size() < 3)//排除外宽内窄孔上顶面
				{
					continue;
				}//排除复合无圆角孔

				SealFace_BossNormalHole.push_back(EachFace);

			}

			else if (InsideFace->SolidFaceType() == Face::FaceTypeCylindrical ||
				InsideFace->SolidFaceType() == Face::FaceTypeConical)
			{

				std::vector<Edge*> ConcialFaceEdge;
				ConcialFaceEdge = InsideFace->GetEdges();
				if (ConcialFaceEdge.size() == 2)
				{
					continue;
				}

				else if (ConcialFaceEdge.size() == 1)//有可能外平面内侧倒直角，现在还没有
				{
					SealFace_BossNormalBlindHole.push_back(EachFace);
				}
				else
				{
					SealFace_BossNormalHole.push_back(EachFace);
				}

			}
		}

	}

	RemoveFaceVectorInFaceVector(SealFace_BossNormalHole, AllFaces);
	RemoveFaceVectorInFaceVector(SealFace_BossOneRightHole, AllFaces);
	RemoveFaceVectorInFaceVector(SealFace_BossOneCircleHole, AllFaces);
	RemoveFaceVectorInFaceVector(SealFace_BossOneRightOneCircleHole, AllFaces);
	RemoveFaceVectorInFaceVector(SealFace_BossDoubleRightHole, AllFaces);
	RemoveFaceVectorInFaceVector(SealFace_BossDoubleCircleHole, AllFaces);
	RemoveFaceVectorInFaceVector(SealFace_BossNormalBlindHole, AllFaces);
	RemoveFaceVectorInFaceVector(SealFace_BossRightBlindHole, AllFaces);

	for (std::vector<Face*>::size_type i = 0; i != AllFaces.size(); i++)
	{
		LeftSealHole_FaceVector.push_back(AllFaces[i]);
	}

	//MT_delete_progress_bar(&aa);//删除进度条
	//UF_unload_library(library_name);
}

void OtherHoleRecognition1(std::vector<Face*>AllFaces, std::vector<NXOpen::Face*>&SealFace_otherHole1,
						   std::vector<NXOpen::Face*>&SealFace_otherHole2,
						   std::vector<NXOpen::Face*>&SealFace_otherHole3)//其它孔)
{
	/*uc1601("进入识别", 1);*/

	for (std::vector<Face*>::size_type i = 0; i != AllFaces.size(); i++)
	{
		/*uc1601("进入for", 1);*/
		
		

		Face*EachFace;
		EachFace = AllFaces[i];

		/*UF_OBJ_set_color(EachFace->Tag(), 50);*/

		double Rad;
		get_face_maxRad(EachFace, Rad);

		std::vector<Edge*>EachFace_EdgeVector;
		EachFace_EdgeVector = EachFace->GetEdges();
		if (EachFace_EdgeVector.size() > 42) //增大容错
		{
			continue;
		}


		else if (EachFace_EdgeVector.size() > 1 && EachFace_EdgeVector.size() <= 42 && EachFace->SolidFaceType() == Face::FaceTypeCylindrical && Rad <100)
		{
			if (EachFace_EdgeVector.size() == 2)
			{

				Edge*RightEdge; Edge*WrongEdge;

				if (IsCircleEdgeComplete(EachFace_EdgeVector[0]) == 1 && IsCircleEdgeComplete(EachFace_EdgeVector[1]) == 1)//两侧完整圆
				{
					{
						//双侧直角，边中心点与坐标原点距离找寻外侧和内侧边 
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
						if (distance1 > distance2)
						{
							RightEdge = EachFace_EdgeVector[0];
							WrongEdge = EachFace_EdgeVector[1];
						}
						else
						{
							RightEdge = EachFace_EdgeVector[1];
							WrongEdge = EachFace_EdgeVector[0];
						}
					}
				}

				else  if (IsCircleEdgeComplete(EachFace_EdgeVector[0]) == 0 && IsCircleEdgeComplete(EachFace_EdgeVector[1]) == 1)//单侧完整圆弧
				{
					/*UF_OBJ_set_color(EachFace->Tag(), 70);*/
					RightEdge = EachFace_EdgeVector[1];
					WrongEdge = EachFace_EdgeVector[0];
				}
				else  if (IsCircleEdgeComplete(EachFace_EdgeVector[0]) == 1 && IsCircleEdgeComplete(EachFace_EdgeVector[1]) == 0)//单侧完整圆弧
				{
					/*UF_OBJ_set_color(EachFace->Tag(), 70);*/
					RightEdge = EachFace_EdgeVector[0];
					WrongEdge = EachFace_EdgeVector[1];
				}

				Face *InsideFace; Face *OutsideFace;//种子面的内外邻面
				GetOneAdjacentFace(WrongEdge, EachFace, InsideFace);
				GetOneAdjacentFace(RightEdge, EachFace, OutsideFace);
				/*continue;*/
				if (OutsideFace->SolidFaceType() == Face::FaceTypeConical || OutsideFace->SolidFaceType() == Face::FaceTypeSurfaceOfRevolution || OutsideFace->SolidFaceType() == Face::FaceTypePlanar)
				{
					SealFace_otherHole3.push_back(EachFace);
					continue;
				}


			}

			if (EachFace_EdgeVector.size() > 2) 
			{
				for (std::vector<Edge*>::size_type i = 0; i != EachFace_EdgeVector.size(); i++)
				{
					if (IsCircleEdgeComplete(EachFace_EdgeVector[i]) == 1)
					{
						SealFace_otherHole3.push_back(EachFace);
						continue;
					}
					

					/*uc1601("保存种子面2", 1);*/
				}
			}			
		}

		if (EachFace_EdgeVector.size() == 2 && Rad <100) //仅用来点亮
		{		
			if (EachFace->SolidFaceType() == Face::FaceTypeCylindrical )
			{
				SealFace_otherHole1.push_back(EachFace);
				/*uc1601("保存种子面3", 1);*/
			}				
		}




		//else if (EachFace_EdgeVector.size() == 2) //内平面孔
		//{
		//	Edge*RightEdge; Edge*WrongEdge;
		//	//这里是找寻外侧和内侧边   直角边有整圆，圆角边没有
		//	if (IsCircleEdgeComplete(EachFace_EdgeVector[0]) == 0 && IsCircleEdgeComplete(EachFace_EdgeVector[1]) == 0)//两侧都无完整圆
		//	{
		//		
		//		if (EachFace->SolidFaceType() == Face::FaceTypeCylindrical)
		//			SealFace_otherHole1.push_back(EachFace);
		//	
		//	}

		//	else if (IsCircleEdgeComplete(EachFace_EdgeVector[0]) == 1 && IsCircleEdgeComplete(EachFace_EdgeVector[1]) == 1)//两侧完整圆
		//	{
		//		{
		//			//双侧直角，边中心点与坐标原点距离找寻外侧和内侧边 
		//			double point[3]; point[0] = 0.0; point[1] = 0.0; point[2] = 0.0;
		//			UF_EVAL_arc_t arc_1;
		//			UF_EVAL_arc_t arc_2;//定义圆
		//			UF_EVAL_p_t evaluator_1 = NULL;
		//			UF_EVAL_p_t evaluator_2 = NULL;
		//			double distance1 = 0; double distance2 = 0;
		//			UF_EVAL_initialize(EachFace_EdgeVector[0]->Tag(), &evaluator_1);
		//			UF_EVAL_initialize(EachFace_EdgeVector[1]->Tag(), &evaluator_2);
		//			UF_EVAL_ask_arc(evaluator_1, &arc_1);
		//			UF_EVAL_ask_arc(evaluator_2, &arc_2);
		//			UF_VEC3_distance(arc_1.center, point, &distance1);//把两个点的连线距离赋值到cydistance1里面
		//			UF_VEC3_distance(arc_2.center, point, &distance2);
		//			if (distance1 > distance2)
		//			{
		//				RightEdge = EachFace_EdgeVector[0];
		//				WrongEdge = EachFace_EdgeVector[1];
		//			}
		//			else
		//			{
		//				RightEdge = EachFace_EdgeVector[1];
		//				WrongEdge = EachFace_EdgeVector[0];
		//			}
		//		}
		//	}

		//	else  if (IsCircleEdgeComplete(EachFace_EdgeVector[0]) == 0 && IsCircleEdgeComplete(EachFace_EdgeVector[1]) == 1)//单侧完整圆弧
		//	{
		//		/*UF_OBJ_set_color(EachFace->Tag(), 70);*/
		//		RightEdge = EachFace_EdgeVector[1];
		//		WrongEdge = EachFace_EdgeVector[0];
		//	}
		//	else  if (IsCircleEdgeComplete(EachFace_EdgeVector[0]) == 1 && IsCircleEdgeComplete(EachFace_EdgeVector[1]) == 0)//单侧完整圆弧
		//	{
		//		/*UF_OBJ_set_color(EachFace->Tag(), 70);*/
		//		RightEdge = EachFace_EdgeVector[0];
		//		WrongEdge = EachFace_EdgeVector[1];
		//	}

		//	Face *InsideFace; Face *OutsideFace;//种子面的内外邻面
		//	GetOneAdjacentFace(WrongEdge, EachFace, OutsideFace);
		//	GetOneAdjacentFace(RightEdge, EachFace, InsideFace);

		//	if (InsideFace->SolidFaceType() == Face::FaceTypeConical)
		//	{
		//		if (OutsideFace->SolidFaceType() == Face::FaceTypePlanar)
		//		{
		//			SealFace_otherHole2.push_back(EachFace);//凸台单侧直角通孔
		//		}
		//		else if (OutsideFace->SolidFaceType() == Face::FaceTypeConical || OutsideFace->SolidFaceType() == Face::FaceTypeCylindrical ||
		//			OutsideFace->SolidFaceType() == Face::FaceTypeSurfaceOfRevolution ||
		//			OutsideFace->SolidFaceType() == Face::FaceTypeParametric || OutsideFace->SolidFaceType() == Face::FaceTypeBlending)
		//		{
		//			double Rad;
		//			get_face_maxRad(OutsideFace, Rad);
		//			if (Rad > 100)
		//			{
		//				SealFace_otherHole2.push_back(EachFace);//凸台单侧直角通孔
		//			}
		//			else
		//			{
		//				if (OutsideFace->SolidFaceType() == Face::FaceTypeConical)
		//				{
		//					std::vector<Edge*> BlindConcialEdge;
		//					BlindConcialEdge.clear();
		//					BlindConcialEdge = OutsideFace->GetEdges();
		//					if (BlindConcialEdge.size() == 1)
		//					{
		//						SealFace_otherHole2.push_back(EachFace);//凸台倒斜角盲孔)
		//					}
		//					else
		//					{
		//						SealFace_otherHole2.push_back(EachFace);//凸台双侧直角通孔
		//					}
		//				}
		//				else if (OutsideFace->SolidFaceType() == Face::FaceTypeParametric)
		//				{
		//					SealFace_otherHole2.push_back(EachFace);//凸台双侧直角通孔
		//				}
		//				else if (OutsideFace->SolidFaceType() == Face::FaceTypeBlending)
		//				{
		//					SealFace_otherHole2.push_back(EachFace);//凸台单侧直角单侧圆角通孔)
		//				}
		//			}
		//		}
		//	}

		//	else if (InsideFace->SolidFaceType() == Face::FaceTypeSurfaceOfRevolution)
		//	{
		//		if (OutsideFace->SolidFaceType() == Face::FaceTypeSurfaceOfRevolution ||
		//			OutsideFace->SolidFaceType() == Face::FaceTypeBlending)
		//		{
		//			SealFace_otherHole2.push_back(EachFace);//凸台双侧圆角通孔
		//		}
		//		else if (OutsideFace->SolidFaceType() == Face::FaceTypeConical ||
		//			OutsideFace->SolidFaceType() == Face::FaceTypeCylindrical)
		//		{//圆锥可能是直角面，构成单侧直角单侧圆角
		//			double rad;
		//			get_face_maxRad(OutsideFace, rad);

		//			if (rad < 100)
		//			{
		//				SealFace_otherHole2.push_back(EachFace);//凸台单侧直角单侧圆角通孔
		//			}
		//		}
		//	}
		//	else if (InsideFace->SolidFaceType() == Face::FaceTypePlanar)
		//	{
		//		Face*TempFace;
		//		Get_Next_Face(InsideFace, EachFace, TempFace);
		//		if (TempFace != InsideFace)
		//		{
		//			int type = 0;       //判断面类型
		//			int norm_dir = 0;   //-1=外R，0=内R
		//			double point[3];  //轴中心 点
		//			double dir[3];    //轴方向
		//			double box[6];    //极点坐标
		//			double radius;    //半径 圆锥=最小半径 + 最大半径 /2
		//			double rad_data;

		//			UF_MODL_ask_face_data(TempFace->Tag(), &type, point, dir, box, &radius, &rad_data, &norm_dir);

		//			if (type == 16 && norm_dir == -1)
		//			{
		//				continue;
		//			}//排除复合无圆角孔
		//			if (type == 19)
		//			{
		//				continue;//多层孔
		//			}
		//		}
		//		if (OutsideFace->SolidFaceType() == Face::FaceTypePlanar)
		//		{

		//			std::vector<Edge*> PlannerFaceEdge;
		//			PlannerFaceEdge.clear();
		//			PlannerFaceEdge = OutsideFace->GetEdges();
		//			if (PlannerFaceEdge.size() < 3)//排除外宽内窄孔上顶面
		//			{
		//				continue;
		//			}//排除复合无圆角孔

		//			SealFace_otherHole2.push_back(EachFace);

		//		}

		//		else if (OutsideFace->SolidFaceType() == Face::FaceTypeCylindrical ||
		//			OutsideFace->SolidFaceType() == Face::FaceTypeConical)
		//		{

		//			std::vector<Edge*> ConcialFaceEdge;
		//			ConcialFaceEdge = OutsideFace->GetEdges();
		//			if (ConcialFaceEdge.size() == 2)
		//			{
		//				continue;
		//			}

		//			else if (ConcialFaceEdge.size() == 1)//有可能外平面内侧倒直角，现在还没有
		//			{
		//				SealFace_otherHole2.push_back(EachFace);
		//			}
		//			else
		//			{
		//				SealFace_otherHole2.push_back(EachFace);
		//			}

		//		}
		//	}
		//}
		//else
		//{
		//	continue;
		//}
	}
}


//凸台复合孔识别  这个识别时间长
void BossCombineHoleRecognition(std::vector<Face*>AllFaces, std::vector<Face*>&LeftSealHole_FaceVector,
	std::vector<NXOpen::Face*>&SealFace_BossManyLayerHole,
	std::vector<NXOpen::Face*>&SealFace_BossOutsideWideCombineHole,
	std::vector<NXOpen::Face*>&SealFace_BossInsideWideCombineHole,
	std::vector<NXOpen::Face*>&SealFace_BossMidCricleCombineHole)
{
	//===============================DEFINE PROGRESS Begin===================================================================
	//char *library_name = "D:\\ug10\\UGII\\libdisp.dll";
	//typedef void(*create_progress_bar)(int v1, bool v2, long *v3);//定义传参
	//create_progress_bar MT_create_progress_bar = NULL;
	//UF_load_library(library_name, "?MT_create_progress_bar@@YA_NH_NPEAJ@Z", (UF_load_f_p_t *)&MT_create_progress_bar);
	//typedef void(*set_progress_pos)(long *v1, int v2);//定义传参
	//set_progress_pos MT_set_progress_pos = NULL;
	//UF_load_library(library_name, "?MT_set_progress_pos@@YAXPEAJH@Z", (UF_load_f_p_t *)&MT_set_progress_pos);
	//typedef void(*show_progress_bar)(long *v1);//定义传参
	//show_progress_bar MT_show_progress_bar = NULL;
	//UF_load_library(library_name, "?MT_show_progress_bar@@YAXPEAJ@Z", (UF_load_f_p_t *)&MT_show_progress_bar);
	//typedef void(*set_progress_range)(long *v1, int v2, int v3);//定义传参
	//set_progress_range MT_set_progress_range = NULL;
	//UF_load_library(library_name, "?MT_set_progress_range@@YAXPEAJHH@Z", (UF_load_f_p_t *)&MT_set_progress_range);
	//typedef __int64(*ask_progress_cue_context)(void);//定义传参
	//ask_progress_cue_context MT_ask_progress_cue_context = NULL;
	//UF_load_library(library_name, "?MT_ask_progress_cue_context@@YAHXZ", (UF_load_f_p_t *)&MT_ask_progress_cue_context);
	//typedef __int64(*delete_progress_bar)(long *v1);//定义传参
	//delete_progress_bar MT_delete_progress_bar = NULL;
	//UF_load_library(library_name, "?MT_delete_progress_bar@@YAXPEAJ@Z", (UF_load_f_p_t *)&MT_delete_progress_bar);
	//typedef __int64(*set_progress_text)(long *v1, char *v2, bool v3);//定义传参
	//set_progress_text MT_set_progress_text = NULL;
	//UF_load_library(library_name, "?MT_set_progress_text@@YAXPEAJPEAD_N@Z", (UF_load_f_p_t *)&MT_set_progress_text);
	//if (MT_ask_progress_cue_context == NULL || MT_create_progress_bar == NULL || MT_show_progress_bar == NULL
	//	|| MT_set_progress_text == NULL || MT_set_progress_range == NULL || MT_set_progress_pos == NULL
	//	|| MT_delete_progress_bar == NULL)
	//{
	//	uc1601("进度条报错提示", 1);
	//	return;
	//}

	//__int64 cc = MT_ask_progress_cue_context();
	//long aa = 0;
	//MT_create_progress_bar(cc, true, &aa);//true和false控制是否显示百分比数字
	//MT_show_progress_bar(&aa);
	//MT_set_progress_text(&aa, "Loading", true);//此处用中文好像不显示

	//int num0=AllFaces.size()-1;
	//MT_set_progress_range(&aa, 0, num0);
	//int NowNum=0;
	//===============================DEFINE PROGRESS END===================================================================
	for (std::vector<Face*>::size_type i = 0; i != AllFaces.size(); i++)
	{
		/*MT_set_progress_pos(&aa, NowNum++);*/
		/*UF_UI_set_status("凸台复合孔特征识别");*/
		Face*EachFace;
		EachFace = AllFaces[i];

		std::vector<Edge*>EachFace_EdgeVector;
		EachFace_EdgeVector = EachFace->GetEdges();
		if (EachFace_EdgeVector.size() != 2)
		{
			continue;
		}//凸台孔种子面的边必须是两个

		Edge*RightEdge; Edge*WrongEdge;
		//这里是找寻外侧和内侧边
		if (IsCircleEdgeComplete(EachFace_EdgeVector[0]) == 0 && IsCircleEdgeComplete(EachFace_EdgeVector[1]) == 0)//两侧均为不完整圆弧
		{
			continue;
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
			}
			else
			{
				RightEdge = EachFace_EdgeVector[1];
				WrongEdge = EachFace_EdgeVector[0];
			}
		}
		else  if (IsCircleEdgeComplete(EachFace_EdgeVector[0]) == 0 && IsCircleEdgeComplete(EachFace_EdgeVector[1]) == 1)//单侧完整圆弧
		{
			RightEdge = EachFace_EdgeVector[1];
			WrongEdge = EachFace_EdgeVector[0];
		}
		else  if (IsCircleEdgeComplete(EachFace_EdgeVector[0]) == 1 && IsCircleEdgeComplete(EachFace_EdgeVector[1]) == 0)//单侧完整圆弧
		{
			RightEdge = EachFace_EdgeVector[0];
			WrongEdge = EachFace_EdgeVector[1];
		}

		Face *InsideFace; Face *OutsideFace;//种子面的内外邻面
		GetOneAdjacentFace(WrongEdge, EachFace, InsideFace);
		GetOneAdjacentFace(RightEdge, EachFace, OutsideFace);

		if (InsideFace->SolidFaceType() == Face::FaceTypeConical)//有一种特殊的中间没有平面，直接是锥面,此种孔判断Inside和OutsideFace也相反
		{
			Face*MidFace;
			Get_Next_Face(InsideFace, EachFace, MidFace);
			if (MidFace != InsideFace)
			{
				if (MidFace->SolidFaceType() == Face::FaceTypeCylindrical)
				{
					int type = 0, type1 = 0;       //判断面类型
					int norm_dir = 0, norm_dir1 = 0;   //-1=外R，0=内R
					double point[3], point1[3];  //轴中心 点
					double dir[3], dir1[3];    //轴方向
					double box[6], box1[6];    //极点坐标
					double radius, radius1;    //半径 圆锥=最小半径 + 最大半径 /2
					double rad_data, rad_data1;
					double Opoint[3]; Opoint[0] = 0.0; Opoint[1] = 0.0; Opoint[2] = 0.0;
					double distance1, distance2;
					UF_MODL_ask_face_data(EachFace->Tag(), &type, point, dir, box, &radius, &rad_data, &norm_dir);
					UF_MODL_ask_face_data(MidFace->Tag(), &type1, point1, dir1, box1, &radius1, &rad_data1, &norm_dir1);
					if (radius>radius1)
					{
						//UF_DISP_set_highlight(EachFace->Tag(),1);
						SealFace_BossInsideWideCombineHole.push_back(EachFace);
					}
				}
			}

		}

		if (OutsideFace->SolidFaceType() != Face::FaceTypePlanar)//外侧邻面必须是平面
		{
			continue;
		}


		Face*MidFace;
		Get_Next_Face(OutsideFace, EachFace, MidFace);
		if (MidFace == OutsideFace){ continue; }


		if (MidFace->SolidFaceType() == Face::FaceTypeCylindrical)//说明是双侧面无倒角孔候选面11.3***********************************************************问题很大，需要重新做
		{
			int type = 0, type1 = 0;       //判断面类型
			int norm_dir = 0, norm_dir1 = 0;   //-1=外R，0=内R
			double point[3], point1[3];  //轴中心 点
			double dir[3], dir1[3];    //轴方向
			double box[6], box1[6];    //极点坐标
			double radius, radius1;    //半径 圆锥=最小半径 + 最大半径 /2
			double rad_data, rad_data1;
			double Opoint[3]; Opoint[0] = 0.0; Opoint[1] = 0.0; Opoint[2] = 0.0;
			double distance1, distance2;
			UF_MODL_ask_face_data(EachFace->Tag(), &type, point, dir, box, &radius, &rad_data, &norm_dir);
			UF_MODL_ask_face_data(MidFace->Tag(), &type1, point1, dir1, box1, &radius1, &rad_data1, &norm_dir1);
			//UF_VEC3_distance(Opoint,point,&distance1);
			//UF_VEC3_distance(Opoint,point1,&distance2);
			if (norm_dir != 1 && norm_dir1 != 1)
			{
				//if(distance1>distance2)//说明EachFace在外侧
				//{
				if (radius<radius1)
				{
					SealFace_BossOutsideWideCombineHole.push_back(EachFace);
					//UF_DISP_set_highlight(EachFace->Tag(),1);
				}
			}
		}
		else if (MidFace->SolidFaceType() == Face::FaceTypeSurfaceOfRevolution)//说明是多层孔或者中间倒圆角复合孔候选面
		{

			Face *NextFace;
			Get_Next_Face(MidFace, OutsideFace, NextFace);
			if (NextFace == MidFace){ continue; }

			if (NextFace->SolidFaceType() == Face::FaceTypeConical)
			{
				//UF_DISP_set_highlight(EachFace->Tag(),1);
				SealFace_BossManyLayerHole.push_back(EachFace);
			}
			else if (NextFace->SolidFaceType() == Face::FaceTypeCylindrical)
			{
				//UF_DISP_set_highlight(EachFace->Tag(),1);
				SealFace_BossMidCricleCombineHole.push_back(EachFace);
			}
		}
	}

	

	RemoveFaceVectorInFaceVector(SealFace_BossManyLayerHole, AllFaces);
	RemoveFaceVectorInFaceVector(SealFace_BossOutsideWideCombineHole, AllFaces);
	RemoveFaceVectorInFaceVector(SealFace_BossInsideWideCombineHole, AllFaces);
	RemoveFaceVectorInFaceVector(SealFace_BossMidCricleCombineHole, AllFaces);
	for (std::vector<Face*>::size_type i = 0; i != AllFaces.size(); i++)
	{
		LeftSealHole_FaceVector.push_back(AllFaces[i]);
	}

	//MT_delete_progress_bar(&aa);//删除进度条
	//UF_unload_library(library_name);
}

void CreatOtherHoleFeature(std::vector<NXOpen::Face*>&SealFace_otherHole1,
	std::vector<NXOpen::Face*>&SealFace_otherHole2,
	std::vector<NXOpen::Face*>&SealFace_otherHole3,
	std::vector<Other1Hole*>&Feature_otherHole1,
	std::vector<Other2Hole*>&Feature_otherHole2,
	std::vector<Other3Hole*>&Feature_otherHole3)
{

	for (std::vector<Face*>::size_type i = 0; i != SealFace_otherHole1.size(); i++)
	{
		Face*EachFace;
		EachFace = SealFace_otherHole1[i];

		std::vector<Edge*>EachFace_EdgeVector;
		EachFace_EdgeVector = EachFace->GetEdges();

		/*Edge*RightEdge; Edge*WrongEdge;*/
		Face *Face1;
		Face *Face2;


		GetOneAdjacentFace(EachFace_EdgeVector[0], EachFace, Face1);
		GetOneAdjacentFace(EachFace_EdgeVector[1], EachFace, Face2);

		Other1Hole *Feature = new Other1Hole;
		Feature->OuterVector.push_back(Face1);
		Feature->InterVector.push_back(Face2);
		Feature->InWallVector.push_back(EachFace);
		Feature_otherHole1.push_back(Feature);


		//double r1 = 0;
		//double r2 = 0;


		//if (Face1->SolidFaceType() == Face::FaceTypeConical || Face1->SolidFaceType() == Face::FaceTypeCylindrical ||
		//	Face1->SolidFaceType() == Face::FaceTypeSurfaceOfRevolution ||
		//	Face1->SolidFaceType() == Face::FaceTypeParametric || Face1->SolidFaceType() == Face::FaceTypeBlending)
		//{			
		//	get_face_maxRad(Face1, r1);
		//}

		//if (Face2->SolidFaceType() == Face::FaceTypeConical || Face2->SolidFaceType() == Face::FaceTypeCylindrical ||
		//	Face2->SolidFaceType() == Face::FaceTypeSurfaceOfRevolution ||
		//	Face2->SolidFaceType() == Face::FaceTypeParametric || Face2->SolidFaceType() == Face::FaceTypeBlending)
		//{
		//	get_face_maxRad(Face2, r2);
		//}

		//if (r1 == 0 || r2 == 0) { continue; }

		//Other1Hole *Feature = new Other1Hole;

		//if (r1 > r2) //face1 是外
		//{
		//	Feature->OuterVector.push_back(Face1);
		//	Feature->InterVector.push_back(Face2);
		//	Feature->InWallVector.push_back(EachFace);
		//	Feature_otherHole1.push_back(Feature);
		//}

		//else
		//{
		//	Feature->OuterVector.push_back(Face2);
		//	Feature->InterVector.push_back(Face1);
		//	Feature->InWallVector.push_back(EachFace);
		//	Feature_otherHole1.push_back(Feature);
		//}
		
	}

	for (std::vector<Face*>::size_type i = 0; i != SealFace_otherHole2.size(); i++)
	{
		Face*EachFace;
		EachFace = SealFace_otherHole2[i];

		/*Edge*RightEdge; Edge*WrongEdge;
		if (Get_RightAndWrong_Edge_FromCylinder(EachFace, RightEdge, WrongEdge) == 0)
		{
			continue;
		}*/

		std::vector<Edge*>EachFace_EdgeVector;
		EachFace_EdgeVector = EachFace->GetEdges();

		Face *TopFace;
		Face *BottomFace;

		GetOneAdjacentFace(EachFace_EdgeVector[0], EachFace, TopFace);
		GetOneAdjacentFace(EachFace_EdgeVector[1], EachFace, BottomFace);

		Other2Hole *Feature = new Other2Hole;
		Feature->OuterVector.push_back(TopFace);
		Feature->InWallVector.push_back(EachFace);
		Feature->InterVector.push_back(BottomFace);
		Feature_otherHole2.push_back(Feature);
	}

	for (std::vector<Face*>::size_type i = 0; i != SealFace_otherHole3.size(); i++)
	{
		/*uc1601("进入1", 1);*/
		Face*EachFace;
		EachFace = SealFace_otherHole3[i];

		Face *TopFace;

		std::vector<Edge*>EachFace_EdgeVector;
		EachFace_EdgeVector = EachFace->GetEdges();

		if (EachFace_EdgeVector.size() == 2)
		{
			Edge*RightEdge; Edge*WrongEdge;
			if (Get_RightAndWrong_Edge_FromCylinder(EachFace, RightEdge, WrongEdge) == 0)
			{
				continue;
			}
			GetOneAdjacentFace(RightEdge, EachFace, TopFace);

			if (TopFace->SolidFaceType() == Face::FaceTypePlanar)
			{
				Other3Hole *Feature = new Other3Hole;
				Feature->OuterVector.push_back(TopFace);
				Feature->InWallVector.push_back(EachFace);
				Feature_otherHole3.push_back(Feature);
			}
			else
			{
				Face*RightFace;
				Get_Next_Face(TopFace, EachFace, RightFace);
				Other3Hole *Feature = new Other3Hole;
				Feature->OuterVector.push_back(RightFace);
				Feature->InWallVector.push_back(EachFace);
				Feature_otherHole3.push_back(Feature);
			}

		}

		else if (EachFace_EdgeVector.size() > 2 && EachFace_EdgeVector.size() <= 42)
		{
			for (std::vector<Edge*>::size_type i = 0; i != EachFace_EdgeVector.size(); i++)
			{
				if (IsCircleEdgeComplete(EachFace_EdgeVector[i]) == 1)
				{
					/*uc1601("进入if", 1);*/
					GetOneAdjacentFace(EachFace_EdgeVector[i], EachFace, TopFace);

					if (TopFace->SolidFaceType() == Face::FaceTypePlanar)
					{
						Other3Hole *Feature = new Other3Hole;
						Feature->OuterVector.push_back(TopFace);
						Feature->InWallVector.push_back(EachFace);
						Feature_otherHole3.push_back(Feature);
					}
					
					else
					{
						Face*RightFace;
						Get_Next_Face(TopFace, EachFace, RightFace);
						Other3Hole *Feature = new Other3Hole;
						Feature->OuterVector.push_back(RightFace);
						Feature->InWallVector.push_back(EachFace);
						Feature_otherHole3.push_back(Feature);
					}
				}
			}
		}
	}
	/*char message[13];
	UF_UI_open_listing_window();
	UF_UI_write_listing_window("调试模板:\n");

	
	sprintf(message, "%d\n", Feature_otherHole3.size());
	UF_UI_write_listing_window(message);
	sprintf(message, "%d\n", Feature_otherHole2.size());
	UF_UI_write_listing_window(message);
	sprintf(message, "%d\n", Feature_otherHole1.size());
	UF_UI_write_listing_window(message);*/

}

//构建凸台孔特征
void CreateBossHoleFeature(std::vector<NXOpen::Face*>SealFace_BossNormalHole,//凸台通孔
	std::vector<NXOpen::Face*>SealFace_BossOneRightHole,//凸台单侧直角通孔
	std::vector<NXOpen::Face*>SealFace_BossOneCircleHole,//凸台单侧圆角通孔
	std::vector<NXOpen::Face*>SealFace_BossDoubleRightHole,//凸台双侧直角通孔
	std::vector<NXOpen::Face*>SealFace_BossDoubleCircleHole,//凸台双侧圆角通孔
	std::vector<NXOpen::Face*>SealFace_BossOneRightOneCircleHole,//凸台单侧直角单侧圆角通孔
	std::vector<NXOpen::Face*>SealFace_BossNormalBlindHole,//凸台无倒角盲孔);
	std::vector<NXOpen::Face*>SealFace_BossRightBlindHole,//凸台倒斜角盲孔))
	std::vector<NXOpen::Face*>SealFace_BossManyLayerHole,
	std::vector<NXOpen::Face*>SealFace_BossOutsideWideCombineHole,
	std::vector<NXOpen::Face*>SealFace_BossInsideWideCombineHole,
	std::vector<NXOpen::Face*>SealFace_BossMidCricleCombineHole,
	std::vector<BossNormalHole*>&Feature_BossNormalHole,
	std::vector<BossOneRightHole*>&Feature_BossOneRightHole,
	std::vector<BossOneCircleHole*>&Feature_BossOneCircleHole,
	std::vector<BossDoubleRightHole*>&Feature_BossDoubleRightHole,
	std::vector<BossDoubleCircleHole*>&Feature_BossDoubleCircleHole,
	std::vector<BossOneRightOneCircleHole*>&Feature_BossOneRightOneCircleHole,
	std::vector<BossManyLayerHole*>&Feature_BossManyLayerHole,
	std::vector<BossOutsideWideCombineHole*>&Feature_BossOutsideWideCombineHole,
	std::vector<BossInsideWideCombineHole*>&Feature_BossInsideWideCombineHole,
	std::vector<BossMidCricleCombineHole*>&Feature_BossMidCricleCombineHole,
	std::vector<BossNormalBlindHole*>&Feature_BossNormalBlindHole,
	std::vector<BossRightBlindHole*>&Feature_BossRightBlindHole)
{
	//===============================DEFINE PROGRESS Begin===================================================================
	//int num0=SealFace_BossNromalHole.size()+SealFace_BossOneRightHole.size()+SealFace_BossOneCircleHole.size()+SealFace_BossDoubleRightHole.size()+
	//	SealFace_BossDoubleCircleHole.size()+SealFace_BossOneRightOneCircleHole.size()+SealFace_BossNormalBlindHole.size()+SealFace_BossRightBlindHole.size()+
	//	SealFace_BossManyLayerHole.size()+SealFace_BossOutsideWideCombineHole.size()+SealFace_BossInsideWideCombineHole.size()+SealFace_BossMidCricleCombineHole.size()-12;
	//char *library_name = "D:\\ug10\\UGII\\libdisp.dll";
	//typedef void(*create_progress_bar)(int v1, bool v2, long *v3);//定义传参
	//create_progress_bar MT_create_progress_bar = NULL;
	//UF_load_library(library_name, "?MT_create_progress_bar@@YA_NH_NPEAJ@Z", (UF_load_f_p_t *)&MT_create_progress_bar);
	//typedef void(*set_progress_pos)(long *v1, int v2);//定义传参
	//set_progress_pos MT_set_progress_pos = NULL;
	//UF_load_library(library_name, "?MT_set_progress_pos@@YAXPEAJH@Z", (UF_load_f_p_t *)&MT_set_progress_pos);
	//typedef void(*show_progress_bar)(long *v1);//定义传参
	//show_progress_bar MT_show_progress_bar = NULL;
	//UF_load_library(library_name, "?MT_show_progress_bar@@YAXPEAJ@Z", (UF_load_f_p_t *)&MT_show_progress_bar);
	//typedef void(*set_progress_range)(long *v1, int v2, int v3);//定义传参
	//set_progress_range MT_set_progress_range = NULL;
	//UF_load_library(library_name, "?MT_set_progress_range@@YAXPEAJHH@Z", (UF_load_f_p_t *)&MT_set_progress_range);
	//typedef __int64(*ask_progress_cue_context)(void);//定义传参
	//ask_progress_cue_context MT_ask_progress_cue_context = NULL;
	//UF_load_library(library_name, "?MT_ask_progress_cue_context@@YAHXZ", (UF_load_f_p_t *)&MT_ask_progress_cue_context);
	//typedef __int64(*delete_progress_bar)(long *v1);//定义传参
	//delete_progress_bar MT_delete_progress_bar = NULL;
	//UF_load_library(library_name, "?MT_delete_progress_bar@@YAXPEAJ@Z", (UF_load_f_p_t *)&MT_delete_progress_bar);
	//typedef __int64(*set_progress_text)(long *v1, char *v2, bool v3);//定义传参
	//set_progress_text MT_set_progress_text = NULL;
	//UF_load_library(library_name, "?MT_set_progress_text@@YAXPEAJPEAD_N@Z", (UF_load_f_p_t *)&MT_set_progress_text);
	//if (MT_ask_progress_cue_context == NULL || MT_create_progress_bar == NULL || MT_show_progress_bar == NULL
	//	|| MT_set_progress_text == NULL || MT_set_progress_range == NULL || MT_set_progress_pos == NULL
	//	|| MT_delete_progress_bar == NULL)
	//{
	//	uc1601("进度条报错提示", 1);
	//	return;
	//}
	//__int64 cc = MT_ask_progress_cue_context();
	//long aa = 0;
	//MT_create_progress_bar(cc, true, &aa);//true和false控制是否显示百分比数字
	//MT_show_progress_bar(&aa);
	//MT_set_progress_text(&aa, "Loading", true);//此处用中文好像不显示

	//MT_set_progress_range(&aa, 0, num0);
	//int NowNum=0;
	//===============================DEFINE PROGRESS END===================================================================
	//凸台通孔 test ok
	for (std::vector<Face*>::size_type i = 0; i != SealFace_BossNormalHole.size(); i++)
	{
		/*MT_set_progress_pos(&aa, NowNum++);
		UF_UI_set_status("孔特征");*/
		Face*EachFace;
		EachFace = SealFace_BossNormalHole[i];
		Edge*RightEdge; Edge*WrongEdge;
		if (Get_RightAndWrong_Edge_FromCylinder(EachFace, RightEdge, WrongEdge) == 0)
		{
			continue;
		}

		Face *TopFace;
		Face *BottomFace;
		GetOneAdjacentFace(RightEdge, EachFace, TopFace);
		GetOneAdjacentFace(WrongEdge, EachFace, BottomFace);

		BossNormalHole *Feature = new BossNormalHole;
		Feature->TopFaceVector.push_back(TopFace);
		Feature->InWallVector.push_back(EachFace);
		Feature->BottomFaceVector.push_back(BottomFace);
		Feature_BossNormalHole.push_back(Feature);

	}

	//凸台单侧直角通孔 test ok
	for (std::vector<Face*>::size_type i = 0; i != SealFace_BossOneRightHole.size(); i++)
	{

		//MT_set_progress_pos(&aa, NowNum++);
		Face*EachFace;
		EachFace = SealFace_BossOneRightHole[i];
		Edge*RightEdge; Edge*WrongEdge;
		if (Get_RightAndWrong_Edge_FromCylinder(EachFace, RightEdge, WrongEdge) == 0)
		{
			continue;
		}
		Face *TopFace;
		Face *RightFace;
		Face *BottomFace;
		GetOneAdjacentFace(RightEdge, EachFace, RightFace);
		GetOneAdjacentFace(WrongEdge, EachFace, BottomFace);
		Get_Next_Face(RightFace, EachFace, TopFace);
		if (TopFace == RightFace)
		{
			continue;
		}

		BossOneRightHole *Feature = new BossOneRightHole;
		Feature->TopFaceVector.push_back(TopFace);
		Feature->RightVector.push_back(RightFace);
		Feature->InWallVector.push_back(EachFace);
		Feature->BottomFaceVector.push_back(BottomFace);
		Feature_BossOneRightHole.push_back(Feature);

	}

	//凸台双侧斜角通孔
	for (std::vector<Face*>::size_type i = 0; i != SealFace_BossDoubleRightHole.size(); i++)
	{

		//MT_set_progress_pos(&aa, NowNum++);
		Face*EachFace;
		EachFace = SealFace_BossDoubleRightHole[i];
		Edge*RightEdge; Edge*WrongEdge;
		if (Get_RightAndWrong_Edge_FromCylinder(EachFace, RightEdge, WrongEdge) == 0)
		{
			continue;
		}
		Face *TopFace;
		Face *TopRightFace;
		Face *BottomRightFace;
		Face *BottomFace;
		GetOneAdjacentFace(RightEdge, EachFace, TopRightFace);
		GetOneAdjacentFace(WrongEdge, EachFace, BottomRightFace);
		Get_Next_Face(TopRightFace, EachFace, TopFace);
		if (TopFace == TopRightFace){ continue; }
		Get_Next_Face(BottomRightFace, EachFace, BottomFace);
		if (BottomFace == BottomRightFace){ continue; }

		BossDoubleRightHole *Feature = new BossDoubleRightHole;
		Feature->TopFaceVector.push_back(TopFace);
		Feature->TopRightVector.push_back(TopRightFace);
		Feature->InWallVector.push_back(EachFace);
		Feature->BottomRightVector.push_back(BottomRightFace);
		Feature->BottomFaceVector.push_back(BottomFace);
		Feature_BossDoubleRightHole.push_back(Feature);
	}

	//凸台双侧圆角通孔
	for (std::vector<Face*>::size_type i = 0; i != SealFace_BossDoubleCircleHole.size(); i++)
	{

		//MT_set_progress_pos(&aa, NowNum++);
		Face*EachFace;
		EachFace = SealFace_BossDoubleCircleHole[i];
		Edge*RightEdge; Edge*WrongEdge;
		if (Get_RightAndWrong_Edge_FromCylinder(EachFace, RightEdge, WrongEdge) == 0)
		{
			continue;
		}
		Face *TopFace;
		Face *TopCircleFace;
		Face *BottomCircleFace;
		Face *BottomFace;
		GetOneAdjacentFace(RightEdge, EachFace, TopCircleFace);
		GetOneAdjacentFace(WrongEdge, EachFace, BottomCircleFace);
		Get_Next_Face(TopCircleFace, EachFace, TopFace);
		if (TopFace == TopCircleFace){ continue; }
		Get_Next_Face(BottomCircleFace, EachFace, BottomFace);
		if (BottomFace == BottomCircleFace){ continue; }

		BossDoubleCircleHole *Feature = new BossDoubleCircleHole;
		Feature->TopFaceVector.push_back(TopFace);
		Feature->TopCircleVector.push_back(TopCircleFace);
		Feature->InWallVector.push_back(EachFace);
		Feature->BottomCircleVector.push_back(BottomCircleFace);
		Feature->BottomFaceVector.push_back(BottomFace);
		Feature_BossDoubleCircleHole.push_back(Feature);
	}

	//凸台单侧圆角单侧直角通孔
	for (std::vector<Face*>::size_type i = 0; i != SealFace_BossOneRightOneCircleHole.size(); i++)
	{

		//MT_set_progress_pos(&aa, NowNum++);
		Face*EachFace;
		EachFace = SealFace_BossOneRightOneCircleHole[i];
		Edge*RightEdge; Edge*WrongEdge;
		if (Get_RightAndWrong_Edge_FromCylinder(EachFace, RightEdge, WrongEdge) == 0)
		{
			continue;
		}
		Face *TopFace;
		Face *TopRightFace;
		Face *BottomCircleFace;
		Face *BottomFace;
		GetOneAdjacentFace(RightEdge, EachFace, TopRightFace);
		GetOneAdjacentFace(WrongEdge, EachFace, BottomCircleFace);
		Get_Next_Face(TopRightFace, EachFace, TopFace);
		if (TopFace == TopRightFace){ continue; }
		Get_Next_Face(BottomCircleFace, EachFace, BottomFace);
		if (BottomFace == BottomCircleFace){ continue; }


		BossOneRightOneCircleHole *Feature = new BossOneRightOneCircleHole;
		Feature->TopFaceVector.push_back(TopFace);
		Feature->RightVector.push_back(TopRightFace);
		Feature->InWallVector.push_back(EachFace);
		Feature->CircleVector.push_back(BottomCircleFace);
		Feature->BottomFaceVector.push_back(BottomFace);
		Feature_BossOneRightOneCircleHole.push_back(Feature);
	}

	//凸台多层通孔
	for (std::vector<Face*>::size_type i = 0; i != SealFace_BossManyLayerHole.size(); i++)
	{

		//MT_set_progress_pos(&aa, NowNum++);

		Face*EachFace;
		EachFace = SealFace_BossManyLayerHole[i];
		Edge*RightEdge; Edge*WrongEdge;
		if (Get_RightAndWrong_Edge_FromCylinder(EachFace, RightEdge, WrongEdge) == 0)
		{
			continue;
		}
		Face *TopFace;
		Face *RightFace;
		Face *CircleFace;
		Face *PlanerFace;
		Face *BottomFace;
		GetOneAdjacentFace(RightEdge, EachFace, PlanerFace);
		GetOneAdjacentFace(WrongEdge, EachFace, BottomFace);
		Get_Next_Face(PlanerFace, EachFace, CircleFace);
		if (PlanerFace == CircleFace){ continue; }
		Get_Next_Face(CircleFace, PlanerFace, RightFace);
		if (CircleFace == RightFace){ continue; }
		Get_Next_Face(RightFace, CircleFace, TopFace);
		if (RightFace == TopFace){ continue; }

		BossManyLayerHole *Feature = new BossManyLayerHole;
		Feature->TopFaceVector.push_back(TopFace);
		Feature->PlanerVector.push_back(PlanerFace);
		Feature->RightVector.push_back(RightFace);
		Feature->InWallVector.push_back(EachFace);
		Feature->CircleVector.push_back(CircleFace);
		Feature->BottomFaceVector.push_back(BottomFace);
		Feature_BossManyLayerHole.push_back(Feature);
	}

	//凸台外宽内窄复合通孔
	for (std::vector<Face*>::size_type i = 0; i != SealFace_BossOutsideWideCombineHole.size(); i++)
	{

		//MT_set_progress_pos(&aa, NowNum++);

		Face*EachFace;
		EachFace = SealFace_BossOutsideWideCombineHole[i];
		Edge*RightEdge; Edge*WrongEdge;
		if (Get_RightAndWrong_Edge_FromCylinder(EachFace, RightEdge, WrongEdge) == 0)
		{
			continue;
		}
		Face *TopFace;
		Face *TopWall;
		Face *PlanerFace;
		Face *BottomFace;
		GetOneAdjacentFace(RightEdge, EachFace, PlanerFace);
		GetOneAdjacentFace(WrongEdge, EachFace, BottomFace);
		Get_Next_Face(PlanerFace, EachFace, TopWall);
		if (PlanerFace == TopWall){ continue; }
		Get_Next_Face(TopWall, PlanerFace, TopFace);
		if (TopFace == TopWall){ continue; }

		BossOutsideWideCombineHole *Feature = new BossOutsideWideCombineHole;
		Feature->TopFaceVector.push_back(TopFace);
		Feature->PlanerVector.push_back(PlanerFace);
		Feature->BottomWallVector.push_back(EachFace);
		Feature->TopWallVector.push_back(TopWall);
		Feature->BottomFaceVector.push_back(BottomFace);
		Feature_BossOutsideWideCombineHole.push_back(Feature);
	}

	//凸台内宽外窄复合通孔  ////////这两种孔的区分问题太大，回头再做11.2*******************************************************************************************************************
	for (std::vector<Face*>::size_type i = 0; i != SealFace_BossInsideWideCombineHole.size(); i++)
	{

		//MT_set_progress_pos(&aa, NowNum++);

		Face*EachFace;
		EachFace = SealFace_BossInsideWideCombineHole[i];
		Edge*RightEdge; Edge*WrongEdge;
		if (Get_RightAndWrong_Edge_FromCylinder(EachFace, WrongEdge, RightEdge) == 0)//唯一的内外相反的
		{
			continue;
		}
		Face *TopFace;
		Face *TopWall;
		Face *PlanerFace;
		Face *BottomFace;
		GetOneAdjacentFace(RightEdge, EachFace, PlanerFace);
		GetOneAdjacentFace(WrongEdge, EachFace, BottomFace);
		Get_Next_Face(PlanerFace, EachFace, TopWall);
		if (PlanerFace == TopWall){ continue; }
		Get_Next_Face(TopWall, PlanerFace, TopFace);
		if (TopFace == TopWall){ continue; }

		BossInsideWideCombineHole *Feature = new BossInsideWideCombineHole;
		Feature->TopFaceVector.push_back(TopFace);
		Feature->PlanerVector.push_back(PlanerFace);
		Feature->BottomWallVector.push_back(EachFace);
		Feature->TopWallVector.push_back(TopWall);
		Feature->BottomFaceVector.push_back(BottomFace);
		Feature_BossInsideWideCombineHole.push_back(Feature);
	}

	//凸台中圆角复合通孔
	for (std::vector<Face*>::size_type i = 0; i != SealFace_BossMidCricleCombineHole.size(); i++)
	{
		//MT_set_progress_pos(&aa, NowNum++);

		Face*EachFace;
		EachFace = SealFace_BossMidCricleCombineHole[i];
		Edge*RightEdge; Edge*WrongEdge;
		if (Get_RightAndWrong_Edge_FromCylinder(EachFace, RightEdge, WrongEdge) == 0)
		{
			continue;
		}
		Face *TopFace;
		Face *TopWall;
		Face *PlanerFace;
		Face *CircleFace;
		Face *BottomFace;
		GetOneAdjacentFace(RightEdge, EachFace, PlanerFace);
		GetOneAdjacentFace(WrongEdge, EachFace, BottomFace);
		Get_Next_Face(PlanerFace, EachFace, CircleFace);
		if (PlanerFace == CircleFace){ continue; }
		Get_Next_Face(CircleFace, PlanerFace, TopWall);
		if (CircleFace == TopWall){ continue; }
		Get_Next_Face(TopWall, CircleFace, TopFace);
		if (TopFace == TopWall){ continue; }

		BossMidCricleCombineHole *Feature = new BossMidCricleCombineHole;
		Feature->TopFaceVector.push_back(TopFace);
		Feature->PlanerVector.push_back(PlanerFace);
		Feature->BottomWallVector.push_back(EachFace);
		Feature->CircleVector.push_back(CircleFace);
		Feature->TopWallVector.push_back(TopWall);
		Feature->BottomFaceVector.push_back(BottomFace);
		Feature_BossMidCricleCombineHole.push_back(Feature);
	}

	//凸台无倒角盲孔
	for (std::vector<Face*>::size_type i = 0; i != SealFace_BossNormalBlindHole.size(); i++)
	{

		//MT_set_progress_pos(&aa, NowNum++);
		Face*EachFace;
		EachFace = SealFace_BossNormalBlindHole[i];
		Edge*RightEdge; Edge*WrongEdge;
		if (Get_RightAndWrong_Edge_FromCylinder(EachFace, RightEdge, WrongEdge) == 0)
		{
			continue;
		}
		Face *TopFace;
		Face *BottomFace;
		GetOneAdjacentFace(RightEdge, EachFace, TopFace);
		GetOneAdjacentFace(WrongEdge, EachFace, BottomFace);

		BossNormalBlindHole *Feature = new BossNormalBlindHole;
		Feature->TopFaceVector.push_back(TopFace);
		Feature->InWallVector.push_back(EachFace);
		Feature->BottomFaceVector.push_back(BottomFace);
		Feature_BossNormalBlindHole.push_back(Feature);
	}

	//凸台倒斜角盲孔
	for (std::vector<Face*>::size_type i = 0; i != SealFace_BossRightBlindHole.size(); i++)
	{

		//MT_set_progress_pos(&aa, NowNum++);

		Face*EachFace;
		EachFace = SealFace_BossRightBlindHole[i];
		Edge*RightEdge; Edge*WrongEdge;
		if (Get_RightAndWrong_Edge_FromCylinder(EachFace, RightEdge, WrongEdge) == 0)
		{
			continue;
		}
		Face *TopFace;
		Face *RightFace;
		Face *BottomFace;
		GetOneAdjacentFace(RightEdge, EachFace, RightFace);
		GetOneAdjacentFace(WrongEdge, EachFace, BottomFace);
		Get_Next_Face(RightFace, EachFace, TopFace);
		if (RightFace == TopFace){ continue; }

		BossRightBlindHole *Feature = new BossRightBlindHole;
		Feature->TopFaceVector.push_back(TopFace);
		Feature->RightVector.push_back(RightFace);
		Feature->InWallVector.push_back(EachFace);
		Feature->BottomFaceVector.push_back(BottomFace);
		Feature_BossRightBlindHole.push_back(Feature);
	}


	//MT_delete_progress_bar(&aa);//删除进度条
	//UF_unload_library(library_name);
}

//环形槽种子面
void CircularGrooveRecognition(std::vector<NXOpen::Face*>AllFaces, Face *Begin_Face, NXOpen::Part*workPart, std::vector<NXOpen::Face*>&SealFace_CircularGroove0, std::vector<NXOpen::Face*>&SealFace_CircularGroove1, Face *_ApproachFace)
{
	//===============================DEFINE PROGRESS Begin===================================================================
	//char *library_name = "D:\\ug10\\UGII\\libdisp.dll";
	//typedef void(*create_progress_bar)(int v1, bool v2, long *v3);//定义传参
	//create_progress_bar MT_create_progress_bar = NULL;
	//UF_load_library(library_name, "?MT_create_progress_bar@@YA_NH_NPEAJ@Z", (UF_load_f_p_t *)&MT_create_progress_bar);
	//typedef void(*set_progress_pos)(long *v1, int v2);//定义传参
	//set_progress_pos MT_set_progress_pos = NULL;
	//UF_load_library(library_name, "?MT_set_progress_pos@@YAXPEAJH@Z", (UF_load_f_p_t *)&MT_set_progress_pos);
	//typedef void(*show_progress_bar)(long *v1);//定义传参
	//show_progress_bar MT_show_progress_bar = NULL;
	//UF_load_library(library_name, "?MT_show_progress_bar@@YAXPEAJ@Z", (UF_load_f_p_t *)&MT_show_progress_bar);
	//typedef void(*set_progress_range)(long *v1, int v2, int v3);//定义传参
	//set_progress_range MT_set_progress_range = NULL;
	//UF_load_library(library_name, "?MT_set_progress_range@@YAXPEAJHH@Z", (UF_load_f_p_t *)&MT_set_progress_range);
	//typedef __int64(*ask_progress_cue_context)(void);//定义传参
	//ask_progress_cue_context MT_ask_progress_cue_context = NULL;
	//UF_load_library(library_name, "?MT_ask_progress_cue_context@@YAHXZ", (UF_load_f_p_t *)&MT_ask_progress_cue_context);
	//typedef __int64(*delete_progress_bar)(long *v1);//定义传参
	//delete_progress_bar MT_delete_progress_bar = NULL;
	//UF_load_library(library_name, "?MT_delete_progress_bar@@YAXPEAJ@Z", (UF_load_f_p_t *)&MT_delete_progress_bar);
	//typedef __int64(*set_progress_text)(long *v1, char *v2, bool v3);//定义传参
	//set_progress_text MT_set_progress_text = NULL;
	//UF_load_library(library_name, "?MT_set_progress_text@@YAXPEAJPEAD_N@Z", (UF_load_f_p_t *)&MT_set_progress_text);
	//if (MT_ask_progress_cue_context == NULL || MT_create_progress_bar == NULL || MT_show_progress_bar == NULL
	//	|| MT_set_progress_text == NULL || MT_set_progress_range == NULL || MT_set_progress_pos == NULL
	//	|| MT_delete_progress_bar == NULL)
	//{
	//	uc1601("进度条报错提示", 1);
	//	return;
	//}

	//__int64 cc = MT_ask_progress_cue_context();
	//long aa = 0;
	//MT_create_progress_bar(cc, true, &aa);//true和false控制是否显示百分比数字
	//MT_show_progress_bar(&aa);
	//MT_set_progress_text(&aa, "Loading", true);//此处用中文好像不显示

	//int num0=AllFaces.size()-1;
	//MT_set_progress_range(&aa, 0, num0);
	//int NowNum=0;
	//===============================DEFINE PROGRESS END===================================================================
	Vector3d _Approach_Normal;
	get_MainNormal_fr_plane(Begin_Face, _Approach_Normal);
	for (std::vector<Face*>::size_type i = 0; i != AllFaces.size(); i++)
	{
		/*MT_set_progress_pos(&aa, NowNum++);*/
		UF_UI_set_status("环形槽特征识别");
		Face * EachFace = AllFaces[i];

		if (EachFace->SolidFaceType() == Face::FaceTypeCylindrical || EachFace->SolidFaceType() == Face::FaceTypeConical
			|| EachFace->SolidFaceType() == Face::FaceTypeSurfaceOfRevolution)
		{

			int type = 0;       //判断面类型
			int norm_dir = 0;
			double point[3];  //轴中心 点
			double dir[3];    //轴方向
			double box[6];    //极点坐标
			double radius;    //半径 圆锥=最小半径 + 最大半径 /2
			double rad_data;

			UF_MODL_ask_face_data(EachFace->Tag(), &type, point, dir, box, &radius, &rad_data, &norm_dir);

			if (norm_dir == +1)
			{
				std::vector<Edge*>EachFaceEdges;
				EachFaceEdges = EachFace->GetEdges();
				for (std::vector<Edge*>::size_type j = 0; j != EachFaceEdges.size(); j++)
				{
					Edge *EachEdge;
					EachEdge = EachFaceEdges[j];
					if (EachEdge->SolidEdgeType() == Edge::EdgeTypeCircular)
					{
						double EdgeLength = EachEdge->GetLength();
						if (EdgeLength>500)
						{
							double area = calcArea(workPart, EachFace);
							if (area > 5000)
							{
								if (EachFace->SolidFaceType() == Face::FaceTypeSurfaceOfRevolution)
								{
									int num = Get_FullCircle_Num(EachFace, _Approach_Normal);
									if (num < 1)
									{
										SealFace_CircularGroove1.push_back(EachFace);
										break;
									}
								}
								else
								{
									SealFace_CircularGroove1.push_back(EachFace);
									break;
								}
							}
						}
					}
				}
			}
			else if (norm_dir == -1)
			{
				std::vector<Edge*>EachFaceEdges;
				EachFaceEdges = EachFace->GetEdges();
				for (std::vector<Edge*>::size_type j = 0; j != EachFaceEdges.size(); j++)
				{
					Edge *EachEdge;
					EachEdge = EachFaceEdges[j];
					if (EachEdge->SolidEdgeType() == Edge::EdgeTypeCircular)
					{
						double EdgeLength = EachEdge->GetLength();
						if (EdgeLength>500)
						{
							double area = calcArea(workPart, EachFace);
							if (area > 5000)
							{
								if (EachFace->SolidFaceType() == Face::FaceTypeSurfaceOfRevolution)
								{
									int num = Get_FullCircle_Num(EachFace, _Approach_Normal);
									if (num < 1)
									{
										SealFace_CircularGroove0.push_back(EachFace);
										break;
									}
								}
								else
								{
									SealFace_CircularGroove0.push_back(EachFace);
									break;
								}
							}
						}
					}
				}
			}
		}
	}

	//MT_delete_progress_bar(&aa);//删除进度条
	//UF_unload_library(library_name);

}

void CreateCircularGrooveFeature(Face *Begin_Face, std::vector<NXOpen::Face*>SealFace_CircularGroove0, std::vector<NXOpen::Face*>SealFace_CircularGroove1, std::vector<CircularGroove*>&Feature_CircularGroove0, std::vector<CircularGroove*>&Feature_CircularGroove1)
{
	//===============================DEFINE PROGRESS Begin===================================================================
	//char *library_name = "D:\\ug10\\UGII\\libdisp.dll";
	//typedef void(*create_progress_bar)(int v1, bool v2, long *v3);//定义传参
	//create_progress_bar MT_create_progress_bar = NULL;
	//UF_load_library(library_name, "?MT_create_progress_bar@@YA_NH_NPEAJ@Z", (UF_load_f_p_t *)&MT_create_progress_bar);
	//typedef void(*set_progress_pos)(long *v1, int v2);//定义传参
	//set_progress_pos MT_set_progress_pos = NULL;
	//UF_load_library(library_name, "?MT_set_progress_pos@@YAXPEAJH@Z", (UF_load_f_p_t *)&MT_set_progress_pos);
	//typedef void(*show_progress_bar)(long *v1);//定义传参
	//show_progress_bar MT_show_progress_bar = NULL;
	//UF_load_library(library_name, "?MT_show_progress_bar@@YAXPEAJ@Z", (UF_load_f_p_t *)&MT_show_progress_bar);
	//typedef void(*set_progress_range)(long *v1, int v2, int v3);//定义传参
	//set_progress_range MT_set_progress_range = NULL;
	//UF_load_library(library_name, "?MT_set_progress_range@@YAXPEAJHH@Z", (UF_load_f_p_t *)&MT_set_progress_range);
	//typedef __int64(*ask_progress_cue_context)(void);//定义传参
	//ask_progress_cue_context MT_ask_progress_cue_context = NULL;
	//UF_load_library(library_name, "?MT_ask_progress_cue_context@@YAHXZ", (UF_load_f_p_t *)&MT_ask_progress_cue_context);
	//typedef __int64(*delete_progress_bar)(long *v1);//定义传参
	//delete_progress_bar MT_delete_progress_bar = NULL;
	//UF_load_library(library_name, "?MT_delete_progress_bar@@YAXPEAJ@Z", (UF_load_f_p_t *)&MT_delete_progress_bar);
	//typedef __int64(*set_progress_text)(long *v1, char *v2, bool v3);//定义传参
	//set_progress_text MT_set_progress_text = NULL;
	//UF_load_library(library_name, "?MT_set_progress_text@@YAXPEAJPEAD_N@Z", (UF_load_f_p_t *)&MT_set_progress_text);
	//if (MT_ask_progress_cue_context == NULL || MT_create_progress_bar == NULL || MT_show_progress_bar == NULL
	//	|| MT_set_progress_text == NULL || MT_set_progress_range == NULL || MT_set_progress_pos == NULL
	//	|| MT_delete_progress_bar == NULL)
	//{
	//	uc1601("进度条报错提示", 1);
	//	return;
	//}
	//__int64 cc = MT_ask_progress_cue_context();
	//long aa = 0;
	//MT_create_progress_bar(cc, true, &aa);//true和false控制是否显示百分比数字
	//MT_show_progress_bar(&aa);
	//MT_set_progress_text(&aa, "Loading", true);//此处用中文好像不显示
	//int num0=SealFace_CircularGroove0.size()+SealFace_CircularGroove1.size();
	//MT_set_progress_range(&aa, 0, num0);

	//==============================DEFINE PROGRESS END=================================================================
	Vector3d _Approach_Normal;
	get_MainNormal_fr_plane(Begin_Face, _Approach_Normal);
	for (std::vector<Face*>::size_type i = 0; i != SealFace_CircularGroove0.size(); i++)
	{
		Face * EachFace = SealFace_CircularGroove0[i];

		CircularGroove* Feature = new CircularGroove;
		Feature->TopFaceVector.push_back(EachFace);

		std::vector<Face*>EachAdFaces;
		get_Adjacent_Faces(EachFace, EachAdFaces);

		bool IsRightFlag = 1;
		for (std::vector<Face*>::size_type j = 0; j != EachAdFaces.size(); j++)
		{
			Face *OtherFace;
			OtherFace = EachAdFaces[j];

			/*MT_set_progress_pos(&aa, i);*/
			UF_UI_set_status("环形槽特征");
			if (OtherFace->SolidFaceType() == Face::FaceTypePlanar)
			{
				IsRightFlag = 0;
				break;
			}
			else if (OtherFace->SolidFaceType() == Face::FaceTypeConical ||
				OtherFace->SolidFaceType() == Face::FaceTypeCylindrical)
			{
				int num = Get_FullCircle_Num(OtherFace, _Approach_Normal);
				if (num > 1)
				{
					std::vector<Edge*>CorEdges;
					GetEdge_From_TwoFaces(EachFace, OtherFace, CorEdges);

					double Angle;
					get_FaceAngle(EachFace, OtherFace, CorEdges[0], Angle);
					Angle = Angle * 180 / PI;
					if (Angle>180)
					{
						IsRightFlag = 0;
						break;
					}
					else
					{
						Feature->BottomCornerFaceVector.push_back(OtherFace);
					}
				}
				else if (Is_Face_in_lpFace(OtherFace, SealFace_CircularGroove1) == -1)
				{
					Feature->LimitFaceVector.push_back(OtherFace);
				}
			}
			else if (OtherFace->SolidFaceType() == Face::FaceTypeSurfaceOfRevolution)
			{
				int num = Get_FullCircle_Num(OtherFace, _Approach_Normal);
				if (num > 0)
				{
					std::vector<Edge*>CorEdges;
					GetEdge_From_TwoFaces(EachFace, OtherFace, CorEdges);

					double Angle;
					get_FaceAngle(EachFace, OtherFace, CorEdges[0], Angle);
					Angle = Angle * 180 / PI;
					if (Angle>180)
					{
						IsRightFlag = 0;
						break;
					}
					else
					{
						Feature->BottomCornerFaceVector.push_back(OtherFace);
					}
				}
				else if (Is_Face_in_lpFace(OtherFace, SealFace_CircularGroove1) == -1)
				{
					Feature->LimitFaceVector.push_back(OtherFace);
				}
			}
			else if (OtherFace->SolidFaceType() == Face::FaceTypeBlending)
			{
				Feature->LimitFaceVector.push_back(OtherFace);
			}
		}
		if (IsRightFlag)
		{
			Feature_CircularGroove0.push_back(Feature);
		}
	}

	/*num0=SealFace_CircularGroove0.size()-1;*/

	for (std::vector<Face*>::size_type i = 0; i != SealFace_CircularGroove1.size(); i++)
	{
		Face * EachFace = SealFace_CircularGroove1[i];

		CircularGroove* Feature = new CircularGroove;
		Feature->TopFaceVector.push_back(EachFace);

		std::vector<Face*>EachAdFaces;
		get_Adjacent_Faces(EachFace, EachAdFaces);

		bool IsRightFlag = 1;
		for (std::vector<Face*>::size_type j = 0; j != EachAdFaces.size(); j++)
		{
			Face *OtherFace;
			OtherFace = EachAdFaces[j];

			/*MT_set_progress_pos(&aa, i);*/
			UF_UI_set_status("环形槽特征");
			if (OtherFace->SolidFaceType() == Face::FaceTypePlanar)
			{
				IsRightFlag = 0;
				break;
			}
			else if (OtherFace->SolidFaceType() == Face::FaceTypeConical ||
				OtherFace->SolidFaceType() == Face::FaceTypeCylindrical)
			{
				int num = Get_FullCircle_Num(OtherFace, _Approach_Normal);
				if (num > 1)
				{
					std::vector<Edge*>CorEdges;
					GetEdge_From_TwoFaces(EachFace, OtherFace, CorEdges);

					double Angle;
					get_FaceAngle(EachFace, OtherFace, CorEdges[0], Angle);
					Angle = Angle * 180 / PI;
					if (Angle>180)
					{
						IsRightFlag = 0;
						break;
					}
					else
					{
						Feature->BottomCornerFaceVector.push_back(OtherFace);
					}
				}
				else if (Is_Face_in_lpFace(OtherFace, SealFace_CircularGroove1) == -1)
				{
					Feature->LimitFaceVector.push_back(OtherFace);
				}
			}
			else if (OtherFace->SolidFaceType() == Face::FaceTypeSurfaceOfRevolution)
			{
				int num = Get_FullCircle_Num(OtherFace, _Approach_Normal);
				if (num > 0)
				{
					std::vector<Edge*>CorEdges;
					GetEdge_From_TwoFaces(EachFace, OtherFace, CorEdges);

					double Angle;
					get_FaceAngle(EachFace, OtherFace, CorEdges[0], Angle);
					Angle = Angle * 180 / PI;
					if (Angle>180)
					{
						IsRightFlag = 0;
						break;
					}
					else
					{
						Feature->BottomCornerFaceVector.push_back(OtherFace);
					}
				}
				else if (Is_Face_in_lpFace(OtherFace, SealFace_CircularGroove1) == -1)
				{
					Feature->LimitFaceVector.push_back(OtherFace);
				}
			}
			else if (OtherFace->SolidFaceType() == Face::FaceTypeBlending)
			{
				Feature->LimitFaceVector.push_back(OtherFace);
			}
		}

		if (IsRightFlag)
		{
			Feature_CircularGroove1.push_back(Feature);
		}
	}
	//MT_delete_progress_bar(&aa);//删除进度条
	//UF_unload_library(library_name);

}
void SubsidenceGrooveRecognition(std::vector<NXOpen::Face*>AllFaces, Face* BottomPlaneFace, Face* TopPlaneFace,
	std::vector<Face*> ParallelPlaneFace, std::vector<NXOpen::Face*>&SealFace_SubsidenceGroove, std::vector<NXOpen::Face*>&SealFace_HookFaceRib
	)
{
	std::vector<Face*>CylinderConicalRevolution_FaceVector;
	std::vector<Face*>SeedFace;
	CylinderConicalRevolution_FaceVector.clear(); SeedFace.clear();
	for (std::vector<Face*>::size_type i = 0; i != AllFaces.size(); i++)
	{
		/*MT_set_progress_pos(&aa, NowNum++);*/
		UF_UI_set_status("槽筋特征识别");
		Face* EachFace = AllFaces[i];
		if (EachFace->SolidFaceType() == Face::FaceTypeCylindrical || EachFace->SolidFaceType() == Face::FaceTypeConical)
		{
			//筛选出圆柱面圆锥面
			bool Flag = true;
			int type = 0;       //判断面类型
			int norm_dir = 0;   //外R1，内R-1
			double point[3];  //轴中心 点
			double dir[3];    //轴方向
			double box[6];    //极点坐标
			double radius;    //半径 圆锥=最小半径 + 最大半径 /2
			double rad_data;
			UF_MODL_ask_face_data(EachFace->Tag(), &type, point, dir, box, &radius, &rad_data, &norm_dir); // 分析面
			std::vector<Edge*>EachFace_EdgeVector;
			EachFace_EdgeVector = EachFace->GetEdges();
			for (std::vector<Edge*>::size_type j = 0; j != EachFace_EdgeVector.size(); j++)
			{
				double Length = EachFace_EdgeVector[j]->GetLength();
				if (Length>300)
				{
					Flag = false;
					break;
				}
			}

			if (radius>200 && norm_dir == 1 && Flag)
			{
				uf_list_p_t edge_list;
				UF_MODL_ask_face_edges(EachFace->Tag(), &edge_list);

				//获取链表的数量
				int count;
				UF_MODL_ask_list_count(edge_list, &count);

				for (int j = 0; j < count; j++)
				{
					//获取链表里的tag
					tag_t EdgeTag = NULL_TAG;
					UF_MODL_ask_list_item(edge_list, j, &EdgeTag);
					//获取边的光顺性
					logical is_smooth;
					UF_MODL_ask_edge_smoothness(EdgeTag, 0, &is_smooth);

					if (is_smooth == false)
					{
						//边找?
						uf_list_p_t face_list;
						UF_MODL_ask_edge_faces(EdgeTag, &face_list);

						//获得链表数量
						int face_count;
						UF_MODL_ask_list_count(face_list, &face_count);
						for (int k = 0; k < face_count; k++)
						{
							//获取链表?的tag
							tag_t FaceTag = NULL_TAG;
							UF_MODL_ask_list_item(face_list, k, &FaceTag);

							if (EachFace->Tag() != FaceTag)
							{
								NXObjectManager ObjectManager;
								TaggedObject* pTaggedObject = ObjectManager.Get(FaceTag);
								Face* pFace = dynamic_cast<Face*>(pTaggedObject);
								if (pFace->SolidFaceType() == Face::FaceTypeParametric)
								{
									SealFace_HookFaceRib.push_back(EachFace);
									RemoveDplicateFace(SealFace_HookFaceRib);
								}
								else if (pFace->SolidFaceType() == Face::FaceTypeSurfaceOfRevolution)//这是主轴回转筋 并不准确
								{
									double Rad;
									get_face_maxRad(pFace, Rad);
									if (Rad<10)
									{
										//UF_OBJ_set_color(EachFace->Tag(),186);
										SealFace_HookFaceRib.push_back(EachFace);
										RemoveDplicateFace(SealFace_HookFaceRib);
									}
								}
								else if (pFace->SolidFaceType() == Face::FaceTypePlanar)
								{
									SeedFace.push_back(EachFace);
									RemoveDplicateFace(SeedFace);
								}
							}
						}
					}

					else
					{
						//边找?
						uf_list_p_t face_list;
						UF_MODL_ask_edge_faces(EdgeTag, &face_list);

						//获得链表数量
						int face_count;
						UF_MODL_ask_list_count(face_list, &face_count);

						for (int k = 0; k < face_count; k++)
						{
							//获取链表?的tag
							tag_t FaceTag = NULL_TAG;
							UF_MODL_ask_list_item(face_list, k, &FaceTag);

							if (EachFace->Tag() != FaceTag)
							{
								NXObjectManager ObjectManager;
								TaggedObject* pTaggedObject = ObjectManager.Get(FaceTag);
								Face* pFace = dynamic_cast<Face*>(pTaggedObject);

								if (pFace->SolidFaceType() == Face::FaceTypeBlending
									&& Is_Face_in_lpFace(EachFace, SealFace_HookFaceRib) == -1)
								{
									//UF_OBJ_set_color(EachFace->Tag(),216);
									CylinderConicalRevolution_FaceVector.push_back(EachFace);
									RemoveDplicateFace(CylinderConicalRevolution_FaceVector);
								}
							}
						}
					}

				}
			}
		}
		else if (EachFace->SolidFaceType() == Face::FaceTypeSurfaceOfRevolution)
		{
			////或者是回转面的时候,
			double FaceRad; //曲率半径
			bool Flag = true;
			std::vector<Edge*>EachFace_EdgeVector;
			EachFace_EdgeVector = EachFace->GetEdges();
			get_face_maxRad(EachFace, FaceRad);
			for (std::vector<Edge*>::size_type j = 0; j != EachFace_EdgeVector.size(); j++)
			{
				double Length = EachFace_EdgeVector[j]->GetLength();
				if (Length>300)
				{
					Flag = false;
					break;
				}
			}
			if (Flag&&FaceRad>80)
			{
				std::vector<Face*> OutAdjacentFaces;
				get_External_Adjacent_Faces(EachFace, OutAdjacentFaces);
				for (int l = 0; l < OutAdjacentFaces.size(); l++)
				{
					Face* EachAdjFace = OutAdjacentFaces[l];
					if (EachAdjFace->SolidFaceType() == Face::FaceTypePlanar || EachAdjFace->SolidFaceType() == Face::FaceTypeParametric)
					{
						//UF_OBJ_set_color(EachFace->Tag(),186);

						SealFace_HookFaceRib.push_back(EachFace);
						RemoveDplicateFace(SealFace_HookFaceRib);
					}
					else if (EachAdjFace->SolidFaceType() == Face::FaceTypeBlending &&
						Is_Face_in_lpFace(EachFace, SealFace_HookFaceRib) == -1)
					{
						//UF_OBJ_set_color(EachFace->Tag(),216);
						CylinderConicalRevolution_FaceVector.push_back(EachFace);
						RemoveDplicateFace(CylinderConicalRevolution_FaceVector);
					}
				}
			}
		}
	}

	/*num0=SeedFace.size()-1;
	MT_set_progress_range(&aa, 0, num0);
	NowNum=0;*/
	//筋
	for (std::vector<Face*>::size_type i = 0; i != SeedFace.size(); i++)
	{
		/*MT_set_progress_pos(&aa, NowNum++);*/
		UF_UI_set_status("筋特征识别");
		Face* EachFace = SeedFace[i];

		std::vector<Face*> OutAdjacentFaces, InnerAdjacentFaces;
		get_External_Adjacent_Faces(EachFace, OutAdjacentFaces);
		get_Inner_Adjacent_Faces(EachFace, InnerAdjacentFaces);
		bool IsTopFace = true;
		for (int l = 0; l < OutAdjacentFaces.size(); l++)
		{
			Face* EachAdjFace = OutAdjacentFaces[l];
			if (EachAdjFace->Tag() == TopPlaneFace->Tag() || EachAdjFace->Tag() == BottomPlaneFace->Tag() ||
				Is_Face_in_lpFace(EachAdjFace, ParallelPlaneFace) != -1)
			{
				IsTopFace = false;
				break;
			}
			else if (EachAdjFace->SolidFaceType() == Face::FaceTypeConical &&
				Is_Face_in_lpFace(EachAdjFace, SeedFace) != -1)
			{
				std::vector<Face*> OutAdjancentFaces1;
				get_External_Adjacent_Faces(EachAdjFace, OutAdjancentFaces1);
				for (std::vector<Face*>::size_type l = 0; l != OutAdjancentFaces1.size(); l++)
				{
					Face* EachAdjacentFace1 = OutAdjancentFaces1[l];
					if (EachAdjacentFace1->SolidFaceType() == Face::FaceType::FaceTypePlanar)
					{
						if (EachAdjacentFace1->Tag() == TopPlaneFace->Tag() || EachAdjacentFace1->Tag() == BottomPlaneFace->Tag() ||
							Is_Face_in_lpFace(EachAdjacentFace1, ParallelPlaneFace) != -1)
						{
							IsTopFace = false;
							break;
						}
					}
				}

			}
			else if (EachAdjFace->SolidFaceType() == Face::FaceTypePlanar)
			{
				int num = Get_FullCircle_Num1(EachAdjFace);
				if (num>0)
				{
					IsTopFace = false;
					break;
				}
			}
		}
		for (int l = 0; l < InnerAdjacentFaces.size(); l++)
		{
			Face* EachAdjFace = InnerAdjacentFaces[l];
			if (EachAdjFace->SolidFaceType() == Face::FaceTypePlanar)
			{
				IsTopFace = false;
				break;
			}
		}

		if (IsTopFace)
		{
			//UF_OBJ_set_color(EachFace->Tag(),186);
			SealFace_HookFaceRib.push_back(EachFace);
			RemoveDplicateFace(SealFace_HookFaceRib);
		}
	}
	//槽
	/*num0=CylinderConicalRevolution_FaceVector.size()-1;
	MT_set_progress_range(&aa, 0, num0);
	NowNum=0;*/
	for (std::vector<Face*>::size_type i = 0; i != CylinderConicalRevolution_FaceVector.size(); i++)
	{
		/*MT_set_progress_pos(&aa, NowNum++);*/
		UF_UI_set_status("下陷槽特征识别");
		Face* EachFace = CylinderConicalRevolution_FaceVector[i];
		std::vector<Face*> AdjacentFaces;
		get_External_Adjacent_Faces(EachFace, AdjacentFaces);


		bool IsTopFace = true;

		for (int l = 0; l < AdjacentFaces.size(); l++)
		{
			Face* EachAdjFace = AdjacentFaces[l];
			if (EachAdjFace->SolidFaceType() == Face::FaceTypePlanar)
			{
				IsTopFace = false;
				break;
			}

		}

		if (IsTopFace && Is_Face_in_lpFace(EachFace, SealFace_HookFaceRib) == -1)
		{
			//UF_OBJ_set_color(EachFace->Tag(),216);
			SealFace_SubsidenceGroove.push_back(EachFace);

		}
	}


	//MT_delete_progress_bar(&aa);//删除进度条
	//UF_unload_library(library_name);
}
void CreateSubsidenceGrooveFeature(std::vector<NXOpen::Face*>SealFace_SubsidenceGroove, std::vector<SubsidenceGroove*>&Feature_SubsidenceGroove, Face *_ApproachFace)
{
	//===============================DEFINE PROGRESS Begin===================================================================

	//char *library_name = "D:\\ug10\\UGII\\libdisp.dll";
	//typedef void(*create_progress_bar)(int v1, bool v2, long *v3);//定义传参
	//create_progress_bar MT_create_progress_bar = NULL;
	//UF_load_library(library_name, "?MT_create_progress_bar@@YA_NH_NPEAJ@Z", (UF_load_f_p_t *)&MT_create_progress_bar);
	//typedef void(*set_progress_pos)(long *v1, int v2);//定义传参
	//set_progress_pos MT_set_progress_pos = NULL;
	//UF_load_library(library_name, "?MT_set_progress_pos@@YAXPEAJH@Z", (UF_load_f_p_t *)&MT_set_progress_pos);
	//typedef void(*show_progress_bar)(long *v1);//定义传参
	//show_progress_bar MT_show_progress_bar = NULL;
	//UF_load_library(library_name, "?MT_show_progress_bar@@YAXPEAJ@Z", (UF_load_f_p_t *)&MT_show_progress_bar);
	//typedef void(*set_progress_range)(long *v1, int v2, int v3);//定义传参
	//set_progress_range MT_set_progress_range = NULL;
	//UF_load_library(library_name, "?MT_set_progress_range@@YAXPEAJHH@Z", (UF_load_f_p_t *)&MT_set_progress_range);
	//typedef __int64(*ask_progress_cue_context)(void);//定义传参
	//ask_progress_cue_context MT_ask_progress_cue_context = NULL;
	//UF_load_library(library_name, "?MT_ask_progress_cue_context@@YAHXZ", (UF_load_f_p_t *)&MT_ask_progress_cue_context);
	//typedef __int64(*delete_progress_bar)(long *v1);//定义传参
	//delete_progress_bar MT_delete_progress_bar = NULL;
	//UF_load_library(library_name, "?MT_delete_progress_bar@@YAXPEAJ@Z", (UF_load_f_p_t *)&MT_delete_progress_bar);
	//typedef __int64(*set_progress_text)(long *v1, char *v2, bool v3);//定义传参
	//set_progress_text MT_set_progress_text = NULL;
	//UF_load_library(library_name, "?MT_set_progress_text@@YAXPEAJPEAD_N@Z", (UF_load_f_p_t *)&MT_set_progress_text);
	//if (MT_ask_progress_cue_context == NULL || MT_create_progress_bar == NULL || MT_show_progress_bar == NULL
	//	|| MT_set_progress_text == NULL || MT_set_progress_range == NULL || MT_set_progress_pos == NULL
	//	|| MT_delete_progress_bar == NULL)
	//{
	//	uc1601("进度条报错提示", 1);
	//	return;
	//}

	//__int64 cc = MT_ask_progress_cue_context();
	//long aa = 0;
	//MT_create_progress_bar(cc, true, &aa);//true和false控制是否显示百分比数字
	//MT_show_progress_bar(&aa);
	//MT_set_progress_text(&aa, "Loading", true);//此处用中文好像不显示

	//===============================DEFINE PROGRESS END===================================================================
	std::vector<Face*>AllSealFaces;
	for (std::vector<Face*>::size_type i = 0; i != SealFace_SubsidenceGroove.size(); i++)
	{
		Face *EachFace;
		EachFace = SealFace_SubsidenceGroove[i];
		AllSealFaces.push_back(EachFace);
		SubsidenceGroove*Feature = new SubsidenceGroove;
		Feature->TopFaceVector.push_back(EachFace);
		Feature_SubsidenceGroove.push_back(Feature);
	}

	/*while(AllSealFaces.size()!=0)
	{
	SubsidenceGroove *Feature=new SubsidenceGroove;
	std::vector<NXOpen::Face*>FirstFaces;
	FirstFaces.clear();
	CombineSealFace(FirstFaces,AllSealFaces);
	PutFaceVectorIntoFaceVector(FirstFaces,Feature->TopFaceVector);
	OrderCircularGroove(Feature->TopFaceVector,_ApproachFace);
	Feature_SubsidenceGroove.push_back(Feature);
	}*/

	int num0 = SealFace_SubsidenceGroove.size() - 1;
	/*MT_set_progress_range(&aa, 0, num0);*/
	int NowNum = 0;


	for (std::vector<SubsidenceGroove*>::size_type i = 0; i != Feature_SubsidenceGroove.size(); i++)
	{
		SubsidenceGroove* EachSubsidenceGroove = Feature_SubsidenceGroove[i];
		for (std::vector<Face*>::size_type j = 0; j != EachSubsidenceGroove->TopFaceVector.size(); j++)
		{
			/*MT_set_progress_pos(&aa, NowNum++);*/
			UF_UI_set_status("下陷槽特征");
			Face *EachFace;
			EachFace = Feature_SubsidenceGroove[i]->TopFaceVector[j];
			std::vector<Face*>EachAd_FaceVector;
			get_External_Adjacent_Faces(EachFace, EachAd_FaceVector);

			for (std::vector<Face*>::size_type t = 0; t != EachAd_FaceVector.size(); t++)
			{
				Face *OtherFace;
				OtherFace = EachAd_FaceVector[t];
				if (Is_Face_in_lpFace(OtherFace, Feature_SubsidenceGroove[i]->TopFaceVector) == -1)
				{
					//Feature_SubsidenceGroove[i]->BottomCornerFaceVector.push_back(OtherFace);
					if (OtherFace->SolidFaceType() == Face::FaceTypeBlending)
					{
						Feature_SubsidenceGroove[i]->BottomCornerFaceVector.push_back(OtherFace);
					}
					else if (OtherFace->SolidFaceType() == Face::FaceTypeConical ||
						OtherFace->SolidFaceType() == Face::FaceTypeCylindrical || OtherFace->SolidFaceType() == Face::FaceTypeSurfaceOfRevolution)
					{
						double Rad;
						get_face_maxRad(OtherFace, Rad);
						if (Rad>50)
						{
							continue;
						}
						else
						{
							Feature_SubsidenceGroove[i]->BottomCornerFaceVector.push_back(OtherFace);
						}

					}
				}
			}
		}
		//OrderSingleFeature(Feature_SubsidenceGroove[i]->BottomCornerFaceVector);
	}

	//MT_delete_progress_bar(&aa);//删除进度条
	//UF_unload_library(library_name);
}
void PlanerBlindGrooveRecognition(std::vector<NXOpen::Face*>AllFaces, Face* BottomPlaneFace, Face* TopPlaneFace,
	std::vector<Face*> ParallelPlaneFace, std::vector<NXOpen::Face*>&SealFace_PlanerBlindGroove)
{
	//===============================DEFINE PROGRESS Begin===================================================================
	//char *library_name = "D:\\ug10\\UGII\\libdisp.dll";
	//typedef void(*create_progress_bar)(int v1, bool v2, long *v3);//定义传参
	//create_progress_bar MT_create_progress_bar = NULL;
	//UF_load_library(library_name, "?MT_create_progress_bar@@YA_NH_NPEAJ@Z", (UF_load_f_p_t *)&MT_create_progress_bar);
	//typedef void(*set_progress_pos)(long *v1, int v2);//定义传参
	//set_progress_pos MT_set_progress_pos = NULL;
	//UF_load_library(library_name, "?MT_set_progress_pos@@YAXPEAJH@Z", (UF_load_f_p_t *)&MT_set_progress_pos);
	//typedef void(*show_progress_bar)(long *v1);//定义传参
	//show_progress_bar MT_show_progress_bar = NULL;
	//UF_load_library(library_name, "?MT_show_progress_bar@@YAXPEAJ@Z", (UF_load_f_p_t *)&MT_show_progress_bar);
	//typedef void(*set_progress_range)(long *v1, int v2, int v3);//定义传参
	//set_progress_range MT_set_progress_range = NULL;
	//UF_load_library(library_name, "?MT_set_progress_range@@YAXPEAJHH@Z", (UF_load_f_p_t *)&MT_set_progress_range);
	//typedef __int64(*ask_progress_cue_context)(void);//定义传参
	//ask_progress_cue_context MT_ask_progress_cue_context = NULL;
	//UF_load_library(library_name, "?MT_ask_progress_cue_context@@YAHXZ", (UF_load_f_p_t *)&MT_ask_progress_cue_context);
	//typedef __int64(*delete_progress_bar)(long *v1);//定义传参
	//delete_progress_bar MT_delete_progress_bar = NULL;
	//UF_load_library(library_name, "?MT_delete_progress_bar@@YAXPEAJ@Z", (UF_load_f_p_t *)&MT_delete_progress_bar);
	//typedef __int64(*set_progress_text)(long *v1, char *v2, bool v3);//定义传参
	//set_progress_text MT_set_progress_text = NULL;
	//UF_load_library(library_name, "?MT_set_progress_text@@YAXPEAJPEAD_N@Z", (UF_load_f_p_t *)&MT_set_progress_text);
	//if (MT_ask_progress_cue_context == NULL || MT_create_progress_bar == NULL || MT_show_progress_bar == NULL
	//	|| MT_set_progress_text == NULL || MT_set_progress_range == NULL || MT_set_progress_pos == NULL
	//	|| MT_delete_progress_bar == NULL)
	//{
	//	uc1601("进度条报错提示", 1);
	//	return;
	//}

	//__int64 cc = MT_ask_progress_cue_context();
	//long aa = 0;
	//MT_create_progress_bar(cc, true, &aa);//true和false控制是否显示百分比数字
	//MT_show_progress_bar(&aa);
	//MT_set_progress_text(&aa, "Loading", true);//此处用中文好像不显示

	//===============================DEFINE PROGRESS END===================================================================
	std::vector<Face*> AllPlaneFaces;
	for (std::vector<Face*>::size_type i = 0; i != AllFaces.size(); i++)
	{
		Face* EachFace = AllFaces[i];

		if (EachFace->SolidFaceType() == Face::FaceType::FaceTypePlanar)
		{
			if (EachFace->Tag() != TopPlaneFace->Tag() && EachFace->Tag() != BottomPlaneFace->Tag()
				&& Is_Face_in_lpFace(EachFace, ParallelPlaneFace) == -1)
			{
				/*Vector3d Normal_Vector,MainNormal;
				get_MainNormal_fr_plane(BeginFace, Normal_Vector);
				get_MainNormal_fr_plane(EachFace, MainNormal);
				double angle = get_Angle_Two_vector3d(MainNormal, Normal_Vector);
				angle = angle / PI * 180;*/
				AllPlaneFaces.push_back(EachFace);
			}
		}
	}
	//////////////////////////////////////////////////////
	std::vector<Face*> CornerFaces;
	CornerFaces.clear();
	for (std::vector<Face*>::size_type i = 0; i != AllPlaneFaces.size(); i++)
	{
		Face* EachPlaneFace = AllPlaneFaces[i];

		uf_list_p_t edge_list;
		UF_MODL_ask_face_edges(EachPlaneFace->Tag(), &edge_list);

		//获取链表的数量
		int count;
		UF_MODL_ask_list_count(edge_list, &count);

		for (int j = 0; j < count; j++)
		{
			//获取链表里的tag
			tag_t EdgeTag = NULL_TAG;
			UF_MODL_ask_list_item(edge_list, j, &EdgeTag);
			//获取边的光顺性
			logical is_smooth;
			UF_MODL_ask_edge_smoothness(EdgeTag, 0, &is_smooth);

			if (is_smooth == true)
			{
				//边找?
				uf_list_p_t face_list;
				UF_MODL_ask_edge_faces(EdgeTag, &face_list);

				//获得链表数量
				int face_count;
				UF_MODL_ask_list_count(face_list, &face_count);

				for (int k = 0; k < face_count; k++)
				{
					//获取链表?的tag
					tag_t FaceTag = NULL_TAG;
					UF_MODL_ask_list_item(face_list, k, &FaceTag);

					if (EachPlaneFace->Tag() != FaceTag)
					{
						NXObjectManager ObjectManager;
						TaggedObject* pTaggedObject = ObjectManager.Get(FaceTag);
						Face* pFace = dynamic_cast<Face*>(pTaggedObject);
						if (pFace->SolidFaceType() == Face::FaceTypeBlending ||
							pFace->SolidFaceType() == Face::FaceTypeCylindrical ||
							pFace->SolidFaceType() == Face::FaceTypeSurfaceOfRevolution)
						{
							double CornerRad;
							get_face_maxRad(pFace, CornerRad);
							if (CornerRad < 10)
							{
								CornerFaces.push_back(pFace);
							}

						}
					}
				}
			}
		}
	}


	for (std::vector<Face*>::size_type i = 0; i != AllPlaneFaces.size(); i++)
	{
		Face* EachPlaneFace = AllPlaneFaces[i];
		std::vector<Face*> AdjacentFaces;
		get_External_Adjacent_Faces(EachPlaneFace, AdjacentFaces);
		bool Flag = 1; int CornerNum = 0;
		for (std::vector<Face*>::size_type j = 0; j != AdjacentFaces.size(); j++)
		{
			Face* EachAdjFace = AdjacentFaces[j];
			if (Is_Face_in_lpFace(EachAdjFace, CornerFaces) != -1)
			{
				CornerNum++;
			}
			else
			{
				Flag = 0;
				break;
			}
		}
		if (Flag&&CornerNum>1)
		{
			SealFace_PlanerBlindGroove.push_back(EachPlaneFace);
		}
	}

	//int num0=PlanerFaces.size()-1;
	//MT_set_progress_range(&aa, 0, num0);
	//int NowNum=0;
	//for(std::vector<Face*>::size_type i=0;i!=PlanerFaces.size();i++)
	//{
	//	MT_set_progress_pos(&aa, NowNum++);
	//	Face *EachFace;
	//	EachFace=PlanerFaces[i];
	//	std::vector<Face*>EachAd_FaceVector;
	//	get_External_Adjacent_Faces(EachFace,EachAd_FaceVector);
	//	bool Flag=1;int CornerNum=0;
	//	for(std::vector<Face*>::size_type j=0;j!=EachAd_FaceVector.size();j++)
	//	{
	//		Face *EachCornerFace;
	//		EachCornerFace=EachAd_FaceVector[j];

	//		double Angle;std::vector<Edge*>CorEdge;
	//		GetEdge_From_TwoFaces(EachFace,EachCornerFace,CorEdge);
	//		get_FaceAngle(EachFace,EachCornerFace,CorEdge[0],Angle);
	//		Angle=Angle/PI*180;

	//		if(Angle<181&&Angle>0)
	//		{
	//			double CornerRad;
	//			get_face_maxRad(EachCornerFace,CornerRad);
	//			if(CornerRad<5)
	//			{
	//				CornerNum++;
	//			}
	//			else
	//			{
	//				Flag=0;
	//				break;
	//			}
	//		}
	//		else
	//		{
	//			Flag=0;
	//			break;
	//		}

	//	}
	//	if(Flag&&CornerNum>1)
	//	{
	//		//UF_OBJ_set_color(EachFace->Tag(),186);
	//		SealFace_PlanerBlindGroove.push_back(EachFace);
	//	}
	//}

	//MT_delete_progress_bar(&aa);//删除进度条
	//UF_unload_library(library_name);
}
//构建平面浅槽特征
void CreatePlanerBlindGrooveFeature(std::vector<NXOpen::Face*>SealFace_PlanerBlindGroove, std::vector<PlanerBlindGroove*>&Feature_PlanerBlindGroove)
{
	//===============================DEFINE PROGRESS Begin===================================================================
	//char *library_name = "D:\\ug10\\UGII\\libdisp.dll";
	//typedef void(*create_progress_bar)(int v1, bool v2, long *v3);//定义传参
	//create_progress_bar MT_create_progress_bar = NULL;
	//UF_load_library(library_name, "?MT_create_progress_bar@@YA_NH_NPEAJ@Z", (UF_load_f_p_t *)&MT_create_progress_bar);
	//typedef void(*set_progress_pos)(long *v1, int v2);//定义传参
	//set_progress_pos MT_set_progress_pos = NULL;
	//UF_load_library(library_name, "?MT_set_progress_pos@@YAXPEAJH@Z", (UF_load_f_p_t *)&MT_set_progress_pos);
	//typedef void(*show_progress_bar)(long *v1);//定义传参
	//show_progress_bar MT_show_progress_bar = NULL;
	//UF_load_library(library_name, "?MT_show_progress_bar@@YAXPEAJ@Z", (UF_load_f_p_t *)&MT_show_progress_bar);
	//typedef void(*set_progress_range)(long *v1, int v2, int v3);//定义传参
	//set_progress_range MT_set_progress_range = NULL;
	//UF_load_library(library_name, "?MT_set_progress_range@@YAXPEAJHH@Z", (UF_load_f_p_t *)&MT_set_progress_range);
	//typedef __int64(*ask_progress_cue_context)(void);//定义传参
	//ask_progress_cue_context MT_ask_progress_cue_context = NULL;
	//UF_load_library(library_name, "?MT_ask_progress_cue_context@@YAHXZ", (UF_load_f_p_t *)&MT_ask_progress_cue_context);
	//typedef __int64(*delete_progress_bar)(long *v1);//定义传参
	//delete_progress_bar MT_delete_progress_bar = NULL;
	//UF_load_library(library_name, "?MT_delete_progress_bar@@YAXPEAJ@Z", (UF_load_f_p_t *)&MT_delete_progress_bar);
	//typedef __int64(*set_progress_text)(long *v1, char *v2, bool v3);//定义传参
	//set_progress_text MT_set_progress_text = NULL;
	//UF_load_library(library_name, "?MT_set_progress_text@@YAXPEAJPEAD_N@Z", (UF_load_f_p_t *)&MT_set_progress_text);
	//if (MT_ask_progress_cue_context == NULL || MT_create_progress_bar == NULL || MT_show_progress_bar == NULL
	//	|| MT_set_progress_text == NULL || MT_set_progress_range == NULL || MT_set_progress_pos == NULL
	//	|| MT_delete_progress_bar == NULL)
	//{
	//	uc1601("进度条报错提示", 1);
	//	return;
	//}

	//__int64 cc = MT_ask_progress_cue_context();
	//long aa = 0;
	//MT_create_progress_bar(cc, true, &aa);//true和false控制是否显示百分比数字
	//MT_show_progress_bar(&aa);
	//MT_set_progress_text(&aa, "CreatePlanerBlindGrooveFeature", true);//此处用中文好像不显示

	//int num0=SealFace_PlanerBlindGroove.size()-1;
	//MT_set_progress_range(&aa, 0, num0);
	//int NowNum=0;
	//===============================DEFINE PROGRESS END===================================================================

	for (std::vector<Face*>::size_type i = 0; i != SealFace_PlanerBlindGroove.size(); i++)
	{
		/*MT_set_progress_pos(&aa, NowNum++);*/
		UF_UI_set_status("平底槽特征");
		Face *EachFace;
		EachFace = SealFace_PlanerBlindGroove[i];
		PlanerBlindGroove *Feature = new PlanerBlindGroove;
		Feature->BottomFaceVector.push_back(EachFace);

		uf_list_p_t edge_list;
		UF_MODL_ask_face_edges(EachFace->Tag(), &edge_list);

		//获取链表的数量
		int count;
		UF_MODL_ask_list_count(edge_list, &count);

		for (int j = 0; j < count; j++)
		{
			//获取链表里的tag
			tag_t EdgeTag = NULL_TAG;
			UF_MODL_ask_list_item(edge_list, j, &EdgeTag);
			//获取边的光顺性
			logical is_smooth;
			UF_MODL_ask_edge_smoothness(EdgeTag, 0, &is_smooth);

			if (is_smooth == true)
			{
				//边找?
				uf_list_p_t face_list;
				UF_MODL_ask_edge_faces(EdgeTag, &face_list);

				//获得链表数量
				int face_count;
				UF_MODL_ask_list_count(face_list, &face_count);

				for (int k = 0; k < face_count; k++)
				{
					//获取链表?的tag
					tag_t FaceTag = NULL_TAG;
					UF_MODL_ask_list_item(face_list, k, &FaceTag);

					if (EachFace->Tag() != FaceTag)
					{
						NXObjectManager ObjectManager;
						TaggedObject* pTaggedObject = ObjectManager.Get(FaceTag);
						Face* pFace = dynamic_cast<Face*>(pTaggedObject);
						if (pFace->SolidFaceType() == Face::FaceTypeBlending ||
							pFace->SolidFaceType() == Face::FaceTypeCylindrical ||
							pFace->SolidFaceType() == Face::FaceTypeSurfaceOfRevolution)
						{
							double CornerRad;
							get_face_maxRad(pFace, CornerRad);
							if (CornerRad < 10)
							{
								Feature->BottomCornerFaceVector.push_back(pFace);
							}
						}
					}
				}
			}
		}



		//std::vector<Face*>EachAd_FaceVector;
		//get_External_Adjacent_Faces(EachFace,EachAd_FaceVector);

		//bool Flag=1;
		////找底角面
		//for(std::vector<Face*>::size_type j=0;j!=EachAd_FaceVector.size();j++)
		//{
		//	Face *EachCornerFace;
		//	EachCornerFace=EachAd_FaceVector[j];

		//	double Angle;std::vector<Edge*>CorEdge;
		//	GetEdge_From_TwoFaces(EachFace,EachCornerFace,CorEdge);
		//	get_FaceAngle(EachFace,EachCornerFace,CorEdge[0],Angle);
		//	Angle=Angle/PI*180;

		//	if(Angle<181&&Angle>0)
		//	{
		//		double CornerRad;
		//		get_face_maxRad(EachCornerFace,CornerRad);
		//		if(CornerRad<5)
		//		{
		//			Feature->BottomCornerFaceVector.push_back(EachCornerFace);
		//		}
		//		else
		//		{
		//			Flag=0;
		//			break;
		//		}
		//	}
		//	else
		//	{
		//		Flag=0;
		//		break;
		//	}

		//}

		//找侧面
		for (std::vector<Face*>::size_type t = 0; t != Feature->BottomCornerFaceVector.size(); t++)
		{
			Face *EachCornerFace;
			EachCornerFace = Feature->BottomCornerFaceVector[t];
			std::vector<Face*>ConnerAd_FaceVector;
			get_Adjacent_Faces(EachCornerFace, ConnerAd_FaceVector);
			for (std::vector<Face*>::size_type z = 0; z != ConnerAd_FaceVector.size(); z++)
			{
				Face *EachSideFace;
				EachSideFace = ConnerAd_FaceVector[z];
				if (EachSideFace != EachFace&&Is_Face_in_lpFace(EachSideFace, Feature->BottomCornerFaceVector) == -1
					&& Is_Face_in_lpFace(EachFace, Feature->SideFaceVector) == -1)
				{
					Feature->SideFaceVector.push_back(EachSideFace);
				}
			}

		}


		/*if(Flag&&Feature->SideFaceVector.size()>0&&Feature->BottomCornerFaceVector.size()>0)
		{*/
		//OrderSingleFeature(Feature->SideFaceVector);
		Feature_PlanerBlindGroove.push_back(Feature);
		//}

	}
	//MT_delete_progress_bar(&aa);//删除进度条
	//UF_unload_library(library_name);
}

void PassGrooveRecognition(std::vector<NXOpen::Face*>AllFaces, std::vector<NXOpen::Face*>&SealFace_PassGroove)
{
	//===============================DEFINE PROGRESS Begin===================================================================
	//char *library_name = "D:\\ug10\\UGII\\libdisp.dll";
	//typedef void(*create_progress_bar)(int v1, bool v2, long *v3);//定义传参
	//create_progress_bar MT_create_progress_bar = NULL;
	//UF_load_library(library_name, "?MT_create_progress_bar@@YA_NH_NPEAJ@Z", (UF_load_f_p_t *)&MT_create_progress_bar);
	//typedef void(*set_progress_pos)(long *v1, int v2);//定义传参
	//set_progress_pos MT_set_progress_pos = NULL;
	//UF_load_library(library_name, "?MT_set_progress_pos@@YAXPEAJH@Z", (UF_load_f_p_t *)&MT_set_progress_pos);
	//typedef void(*show_progress_bar)(long *v1);//定义传参
	//show_progress_bar MT_show_progress_bar = NULL;
	//UF_load_library(library_name, "?MT_show_progress_bar@@YAXPEAJ@Z", (UF_load_f_p_t *)&MT_show_progress_bar);
	//typedef void(*set_progress_range)(long *v1, int v2, int v3);//定义传参
	//set_progress_range MT_set_progress_range = NULL;
	//UF_load_library(library_name, "?MT_set_progress_range@@YAXPEAJHH@Z", (UF_load_f_p_t *)&MT_set_progress_range);
	//typedef __int64(*ask_progress_cue_context)(void);//定义传参
	//ask_progress_cue_context MT_ask_progress_cue_context = NULL;
	//UF_load_library(library_name, "?MT_ask_progress_cue_context@@YAHXZ", (UF_load_f_p_t *)&MT_ask_progress_cue_context);
	//typedef __int64(*delete_progress_bar)(long *v1);//定义传参
	//delete_progress_bar MT_delete_progress_bar = NULL;
	//UF_load_library(library_name, "?MT_delete_progress_bar@@YAXPEAJ@Z", (UF_load_f_p_t *)&MT_delete_progress_bar);
	//typedef __int64(*set_progress_text)(long *v1, char *v2, bool v3);//定义传参
	//set_progress_text MT_set_progress_text = NULL;
	//UF_load_library(library_name, "?MT_set_progress_text@@YAXPEAJPEAD_N@Z", (UF_load_f_p_t *)&MT_set_progress_text);
	//if (MT_ask_progress_cue_context == NULL || MT_create_progress_bar == NULL || MT_show_progress_bar == NULL
	//	|| MT_set_progress_text == NULL || MT_set_progress_range == NULL || MT_set_progress_pos == NULL
	//	|| MT_delete_progress_bar == NULL)
	//{
	//	uc1601("进度条报错提示", 1);
	//	return;
	//}

	//__int64 cc = MT_ask_progress_cue_context();
	//long aa = 0;
	//MT_create_progress_bar(cc, true, &aa);//true和false控制是否显示百分比数字
	//MT_show_progress_bar(&aa);
	//MT_set_progress_text(&aa, "Loading", true);//此处用中文好像不显示

	//===============================DEFINE PROGRESS END===================================================================
	std::vector<NXOpen::Face*>PlanerFaces;
	for (std::vector<Face*>::size_type i = 0; i != AllFaces.size(); i++)
	{
		Face *EachFace;
		EachFace = AllFaces[i];
		if (EachFace->SolidFaceType() == Face::FaceTypePlanar)
		{
			PlanerFaces.push_back(EachFace);
		}
	}
	//////////////////////////////////////////////////////

	/*int num0=PlanerFaces.size()-1;
	MT_set_progress_range(&aa, 0, num0);
	int NowNum=0;*/
	for (std::vector<Face*>::size_type i = 0; i != PlanerFaces.size(); i++)
	{
		/*MT_set_progress_pos(&aa, NowNum++);*/
		UF_UI_set_status("通槽特征识别");
		Face *EachFace;
		EachFace = PlanerFaces[i];
		std::vector<Edge*>EachInner_EdgeVector;
		get_Inner_Edge(EachFace, EachInner_EdgeVector);
		std::vector<Edge*>NoComplete_EdgeVector;
		//找正确的内边
		int CompleteEdgeNum = 0;
		for (std::vector<Edge*>::size_type j = 0; j != EachInner_EdgeVector.size(); j++)
		{
			Edge *EachEdge;
			EachEdge = EachInner_EdgeVector[j];
			if (IsCircleEdgeComplete(EachEdge) == 0)
			{
				NoComplete_EdgeVector.push_back(EachEdge);
			}
			else
			{
				CompleteEdgeNum++;
			}
		}
		if (CompleteEdgeNum == 0){ continue; }//面上必须有内侧完整圆
		//找顶面的下一个邻面
		std::vector<Face*>Next_FaceVector;
		for (std::vector<Edge*>::size_type j = 0; j != NoComplete_EdgeVector.size(); j++)
		{
			Edge *EachEdge;
			EachEdge = NoComplete_EdgeVector[j];
			std::vector<Face*>Temp_FaceVector;
			Temp_FaceVector = EachEdge->GetFaces();
			for (std::vector<Face*>::size_type t = 0; t != Temp_FaceVector.size(); t++)
			{
				Face *TempFace;
				TempFace = Temp_FaceVector[t];
				if (TempFace != EachFace&&Is_Face_in_lpFace(TempFace, Next_FaceVector) == -1)
				{
					Next_FaceVector.push_back(TempFace);
					//UF_OBJ_set_color(TempFace->Tag(),186);
				}
			}
		}

		if (Next_FaceVector.size() != 0)
		{
			//UF_OBJ_set_color(EachFace->Tag(),186);
			SealFace_PassGroove.push_back(EachFace);
		}

	}

	//MT_delete_progress_bar(&aa);//删除进度条
	//UF_unload_library(library_name);
}

//构建通槽特征
void CreatePassGrooveFeature(std::vector<NXOpen::Face*>SealFace_PassGroove, std::vector<PassGroove*>&Feature_PassGroove)
{
	//===============================DEFINE PROGRESS Begin===================================================================
	//char *library_name = "D:\\ug10\\UGII\\libdisp.dll";
	//typedef void(*create_progress_bar)(int v1, bool v2, long *v3);//定义传参
	//create_progress_bar MT_create_progress_bar = NULL;
	//UF_load_library(library_name, "?MT_create_progress_bar@@YA_NH_NPEAJ@Z", (UF_load_f_p_t *)&MT_create_progress_bar);
	//typedef void(*set_progress_pos)(long *v1, int v2);//定义传参
	//set_progress_pos MT_set_progress_pos = NULL;
	//UF_load_library(library_name, "?MT_set_progress_pos@@YAXPEAJH@Z", (UF_load_f_p_t *)&MT_set_progress_pos);
	//typedef void(*show_progress_bar)(long *v1);//定义传参
	//show_progress_bar MT_show_progress_bar = NULL;
	//UF_load_library(library_name, "?MT_show_progress_bar@@YAXPEAJ@Z", (UF_load_f_p_t *)&MT_show_progress_bar);
	//typedef void(*set_progress_range)(long *v1, int v2, int v3);//定义传参
	//set_progress_range MT_set_progress_range = NULL;
	//UF_load_library(library_name, "?MT_set_progress_range@@YAXPEAJHH@Z", (UF_load_f_p_t *)&MT_set_progress_range);
	//typedef __int64(*ask_progress_cue_context)(void);//定义传参
	//ask_progress_cue_context MT_ask_progress_cue_context = NULL;
	//UF_load_library(library_name, "?MT_ask_progress_cue_context@@YAHXZ", (UF_load_f_p_t *)&MT_ask_progress_cue_context);
	//typedef __int64(*delete_progress_bar)(long *v1);//定义传参
	//delete_progress_bar MT_delete_progress_bar = NULL;
	//UF_load_library(library_name, "?MT_delete_progress_bar@@YAXPEAJ@Z", (UF_load_f_p_t *)&MT_delete_progress_bar);
	//typedef __int64(*set_progress_text)(long *v1, char *v2, bool v3);//定义传参
	//set_progress_text MT_set_progress_text = NULL;
	//UF_load_library(library_name, "?MT_set_progress_text@@YAXPEAJPEAD_N@Z", (UF_load_f_p_t *)&MT_set_progress_text);
	//if (MT_ask_progress_cue_context == NULL || MT_create_progress_bar == NULL || MT_show_progress_bar == NULL
	//	|| MT_set_progress_text == NULL || MT_set_progress_range == NULL || MT_set_progress_pos == NULL
	//	|| MT_delete_progress_bar == NULL)
	//{
	//	uc1601("进度条报错提示", 1);
	//	return;
	//}

	//__int64 cc = MT_ask_progress_cue_context();
	//long aa = 0;
	//MT_create_progress_bar(cc, true, &aa);//true和false控制是否显示百分比数字
	//MT_show_progress_bar(&aa);
	//MT_set_progress_text(&aa, "Loading", true);//此处用中文好像不显示

	//int num0=SealFace_PassGroove.size()-1;
	//MT_set_progress_range(&aa, 0, num0);
	//int NowNum=0;
	//===============================DEFINE PROGRESS END===================================================================
	for (std::vector<Face*>::size_type i = 0; i != SealFace_PassGroove.size(); i++)
	{
		/*MT_set_progress_pos(&aa, NowNum++);*/
		UF_UI_set_status("通槽特征");

		Face *EachFace;
		EachFace = SealFace_PassGroove[i];
		PassGroove*Feature = new PassGroove;
		Feature->TopFaceVector.push_back(EachFace);

		std::vector<Edge*>EachInner_EdgeVector;
		get_Inner_Edge(EachFace, EachInner_EdgeVector);
		std::vector<Edge*>NoComplete_EdgeVector;
		//找正确的内边
		for (std::vector<Edge*>::size_type j = 0; j != EachInner_EdgeVector.size(); j++)
		{
			Edge *EachEdge;
			EachEdge = EachInner_EdgeVector[j];
			if (IsCircleEdgeComplete(EachEdge) == 0)
			{
				NoComplete_EdgeVector.push_back(EachEdge);
			}
		}
		//找顶面的下一个邻面
		for (std::vector<Edge*>::size_type j = 0; j != NoComplete_EdgeVector.size(); j++)
		{
			Edge *EachEdge;
			EachEdge = NoComplete_EdgeVector[j];
			std::vector<Face*>Temp_FaceVector;
			Temp_FaceVector = EachEdge->GetFaces();
			for (std::vector<Face*>::size_type t = 0; t != Temp_FaceVector.size(); t++)
			{
				Face *TempFace;
				TempFace = Temp_FaceVector[t];
				if (TempFace == EachFace)
				{
					continue;
				}
				double Angle; std::vector<Edge*>CorEdge;
				GetEdge_From_TwoFaces(EachFace, TempFace, CorEdge);
				get_FaceAngle(EachFace, TempFace, CorEdge[0], Angle);
				Angle = Angle / PI * 180;

				if (Angle>220 && Angle<230)//这里是找倒角面
				{
					if (Is_Face_in_lpFace(TempFace, Feature->RightFaceVector) == -1)
					{
						Feature->RightFaceVector.push_back(TempFace);
					}
				}
				else if (Angle>265 && Angle<275)//这里是不存在倒角面，直接找侧面
				{
					if (Is_Face_in_lpFace(TempFace, Feature->SideFaceVector) == -1)
					{
						Feature->SideFaceVector.push_back(TempFace);
					}
				}
			}
		}
		//这里找一下侧面，也就是存在倒角面的情况
		if (Feature->SideFaceVector.size() == 0)
		{
			for (std::vector<Edge*>::size_type j = 0; j != Feature->RightFaceVector.size(); j++)
			{
				Face *EachRightFace;
				EachRightFace = Feature->RightFaceVector[j];
				std::vector<Face*>Side_FaceVector;
				get_Adjacent_Faces(EachRightFace, Side_FaceVector);
				for (std::vector<Edge*>::size_type t = 0; t != Side_FaceVector.size(); t++)
				{
					Face *EachSideFace;
					EachSideFace = Side_FaceVector[t];
					if (EachSideFace != EachFace&&Is_Face_in_lpFace(EachSideFace, Feature->RightFaceVector) == -1
						&& Is_Face_in_lpFace(EachSideFace, Feature->SideFaceVector) == -1)
					{
						double Angle; std::vector<Edge*>CorEdge;
						GetEdge_From_TwoFaces(EachRightFace, EachSideFace, CorEdge);
						get_FaceAngle(EachRightFace, EachSideFace, CorEdge[0], Angle);
						Angle = Angle / PI * 180;

						if (Angle>220 && Angle<230)
						{
							Feature->SideFaceVector.push_back(EachSideFace);
						}
					}
				}
			}

		}
		//找背面
		for (std::vector<Edge*>::size_type j = 0; j != Feature->SideFaceVector.size(); j++)
		{
			Face *EachSideFace;
			EachSideFace = Feature->SideFaceVector[j];
			std::vector<Face*>Bottom_FaceVector;
			get_Adjacent_Faces(EachSideFace, Bottom_FaceVector);
			for (std::vector<Edge*>::size_type t = 0; t != Bottom_FaceVector.size(); t++)
			{
				Face *EachBottomFace;
				EachBottomFace = Bottom_FaceVector[t];
				if (EachBottomFace != EachFace&&Is_Face_in_lpFace(EachBottomFace, Feature->RightFaceVector) == -1
					&& Is_Face_in_lpFace(EachBottomFace, Feature->BottomFaceVector) == -1
					&& Is_Face_in_lpFace(EachBottomFace, Feature->SideFaceVector) == -1)
				{
					Feature->BottomFaceVector.push_back(EachBottomFace);
				}
			}
		}

		Feature_PassGroove.push_back(Feature);

	}
	//MT_delete_progress_bar(&aa);//删除进度条
	//UF_unload_library(library_name);
}
//构建主轴回转规则筋
void CreateDirectionRibFeature(std::vector<Face*>SealFace_DirectionRib, std::vector<DirectionRib*>&Feature_DirectionRib)
{

	int num0 = SealFace_DirectionRib.size() - 1;
	int NowNum = 0;
	//===============================DEFINE PROGRESS END===================================================================
	for (std::vector<Face*>::size_type i = 0; i != SealFace_DirectionRib.size(); i++)
	{
		//MT_set_progress_pos(&aa, NowNum++);
		UF_UI_set_status("筋特征");
		Face *EachFace;
		EachFace = SealFace_DirectionRib[i];
		std::vector<Face*>EachSide_FaceVector;
		get_Adjacent_Faces(EachFace, EachSide_FaceVector);

		DirectionRib *Feature = new DirectionRib;
		Feature->TopFaceVector.push_back(EachFace);

		//这里找侧面
		for (std::vector<Face*>::size_type j = 0; j != EachSide_FaceVector.size(); j++)
		{
			Face *EachSideFace;
			EachSideFace = EachSide_FaceVector[j];

			double Angle; std::vector<Edge*>CorEdge;
			GetEdge_From_TwoFaces(EachFace, EachSideFace, CorEdge);
			get_FaceAngle(EachFace, EachSideFace, CorEdge[0], Angle);
			Angle = Angle / PI * 180;
			double SideRad;
			get_face_maxRad(EachSideFace, SideRad);

			if (Angle<180 && SideRad<8 && Is_Face_in_lpFace(EachSideFace, Feature->LimitFaceVector) == -1) //这里找到的是正确的sideface
			{
				Feature->LimitFaceVector.push_back(EachSideFace);

			}
			else if (Angle>180 && SideRad<8 && Is_Face_in_lpFace(EachSideFace, Feature->BottomCornerFaceVector) == -1) //侧面约束面
			{
				Feature->BottomCornerFaceVector.push_back(EachSideFace);
			}
		}

		if (Feature->BottomCornerFaceVector.size() != 0 && Feature->LimitFaceVector.size() != 0) //底角面不为空
		{
			Feature_DirectionRib.push_back(Feature);
		}
	}
}

//--------------------------------------------
void DirectionRibRecognition(std::vector<Face*>AllFaces, std::vector<Face*>&SealFace_DirectionRib)
{
	//===============================DEFINE PROGRESS Begin===================================================================

	std::vector<Face*>SealFacesVector;
	for (std::vector<Face*>::size_type i = 0; i != AllFaces.size(); i++)
	{
		Face *EachFace;
		EachFace = AllFaces[i];
		if (EachFace->SolidFaceType() == Face::FaceTypeCylindrical || EachFace->SolidFaceType() == Face::FaceTypeConical)
		{
			int type = 0;       //判断面类型
			int norm_dir = 0;   //1=外R，-1=内R
			double point[3];  //轴中心 点
			double dir[3];    //轴方向
			double box[6];    //极点坐标
			double radius;    //半径 圆锥=最小半径 + 最大半径 /2
			double rad_data;
			UF_MODL_ask_face_data(EachFace->Tag(), &type, point, dir, box, &radius, &rad_data, &norm_dir);
			std::vector<Edge*>Each_EdgeVector;
			Each_EdgeVector = EachFace->GetEdges();
			double EdgeNum = Each_EdgeVector.size();
			if (norm_dir == 1 && radius>50 && EdgeNum == 4)
			{
				//UF_DISP_set_highlight(EachFace->Tag(),1);
				SealFacesVector.push_back(EachFace);
			}
		}
		else if (EachFace->SolidFaceType() == Face::FaceTypeSurfaceOfRevolution)
		{
			bool Flag = 1; double Rad = 0;
			std::vector<Edge*>EachFace_EdgeVector;
			EachFace_EdgeVector = EachFace->GetEdges();
			for (std::vector<Edge*>::size_type j = 0; j != EachFace_EdgeVector.size(); j++)
			{
				double Length;
				Length = EachFace_EdgeVector[j]->GetLength();
				if (Length>200)
				{
					Flag = 0;
					continue;
				}
			}
			get_face_maxRad(EachFace, Rad);
			std::vector<Edge*>Each_EdgeVector;
			Each_EdgeVector = EachFace->GetEdges();
			double EdgeNum = Each_EdgeVector.size();
			if (Flag&&Rad>50 && EdgeNum == 4)
			{
				//UF_DISP_set_highlight(EachFace->Tag(),1);
				SealFacesVector.push_back(EachFace);
			}
		}
	}

	int num0 = SealFacesVector.size() - 1;
	int NowNum = 0;

	for (std::vector<Face*>::size_type i = 0; i != SealFacesVector.size(); i++)
	{

		UF_UI_set_status("主轴规则回转筋特征识别");
		bool Flag = 1; int BossLimitFaceNum = 0; int SideFaceNum = 0;
		Face *EachFace;
		EachFace = SealFacesVector[i];
		std::vector<Face*>EachAd_FaceVector;
		get_External_Adjacent_Faces(EachFace, EachAd_FaceVector);
		for (std::vector<Face*>::size_type j = 0; j != EachAd_FaceVector.size(); j++)
		{
			Face *OtherFace;
			OtherFace = EachAd_FaceVector[j];

			if (OtherFace->SolidFaceType() == Face::FaceTypeSurfaceOfRevolution)
			{
				double Rad;
				get_face_maxRad(OtherFace, Rad);
				if (Rad<10)//说明是直接和凸台相接，或是筋侧面
				{
					double Angle; std::vector<Edge*>CorEdge;
					GetEdge_From_TwoFaces(EachFace, OtherFace, CorEdge);
					get_FaceAngle(EachFace, OtherFace, CorEdge[0], Angle);
					Angle = Angle / PI * 180;
					if (Angle>180 && Angle<270)
					{
						SideFaceNum++;
					}
				}
			}
			else if (OtherFace->SolidFaceType() != Face::FaceTypeCylindrical&&OtherFace->SolidFaceType() != Face::FaceTypeConical
				&&OtherFace->SolidFaceType() != Face::FaceTypePlanar&&OtherFace->SolidFaceType() != Face::FaceTypeSurfaceOfRevolution)
			{
				double Rad;
				get_face_maxRad(OtherFace, Rad);
				if (Rad<10)//说明是直接和凸台相接，或是筋侧面
				{
					double Angle; std::vector<Edge*>CorEdge;
					GetEdge_From_TwoFaces(EachFace, OtherFace, CorEdge);
					get_FaceAngle(EachFace, OtherFace, CorEdge[0], Angle);
					Angle = Angle / PI * 180;
					if (Angle>120 && Angle<170)
					{
						BossLimitFaceNum++;
					}
				}
			}
		}
		if (SideFaceNum>1 && BossLimitFaceNum>1 && Is_Face_in_lpFace(EachFace, SealFace_DirectionRib) == -1)
		{
			//UF_DISP_set_highlight(EachFace->Tag(),1);
			SealFace_DirectionRib.push_back(EachFace);
		}
	}

}

//轮廓特征识别20230203-mql
//外轮廓特征，内轮廓特征：没有凹连接
//环特征三个零件中只有一处，暂时定义为平面和（柱面/锥面）相邻组成的环
void OutlineRrecognition(std::vector<Face*>AllFaces, Face *Begin_Face, Face* BottomPlaneFace, Face* TopPlaneFace,
	std::vector<Face*>&SeelFaces_OutlineVector, std::vector<Face*>&SeelFaces_OutlineVector1)
{

	int num0 = AllFaces.size() - 1;
	//MT_set_progress_range(&aa, 0, num0);
	int NowNum = 0;
	//===============================DEFINE PROGRESS END===================================================================
	//std::vector<Face*>Corner_FaceVector;
	std::vector<Face*>SeedFaces;
	SeedFaces.clear();
	Vector3d _Approach_Normal;
	get_MainNormal_fr_plane(Begin_Face, _Approach_Normal);
	for (std::vector<Face*>::size_type i = 0; i != AllFaces.size(); i++)
	{
		/*MT_set_progress_pos(&aa, NowNum++);
		UF_UI_set_status("轮廓特征识别");*/
		Face* Each_Face;
		Each_Face = AllFaces[i];

		if (Each_Face->SolidFaceType() == Face::FaceTypeSurfaceOfRevolution)
		{
			double Rad;
			get_face_maxRad(Each_Face, Rad);
			std::vector<Edge*>EachFace_EdgeVector;
			EachFace_EdgeVector = Each_Face->GetEdges();
			if (Rad<30 && EachFace_EdgeVector.size()>13)
			{
				SeelFaces_OutlineVector.push_back(Each_Face);
				//UF_DISP_set_highlight(Each_Face->Tag(),1);
			}

		}
		else if (Each_Face->SolidFaceType() == Face::FaceTypeCylindrical || Each_Face->SolidFaceType() == Face::FaceTypeConical)
		{
			int type = 0;       //判断面类型
			int norm_dir = 0;   //外R1，内R-1
			double point[3];  //轴中心 点
			double dir[3];    //轴方向
			double box[6];    //极点坐标
			double radius;    //半径 圆锥=最小半径 + 最大半径 /2
			double rad_data;
			UF_MODL_ask_face_data(Each_Face->Tag(), &type, point, dir, box, &radius, &rad_data, &norm_dir); // 分析面
			if (radius >200 && norm_dir == -1)
			{
				int num = Get_FullCircle_Num(Each_Face, _Approach_Normal);
				if (num>0)
				{
					SeedFaces.push_back(Each_Face);
					std::vector<Face*> OuterAdjacentFaces;
					get_External_Adjacent_Faces(Each_Face, OuterAdjacentFaces);

					bool IsTopFace = true;
					for (std::vector<Face*>::size_type k = 0; k != OuterAdjacentFaces.size(); k++)
					{
						Face* EachOutFace = OuterAdjacentFaces[k];
						if (EachOutFace->Tag() == TopPlaneFace->Tag() || EachOutFace->Tag() == BottomPlaneFace->Tag())
						{
							IsTopFace = false;
							break;
						}
						else
						{
							if (EachOutFace->SolidFaceType() == Face::FaceTypeCylindrical || EachOutFace->SolidFaceType() == Face::FaceTypeConical
								|| EachOutFace->SolidFaceType() == Face::FaceTypeSurfaceOfRevolution)
							{
								std::vector<Edge*> EdgeVector;
								get_pEdge_fr_fafa(Each_Face, EachOutFace, EdgeVector);

								double angle;
								get_FaceAngle(Each_Face, EachOutFace, EdgeVector[0], angle);
								angle = angle / PI * 180;

								if (angle < 180)
								{
									IsTopFace = false;
									break;
								}
							}
						}
					}

					if (IsTopFace)
					{
						SeelFaces_OutlineVector1.push_back(Each_Face);
					}
				}
			}
		}
	}
}

//创建轮廓特征
void CreateOutlineFeature(Face *Begin_Face, std::vector<Face*>SeelFaces_OutlineVector, std::vector<Face*>&SeelFaces_OutlineVector1,
	std::vector<Outline*>&Feature_OutlineVector, std::vector<Outline*>&Feature_OutlineVector1)
{

	int num0 = SeelFaces_OutlineVector.size() + SeelFaces_OutlineVector1.size();
	//MT_set_progress_range(&aa, 0, num0);

	//==============================DEFINE PROGRESS END=================================================================
	Vector3d _Approach_Normal;
	get_MainNormal_fr_plane(Begin_Face, _Approach_Normal);
	std::vector<Face*>CornerFaceVector;
	for (std::vector<Face*>::size_type i = 0; i != SeelFaces_OutlineVector.size(); i++)
	{
		/*	MT_set_progress_pos(&aa, i);
		UF_UI_set_status("轮廓特征");*/
		Face *EachFace;
		EachFace = SeelFaces_OutlineVector[i];

		if (Is_Face_in_lpFace(EachFace, CornerFaceVector) != -1)
		{
			continue;
		}
		CornerFaceVector.push_back(EachFace);
		std::vector<Face*>EachAd_FaceVector;
		get_External_Adjacent_Faces(EachFace, EachAd_FaceVector);

		Outline *Feature = new Outline;
		Feature->BottomCornerFaceVector.push_back(EachFace);
		for (std::vector<Face*>::size_type j = 0; j != EachAd_FaceVector.size(); j++)
		{


			Face *EachAdFace;
			EachAdFace = EachAd_FaceVector[j];

			if (EachAdFace->SolidFaceType() == Face::FaceTypePlanar&&Is_Face_in_lpFace(EachAdFace, Feature->PlanerFaceVector) == -1)
			{
				Feature->PlanerFaceVector.push_back(EachAdFace);
			}
			else if (EachAdFace->SolidFaceType() == Face::FaceTypeConical&&Is_Face_in_lpFace(EachAdFace, Feature->ConicalFaceVector) == -1)
			{
				std::vector<Edge *>CorEdge;
				GetEdge_From_TwoFaces(EachAdFace, EachFace, CorEdge);
				double Length = CorEdge[0]->GetLength();
				if (Length<100)
				{
					Feature->ConicalFaceVector.push_back(EachAdFace);
				}

			}
		}
		//这里把另外一个底角面也输入
		if (Feature->PlanerFaceVector.size() != 0)
		{
			std::vector<Face*>OnePlanerAdFaceVector;
			get_External_Adjacent_Faces(Feature->PlanerFaceVector[0], OnePlanerAdFaceVector);
			for (std::vector<Face*>::size_type t = 0; t != OnePlanerAdFaceVector.size(); t++)
			{
				if (OnePlanerAdFaceVector[t]->SolidFaceType() == Face::FaceTypeSurfaceOfRevolution)
				{
					double Rad;
					get_face_maxRad(OnePlanerAdFaceVector[t], Rad);
					std::vector<Edge*>EachFace_EdgeVector;
					EachFace_EdgeVector = OnePlanerAdFaceVector[t]->GetEdges();
					if (Rad<30 && EachFace_EdgeVector.size()>13 && OnePlanerAdFaceVector[t] != EachFace)
					{
						CornerFaceVector.push_back(OnePlanerAdFaceVector[t]);
						Feature->BottomCornerFaceVector.push_back(OnePlanerAdFaceVector[t]);
					}
				}
			}
		}

		Feature_OutlineVector.push_back(Feature);
	}

	num0 = SeelFaces_OutlineVector.size() - 1;

	for (std::vector<Face*>::size_type i = 0; i != SeelFaces_OutlineVector1.size(); i++)
	{
		//MT_set_progress_pos(&aa, i);
		UF_UI_set_status("轮廓特征");
		Face *EachFace;
		EachFace = SeelFaces_OutlineVector1[i];
		std::vector<Face*>EachAd_FaceVector;
		get_External_Adjacent_Faces(EachFace, EachAd_FaceVector);

		Outline *Feature = new Outline;
		Feature->TopFaceVector.push_back(EachFace);

		for (std::vector<Face*>::size_type j = 0; j != EachAd_FaceVector.size(); j++)
		{


			Face *EachAdFace;
			EachAdFace = EachAd_FaceVector[j];

			if (EachAdFace->SolidFaceType() == Face::FaceTypePlanar || EachAdFace->SolidFaceType() == Face::FaceTypeConical ||
				EachAdFace->SolidFaceType() == Face::FaceTypeCylindrical || EachAdFace->SolidFaceType() == Face::FaceTypeParametric)
			{
				int num = Get_FullCircle_Num(EachAdFace, _Approach_Normal);
				if (num>0 && Is_Face_in_lpFace(EachAdFace, Feature->TopFaceVector) == -1)
				{
					Feature->SideFaceVector.push_back(EachAdFace);
				}
				else
				{
					Feature->RightFaceVector.push_back(EachAdFace);
				}
			}
			else if (EachAdFace->SolidFaceType() == Face::FaceTypeSurfaceOfRevolution)
			{
				int num = Get_FullCircle_Num(EachAdFace, _Approach_Normal);
				if (num>0)
				{
					Feature->BottomCornerFaceVector.push_back(EachAdFace);
				}
			}
		}

		//找侧面
		if (Feature->RightFaceVector.size() != 0)
		{
			for (std::vector<Face*>::size_type k = 0; k != Feature->RightFaceVector.size(); k++)
			{
				Face* EachRightFace = Feature->RightFaceVector[k];
				std::vector<Face*> RightAdjacentFaces;
				get_External_Adjacent_Faces(EachRightFace, RightAdjacentFaces);
				for (std::vector<Face*>::size_type t = 0; t != RightAdjacentFaces.size(); t++)
				{
					Face* EachAdjFace = RightAdjacentFaces[t];
					if (Is_Face_in_lpFace(EachAdjFace, Feature->RightFaceVector) == -1 &&
						Is_Face_in_lpFace(EachAdjFace, Feature->TopFaceVector) == -1)
					{
						Feature->SideFaceVector.push_back(EachAdjFace);
					}
				}
			}
		}

		//找底角面
		if (Feature->SideFaceVector.size() != 0)
		{
			for (std::vector<Face*>::size_type k = 0; k != Feature->SideFaceVector.size(); k++)
			{
				Face* EachSideFace = Feature->SideFaceVector[k];
				std::vector<Face*> SideAdjacentFaces;
				get_External_Adjacent_Faces(EachSideFace, SideAdjacentFaces);
				for (std::vector<Face*>::size_type t = 0; t != SideAdjacentFaces.size(); t++)
				{
					Face* EachAdjFace = SideAdjacentFaces[t];
					if (Is_Face_in_lpFace(EachAdjFace, Feature->BottomCornerFaceVector) == -1 &&
						Is_Face_in_lpFace(EachAdjFace, Feature->SideFaceVector) == -1 &&
						Is_Face_in_lpFace(EachAdjFace, Feature->RightFaceVector) == -1 &&
						Is_Face_in_lpFace(EachAdjFace, Feature->TopFaceVector) == -1)
					{
						Feature->BottomCornerFaceVector.push_back(EachAdjFace);
					}
				}
			}
		}
		Feature_OutlineVector1.push_back(Feature);
		//uc1601("识别到轮廓特征+1",1);
	}

}

//20230419 曲面槽筋识别
void CreateHookFaceRibFeature(std::vector<Face*>SealFace_HookFaceRib, std::vector<HookFaceRib*>&Feature_HookFaceRib, std::vector<DirectionRib*>&Feature_DirectionRib, Vector3d NormalVector)
{

	class FaceAndAdEdge
	{
	public:
		Face *EachAdFace0;
		Edge *Edge0;
	};

	int num0 = SealFace_HookFaceRib.size() - 1;

	int NowNum = 0;
	//===============================DEFINE PROGRESS END===================================================================


	for (std::vector<Face*>::size_type i = 0; i != SealFace_HookFaceRib.size(); i++)
	{
		Face *EachFace;
		EachFace = SealFace_HookFaceRib[i];

		std::vector<FaceAndAdEdge*>Faces_Edges;
		std::vector<Face*>EachAdFaces;
		std::vector<Edge*>EachEdges;

		HookFaceRib *Feature = new HookFaceRib;
		Feature->TopFaceVector.push_back(EachFace);

		std::vector<Face*> AdjacentFaces;
		get_External_Adjacent_Faces(EachFace, AdjacentFaces);
		for (std::vector<Face*>::size_type j = 0; j != AdjacentFaces.size(); j++)
		{
			Face* EachAdjFace = AdjacentFaces[j];

			if (EachAdjFace->SolidFaceType() == Face::FaceTypeParametric)
			{
				Feature->SideFaceVector.push_back(EachAdjFace);
			}
			else if (EachAdjFace->SolidFaceType() == Face::FaceTypeBlending)
			{
				double Rad;
				get_face_maxRad(EachAdjFace, Rad);
				if (Rad>1.500) //凸关系，为侧面
				{
					Feature->SideFaceVector.push_back(EachAdjFace);
				}
				else//凹关系为限制面
				{
					Feature->LimitFaceVector.push_back(EachAdjFace);
				}

				//uf_list_p_t edge_list;
				//UF_MODL_ask_face_edges(EachFace->Tag(), &edge_list);

				////获取链表的数量
				//int count;
				//UF_MODL_ask_list_count(edge_list, &count);

				//for (int j = 0; j < count; j++)
				//{
				//	//获取链表里的tag
				//	tag_t EdgeTag = NULL_TAG;
				//	UF_MODL_ask_list_item(edge_list, j, &EdgeTag);
				//	//获取边的光顺性
				//	logical is_smooth;
				//	UF_MODL_ask_edge_smoothness(EdgeTag, 0, &is_smooth);

				//	if (is_smooth == true)
				//	{
				//		//边找?
				//		uf_list_p_t face_list;
				//		UF_MODL_ask_edge_faces(EdgeTag, &face_list);

				//		//获得链表数量
				//		int face_count;
				//		UF_MODL_ask_list_count(face_list, &face_count);
				//		for (int k = 0; k < face_count; k++)
				//		{
				//			//获取链表?的tag
				//			tag_t FaceTag = NULL_TAG;
				//			UF_MODL_ask_list_item(face_list, k, &FaceTag);

				//			if (EachFace->Tag() != FaceTag)
				//			{
				//				NXObjectManager ObjectManager;
				//				TaggedObject* pTaggedObject = ObjectManager.Get(FaceTag);
				//				Face* pFace = dynamic_cast<Face*>(pTaggedObject);
				//				if (pFace->SolidFaceType() == Face::FaceTypeBlending )
				//				{
				//					Feature->LimitFaceVector.push_back(EachAdjFace);//光顺为限制面
				//				}
				//			}
				//		}
				//	}					
				//}		
				//if (Is_Face_in_lpFace(EachAdjFace,Feature->LimitFaceVector)==-1)
				//{
				//	Feature->SideFaceVector.push_back(EachAdjFace);
				//}
			}
			else if (EachAdjFace->SolidFaceType() == Face::FaceTypeSurfaceOfRevolution)
			{
				if (Is_Face_in_lpFace(EachAdjFace, SealFace_HookFaceRib) == -1)
				{
					Feature->LimitFaceVector.push_back(EachAdjFace);
				}
			}
			else if (EachAdjFace->SolidFaceType() == Face::FaceTypeCylindrical&&
				Is_Face_in_lpFace(EachAdjFace, SealFace_HookFaceRib) == -1 &&
				Is_Face_in_lpFace(EachAdjFace, Feature->LimitFaceVector) == -1)
			{
				int num = Get_FullCircle_Num2(EachAdjFace);
				if (num == 1)
				{
					Feature->LimitFaceVector.push_back(EachAdjFace);
				}
				else
				{
					Feature->SideFaceVector.push_back(EachAdjFace);
				}

			}
			else if (EachAdjFace->SolidFaceType() == Face::FaceTypePlanar&&
				Is_Face_in_lpFace(EachAdjFace, Feature->SideFaceVector) == -1)
			{
				EachEdges = EachAdjFace->GetEdges();
				if (EachEdges.size()>6)
				{
					Feature->LimitFaceVector.push_back(EachAdjFace);
				}
				else
				{
					Feature->SideFaceVector.push_back(EachAdjFace);
				}
			}
		}
		Feature_HookFaceRib.push_back(Feature);
	}

}