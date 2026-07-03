#include "SLHoleDrillingcmd.h"
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

//-----------------------------------------------------------------------------
// HoleDrillingCmd : constructor
//-----------------------------------------------------------------------------
SLHoleDrillingcmd::SLHoleDrillingcmd() :
TaggedObject()
{
}

SLHoleDrillingcmd::SLHoleDrillingcmd(AllFeatureHole allFeatureHolelist, HoleProcessParameter ParameterSet, Part* WorkPart, Body* WorkBody, std::vector<NXOpen::Face*> SelectedHoleInWallFaceList) :
TaggedObject()
{
	allFeatureHole = allFeatureHolelist;
	HoleDNList = allFeatureHolelist.ListOfDirectionNormalHole;
	HoleDRList = allFeatureHolelist.ListOfDirectionRightHole;
	HoleWNList = allFeatureHolelist.ListOfWallNormalHole;
	HoleWCCList = allFeatureHolelist.ListOfWallCircleCombineHole;
	HoleRCList = allFeatureHolelist.ListOfRibCombineHole;
	HoleBNList = allFeatureHolelist.ListOfBossNormalHole;
	HoleBORList = allFeatureHolelist.ListOfBossOneRightHole;
	HoleBOCList = allFeatureHolelist.ListOfBossOneCircleHole;
	HoleBDRList = allFeatureHolelist.ListOfBossDoubleRightHole;
	HoleBDCList = allFeatureHolelist.ListOfBossDoubleCircleHole;
	HoleBOROCRList = allFeatureHolelist.ListOfBossOneRightOneCircleHole;
	HoleBOList = allFeatureHolelist.ListOfBossOtherHole;
	//多层孔
	HoleBOWCList = allFeatureHolelist.ListOfBossOutsideWideCombineHole;
	HoleBMList = allFeatureHolelist.ListOfBossManyLayerHole;
	//盲孔
	HoleDBList = allFeatureHolelist.ListOfDirectionBlindHole;
	HoleBNBList = allFeatureHolelist.ListOfBossNormalBlindHole;
	HoleBRBList = allFeatureHolelist.ListOfBossRightBlindHole;
	//差凸台中圆角复合通孔、凸台内宽外窄复合通孔

	//HoleResultList = ListOfDirectionNormalHole;
	HoleParameterSet = ParameterSet;
	workPart = WorkPart;
	selectedBody = WorkBody;
	std::vector<Face*> selectedHoleInWallFaceList;
	selectedHoleInWallFaceList = SelectedHoleInWallFaceList;

	UF_initialize();
	HoleParameterSet.FaceSelect = true;

	if (selectedHoleInWallFaceList.size() != 0)
	{
		selectedFaceList.clear();
		selectedFaceOtherList.clear();//所选面相邻的面
		selectedTopFaceList.clear();
		selectedHoleNameList.clear();

		Index.clear();

		for (std::vector<Face*>::size_type i = 0; i != selectedHoleInWallFaceList.size(); i++)
		{
			Face* EachFace = selectedHoleInWallFaceList[i];
			tag_t EachFaceTag = EachFace->Tag();
			for (std::vector<DirectionNormalHole*>::size_type j = 0; j != HoleDNList.size(); j++)
			{
				DirectionNormalHole* EachHole = HoleDNList[j];
				Face* EachHoleInWallVector = EachHole->InWallVector[0];
				if (EachFaceTag == EachHoleInWallVector->Tag() )
				{
					//uc1601(EachHole->name, 1);
					selectedFaceList.push_back(EachHoleInWallVector);
					selectedFaceOtherList.push_back(EachHole->TopFaceVector[0]);
					selectedTopFaceList.push_back(EachHole->TopFaceVector[0]);
					selectedHoleNameList.push_back(EachHole->name);
					IfBlindList.push_back(0);
					Index.push_back(j);
				}
				else{};
			}
			for (std::vector<DirectionRightHole*>::size_type j = 0; j != HoleDRList.size(); j++)//倒斜角通孔
			{
				DirectionRightHole* EachHole = HoleDRList[j];
				Face* EachHoleInWallVector = EachHole->InWallVector[0];
				if (EachFaceTag == EachHoleInWallVector->Tag())
				{
					//uc1601(EachHole->name, 1);
					selectedFaceList.push_back(EachHoleInWallVector);
					selectedFaceOtherList.push_back(EachHole->RightVector[0]);
					selectedTopFaceList.push_back(EachHole->TopFaceVector[0]);
					selectedHoleNameList.push_back(EachHole->name);
					IfBlindList.push_back(0);
					Index.push_back(j);
				}
				else{};
			}
			for (std::vector<WallNormalHole*>::size_type j = 0; j != HoleWNList.size(); j++)
			{
				WallNormalHole* EachHole = HoleWNList[j];
				Face* EachHoleInWallVector = EachHole->InWallVector[0];
				if (EachFaceTag == EachHoleInWallVector->Tag())
				{
					//uc1601(EachHole->name, 1);
					selectedFaceList.push_back(EachHoleInWallVector);
					selectedFaceOtherList.push_back(EachHole->TopFaceVector[0]);
					selectedTopFaceList.push_back(EachHole->TopFaceVector[0]);
					selectedHoleNameList.push_back(EachHole->name);
					IfBlindList.push_back(0);
					Index.push_back(j);
				}
				else{};
			}
			for (std::vector<BossNormalHole*>::size_type j = 0; j != HoleBNList.size(); j++)//凸台无倒角角通孔
			{
				BossNormalHole* EachHole = HoleBNList[j];
				Face* EachHoleInWallVector = EachHole->InWallVector[0];
				if (EachFaceTag == EachHoleInWallVector->Tag())
				{
					//uc1601(EachHole->name, 1);
					selectedFaceList.push_back(EachHoleInWallVector);
					selectedFaceOtherList.push_back(EachHole->TopFaceVector[0]);
					selectedTopFaceList.push_back(EachHole->TopFaceVector[0]);
					selectedHoleNameList.push_back(EachHole->name);
					IfBlindList.push_back(0);
					Index.push_back(j);
				}
				else{};
			}
			for (std::vector<BossOneRightHole*>::size_type j = 0; j != HoleBORList.size(); j++)//凸台单侧斜角通孔
			{
				BossOneRightHole* EachHole = HoleBORList[j];
				Face* EachHoleInWallVector = EachHole->InWallVector[0];
				if (EachFaceTag == EachHoleInWallVector->Tag())
				{
					//uc1601(EachHole->name, 1);
					selectedFaceList.push_back(EachHoleInWallVector);
					selectedFaceOtherList.push_back(EachHole->RightVector[0]);
					selectedTopFaceList.push_back(EachHole->TopFaceVector[0]);
					selectedHoleNameList.push_back(EachHole->name);
					IfBlindList.push_back(0);
					Index.push_back(j);
				}
				else{};
			}
			for (std::vector<BossOneCircleHole*>::size_type j = 0; j != HoleBOCList.size(); j++)//凸台单侧圆角通孔
			{
				BossOneCircleHole* EachHole = HoleBOCList[j];
				Face* EachHoleInWallVector = EachHole->InWallVector[0];
				if (EachFaceTag == EachHoleInWallVector->Tag())
				{
					//uc1601(EachHole->name, 1);
					selectedFaceList.push_back(EachHoleInWallVector);
					selectedFaceOtherList.push_back(EachHole->CircleVector[0]);
					selectedTopFaceList.push_back(EachHole->TopFaceVector[0]);
					selectedHoleNameList.push_back(EachHole->name);
					IfBlindList.push_back(0);
					Index.push_back(j);
				}
				else{};
			}
			for (std::vector<BossDoubleRightHole*>::size_type j = 0; j != HoleBDRList.size(); j++)//凸台双侧斜角通孔
			{
				BossDoubleRightHole* EachHole = HoleBDRList[j];
				Face* EachHoleInWallVector = EachHole->InWallVector[0];
				if (EachFaceTag == EachHoleInWallVector->Tag())
				{
					//uc1601(EachHole->name, 1);
					selectedFaceList.push_back(EachHoleInWallVector);
					selectedFaceOtherList.push_back(EachHole->TopRightVector[0]);
					selectedTopFaceList.push_back(EachHole->TopFaceVector[0]);
					selectedHoleNameList.push_back(EachHole->name);
					IfBlindList.push_back(0);
					Index.push_back(j);
				}
				else{};
			}
			for (std::vector<BossDoubleCircleHole*>::size_type j = 0; j != HoleBDCList.size(); j++)//凸台双侧圆角通孔
			{
				BossDoubleCircleHole* EachHole = HoleBDCList[j];
				Face* EachHoleInWallVector = EachHole->InWallVector[0];
				if (EachFaceTag == EachHoleInWallVector->Tag())
				{
					//uc1601(EachHole->name, 1);
					selectedFaceList.push_back(EachHoleInWallVector);
					selectedFaceOtherList.push_back(EachHole->TopCircleVector[0]);
					selectedTopFaceList.push_back(EachHole->TopFaceVector[0]);
					selectedHoleNameList.push_back(EachHole->name);
					IfBlindList.push_back(0);
					Index.push_back(j);
				}
				else{};
			}
			for (std::vector<BossOneRightOneCircleHole*>::size_type j = 0; j != HoleBOROCRList.size(); j++)//凸台单侧斜角单侧圆角通孔
			{
				BossOneRightOneCircleHole* EachHole = HoleBOROCRList[j];
				Face* EachHoleInWallVector = EachHole->InWallVector[0];
				if (EachFaceTag == EachHoleInWallVector->Tag())
				{
					//uc1601(EachHole->name, 1);
					selectedFaceList.push_back(EachHoleInWallVector);
					selectedFaceOtherList.push_back(EachHole->CircleVector[0]);
					selectedTopFaceList.push_back(EachHole->TopFaceVector[0]);
					selectedHoleNameList.push_back(EachHole->name);
					IfBlindList.push_back(0);
					Index.push_back(j);
				}
				else{};
			}
			for (std::vector<Other3Hole*>::size_type j = 0; j != HoleBOList.size(); j++)//
			{
				Other3Hole* EachHole = HoleBOList[j];
				Face* EachHoleInWallVector = EachHole->InWallVector[0];
				if (EachFaceTag == EachHoleInWallVector->Tag())
				{
					//uc1601(EachHole->name, 1);
					selectedFaceList.push_back(EachHoleInWallVector);
					selectedFaceOtherList.push_back(EachHole->OuterVector[0]);
					selectedTopFaceList.push_back(EachHole->OuterVector[0]);
					selectedHoleNameList.push_back(EachHole->name);
					IfBlindList.push_back(0);
					Index.push_back(j);
				}
				else{};
			}
			//多层孔
			for (std::vector<BossOutsideWideCombineHole*>::size_type j = 0; j != HoleBOWCList.size(); j++)//凸台外宽内窄复合通孔
			{
				BossOutsideWideCombineHole* EachHole = HoleBOWCList[j];
				Face* EachHoleInWallVector = EachHole->TopWallVector[0];
				if (EachFaceTag == EachHoleInWallVector->Tag())
				{
					//uc1601(EachHole->name, 1);
					selectedFaceList.push_back(EachHoleInWallVector);
					selectedFaceOtherList.push_back(EachHole->TopFaceVector[0]);
					selectedTopFaceList.push_back(EachHole->TopFaceVector[0]);		
					selectedHoleNameList.push_back(EachHole->name);
					IfBlindList.push_back(0);
					Index.push_back(j);
				}
				else{};
			}
			for (std::vector<BossOutsideWideCombineHole*>::size_type j = 0; j != HoleBOWCList.size(); j++)//凸台外宽内窄复合通孔
			{
				BossOutsideWideCombineHole* EachHole = HoleBOWCList[j];
				Face* EachHoleInWallVector = EachHole->TopWallVector[0];
				Face* EachHoleInWallVector1 = EachHole->BottomWallVector[0];
				if (EachFaceTag == EachHoleInWallVector->Tag())
				{
					//uc1601(EachHole->name, 1);
					selectedFaceList1.push_back(EachHoleInWallVector1);
					selectedFaceOtherList1.push_back(EachHole->PlanerVector[0]);
					selectedTopFaceList1.push_back(EachHole->TopFaceVector[0]);
					selectedHoleNameList1.push_back(EachHole->name);
					IfBlindList.push_back(0);
					Index.push_back(j);
				}
				else{};
			}
			for (std::vector<WallCircleCombineHole*>::size_type j = 0; j != HoleWCCList.size(); j++)//壁面复合通孔
			{
				WallCircleCombineHole* EachHole = HoleWCCList[j];
				Face* EachHoleInWallVector = EachHole->TopInWallVector[0];
				if (EachFaceTag == EachHoleInWallVector->Tag())
				{
					//uc1601(EachHole->name, 1);
					selectedFaceList.push_back(EachHoleInWallVector);
					selectedFaceOtherList.push_back(EachHole->TopFaceVector[0]);
					selectedTopFaceList.push_back(EachHole->TopFaceVector[0]);
					selectedHoleNameList.push_back(EachHole->name);
					IfBlindList.push_back(0);
					Index.push_back(j);
				}
				else{};
			}
			for (std::vector<WallCircleCombineHole*>::size_type j = 0; j != HoleWCCList.size(); j++)//壁面复合通孔
			{
				WallCircleCombineHole* EachHole = HoleWCCList[j];
				Face* EachHoleInWallVector = EachHole->TopInWallVector[0];
				Face* EachHoleInWallVector1 = EachHole->BottomInWallVector[0];
				if (EachFaceTag == EachHoleInWallVector->Tag())
				{
					//uc1601(EachHole->name, 1);
					selectedFaceList1.push_back(EachHoleInWallVector1);
					selectedFaceOtherList1.push_back(EachHole->PlanerVector[0]);
					selectedTopFaceList1.push_back(EachHole->TopFaceVector[0]);
					selectedHoleNameList1.push_back(EachHole->name);
					IfBlindList.push_back(0);
					Index.push_back(j);
				}
				else{};
			}
			for (std::vector<RibCombineHole*>::size_type j = 0; j != HoleRCList.size(); j++)//筋面复合通孔
			{
				RibCombineHole* EachHole = HoleRCList[j];
				Face* EachHoleInWallVector = EachHole->InWallVector[0];
				if (EachFaceTag == EachHoleInWallVector->Tag())
				{
					//uc1601(EachHole->name, 1);//未进入该循环
					selectedFaceList.push_back(EachHoleInWallVector);
					selectedFaceOtherList.push_back(EachHole->PlanerVector[0]);
					selectedTopFaceList.push_back(EachHole->TopFaceVector[0]);
					selectedHoleNameList.push_back(EachHole->name);
					IfBlindList.push_back(0);
					Index.push_back(j);
				}
				else{};
			}
			for (std::vector<BossManyLayerHole*>::size_type j = 0; j != HoleBMList.size(); j++)//凸台多层通孔
			{
				BossManyLayerHole* EachHole = HoleBMList[j];
				Face* EachHoleInWallVector = EachHole->InWallVector[0];
				if (EachFaceTag == EachHoleInWallVector->Tag())
				{
					//uc1601(EachHole->name, 1);
					selectedFaceList.push_back(EachHoleInWallVector);
					selectedFaceOtherList.push_back(EachHole->PlanerVector[0]);
					selectedTopFaceList.push_back(EachHole->TopFaceVector[0]);
					selectedHoleNameList.push_back(EachHole->name);
					IfBlindList.push_back(0);
					Index.push_back(j);
				}
				else{};
			}

			//#####################################################################盲孔
			//std::vector<DirectionBlindHole*> HoleDBList;
			for (std::vector<DirectionBlindHole*>::size_type j = 0; j != HoleDBList.size(); j++)
			{
				DirectionBlindHole* EachHole = HoleDBList[j];
				Face* EachHoleInWallVector = EachHole->InWallVector[0];
				if (EachFaceTag == EachHoleInWallVector->Tag())
				{
					//uc1601(EachHole->name, 1);
					selectedFaceList.push_back(EachHoleInWallVector);
					selectedFaceOtherList.push_back(EachHole->TopFaceVector[0]);
					selectedTopFaceList.push_back(EachHole->TopFaceVector[0]);
					selectedHoleNameList.push_back(EachHole->name);
					IfBlindList.push_back(1);
					Index.push_back(j);
				}
				else{};
			}
			//std::vector<BossNormalBlindHole*> HoleBNBList;
			for (std::vector<BossNormalBlindHole*>::size_type j = 0; j != HoleBNBList.size(); j++)
			{
				BossNormalBlindHole* EachHole = HoleBNBList[j];
				Face* EachHoleInWallVector = EachHole->InWallVector[0];
				if (EachFaceTag == EachHoleInWallVector->Tag())
				{
					//uc1601(EachHole->name, 1);
					selectedFaceList.push_back(EachHoleInWallVector);
					selectedFaceOtherList.push_back(EachHole->TopFaceVector[0]);
					selectedTopFaceList.push_back(EachHole->TopFaceVector[0]);
					selectedHoleNameList.push_back(EachHole->name);
					IfBlindList.push_back(1);
					Index.push_back(j);
				}
				else{};
			}
			//std::vector<BossRightBlindHole*> HoleBRBList;
			for (std::vector<BossRightBlindHole*>::size_type j = 0; j != HoleBRBList.size(); j++)
			{
				BossRightBlindHole* EachHole = HoleBRBList[j];
				Face* EachHoleInWallVector = EachHole->InWallVector[0];
				if (EachFaceTag == EachHoleInWallVector->Tag())
				{
					//uc1601(EachHole->name, 1);
					selectedFaceList.push_back(EachHoleInWallVector);
					selectedFaceOtherList.push_back(EachHole->RightVector[0]);
					selectedTopFaceList.push_back(EachHole->TopFaceVector[0]);
					selectedHoleNameList.push_back(EachHole->name);
					IfBlindList.push_back(1);
					Index.push_back(j);
				}
				else{};
			}
		}

		AllDrGeo1.clear();
		for (std::vector<Face*>::size_type k = 0; k != selectedFaceList.size(); k++)
		{
			DrGeo drGeo1;
			drGeo1.selectedHoleTop = selectedTopFaceList[k];
			drGeo1.selectedHoleInWall = selectedFaceList[k];
			drGeo1.selectedHoleOther = selectedFaceOtherList[k];
			drGeo1.selectedHoleName = selectedHoleNameList[k];
			AllDrGeo1.push_back(drGeo1);			
		}

		AllDrGeo2.clear();
		for (std::vector<Face*>::size_type k = 0; k != selectedFaceList1.size(); k++)
		{
			DrGeo drGeo2;
			drGeo2.selectedHoleTop = selectedTopFaceList1[k];
			drGeo2.selectedHoleInWall = selectedFaceList1[k];
			drGeo2.selectedHoleOther = selectedFaceOtherList1[k];
			drGeo2.selectedHoleName = selectedHoleNameList1[k];
			AllDrGeo2.push_back(drGeo2);
		}

		//孔分类
		holeClassification(AllDrGeo1);
		holeClassification2(AllDrGeo2);
		int OperatorType;
		int NumofOperator = 0;
		//#############################################################所选孔进行的工序
		if (HoleParameterSet.SpotDrill)//中心钻
		{
			NumofOperator++;
			//uc1601("中心钻！", 1);
			if (HoleParameterSet.IfACsys)//全局坐标系
			{
				HoleSpotDrillingDecision(selectedFaceList, selectedFaceOtherList, selectedTopFaceList, selectedHoleNameList);//通孔
			}
			else
			{
				OperatorType = 0;
				OperatorDecision(AxisDrGeo, BossDrGeo, WallRibDrGeo, OperatorType);	
			}			
		}
		else{};
		if (HoleParameterSet.Drill)//钻
		{
			NumofOperator++;
			//uc1601("钻！", 1);
			OperatorType = 1;
			OperatorDecision(AxisDrGeo, BossDrGeo, WallRibDrGeo, OperatorType);
		}
		else{};
		if (HoleParameterSet.SpotFacing)//锪
		{
			NumofOperator++;
			OperatorType = 2;
			//uc1601("锪！", 1);
			OperatorDecision(AxisDrGeo, BossDrGeo, WallRibDrGeo, OperatorType);
		}
		else{};
		if (HoleParameterSet.Boring)//镗
		{
			NumofOperator++;
			OperatorType = 3;
			//uc1601("镗！", 1);
			OperatorDecision(AxisDrGeo, BossDrGeo, WallRibDrGeo, OperatorType);
		}
		else{};
		if (HoleParameterSet.Reaming)//铰
		{
			NumofOperator++;
			OperatorType = 4;
			//uc1601("铰！", 1);
			OperatorDecision(AxisDrGeo, BossDrGeo, WallRibDrGeo, OperatorType);

		}	
		else{};
		if (HoleParameterSet.HoleMilling)//孔铣
		{
			NumofOperator++;
			OperatorType = 5;
			//uc1601("孔铣！", 1);
			OperatorDecision(AxisDrGeo, BossDrGeo, WallRibDrGeo, OperatorType);
			OperatorDecision(AxisDrGeo2, BossDrGeo2, WallRibDrGeo2, OperatorType);
			
		}
		else{};
		if (HoleParameterSet.THreadMilling)//铣螺纹
		{
			NumofOperator++;
			OperatorType = 6;
			//uc1601("铣螺纹！", 1);
			OperatorDecision(AxisDrGeo, BossDrGeo, WallRibDrGeo, OperatorType);
			OperatorDecision(AxisDrGeo2, BossDrGeo2, WallRibDrGeo2, OperatorType);
			//if (AllDrGeo2.size() != 0)
			//{
			//	OperatorDecision(AxisDrGeo2, BossDrGeo2, WallRibDrGeo2, OperatorType);
			//}
		}
		else{};
		if (NumofOperator =0 )
		{
			uc1601("未选择加工操作", 1);
		}
		else{};
		//uc1601("finish111", 1);

	}
	else
	{
		uc1601("未选择加工面", 1);
	}
	UF_terminate();

	//分析两个面是否有交线    ask shared edge 

}

