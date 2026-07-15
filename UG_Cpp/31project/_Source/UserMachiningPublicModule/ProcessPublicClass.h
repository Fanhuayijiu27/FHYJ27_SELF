#undef CreateDialog
#ifndef PROCESSPUBLICCLASS_H
#define PROCESSPUBLICCLASS_H

#include <NXOpen/NXString.hxx>

using namespace NXOpen;

////系统配置参数
//NXString SelectionPathNXString;  //直接选择的路径
//bool IsPartPath;         //是否与部件所处路径相同

//特征识别参数
struct NormalParameter
{
	NXOpen::Vector3d _Normal;  //特征识别得到的零件旋转矢量
};




//刀具参数
struct ToolParameter
{
	NXString ToolName;        //刀具名称
	double   ToolDiameter;    //刀具直径
	double   ToolCorner;      //刀具圆角半径
	double   ToolLength;      //刀具长度
	double   ToolFluteLength; //刀具刃长
};

//面铣进退刀参数
struct FaceMillApproachAndRetract
{
	int ToolMotionType;       //进退刀方式

	double ToolMotionAngle;   //倾斜角度
	double ToolMotionHeight;  //高度
	int ToolMotionHeightUnit; //高度单位
	int HeightFrom;           //高度自

	double HelicDiameter;     //螺旋方式直径
	int HelicDiameterUnit; //螺旋方式直径单位

	int WidthMode;            //宽度模式
	double Distance;          //指定宽度时的距离
	int DistanceUnit;      //指定宽度时的距离单位
};

//轮廓铣进退刀参数
struct ContourMillApproachAndRetract
{
	int ToolMotionType;                //进退刀方式

	double ToolMotionDistance;         //进刀距离
	int    ToolMotionDistanceUnit;     //进刀距离单位
	double CircleRadius;               //圆弧半径
	int    CircleRadiusUnit;           //圆弧半径单位

	double RotationAngle;           //旋转角度
	double TiltAngle;               //倾斜角度
	double CircleAngle;             //圆弧角度

	double LinearExtension;            //线性延伸
	int    LinearExtensionUnit;        //线性延伸单位
};

//凸台顶面加工参数
struct BossTopProcessParameter
{
	//主法向
	NXOpen::Vector3d Approch_Normal ;

	//选择
	NXString PROGRAM;         //Program
	NXString GeomtryMCS;      //MCS
	NXString METHOD;
	ToolParameter ToolMsg;    //刀具

	bool Finishing;          //精加工
	bool LCsys;            //创建局部坐标系
	bool NotLCsys;            //不创建局部坐标系

	//切削策略
	bool   CurveDrive;           //曲线驱动开关
	bool   FaceMill;             //面铣开关
	bool   FaceSelect;      //选面开关
	bool   ONEboss;
	bool   MOREboss;
	int    CuttingDirection;     //切削方向-----------顺铣和逆铣
	int    CuttingMode;          //切削模式
	bool   StepDisFlag;          //步距开关
	int    StepDisUnit;          //步距方法的单位
	double StepDisAmount;        //步距量--------
	bool   ScallopHeightFlag;           //残余高度
	double ScallopHeightAmount;         //残余高度量
	double CuttingStepdistance;       //切削步长
	int    CuttingStepdistanceunit;      //切削步长单位

	int    syspart;

	//切削参数
	double Tolerance;            //公差
	double PartAllowance;        //部件余量
	int    SpindleSpeed;         //主轴转速
	double Machining;            //进给速度
	double Approach;             //进刀速度
	double Retract;              //退刀速度

	//非切削移动
	ContourMillApproachAndRetract ToolMotion;
	int  SafeSetType;             //安全设置选项------------平面或者包容圆柱体
	double SafeSetOffset;         //安全平面的偏置量
};

//凸台侧面加工参数
struct BossSideProcessParameter
{
	//主法向
	NXOpen::Vector3d Approch_Normal;

	//选择
	NXString PROGRAM;         //Program
	NXString GeomtryMCS;      //MCS
	NXString METHOD;
	ToolParameter ToolMsg;    //刀具

