#undef CreateDialog
#ifndef FEATUREPUBLICCLASS_H
#define FEATUREPUBLICCLASS_H

#include <vector>
#include <NXOpen/Face.hxx>

//=============================付定义的凸台类结构体======================================

struct DoubleLayerBoss
{
	std::vector<NXOpen::Face*> TopFaceVector;//上顶面
	std::vector<NXOpen::Face*> SideFaceVector;
	std::vector<NXOpen::Face*> NTopFaceVector;
	std::vector<NXOpen::Face*> NSideFaceVector;
	std::vector<NXOpen::Face*> ConnectFaceVector;
	std::vector<NXOpen::Face*> BottomCornerFaceVector;
	std::vector<NXOpen::Face*> BottomFaceVector;  //凸台底面 容器
	bool OutOrIn;//0代表内，1代表外
	bool ifselected = 0;//被选中为1,否则为0
	char* name;

};

struct NormalBoss
{
	std::vector<NXOpen::Face*> TopFaceVector;
	std::vector<NXOpen::Face*> SideFaceVector;
	std::vector<NXOpen::Face*> BottomCornerFaceVector;
	std::vector<NXOpen::Face*> BottomFaceVector;  //凸台底面 容器
	bool OutOrIn;//0代表内，1代表外
	char* name;
	bool ifselected = 0;//被选中为1,否则为0
};

struct NoSideBoss
{
	std::vector<NXOpen::Face*> TopFaceVector;
	std::vector<NXOpen::Face*> BottomCornerFaceVector;
	std::vector<NXOpen::Face*> BottomFaceVector;  //凸台底面 容器
	bool OutOrIn;//0代表内，1代表外
	bool ifselected = 0;//被选中为1,否则为0
	char* name;
};


//=============================付定义的槽类结构体======================================

//环形槽特征
struct CircularGroove
{
	std::vector<NXOpen::Face*> TopFaceVector;
	std::vector<NXOpen::Face*> BottomCornerFaceVector;
	std::vector<NXOpen::Face*> LimitFaceVector;
	bool ifselected = 0;//被选中为1,否则为0
};

//下陷槽（曲面槽）特征
struct SubsidenceGroove
{
	std::vector<NXOpen::Face*> TopFaceVector;
	std::vector<NXOpen::Face*> BottomCornerFaceVector;
	bool ifselected = 0;//被选中为1,否则为0
	int deter = 0;
};

//平面浅槽
struct PlanerBlindGroove
{
	std::vector<NXOpen::Face*> BottomFaceVector;
	std::vector<NXOpen::Face*> BottomCornerFaceVector;
	std::vector<NXOpen::Face*> SideFaceVector;
	bool ifselected = 0;//被选中为1,否则为0
	char* name;
};

//通槽
struct PassGroove
{
	std::vector<NXOpen::Face*> TopFaceVector;
	std::vector<NXOpen::Face*> RightFaceVector;
	std::vector<NXOpen::Face*> SideFaceVector;
	std::vector<NXOpen::Face*> BottomFaceVector;
	bool ifselected = 0;//被选中为1,否则为0
	char* name;
};

//=============================付定义的筋类结构体======================================

//无约束筋
struct NoLimitRib
{
	std::vector<NXOpen::Face*> TopFaceVector;
	std::vector<NXOpen::Face*> SideFaceVector;
	std::vector<NXOpen::Face*> BottomCornerFaceVector;
	char* name;
};

//凸台约束筋
struct BossLimitRib
{
	std::vector<NXOpen::Face*> TopFaceVector;
	std::vector<NXOpen::Face*> LimitFaceVector;
	std::vector<NXOpen::Face*> SideFaceVector;
	std::vector<NXOpen::Face*> BottomCornerFaceVector;
	char* name;
};

//侧面约束筋
struct SideLimitRib
{
	std::vector<NXOpen::Face*> TopFaceVector;
	std::vector<NXOpen::Face*> LimitFaceVector;
	std::vector<NXOpen::Face*> SideFaceVector;
	std::vector<NXOpen::Face*> BottomCornerFaceVector;
	char* name;
};