SLHoleDrillingcmd::~SLHoleDrillingcmd()
{

}

void SLHoleDrillingcmd::HoleSpotDrillingDecision(std::vector<Face*> selectedFaceList, std::vector<Face*> selectedFaceOtherList, std::vector<Face*> selectedTopFaceList, std::vector<char*> selectedHoleNameList)
{
	//uc1601("HoleSpotDrillingDecision!", 1);
	char *BossHole = "凸台";
	char *BossotherHole = "其它孔";
	char *RibHole = "筋面";
	char *WallHole = "壁面";
	char *AxisHole2 = "轴向";

	std::vector<Face*> DMTopFaceList;//端面孔的顶面
	std::vector<Face*> DMInWallFaceList;//端面孔的侧面
	std::vector<Face*> QTTopFaceList;//其它孔的顶面
	std::vector<Face*> QTInWallFaceList;//其它孔的侧面

	std::vector<std::vector<Face*>>  AllInWallVectorList;//所有侧面孔向量的列表
	std::vector<std::vector<Face*>>  AllTopVectorList;//所有顶面孔向量的列表
	AllInWallVectorList.clear();
	AllTopVectorList.clear();

	for (std::vector<Face*>::size_type i = 0; i != selectedFaceList.size(); i++)
	{
		NXOpen::Face* HoleTopFace = selectedTopFaceList[i];//取出每一个孔顶面;
		tag_t HoleTopFaceTag = HoleTopFace->Tag();
		NXOpen::Face* HoleInWallFace = selectedFaceList[i];//取出每一个孔壁面;
		tag_t HoleInWallFaceTag = HoleInWallFace->Tag();
		char *Eachname = selectedHoleNameList[i];

		if (strstr(Eachname, AxisHole2) != NULL)//轴向
		{
			DMTopFaceList.push_back(HoleTopFace);
			DMInWallFaceList.push_back(HoleInWallFace);
		}
		else
		{
			QTTopFaceList.push_back(HoleTopFace);
			QTInWallFaceList.push_back(HoleInWallFace);
		}
	}

	AllInWallVectorList.push_back(DMInWallFaceList);
	AllInWallVectorList.push_back(QTInWallFaceList);
	AllTopVectorList.push_back(DMTopFaceList);
	AllTopVectorList.push_back(QTTopFaceList);


	for (std::vector<std::vector<Face*>>::size_type i = 0; i != AllInWallVectorList.size(); i++)
	{
		CAM::NCGroup *nCGroup1(dynamic_cast<CAM::NCGroup *>(workPart->CAMSetup()->CAMGroupCollection()->FindObject(HoleParameterSet.PROGRAM)));
		CAM::Method *method1(dynamic_cast<CAM::Method *>(workPart->CAMSetup()->CAMGroupCollection()->FindObject(HoleParameterSet.METHOD)));
		CAM::Tool *tool1(dynamic_cast<CAM::Tool *>(workPart->CAMSetup()->CAMGroupCollection()->FindObject(HoleParameterSet.ToolMsg.ToolName)));			
		CAM::Operation *operation1;	
		CAM::OrientGeometry *orientGeometry33(dynamic_cast<CAM::OrientGeometry *>(workPart->CAMSetup()->CAMGroupCollection()->FindObject("MCS_MILL")));//"MCS_G54",MCSName,Eachname,mcsName，凸台94806

		
		int EachIndex = Index[i];
		char* StringIndex = new char;
		sprintf(StringIndex, "%d", EachIndex);
		NXString NXStringIndex = StringIndex;
		delete StringIndex;
		NXString OperationName = "HoleSpotDrilling." + NXStringIndex;

		//创建操作;
		try
		{
			operation1 = workPart->CAMSetup()->CAMOperationCollection()->Create(nCGroup1, method1, tool1, orientGeometry33, "hole_making", "SPOT_DRILLING", NXOpen::CAM::OperationCollection::UseDefaultNameFalse, OperationName); // "DRILLING"
		}
		catch (...)
		{
			operation1 = workPart->CAMSetup()->CAMOperationCollection()->Create(nCGroup1, method1, tool1, orientGeometry33, "hole_making", "SPOT_DRILLING", NXOpen::CAM::OperationCollection::UseDefaultNameTrue, OperationName);
		}
		NXOpen::CAM::HoleDrilling *holeDrilling1(dynamic_cast<NXOpen::CAM::HoleDrilling *>(operation1));
		NXOpen::CAM::HoleDrillingBuilder *holeDrillingBuilder1;
		holeDrillingBuilder1 = workPart->CAMSetup()->CAMOperationCollection()->CreateHoleDrillingBuilder(holeDrilling1);
		NXOpen::CAM::HoleMachiningCutParameters *holeMachiningCutParameters1;
		holeMachiningCutParameters1 = holeDrillingBuilder1->CuttingParameters();
		NXOpen::CAM::FBM::FeatureGeometry *featureGeometry1;
		featureGeometry1 = holeDrillingBuilder1->GetFeatureGeometry();

		NXOpen::CAM::FBM::MachiningFeatureGeometry *machiningFeatureGeometry1(dynamic_cast<NXOpen::CAM::FBM::MachiningFeatureGeometry *>(featureGeometry1));

		
		//创建孔几何
		NXOpen::CAM::GeometrySetList *geometrySetList1;
		geometrySetList1 = machiningFeatureGeometry1->GeometryList();
		NXOpen::CAM::CAMFeature *nullNXOpen_CAM_CAMFeature(NULL);

		std::vector<Face*> InWallVectorList = AllInWallVectorList[i];
		for (std::vector<Face*>::size_type k = 0; k != InWallVectorList.size(); k++)
		{
			NXOpen::CAM::FBM::FeatureSet *featureSet1;
			featureSet1 = machiningFeatureGeometry1->AddFeatureSet(nullNXOpen_CAM_CAMFeature, "NXHOLECENTER");//钻操作换成"NXHOLE"

			geometrySetList1->Append(featureSet1);

			featureSet1->SetAngleToleranceEdges(0.0);
			featureSet1->SetIntol(0.0);
			featureSet1->SetOuttol(0.0);

			//赋值孔内壁面
			std::vector<NXOpen::NXObject *> entities1(1);
			entities1[0] = InWallVectorList[k];
			NXOpen::CAM::FBM::Feature *feature1;
			feature1 = featureSet1->CreateFeature(entities1);
		}

		//循环类型
		switch (HoleParameterSet.Cycling)
		{
		case 0:
			holeDrillingBuilder1->CycleTable()->SetCycleType("Drill");
			break;
		case 1:
			holeDrillingBuilder1->CycleTable()->SetCycleType("Drill,Text");
			break;
		case 2:
			holeDrillingBuilder1->CycleTable()->SetCycleType("Drill,Csink");
			break;
		case 3:
			holeDrillingBuilder1->CycleTable()->SetCycleType("Drill,Deep");
			break;
		case 4:
			holeDrillingBuilder1->CycleTable()->SetCycleType("Drill,Deep,BreakChip");
			break;
		case 5:
			holeDrillingBuilder1->CycleTable()->SetCycleType("Drill,Tap");
			break;
		case 6:
			holeDrillingBuilder1->CycleTable()->SetCycleType("Drill,Tap,Float");
			break;
		case 7:
			holeDrillingBuilder1->CycleTable()->SetCycleType("Drill,Tap,Deep");
			break;
		case 8:
			holeDrillingBuilder1->CycleTable()->SetCycleType("Drill,Tap,BreakChip");
			break;
		case 9:
			holeDrillingBuilder1->CycleTable()->SetCycleType("Drill,Bore");
			break;
		case 10:
			holeDrillingBuilder1->CycleTable()->SetCycleType("Drill,Bore,Drag");
			break;
		case 11:
			holeDrillingBuilder1->CycleTable()->SetCycleType("Drill,Bore,Nodrag");
			break;
		case 12:
			holeDrillingBuilder1->CycleTable()->SetCycleType("Drill,Bore,Back");
			break;
		case 13:
			holeDrillingBuilder1->CycleTable()->SetCycleType("Drill,Bore,Manual");
			break;
		default:
			holeDrillingBuilder1->CycleTable()->SetCycleType("Drill");
			break;
		}


		//偏置
		NXOpen::CAM::HoleDrillingCutParameters *holeDrillingCutParameters1(dynamic_cast<NXOpen::CAM::HoleDrillingCutParameters *>(holeMachiningCutParameters1));
		NXOpen::CAM::VerticalPosition *verticalPosition1;
		holeDrillingCutParameters1->BottomOffset()->SetDistance(HoleParameterSet.MBottomOffset);//底部
		NXOpen::CAM::VerticalPosition *verticalPosition2;
		holeDrillingCutParameters1->RaptoOffset()->SetDistance(HoleParameterSet.MRaptoOffset);//平行
		//顶部
		holeDrillingBuilder1->CuttingParameters()->TopOffset()->SetDistance(HoleParameterSet.MTopOffset);//

		NXObject *nXObject2 = holeDrillingBuilder1->Commit();

		//切削参数
		NXOpen::CAM::HoleDrillingBuilder *holeDrillingBuilder2;
		holeDrillingBuilder2 = workPart->CAMSetup()->CAMOperationCollection()->CreateHoleDrillingBuilder(holeDrilling1);

		holeDrillingBuilder2->FeedsBuilder()->SpindleRpmBuilder()->SetValue(HoleParameterSet.SpindleSpeed);
		holeDrillingBuilder2->FeedsBuilder()->FeedCutBuilder()->SetValue(HoleParameterSet.Machining);
		NXObject *nXObject1 = holeDrillingBuilder2->Commit();

//生成刀轨
		std::vector<CAM::CAMObject *> objects1(1);
		objects1[0] = operation1;
		workPart->CAMSetup()->GenerateToolPath(objects1);
		UF_UI_ONT_refresh();
	}
}