	bool FaceSelect;
	bool   ONEboss;
	bool   MOREboss;

	int    syspart;

	bool Finishing;         //精加工
	bool LCsys;            //创建局部坐标系
	bool NotLCsys;            //不创建局部坐标系

	bool PointPart;         //指定部件

	//切削策略
	int    CuttingDirection;     //切削方向-----------顺铣和逆铣
	int    CuttingMode;          //切削模式
	double BoundaryOffset;       //边界偏置
	bool   MultiDepthFlag;              //多重深度开关
	int    MultiDepthStepUnit;          //步进方法的单位
	double MultiDepthStepAmount;        //步进量----------刀路数目或者单步增量值
	bool   MultiPathFlag;               //多刀路、附加刀路
	int    MultiPathNum;                //附加刀路的个数
	int    MultiPathStepUnit;           //附加刀路步距单位
	double MultiPathStepAmount;         //附加刀路步距量

	double CuttingStepdistance;       //切削步长
	int    CuttingStepdistanceunit;      //切削步长单位

	//切削参数
	double Tolerance;            //公差
	double PartAllowance;        //部件余量
	double PartSafetyDistance;   //部件安全距离
	int SpindleSpeed;            //主轴转速
	double Machining;            //进给速度
	double Approach;             //进刀速度
	double Retract;              //退刀速度

	//非切削移动
	ContourMillApproachAndRetract ToolMotion;
	int  SafeSetType;             //安全设置选项------------平面或者包容圆柱体
	double SafeSetOffset;         //安全平面的偏置量
};

//环形槽加工参数
struct AnnulusCavityProcessParameter
{
	//主法向
	NXOpen::Vector3d Approch_Normal ;

	//选择
	NXString PROGRAM;         //Program
	NXString GeomtryMCS;      //MCS
	NXString METHOD;          //method
	ToolParameter ToolMsg;    //刀具

	bool FaceSelect ;         //面选开关
	bool  IfLCsys;		//局部坐标系

	//切削策略
	bool   curveDriveFlag;       //曲线驱动
	bool   surfaceDriveFlag;     //曲面驱动
	int    CuttingDirection;     //切削方向-----------顺铣和逆铣
	int    CuttingMode;          //切削模式
	bool   StepDisFlag;          //步距开关
	double StepDisAmount;        //步距量--------
	bool   ScallopHeightFlag;           //残余高度
	double ScallopHeightAmount;         //残余高度量

	//切削参数
	double Tolerance;            //公差
	double PartAllowance;        //部件余量
	int    SpindleSpeed;         //主轴转速
	double Machining;            //进给速度
	double Approach;             //进刀速度
	double Retract;              //退刀速度

	//非切削移动
	ContourMillApproachAndRetract ToolMotion;
	int  SafeSetType;             //安全设置选项------------平面或者包容圆柱体
	double SafeSetOffset;         //安全平面的偏置量
};

//下陷槽加工参数
struct SubsidenceCavityProcessParameter
{
	//主法向
	NXOpen::Vector3d Approch_Normal ;

	//选择
	NXString PROGRAM;         //Program
	NXString GeomtryMCS;      //MCS
	NXString METHOD;          //method
	ToolParameter ToolMsg;    //刀具

	bool FaceSelected;        //单独选面

	//切削策略
	int    CuttingDirection;     //切削方向-----------顺铣和逆铣
	int    CuttingMode;          //切削模式
	bool   StepDisFlag;          //步距开关
	int    StepDisUnit;          //步距方法的单位
	double StepDisAmount;        //步距量--------
	bool   ScallopHeightFlag;           //残余高度
	double ScallopHeightAmount;         //残余高度量
	int    StepUsedOnP;                 //步距应用于
	int    CuttingAngle;                //切削角
	double XcAngle;                     //从xc的角度

	//切削参数
	double Tolerance;            //公差
	double PartAllowance;        //部件余量
	int    SpindleSpeed;         //主轴转速
	double Machining;            //进给速度
	double Approach;             //进刀速度
	double Retract;              //退刀速度