//主轴规则回转筋
struct DirectionRib
{
	std::vector<NXOpen::Face*> TopFaceVector;
	std::vector<NXOpen::Face*> LimitFaceVector;
	std::vector<NXOpen::Face*> BottomCornerFaceVector;
	char* name;
	bool ifselected = 0;//被选中为1,否则为0
};

//曲面筋
struct HookFaceRib
{
	std::vector<NXOpen::Face*> TopFaceVector;
	std::vector<NXOpen::Face*> LimitFaceVector;
	std::vector<NXOpen::Face*> SideFaceVector;
	std::vector<NXOpen::Face*> BottomCornerFaceVector;
	char* name;
	bool ifselected = 0;//被选中为1,否则为0
};

//==============================付定义的孔类结构体================================


//轴向普通孔
struct DirectionNormalHole
{
	std::vector<NXOpen::Face*> TopFaceVector;
	std::vector<NXOpen::Face*> InWallVector;
	std::vector<NXOpen::Face*> BottomFaceVector;
	char* name;
};
//轴向单侧斜角通孔
struct DirectionRightHole
{
	std::vector<NXOpen::Face*> TopFaceVector;
	std::vector<NXOpen::Face*> RightVector;
	std::vector<NXOpen::Face*> InWallVector;
	std::vector<NXOpen::Face*> BottomFaceVector;
	char* name;
};
//轴向无倒角盲孔
struct DirectionBlindHole
{
	std::vector<NXOpen::Face*> TopFaceVector;
	std::vector<NXOpen::Face*> InWallVector;
	std::vector<NXOpen::Face*> BottomFaceVector;
	char* name;
};

//轴向两级阶梯通孔
struct DirectionStepThroughHole_31
{
	std::vector<NXOpen::Face*> TopFaceVector;
	std::vector<NXOpen::Face*> TopInWallVector;
	std::vector<NXOpen::Face*> PlanerVector;
	std::vector<NXOpen::Face*> BottomInWallVector;
	std::vector<NXOpen::Face*> BottomFaceVector;
	char* name;
};

//轴向长圆通孔
struct DirectionLongRoundThroughHole_31
{
	std::vector<NXOpen::Face*> TopFaceVector;
	std::vector<NXOpen::Face*> CylinderVector;
	std::vector<NXOpen::Face*> PlanerVector;
	std::vector<NXOpen::Face*> BottomFaceVector;
	char* name;
};

//新模型轴向普通盲孔
struct DirectionBlindHole_31
{
	std::vector<NXOpen::Face*> TopFaceVector;
	std::vector<NXOpen::Face*> InWallVector;
	std::vector<NXOpen::Face*> BottomFaceVector;
	char* name;
};
//轴向倒圆角盲孔
struct DirectionRoundBottomBlindHole_31
{
	std::vector<NXOpen::Face*> TopFaceVector;
	std::vector<NXOpen::Face*> InWallVector;
	std::vector<NXOpen::Face*> CircleVector;
	std::vector<NXOpen::Face*> BottomFaceVector;
	char* name;
};

//壁面普通通孔
struct WallNormalHole
{
	std::vector<NXOpen::Face*> TopFaceVector;
	std::vector<NXOpen::Face*> InWallVector;
	std::vector<NXOpen::Face*> BottomFaceVector;
	char* name;
};
//壁面复合通孔
struct WallCircleCombineHole
{
	std::vector<NXOpen::Face*> TopFaceVector;
	std::vector<NXOpen::Face*> TopInWallVector;
	std::vector<NXOpen::Face*> CircleVector;
	std::vector<NXOpen::Face*> PlanerVector;
	std::vector<NXOpen::Face*> BottomInWallVector;
	std::vector<NXOpen::Face*> BottomFaceVector;
	/*std::vector<NXOpen::Face*> WallotherVector;*/
	char* name;
};

//筋面复合通孔
struct RibCombineHole
{
	std::vector<NXOpen::Face*> TopFaceVector;
	std::vector<NXOpen::Face*> CircleVector;
	std::vector<NXOpen::Face*> PlanerVector;
	std::vector<NXOpen::Face*> InWallVector;
	std::vector<NXOpen::Face*> RightVector;
	std::vector<NXOpen::Face*> BottomFaceVector;
	/*std::vector<NXOpen::Face*> RibotherVector;*/
	