void SLHoleDrillingcmd::HoleMillingDecisioncmd(char* mcsname, std::vector<Face*> InWallVectorList)
{
	CAM::NCGroup *nCGroup1(dynamic_cast<CAM::NCGroup *>(workPart->CAMSetup()->CAMGroupCollection()->FindObject(HoleParameterSet.PROGRAM)));
	CAM::Method *method1(dynamic_cast<CAM::Method *>(workPart->CAMSetup()->CAMGroupCollection()->FindObject(HoleParameterSet.HMMETHOD)));
	CAM::OrientGeometry *orientGeometry1(dynamic_cast<CAM::OrientGeometry *>(workPart->CAMSetup()->CAMGroupCollection()->FindObject(mcsname)));//"MCS_G54",MCSName,Eachname,mcsName，凸台94806

	CAM::Tool *tool1(dynamic_cast<CAM::Tool *>(workPart->CAMSetup()->CAMGroupCollection()->FindObject(HoleParameterSet.HMToolMsg.ToolName)));

	CAM::Operation *operation1;

	tag_t HoleInWallFaceTag = InWallVectorList[0]->Tag();
	std::string tagStr = std::to_string(HoleInWallFaceTag);
	NXString nxString(tagStr.c_str());
	NXString OperationName = "HoleMilling." + nxString;

	//创建操作;
	try
	{
		operation1 = workPart->CAMSetup()->CAMOperationCollection()->Create(nCGroup1, method1, tool1, orientGeometry1, "hole_making", "HOLE_MILLING", CAM::OperationCollection::UseDefaultNameFalse, OperationName);
	}
	catch (...)
	{
		operation1 = workPart->CAMSetup()->CAMOperationCollection()->Create(nCGroup1, method1, tool1, orientGeometry1, "hole_making", "HOLE_MILLING", CAM::OperationCollection::UseDefaultNameTrue, OperationName);
	}//发生访问冲突


	NXOpen::CAM::CylinderMilling *cylinderMilling1(dynamic_cast<NXOpen::CAM::CylinderMilling *>(operation1));
	NXOpen::CAM::CylinderMillingBuilder *cylinderMillingBuilder1;
	cylinderMillingBuilder1 = workPart->CAMSetup()->CAMOperationCollection()->CreateCylinderMillingBuilder(cylinderMilling1);
	NXOpen::CAM::HoleMachiningCutParameters *holeMachiningCutParameters1;
	holeMachiningCutParameters1 = cylinderMillingBuilder1->CuttingParameters();
	NXOpen::CAM::FBM::FeatureGeometry *featureGeometry1;
	featureGeometry1 = cylinderMillingBuilder1->GetFeatureGeometry();

	NXOpen::CAM::FBM::MachiningFeatureGeometry *machiningFeatureGeometry1(dynamic_cast<NXOpen::CAM::FBM::MachiningFeatureGeometry *>(featureGeometry1));

	//创建孔几何
	NXOpen::CAM::GeometrySetList *geometrySetList1;
	geometrySetList1 = machiningFeatureGeometry1->GeometryList();
	NXOpen::CAM::CAMFeature *nullNXOpen_CAM_CAMFeature(NULL);
	for (std::vector<Face*>::size_type k = 0; k != InWallVectorList.size(); k++)
	{
		NXOpen::CAM::FBM::FeatureSet *featureSet1;
		featureSet1 = machiningFeatureGeometry1->AddFeatureSet(nullNXOpen_CAM_CAMFeature, "NXHOLE");//钻操作换成"NXHOLE"

		geometrySetList1->Append(featureSet1);

		featureSet1->SetAngleToleranceEdges(0.0);
		featureSet1->SetIntol(0.0);
		featureSet1->SetOuttol(0.0);

		//赋值孔内壁面
		std::vector<NXOpen::NXObject *> entities1(1);
		entities1[0] = InWallVectorList[k];
		NXOpen::CAM::FBM::Feature *feature1;
		feature1 = featureSet1->CreateFeature(entities1);
	}

	//切削模式，HMCuttingType，螺旋1、螺旋2、螺旋/平面螺旋、圆形
	//CutPatternTypesSpiral，CutPatternTypesCircular,CutPatternTypesHelical,CutPatternTypesHelicalAndSpiral
	switch (HoleParameterSet.HMCuttingType)
	{
	case 0:
		cylinderMillingBuilder1->SetCutPattern(NXOpen::CAM::CylinderMillingBuilder::CutPatternTypesHelical);
		break;
	case 1:
		cylinderMillingBuilder1->SetCutPattern(NXOpen::CAM::CylinderMillingBuilder::CutPatternTypesSpiral);
		break;
	case 2:
		cylinderMillingBuilder1->SetCutPattern(NXOpen::CAM::CylinderMillingBuilder::CutPatternTypesHelicalAndSpiral);
		break;
	case 3:
		cylinderMillingBuilder1->SetCutPattern(NXOpen::CAM::CylinderMillingBuilder::CutPatternTypesCircular);
		break;
	}


	//轴向螺距
	NXOpen::CAM::InheritableToolDepBuilder *inheritableToolDepBuilder1;
	inheritableToolDepBuilder1 = cylinderMillingBuilder1->AxialDistance();
	switch (HoleParameterSet.HMAPIntent)
	{
	case 0:
		inheritableToolDepBuilder1->SetIntent(NXOpen::CAM::ParamValueIntentToolDep);   //刀具百分比
		break;
	case 1:
		inheritableToolDepBuilder1->SetIntent(NXOpen::CAM::ParamValueIntentPartUnits);//mm
		break;
	}
	inheritableToolDepBuilder1->SetValue(HoleParameterSet.HMAxisPitchometer);
	//轴向步距，恒定、多个、刀路
	cylinderMillingBuilder1->AxialStepover()->SetStepoverType(NXOpen::CAM::StepoverBuilder::StepoverTypesNumber);//StepoverTypesNumber,StepoverTypesMultiple，StepoverTypesConstant
	cylinderMillingBuilder1->AxialStepover()->SetNumberOfStepovers(HoleParameterSet.HMNumOfCuttingStep);//刀路数

	//径向最大距离
	switch (HoleParameterSet.HMSMDIntent)
	{
	case 0:
		cylinderMillingBuilder1->RadialStepover()->DistanceBuilder()->SetIntent(NXOpen::CAM::ParamValueIntentToolDep); //刀具百分比
		break;
	case 1:
		cylinderMillingBuilder1->RadialStepover()->DistanceBuilder()->SetIntent(NXOpen::CAM::ParamValueIntentPartUnits);//mm
		break;
	}		
	cylinderMillingBuilder1->RadialStepover()->DistanceBuilder()->SetValue(HoleParameterSet.HMSlipMaxDistance);

	//非切削移动
	//进刀形式螺旋、线形、圆形，EngRetTypesCircular，EngRetTypesHelical
	switch (HoleParameterSet.HMNoCuttingType)
	{
	case 0:
		cylinderMillingBuilder1->NonCuttingBuilder()->Engage()->SetEngRetType(NXOpen::CAM::NcmHoleMachiningEngRet::EngRetTypesHelical);
		break;
	case 1:
		cylinderMillingBuilder1->NonCuttingBuilder()->Engage()->SetEngRetType(NXOpen::CAM::NcmHoleMachiningEngRet::EngRetTypesLinear);
		break;
	case 2:
		cylinderMillingBuilder1->NonCuttingBuilder()->Engage()->SetEngRetType(NXOpen::CAM::NcmHoleMachiningEngRet::EngRetTypesCircular);
		break;
	}
	cylinderMillingBuilder1->NonCuttingBuilder()->Engage()->MinClearance()->SetValue(HoleParameterSet.HMMinSafeDistance);//最小安全距离，HMMinSafeDistance

	//余量
	cylinderMillingBuilder1->CuttingParameters()->PartStock()->SetValue(HoleParameterSet.HMPartAllowance);

	//公差
	NXOpen::CAM::HoleMachiningCutParameters *holeMachiningCutParameters2;
	holeMachiningCutParameters2 = cylinderMillingBuilder1->CuttingParameters();
	NXOpen::CAM::CylinderMillingCutParameters *cylinderMillingCutParameters1(dynamic_cast<NXOpen::CAM::CylinderMillingCutParameters *>(holeMachiningCutParameters2));
	NXOpen::CAM::VerticalPosition *verticalPosition1;
	verticalPosition1 = cylinderMillingCutParameters1->BottomOffset();

	NXOpen::CAM::Inheritable2dLength *inheritable2dLength1;
	inheritable2dLength1 = cylinderMillingCutParameters1->Tolerances();
	inheritable2dLength1->SetValue0(HoleParameterSet.HMTolerance1);//内公差
	inheritable2dLength1->SetValue1(HoleParameterSet.HMTolerance2);//外公差
	delete inheritable2dLength1;

	//进退刀方式

	//主轴转速、进退刀速度Feeds and Speeds
	cylinderMillingBuilder1->FeedsBuilder()->SetSpindleRpmToggle(1);
	cylinderMillingBuilder1->FeedsBuilder()->SpindleRpmBuilder()->SetValue(HoleParameterSet.HMSpindleSpeed);
	cylinderMillingBuilder1->FeedsBuilder()->FeedCutBuilder()->SetValue(HoleParameterSet.HMMachining);
	cylinderMillingBuilder1->FeedsBuilder()->RecalculateData(NXOpen::CAM::FeedsBuilder::RecalcuateBasedOnSpindleSpeed);

	NXOpen::NXObject *nXObject1= cylinderMillingBuilder1->Commit();
	delete nXObject1;

	//生成刀轨
	std::vector<CAM::CAMObject *> objects1(1);
	objects1[0] = operation1;
	workPart->CAMSetup()->GenerateToolPath(objects1);
	UF_UI_ONT_refresh();

}