	//非切削移动
	ContourMillApproachAndRetract ToolMotion;
	int  SafeSetType;             //安全设置选项------------平面或者包容圆柱体
	double SafeSetOffset;         //安全平面的偏置量
};

//弧环形槽加工参数
struct ArcCavityProcessParameter
{
	//单位主法向
	NXOpen::Vector3d Approch_Normal;

	//选择
	NXString PROGRAM;         //Program
	NXString GeomtryMCS;      //MCS
	NXString METHOD;          //method
	ToolParameter ToolMsg;    //刀具

	bool FaceSelected;

	//切削策略
	bool   curveDriveFlag;       //曲线驱动
	bool   surfaceDriveFlag;     //曲面驱动
	int    CuttingDirection;     //切削方向-----------顺铣和逆铣
	int    CuttingMode;          //切削模式
	bool   StepDisFlag;          //步距开关
	double StepDisAmount;        //步距量--------
	bool   ScallopHeightFlag;           //残余高度
	double ScallopHeightAmount;         //残余高度量

	//切削参数
	double Tolerance;            //公差
	double PartAllowance;        //部件余量
	int    SpindleSpeed;         //主轴转速
	double Machining;            //进给速度
	double Approach;             //进刀速度
	double Retract;              //退刀速度

	//非切削移动
	ContourMillApproachAndRetract ToolMotion;
	int  SafeSetType;             //安全设置选项------------平面或者包容圆柱体
	double SafeSetOffset;         //安全平面的偏置量
};

//筋顶面加工参数
struct RibTopProcessParameter
{
    //单位主法向
	NXOpen::Vector3d Approch_Normal;

	//选择
	NXString PROGRAM;         //Program
	NXString GeomtryMCS;      //MCS
	NXString METHOD;          //method
	ToolParameter ToolMsg;    //刀具

	bool FaceSelect;

	//切削策略
	bool   CurveDrive;           //曲线驱动开关
	bool   FaceMill;             //区域铣削开关
	int    CuttingDirection;     //切削方向-----------顺铣和逆铣
	int    CuttingMode;          //切削模式
	bool   StepDisFlag;          //步距开关
	int    StepDisUnit;          //步距方法的单位
	double StepDisAmount;        //步距量--------
	bool   ScallopHeightFlag;           //残余高度
	double ScallopHeightAmount;         //残余高度量
	int    StepUsedOnP;                 //步距应用于
	int    CuttingAngle;                //切削角
	double XcAngle;                     //从xc的角度

	//切削参数
	double Tolerance;            //公差
	double PartAllowance;        //部件余量
	int    SpindleSpeed;         //主轴转速
	double Machining;            //进给速度
	double Approach;             //进刀速度
	double Retract;              //退刀速度

	//非切削移动
	ContourMillApproachAndRetract ToolMotion;
	int  SafeSetType;             //安全设置选项------------平面或者包容圆柱体
	double SafeSetOffset;         //安全平面的偏置量
};

//插铣粗加工参数
struct RoughPlungeProcessParameter
{
	NXOpen::Vector3d Approch_Normal;

	//选择
	NXString PROGRAM;         //Program
	NXString GeomtryMCS;      //MCS
	NXString METHOD;          //method
	ToolParameter ToolMsg;    //刀具

	bool FaceSelect;
	bool  IfLCsys;

	//切削策略
	bool   AnnulusCavityFlag;
	bool   CircularGrooveFlag;
	bool   ArcCavityFlag;        
	bool   SubsidenceCavityFlag; 
	bool   RibTopFlag;           
	bool   AreaPlungingFlag ;
	double MaxAngle ;
	int    CuttingDirection;     //切削方向-----------顺铣和逆铣
	int    CuttingMode;          //切削模式
	int    StepDisType;          //步距类型
	double StepDisAmount;        //步距量
	int    ForwardStepType;      //向前步长类型
	double ForwardStepAmount;    //向前步长量
	int    SingleStepType;       //单步向上类型
	double SingleStepAmount;     //单步向上量
	int    MaxCutDisType;        //最大切削宽度类型
	double MaxCutDisAmount;      //最大切削宽度量