	char* name;
};

//凸台无倒角通孔
struct BossNormalHole
{
	std::vector<NXOpen::Face*> TopFaceVector;
	std::vector<NXOpen::Face*> InWallVector;
	std::vector<NXOpen::Face*> BottomFaceVector;
	char* name;
};

//凸台单侧斜角通孔
struct BossOneRightHole
{
	std::vector<NXOpen::Face*> TopFaceVector;
	std::vector<NXOpen::Face*> RightVector;
	std::vector<NXOpen::Face*> InWallVector;
	std::vector<NXOpen::Face*> BottomFaceVector;
	char* name;
};

//凸台单侧圆角通孔
struct BossOneCircleHole
{
	std::vector<NXOpen::Face*> TopFaceVector;
	std::vector<NXOpen::Face*> CircleVector;
	std::vector<NXOpen::Face*> InWallVector;
	std::vector<NXOpen::Face*> BottomFaceVector;
	char* name;
};

//凸台双侧斜角通孔
struct BossDoubleRightHole
{
	std::vector<NXOpen::Face*> TopFaceVector;
	std::vector<NXOpen::Face*> TopRightVector;
	std::vector<NXOpen::Face*> InWallVector;
	std::vector<NXOpen::Face*> BottomRightVector;
	std::vector<NXOpen::Face*> BottomFaceVector;
	char* name;
};

//凸台双侧圆角通孔
struct BossDoubleCircleHole
{
	std::vector<NXOpen::Face*> TopFaceVector;
	std::vector<NXOpen::Face*> TopCircleVector;
	std::vector<NXOpen::Face*> InWallVector;
	std::vector<NXOpen::Face*> BottomCircleVector;
	std::vector<NXOpen::Face*> BottomFaceVector;
	char* name;
};

//凸台单侧斜角单侧圆角通孔
struct BossOneRightOneCircleHole
{
	std::vector<NXOpen::Face*> TopFaceVector;
	std::vector<NXOpen::Face*> CircleVector;
	std::vector<NXOpen::Face*> InWallVector;
	std::vector<NXOpen::Face*> RightVector;
	std::vector<NXOpen::Face*> BottomFaceVector;
	char* name;
};

//凸台多层通孔
struct BossManyLayerHole
{
	std::vector<NXOpen::Face*> TopFaceVector;
	std::vector<NXOpen::Face*> RightVector;
	std::vector<NXOpen::Face*> CircleVector;
	std::vector<NXOpen::Face*> PlanerVector;
	std::vector<NXOpen::Face*> InWallVector;
	std::vector<NXOpen::Face*> BottomFaceVector;
	char* name;
};

//凸台外宽内窄复合通孔
struct BossOutsideWideCombineHole
{
	std::vector<NXOpen::Face*> TopFaceVector;
	std::vector<NXOpen::Face*> TopWallVector;
	std::vector<NXOpen::Face*> PlanerVector;
	std::vector<NXOpen::Face*> BottomWallVector;
	std::vector<NXOpen::Face*> BottomFaceVector;
	char* name;
};

//凸台内宽外窄复合通孔
struct BossInsideWideCombineHole
{
	std::vector<NXOpen::Face*> TopFaceVector;
	std::vector<NXOpen::Face*> TopWallVector;
	std::vector<NXOpen::Face*> PlanerVector;
	std::vector<NXOpen::Face*> BottomWallVector;
	std::vector<NXOpen::Face*> BottomFaceVector;
	char* name;
};


//凸台中圆角复合通孔
struct BossMidCricleCombineHole
{
	std::vector<NXOpen::Face*> TopFaceVector;
	std::vector<NXOpen::Face*> TopWallVector;
	std::vector<NXOpen::Face*> CircleVector;
	std::vector<NXOpen::Face*> PlanerVector;
	std::vector<NXOpen::Face*> BottomWallVector;
	std::vector<NXOpen::Face*> BottomFaceVector;
	char* name;
};