void SLHoleDrillingcmd::ThreadMillingDecisioncmd(char* mcsname, std::vector<Face*> InWallVectorList)
{		
	CAM::NCGroup *nCGroup1(dynamic_cast<CAM::NCGroup *>(workPart->CAMSetup()->CAMGroupCollection()->FindObject(HoleParameterSet.PROGRAM)));
	CAM::Method *method1(dynamic_cast<CAM::Method *>(workPart->CAMSetup()->CAMGroupCollection()->FindObject(HoleParameterSet.THMMETHOD)));
	CAM::OrientGeometry *orientGeometry1(dynamic_cast<CAM::OrientGeometry *>(workPart->CAMSetup()->CAMGroupCollection()->FindObject(mcsname)));//"MCS_G54",MCSName,Eachname,mcsName，凸台94806

	CAM::Tool *tool1(dynamic_cast<CAM::Tool *>(workPart->CAMSetup()->CAMGroupCollection()->FindObject(HoleParameterSet.THMToolMsg.ToolName)));

	CAM::Operation *operation1;

	tag_t HoleInWallFaceTag = InWallVectorList[0]->Tag();
	std::string tagStr = std::to_string(HoleInWallFaceTag);
	NXString nxString(tagStr.c_str());
	NXString OperationName = "ThreadMilling." + nxString;

	//创建操作;
	try
	{
		operation1 = workPart->CAMSetup()->CAMOperationCollection()->Create(nCGroup1, method1, tool1, orientGeometry1, "hole_making", "THREAD_MILLING", CAM::OperationCollection::UseDefaultNameFalse, OperationName);

	}
	catch (...)
	{
		operation1 = workPart->CAMSetup()->CAMOperationCollection()->Create(nCGroup1, method1, tool1, orientGeometry1, "hole_making", "THREAD_MILLING", CAM::OperationCollection::UseDefaultNameTrue, OperationName);
	}		


	NXOpen::CAM::ThreadMilling *threadMilling1(dynamic_cast<NXOpen::CAM::ThreadMilling *>(operation1));
	NXOpen::CAM::ThreadMillingBuilder *threadMillingBuilder1;
	threadMillingBuilder1 = workPart->CAMSetup()->CAMOperationCollection()->CreateThreadMillingBuilder(threadMilling1);

	NXOpen::CAM::HoleMachiningCutParameters *holeMachiningCutParameters1;
	holeMachiningCutParameters1 = threadMillingBuilder1->CuttingParameters();

	NXOpen::CAM::FBM::FeatureGeometry *featureGeometry1;
	featureGeometry1 = threadMillingBuilder1->GetFeatureGeometry();

	NXOpen::CAM::FBM::ThreadFeatureGeometry *threadFeatureGeometry1(dynamic_cast<NXOpen::CAM::FBM::ThreadFeatureGeometry *>(featureGeometry1));
	NXOpen::CAM::GeometrySetList *geometrySetList1;
	geometrySetList1 = threadFeatureGeometry1->GeometryList();
	NXOpen::CAM::CAMFeature *nullNXOpen_CAM_CAMFeature(NULL);

	for (std::vector<Face*>::size_type k = 0; k != InWallVectorList.size(); k++)
	{
		NXOpen::CAM::FBM::FeatureSet *featureSet1;
		featureSet1 = threadFeatureGeometry1->AddFeatureSet(nullNXOpen_CAM_CAMFeature, "NXHOLETHREAD");//钻操作换成"NXHOLE"

		geometrySetList1->Append(featureSet1);

		featureSet1->SetAngleToleranceEdges(0.0);
		featureSet1->SetIntol(0.0);
		featureSet1->SetOuttol(0.0);

		NXOpen::PartLoadStatus *partLoadStatus1;
		partLoadStatus1 = workPart->LoadThisPartFully();
		delete partLoadStatus1;

		//赋值孔内壁面
		std::vector<NXOpen::NXObject *> entities1(1);
		entities1[0] = InWallVectorList[k];
		NXOpen::CAM::FBM::Feature *feature1;
		feature1 = featureSet1->CreateFeature(entities1);
	}
	
	//轴向步距，刀具百分比
	threadMillingBuilder1->AxialStepover()->SetStepoverType(NXOpen::CAM::StepoverBuilder::StepoverTypesPercentToolFlat);//刀具百分比
	threadMillingBuilder1->AxialStepover()->PercentToolFlatBuilder()->SetValue(HoleParameterSet.THMAxisStepDistance);

	threadMillingBuilder1->SetSpringPasses(HoleParameterSet.THMNumOfCuttingStep);//螺旋刀路数

	//径向最大距离，恒定
	threadMillingBuilder1->RadialStepover()->SetStepoverType(NXOpen::CAM::StepoverBuilder::StepoverTypesConstant);//StepoverTypesNumber,StepoverTypesMultiple，StepoverTypesConstant	
	threadMillingBuilder1->RadialStepover()->InheritableDistance()->SetValue(HoleParameterSet.THMSlipMaxDistance);	


	//公差
	NXOpen::CAM::HoleMachiningCutParameters *holeMachiningCutParameters2;
	holeMachiningCutParameters2 = threadMillingBuilder1->CuttingParameters();
	NXOpen::CAM::ThreadMillingCutParameters *threadMillingCutParameters1(dynamic_cast<NXOpen::CAM::ThreadMillingCutParameters *>(holeMachiningCutParameters2));
	threadMillingCutParameters1->SetCuttingDirection(NXOpen::CAM::FeatureMillingCutParameters::CuttingDirectionTypesConventional);

	NXOpen::CAM::Inheritable2dLength *inheritable2dLength1;
	inheritable2dLength1 = threadMillingCutParameters1->Tolerances();

	inheritable2dLength1->SetValue0(HoleParameterSet.THMTolerance1);//内公差
	inheritable2dLength1->SetValue1(HoleParameterSet.THMTolerance2);//外公差

	//余量
	threadMillingBuilder1->CuttingParameters()->PartStock()->SetValue(HoleParameterSet.THMPartAllowance);

	//非切削移动
	//进刀形式螺旋、线形EngRetTypesHelical,EngRetTypesLinear
	switch (HoleParameterSet.THMNoCuttingType)
	{
	case 0:
		threadMillingBuilder1->NonCuttingBuilder()->Engage()->SetEngRetType(NXOpen::CAM::NcmHoleMachiningEngRet::EngRetTypesHelical);
		break;
	case 1:
		threadMillingBuilder1->NonCuttingBuilder()->Engage()->SetEngRetType(NXOpen::CAM::NcmHoleMachiningEngRet::EngRetTypesLinear);
		break;
	}
	threadMillingBuilder1->NonCuttingBuilder()->Engage()->MinClearance()->SetValue(HoleParameterSet.THMMinSafeDistance);//最小安全距离，THMMinSafeDistance


	//主轴转速、进退刀速度Feeds and Speeds
	threadMillingBuilder1->FeedsBuilder()->SetSpindleRpmToggle(1);
	threadMillingBuilder1->FeedsBuilder()->SpindleRpmBuilder()->SetValue(HoleParameterSet.THMSpindleSpeed);
	threadMillingBuilder1->FeedsBuilder()->FeedCutBuilder()->SetValue(HoleParameterSet.THMMachining);
	threadMillingBuilder1->FeedsBuilder()->RecalculateData(NXOpen::CAM::FeedsBuilder::RecalcuateBasedOnSpindleSpeed);
		

	NXOpen::NXObject *nXObject1;
	nXObject1 = threadMillingBuilder1->Commit();
	delete nXObject1;

	//生成刀轨
	std::vector<CAM::CAMObject *> objects1(1);
	objects1[0] = operation1;
	workPart->CAMSetup()->GenerateToolPath(objects1);
	UF_UI_ONT_refresh();
}