	//切削参数
	double Tolerance;            //公差
	double PartAllowance;        //部件余量
	int    SpindleSpeed;         //主轴转速
	double Machining;            //进给速度
	double Approach;             //进刀速度
	double Retract;              //退刀速度

	//非切削移动
	double RetracttingDis;           //退刀距离
	double RetracttingAngle;         //退刀角
	//ContourMillApproachAndRetract ToolMotion;
	//int  SafeSetType;             //安全设置选项------------平面或者包容圆柱体
	//double SafeSetOffset;         //安全平面的偏置量
};

//孔钻削加工参数
struct HoleProcessParameter
{
	//主法向
	NXOpen::Vector3d Approch_Normal ;

	//选择
	NXString PROGRAM;         //Program
	NXString GeomtryMCS;      //MCS

	bool IfLCsys;            //局部坐标系
	bool IfACsys;            //全局坐标系

	bool FaceSelect;
	int NumofDMHoleOperation; //端面数

	bool SpotDrill;               //中心钻
	bool Drill;                   //钻
	bool SpotFacing;              //锪
	bool Boring;                  //镗
	bool Reaming;                 //铰
	bool HoleMilling;             //孔铣
	bool THreadMilling;           //螺纹铣

	//#####################中心钻
	NXString METHOD;
	ToolParameter ToolMsg;    //刀具
	//切削策略
	int  Cycling;					//循环
	int  MinSafetyDistance ;       //最小安全距离 
	int  ThroHoleSafetyDistance ;  //通孔安全距离
	int  BlindHoleAllowance ;      //盲孔余量

	//偏置
	double MTopOffset;             //顶偏置
	double MRaptoOffset;           //Rapto偏置
	double MBottomOffset;          //底偏置

	//切削参数
	int SpindleSpeed;            //主轴转速
	double Machining;            //进给速度
	double Approach;             //进刀速度
	double Retract;              //退刀速度

	//##################################钻孔
	NXString DMETHOD;
	ToolParameter DToolMsg;    //刀具
	//切削参数
	int DMSpindleSpeed;            //主轴转速
	double DMMachining;            //进给速度
	//非切削移动
	int DCycling;                 //循环
	double DMMinSafeDistance;           //最小安全距离
	int  DThroHoleSafetyDistance;  //通孔安全距离
	int  DBlindHoleAllowance;      //盲孔余量

	//偏置
	double DTopOffset;             //顶偏置
	double DRaptoOffset;           //Rapto偏置
	double DBottomOffset;          //底偏置

	//##################################铰孔
	NXString RMETHOD;
	ToolParameter RToolMsg;    //刀具
	//切削参数
	int RMSpindleSpeed;            //主轴转速
	double RMMachining;            //进给速度
	//非切削移动
	int RMCycling;                 //循环
	double RMMinSafeDistance;           //最小安全距离
	int  RThroHoleSafetyDistance;  //通孔安全距离
	int  RBlindHoleAllowance;      //盲孔余量

	//偏置
	double RMTopOffset;             //顶偏置
	double RMRaptoOffset;           //Rapto偏置
	double RMBottomOffset;          //底偏置

	//##################################镗孔
	NXString BMETHOD;
	ToolParameter BToolMsg;    //刀具
	//切削参数
	int BMSpindleSpeed;            //主轴转速
	double BMMachining;            //进给速度
	//非切削移动
	int BMCycling;                 //循环
	double BMMinSafeDistance;           //最小安全距离
	int  BThroHoleSafetyDistance;  //通孔安全距离
	int  BBlindHoleAllowance;      //盲孔余量

	//偏置
	double BMTopOffset;             //顶偏置
	double BMRaptoOffset;           //Rapto偏置
	double BMBottomOffset;          //底偏

	//##################################锪孔
	NXString SFMETHOD;
	ToolParameter SFToolMsg;    //刀具
	//切削参数
	int SFMSpindleSpeed;            //主轴转速
	double SFMMachining;            //进给速度
	//非切削移动
	int SFCycling;                  //循环类型
	double SFMinSafeDistance;           //最小安全距离