//凸台无倒角盲孔
struct BossNormalBlindHole
{
	std::vector<NXOpen::Face*> TopFaceVector;
	std::vector<NXOpen::Face*> InWallVector;
	std::vector<NXOpen::Face*> BottomFaceVector;
	char* name;
};

//凸台倒斜角盲孔
struct BossRightBlindHole
{
	std::vector<NXOpen::Face*> TopFaceVector;
	std::vector<NXOpen::Face*> RightVector;
	std::vector<NXOpen::Face*> InWallVector;
	std::vector<NXOpen::Face*> BottomFaceVector;
	char* name;
};


struct Other1Hole
{
	std::vector<NXOpen::Face*> OuterVector;
	std::vector<NXOpen::Face*> InWallVector;
	std::vector<NXOpen::Face*> InterVector;
	char* name;
};

struct Other2Hole
{
	std::vector<NXOpen::Face*> OuterVector;
	std::vector<NXOpen::Face*> InWallVector;
	std::vector<NXOpen::Face*> InterVector;
	char* name;
};

struct Other3Hole
{
	std::vector<NXOpen::Face*> OuterVector;
	std::vector<NXOpen::Face*> InWallVector;

	char* name;
};



//========================================================工艺决策部分重定义的基础类结构体====================================

//凸台
struct Boss
{
	std::vector<NXOpen::Face*> TopFaceVector;  //凸台顶面 容器
	std::vector<NXOpen::Face*> SideFaceVector;  //凸台侧面 容器
	std::vector<NXOpen::Face*> BottomCornerFaceVector;  //凸台底角面 容器
	std::vector<NXOpen::Face*> BottomFaceVector;  //凸台底面 容器
	char* name;
	bool OutOrIn;//0代表内，1代表外
};

//下陷槽
struct SubsidenceCavity
{
	std::vector<NXOpen::Face*> BottomFaceVector;
	std::vector<NXOpen::Face*> BottomCornerFaceVector;
	char* name;
};

//弧形槽
struct ArcCavity
{
	std::vector<NXOpen::Face*> BottomFaceVector;
	std::vector<NXOpen::Face*> BottomCornerFaceVector;
};

//筋
struct Rib
{
	std::vector<NXOpen::Face*> TopFaceVector;
	std::vector<NXOpen::Face*> LimitFaceVector;
	std::vector<NXOpen::Face*> SideFaceVector;
	std::vector<NXOpen::Face*> BottomCornerFaceVector;
	char* name;
};

//孔
struct JoniorHole
{
	std::vector<NXOpen::Face*> JoniorTopFaceVector;
	std::vector<NXOpen::Face*> JoniorInWallVector;
	std::vector<NXOpen::Face*> JoniorCornerVector;
	std::vector<NXOpen::Face*> JoniorBottomFaceVector;
};

struct Hole
{
	std::vector<NXOpen::Face*> TopFaceVector;
	std::vector<NXOpen::Face*> InWallVector;
	std::vector<NXOpen::Face*> CornerVector;
	std::vector<NXOpen::Face*> BottomFaceVector;
	std::vector<JoniorHole*> JoniorHoleVector;
	int NumLayers;  //以下三个统一定义：0标识无，1标识有,层数用数字标识
	int NumCorners;
	int NumThreads;
};