void SLHoleDrillingcmd::OperatorDecision(std::vector<DrGeo> axisDrGeo,std::vector<DrGeo> bossDrGeo,std::vector<DrGeo> wallRibDrGeo,int OperatorType)
{

	//获取坐标系原点
	tag_t wcsMatrixTag = NULL_TAG;
	double wcsMatrixValues[9] = { 0.0 };
	tag_t wcsTag = NULL_TAG;
	UF_CSYS_ask_wcs(&wcsTag); //拿到WCS的Tag
	UF_CSYS_ask_matrix_of_object(wcsTag, &wcsMatrixTag);
	UF_CSYS_ask_matrix_values(wcsMatrixTag, wcsMatrixValues);
	double wcsX[3] = { wcsMatrixValues[0], wcsMatrixValues[1], wcsMatrixValues[2] };
	double wcsY[3] = { wcsMatrixValues[3], wcsMatrixValues[4], wcsMatrixValues[5] };
	double wcsZ[3] = { wcsMatrixValues[6], wcsMatrixValues[7], wcsMatrixValues[8] };
	double wcs_origin[3];//拿到WCS的Tagorigin
	UF_CSYS_ask_csys_info(wcsTag, &wcsMatrixTag, wcs_origin);

	if (axisDrGeo.size() != NULL)//端面孔
	{
		//所有端面孔划分为n个操作
		//获取n个操作中心点坐标
		std::vector <DrGeo>::iterator first = AxisDrGeo.begin();
		NXOpen::Face* HoleTopFace0 = first->selectedHoleTop;
		NXOpen::Face* HoleInWallFace0 = first->selectedHoleInWall;

		Point3d centeripoint2 = GetCenterPoint(HoleInWallFace0);
		double originwall[3] = { centeripoint2.X, centeripoint2.Y, centeripoint2.Z };//壁面中心点的绝对坐标
		Point3d R = Get_opposite_coordinate(centeripoint2, wcs_origin, wcsMatrixValues);
		double wcs_originwall[3] = { R.X, R.Y, R.Z };//获取壁面中心点在wcs的相对坐标		

		int n ;
		n = HoleParameterSet.NumofDMHoleOperation;
		double MaxAngle = 360 / n;
		double FaceRadius;                  //坐标系所在面的中心半径
		FaceRadius = sqrt(wcs_originwall[0] * wcs_originwall[0] + wcs_originwall[1] * wcs_originwall[1]);

		Point3d centeripoint = GetCenterPoint(HoleTopFace0);
		Point3d R1 = Get_opposite_coordinate(centeripoint, wcs_origin, wcsMatrixValues);
		double wcs_origincenter[3] = { R1.X, R1.Y, R1.Z };//获取面中心点在wcs的相对坐标
	
		std::vector<Point3d> mcs_centerlist; //n个操作中心点坐标
		mcs_centerlist.clear();
		//寻找n个操作中心
		for (int j = 0; j < n; j++)
		{
			//n个操作中心点绝对坐标
			//mcs坐标原点的相对坐标;
			double xw = FaceRadius*cos(MaxAngle*j * 2 * PI / 360);
			double yw = FaceRadius*sin(MaxAngle*j * 2 * PI / 360);
			double zw = wcs_origincenter[2];
			double opposite_point[3] = { xw, yw, zw };
			Point3d R2 = Get_absolute_coordinate(opposite_point, wcs_origin, wcsMatrixValues);
			mcs_centerlist.push_back(R2);
		}
		std::vector<Point3d> mcs_centerlist2 = mcs_centerlist; //n个操作中心点坐标(副本)

		//对于n个中心的孔列表，编写n个操作
		for (int j = 0; j < mcs_centerlist.size(); j++)
		{
			//uc1601("编写n个操作", 1);
			Point3d MCS = mcs_centerlist[j];
			double MCS_point[3] = { MCS.X, MCS.Y, MCS.Z };

			std::vector<Face*> TopListOfOneCenter;//端面孔的顶面
			std::vector<Face*> InWallListOfOneCenter;//端面孔的侧面
			for (int i = 0; i < AxisDrGeo.size(); i++)
			{
				//uc1601("AxisDrGeo!", 1);
				NXOpen::Face* AxisTopFace = AxisDrGeo[i].selectedHoleTop;//取出每一个孔顶面;	
				NXOpen::Face* AxisInWallFace = AxisDrGeo[i].selectedHoleInWall;//取出每一个孔壁面;

				Point3d centeripoint = GetCenterPoint(AxisInWallFace);
				double centerofhole[3] = { centeripoint.X, centeripoint.Y, centeripoint.Z };//壁面中心点的绝对坐标
				double PointDistance;//与当前操作中心的距离;
				PointDistance = sqrt((centerofhole[0] - MCS_point[0])*(centerofhole[0] - MCS_point[0]) + (centerofhole[1] - MCS_point[1])*(centerofhole[1] - MCS_point[1]) + (centerofhole[2] - MCS_point[2])*(centerofhole[2] - MCS_point[2]));
				int M = 0;
				//若该孔不存在其它操作中心更近,则该孔追加到当前操作中心的孔列表;
				for (int k = 0; k < mcs_centerlist2.size(); k++)
				{
					Point3d MCS_k = mcs_centerlist2[k];
					double MCS_k_point[3] = { MCS_k.X, MCS_k.Y, MCS_k.Z };
					double KDistance;
					KDistance = sqrt((centerofhole[0] - MCS_k_point[0])*(centerofhole[0] - MCS_k_point[0]) + (centerofhole[1] - MCS_k_point[1])*(centerofhole[1] - MCS_k_point[1]) + (centerofhole[2] - MCS_k_point[2])*(centerofhole[2] - MCS_k_point[2]));
					if (KDistance < PointDistance)
					{
						M = M + 1;
					}
					else{};

				}
				if (M == 0)
				{
					TopListOfOneCenter.push_back(AxisTopFace);
					InWallListOfOneCenter.push_back(AxisInWallFace);
				}
				else{};
			}
			//对每个操作中心的孔列表编程，一个操作
			//坐标系创建**
			//uc1601("坐标系创建", 1);
			if (InWallListOfOneCenter.size() != 0)
			{
				char* mcsname = new char[20];//预留两位数作为坐标id
				if (HoleParameterSet.IfLCsys)
				{
					//uc1601("HoleParameterSet.IfLCsys", 1);
					NXOpen::Face* HoleTopFace = TopListOfOneCenter[0];
					tag_t TopFaceTag = HoleTopFace->Tag();
					////获得面的法矢量
					Point3d centernormal = get_Centre_fr_Face(HoleTopFace);
					double Facenormal[3];
					Facenormal[0] = centernormal.X;
					Facenormal[1] = centernormal.Y;
					Facenormal[2] = centernormal.Z;

					double origin[3]; //操作中心点坐标，即局部坐标系原点
					origin[0] = MCS_point[0];
					origin[1] = MCS_point[1];
					origin[2] = MCS_point[2];

					//操作名称 MCS_face + TopFaceTag + 第j个操作中心;
					const char* prefix = "MCS_face";//前缀
					int lenPrefix = strlen(prefix);
					std::string tagStr1 = std::to_string(TopFaceTag);
					const char* tagcStr1 = tagStr1.c_str();
					std::string Indexstr = std::to_string(j);
					const char* Indexcstr = Indexstr.c_str();
					std::strcpy(mcsname, prefix);//加前缀	
					mcsname = strcat(mcsname, tagcStr1);//加tag值
					mcsname = strcat(mcsname, Indexcstr);//加id						

					CreateMCS(TopFaceTag, Facenormal, mcsname, wcsZ, origin);
				}
				else
				{
					//uc1601("WORKPIECE", 1);
					mcsname = "MCS_MILL";
				}
				//加工操作生成**
				if (OperatorType == 0)//中心钻;
				{
					SpotDrillingDecisioncmd(mcsname, InWallListOfOneCenter);
				}
				else{};
				if (OperatorType == 1)//钻、锪、铰、镗;
				{
					DrillingDecisioncmd(mcsname, InWallListOfOneCenter);
				}
				else{};
				if (OperatorType == 2)
				{
					SpotFacingDecisioncmd(mcsname, InWallListOfOneCenter);
				}
				if (OperatorType == 3)
				{
					BoringDecisioncmd(mcsname, InWallListOfOneCenter);
				}
				else{};
				if (OperatorType == 4)
				{
					ReamingDecisioncmd(mcsname, InWallListOfOneCenter);
				}
				else{};
				if (OperatorType == 5)//孔铣;
				{
					HoleMillingDecisioncmd(mcsname, InWallListOfOneCenter);
				}
				else{};
				if (OperatorType == 6)//螺纹铣;
				{
					ThreadMillingDecisioncmd(mcsname, InWallListOfOneCenter);
				}
				else{};
				
			}
			else{};
		}

	}
	else{};
	if (bossDrGeo.size() != NULL)//凸台孔
	{
		//uc1601("凸台孔",1);
		//筛选同一凸台上的孔
		//char msg1[256];
		//double Sizeofboss = bossDrGeo.size();
		//UF_UI_open_listing_window();
		//sprintf(msg1, "凸台孔个数：\n%f\n", Sizeofboss);
		//UF_UI_write_listing_window(msg1);
		
		while (bossDrGeo.size() != NULL)
		{
			//uc1601("bossDrGeo", 1);
			std::vector<Face*> TopListOfOneCenter;//端面孔的顶面
			std::vector<Face*> InWallListOfOneCenter;//端面孔的侧面
			TopListOfOneCenter.clear();
			InWallListOfOneCenter.clear();

			std::vector <DrGeo>::iterator first;
			first = bossDrGeo.begin();
			NXOpen::Face* BossHoleTopFace = first->selectedHoleTop;
			NXOpen::Face* BossHoleInWallFace = first->selectedHoleInWall;
			//获取面Tag的字符串
			tag_t FirstTopFaceTag = BossHoleTopFace->Tag();
			std::string tagStr1;
			tagStr1 = std::to_string(FirstTopFaceTag);
			const char* FirstTopTagchar = tagStr1.c_str();
			//获取面半径
			int _type1;
			double _point1[3];
			double _dir1[3];
			double _box1[6];
			double _radius1;
			double _rad_data1;
			int _norm_dir1;
			UF_MODL_ask_face_data(BossHoleInWallFace->Tag(), &_type1, _point1, _dir1,
				_box1, &_radius1, &_rad_data1, &_norm_dir1);				

			TopListOfOneCenter.push_back(BossHoleTopFace);
			InWallListOfOneCenter.push_back(BossHoleInWallFace);
			bossDrGeo.erase(first);



			std::vector <DrGeo>::iterator  it;
			for (it = bossDrGeo.begin(); it != bossDrGeo.end(); )
			{
				//uc1601("bossDrGeo1", 1);
				NXOpen::Face* bossHoleTopFace = it->selectedHoleTop;
				NXOpen::Face* bossHoleInWallFace = it->selectedHoleInWall;
				//获取面Tag的字符串
				tag_t ItTopFaceTag = bossHoleTopFace->Tag();
				std::string tagStr2;
				tagStr2 = std::to_string(ItTopFaceTag);
				const char* ItTopTagchar = tagStr2.c_str();
				//获取面半径
				int _type2;
				double _point2[3];
				double _dir2[3];
				double _box2[6];
				double _radius2;
				double _rad_data2;
				int _norm_dir2;
				UF_MODL_ask_face_data(bossHoleInWallFace->Tag(), &_type2, _point2, _dir2,
					_box2, &_radius2, &_rad_data2, &_norm_dir2);

				//char msg[256];
				//UF_UI_open_listing_window();
				//sprintf(msg, "孔1和2的半径分别为：\n%f, %f,\n\n",_radius1,_radius2);
				//UF_UI_write_listing_window(msg);

				if ((strcmp(FirstTopTagchar, ItTopTagchar) == 0) && abs(_radius1 - _radius2)<0.00001)
				{
					//uc1601("bossDrGeo2", 1);
					TopListOfOneCenter.push_back(bossHoleTopFace);
					InWallListOfOneCenter.push_back(bossHoleInWallFace);
					bossDrGeo.erase(it);
				}
				else
				{
					++it;
				}
			}

			//工艺编程
			//坐标系创建**	
			//uc1601("坐标系创建", 1);
			char* mcsname = new char[20];//预留两位数作为坐标id
			if (HoleParameterSet.IfLCsys)
			{
				//uc1601("HoleParameterSet.IfLCsys", 1);
				NXOpen::Face* HoleTopFace = TopListOfOneCenter[0];
				tag_t TopFaceTag = HoleTopFace->Tag();
				////获得面的法矢量
				Point3d centernormal = get_Centre_fr_Face(HoleTopFace);
				double Facenormal[3];
				Facenormal[0] = centernormal.X;
				Facenormal[1] = centernormal.Y;
				Facenormal[2] = centernormal.Z;

				double origin[3];
				Point3d centeripoint0 = GetCenterPoint(HoleTopFace);
				origin[0] = centeripoint0.X;
				origin[1] = centeripoint0.Y;
				origin[2] = centeripoint0.Z;

				//操作名称 MCS_face + TopFaceTag + 第j个操作中心;
				const char* prefix = "MCS_face";//前缀
				int lenPrefix = strlen(prefix);
				std::string tagStr1 = std::to_string(TopFaceTag);
				const char* tagcStr1 = tagStr1.c_str();
				std::strcpy(mcsname, prefix);//加前缀	
				mcsname = strcat(mcsname, tagcStr1);//加tag值				

				CreateMCS(TopFaceTag, Facenormal, mcsname, wcsZ, origin);
			}
			else
			{
				mcsname = "MCS_MILL";
			}
			//加工操作生成**
			if (OperatorType == 0)//中心钻;
			{
				SpotDrillingDecisioncmd(mcsname, InWallListOfOneCenter);
			}
			else{};
			if (OperatorType == 1 )//钻、锪、铰、镗;
			{
				//uc1601("钻创建", 1);
				DrillingDecisioncmd(mcsname, InWallListOfOneCenter);
			}
			else{};
			if (OperatorType == 2)
			{
				SpotFacingDecisioncmd (mcsname, InWallListOfOneCenter);
			}
			else{};
			if (OperatorType == 3)
			{
				BoringDecisioncmd (mcsname, InWallListOfOneCenter);
			}
			else{};
			if (OperatorType == 4)
			{
				ReamingDecisioncmd(mcsname, InWallListOfOneCenter);
			}
			else{};

			if (OperatorType == 5)//孔铣;
			{
				//uc1601("孔铣创建", 1);
				HoleMillingDecisioncmd(mcsname, InWallListOfOneCenter);
			}
			else{};
			if (OperatorType == 6)//螺纹铣;
			{
				//uc1601("螺纹铣创建", 1);
				ThreadMillingDecisioncmd(mcsname, InWallListOfOneCenter);
				//uc1601("螺纹铣创建成功", 1);
			}
			else{};

		}
		//uc1601("凸台孔加工完毕",1);

	}
	else{};
	if (wallRibDrGeo.size() != NULL)
	{
		//uc1601("筋面和壁面孔", 1);	
		char *Eachname;
		char *RibHole = "筋面";
		char *WallHole = "壁面";

		while (wallRibDrGeo.size() != NULL)
		{
			std::vector<Face*> TopListOfOneCenter;//端面孔的顶面
			std::vector<Face*> InWallListOfOneCenter;//端面孔的侧面
			std::vector<Face*> OtherListOfOneCenter;//端面孔的其它面
			TopListOfOneCenter.clear();
			InWallListOfOneCenter.clear();
			OtherListOfOneCenter.clear();

			std::vector <DrGeo>::iterator first;
			first = wallRibDrGeo.begin();
			NXOpen::Face* WallRibHoleTopFace = first->selectedHoleTop;
			NXOpen::Face* WallRibHoleInWallFace = first->selectedHoleInWall;
			NXOpen::Face* WallRibHoleOtherFace = first->selectedHoleOther;
			Eachname = first->selectedHoleName;

			TopListOfOneCenter.push_back(WallRibHoleTopFace);
			InWallListOfOneCenter.push_back(WallRibHoleInWallFace);
			OtherListOfOneCenter.push_back(WallRibHoleOtherFace);
			wallRibDrGeo.erase(first);

			//工艺编程
			//坐标系创建**	
			//uc1601("坐标系创建", 1);
			char* mcsname = new char[20];//预留两位数作为坐标id
			if (HoleParameterSet.IfLCsys)
			{
				if (strstr(Eachname, WallHole) != NULL)//壁面孔
				{
					NXOpen::Face* HoleTopFace = TopListOfOneCenter[0];
					NXOpen::Face* HoleInWallFace = InWallListOfOneCenter[0];
					NXOpen::Face* HoleOtheFace = OtherListOfOneCenter[0];
					tag_t TopFaceTag = HoleTopFace->Tag();
					tag_t HoleInWallFaceTag = HoleInWallFace->Tag();
					//孔边缘
					std::vector<NXOpen::Edge*> pConEdges;
					bool ConEdgeFlag = get_pEdge_fr_fafa(HoleOtheFace, HoleInWallFace, pConEdges);
					tag_t iEdge_tag = pConEdges[0]->Tag();

					double Edge_center[3];  //曲线中心点(质心)
					UF_CURVE_ask_centroid(iEdge_tag, Edge_center);

					//获取孔轴向
					int type = 0;       //判断面类型
					int norm_dir = 0;   //1=外R，-1=内R
					double point[3];  //轴中心 点
					double dir[3];    //轴方向
					double box[6];    //极点坐标
					double radius;    //半径 圆锥=最小半径 + 最大半径 /2
					double rad_data;
					UF_MODL_ask_face_data(HoleInWallFaceTag, &type, point, dir, box, &radius, &rad_data, &norm_dir);

					//过质心垂直轴向创建一个平面
					tag_t PlaneTag = NULL_TAG;
					UF_MODL_create_plane(Edge_center, dir, &PlaneTag);

					NXOpen::TaggedObject*selectobject((NXOpen::TaggedObject*)NXObjectManager::Get(PlaneTag));
					NXOpen::NXObject * NXobjectFace = (NXOpen::NXObject*) selectobject;
					UF_OBJ_set_blank_status(NXobjectFace->Tag(), UF_OBJ_BLANKED);          //隐藏

					//坐标系名称设置为平面tag值;
					const char* prefix = "MCS_face";//前缀
					int lenPrefix = strlen(prefix);
					std::string tagStr1 = std::to_string(HoleInWallFaceTag);
					const char* tagcStr1 = tagStr1.c_str();
					std::strcpy(mcsname, prefix);//加前缀	
					mcsname = strcat(mcsname, tagcStr1);//加tag值	

					double origin[3];
					Point3d centeripoint0 = GetCenterPoint(HoleTopFace);
					origin[0] = Edge_center[0];
					origin[1] = Edge_center[1];
					origin[2] = Edge_center[2];

					TopFaceTag = PlaneTag;
					////获得面的法矢量
					double Facenormal[3];
					Facenormal[0] = dir[0];
					Facenormal[1] = dir[1];
					Facenormal[2] = dir[2];

					CreateMCS(TopFaceTag, Facenormal, mcsname, wcsZ, origin);
				}
				else if (strstr(Eachname, RibHole) != NULL)//筋面孔
				{
					//uc1601("HoleParameterSet.IfLCsys", 1);
					NXOpen::Face* HoleTopFace = TopListOfOneCenter[0];
					tag_t TopFaceTag = HoleTopFace->Tag();
					////获得面的法矢量
					Point3d centernormal = get_Centre_fr_Face(HoleTopFace);
					double Facenormal[3];
					Facenormal[0] = centernormal.X;
					Facenormal[1] = centernormal.Y;
					Facenormal[2] = centernormal.Z;

					double origin[3];
					Point3d centeripoint0 = GetCenterPoint(HoleTopFace);
					origin[0] = centeripoint0.X;
					origin[1] = centeripoint0.Y;
					origin[2] = centeripoint0.Z;

					//操作名称 MCS_face + TopFaceTag + 第j个操作中心;
					const char* prefix = "MCS_face";//前缀
					int lenPrefix = strlen(prefix);
					std::string tagStr1 = std::to_string(TopFaceTag);
					const char* tagcStr1 = tagStr1.c_str();
					std::strcpy(mcsname, prefix);//加前缀	
					mcsname = strcat(mcsname, tagcStr1);//加tag值				

					CreateMCS(TopFaceTag, Facenormal, mcsname, wcsZ, origin);
				}
				else{};
			}
			else
			{
				mcsname = "MCS_MILL";
			}
			//加工操作生成**
			if (OperatorType == 0)//中心钻;
			{
				SpotDrillingDecisioncmd(mcsname, InWallListOfOneCenter);
			}
			else{};
			if (OperatorType == 1)//钻、锪、铰、镗;
			{
				//uc1601("钻创建", 1);
				DrillingDecisioncmd(mcsname, InWallListOfOneCenter);
			}
			if (OperatorType == 2)
			{
				SpotFacingDecisioncmd(mcsname, InWallListOfOneCenter);
			}
			else{};
			if (OperatorType == 3)
			{
				BoringDecisioncmd(mcsname, InWallListOfOneCenter);
			}
			else{};
			if (OperatorType == 4)
			{
				ReamingDecisioncmd(mcsname, InWallListOfOneCenter);
			}
			else{};

			if (OperatorType == 5)//孔铣;
			{
				//uc1601("孔铣创建", 1);
				HoleMillingDecisioncmd(mcsname, InWallListOfOneCenter);
			}
			else{};
			if (OperatorType == 6)//螺纹铣;
			{
				//uc1601("螺纹铣创建", 1);
				ThreadMillingDecisioncmd(mcsname, InWallListOfOneCenter);
				//uc1601("螺纹铣创建成功", 1);
			}
			else{};
		}
	}
	else{};
	//uc1601("加工完成", 1);

}
void SLHoleDrillingcmd::SpotDrillingDecisioncmd(char* mcsname, std::vector<Face*> InWallVectorList)
{
	//uc1601("SpotDrillingDecisioncmd == 0", 1);
	CAM::NCGroup *nCGroup1(dynamic_cast<CAM::NCGroup *>(workPart->CAMSetup()->CAMGroupCollection()->FindObject(HoleParameterSet.PROGRAM)));
	CAM::Method *method1(dynamic_cast<CAM::Method *>(workPart->CAMSetup()->CAMGroupCollection()->FindObject(HoleParameterSet.METHOD)));
	CAM::Tool *tool1(dynamic_cast<CAM::Tool *>(workPart->CAMSetup()->CAMGroupCollection()->FindObject(HoleParameterSet.ToolMsg.ToolName)));
	CAM::Operation *operation1;
	CAM::OrientGeometry *orientGeometry33(dynamic_cast<CAM::OrientGeometry *>(workPart->CAMSetup()->CAMGroupCollection()->FindObject(mcsname)));//"MCS_G54",MCSName,Eachname,mcsName，凸台94806

	tag_t HoleInWallFaceTag = InWallVectorList[0]->Tag();
	std::string tagStr = std::to_string(HoleInWallFaceTag);
	NXString nxString(tagStr.c_str());
	NXString OperationName = "HoleSpotDrilling." + nxString;

	//创建操作;
	try
	{
		operation1 = workPart->CAMSetup()->CAMOperationCollection()->Create(nCGroup1, method1, tool1, orientGeometry33, "hole_making", "SPOT_DRILLING", NXOpen::CAM::OperationCollection::UseDefaultNameFalse, OperationName); // "DRILLING"
	}
	catch (...)
	{
		operation1 = workPart->CAMSetup()->CAMOperationCollection()->Create(nCGroup1, method1, tool1, orientGeometry33, "hole_making", "SPOT_DRILLING", NXOpen::CAM::OperationCollection::UseDefaultNameTrue, OperationName);
	}
	NXOpen::CAM::HoleDrilling *holeDrilling1(dynamic_cast<NXOpen::CAM::HoleDrilling *>(operation1));
	NXOpen::CAM::HoleDrillingBuilder *holeDrillingBuilder1;
	holeDrillingBuilder1 = workPart->CAMSetup()->CAMOperationCollection()->CreateHoleDrillingBuilder(holeDrilling1);
	NXOpen::CAM::HoleMachiningCutParameters *holeMachiningCutParameters1;
	holeMachiningCutParameters1 = holeDrillingBuilder1->CuttingParameters();
	NXOpen::CAM::FBM::FeatureGeometry *featureGeometry1;
	featureGeometry1 = holeDrillingBuilder1->GetFeatureGeometry();

	NXOpen::CAM::FBM::MachiningFeatureGeometry *machiningFeatureGeometry1(dynamic_cast<NXOpen::CAM::FBM::MachiningFeatureGeometry *>(featureGeometry1));


	//创建孔几何
	NXOpen::CAM::GeometrySetList *geometrySetList1;
	geometrySetList1 = machiningFeatureGeometry1->GeometryList();
	NXOpen::CAM::CAMFeature *nullNXOpen_CAM_CAMFeature(NULL);

	for (std::vector<Face*>::size_type k = 0; k != InWallVectorList.size(); k++)
	{
		NXOpen::CAM::FBM::FeatureSet *featureSet1;
		featureSet1 = machiningFeatureGeometry1->AddFeatureSet(nullNXOpen_CAM_CAMFeature, "NXHOLECENTER");//钻操作换成"NXHOLE"

		geometrySetList1->Append(featureSet1);

		featureSet1->SetAngleToleranceEdges(0.0);
		featureSet1->SetIntol(0.0);
		featureSet1->SetOuttol(0.0);

		//赋值孔内壁面
		std::vector<NXOpen::NXObject *> entities1(1);
		entities1[0] = InWallVectorList[k];
		NXOpen::CAM::FBM::Feature *feature1;
		feature1 = featureSet1->CreateFeature(entities1);
		//uc1601("孔几何+1!", 1);
	}

	//循环类型
	switch (HoleParameterSet.Cycling)
	{
	case 0:
		holeDrillingBuilder1->CycleTable()->SetCycleType("Drill");
		break;
	case 1:
		holeDrillingBuilder1->CycleTable()->SetCycleType("Drill,Text");
		break;
	case 2:
		holeDrillingBuilder1->CycleTable()->SetCycleType("Drill,Csink");
		break;
	case 3:
		holeDrillingBuilder1->CycleTable()->SetCycleType("Drill,Deep");
		break;
	case 4:
		holeDrillingBuilder1->CycleTable()->SetCycleType("Drill,Deep,BreakChip");
		break;
	case 5:
		holeDrillingBuilder1->CycleTable()->SetCycleType("Drill,Tap");
		break;
	case 6:
		holeDrillingBuilder1->CycleTable()->SetCycleType("Drill,Tap,Float");
		break;
	case 7:
		holeDrillingBuilder1->CycleTable()->SetCycleType("Drill,Tap,Deep");
		break;
	case 8:
		holeDrillingBuilder1->CycleTable()->SetCycleType("Drill,Tap,BreakChip");
		break;
	case 9:
		holeDrillingBuilder1->CycleTable()->SetCycleType("Drill,Bore");
		break;
	case 10:
		holeDrillingBuilder1->CycleTable()->SetCycleType("Drill,Bore,Drag");
		break;
	case 11:
		holeDrillingBuilder1->CycleTable()->SetCycleType("Drill,Bore,Nodrag");
		break;
	case 12:
		holeDrillingBuilder1->CycleTable()->SetCycleType("Drill,Bore,Back");
		break;
	case 13:
		holeDrillingBuilder1->CycleTable()->SetCycleType("Drill,Bore,Manual");
		break;
	default:
		holeDrillingBuilder1->CycleTable()->SetCycleType("Drill");
		break;
	}


	//偏置
	NXOpen::CAM::HoleDrillingCutParameters *holeDrillingCutParameters1(dynamic_cast<NXOpen::CAM::HoleDrillingCutParameters *>(holeMachiningCutParameters1));
	NXOpen::CAM::VerticalPosition *verticalPosition1;
	holeDrillingCutParameters1->BottomOffset()->SetDistance(HoleParameterSet.MBottomOffset);//底部
	NXOpen::CAM::VerticalPosition *verticalPosition2;
	holeDrillingCutParameters1->RaptoOffset()->SetDistance(HoleParameterSet.MRaptoOffset);//平行
	//顶部
	holeDrillingBuilder1->CuttingParameters()->TopOffset()->SetDistance(HoleParameterSet.MTopOffset);//

	NXObject *nXObject2 = holeDrillingBuilder1->Commit();

	//切削参数
	NXOpen::CAM::HoleDrillingBuilder *holeDrillingBuilder2;
	holeDrillingBuilder2 = workPart->CAMSetup()->CAMOperationCollection()->CreateHoleDrillingBuilder(holeDrilling1);

	holeDrillingBuilder2->FeedsBuilder()->SpindleRpmBuilder()->SetValue(HoleParameterSet.SpindleSpeed);
	holeDrillingBuilder2->FeedsBuilder()->FeedCutBuilder()->SetValue(HoleParameterSet.Machining);
	NXObject *nXObject1 = holeDrillingBuilder2->Commit();

	//生成刀轨
	std::vector<CAM::CAMObject *> objects1(1);
	objects1[0] = operation1;
	workPart->CAMSetup()->GenerateToolPath(objects1);
	UF_UI_ONT_refresh();
}
void SLHoleDrillingcmd::DrillingDecisioncmd(char* mcsname, std::vector<Face*> InWallVectorList)
{
	//uc1601("啊咧咧！", 1);
	CAM::NCGroup *nCGroup1(dynamic_cast<CAM::NCGroup *>(workPart->CAMSetup()->CAMGroupCollection()->FindObject(HoleParameterSet.PROGRAM)));
	CAM::Method *method1(dynamic_cast<CAM::Method *>(workPart->CAMSetup()->CAMGroupCollection()->FindObject(HoleParameterSet.METHOD)));
	CAM::Tool *tool1(dynamic_cast<CAM::Tool *>(workPart->CAMSetup()->CAMGroupCollection()->FindObject(HoleParameterSet.DToolMsg.ToolName)));
	CAM::Operation *operation1;
	//uc1601("ERROR4", 1);
	CAM::OrientGeometry *orientGeometry33(dynamic_cast<CAM::OrientGeometry *>(workPart->CAMSetup()->CAMGroupCollection()->FindObject(mcsname)));//"MCS_G54",MCSName,Eachname,mcsName，凸台94806


	//char* StringIndex = new char;
	//sprintf(StringIndex, "%d", EachIndex);
	//NXString NXStringIndex = StringIndex;
	//delete StringIndex;
	//uc1601("ERROR5", 1);
	tag_t HoleInWallFaceTag = InWallVectorList[0]->Tag();
	std::string tagStr = std::to_string(HoleInWallFaceTag);
	NXString nxString(tagStr.c_str());
	NXString OperationName = "Drilling." + nxString;
	//uc1601("ERROR6", 1);
	//创建操作;
	try
	{
		operation1 = workPart->CAMSetup()->CAMOperationCollection()->Create(nCGroup1, method1, tool1, orientGeometry33, "hole_making", "DRILLING", NXOpen::CAM::OperationCollection::UseDefaultNameFalse, OperationName); // "DRILLING"
	}
	catch (...)
	{
		operation1 = workPart->CAMSetup()->CAMOperationCollection()->Create(nCGroup1, method1, tool1, orientGeometry33, "hole_making", "DRILLING", NXOpen::CAM::OperationCollection::UseDefaultNameTrue, OperationName);
		//uc1601("catch exception!", 1);
	}
	//uc1601("ERROR7", 1);
	NXOpen::CAM::HoleDrilling *holeDrilling1(dynamic_cast<NXOpen::CAM::HoleDrilling *>(operation1));
	NXOpen::CAM::HoleDrillingBuilder *holeDrillingBuilder1;
	holeDrillingBuilder1 = workPart->CAMSetup()->CAMOperationCollection()->CreateHoleDrillingBuilder(holeDrilling1);
	NXOpen::CAM::HoleMachiningCutParameters *holeMachiningCutParameters1;
	holeMachiningCutParameters1 = holeDrillingBuilder1->CuttingParameters();
	NXOpen::CAM::FBM::FeatureGeometry *featureGeometry1;
	featureGeometry1 = holeDrillingBuilder1->GetFeatureGeometry();

	//uc1601("ERROR8", 1);
	NXOpen::CAM::FBM::MachiningFeatureGeometry *machiningFeatureGeometry1(dynamic_cast<NXOpen::CAM::FBM::MachiningFeatureGeometry *>(featureGeometry1));

	//创建孔几何
	NXOpen::CAM::GeometrySetList *geometrySetList1;
	geometrySetList1 = machiningFeatureGeometry1->GeometryList();
	NXOpen::CAM::CAMFeature *nullNXOpen_CAM_CAMFeature(NULL);

	//uc1601("创建孔几何!", 1);
	for (std::vector<Face*>::size_type k = 0; k != InWallVectorList.size(); k++)
	{
		NXOpen::CAM::FBM::FeatureSet *featureSet1;
		featureSet1 = machiningFeatureGeometry1->AddFeatureSet(nullNXOpen_CAM_CAMFeature, "NXHOLE");//钻操作换成"NXHOLE"

		geometrySetList1->Append(featureSet1);

		featureSet1->SetAngleToleranceEdges(0.0);
		featureSet1->SetIntol(0.0);
		featureSet1->SetOuttol(0.0);

		//赋值孔内壁面
		std::vector<NXOpen::NXObject *> entities1(1);
		entities1[0] = InWallVectorList[k];
		NXOpen::CAM::FBM::Feature *feature1;
		feature1 = featureSet1->CreateFeature(entities1);
		//uc1601("孔几何+1!", 1);
	}

	//循环类型
	//holeDrillingBuilder1->CycleTable()->SetCycleType("Drill,Deep"); //深孔钻Deep,Csink,Text,Drill,
	switch (HoleParameterSet.DCycling)
	{
	case 0:
		holeDrillingBuilder1->CycleTable()->SetCycleType("Drill");
		break;
	case 1:
		holeDrillingBuilder1->CycleTable()->SetCycleType("Drill,Text");
		break;
	case 2:
		holeDrillingBuilder1->CycleTable()->SetCycleType("Drill,Csink");
		break;
	case 3:
		holeDrillingBuilder1->CycleTable()->SetCycleType("Drill,Deep");
		break;
	case 4:
		holeDrillingBuilder1->CycleTable()->SetCycleType("Drill,Deep,BreakChip");
		break;
	case 5:
		holeDrillingBuilder1->CycleTable()->SetCycleType("Drill,Tap");
		break;
	case 6:
		holeDrillingBuilder1->CycleTable()->SetCycleType("Drill,Tap,Float");
		break;
	case 7:
		holeDrillingBuilder1->CycleTable()->SetCycleType("Drill,Tap,Deep");
		break;
	case 8:
		holeDrillingBuilder1->CycleTable()->SetCycleType("Drill,Tap,BreakChip");
		break;
	case 9:
		holeDrillingBuilder1->CycleTable()->SetCycleType("Drill,Bore");
		break;
	case 10:
		holeDrillingBuilder1->CycleTable()->SetCycleType("Drill,Bore,Drag");
		break;
	case 11:
		holeDrillingBuilder1->CycleTable()->SetCycleType("Drill,Bore,Nodrag");
		break;
	case 12:
		holeDrillingBuilder1->CycleTable()->SetCycleType("Drill,Bore,Back");
		break;
	case 13:
		holeDrillingBuilder1->CycleTable()->SetCycleType("Drill,Bore,Manual");
		break;
	default:
		holeDrillingBuilder1->CycleTable()->SetCycleType("Drill");
		break;
	}

	//偏置
	NXOpen::CAM::HoleDrillingCutParameters *holeDrillingCutParameters1(dynamic_cast<NXOpen::CAM::HoleDrillingCutParameters *>(holeMachiningCutParameters1));
	NXOpen::CAM::VerticalPosition *verticalPosition1;
	holeDrillingCutParameters1->BottomOffset()->SetDistance(HoleParameterSet.DBottomOffset);//底部
	NXOpen::CAM::VerticalPosition *verticalPosition2;
	holeDrillingCutParameters1->RaptoOffset()->SetDistance(HoleParameterSet.DRaptoOffset);//平行
	//顶部
	holeDrillingBuilder1->CuttingParameters()->TopOffset()->SetDistance(HoleParameterSet.DTopOffset);//BottomOffset,RaptoOffset
	NXObject *nXObject2 = holeDrillingBuilder1->Commit();

	//切削参数
	NXOpen::CAM::HoleDrillingBuilder *holeDrillingBuilder2;
	holeDrillingBuilder2 = workPart->CAMSetup()->CAMOperationCollection()->CreateHoleDrillingBuilder(holeDrilling1);

	holeDrillingBuilder2->FeedsBuilder()->SpindleRpmBuilder()->SetValue(HoleParameterSet.DMSpindleSpeed);
	holeDrillingBuilder2->FeedsBuilder()->FeedCutBuilder()->SetValue(HoleParameterSet.DMMachining);

	NXObject *nXObject1 = holeDrillingBuilder2->Commit();

	//生成刀轨
	std::vector<CAM::CAMObject *> objects1(1);
	objects1[0] = operation1;
	workPart->CAMSetup()->GenerateToolPath(objects1);
	UF_UI_ONT_refresh();

}
void SLHoleDrillingcmd::SpotFacingDecisioncmd(char* mcsname, std::vector<Face*> InWallVectorList)
{

	CAM::NCGroup *nCGroup1(dynamic_cast<CAM::NCGroup *>(workPart->CAMSetup()->CAMGroupCollection()->FindObject(HoleParameterSet.PROGRAM)));
	CAM::Method *method1(dynamic_cast<CAM::Method *>(workPart->CAMSetup()->CAMGroupCollection()->FindObject(HoleParameterSet.METHOD)));
	CAM::Tool *tool1(dynamic_cast<CAM::Tool *>(workPart->CAMSetup()->CAMGroupCollection()->FindObject(HoleParameterSet.SFToolMsg.ToolName)));
	CAM::Operation *operation1;

	CAM::OrientGeometry *orientGeometry33(dynamic_cast<CAM::OrientGeometry *>(workPart->CAMSetup()->CAMGroupCollection()->FindObject(mcsname)));//"MCS_G54",MCSName,Eachname,mcsName，凸台94806

	tag_t HoleInWallFaceTag = InWallVectorList[0]->Tag();
	std::string tagStr = std::to_string(HoleInWallFaceTag);
	NXString nxString(tagStr.c_str());
	NXString OperationName = "SpotFacing." + nxString;


	//创建操作;
	try
	{
		operation1 = workPart->CAMSetup()->CAMOperationCollection()->Create(nCGroup1, method1, tool1, orientGeometry33, "hole_making", "DRILLING", NXOpen::CAM::OperationCollection::UseDefaultNameFalse, OperationName); // "DRILLING"
	}
	catch (...)
	{
		operation1 = workPart->CAMSetup()->CAMOperationCollection()->Create(nCGroup1, method1, tool1, orientGeometry33, "hole_making", "DRILLING", NXOpen::CAM::OperationCollection::UseDefaultNameTrue, OperationName);
		//uc1601("catch exception!", 1);
	}
	NXOpen::CAM::HoleDrilling *holeDrilling1(dynamic_cast<NXOpen::CAM::HoleDrilling *>(operation1));
	NXOpen::CAM::HoleDrillingBuilder *holeDrillingBuilder1;
	holeDrillingBuilder1 = workPart->CAMSetup()->CAMOperationCollection()->CreateHoleDrillingBuilder(holeDrilling1);
	NXOpen::CAM::HoleMachiningCutParameters *holeMachiningCutParameters1;
	holeMachiningCutParameters1 = holeDrillingBuilder1->CuttingParameters();
	NXOpen::CAM::FBM::FeatureGeometry *featureGeometry1;
	featureGeometry1 = holeDrillingBuilder1->GetFeatureGeometry();

	NXOpen::CAM::FBM::MachiningFeatureGeometry *machiningFeatureGeometry1(dynamic_cast<NXOpen::CAM::FBM::MachiningFeatureGeometry *>(featureGeometry1));

	//创建孔几何
	NXOpen::CAM::GeometrySetList *geometrySetList1;
	geometrySetList1 = machiningFeatureGeometry1->GeometryList();
	NXOpen::CAM::CAMFeature *nullNXOpen_CAM_CAMFeature(NULL);
	//uc1601("创建孔几何!", 1);
	for (std::vector<Face*>::size_type k = 0; k != InWallVectorList.size(); k++)
	{
		NXOpen::CAM::FBM::FeatureSet *featureSet1;
		featureSet1 = machiningFeatureGeometry1->AddFeatureSet(nullNXOpen_CAM_CAMFeature, "NXHOLE");//钻操作换成"NXHOLE"

		geometrySetList1->Append(featureSet1);

		featureSet1->SetAngleToleranceEdges(0.0);
		featureSet1->SetIntol(0.0);
		featureSet1->SetOuttol(0.0);

		//赋值孔内壁面
		std::vector<NXOpen::NXObject *> entities1(1);
		entities1[0] = InWallVectorList[k];
		NXOpen::CAM::FBM::Feature *feature1;
		feature1 = featureSet1->CreateFeature(entities1);
		//uc1601("孔几何+1!", 1);
	}

	//循环类型
	//holeDrillingBuilder1->CycleTable()->SetCycleType("Drill,Deep"); //深孔钻Deep,Csink,Text,Drill,
	switch (HoleParameterSet.SFCycling)
	{
	case 0:
		holeDrillingBuilder1->CycleTable()->SetCycleType("Drill");
		break;
	case 1:
		holeDrillingBuilder1->CycleTable()->SetCycleType("Drill,Text");
		break;
	case 2:
		holeDrillingBuilder1->CycleTable()->SetCycleType("Drill,Csink");
		break;
	case 3:
		holeDrillingBuilder1->CycleTable()->SetCycleType("Drill,Deep");
		break;
	case 4:
		holeDrillingBuilder1->CycleTable()->SetCycleType("Drill,Deep,BreakChip");
		break;
	case 5:
		holeDrillingBuilder1->CycleTable()->SetCycleType("Drill,Tap");
		break;
	case 6:
		holeDrillingBuilder1->CycleTable()->SetCycleType("Drill,Tap,Float");
		break;
	case 7:
		holeDrillingBuilder1->CycleTable()->SetCycleType("Drill,Tap,Deep");
		break;
	case 8:
		holeDrillingBuilder1->CycleTable()->SetCycleType("Drill,Tap,BreakChip");
		break;
	case 9:
		holeDrillingBuilder1->CycleTable()->SetCycleType("Drill,Bore");
		break;
	case 10:
		holeDrillingBuilder1->CycleTable()->SetCycleType("Drill,Bore,Drag");
		break;
	case 11:
		holeDrillingBuilder1->CycleTable()->SetCycleType("Drill,Bore,Nodrag");
		break;
	case 12:
		holeDrillingBuilder1->CycleTable()->SetCycleType("Drill,Bore,Back");
		break;
	case 13:
		holeDrillingBuilder1->CycleTable()->SetCycleType("Drill,Bore,Manual");
		break;
	default:
		holeDrillingBuilder1->CycleTable()->SetCycleType("Drill");
		break;
	}

	//偏置
	NXOpen::CAM::HoleDrillingCutParameters *holeDrillingCutParameters1(dynamic_cast<NXOpen::CAM::HoleDrillingCutParameters *>(holeMachiningCutParameters1));
	NXOpen::CAM::VerticalPosition *verticalPosition1;
	holeDrillingCutParameters1->BottomOffset()->SetDistance(HoleParameterSet.SFBottomOffset);//底部
	NXOpen::CAM::VerticalPosition *verticalPosition2;
	holeDrillingCutParameters1->RaptoOffset()->SetDistance(HoleParameterSet.SFRaptoOffset);//平行
	//顶部
	holeDrillingBuilder1->CuttingParameters()->TopOffset()->SetDistance(HoleParameterSet.SFTopOffset);//BottomOffset,RaptoOffset
	NXObject *nXObject2 = holeDrillingBuilder1->Commit();

	//切削参数
	NXOpen::CAM::HoleDrillingBuilder *holeDrillingBuilder2;
	holeDrillingBuilder2 = workPart->CAMSetup()->CAMOperationCollection()->CreateHoleDrillingBuilder(holeDrilling1);

	holeDrillingBuilder2->FeedsBuilder()->SpindleRpmBuilder()->SetValue(HoleParameterSet.SFMSpindleSpeed);
	holeDrillingBuilder2->FeedsBuilder()->FeedCutBuilder()->SetValue(HoleParameterSet.SFMMachining);
	NXObject *nXObject1 = holeDrillingBuilder2->Commit();

	//生成刀轨
	std::vector<CAM::CAMObject *> objects1(1);
	objects1[0] = operation1;
	workPart->CAMSetup()->GenerateToolPath(objects1);
	UF_UI_ONT_refresh();
}
void SLHoleDrillingcmd::ReamingDecisioncmd(char* mcsname, std::vector<Face*> InWallVectorList)
{

	CAM::NCGroup *nCGroup1(dynamic_cast<CAM::NCGroup *>(workPart->CAMSetup()->CAMGroupCollection()->FindObject(HoleParameterSet.PROGRAM)));
	CAM::Method *method1(dynamic_cast<CAM::Method *>(workPart->CAMSetup()->CAMGroupCollection()->FindObject(HoleParameterSet.METHOD)));
	CAM::Tool *tool1(dynamic_cast<CAM::Tool *>(workPart->CAMSetup()->CAMGroupCollection()->FindObject(HoleParameterSet.RToolMsg.ToolName)));
	CAM::Operation *operation1;
	CAM::OrientGeometry *orientGeometry33(dynamic_cast<CAM::OrientGeometry *>(workPart->CAMSetup()->CAMGroupCollection()->FindObject(mcsname)));//"MCS_G54",MCSName,Eachname,mcsName，凸台94806

	tag_t HoleInWallFaceTag = InWallVectorList[0]->Tag();
	std::string tagStr = std::to_string(HoleInWallFaceTag);
	NXString nxString(tagStr.c_str());
	NXString OperationName = "Reaming." + nxString;


	//创建操作;
	try
	{
		operation1 = workPart->CAMSetup()->CAMOperationCollection()->Create(nCGroup1, method1, tool1, orientGeometry33, "hole_making", "DRILLING", NXOpen::CAM::OperationCollection::UseDefaultNameFalse, OperationName); // "DRILLING"
	}
	catch (...)
	{
		operation1 = workPart->CAMSetup()->CAMOperationCollection()->Create(nCGroup1, method1, tool1, orientGeometry33, "hole_making", "DRILLING", NXOpen::CAM::OperationCollection::UseDefaultNameTrue, OperationName);
		//uc1601("catch exception!", 1);
	}
	NXOpen::CAM::HoleDrilling *holeDrilling1(dynamic_cast<NXOpen::CAM::HoleDrilling *>(operation1));
	NXOpen::CAM::HoleDrillingBuilder *holeDrillingBuilder1;
	holeDrillingBuilder1 = workPart->CAMSetup()->CAMOperationCollection()->CreateHoleDrillingBuilder(holeDrilling1);
	NXOpen::CAM::HoleMachiningCutParameters *holeMachiningCutParameters1;
	holeMachiningCutParameters1 = holeDrillingBuilder1->CuttingParameters();
	NXOpen::CAM::FBM::FeatureGeometry *featureGeometry1;
	featureGeometry1 = holeDrillingBuilder1->GetFeatureGeometry();

	NXOpen::CAM::FBM::MachiningFeatureGeometry *machiningFeatureGeometry1(dynamic_cast<NXOpen::CAM::FBM::MachiningFeatureGeometry *>(featureGeometry1));

	//创建孔几何
	NXOpen::CAM::GeometrySetList *geometrySetList1;
	geometrySetList1 = machiningFeatureGeometry1->GeometryList();
	NXOpen::CAM::CAMFeature *nullNXOpen_CAM_CAMFeature(NULL);
	//uc1601("创建孔几何!", 1);

	for (std::vector<Face*>::size_type k = 0; k != InWallVectorList.size(); k++)
	{
		NXOpen::CAM::FBM::FeatureSet *featureSet1;
		featureSet1 = machiningFeatureGeometry1->AddFeatureSet(nullNXOpen_CAM_CAMFeature, "NXHOLE");//钻操作换成"NXHOLE"

		geometrySetList1->Append(featureSet1);

		featureSet1->SetAngleToleranceEdges(0.0);
		featureSet1->SetIntol(0.0);
		featureSet1->SetOuttol(0.0);

		//赋值孔内壁面
		std::vector<NXOpen::NXObject *> entities1(1);
		entities1[0] = InWallVectorList[k];
		NXOpen::CAM::FBM::Feature *feature1;
		feature1 = featureSet1->CreateFeature(entities1);
		//uc1601("孔几何+1!", 1);
	}

	//循环类型
	//holeDrillingBuilder1->CycleTable()->SetCycleType("Drill,Deep"); //深孔钻Deep,Csink,Text,Drill,
	switch (HoleParameterSet.RMCycling)
	{
	case 0:
		holeDrillingBuilder1->CycleTable()->SetCycleType("Drill");
		break;
	case 1:
		holeDrillingBuilder1->CycleTable()->SetCycleType("Drill,Text");
		break;
	case 2:
		holeDrillingBuilder1->CycleTable()->SetCycleType("Drill,Csink");
		break;
	case 3:
		holeDrillingBuilder1->CycleTable()->SetCycleType("Drill,Deep");
		break;
	case 4:
		holeDrillingBuilder1->CycleTable()->SetCycleType("Drill,Deep,BreakChip");
		break;
	case 5:
		holeDrillingBuilder1->CycleTable()->SetCycleType("Drill,Tap");
		break;
	case 6:
		holeDrillingBuilder1->CycleTable()->SetCycleType("Drill,Tap,Float");
		break;
	case 7:
		holeDrillingBuilder1->CycleTable()->SetCycleType("Drill,Tap,Deep");
		break;
	case 8:
		holeDrillingBuilder1->CycleTable()->SetCycleType("Drill,Tap,BreakChip");
		break;
	case 9:
		holeDrillingBuilder1->CycleTable()->SetCycleType("Drill,Bore");
		break;
	case 10:
		holeDrillingBuilder1->CycleTable()->SetCycleType("Drill,Bore,Drag");
		break;
	case 11:
		holeDrillingBuilder1->CycleTable()->SetCycleType("Drill,Bore,Nodrag");
		break;
	case 12:
		holeDrillingBuilder1->CycleTable()->SetCycleType("Drill,Bore,Back");
		break;
	case 13:
		holeDrillingBuilder1->CycleTable()->SetCycleType("Drill,Bore,Manual");
		break;
	default:
		holeDrillingBuilder1->CycleTable()->SetCycleType("Drill");
		break;
	}

	//偏置
	NXOpen::CAM::HoleDrillingCutParameters *holeDrillingCutParameters1(dynamic_cast<NXOpen::CAM::HoleDrillingCutParameters *>(holeMachiningCutParameters1));
	NXOpen::CAM::VerticalPosition *verticalPosition1;
	holeDrillingCutParameters1->BottomOffset()->SetDistance(HoleParameterSet.RMBottomOffset);//底部
	NXOpen::CAM::VerticalPosition *verticalPosition2;
	holeDrillingCutParameters1->RaptoOffset()->SetDistance(HoleParameterSet.RMRaptoOffset);//平行
	//顶部
	holeDrillingBuilder1->CuttingParameters()->TopOffset()->SetDistance(HoleParameterSet.RMTopOffset);//BottomOffset,RaptoOffset
	NXObject *nXObject2 = holeDrillingBuilder1->Commit();

	//切削参数
	NXOpen::CAM::HoleDrillingBuilder *holeDrillingBuilder2;
	holeDrillingBuilder2 = workPart->CAMSetup()->CAMOperationCollection()->CreateHoleDrillingBuilder(holeDrilling1);

	holeDrillingBuilder2->FeedsBuilder()->SpindleRpmBuilder()->SetValue(HoleParameterSet.RMSpindleSpeed);
	holeDrillingBuilder2->FeedsBuilder()->FeedCutBuilder()->SetValue(HoleParameterSet.RMMachining);
	NXObject *nXObject1 = holeDrillingBuilder2->Commit();

	//生成刀轨
	std::vector<CAM::CAMObject *> objects1(1);
	objects1[0] = operation1;
	workPart->CAMSetup()->GenerateToolPath(objects1);
	UF_UI_ONT_refresh();
}
void SLHoleDrillingcmd::BoringDecisioncmd(char* mcsname, std::vector<Face*> InWallVectorList)
{

	CAM::NCGroup *nCGroup1(dynamic_cast<CAM::NCGroup *>(workPart->CAMSetup()->CAMGroupCollection()->FindObject(HoleParameterSet.PROGRAM)));
	CAM::Method *method1(dynamic_cast<CAM::Method *>(workPart->CAMSetup()->CAMGroupCollection()->FindObject(HoleParameterSet.METHOD)));
	CAM::Tool *tool1(dynamic_cast<CAM::Tool *>(workPart->CAMSetup()->CAMGroupCollection()->FindObject(HoleParameterSet.BToolMsg.ToolName)));
	CAM::Operation *operation1;
	CAM::OrientGeometry *orientGeometry33(dynamic_cast<CAM::OrientGeometry *>(workPart->CAMSetup()->CAMGroupCollection()->FindObject(mcsname)));//"MCS_G54",MCSName,Eachname,mcsName，凸台94806

	tag_t HoleInWallFaceTag = InWallVectorList[0]->Tag();
	std::string tagStr = std::to_string(HoleInWallFaceTag);
	NXString nxString(tagStr.c_str());
	NXString OperationName = "Boring." + nxString;


	//创建操作;
	try
	{
		operation1 = workPart->CAMSetup()->CAMOperationCollection()->Create(nCGroup1, method1, tool1, orientGeometry33, "hole_making", "DRILLING", NXOpen::CAM::OperationCollection::UseDefaultNameFalse, OperationName); // "DRILLING"
	}
	catch (...)
	{
		operation1 = workPart->CAMSetup()->CAMOperationCollection()->Create(nCGroup1, method1, tool1, orientGeometry33, "hole_making", "DRILLING", NXOpen::CAM::OperationCollection::UseDefaultNameTrue, OperationName);
		//uc1601("catch exception!", 1);
	}
	NXOpen::CAM::HoleDrilling *holeDrilling1(dynamic_cast<NXOpen::CAM::HoleDrilling *>(operation1));
	NXOpen::CAM::HoleDrillingBuilder *holeDrillingBuilder1;
	holeDrillingBuilder1 = workPart->CAMSetup()->CAMOperationCollection()->CreateHoleDrillingBuilder(holeDrilling1);
	NXOpen::CAM::HoleMachiningCutParameters *holeMachiningCutParameters1;
	holeMachiningCutParameters1 = holeDrillingBuilder1->CuttingParameters();
	NXOpen::CAM::FBM::FeatureGeometry *featureGeometry1;
	featureGeometry1 = holeDrillingBuilder1->GetFeatureGeometry();

	NXOpen::CAM::FBM::MachiningFeatureGeometry *machiningFeatureGeometry1(dynamic_cast<NXOpen::CAM::FBM::MachiningFeatureGeometry *>(featureGeometry1));

	//创建孔几何
	NXOpen::CAM::GeometrySetList *geometrySetList1;
	geometrySetList1 = machiningFeatureGeometry1->GeometryList();
	NXOpen::CAM::CAMFeature *nullNXOpen_CAM_CAMFeature(NULL);

	//uc1601("创建孔几何!", 1);
	for (std::vector<Face*>::size_type k = 0; k != InWallVectorList.size(); k++)
	{
		NXOpen::CAM::FBM::FeatureSet *featureSet1;
		featureSet1 = machiningFeatureGeometry1->AddFeatureSet(nullNXOpen_CAM_CAMFeature, "NXHOLE");//钻操作换成"NXHOLE"

		geometrySetList1->Append(featureSet1);

		featureSet1->SetAngleToleranceEdges(0.0);
		featureSet1->SetIntol(0.0);
		featureSet1->SetOuttol(0.0);

		//赋值孔内壁面
		std::vector<NXOpen::NXObject *> entities1(1);
		entities1[0] = InWallVectorList[k];
		NXOpen::CAM::FBM::Feature *feature1;
		feature1 = featureSet1->CreateFeature(entities1);
		//uc1601("孔几何+1!", 1);
	}

	//循环类型
	holeDrillingBuilder1->CycleTable()->SetCycleType("Drill,Deep"); //深孔钻Deep,Csink,Text,Drill,
	switch (HoleParameterSet.BMCycling)
	{
	case 0:
		holeDrillingBuilder1->CycleTable()->SetCycleType("Drill");
		break;
	case 1:
		holeDrillingBuilder1->CycleTable()->SetCycleType("Drill,Text");
		break;
	case 2:
		holeDrillingBuilder1->CycleTable()->SetCycleType("Drill,Csink");
		break;
	case 3:
		holeDrillingBuilder1->CycleTable()->SetCycleType("Drill,Deep");
		break;
	case 4:
		holeDrillingBuilder1->CycleTable()->SetCycleType("Drill,Deep,BreakChip");
		break;
	case 5:
		holeDrillingBuilder1->CycleTable()->SetCycleType("Drill,Tap");
		break;
	case 6:
		holeDrillingBuilder1->CycleTable()->SetCycleType("Drill,Tap,Float");
		break;
	case 7:
		holeDrillingBuilder1->CycleTable()->SetCycleType("Drill,Tap,Deep");
		break;
	case 8:
		holeDrillingBuilder1->CycleTable()->SetCycleType("Drill,Tap,BreakChip");
		break;
	case 9:
		holeDrillingBuilder1->CycleTable()->SetCycleType("Drill,Bore");
		break;
	case 10:
		holeDrillingBuilder1->CycleTable()->SetCycleType("Drill,Bore,Drag");
		break;
	case 11:
		holeDrillingBuilder1->CycleTable()->SetCycleType("Drill,Bore,Nodrag");
		break;
	case 12:
		holeDrillingBuilder1->CycleTable()->SetCycleType("Drill,Bore,Back");
		break;
	case 13:
		holeDrillingBuilder1->CycleTable()->SetCycleType("Drill,Bore,Manual");
		break;
	default:
		holeDrillingBuilder1->CycleTable()->SetCycleType("Drill");
		break;
	}

	//偏置
	NXOpen::CAM::HoleDrillingCutParameters *holeDrillingCutParameters1(dynamic_cast<NXOpen::CAM::HoleDrillingCutParameters *>(holeMachiningCutParameters1));
	NXOpen::CAM::VerticalPosition *verticalPosition1;
	holeDrillingCutParameters1->BottomOffset()->SetDistance(HoleParameterSet.BMBottomOffset);//底部
	NXOpen::CAM::VerticalPosition *verticalPosition2;
	holeDrillingCutParameters1->RaptoOffset()->SetDistance(HoleParameterSet.BMRaptoOffset);//平行
	//顶部
	holeDrillingBuilder1->CuttingParameters()->TopOffset()->SetDistance(HoleParameterSet.BMTopOffset);//BottomOffset,RaptoOffset
	NXObject *nXObject2 = holeDrillingBuilder1->Commit();

	//切削参数
	NXOpen::CAM::HoleDrillingBuilder *holeDrillingBuilder2;
	holeDrillingBuilder2 = workPart->CAMSetup()->CAMOperationCollection()->CreateHoleDrillingBuilder(holeDrilling1);

	holeDrillingBuilder2->FeedsBuilder()->SpindleRpmBuilder()->SetValue(HoleParameterSet.BMSpindleSpeed);
	holeDrillingBuilder2->FeedsBuilder()->FeedCutBuilder()->SetValue(HoleParameterSet.BMMachining);
	NXObject *nXObject1 = holeDrillingBuilder2->Commit();

	//生成刀轨
	std::vector<CAM::CAMObject *> objects1(1);
	objects1[0] = operation1;
	workPart->CAMSetup()->GenerateToolPath(objects1);
	UF_UI_ONT_refresh();
}