	//偏置
	double SFTopOffset;             //顶偏置
	double SFRaptoOffset;           //Rapto偏置
	double SFBottomOffset;          //底偏置

	//################################孔铣HM
	NXString HMMETHOD;
	ToolParameter HMToolMsg;    //刀具
	//切削策略
	int  HMCuttingType;					//切削模式
	int  HMAxisPitchometer;       //轴向螺距
	int  HMAPIntent;               //轴向螺距单位
	int  HMSlipMaxDistance;      //切向最大距离
	int  HMSMDIntent;            //径向最大距离单位
	int  HMNumOfCuttingStep;      //刀路数
	//切削参数
	double HMTolerance;            //公差
	double HMTolerance1;            //内公差
	double HMTolerance2;            //外公差
	double HMPartAllowance;        //部件余量
	int HMSpindleSpeed;            //主轴转速
	double HMMachining;            //进给速度
	//double HMApproach;             //进刀速度
	//double HMRetract;              //退刀速度
	//非切削移动
	int  HMNoCuttingType;				//进退刀类型
	double HMMinSafeDistance;           //最小安全距离

	//##################################螺纹铣THM
	NXString THMMETHOD;
	ToolParameter THMToolMsg;    //刀具
	//切削策略
	int  THMAxisStepDistance;       //轴向步距百分比
	int  THMSlipMaxDistance;      //切向最大距离
	int  THMNumOfCuttingStep;      //刀路数
	//切削参数
	double THMTolerance1;            //内公差
	double THMTolerance2;            //外公差
	double THMPartAllowance;        //部件余量
	int THMSpindleSpeed;            //主轴转速
	double THMMachining;            //进给速度
	//非切削移动
	int  THMNoCuttingType;				//进退刀类型
	double THMMinSafeDistance;           //最小安全距离
};

struct ResidueFeatureProcessParameter 
{
	//主法向
	NXOpen::Vector3d Approch_Normal ;

	//选择
	NXString PROGRAM;         //Program
	NXString GeomtryMCS;      //MCS
	NXString METHOD;
	ToolParameter ToolMsg;    //刀具

	//切削策略
	int    CuttingDirection;     //切削方向-----------顺铣和逆铣
	int    CuttingMode;          //切削模式
	double BoundaryOffset;       //边界偏置
	bool   MultiDepthFlag;              //多重深度开关
	int    MultiDepthStepUnit;          //步进方法的单位
	double MultiDepthStepAmount;        //步进量----------刀路数目或者单步增量值
	bool   MultiPathFlag;               //多刀路、附加刀路
	int    MultiPathNum;                //附加刀路的个数
	int    MultiPathStepUnit;           //附加刀路步距单位
	double MultiPathStepAmount;         //附加刀路步距量

	//切削参数
	double Tolerance;            //公差
	double PartAllowance;        //部件余量
	double PartSafetyDistance;   //部件安全距离
	int SpindleSpeed;            //主轴转速
	double Machining;            //进给速度
	double Approach;             //进刀速度
	double Retract;              //退刀速度

	//非切削移动
	ContourMillApproachAndRetract ToolMotion;
	int  SafeSetType;             //安全设置选项------------平面或者包容圆柱体
	double SafeSetOffset;         //安全平面的偏置量
};

struct PlanerBlindGrooveProcessParameter
{
	//主法向
	NXOpen::Vector3d Approch_Normal;

	//选择
	NXString PROGRAM;         //Program
	NXString GeomtryMCS;      //MCS
	NXString METHOD;
	ToolParameter ToolMsg;    //刀具

	bool LCsys;            //创建局部坐标系
	bool NotLCsys;            //不创建局部坐标系

	int    syspart;
	//切削策略
	bool   CurveDrive;           //曲线驱动开关
	bool   FaceMill;             //面铣开关
	bool   FaceSelect;      //选面开关
	bool   AllFace;
	int    CuttingDirection;     //切削方向-----------顺铣和逆铣
	int    CuttingMode;          //切削模式
	bool   StepDisFlag;          //步距开关
	int    StepDisUnit;          //步距方法的单位
	double StepDisAmount;        //步距量--------
	bool   ScallopHeightFlag;           //残余高度
	double ScallopHeightAmount;         //残余高度量
	double depth;                 //切削层每刀切削深度