//孔特征列表集合
struct AllFeatureHole
{
	std::vector<DirectionNormalHole*> ListOfDirectionNormalHole;//轴向普通孔列表
	std::vector<DirectionRightHole*> ListOfDirectionRightHole;//轴向单侧斜角通孔
	std::vector<DirectionBlindHole*> ListOfDirectionBlindHole;//轴向无倒角盲孔
	std::vector<WallNormalHole*> ListOfWallNormalHole;//壁面普通通孔	
	std::vector<WallCircleCombineHole*>ListOfWallCircleCombineHole;//壁面复合通孔	
	std::vector<RibCombineHole*>ListOfRibCombineHole;//筋面复合通孔		
	std::vector<BossNormalHole*>ListOfBossNormalHole;//凸台无倒角通孔
	std::vector<BossOneRightHole*> ListOfBossOneRightHole;//凸台单侧斜角通孔
	std::vector<BossOneCircleHole*> ListOfBossOneCircleHole;//凸台单侧圆角通孔
	std::vector<BossDoubleRightHole*> ListOfBossDoubleRightHole; //凸台双侧斜角通孔
	std::vector<BossDoubleCircleHole*> ListOfBossDoubleCircleHole;//凸台双侧圆角通孔
	std::vector<BossOneRightOneCircleHole*> ListOfBossOneRightOneCircleHole;//凸台单侧斜角单侧圆角通孔
	std::vector<BossManyLayerHole*> ListOfBossManyLayerHole;//凸台多层通孔
	std::vector<BossOutsideWideCombineHole*> ListOfBossOutsideWideCombineHole;//凸台外宽内窄复合通孔
	std::vector<BossInsideWideCombineHole*> ListOfBossInsideWideCombineHole;//凸台内宽外窄复合通孔
	std::vector<BossMidCricleCombineHole*> ListOfBossMidCricleCombineHole;//凸台中圆角复合通孔
	std::vector<BossNormalBlindHole*> ListOfBossNormalBlindHole;//凸台无倒角盲孔
	std::vector<BossRightBlindHole*> ListOfBossRightBlindHole;//凸台倒斜角盲孔
	std::vector<Other3Hole*>ListOfBossOtherHole;//凸台其它孔

};

//环形槽的层
struct AnnulusCavityLayer
{
	std::vector<NXOpen::Face*> BottomFaceVector;
	std::vector<NXOpen::Face*> BottomCornerFaceVector;
	std::vector<NXOpen::Face*> SideFaceVector;
	std::vector<NXOpen::Face*> TopFaceVector;
};
//
//环形槽
struct AnnulusCavity
{
	std::vector<AnnulusCavityLayer*> AnnulusCavityLayerVector;
};

//====================================轮廓特征也暂时保留，付看情况删========================

struct Outline
{
	std::vector<NXOpen::Face*> PlanerFaceVector;
	std::vector<NXOpen::Face*> ConicalFaceVector;
	std::vector<NXOpen::Face*> TopFaceVector;
	std::vector<NXOpen::Face*> RightFaceVector;
	std::vector<NXOpen::Face*> SideFaceVector;
	std::vector<NXOpen::Face*> BottomCornerFaceVector;
};



//struct AnnulusCavity
//{
//	std::vector<AnnulusCavityLayer*> AnnulusCavityLayerVector;
//};

//环形槽的层
//struct AnnulusCavityLayer
//{
//	std::vector<NXOpen::Face*> BottomFaceVector;
//	std::vector<NXOpen::Face*> BottomCornerFaceVector;
//	std::vector<NXOpen::Face*> SideFaceVector;
//	std::vector<NXOpen::Face*> TopFaceVector;
//};
////
////环形槽
//struct AnnulusCavity
//{
//	std::vector<AnnulusCavityLayer*> AnnulusCavityLayerVector;
//};
////

//====================================20240102-fxy:新增刀具数据库结构体========================
//Milling：铣刀1	Drill：钻刀2	PointDrill：点钻刀3	 ThreadMilling：螺纹铣刀4	Reamer：铰刀5	Boring：镗刀6
//凸台1 筋2 孔3 槽4 
//GH4169：1 GH4169D：2 GH141：3
//粗加工：1 半精加工：2 精加工：3
struct Cuttingdata
{
	int CutterType;
	int CuttingFeature;
	int CuttingMaterial;
	int ProcessingStage;

	float ToolDiameter;    //刀具直径
	float ToolCorner;      //刀具圆角半径
	float ToolLength;      //刀具长度
	float ToolFluteLength; //刀具刃长

	float Tolerance;       //公差
	float PartAllowance;   //部件余量
	
	float Machining;       //进给速度
	float Approach;        //进刀速度
	float Retract;         //退刀速度
	float SpindleSpeed;    //主轴转速

	bool CutterTypeLable = false;		//是否被选中
	bool CuttingFeatureLable = false;		//是否被选中
	bool CuttingMaterialLable = false;		//是否被选中
	bool ProcessingStageLable = false;		//是否被选中
	bool ifselect = false;
};
#endif