Point3d SLHoleDrillingcmd::GetCenterPoint(Face *face)//小满——获得中心点
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
	};//可选对象类型
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

tag_t SLHoleDrillingcmd::SelectPart()//工件单选对话框，每次选取一个对象，对象类型可设置
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

tag_t SLHoleDrillingcmd::SelectBlank()//毛坯单选对话框，每次选取一个对象，对象类型可设置
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

//获取面中心点法矢，实体默认法矢向外
Point3d SLHoleDrillingcmd::get_Centre_fr_Face(Face* ipFace)
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
	Point3d centernormal(unit_norm[0], unit_norm[1], unit_norm[2]);

	return centernormal;
}

double SLHoleDrillingcmd::DotProduct(double a[3], double b[3])
{
	double result;
	result = a[0] * b[0] + a[1] * b[1] + a[2] * b[2];
	return result;

}

void SLHoleDrillingcmd::CreateMCS(tag_t TopFaceTag, double Facenormal[3], char *Eachname, double wcsZ[3], double origin[3])//创建局部加工坐标系,Face* MCSTopFace
{

	//2.2 获得NX自动捕获的顶面上的坐标原点、矩阵坐标

	//坐标矩阵
	//tag_t EachFaceTag = MCSTopFace->Tag();
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

	int boolbutton = 0;

	if (abs(product1) < Epslion  && product2 > 0)
	{
		boolbutton = 1;
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
		else if (product3 < 0)
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
		else{};
	}
	else{};
	if (abs(product1) < Epslion  && product2 < 0) //首先反向autoY
	{
		boolbutton = 1;
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
		else if (product4 < 0) //说明autoX有问题，需要反向
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
		else{};

	}
	else{};
	if (abs(product2) < Epslion  && product1 > 0) //说明Y方向占了我们所希望的X的方向
	{
		boolbutton = 1;
		
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
		else if (product4 < 0)
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
		else{};
	}
	else{};
	if (abs(product2) < Epslion && product1 < 0)//说明Y方向占了我们所希望的X的方向,autoX方向也是反的
	{
		boolbutton = 1;
		
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
		else if (product4 < 0) //要反向最后的x方向
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
		else{};
	}
	else{};


	tag_t MatrixID = NULL_TAG;
	UF_CSYS_create_matrix(targetMTx, &MatrixID);

	double CsysOrigin[3] = { origin[0], origin[1], origin[2] }; //坐标原点
	tag_t CsysId = NULL_TAG;
	//UF_CSYS_create_temp_csys(CsysOrigin, MatrixTag, &CsysId);//创建临时坐标系（不显示在屏幕）
	if (boolbutton == 0)
	{
		UF_CSYS_create_temp_csys(CsysOrigin, MatrixTag, &CsysId);//创建临时坐标系（不显示在屏幕）
	}
	else
	{
		UF_CSYS_create_temp_csys(CsysOrigin, MatrixID, &CsysId);//创建临时坐标系（不显示在屏幕）
	}

	//创建新的加工坐标系MCS
	tag_t  newMCSObject = NULL_TAG;
	UF_NCGEOM_create("mill_planar", "MCS", &newMCSObject);

	//设定新加工坐标系名称
	const char *MCSName = nullptr;
	MCSName = Eachname;        
	UF_OBJ_set_name(newMCSObject, MCSName);

	//2.3 在当前几何视图根组下插入新的坐标系,并将新的坐标系位置设置到临时坐标系的位置

	//获取当前UF_SETUP对象
	tag_t setupTag = NULL_TAG;
	UF_SETUP_ask_setup(&setupTag);

	if (setupTag == NULL_TAG)
	{
		uc1601("提示:请先初始化加工环境", 1);
		return;
	}
	else{};

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
			//uc1601("不接受新建的成员？", 1);
		}
		else{};
		if (answer)
		{
			error = UF_NCGROUP_accept_member(GeomRootGroup, newMCSObject);
			if (error)
			{
				//uc1601("在根组中添加失败", 1);
			}
			else
			{
				//在插入后，这里将临时坐标系设置给新的这个加工坐标
				UF_PARAM_set_tag_value(newMCSObject, UF_PARAM_MCS, CsysId);
				//刷新导航器
				UF_UI_ONT_refresh();
				//uc1601("坐标系创建成功", 1);
			}
		}
		else{};
	}
	else
	{
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

		/*uc1601(WorkpieceName, 1);*/
		/*uc1601("已存在同名坐标系", 1);*/
	}

}
//三阶行列式计算
double SLHoleDrillingcmd::Get_result_determinant(double D[9])
{
	//坐标变换，求壁面中心点在wcs下的相对坐标;
	double A = D[0] * (D[4] * D[8] - D[5] * D[7]);
	double B = D[1] * (D[3] * D[8] - D[5] * D[6]);
	double C = D[2] * (D[3] * D[7] - D[4] * D[6]);
	double R = A-B+C;
	return R;
}