	//切削参数
	double Tolerance;            //公差
	double PartAllowance;        //部件余量
	int    SpindleSpeed;         //主轴转速
	double Machining;            //进给速度
	double Approach;             //进刀速度
	double Retract;              //退刀速度

	//非切削移动
	ContourMillApproachAndRetract ToolMotion;
	int  SafeSetType;             //安全设置选项------------平面或者包容圆柱体
	double SafeSetOffset;         //安全平面的偏置量
};

struct PlanerBlindGrooveSideProcessParameter
{
	//主法向
	NXOpen::Vector3d Approch_Normal;

	//选择
	NXString PROGRAM;         //Program
	NXString GeomtryMCS;      //MCS
	NXString METHOD;
	ToolParameter ToolMsg;    //刀具

	bool FaceSelect;
	bool IfLCsys;            //局部坐标系

	//切削策略
	int    CuttingDirection;     //切削方向-----------顺铣和逆铣
	int    CuttingMode;          //切削模式
	double BoundaryOffset;       //边界偏置
	bool   MultiDepthFlag;              //多重深度开关
	int    MultiDepthStepUnit;          //步进方法的单位
	double MultiDepthStepAmount;        //步进量----------刀路数目或者单步增量值
	bool   MultiPathFlag;               //多刀路、附加刀路
	int    MultiPathNum;                //附加刀路的个数
	int    MultiPathStepUnit;           //附加刀路步距单位
	double MultiPathStepAmount;         //附加刀路步距量

	//切削参数
	double Tolerance;            //公差
	double PartAllowance;        //部件余量
	double PartSafetyDistance;   //部件安全距离
	int SpindleSpeed;            //主轴转速
	double Machining;            //进给速度
	double Approach;             //进刀速度
	double Retract;              //退刀速度

	//非切削移动
	ContourMillApproachAndRetract ToolMotion;
	int  SafeSetType;             //安全设置选项------------平面或者包容圆柱体
	double SafeSetOffset;         //安全平面的偏置量
};

struct PassGrooveSideProcessParameter
{
	//主法向
	NXOpen::Vector3d Approch_Normal;

	//选择
	NXString PROGRAM;         //Program
	NXString GeomtryMCS;      //MCS
	NXString METHOD;
	ToolParameter ToolMsg;    //刀具

	bool AllGroove;
	bool FaceSelect;
	bool LCsys;            //局部坐标系
	bool NotLCsys;            //不创建局部坐标系

	int syspart;

	//切削策略
	int    CuttingDirection;     //切削方向-----------顺铣和逆铣
	int    CuttingMode;          //切削模式
	double BoundaryOffset;       //边界偏置
	bool   MultiDepthFlag;              //多重深度开关
	int    MultiDepthStepUnit;          //步进方法的单位
	double MultiDepthStepAmount;        //步进量----------刀路数目或者单步增量值
	bool   MultiPathFlag;               //多刀路、附加刀路
	int    MultiPathNum;                //附加刀路的个数
	int    MultiPathStepUnit;           //附加刀路步距单位
	double MultiPathStepAmount;         //附加刀路步距量
	double StepDistance;          //步距
	int    StepDistanceunit;      //步距单位
	double depth;                 //切削层每刀切削深度

	//切削参数
	double Tolerance;            //公差
	double PartAllowance;        //部件余量
	double PartSafetyDistance;   //部件安全距离
	int SpindleSpeed;            //主轴转速
	double Machining;            //进给速度
	double Approach;             //进刀速度
	double Retract;              //退刀速度

	//非切削移动
	ContourMillApproachAndRetract ToolMotion;
	int  SafeSetType;             //安全设置选项------------平面或者包容圆柱体
	double SafeSetOffset;         //安全平面的偏置量
};
#endif