//已知一点的绝对坐标，wcs坐标系的原点和坐标矩阵，求改点在wcs坐标系下的相对坐标；
Point3d SLHoleDrillingcmd::Get_opposite_coordinate(Point3d centeripoint, double wcs_origin[3], double wcsMatrixValues[9])
{
	//坐标变换，求壁面中心点在wcs下的相对坐标;
	double m = centeripoint.X - wcs_origin[0];
	double n = centeripoint.Y - wcs_origin[1];
	double k = centeripoint.Z - wcs_origin[2];
	double w0 = wcsMatrixValues[0];
	double w1 = wcsMatrixValues[1];
	double w2 = wcsMatrixValues[2];
	double w3 = wcsMatrixValues[3];
	double w4 = wcsMatrixValues[4];
	double w5 = wcsMatrixValues[5];
	double w6 = wcsMatrixValues[6];
	double w7 = wcsMatrixValues[7];
	double w8 = wcsMatrixValues[8];

	//double D[9] = { w0, w1, w2, w3, w4, w5, w6, w7, w8 };
	//double Dx[9] = { m, w1, w2, n, w4, w5, k, w7, w8 };
	//double Dy[9] = { w0, m, w2, w3, n, w5, w6, k, w8 };
	//double Dz[9] = { w0, w1, m, w3, w4, n, w6, w7, k };

	double D[9] = { w0, w3, w6, w1, w4, w7, w2, w5, w8 };
	double Dx[9] = { m, w3, w6, n, w4, w7, k, w5, w8 };
	double Dy[9] = { w0, m, w6, w1, n, w7, w2, k, w8 };
	double Dz[9] = { w0, w3, m, w1, w4, n, w2, w5, k };
	double a;
	double b;
	double c;
	a = Get_result_determinant(Dx) / Get_result_determinant(D);
	b = Get_result_determinant(Dy) / Get_result_determinant(D);
	c = Get_result_determinant(Dz) / Get_result_determinant(D);

	Point3d R;
	R.X = a;
	R.Y = b;
	R.Z = c;
	return R;

}

//已知点在wcs坐标系下的相对坐标，wcs坐标系的原点和坐标矩阵，求该点的绝对坐标；
Point3d SLHoleDrillingcmd::Get_absolute_coordinate(double opposite_point[3], double wcs_origin[3], double wcsMatrixValues[9])
{
	//坐标变换，求壁面中心点在wcs下的相对坐标;
	double A = wcs_origin[0] + opposite_point[0] * wcsMatrixValues[0] + opposite_point[1] * wcsMatrixValues[3] + opposite_point[2] * wcsMatrixValues[6];
	double B = wcs_origin[1] + opposite_point[0] * wcsMatrixValues[1] + opposite_point[1] * wcsMatrixValues[4] + opposite_point[2] * wcsMatrixValues[7];
	double C = wcs_origin[2] + opposite_point[0] * wcsMatrixValues[2] + opposite_point[1] * wcsMatrixValues[5] + opposite_point[2] * wcsMatrixValues[8];
	Point3d R;
	R.X = A;
	R.Y = B;
	R.Z = C;
	return R;
}

void SLHoleDrillingcmd::holeClassification(std::vector<DrGeo> AllDrGeo)
{
	//uc1601("HoleSpotDrillingDecision!", 1);
	char *BossHole = "凸台";
	char *BossotherHole = "其它孔";
	char *RibHole = "筋面";
	char *WallHole = "壁面";
	char *AxisHole = "轴向";
	AxisDrGeo.clear();//轴向孔
	BossDrGeo.clear();//凸台孔
	WallRibDrGeo.clear();//筋面和壁面孔

	std::vector <DrGeo>::iterator  it;
	for (it = AllDrGeo.begin(); it != AllDrGeo.end(); it++)
	{
		char *Eachname = it->selectedHoleName;
		if (strstr(Eachname, AxisHole) != NULL)//端面孔
		{
			AxisDrGeo.push_back(*it);
		}
		else if ((strstr(Eachname, BossHole) != NULL) || (strstr(Eachname, BossotherHole) != NULL))//凸台孔
		{
			BossDrGeo.push_back(*it);
		}
		else if (strstr(Eachname, RibHole) != NULL || strstr(Eachname, WallHole) != NULL)//壁面孔
		{
			WallRibDrGeo.push_back(*it);
		}
		else{};
	}

}

void SLHoleDrillingcmd::holeClassification2(std::vector<DrGeo> AllDrGeo)
{
	//uc1601("HoleSpotDrillingDecision!", 1);
	char *BossHole = "凸台";
	char *BossotherHole = "其它孔";
	char *RibHole = "筋面";
	char *WallHole = "壁面";
	char *AxisHole = "轴向";
	AxisDrGeo2.clear();//轴向孔
	BossDrGeo2.clear();//凸台孔
	WallRibDrGeo2.clear();//筋面和壁面孔

	std::vector <DrGeo>::iterator  it;
	for (it = AllDrGeo.begin(); it != AllDrGeo.end(); it++)
	{
		char *Eachname = it->selectedHoleName;
		if (strstr(Eachname, AxisHole) != NULL)//端面孔
		{
			AxisDrGeo2.push_back(*it);
		}
		else if ((strstr(Eachname, BossHole) != NULL) || (strstr(Eachname, BossotherHole) != NULL))//凸台孔
		{
			BossDrGeo2.push_back(*it);
		}
		else if (strstr(Eachname, RibHole) != NULL || strstr(Eachname, WallHole) != NULL)//筋面和壁面孔
		{
			WallRibDrGeo2.push_back(*it);
		}
		else{};
	}

}





