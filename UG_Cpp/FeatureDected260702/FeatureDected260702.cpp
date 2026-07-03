//==============================================================================
//  FeatureDected260702.cpp
//  机匣特征识别对话框 — 实现文件
//
//  严格遵循 FBM_Win32_NX11_NUAA_finalversion 项目的代码实现：
//  - 工具函数来自 UserMachiningPublicModule/PublicFunctions.cpp
//  - 特征识别算法来自 FeatureSpy/SpyFunctions.cpp
//  - 特征数据结构来自 UserMachiningPublicModule/FeaturePublicClass.h
//
//  功能：
//    1. 凸台识别 (BossRecognitionAndCreate)
//    2. 槽识别   (CircularGroove / SubsidenceGroove / PlanerBlindGroove / PassGroove)
//    3. 孔识别   (DirectionMill: 普通通孔 / 倒角通孔 / 盲孔)
//    4. 结果高亮显示、外部文件输出
//
//  NX Block Styler 生成，版本 NX 12
//==============================================================================

#include "FeatureDected260702.hpp"
using namespace NXOpen;
using namespace NXOpen::BlockStyler;

//------------------------------------------------------------------------------
// Initialize static variables
//------------------------------------------------------------------------------
Session *(FeatureDected260702::theSession) = NULL;
UI *(FeatureDected260702::theUI) = NULL;

//------------------------------------------------------------------------------
// Constructor
//------------------------------------------------------------------------------
FeatureDected260702::FeatureDected260702()
{
    try
    {
        FeatureDected260702::theSession = NXOpen::Session::GetSession();
        FeatureDected260702::theUI = UI::GetUI();
        theDlxFileName = "FeatureDected260702.dlx";
        theDialog = FeatureDected260702::theUI->CreateDialog(theDlxFileName);

        // 注册回调函数
        theDialog->AddApplyHandler(make_callback(this, &FeatureDected260702::apply_cb));
        theDialog->AddOkHandler(make_callback(this, &FeatureDected260702::ok_cb));
        theDialog->AddUpdateHandler(make_callback(this, &FeatureDected260702::update_cb));
        theDialog->AddInitializeHandler(make_callback(this, &FeatureDected260702::initialize_cb));
        theDialog->AddDialogShownHandler(make_callback(this, &FeatureDected260702::dialogShown_cb));

        // 初始化成员变量
        pSelectedBody = NULL;
        pSelectedFace = NULL;
        _Approach_Normal.X = 0; _Approach_Normal.Y = 0; _Approach_Normal.Z = 1;
        TopPlaneFace = NULL;
        BottomPlaneFace = NULL;
        hasRecognized = false;
    }
    catch(exception& ex)
    {
        throw;
    }
}

//------------------------------------------------------------------------------
// Destructor
//------------------------------------------------------------------------------
FeatureDected260702::~FeatureDected260702()
{
    // 释放凸台特征内存
    for (size_t i = 0; i < Feature_NormalBoss.size(); i++) delete Feature_NormalBoss[i];
    for (size_t i = 0; i < Feature_NoSideBoss.size(); i++) delete Feature_NoSideBoss[i];
    for (size_t i = 0; i < Feature_DoubleLayerBoss.size(); i++) delete Feature_DoubleLayerBoss[i];

    // 释放槽特征内存
    for (size_t i = 0; i < Feature_CircularGroove0.size(); i++) delete Feature_CircularGroove0[i];
    for (size_t i = 0; i < Feature_CircularGroove1.size(); i++) delete Feature_CircularGroove1[i];
    for (size_t i = 0; i < Feature_SubsidenceGroove.size(); i++) delete Feature_SubsidenceGroove[i];
    for (size_t i = 0; i < Feature_PlanerBlindGroove.size(); i++) delete Feature_PlanerBlindGroove[i];
    for (size_t i = 0; i < Feature_PassGroove.size(); i++) delete Feature_PassGroove[i];

    // 释放孔特征内存
    for (size_t i = 0; i < Feature_DirectionNormalHole.size(); i++) delete Feature_DirectionNormalHole[i];
    for (size_t i = 0; i < Feature_DirectionRightHole.size(); i++) delete Feature_DirectionRightHole[i];
    for (size_t i = 0; i < Feature_DirectionBlindHole.size(); i++) delete Feature_DirectionBlindHole[i];

    if (theDialog != NULL)
    {
        delete theDialog;
        theDialog = NULL;
    }
}

//=============================================================================
// NX Open 入口函数
//=============================================================================
extern "C" DllExport void ufusr(char *param, int *retcod, int param_len)
{
    FeatureDected260702 *theFeatureDected260702 = NULL;
    try
    {
        theFeatureDected260702 = new FeatureDected260702();
        theFeatureDected260702->Show();
    }
    catch(exception& ex)
    {
        FeatureDected260702::theUI->NXMessageBox()->Show("Block Styler",
            NXOpen::NXMessageBox::DialogTypeError, ex.what());
    }
    if(theFeatureDected260702 != NULL)
    {
        delete theFeatureDected260702;
        theFeatureDected260702 = NULL;
    }
}

extern "C" DllExport int ufusr_ask_unload()
{
    return (int)Session::LibraryUnloadOptionImmediately;
}

extern "C" DllExport void ufusr_cleanup(void)
{
    try { }
    catch(exception& ex)
    {
        FeatureDected260702::theUI->NXMessageBox()->Show("Block Styler",
            NXOpen::NXMessageBox::DialogTypeError, ex.what());
    }
}

//------------------------------------------------------------------------------
// Show
//------------------------------------------------------------------------------
int FeatureDected260702::Show()
{
    try { theDialog->Show(); }
    catch(exception& ex)
    {
        FeatureDected260702::theUI->NXMessageBox()->Show("Block Styler",
            NXOpen::NXMessageBox::DialogTypeError, ex.what());
    }
    return 0;
}

//=============================================================================
// Block UI Styler Callback Functions
//=============================================================================

//------------------------------------------------------------------------------
// initialize_cb
//------------------------------------------------------------------------------
void FeatureDected260702::initialize_cb()
{
    try
    {
        group = dynamic_cast<NXOpen::BlockStyler::Group*>(theDialog->TopBlock()->FindBlock("group"));
        selection0 = dynamic_cast<NXOpen::BlockStyler::SelectObject*>(theDialog->TopBlock()->FindBlock("selection0"));
        selection01 = dynamic_cast<NXOpen::BlockStyler::SelectObject*>(theDialog->TopBlock()->FindBlock("selection01"));
        group0 = dynamic_cast<NXOpen::BlockStyler::Group*>(theDialog->TopBlock()->FindBlock("group0"));
        enum0 = dynamic_cast<NXOpen::BlockStyler::Enumeration*>(theDialog->TopBlock()->FindBlock("enum0"));
        button0 = dynamic_cast<NXOpen::BlockStyler::Button*>(theDialog->TopBlock()->FindBlock("button0"));
        group1 = dynamic_cast<NXOpen::BlockStyler::Group*>(theDialog->TopBlock()->FindBlock("group1"));
        multiline_string0 = dynamic_cast<NXOpen::BlockStyler::MultilineString*>(theDialog->TopBlock()->FindBlock("multiline_string0"));
        button01 = dynamic_cast<NXOpen::BlockStyler::Button*>(theDialog->TopBlock()->FindBlock("button01"));
        group2 = dynamic_cast<NXOpen::BlockStyler::Group*>(theDialog->TopBlock()->FindBlock("group2"));
        enum01 = dynamic_cast<NXOpen::BlockStyler::Enumeration*>(theDialog->TopBlock()->FindBlock("enum01"));
        button02 = dynamic_cast<NXOpen::BlockStyler::Button*>(theDialog->TopBlock()->FindBlock("button02"));
    }
    catch(exception& ex)
    {
        FeatureDected260702::theUI->NXMessageBox()->Show("Block Styler",
            NXOpen::NXMessageBox::DialogTypeError, ex.what());
    }
}

//------------------------------------------------------------------------------
// dialogShown_cb
//------------------------------------------------------------------------------
void FeatureDected260702::dialogShown_cb()
{
    try { }
    catch(exception& ex)
    {
        FeatureDected260702::theUI->NXMessageBox()->Show("Block Styler",
            NXOpen::NXMessageBox::DialogTypeError, ex.what());
    }
}

//------------------------------------------------------------------------------
// apply_cb
//------------------------------------------------------------------------------
int FeatureDected260702::apply_cb()
{
    int errorCode = 0;
    try { }
    catch(exception& ex)
    {
        errorCode = 1;
        FeatureDected260702::theUI->NXMessageBox()->Show("Block Styler",
            NXOpen::NXMessageBox::DialogTypeError, ex.what());
    }
    return errorCode;
}

//------------------------------------------------------------------------------
// update_cb
// 所有按钮的功能实现
//------------------------------------------------------------------------------
int FeatureDected260702::update_cb(NXOpen::BlockStyler::UIBlock* block)
{
    try
    {
        //=====================================================================
        // selection0: 选择零件实体模型
        //=====================================================================
        if(block == selection0)
        {
            PropertyList* selection0Props = selection0->GetProperties();
            std::vector<NXOpen::TaggedObject*> pTaggedObject_BodyList =
                selection0Props->GetTaggedObjectVector("SelectedObjects");
            delete selection0Props;

            int num_BodyList = (int)pTaggedObject_BodyList.size();
            if (num_BodyList > 0)
            {
                pSelectedBody = dynamic_cast<NXOpen::Body*>(pTaggedObject_BodyList[0]);
                AllFaces.clear();
                AllFaces = pSelectedBody->GetFaces();
            }
        }

        //=====================================================================
        // selection01: 选择分型面
        //=====================================================================
        else if(block == selection01)
        {
            PropertyList* selection01Props = selection01->GetProperties();
            std::vector<NXOpen::TaggedObject*> pTaggedObject_FaceList =
                selection01Props->GetTaggedObjectVector("SelectedObjects");
            delete selection01Props;

            int num_FaceList = (int)pTaggedObject_FaceList.size();
            if (num_FaceList == 1)
            {
                pSelectedFace = dynamic_cast<NXOpen::Face*>(pTaggedObject_FaceList[0]);
                get_MainNormal_fr_plane(pSelectedFace, _Approach_Normal);

                // 锁定选择控件，转到下一步
                PropertyList* faceProps = selection01->GetProperties();
                faceProps->SetLogical("Enable", FALSE);
                delete faceProps;
                PropertyList* bodyProps = selection0->GetProperties();
                bodyProps->SetLogical("Enable", FALSE);
                delete bodyProps;
            }
            else
            {
                uc1601("请选择一个分型面", 1);
            }
        }

        //=====================================================================
        // enum0: 识别类型切换（可选：根据选择锁定/解锁按钮）
        //=====================================================================
        else if(block == enum0)
        {
            // 可在切换枚举时更新界面状态
        }

        //=====================================================================
        // button0: 开始特征识别
        //=====================================================================
        else if(block == button0)
        {
            if (pSelectedBody == NULL)
            {
                uc1601("请先选择零件实体模型!", 1);
                return 0;
            }
            if (pSelectedFace == NULL)
            {
                uc1601("请先选择分型面!", 1);
                return 0;
            }

            // 获取选择类型
            PropertyList* enumProps = enum0->GetProperties();
            NXString nxStrEnum = enumProps->GetEnumAsString("Value");
            delete enumProps;
            string strEnum = nxStrEnum.GetLocaleText();

            // 清除之前的识别结果
            ClearAllFeatures();
            UnhighlightAll();

            // 分析平面：确定顶面/底面/侧面
            AnalyzePlanes(pSelectedFace, TopPlaneFace, BottomPlaneFace, ParallelPlaneFace);

            if (TopPlaneFace == NULL || BottomPlaneFace == NULL)
            {
                uc1601("未能正确识别顶面和底面，请检查分型面选择!", 1);
                return 0;
            }

            // 获取工作部件
            Part* workPart = NXOpen::Session::GetSession()->Parts()->Work();

            if (strEnum == "全部特征" || strEnum == "凸台类特征")
            {
                // ---- 凸台识别 ----
                std::vector<NXOpen::Face*> SealFace_NormalBoss;
                std::vector<NXOpen::Face*> SealFace_NoSideBoss;
                std::vector<NXOpen::Face*> SealFace_DoubleLayerBoss;

                BossRecognitionAndCreate(AllFaces,
                    SealFace_NormalBoss, Feature_NormalBoss,
                    SealFace_NoSideBoss, Feature_NoSideBoss,
                    SealFace_DoubleLayerBoss, Feature_DoubleLayerBoss,
                    _Approach_Normal, workPart);

                // 区分内外凸台
                double approachPoint[3] = {0.0, 0.0, 0.0};
                DistinguishBossOutOrIn(Feature_NormalBoss, Feature_NoSideBoss,
                    Feature_DoubleLayerBoss, approachPoint);
            }

            if (strEnum == "全部特征" || strEnum == "槽类特征")
            {
                // ---- 弧形槽识别 ----
                std::vector<NXOpen::Face*> SealFace_CircularGroove0;
                std::vector<NXOpen::Face*> SealFace_CircularGroove1;
                CircularGrooveRecognition(AllFaces, pSelectedFace, workPart,
                    SealFace_CircularGroove0, SealFace_CircularGroove1, pSelectedFace);
                OrderCircularGroove(SealFace_CircularGroove0, pSelectedFace);
                OrderCircularGroove(SealFace_CircularGroove1, pSelectedFace);
                CreateCircularGrooveFeature(pSelectedFace,
                    SealFace_CircularGroove0, SealFace_CircularGroove1,
                    Feature_CircularGroove0, Feature_CircularGroove1);

                // ---- 下陷槽识别 ----
                std::vector<NXOpen::Face*> SealFace_SubsidenceGroove;
                std::vector<NXOpen::Face*> SealFace_HookFaceRib;
                SubsidenceGrooveRecognition(AllFaces, BottomPlaneFace, TopPlaneFace,
                    ParallelPlaneFace, SealFace_SubsidenceGroove, SealFace_HookFaceRib);
                CreateSubsidenceGrooveFeature(SealFace_SubsidenceGroove,
                    Feature_SubsidenceGroove, pSelectedFace);

                // ---- 平面浅槽识别 ----
                std::vector<NXOpen::Face*> SealFace_PlanerBlindGroove;
                PlanerBlindGrooveRecognition(AllFaces, BottomPlaneFace, TopPlaneFace,
                    ParallelPlaneFace, SealFace_PlanerBlindGroove);
                OrderingFace(SealFace_PlanerBlindGroove, _Approach_Normal);
                CreatePlanerBlindGrooveFeature(SealFace_PlanerBlindGroove,
                    Feature_PlanerBlindGroove);

                // ---- 通槽识别 ----
                std::vector<NXOpen::Face*> SealFace_PassGroove;
                PassGrooveRecognition(AllFaces, SealFace_PassGroove);
                OrderingFace(SealFace_PassGroove, _Approach_Normal);
                CreatePassGrooveFeature(SealFace_PassGroove, Feature_PassGroove);
            }

            if (strEnum == "全部特征" || strEnum == "孔类特征")
            {
                // ---- 轴向孔识别 ----
                std::vector<NXOpen::Face*> LeftSealHole_FaceVector;
                std::vector<NXOpen::Face*> SealFace_DirectionNormalHole;
                std::vector<NXOpen::Face*> SealFace_DirectionRightHole;
                std::vector<NXOpen::Face*> SealFace_DirectionBlindHole;

                DirectionMillRecognition(AllFaces, pSelectedFace,
                    BottomPlaneFace, TopPlaneFace, ParallelPlaneFace,
                    LeftSealHole_FaceVector,
                    SealFace_DirectionNormalHole,
                    SealFace_DirectionRightHole,
                    SealFace_DirectionBlindHole);

                CreateDirectionMillFeature(BottomPlaneFace, TopPlaneFace,
                    ParallelPlaneFace,
                    SealFace_DirectionNormalHole,
                    SealFace_DirectionRightHole,
                    SealFace_DirectionBlindHole,
                    Feature_DirectionNormalHole,
                    Feature_DirectionRightHole,
                    Feature_DirectionBlindHole);
            }

            hasRecognized = true;

            // 更新结果显示
            UpdateResultDisplay();

            // 高亮所有识别到的特征
            HighlightFeatures("全部特征");
        }

        //=====================================================================
        // multiline_string0: 结果显示（只读，无需处理）
        //=====================================================================
        else if(block == multiline_string0)
        {
            // 多行字符串仅用于显示，无交互逻辑
        }

        //=====================================================================
        // button01: 输出识别结果到外部文件
        //=====================================================================
        else if(block == button01)
        {
            if (!hasRecognized)
            {
                uc1601("请先进行特征识别!", 1);
                return 0;
            }
            ExportResultsToFile();
        }

        //=====================================================================
        // enum01: 高亮显示已识别特征
        //=====================================================================
        else if(block == enum01)
        {
            PropertyList* enum01Props = enum01->GetProperties();
            NXString nxStrEnum01 = enum01Props->GetEnumAsString("Value");
            delete enum01Props;
            string strEnum01 = nxStrEnum01.GetLocaleText();

            if (!hasRecognized)
            {
                uc1601("请先进行特征识别!", 1);
                return 0;
            }

            UnhighlightAll();
            HighlightFeatures(strEnum01);
        }

        //=====================================================================
        // button02: 删除鼠标选中的特征
        //=====================================================================
        else if(block == button02)
        {
            // 获取用户当前选中的面（通过selection0或selection01）
            // 查找该面所属的特征并从结果集中移除
            PropertyList* selection0Props = selection0->GetProperties();
            std::vector<NXOpen::TaggedObject*> selectedObjects =
                selection0Props->GetTaggedObjectVector("SelectedObjects");
            delete selection0Props;

            if (selectedObjects.size() == 0)
            {
                uc1601("请先在模型中选中要删除的特征面!", 1);
                return 0;
            }

            // 遍历选中对象，查找属于哪个特征，然后删除该特征
            for (size_t s = 0; s < selectedObjects.size(); s++)
            {
                NXOpen::Face* selFace = dynamic_cast<NXOpen::Face*>(selectedObjects[s]);
                if (selFace == NULL) continue;

                bool found = false;

                // 搜索凸台特征
                for (size_t i = 0; i < Feature_NormalBoss.size() && !found; i++)
                {
                    NormalBoss* f = Feature_NormalBoss[i];
                    if (Is_Face_in_lpFace(selFace, f->TopFaceVector) != -1 ||
                        Is_Face_in_lpFace(selFace, f->SideFaceVector) != -1 ||
                        Is_Face_in_lpFace(selFace, f->BottomFaceVector) != -1)
                    {
                        delete f;
                        Feature_NormalBoss.erase(Feature_NormalBoss.begin() + i);
                        found = true;
                    }
                }
                for (size_t i = 0; i < Feature_NoSideBoss.size() && !found; i++)
                {
                    NoSideBoss* f = Feature_NoSideBoss[i];
                    if (Is_Face_in_lpFace(selFace, f->TopFaceVector) != -1 ||
                        Is_Face_in_lpFace(selFace, f->BottomFaceVector) != -1)
                    {
                        delete f;
                        Feature_NoSideBoss.erase(Feature_NoSideBoss.begin() + i);
                        found = true;
                    }
                }
                for (size_t i = 0; i < Feature_DoubleLayerBoss.size() && !found; i++)
                {
                    DoubleLayerBoss* f = Feature_DoubleLayerBoss[i];
                    if (Is_Face_in_lpFace(selFace, f->TopFaceVector) != -1 ||
                        Is_Face_in_lpFace(selFace, f->SideFaceVector) != -1 ||
                        Is_Face_in_lpFace(selFace, f->BottomFaceVector) != -1)
                    {
                        delete f;
                        Feature_DoubleLayerBoss.erase(Feature_DoubleLayerBoss.begin() + i);
                        found = true;
                    }
                }

                // 搜索槽特征
                for (size_t i = 0; i < Feature_CircularGroove0.size() && !found; i++)
                {
                    if (Is_Face_in_lpFace(selFace, Feature_CircularGroove0[i]->TopFaceVector) != -1)
                    {
                        delete Feature_CircularGroove0[i];
                        Feature_CircularGroove0.erase(Feature_CircularGroove0.begin() + i);
                        found = true;
                    }
                }
                for (size_t i = 0; i < Feature_CircularGroove1.size() && !found; i++)
                {
                    if (Is_Face_in_lpFace(selFace, Feature_CircularGroove1[i]->TopFaceVector) != -1)
                    {
                        delete Feature_CircularGroove1[i];
                        Feature_CircularGroove1.erase(Feature_CircularGroove1.begin() + i);
                        found = true;
                    }
                }
                for (size_t i = 0; i < Feature_SubsidenceGroove.size() && !found; i++)
                {
                    if (Is_Face_in_lpFace(selFace, Feature_SubsidenceGroove[i]->TopFaceVector) != -1)
                    {
                        delete Feature_SubsidenceGroove[i];
                        Feature_SubsidenceGroove.erase(Feature_SubsidenceGroove.begin() + i);
                        found = true;
                    }
                }
                for (size_t i = 0; i < Feature_PlanerBlindGroove.size() && !found; i++)
                {
                    PlanerBlindGroove* f = Feature_PlanerBlindGroove[i];
                    if (Is_Face_in_lpFace(selFace, f->BottomFaceVector) != -1 ||
                        Is_Face_in_lpFace(selFace, f->SideFaceVector) != -1)
                    {
                        delete f;
                        Feature_PlanerBlindGroove.erase(Feature_PlanerBlindGroove.begin() + i);
                        found = true;
                    }
                }
                for (size_t i = 0; i < Feature_PassGroove.size() && !found; i++)
                {
                    PassGroove* f = Feature_PassGroove[i];
                    if (Is_Face_in_lpFace(selFace, f->TopFaceVector) != -1 ||
                        Is_Face_in_lpFace(selFace, f->SideFaceVector) != -1 ||
                        Is_Face_in_lpFace(selFace, f->BottomFaceVector) != -1)
                    {
                        delete f;
                        Feature_PassGroove.erase(Feature_PassGroove.begin() + i);
                        found = true;
                    }
                }

                // 搜索孔特征
                for (size_t i = 0; i < Feature_DirectionNormalHole.size() && !found; i++)
                {
                    DirectionNormalHole* f = Feature_DirectionNormalHole[i];
                    if (Is_Face_in_lpFace(selFace, f->InWallVector) != -1 ||
                        Is_Face_in_lpFace(selFace, f->TopFaceVector) != -1)
                    {
                        delete f;
                        Feature_DirectionNormalHole.erase(Feature_DirectionNormalHole.begin() + i);
                        found = true;
                    }
                }
                for (size_t i = 0; i < Feature_DirectionRightHole.size() && !found; i++)
                {
                    DirectionRightHole* f = Feature_DirectionRightHole[i];
                    if (Is_Face_in_lpFace(selFace, f->InWallVector) != -1 ||
                        Is_Face_in_lpFace(selFace, f->RightVector) != -1)
                    {
                        delete f;
                        Feature_DirectionRightHole.erase(Feature_DirectionRightHole.begin() + i);
                        found = true;
                    }
                }
                for (size_t i = 0; i < Feature_DirectionBlindHole.size() && !found; i++)
                {
                    DirectionBlindHole* f = Feature_DirectionBlindHole[i];
                    if (Is_Face_in_lpFace(selFace, f->InWallVector) != -1 ||
                        Is_Face_in_lpFace(selFace, f->TopFaceVector) != -1)
                    {
                        delete f;
                        Feature_DirectionBlindHole.erase(Feature_DirectionBlindHole.begin() + i);
                        found = true;
                    }
                }
            }

            UnhighlightAll();
            UpdateResultDisplay();
            HighlightFeatures("全部特征");
        }
    }
    catch(exception& ex)
    {
        FeatureDected260702::theUI->NXMessageBox()->Show("Block Styler",
            NXOpen::NXMessageBox::DialogTypeError, ex.what());
    }
    return 0;
}

//------------------------------------------------------------------------------
// ok_cb
//------------------------------------------------------------------------------
int FeatureDected260702::ok_cb()
{
    int errorCode = 0;
    try { errorCode = apply_cb(); }
    catch(exception& ex)
    {
        errorCode = 1;
        FeatureDected260702::theUI->NXMessageBox()->Show("Block Styler",
            NXOpen::NXMessageBox::DialogTypeError, ex.what());
    }
    return errorCode;
}

//------------------------------------------------------------------------------
// GetBlockProperties
//------------------------------------------------------------------------------
PropertyList* FeatureDected260702::GetBlockProperties(const char *blockID)
{
    return theDialog->GetBlockProperties(blockID);
}

//=============================================================================
// 工具函数实现（严格遵循 PublicFunctions.cpp）
//=============================================================================

//------------------------------------------------------------------------------
// get_MainNormal_fr_plane: 获取平面的主法向量
//------------------------------------------------------------------------------
void FeatureDected260702::get_MainNormal_fr_plane(NXOpen::Face* ipFace, NXOpen::Vector3d& MainNormal)
{
    tag_t facetag = ipFace->Tag();
    double param[2];
    double point[3];
    double u1[3], v1[3], u2[3], v2[3];
    double unit_norm[3];
    double radii[2];
    double uv_min_max[4];

    UF_MODL_ask_face_uv_minmax(facetag, uv_min_max);

    double uaverage = (uv_min_max[0] + uv_min_max[1]) / 2;
    double vaverage = (uv_min_max[2] + uv_min_max[3]) / 2;

    param[0] = uaverage;
    param[1] = vaverage;

    UF_MODL_ask_face_props(facetag, param, point, u1, v1, u2, v2, unit_norm, radii);

    MainNormal.X = unit_norm[0];
    MainNormal.Y = unit_norm[1];
    MainNormal.Z = unit_norm[2];
}

//------------------------------------------------------------------------------
// get_Angle_Two_vector3d: 计算两个3D向量之间的夹角（弧度）
//------------------------------------------------------------------------------
double FeatureDected260702::get_Angle_Two_vector3d(NXOpen::Vector3d v1, NXOpen::Vector3d v2)
{
    double AA = v1.X*v2.X + v1.Y*v2.Y + v1.Z*v2.Z;
    double B1 = sqrt(v1.X*v1.X + v1.Y*v1.Y + v1.Z*v1.Z);
    double B2 = sqrt(v2.X*v2.X + v2.Y*v2.Y + v2.Z*v2.Z);
    double BB = B1 * B2;
    double cos_value = AA / BB;

    if (cos_value < 1.00001 && cos_value > 0.99999) cos_value = 1;
    if (cos_value > -1.00001 && cos_value < -0.99999) cos_value = -1;

    return acos(cos_value);
}

//------------------------------------------------------------------------------
// get_Adjacent_Faces: 获取一个面的所有邻接面（通过共享边）
//------------------------------------------------------------------------------
void FeatureDected260702::get_Adjacent_Faces(NXOpen::Face* ipFace,
    std::vector<NXOpen::Face*>& AdjacentFaces)
{
    AdjacentFaces.clear();
    std::vector<NXOpen::Edge*> pFace_Each_Edge = ipFace->GetEdges();

    for (size_t i = 0; i != pFace_Each_Edge.size(); i++)
    {
        NXOpen::Edge* pEdge_Each = pFace_Each_Edge[i];
        std::vector<NXOpen::Face*> pEdge_Each_Faces = pEdge_Each->GetFaces();
        for (size_t j = 0; j != pEdge_Each_Faces.size(); j++)
        {
            NXOpen::Face* pFace_Each = pEdge_Each_Faces[j];
            if (pFace_Each->Tag() != ipFace->Tag())
                AdjacentFaces.push_back(pFace_Each);
        }
    }
}

//------------------------------------------------------------------------------
// get_External_Adjacent_Faces: 获取外邻接面
//（共享边中至少有一条"非光滑"边的邻面）
//------------------------------------------------------------------------------
void FeatureDected260702::get_External_Adjacent_Faces(NXOpen::Face* ipFace,
    std::vector<NXOpen::Face*>& ExternalAdjacentFaces)
{
    ExternalAdjacentFaces.clear();
    uf_list_p_t edge_list;
    UF_MODL_ask_face_edges(ipFace->Tag(), &edge_list);

    int count;
    UF_MODL_ask_list_count(edge_list, &count);

    for (int j = 0; j < count; j++)
    {
        tag_t EdgeTag = NULL_TAG;
        UF_MODL_ask_list_item(edge_list, j, &EdgeTag);

        logical is_smooth;
        UF_MODL_ask_edge_smoothness(EdgeTag, 0, &is_smooth);

        if (is_smooth == false)
        {
            uf_list_p_t face_list;
            UF_MODL_ask_edge_faces(EdgeTag, &face_list);

            int face_count;
            UF_MODL_ask_list_count(face_list, &face_count);

            for (int k = 0; k < face_count; k++)
            {
                tag_t FaceTag = NULL_TAG;
                UF_MODL_ask_list_item(face_list, k, &FaceTag);
                if (ipFace->Tag() != FaceTag)
                {
                    NXObjectManager ObjectManager;
                    TaggedObject* pTaggedObject = ObjectManager.Get(FaceTag);
                    Face* pFace = dynamic_cast<Face*>(pTaggedObject);
                    if (pFace != NULL)
                        ExternalAdjacentFaces.push_back(pFace);
                }
            }
        }
    }
    RemoveDplicateFace(ExternalAdjacentFaces);
}

//------------------------------------------------------------------------------
// get_Inner_Adjacent_Faces: 获取内邻接面
//（共享边中全部为"光滑"边的邻面）
//------------------------------------------------------------------------------
void FeatureDected260702::get_Inner_Adjacent_Faces(NXOpen::Face* ipFace,
    std::vector<NXOpen::Face*>& InnerAdjacentFaces)
{
    InnerAdjacentFaces.clear();
    uf_list_p_t edge_list;
    UF_MODL_ask_face_edges(ipFace->Tag(), &edge_list);

    int count;
    UF_MODL_ask_list_count(edge_list, &count);

    for (int j = 0; j < count; j++)
    {
        tag_t EdgeTag = NULL_TAG;
        UF_MODL_ask_list_item(edge_list, j, &EdgeTag);

        logical is_smooth;
        UF_MODL_ask_edge_smoothness(EdgeTag, 0, &is_smooth);

        if (is_smooth == true)
        {
            uf_list_p_t face_list;
            UF_MODL_ask_edge_faces(EdgeTag, &face_list);

            int face_count;
            UF_MODL_ask_list_count(face_list, &face_count);

            for (int k = 0; k < face_count; k++)
            {
                tag_t FaceTag = NULL_TAG;
                UF_MODL_ask_list_item(face_list, k, &FaceTag);
                if (ipFace->Tag() != FaceTag)
                {
                    NXObjectManager ObjectManager;
                    TaggedObject* pTaggedObject = ObjectManager.Get(FaceTag);
                    Face* pFace = dynamic_cast<Face*>(pTaggedObject);
                    if (pFace != NULL)
                        InnerAdjacentFaces.push_back(pFace);
                }
            }
        }
    }
    RemoveDplicateFace(InnerAdjacentFaces);
}

//------------------------------------------------------------------------------
// Is_Face_in_lpFace: 判断面是否在列表中
//------------------------------------------------------------------------------
int FeatureDected260702::Is_Face_in_lpFace(NXOpen::Face* ipFace,
    std::vector<NXOpen::Face*> ilpFace)
{
    if (ipFace == NULL) return -1;
    tag_t order1 = ipFace->Tag();
    for (int i = 0; i < (int)ilpFace.size(); i++)
    {
        if (ilpFace[i] != NULL && order1 == ilpFace[i]->Tag())
            return i;
    }
    return -1;
}

//------------------------------------------------------------------------------
// RemoveFaceVectorInFaceVector
//------------------------------------------------------------------------------
void FeatureDected260702::RemoveFaceVectorInFaceVector(
    std::vector<NXOpen::Face*> FaceVector1,
    std::vector<NXOpen::Face*>& FaceVector2)
{
    for (size_t i = 0; i != FaceVector1.size(); i++)
    {
        for (size_t j = 0; j != FaceVector2.size(); j++)
        {
            if (FaceVector1[i]->Tag() == FaceVector2[j]->Tag())
            {
                FaceVector2.erase(FaceVector2.begin() + j);
                break;
            }
        }
    }
}

//------------------------------------------------------------------------------
// PutFaceVectorIntoFaceVector
//------------------------------------------------------------------------------
void FeatureDected260702::PutFaceVectorIntoFaceVector(
    std::vector<NXOpen::Face*> FaceVector1,
    std::vector<NXOpen::Face*>& FaceVector2)
{
    for (size_t i = 0; i != FaceVector1.size(); i++)
    {
        if (Is_Face_in_lpFace(FaceVector1[i], FaceVector2) == -1)
        {
            FaceVector2.push_back(FaceVector1[i]);
        }
    }
}

//------------------------------------------------------------------------------
// IsTwoFaceSmooth: 判断两面的共享边是否光滑
//------------------------------------------------------------------------------
logical FeatureDected260702::IsTwoFaceSmooth(NXOpen::Face* EachFace, NXOpen::Face* OtherFace)
{
    std::vector<NXOpen::Edge*> Corporate_Edge;
    Corporate_Edge.clear();

    std::vector<NXOpen::Edge*> EachFaceEdges = EachFace->GetEdges();
    std::vector<NXOpen::Edge*> OtherFaceEdges = OtherFace->GetEdges();

    for (size_t i = 0; i < EachFaceEdges.size(); i++)
    {
        tag_t tag1 = EachFaceEdges[i]->Tag();
        for (size_t j = 0; j < OtherFaceEdges.size(); j++)
        {
            tag_t tag2 = OtherFaceEdges[j]->Tag();
            if (tag1 == tag2)
            {
                Corporate_Edge.push_back(EachFaceEdges[i]);
            }
        }
    }

    if (Corporate_Edge.size() == 0) return true;

    logical is_smooth;
    UF_MODL_ask_edge_smoothness(Corporate_Edge[0]->Tag(), 1, &is_smooth);
    return is_smooth;
}

//------------------------------------------------------------------------------
// GetEdge_From_TwoFaces: 获取两个面的共享边
//------------------------------------------------------------------------------
void FeatureDected260702::GetEdge_From_TwoFaces(NXOpen::Face* EachFace,
    NXOpen::Face* OtherFace, std::vector<NXOpen::Edge*>& Corporate_EdgeVector)
{
    Corporate_EdgeVector.clear();
    if (EachFace == NULL || OtherFace == NULL) return;
    if (EachFace->Tag() == OtherFace->Tag()) return;

    std::vector<NXOpen::Edge*> EachFaceEdges = EachFace->GetEdges();
    std::vector<NXOpen::Edge*> OtherFaceEdges = OtherFace->GetEdges();

    for (size_t i = 0; i < EachFaceEdges.size(); i++)
    {
        tag_t tag1 = EachFaceEdges[i]->Tag();
        for (size_t j = 0; j < OtherFaceEdges.size(); j++)
        {
            tag_t tag2 = OtherFaceEdges[j]->Tag();
            if (tag1 == tag2)
                Corporate_EdgeVector.push_back(EachFaceEdges[i]);
        }
    }
}

//------------------------------------------------------------------------------
// get_FaceAngle: Get angle between two faces at a shared edge
// NX 12 compatible: use UF_EVAL_ask_limits + UF_MODL_ask_face_data
//------------------------------------------------------------------------------
void FeatureDected260702::get_FaceAngle(NXOpen::Face* iFaceThis,
    NXOpen::Face* iFaceOther, NXOpen::Edge* iEdge, double& pMainAngle)
{
    // Approach: Get face data (axis/normal direction) for both faces.
    // For planar faces, dir = face normal.
    // For cylindrical/conical faces, dir = cylinder/cone axis.
    // Compute the angle between these direction vectors.
    // NX 12 removed UF_EVAL_ask_mid_param and UF_MODL_ask_face_props_at_point,
    // so we use the more portable UF_MODL_ask_face_data approach.

    int type1 = 0, norm_dir1 = 0;
    double point1[3], dir1[3], box1[6], radius1, rad_data1;
    UF_MODL_ask_face_data(iFaceThis->Tag(), &type1, point1, dir1, box1,
                          &radius1, &rad_data1, &norm_dir1);

    int type2 = 0, norm_dir2 = 0;
    double point2[3], dir2[3], box2[6], radius2, rad_data2;
    UF_MODL_ask_face_data(iFaceOther->Tag(), &type2, point2, dir2, box2,
                          &radius2, &rad_data2, &norm_dir2);

    // Normalize direction vectors
    double mag1 = sqrt(dir1[0]*dir1[0] + dir1[1]*dir1[1] + dir1[2]*dir1[2]);
    double mag2 = sqrt(dir2[0]*dir2[0] + dir2[1]*dir2[1] + dir2[2]*dir2[2]);

    if (mag1 < 1e-10 || mag2 < 1e-10)
    {
        pMainAngle = 0.0;
        return;
    }

    double n1[3] = { dir1[0]/mag1, dir1[1]/mag1, dir1[2]/mag1 };
    double n2[3] = { dir2[0]/mag2, dir2[1]/mag2, dir2[2]/mag2 };

    double dot = n1[0]*n2[0] + n1[1]*n2[1] + n1[2]*n2[2];
    if (dot > 1.0) dot = 1.0;
    if (dot < -1.0) dot = -1.0;

    pMainAngle = acos(dot);
}

//------------------------------------------------------------------------------
// calcArea: Calculate face surface area
// NX 12 compatible: estimate from bounding box for rough comparison
//------------------------------------------------------------------------------
double FeatureDected260702::calcArea(NXOpen::Part* workPart, NXOpen::Face* face)
{
    if (workPart == NULL || face == NULL) return 0.0;

    // Use bounding box diagonal area estimate for rough comparison.
    // Used only for: area > 5000 (circular groove), area ~= 1644 (boss filter)
    int type = 0, norm_dir = 0;
    double point[3], dir[3], box[6], radius, rad_data;
    UF_MODL_ask_face_data(face->Tag(), &type, point, dir, box,
                          &radius, &rad_data, &norm_dir);

    // Estimate area from bounding box
    double dx = fabs(box[3] - box[0]);
    double dy = fabs(box[4] - box[1]);
    double dz = fabs(box[5] - box[2]);

    // Use the two largest dimensions for area estimate
    double dims[3] = { dx, dy, dz };
    // Simple bubble sort for 3 elements
    if (dims[0] < dims[1]) { double t = dims[0]; dims[0] = dims[1]; dims[1] = t; }
    if (dims[1] < dims[2]) { double t = dims[1]; dims[1] = dims[2]; dims[2] = t; }
    if (dims[0] < dims[1]) { double t = dims[0]; dims[0] = dims[1]; dims[1] = t; }

    return dims[0] * dims[1]; // Area = largest two dimensions
}

//------------------------------------------------------------------------------
// get_face_maxRad: 获取面的最大曲率半径
//------------------------------------------------------------------------------
void FeatureDected260702::get_face_maxRad(NXOpen::Face* iFace, double& MaxFaceRad)
{
    MaxFaceRad = 0;
    tag_t FaceTag = iFace->Tag();
    double uv_min_max[4];
    UF_MODL_ask_face_uv_minmax(FaceTag, uv_min_max);

    double u_min = uv_min_max[0];
    double u_max = uv_min_max[1];
    double v_min = uv_min_max[2];
    double v_max = uv_min_max[3];

    int num_samples = 5;
    double max_radius = 0;

    for (int i = 0; i <= num_samples; i++)
    {
        for (int j = 0; j <= num_samples; j++)
        {
            double u = u_min + (u_max - u_min) * i / num_samples;
            double v = v_min + (v_max - v_min) * j / num_samples;
            double param[2] = {u, v};
            double point[3], u1[3], v1[3], u2[3], v2[3], unit_norm[3], radii[2];
            UF_MODL_ask_face_props(FaceTag, param, point, u1, v1, u2, v2, unit_norm, radii);

            double r1 = fabs(radii[0]);
            double r2 = fabs(radii[1]);
            double current_max = (r1 > r2) ? r1 : r2;
            if (current_max > max_radius && current_max < 10000)
                max_radius = current_max;
        }
    }
    MaxFaceRad = max_radius;
}

//------------------------------------------------------------------------------
// get_face_maxRad2
//------------------------------------------------------------------------------
void FeatureDected260702::get_face_maxRad2(NXOpen::Face* iFace, double& MaxFaceRad)
{
    get_face_maxRad(iFace, MaxFaceRad);
}

//------------------------------------------------------------------------------
// Get_FullCircle_Num: 获取面上完整圆边的数量
//------------------------------------------------------------------------------
int FeatureDected260702::Get_FullCircle_Num(NXOpen::Face* inputFace, NXOpen::Vector3d _Normal)
{
    int FullCircleNum = 0;
    std::vector<NXOpen::Edge*> FaceEdges = inputFace->GetEdges();

    for (size_t i = 0; i < FaceEdges.size(); i++)
    {
        NXOpen::Edge* EachEdge = FaceEdges[i];
        if (EachEdge->SolidEdgeType() == Edge::EdgeTypeCircular)
        {
            if (IsCircleEdgeComplete(EachEdge))
            {
                // 检查边与参考法向的关系
                uf_list_p_t face_list;
                UF_MODL_ask_edge_faces(EachEdge->Tag(), &face_list);
                int face_count;
                UF_MODL_ask_list_count(face_list, &face_count);
                if (face_count == 2)
                {
                    FullCircleNum++;
                }
            }
        }
    }
    return FullCircleNum;
}

//------------------------------------------------------------------------------
// Get_FullCircle_Num1: 获取面上的完整圆边数（变体）
//------------------------------------------------------------------------------
int FeatureDected260702::Get_FullCircle_Num1(NXOpen::Face* inputFace)
{
    int FullCircleNum = 0;
    std::vector<NXOpen::Edge*> FaceEdges = inputFace->GetEdges();

    for (size_t i = 0; i < FaceEdges.size(); i++)
    {
        NXOpen::Edge* EachEdge = FaceEdges[i];
        if (EachEdge->SolidEdgeType() == Edge::EdgeTypeCircular)
        {
            if (IsCircleEdgeComplete(EachEdge))
                FullCircleNum++;
        }
    }
    return FullCircleNum;
}

//------------------------------------------------------------------------------
// Get_FullCircle_Num2
//------------------------------------------------------------------------------
int FeatureDected260702::Get_FullCircle_Num2(NXOpen::Face* inputFace)
{
    return Get_FullCircle_Num1(inputFace);
}

//------------------------------------------------------------------------------
// get_Inner_Edge: 获取面的内环边
//------------------------------------------------------------------------------
void FeatureDected260702::get_Inner_Edge(NXOpen::Face* ipFace,
    std::vector<NXOpen::Edge*>& Inner_EdgeVector)
{
    Inner_EdgeVector.clear();
    uf_list_p_t edge_list;
    UF_MODL_ask_face_edges(ipFace->Tag(), &edge_list);

    int count;
    UF_MODL_ask_list_count(edge_list, &count);

    for (int j = 0; j < count; j++)
    {
        tag_t EdgeTag = NULL_TAG;
        UF_MODL_ask_list_item(edge_list, j, &EdgeTag);

        logical is_smooth;
        UF_MODL_ask_edge_smoothness(EdgeTag, 0, &is_smooth);

        if (is_smooth == true)
        {
            NXObjectManager ObjectManager;
            TaggedObject* pTaggedObject = ObjectManager.Get(EdgeTag);
            Edge* pEdge = dynamic_cast<Edge*>(pTaggedObject);
            if (pEdge != NULL)
                Inner_EdgeVector.push_back(pEdge);
        }
    }
}

//------------------------------------------------------------------------------
// IsCircleEdgeComplete: 判断边是否为完整圆（360度闭合）
//------------------------------------------------------------------------------
bool FeatureDected260702::IsCircleEdgeComplete(NXOpen::Edge* EachEdge)
{
    if (EachEdge->SolidEdgeType() != Edge::EdgeTypeCircular)
        return false;

    UF_EVAL_p_t evaluator = NULL;
    UF_EVAL_initialize(EachEdge->Tag(), &evaluator);

    UF_EVAL_arc_t arc;
    UF_EVAL_ask_arc(evaluator, &arc);

    double limit[2];
    UF_EVAL_ask_limits(evaluator, limit);

    double range = limit[1] - limit[0];
    double twoPI = 2 * PI;

    if (fabs(fabs(range) - twoPI) < 0.001)
        return true;
    else
        return false;
}

//------------------------------------------------------------------------------
// GetOneAdjacentFace: 通过边和当前面获取另一个邻面
//------------------------------------------------------------------------------
void FeatureDected260702::GetOneAdjacentFace(NXOpen::Edge* BeforeEdge,
    NXOpen::Face* BeforeFace, NXOpen::Face*& AdjacentFace)
{
    AdjacentFace = BeforeFace;
    std::vector<NXOpen::Face*> EdgeFaces = BeforeEdge->GetFaces();
    for (size_t i = 0; i < EdgeFaces.size(); i++)
    {
        if (EdgeFaces[i]->Tag() != BeforeFace->Tag())
        {
            AdjacentFace = EdgeFaces[i];
            return;
        }
    }
}

//------------------------------------------------------------------------------
// Get_Next_Face: 沿拓扑链获取下一个面（跳过一个"无用"面）
//------------------------------------------------------------------------------
void FeatureDected260702::Get_Next_Face(NXOpen::Face* EachFace,
    NXOpen::Face* UselessFace, NXOpen::Face*& UsefulFace)
{
    UsefulFace = EachFace;
    std::vector<NXOpen::Face*> AdjacentFaces;
    get_Adjacent_Faces(EachFace, AdjacentFaces);

    for (size_t i = 0; i < AdjacentFaces.size(); i++)
    {
        if (AdjacentFaces[i]->Tag() != UselessFace->Tag())
        {
            UsefulFace = AdjacentFaces[i];
            return;
        }
    }
}

//------------------------------------------------------------------------------
// Get_RightAndWrong_Edge_FromCylinder
//------------------------------------------------------------------------------
bool FeatureDected260702::Get_RightAndWrong_Edge_FromCylinder(
    NXOpen::Face* EachFace, NXOpen::Edge*& RightEdge, NXOpen::Edge*& WrongEdge)
{
    RightEdge = NULL;
    WrongEdge = NULL;

    std::vector<NXOpen::Edge*> EachFace_EdgeVector = EachFace->GetEdges();
    if (EachFace_EdgeVector.size() != 2) return false;

    if (!IsCircleEdgeComplete(EachFace_EdgeVector[0]) &&
        !IsCircleEdgeComplete(EachFace_EdgeVector[1]))
        return false;
    else if (IsCircleEdgeComplete(EachFace_EdgeVector[0]) &&
             IsCircleEdgeComplete(EachFace_EdgeVector[1]))
    {
        double point[3] = {0, 0, 0};
        UF_EVAL_arc_t arc1, arc2;
        UF_EVAL_p_t ev1 = NULL, ev2 = NULL;
        UF_EVAL_initialize(EachFace_EdgeVector[0]->Tag(), &ev1);
        UF_EVAL_initialize(EachFace_EdgeVector[1]->Tag(), &ev2);
        UF_EVAL_ask_arc(ev1, &arc1);
        UF_EVAL_ask_arc(ev2, &arc2);

        double d1, d2;
        UF_VEC3_distance(arc1.center, point, &d1);
        UF_VEC3_distance(arc2.center, point, &d2);

        if (d1 > d2)
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
    else if (!IsCircleEdgeComplete(EachFace_EdgeVector[0]) &&
              IsCircleEdgeComplete(EachFace_EdgeVector[1]))
    {
        RightEdge = EachFace_EdgeVector[1];
        WrongEdge = EachFace_EdgeVector[0];
    }
    else
    {
        RightEdge = EachFace_EdgeVector[0];
        WrongEdge = EachFace_EdgeVector[1];
    }
    return true;
}

//------------------------------------------------------------------------------
// GetBossBottomCornerFace / GetBossBottomFace
//------------------------------------------------------------------------------
void FeatureDected260702::GetBossBottomCornerFace(std::vector<NXOpen::Face*> TopFaceVector,
    std::vector<NXOpen::Face*> SideFaceVector,
    std::vector<NXOpen::Face*>& BottomCornerFaceVector,
    std::vector<NXOpen::Face*>& BottomFaceVector)
{
    BottomCornerFaceVector.clear();
    BottomFaceVector.clear();

    if (TopFaceVector.size() == 0 || SideFaceVector.size() == 0) return;

    // 通过侧面查找底角面和底面
    for (size_t i = 0; i < SideFaceVector.size(); i++)
    {
        NXOpen::Face* sideFace = SideFaceVector[i];
        std::vector<NXOpen::Face*> sideAdjFaces;
        get_External_Adjacent_Faces(sideFace, sideAdjFaces);

        for (size_t j = 0; j < sideAdjFaces.size(); j++)
        {
            NXOpen::Face* adjFace = sideAdjFaces[j];
            if (adjFace->Tag() == sideFace->Tag()) continue;
            if (Is_Face_in_lpFace(adjFace, TopFaceVector) != -1) continue;

            if (adjFace->SolidFaceType() == Face::FaceTypeBlending ||
                adjFace->SolidFaceType() == Face::FaceTypeConical ||
                adjFace->SolidFaceType() == Face::FaceTypeSurfaceOfRevolution)
            {
                if (Is_Face_in_lpFace(adjFace, BottomCornerFaceVector) == -1)
                    BottomCornerFaceVector.push_back(adjFace);
            }
            else if (adjFace->SolidFaceType() == Face::FaceTypePlanar)
            {
                if (Is_Face_in_lpFace(adjFace, BottomFaceVector) == -1)
                    BottomFaceVector.push_back(adjFace);
            }
        }
    }
}

void FeatureDected260702::GetBossBottomFace(std::vector<NXOpen::Face*> TopFaceVector,
    std::vector<NXOpen::Face*> SideFaceVector,
    std::vector<NXOpen::Face*> BottomCornerFaceVector,
    std::vector<NXOpen::Face*>& BottomFaceVector)
{
    BottomFaceVector.clear();
    GetBossBottomCornerFace(TopFaceVector, SideFaceVector,
        BottomCornerFaceVector, BottomFaceVector);
}

//------------------------------------------------------------------------------
// get_Two_Point_Distance
//------------------------------------------------------------------------------
double FeatureDected260702::get_Two_Point_Distance(double p1[3], double p2[3])
{
    double dx = p1[0] - p2[0];
    double dy = p1[1] - p2[1];
    double dz = p1[2] - p2[2];
    return sqrt(dx*dx + dy*dy + dz*dz);
}

//------------------------------------------------------------------------------
// RemoveDplicateFace: 面列表去重
//------------------------------------------------------------------------------
void FeatureDected260702::RemoveDplicateFace(std::vector<NXOpen::Face*>& FacesVector)
{
    for (size_t i = 0; i < FacesVector.size(); i++)
    {
        for (size_t j = i + 1; j < FacesVector.size(); j++)
        {
            if (FacesVector[i]->Tag() == FacesVector[j]->Tag())
            {
                FacesVector.erase(FacesVector.begin() + j);
                j--;
            }
        }
    }
}

//------------------------------------------------------------------------------
// OrderingFace: 沿法向对面排序
//------------------------------------------------------------------------------
void FeatureDected260702::OrderingFace(std::vector<NXOpen::Face*>& FacesVector,
    NXOpen::Vector3d _Normal)
{
    if (FacesVector.size() <= 1) return;

    // 冒泡排序：沿法向投影位置排序
    for (size_t i = 0; i < FacesVector.size(); i++)
    {
        for (size_t j = i + 1; j < FacesVector.size(); j++)
        {
            int type1, type2;
            int norm_dir1, norm_dir2;
            double point1[3], point2[3];
            double dir1[3], dir2[3];
            double box1[6], box2[6];
            double radius1, radius2;
            double rad_data1, rad_data2;

            UF_MODL_ask_face_data(FacesVector[i]->Tag(), &type1, point1, dir1,
                box1, &radius1, &rad_data1, &norm_dir1);
            UF_MODL_ask_face_data(FacesVector[j]->Tag(), &type2, point2, dir2,
                box2, &radius2, &rad_data2, &norm_dir2);

            double proj1 = point1[0]*_Normal.X + point1[1]*_Normal.Y + point1[2]*_Normal.Z;
            double proj2 = point2[0]*_Normal.X + point2[1]*_Normal.Y + point2[2]*_Normal.Z;

            if (proj1 > proj2)
            {
                NXOpen::Face* temp = FacesVector[i];
                FacesVector[i] = FacesVector[j];
                FacesVector[j] = temp;
            }
        }
    }
}

//------------------------------------------------------------------------------
// OrderDirectionHole: 轴向孔排序
//------------------------------------------------------------------------------
void FeatureDected260702::OrderDirectionHole(std::vector<NXOpen::Face*>& CylinderFaces,
    NXOpen::Face* _ApproachFace)
{
    if (CylinderFaces.size() <= 1) return;

    // 沿进刀方向对面排序（同OrderingFace）
    OrderingFace(CylinderFaces, _Approach_Normal);
}

//------------------------------------------------------------------------------
// OrderBodyHole: 孔种子面排序
//------------------------------------------------------------------------------
void FeatureDected260702::OrderBodyHole(std::vector<NXOpen::Face*>& CylinderFaces,
    NXOpen::Face* _ApproachFace)
{
    if (CylinderFaces.size() <= 1) return;

    int Begin_Type;
    double Begin_Point[3];
    double Begin_Dir[3];
    double Begin_Box[6];
    double Begin_Radius[3];
    double Begin_RadData[3];
    int Begin_NormDir;
    UF_MODL_ask_face_data(_ApproachFace->Tag(), &Begin_Type, Begin_Point, Begin_Dir,
        Begin_Box, Begin_Radius, Begin_RadData, &Begin_NormDir);

    Vector3d ApproachNormal;
    ApproachNormal.X = fabs(Begin_Dir[0]);
    ApproachNormal.Y = fabs(Begin_Dir[1]);
    ApproachNormal.Z = fabs(Begin_Dir[2]);

    // 按主轴分量排序
    double absHeight = 0;
    int Flag = 0;
    if (ApproachNormal.X == 0 && ApproachNormal.Y == 0 && ApproachNormal.Z == 1)
    {
        Flag = 1;
        absHeight = Begin_Point[2];
    }
    else if (ApproachNormal.X == 1 && ApproachNormal.Y == 0 && ApproachNormal.Z == 0)
    {
        Flag = 2;
        absHeight = Begin_Point[0];
    }
    else if (ApproachNormal.X == 0 && ApproachNormal.Y == 1 && ApproachNormal.Z == 0)
    {
        Flag = 3;
        absHeight = Begin_Point[1];
    }
    else return;

    if (Flag == 0) return;

    // 按主轴方向和高度分组排序
    struct FaceAndHeight
    {
        NXOpen::Face* face;
        double value;
    };

    std::vector<FaceAndHeight> sorted;
    for (size_t i = 0; i < CylinderFaces.size(); i++)
    {
        int Type;
        double Point[3], Dir[3], Box[6], Radius[3], RadData[3];
        int NormDir;
        UF_MODL_ask_face_data(CylinderFaces[i]->Tag(), &Type, Point, Dir,
            Box, Radius, RadData, &NormDir);

        FaceAndHeight fh;
        fh.face = CylinderFaces[i];
        if (Flag == 1) fh.value = Point[2];
        else if (Flag == 2) fh.value = Point[0];
        else fh.value = Point[1];
        sorted.push_back(fh);
    }

    // 冒泡排序
    for (size_t i = 0; i < sorted.size(); i++)
    {
        for (size_t j = i + 1; j < sorted.size(); j++)
        {
            if (sorted[i].value > sorted[j].value)
            {
                FaceAndHeight temp = sorted[i];
                sorted[i] = sorted[j];
                sorted[j] = temp;
            }
        }
    }

    CylinderFaces.clear();
    for (size_t i = 0; i < sorted.size(); i++)
        CylinderFaces.push_back(sorted[i].face);
}

//------------------------------------------------------------------------------
// OrderCircularGroove: 弧形槽种子面排序
//------------------------------------------------------------------------------
void FeatureDected260702::OrderCircularGroove(std::vector<NXOpen::Face*>& CylinderFaces,
    NXOpen::Face* _ApproachFace)
{
    if (CylinderFaces.size() <= 1) return;
    OrderingFace(CylinderFaces, _Approach_Normal);
}

//------------------------------------------------------------------------------
// OrderSingleFeature
//------------------------------------------------------------------------------
void FeatureDected260702::OrderSingleFeature(std::vector<NXOpen::Face*>& FacesVector)
{
    if (FacesVector.size() <= 1) return;
    RemoveDplicateFace(FacesVector);
    OrderingFace(FacesVector, _Approach_Normal);
}

//=============================================================================
// 辅助功能实现
//=============================================================================

//------------------------------------------------------------------------------
// AnalyzePlanes: 分析平面确定顶面、底面和中间侧面
// 严格遵循 FeatureSpy.cpp 中 update_cb 的平面分析逻辑
//------------------------------------------------------------------------------
void FeatureDected260702::AnalyzePlanes(NXOpen::Face* pPartingFace,
    NXOpen::Face*& outTopFace,
    NXOpen::Face*& outBottomFace,
    std::vector<NXOpen::Face*>& outParallelFaces)
{
    outTopFace = NULL;
    outBottomFace = NULL;
    outParallelFaces.clear();

    if (pPartingFace == NULL) return;

    Vector3d ApproachNormal;
    get_MainNormal_fr_plane(pPartingFace, ApproachNormal);

    // Step 1: 收集所有平面（排除边数>50的复杂平面）
    std::vector<NXOpen::Face*> PlanerFaces;
    for (size_t i = 0; i != AllFaces.size(); i++)
    {
        NXOpen::Face* EachFace = AllFaces[i];
        if (EachFace->SolidFaceType() != Face::FaceTypePlanar) continue;

        std::vector<NXOpen::Edge*> Edges = EachFace->GetEdges();
        if (Edges.size() > 50) continue;

        int Type;
        double Point[3], Dir[3], Box[6], Radius[3], RadData[3];
        int NormDir;
        UF_MODL_ask_face_data(EachFace->Tag(), &Type, Point, Dir, Box,
            Radius, RadData, &NormDir);

        Vector3d PlanerVector(Dir[0], Dir[1], Dir[2]);
        double EachAngle = get_Angle_Two_vector3d(PlanerVector, ApproachNormal);

        // 排除法向与分型面平行（即面垂直于分型面，是侧面）的面
        if (EachAngle < 0.01 || EachAngle > 3.13) continue;

        PlanerFaces.push_back(EachFace);
    }

    // Step 2: 将平面分为顶部、底部、中间
    for (size_t i = 0; i != PlanerFaces.size(); i++)
    {
        NXOpen::Face* EachFace = PlanerFaces[i];

        int Type;
        double Point[3], Dir[3], Box[6], Radius[3], RadData[3];
        int NormDir;
        UF_MODL_ask_face_data(EachFace->Tag(), &Type, Point, Dir, Box,
            Radius, RadData, &NormDir);

        Vector3d EachFaceVector(Dir[0], Dir[1], Dir[2]);
        double Angle = get_Angle_Two_vector3d(EachFaceVector, ApproachNormal);

        // 法向同向 → 底面；反向 → 顶面
        if (Angle > 1.5) // 大致同向
        {
            if (outBottomFace == NULL)
                outBottomFace = EachFace;
        }
        else // 大致反向
        {
            if (outTopFace == NULL)
                outTopFace = EachFace;
        }

        // 收集所有中间平行平面
        if (Is_Face_in_lpFace(EachFace, outParallelFaces) == -1)
            outParallelFaces.push_back(EachFace);
    }
}

//------------------------------------------------------------------------------
// ClearAllFeatures: 清除所有识别结果
//------------------------------------------------------------------------------
void FeatureDected260702::ClearAllFeatures()
{
    // 释放凸台特征
    for (size_t i = 0; i < Feature_NormalBoss.size(); i++) delete Feature_NormalBoss[i];
    for (size_t i = 0; i < Feature_NoSideBoss.size(); i++) delete Feature_NoSideBoss[i];
    for (size_t i = 0; i < Feature_DoubleLayerBoss.size(); i++) delete Feature_DoubleLayerBoss[i];
    Feature_NormalBoss.clear();
    Feature_NoSideBoss.clear();
    Feature_DoubleLayerBoss.clear();

    // 释放槽特征
    for (size_t i = 0; i < Feature_CircularGroove0.size(); i++) delete Feature_CircularGroove0[i];
    for (size_t i = 0; i < Feature_CircularGroove1.size(); i++) delete Feature_CircularGroove1[i];
    for (size_t i = 0; i < Feature_SubsidenceGroove.size(); i++) delete Feature_SubsidenceGroove[i];
    for (size_t i = 0; i < Feature_PlanerBlindGroove.size(); i++) delete Feature_PlanerBlindGroove[i];
    for (size_t i = 0; i < Feature_PassGroove.size(); i++) delete Feature_PassGroove[i];
    Feature_CircularGroove0.clear();
    Feature_CircularGroove1.clear();
    Feature_SubsidenceGroove.clear();
    Feature_PlanerBlindGroove.clear();
    Feature_PassGroove.clear();

    // 释放孔特征
    for (size_t i = 0; i < Feature_DirectionNormalHole.size(); i++) delete Feature_DirectionNormalHole[i];
    for (size_t i = 0; i < Feature_DirectionRightHole.size(); i++) delete Feature_DirectionRightHole[i];
    for (size_t i = 0; i < Feature_DirectionBlindHole.size(); i++) delete Feature_DirectionBlindHole[i];
    Feature_DirectionNormalHole.clear();
    Feature_DirectionRightHole.clear();
    Feature_DirectionBlindHole.clear();

    hasRecognized = false;
}

//------------------------------------------------------------------------------
// UnhighlightAll: 取消所有面的高亮
//------------------------------------------------------------------------------
void FeatureDected260702::UnhighlightAll()
{
    for (size_t i = 0; i < AllFaces.size(); i++)
    {
        UF_DISP_set_highlight(AllFaces[i]->Tag(), 0);
    }
}

//------------------------------------------------------------------------------
// HighlightFeatures: 高亮指定类型的特征面
//------------------------------------------------------------------------------
void FeatureDected260702::HighlightFeatures(const std::string& featureType)
{
    int colorIndex = 0;
    int colors[] = {186, 78, 108, 36, 211, 55, 42, 136, 142, 216, 168, 98};

    auto setColor = [&](std::vector<NXOpen::Face*>& faces, int color)
    {
        for (size_t i = 0; i < faces.size(); i++)
        {
            UF_OBJ_set_color(faces[i]->Tag(), color);
        }
    };

    auto nextColor = [&]() -> int
    {
        int c = colors[colorIndex % 12];
        colorIndex++;
        return c;
    };

    if (featureType == "全部特征" || featureType == "凸台类特征")
    {
        for (size_t i = 0; i < Feature_NormalBoss.size(); i++)
        {
            int c = nextColor();
            setColor(Feature_NormalBoss[i]->TopFaceVector, c);
            setColor(Feature_NormalBoss[i]->SideFaceVector, c);
        }
        for (size_t i = 0; i < Feature_NoSideBoss.size(); i++)
        {
            int c = nextColor();
            setColor(Feature_NoSideBoss[i]->TopFaceVector, c);
        }
        for (size_t i = 0; i < Feature_DoubleLayerBoss.size(); i++)
        {
            int c = nextColor();
            setColor(Feature_DoubleLayerBoss[i]->TopFaceVector, c);
            setColor(Feature_DoubleLayerBoss[i]->SideFaceVector, c);
            setColor(Feature_DoubleLayerBoss[i]->NSideFaceVector, c);
        }
    }

    if (featureType == "全部特征" || featureType == "槽类特征")
    {
        for (size_t i = 0; i < Feature_CircularGroove0.size(); i++)
        {
            int c = nextColor();
            setColor(Feature_CircularGroove0[i]->TopFaceVector, c);
        }
        for (size_t i = 0; i < Feature_CircularGroove1.size(); i++)
        {
            int c = nextColor();
            setColor(Feature_CircularGroove1[i]->TopFaceVector, c);
        }
        for (size_t i = 0; i < Feature_SubsidenceGroove.size(); i++)
        {
            int c = nextColor();
            setColor(Feature_SubsidenceGroove[i]->TopFaceVector, c);
        }
        for (size_t i = 0; i < Feature_PlanerBlindGroove.size(); i++)
        {
            int c = nextColor();
            setColor(Feature_PlanerBlindGroove[i]->BottomFaceVector, c);
        }
        for (size_t i = 0; i < Feature_PassGroove.size(); i++)
        {
            int c = nextColor();
            setColor(Feature_PassGroove[i]->TopFaceVector, c);
        }
    }

    if (featureType == "全部特征" || featureType == "孔类特征")
    {
        for (size_t i = 0; i < Feature_DirectionNormalHole.size(); i++)
        {
            int c = nextColor();
            setColor(Feature_DirectionNormalHole[i]->InWallVector, c);
        }
        for (size_t i = 0; i < Feature_DirectionRightHole.size(); i++)
        {
            int c = nextColor();
            setColor(Feature_DirectionRightHole[i]->InWallVector, c);
        }
        for (size_t i = 0; i < Feature_DirectionBlindHole.size(); i++)
        {
            int c = nextColor();
            setColor(Feature_DirectionBlindHole[i]->InWallVector, c);
        }
    }
}

//------------------------------------------------------------------------------
// UpdateResultDisplay: 更新识别结果显示
//------------------------------------------------------------------------------
void FeatureDected260702::UpdateResultDisplay()
{
    int normalBoss = (int)Feature_NormalBoss.size();
    int noSideBoss = (int)Feature_NoSideBoss.size();
    int doubleBoss = (int)Feature_DoubleLayerBoss.size();

    int circular0 = (int)Feature_CircularGroove0.size();
    int circular1 = (int)Feature_CircularGroove1.size();
    int subsidence = (int)Feature_SubsidenceGroove.size();
    int planerBlind = (int)Feature_PlanerBlindGroove.size();
    int passGroove = (int)Feature_PassGroove.size();

    int normalHole = (int)Feature_DirectionNormalHole.size();
    int rightHole = (int)Feature_DirectionRightHole.size();
    int blindHole = (int)Feature_DirectionBlindHole.size();

    int totalBoss = normalBoss + noSideBoss + doubleBoss;
    int totalGroove = circular0 + circular1 + subsidence + planerBlind + passGroove;
    int totalHole = normalHole + rightHole + blindHole;

    char buf[1024];
    sprintf_s(buf, sizeof(buf),
        "========== 特征识别结果 ==========\n\n"
        "【凸台特征】 共 %d 个\n"
        "  普通凸台: %d\n"
        "  无侧面凸台: %d\n"
        "  双层凸台: %d\n\n"
        "【槽特征】 共 %d 个\n"
        "  弧形槽(0): %d\n"
        "  弧形槽(1): %d\n"
        "  下陷槽: %d\n"
        "  平面浅槽: %d\n"
        "  通槽: %d\n\n"
        "【孔特征】 共 %d 个\n"
        "  普通通孔: %d\n"
        "  倒角通孔: %d\n"
        "  盲孔: %d\n\n"
        "===============================\n"
        "总计特征数: %d",
        totalBoss, normalBoss, noSideBoss, doubleBoss,
        totalGroove, circular0, circular1, subsidence, planerBlind, passGroove,
        totalHole, normalHole, rightHole, blindHole,
        totalBoss + totalGroove + totalHole);

    PropertyList* mlProps = multiline_string0->GetProperties();
    mlProps->SetString("Value", buf);
    delete mlProps;
}

//------------------------------------------------------------------------------
// ExportResultsToFile: 输出识别结果到XML文件
// 遵循 FBM/FeatureSpy/xml.cpp 的格式
//------------------------------------------------------------------------------
void FeatureDected260702::ExportResultsToFile()
{
    // 获取零件路径
    Part* workPart = NXOpen::Session::GetSession()->Parts()->Work();
    NXString partPath = workPart->FullPath();
    const char* pathName = partPath.GetUTF8Text();
    std::string partPathStr(pathName);

    // 将 .prt 替换为 .xml
    std::string xmlPath = partPathStr;
    size_t pos = xmlPath.rfind(".prt");
    if (pos == std::string::npos) pos = xmlPath.rfind(".PRT");
    if (pos != std::string::npos)
        xmlPath = xmlPath.substr(0, pos);
    xmlPath += "_features.xml";

    std::ofstream outFile(xmlPath.c_str());
    if (!outFile.is_open())
    {
        uc1601("无法创建输出文件!", 1);
        return;
    }

    // 写入XML头
    outFile << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>" << std::endl;
    outFile << "<FeatureRecognitionResults>" << std::endl;
    outFile << "  <PartName>" << workPart->Leaf().GetUTF8Text() << "</PartName>" << std::endl;

    time_t now = time(0);
    char timeStr[100];
    struct tm timeinfo;
    localtime_s(&timeinfo, &now);
    strftime(timeStr, sizeof(timeStr), "%Y-%m-%d %H:%M:%S", &timeinfo);
    outFile << "  <RecognitionTime>" << timeStr << "</RecognitionTime>" << std::endl;

    // 写入凸台特征
    outFile << "  <BossFeatures count=\"" << (Feature_NormalBoss.size() + Feature_NoSideBoss.size() + Feature_DoubleLayerBoss.size()) << "\">" << std::endl;
    for (size_t i = 0; i < Feature_NormalBoss.size(); i++)
    {
        outFile << "    <NormalBoss index=\"" << i << "\">" << std::endl;
        outFile << "      <TopFaces>" << Feature_NormalBoss[i]->TopFaceVector.size() << "</TopFaces>" << std::endl;
        outFile << "      <SideFaces>" << Feature_NormalBoss[i]->SideFaceVector.size() << "</SideFaces>" << std::endl;
        outFile << "    </NormalBoss>" << std::endl;
    }
    for (size_t i = 0; i < Feature_NoSideBoss.size(); i++)
    {
        outFile << "    <NoSideBoss index=\"" << i << "\">" << std::endl;
        outFile << "      <TopFaces>" << Feature_NoSideBoss[i]->TopFaceVector.size() << "</TopFaces>" << std::endl;
        outFile << "    </NoSideBoss>" << std::endl;
    }
    for (size_t i = 0; i < Feature_DoubleLayerBoss.size(); i++)
    {
        outFile << "    <DoubleLayerBoss index=\"" << i << "\">" << std::endl;
        outFile << "      <TopFaces>" << Feature_DoubleLayerBoss[i]->TopFaceVector.size() << "</TopFaces>" << std::endl;
        outFile << "    </DoubleLayerBoss>" << std::endl;
    }
    outFile << "  </BossFeatures>" << std::endl;

    // 写入槽特征
    outFile << "  <GrooveFeatures count=\"" << (Feature_CircularGroove0.size() + Feature_CircularGroove1.size() + Feature_SubsidenceGroove.size() + Feature_PlanerBlindGroove.size() + Feature_PassGroove.size()) << "\">" << std::endl;
    for (size_t i = 0; i < Feature_CircularGroove0.size(); i++)
    {
        outFile << "    <CircularGroove type=\"0\" index=\"" << i << "\"";
        outFile << " topFaces=\"" << Feature_CircularGroove0[i]->TopFaceVector.size() << "\"";
        outFile << " cornerFaces=\"" << Feature_CircularGroove0[i]->BottomCornerFaceVector.size() << "\"";
        outFile << " limitFaces=\"" << Feature_CircularGroove0[i]->LimitFaceVector.size() << "\"";
        outFile << " />" << std::endl;
    }
    for (size_t i = 0; i < Feature_CircularGroove1.size(); i++)
    {
        outFile << "    <CircularGroove type=\"1\" index=\"" << i << "\"";
        outFile << " topFaces=\"" << Feature_CircularGroove1[i]->TopFaceVector.size() << "\"";
        outFile << " />" << std::endl;
    }
    for (size_t i = 0; i < Feature_SubsidenceGroove.size(); i++)
    {
        outFile << "    <SubsidenceGroove index=\"" << i << "\" />" << std::endl;
    }
    for (size_t i = 0; i < Feature_PlanerBlindGroove.size(); i++)
    {
        outFile << "    <PlanerBlindGroove index=\"" << i << "\"";
        outFile << " bottomFaces=\"" << Feature_PlanerBlindGroove[i]->BottomFaceVector.size() << "\"";
        outFile << " sideFaces=\"" << Feature_PlanerBlindGroove[i]->SideFaceVector.size() << "\"";
        outFile << " />" << std::endl;
    }
    for (size_t i = 0; i < Feature_PassGroove.size(); i++)
    {
        outFile << "    <PassGroove index=\"" << i << "\" />" << std::endl;
    }
    outFile << "  </GrooveFeatures>" << std::endl;

    // 写入孔特征
    outFile << "  <HoleFeatures count=\"" << (Feature_DirectionNormalHole.size() + Feature_DirectionRightHole.size() + Feature_DirectionBlindHole.size()) << "\">" << std::endl;
    for (size_t i = 0; i < Feature_DirectionNormalHole.size(); i++)
    {
        outFile << "    <DirectionNormalHole index=\"" << i << "\" />" << std::endl;
    }
    for (size_t i = 0; i < Feature_DirectionRightHole.size(); i++)
    {
        outFile << "    <DirectionRightHole index=\"" << i << "\" />" << std::endl;
    }
    for (size_t i = 0; i < Feature_DirectionBlindHole.size(); i++)
    {
        outFile << "    <DirectionBlindHole index=\"" << i << "\" />" << std::endl;
    }
    outFile << "  </HoleFeatures>" << std::endl;

    outFile << "</FeatureRecognitionResults>" << std::endl;
    outFile.close();

    char msg[512];
    sprintf_s(msg, sizeof(msg), "识别结果已导出到:\n%s", xmlPath.c_str());
    uc1601(msg, 1);
}

//=============================================================================
// 凸台特征识别（严格遵循 SpyFunctions.cpp）
//=============================================================================

//------------------------------------------------------------------------------
// DistinguishBossOutOrIn: 区分内外凸台
// 来自 SpyFunctions.cpp 第12-108行
//------------------------------------------------------------------------------
void FeatureDected260702::DistinguishBossOutOrIn(
    std::vector<NormalBoss*>& Feature_NormalBoss,
    std::vector<NoSideBoss*>& Feature_NoSideBoss,
    std::vector<DoubleLayerBoss*>& Feature_DoubleLayerBoss,
    double _Approach_Point[3])
{
    for (size_t i = 0; i != Feature_NormalBoss.size(); i++)
    {
        NXOpen::Face* EachFace = Feature_NormalBoss[i]->TopFaceVector[0];

        int type = 0, norm_dir = 0;
        double point[3], dir[3], box[6], radius, rad_data;
        UF_MODL_ask_face_data(EachFace->Tag(), &type, point, dir, box, &radius, &rad_data, &norm_dir);

        Vector3d EachFaceVector1(dir[0], dir[1], dir[2]);
        Vector3d PointToPointVector2(
            point[0] - _Approach_Point[0],
            point[1] - _Approach_Point[1],
            point[2] - _Approach_Point[2]);
        double Angle0 = get_Angle_Two_vector3d(EachFaceVector1, PointToPointVector2);

        Feature_NormalBoss[i]->OutOrIn = (Angle0 < (PI / 2)) ? true : false;
    }

    for (size_t i = 0; i != Feature_NoSideBoss.size(); i++)
    {
        NXOpen::Face* EachFace = Feature_NoSideBoss[i]->TopFaceVector[0];

        int type = 0, norm_dir = 0;
        double point[3], dir[3], box[6], radius, rad_data;
        UF_MODL_ask_face_data(EachFace->Tag(), &type, point, dir, box, &radius, &rad_data, &norm_dir);

        Vector3d EachFaceVector1(dir[0], dir[1], dir[2]);
        Vector3d PointToPointVector2(
            point[0] - _Approach_Point[0],
            point[1] - _Approach_Point[1],
            point[2] - _Approach_Point[2]);
        double Angle0 = get_Angle_Two_vector3d(EachFaceVector1, PointToPointVector2);

        Feature_NoSideBoss[i]->OutOrIn = (Angle0 < (PI / 2)) ? true : false;
    }

    for (size_t i = 0; i != Feature_DoubleLayerBoss.size(); i++)
    {
        NXOpen::Face* EachFace = Feature_DoubleLayerBoss[i]->TopFaceVector[0];

        int type = 0, norm_dir = 0;
        double point[3], dir[3], box[6], radius, rad_data;
        UF_MODL_ask_face_data(EachFace->Tag(), &type, point, dir, box, &radius, &rad_data, &norm_dir);

        Vector3d EachFaceVector1(dir[0], dir[1], dir[2]);
        Vector3d PointToPointVector2(
            point[0] - _Approach_Point[0],
            point[1] - _Approach_Point[1],
            point[2] - _Approach_Point[2]);
        double Angle0 = get_Angle_Two_vector3d(EachFaceVector1, PointToPointVector2);

        Feature_DoubleLayerBoss[i]->OutOrIn = (Angle0 < (PI / 2)) ? true : false;
    }
}

//------------------------------------------------------------------------------
// BossRecognitionAndCreate: 凸台识别和创建
// 来自 SpyFunctions.cpp 第110-509行
//------------------------------------------------------------------------------
void FeatureDected260702::BossRecognitionAndCreate(
    std::vector<NXOpen::Face*> AllFaces,
    std::vector<NXOpen::Face*> SealFace_NormalBoss,
    std::vector<NormalBoss*>& Feature_NormalBoss,
    std::vector<NXOpen::Face*> SealFace_NoSideBoss,
    std::vector<NoSideBoss*>& Feature_NoSideBoss,
    std::vector<NXOpen::Face*> SealFace_DoubleLayerBoss,
    std::vector<DoubleLayerBoss*>& Feature_DoubleLayerBoss,
    NXOpen::Vector3d _Approach_Normal,
    NXOpen::Part* workPart)
{
    // Step 1: 筛选所有符合条件的平面（边数<50，法向与分型面垂直）
    std::vector<NXOpen::Face*> PlanerFaces;
    for (size_t i = 0; i != AllFaces.size(); i++)
    {
        NXOpen::Face* EachFace = AllFaces[i];
        if (EachFace->SolidFaceType() != Face::FaceTypePlanar) continue;

        std::vector<NXOpen::Edge*> Edges = EachFace->GetEdges();
        if (Edges.size() > 50) continue;

        int Type;
        double Point[3], Dir[3], Box[6], Radius[3], RadData[3];
        int NormDir;
        UF_MODL_ask_face_data(EachFace->Tag(), &Type, Point, Dir, Box,
            Radius, RadData, &NormDir);

        Vector3d PlanerVector(Dir[0], Dir[1], Dir[2]);
        double EachAngle = get_Angle_Two_vector3d(PlanerVector, _Approach_Normal);

        // 排除法向平行于分型面的平面（那些是顶面/底面，不是凸台侧面）
        if (EachAngle < 0.01 || EachAngle > 3.13) continue;

        PlanerFaces.push_back(EachFace);
    }

    // Step 2: 对每个平面分析其邻面，判断凸台类型
    for (size_t i = 0; i != PlanerFaces.size(); i++)
    {
        NXOpen::Face* EachFace = PlanerFaces[i];
        std::vector<NXOpen::Face*> SideFaceVector;
        get_External_Adjacent_Faces(EachFace, SideFaceVector);

        bool RightFlag0 = false;
        int PlanerNum = 0;
        int BlendNum = 0;
        int ConnectNum = 0;
        NXOpen::Face* ConnectFace = NULL;

        for (size_t j = 0; j != SideFaceVector.size(); j++)
        {
            NXOpen::Face* OtherFace = SideFaceVector[j];
            bool IsSmooth = IsTwoFaceSmooth(EachFace, OtherFace);

            try
            {
                double area = 0;
                try { area = calcArea(workPart, EachFace); }
                catch (...) { area = 100; }

                if (IsSmooth == true && fabs(area - 1644.37335) > 0.01)
                {
                    RightFlag0 = true;
                    break;
                }
            }
            catch (exception&) { continue; }

            if (OtherFace->SolidFaceType() == Face::FaceTypePlanar)
            {
                PlanerNum++;

                std::vector<NXOpen::Face*> SideAdFaces;
                get_External_Adjacent_Faces(OtherFace, SideAdFaces);

                for (size_t t = 0; t != SideAdFaces.size(); t++)
                {
                    if (SideAdFaces[t]->SolidFaceType() == Face::FaceTypeCylindrical)
                    {
                        int Begin_type = 0, Begin_norm_dir = 0;
                        double Begin_point[3], Begin_dir[3], Begin_box[6];
                        double Begin_radius, rad_data;
                        UF_MODL_ask_face_data(SideAdFaces[t]->Tag(), &Begin_type,
                            Begin_point, Begin_dir, Begin_box, &Begin_radius,
                            &rad_data, &Begin_norm_dir);

                        if (Begin_radius < 1.5)
                        {
                            ConnectFace = SideAdFaces[t];
                            ConnectNum++;
                        }
                    }
                }
            }
            else if (OtherFace->SolidFaceType() == Face::FaceTypeCylindrical)
            {
                // 圆柱邻面，不在此阶段处理
            }
            else if (OtherFace->SolidFaceType() == Face::FaceTypeBlending)
            {
                BlendNum++;
            }
        }

        if (RightFlag0) continue;

        // ----- 无侧面凸台（所有邻面都是过渡面） -----
        if (BlendNum == (int)SideFaceVector.size())
        {
            NoSideBoss* Feature = new NoSideBoss;
            Feature->TopFaceVector.push_back(EachFace);
            PutFaceVectorIntoFaceVector(SideFaceVector, Feature->BottomCornerFaceVector);
            Feature_NoSideBoss.push_back(Feature);
        }
        // ----- 双层凸台（有一个连接面） -----
        else if (ConnectNum == 1)
        {
            DoubleLayerBoss* Feature = new DoubleLayerBoss;
            Feature->TopFaceVector.push_back(EachFace);
            Feature->ConnectFaceVector.push_back(ConnectFace);

            bool DoubleRight = true;
            for (size_t j = 0; j != SideFaceVector.size(); j++)
            {
                NXOpen::Face* OtherFace = SideFaceVector[j];

                if (OtherFace->SolidFaceType() == Face::FaceTypeSurfaceOfRevolution)
                {
                    DoubleRight = false;
                    break;
                }
                else if (OtherFace->SolidFaceType() == Face::FaceTypeCylindrical ||
                         OtherFace->SolidFaceType() == Face::FaceTypePlanar)
                {
                    if (OtherFace == ConnectFace) continue;
                    Feature->SideFaceVector.push_back(OtherFace);
                }
                else if (OtherFace->SolidFaceType() == Face::FaceTypeBlending)
                {
                    Feature->BottomCornerFaceVector.push_back(OtherFace);
                }
            }

            // 从侧面找底角面
            for (size_t j = 0; j != Feature->SideFaceVector.size(); j++)
            {
                NXOpen::Face* EachSideFace = Feature->SideFaceVector[j];
                std::vector<NXOpen::Face*> CornerFaceVector;
                get_Adjacent_Faces(EachSideFace, CornerFaceVector);

                for (size_t t = 0; t != CornerFaceVector.size(); t++)
                {
                    NXOpen::Face* EachCornerFace = CornerFaceVector[t];
                    if (EachCornerFace->SolidFaceType() == Face::FaceTypeBlending ||
                        EachCornerFace->SolidFaceType() == Face::FaceTypeSurfaceOfRevolution)
                    {
                        if (Is_Face_in_lpFace(EachCornerFace, Feature->SideFaceVector) == -1 &&
                            Is_Face_in_lpFace(EachCornerFace, Feature->BottomCornerFaceVector) == -1 &&
                            EachCornerFace != EachFace)
                        {
                            Feature->BottomCornerFaceVector.push_back(EachCornerFace);
                        }
                    }
                }
            }

            // 找下层凸台的顶面
            std::vector<NXOpen::Face*> ConnectAdFaceVector;
            get_Adjacent_Faces(ConnectFace, ConnectAdFaceVector);
            for (size_t j = 0; j != ConnectAdFaceVector.size(); j++)
            {
                NXOpen::Face* EachConnectAdFace = ConnectAdFaceVector[j];
                if (EachConnectAdFace != EachFace &&
                    Is_Face_in_lpFace(EachConnectAdFace, Feature->SideFaceVector) == -1 &&
                    EachConnectAdFace->SolidFaceType() == Face::FaceTypePlanar)
                {
                    Feature->NTopFaceVector.push_back(EachConnectAdFace);
                }
            }

            if (Feature->NTopFaceVector.size() != 1) { delete Feature; continue; }

            std::vector<NXOpen::Face*> NTopAdFaceVector;
            get_External_Adjacent_Faces(Feature->NTopFaceVector[0], NTopAdFaceVector);

            for (size_t j = 0; j != NTopAdFaceVector.size(); j++)
            {
                NXOpen::Face* OtherFace = NTopAdFaceVector[j];
                if (OtherFace == ConnectFace) continue;

                if (OtherFace->SolidFaceType() == Face::FaceTypeCylindrical ||
                    OtherFace->SolidFaceType() == Face::FaceTypePlanar ||
                    OtherFace->SolidFaceType() == Face::FaceTypeConical)
                {
                    Feature->NSideFaceVector.push_back(OtherFace);
                }
                else if (OtherFace->SolidFaceType() == Face::FaceTypeBlending)
                {
                    if (Is_Face_in_lpFace(OtherFace, Feature->BottomCornerFaceVector) != -1) continue;
                    Feature->BottomCornerFaceVector.push_back(OtherFace);
                }
            }

            // 从下层侧面找底角面
            for (size_t j = 0; j != Feature->NSideFaceVector.size(); j++)
            {
                NXOpen::Face* EachNSideFace = Feature->NSideFaceVector[j];
                std::vector<NXOpen::Face*> CornerFaceVector;
                get_Adjacent_Faces(EachNSideFace, CornerFaceVector);

                for (size_t t = 0; t != CornerFaceVector.size(); t++)
                {
                    NXOpen::Face* EachCornerFace = CornerFaceVector[t];
                    if (EachCornerFace->SolidFaceType() == Face::FaceTypeBlending ||
                        EachCornerFace->SolidFaceType() == Face::FaceTypeSurfaceOfRevolution ||
                        EachCornerFace->SolidFaceType() == Face::FaceTypeSpherical)
                    {
                        if (Is_Face_in_lpFace(EachCornerFace, Feature->SideFaceVector) == -1 &&
                            Is_Face_in_lpFace(EachCornerFace, Feature->BottomCornerFaceVector) == -1 &&
                            EachCornerFace != EachFace &&
                            EachCornerFace != Feature->NTopFaceVector[0] &&
                            EachCornerFace != ConnectFace &&
                            Is_Face_in_lpFace(EachCornerFace, Feature->NSideFaceVector) == -1)
                        {
                            Feature->BottomCornerFaceVector.push_back(EachCornerFace);
                        }
                    }
                }
            }

            Feature_DoubleLayerBoss.push_back(Feature);
        }
        // ----- 普通凸台 -----
        else if (ConnectNum == 0)
        {
            bool NorRight = true;
            NormalBoss* Feature = new NormalBoss;
            Feature->TopFaceVector.push_back(EachFace);
            int SidefaceNum = 0;

            for (size_t j = 0; j != SideFaceVector.size(); j++)
            {
                NXOpen::Face* OtherFace = SideFaceVector[j];

                try
                {
                    double area = 0;
                    try { area = calcArea(workPart, EachFace); }
                    catch (...) { area = 100; }

                    if (OtherFace->SolidFaceType() == Face::FaceTypeSurfaceOfRevolution &&
                        fabs(area - 1644.37335) > 0.01)
                    {
                        NorRight = false;
                        break;
                    }
                    else if (OtherFace->SolidFaceType() == Face::FaceTypeCylindrical ||
                             OtherFace->SolidFaceType() == Face::FaceTypePlanar ||
                             OtherFace->SolidFaceType() == Face::FaceTypeConical)
                    {
                        SidefaceNum++;
                        Feature->SideFaceVector.push_back(OtherFace);
                    }
                    else if (OtherFace->SolidFaceType() == Face::FaceTypeBlending)
                    {
                        Feature->BottomCornerFaceVector.push_back(OtherFace);
                    }
                }
                catch (exception&) { continue; }
            }

            if (NorRight == false) { delete Feature; continue; }

            // 从侧面找底角面
            for (size_t j = 0; j != Feature->SideFaceVector.size(); j++)
            {
                NXOpen::Face* EachSideFace = Feature->SideFaceVector[j];
                std::vector<NXOpen::Face*> CornerFaceVector;
                get_Adjacent_Faces(EachSideFace, CornerFaceVector);

                for (size_t t = 0; t != CornerFaceVector.size(); t++)
                {
                    NXOpen::Face* EachCornerFace = CornerFaceVector[t];
                    if (EachCornerFace->SolidFaceType() == Face::FaceTypeBlending ||
                        EachCornerFace->SolidFaceType() == Face::FaceTypeSurfaceOfRevolution)
                    {
                        if (Is_Face_in_lpFace(EachCornerFace, Feature->SideFaceVector) == -1 &&
                            Is_Face_in_lpFace(EachCornerFace, Feature->BottomCornerFaceVector) == -1 &&
                            EachCornerFace != EachFace)
                        {
                            Feature->BottomCornerFaceVector.push_back(EachCornerFace);
                        }
                    }
                }
            }

            if (Feature->BottomCornerFaceVector.size() != 0 ||
                (int)Feature->SideFaceVector.size() == SidefaceNum)
            {
                Feature_NormalBoss.push_back(Feature);
            }
            else
            {
                delete Feature;
            }
        }
    }
}

//=============================================================================
// 槽特征识别（严格遵循 SpyFunctions.cpp）
//=============================================================================

//------------------------------------------------------------------------------
// CircularGrooveRecognition: 弧形槽种子面识别
// 来自 SpyFunctions.cpp 第2828-2969行
//------------------------------------------------------------------------------
void FeatureDected260702::CircularGrooveRecognition(
    std::vector<NXOpen::Face*> AllFaces,
    NXOpen::Face* Begin_Face,
    NXOpen::Part* workPart,
    std::vector<NXOpen::Face*>& SealFace_CircularGroove0,
    std::vector<NXOpen::Face*>& SealFace_CircularGroove1,
    NXOpen::Face* _ApproachFace)
{
    Vector3d _ApproachNormal;
    get_MainNormal_fr_plane(Begin_Face, _ApproachNormal);

    for (size_t i = 0; i != AllFaces.size(); i++)
    {
        NXOpen::Face* EachFace = AllFaces[i];

        if (EachFace->SolidFaceType() != Face::FaceTypeCylindrical &&
            EachFace->SolidFaceType() != Face::FaceTypeConical &&
            EachFace->SolidFaceType() != Face::FaceTypeSurfaceOfRevolution)
            continue;

        int type = 0, norm_dir = 0;
        double point[3], dir[3], box[6], radius, rad_data;
        UF_MODL_ask_face_data(EachFace->Tag(), &type, point, dir, box,
            &radius, &rad_data, &norm_dir);

        // 朝外的面 -> CircularGroove1, 朝内的面 -> CircularGroove0
        if (norm_dir == +1)
        {
            std::vector<NXOpen::Edge*> EachFaceEdges = EachFace->GetEdges();
            for (size_t j = 0; j != EachFaceEdges.size(); j++)
            {
                NXOpen::Edge* EachEdge = EachFaceEdges[j];
                if (EachEdge->SolidEdgeType() == Edge::EdgeTypeCircular)
                {
                    double EdgeLength = EachEdge->GetLength();
                    if (EdgeLength > 500)
                    {
                        double area = calcArea(workPart, EachFace);
                        if (area > 5000)
                        {
                            if (EachFace->SolidFaceType() == Face::FaceTypeSurfaceOfRevolution)
                            {
                                int num = Get_FullCircle_Num(EachFace, _ApproachNormal);
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
            std::vector<NXOpen::Edge*> EachFaceEdges = EachFace->GetEdges();
            for (size_t j = 0; j != EachFaceEdges.size(); j++)
            {
                NXOpen::Edge* EachEdge = EachFaceEdges[j];
                if (EachEdge->SolidEdgeType() == Edge::EdgeTypeCircular)
                {
                    double EdgeLength = EachEdge->GetLength();
                    if (EdgeLength > 500)
                    {
                        double area = calcArea(workPart, EachFace);
                        if (area > 5000)
                        {
                            if (EachFace->SolidFaceType() == Face::FaceTypeSurfaceOfRevolution)
                            {
                                int num = Get_FullCircle_Num(EachFace, _ApproachNormal);
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

//------------------------------------------------------------------------------
// CreateCircularGrooveFeature: 弧形槽特征构建
// 来自 SpyFunctions.cpp 第2971-3193行
//------------------------------------------------------------------------------
void FeatureDected260702::CreateCircularGrooveFeature(
    NXOpen::Face* Begin_Face,
    std::vector<NXOpen::Face*> SealFace_CircularGroove0,
    std::vector<NXOpen::Face*> SealFace_CircularGroove1,
    std::vector<CircularGroove*>& Feature_CircularGroove0,
    std::vector<CircularGroove*>& Feature_CircularGroove1)
{
    Vector3d _ApproachNormal;
    get_MainNormal_fr_plane(Begin_Face, _ApproachNormal);

    auto buildFeature = [&](NXOpen::Face* EachFace, CircularGroove* Feature,
                            std::vector<NXOpen::Face*>& otherSet) -> bool
    {
        bool IsRightFlag = true;
        std::vector<NXOpen::Face*> EachAdFaces;
        get_Adjacent_Faces(EachFace, EachAdFaces);

        for (size_t j = 0; j != EachAdFaces.size(); j++)
        {
            NXOpen::Face* OtherFace = EachAdFaces[j];

            if (OtherFace->SolidFaceType() == Face::FaceTypePlanar)
            {
                IsRightFlag = false;
                break;
            }
            else if (OtherFace->SolidFaceType() == Face::FaceTypeConical ||
                     OtherFace->SolidFaceType() == Face::FaceTypeCylindrical)
            {
                int num = Get_FullCircle_Num(OtherFace, _ApproachNormal);
                if (num > 1)
                {
                    std::vector<NXOpen::Edge*> CorEdges;
                    GetEdge_From_TwoFaces(EachFace, OtherFace, CorEdges);
                    if (CorEdges.size() > 0)
                    {
                        double Angle;
                        get_FaceAngle(EachFace, OtherFace, CorEdges[0], Angle);
                        Angle = Angle * 180 / PI;
                        if (Angle > 180)
                        {
                            IsRightFlag = false;
                            break;
                        }
                        else
                        {
                            Feature->BottomCornerFaceVector.push_back(OtherFace);
                        }
                    }
                }
                else if (Is_Face_in_lpFace(OtherFace, otherSet) == -1)
                {
                    Feature->LimitFaceVector.push_back(OtherFace);
                }
            }
            else if (OtherFace->SolidFaceType() == Face::FaceTypeSurfaceOfRevolution)
            {
                int num = Get_FullCircle_Num(OtherFace, _ApproachNormal);
                if (num > 0)
                {
                    std::vector<NXOpen::Edge*> CorEdges;
                    GetEdge_From_TwoFaces(EachFace, OtherFace, CorEdges);
                    if (CorEdges.size() > 0)
                    {
                        double Angle;
                        get_FaceAngle(EachFace, OtherFace, CorEdges[0], Angle);
                        Angle = Angle * 180 / PI;
                        if (Angle > 180)
                        {
                            IsRightFlag = false;
                            break;
                        }
                        else
                        {
                            Feature->BottomCornerFaceVector.push_back(OtherFace);
                        }
                    }
                }
                else if (Is_Face_in_lpFace(OtherFace, otherSet) == -1)
                {
                    Feature->LimitFaceVector.push_back(OtherFace);
                }
            }
            else if (OtherFace->SolidFaceType() == Face::FaceTypeBlending)
            {
                Feature->LimitFaceVector.push_back(OtherFace);
            }
        }
        return IsRightFlag;
    };

    // 构建 CircularGroove0
    for (size_t i = 0; i != SealFace_CircularGroove0.size(); i++)
    {
        CircularGroove* Feature = new CircularGroove;
        Feature->TopFaceVector.push_back(SealFace_CircularGroove0[i]);
        if (buildFeature(SealFace_CircularGroove0[i], Feature, SealFace_CircularGroove1))
            Feature_CircularGroove0.push_back(Feature);
        else
            delete Feature;
    }

    // 构建 CircularGroove1
    for (size_t i = 0; i != SealFace_CircularGroove1.size(); i++)
    {
        CircularGroove* Feature = new CircularGroove;
        Feature->TopFaceVector.push_back(SealFace_CircularGroove1[i]);
        if (buildFeature(SealFace_CircularGroove1[i], Feature, SealFace_CircularGroove1))
            Feature_CircularGroove1.push_back(Feature);
        else
            delete Feature;
    }
}

//------------------------------------------------------------------------------
// SubsidenceGrooveRecognition: 下陷槽种子面识别
// 来自 SpyFunctions.cpp 第3194-3479行
//------------------------------------------------------------------------------
void FeatureDected260702::SubsidenceGrooveRecognition(
    std::vector<NXOpen::Face*> AllFaces,
    NXOpen::Face* BottomPlaneFace,
    NXOpen::Face* TopPlaneFace,
    std::vector<NXOpen::Face*> ParallelPlaneFace,
    std::vector<NXOpen::Face*>& SealFace_SubsidenceGroove,
    std::vector<NXOpen::Face*>& SealFace_HookFaceRib)
{
    std::vector<NXOpen::Face*> CylinderConicalRevolution_FaceVector;
    std::vector<NXOpen::Face*> SeedFace;

    // Phase A: 在圆柱/圆锥/旋转面中搜索种子面
    for (size_t i = 0; i != AllFaces.size(); i++)
    {
        NXOpen::Face* EachFace = AllFaces[i];

        if (EachFace->SolidFaceType() == Face::FaceTypeCylindrical ||
            EachFace->SolidFaceType() == Face::FaceTypeConical)
        {
            bool Flag = true;
            int type = 0, norm_dir = 0;
            double point[3], dir[3], box[6], radius, rad_data;
            UF_MODL_ask_face_data(EachFace->Tag(), &type, point, dir, box,
                &radius, &rad_data, &norm_dir);

            std::vector<NXOpen::Edge*> EachFace_EdgeVector = EachFace->GetEdges();
            for (size_t j = 0; j != EachFace_EdgeVector.size(); j++)
            {
                double Length = EachFace_EdgeVector[j]->GetLength();
                if (Length > 300)
                {
                    Flag = false;
                    break;
                }
            }

            if (radius > 200 && norm_dir == 1 && Flag)
            {
                uf_list_p_t edge_list;
                UF_MODL_ask_face_edges(EachFace->Tag(), &edge_list);

                int count;
                UF_MODL_ask_list_count(edge_list, &count);

                for (int j = 0; j < count; j++)
                {
                    tag_t EdgeTag = NULL_TAG;
                    UF_MODL_ask_list_item(edge_list, j, &EdgeTag);

                    logical is_smooth;
                    UF_MODL_ask_edge_smoothness(EdgeTag, 0, &is_smooth);

                    if (is_smooth == false)
                    {
                        uf_list_p_t face_list;
                        UF_MODL_ask_edge_faces(EdgeTag, &face_list);

                        int face_count;
                        UF_MODL_ask_list_count(face_list, &face_count);

                        for (int k = 0; k < face_count; k++)
                        {
                            tag_t FaceTag = NULL_TAG;
                            UF_MODL_ask_list_item(face_list, k, &FaceTag);

                            if (EachFace->Tag() != FaceTag)
                            {
                                NXObjectManager ObjectManager;
                                TaggedObject* pTaggedObject = ObjectManager.Get(FaceTag);
                                NXOpen::Face* pFace = dynamic_cast<NXOpen::Face*>(pTaggedObject);

                                if (pFace->SolidFaceType() == Face::FaceTypeParametric)
                                {
                                    SealFace_HookFaceRib.push_back(EachFace);
                                    RemoveDplicateFace(SealFace_HookFaceRib);
                                }
                                else if (pFace->SolidFaceType() == Face::FaceTypeSurfaceOfRevolution)
                                {
                                    double Rad;
                                    get_face_maxRad(pFace, Rad);
                                    if (Rad < 10)
                                    {
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
                    else // is_smooth == true
                    {
                        uf_list_p_t face_list;
                        UF_MODL_ask_edge_faces(EdgeTag, &face_list);

                        int face_count;
                        UF_MODL_ask_list_count(face_list, &face_count);

                        for (int k = 0; k < face_count; k++)
                        {
                            tag_t FaceTag = NULL_TAG;
                            UF_MODL_ask_list_item(face_list, k, &FaceTag);

                            if (EachFace->Tag() != FaceTag)
                            {
                                NXObjectManager ObjectManager;
                                TaggedObject* pTaggedObject = ObjectManager.Get(FaceTag);
                                NXOpen::Face* pFace = dynamic_cast<NXOpen::Face*>(pTaggedObject);

                                if (pFace->SolidFaceType() == Face::FaceTypeBlending &&
                                    Is_Face_in_lpFace(EachFace, SealFace_HookFaceRib) == -1)
                                {
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
            double FaceRad;
            bool Flag = true;
            std::vector<NXOpen::Edge*> EachFace_EdgeVector = EachFace->GetEdges();
            get_face_maxRad(EachFace, FaceRad);

            for (size_t j = 0; j != EachFace_EdgeVector.size(); j++)
            {
                double Length = EachFace_EdgeVector[j]->GetLength();
                if (Length > 300)
                {
                    Flag = false;
                    break;
                }
            }

            if (Flag && FaceRad > 80)
            {
                std::vector<NXOpen::Face*> OutAdjacentFaces;
                get_External_Adjacent_Faces(EachFace, OutAdjacentFaces);

                for (size_t l = 0; l < OutAdjacentFaces.size(); l++)
                {
                    NXOpen::Face* EachAdjFace = OutAdjacentFaces[l];
                    if (EachAdjFace->SolidFaceType() == Face::FaceTypePlanar ||
                        EachAdjFace->SolidFaceType() == Face::FaceTypeParametric)
                    {
                        SealFace_HookFaceRib.push_back(EachFace);
                        RemoveDplicateFace(SealFace_HookFaceRib);
                    }
                    else if (EachAdjFace->SolidFaceType() == Face::FaceTypeBlending &&
                             Is_Face_in_lpFace(EachFace, SealFace_HookFaceRib) == -1)
                    {
                        CylinderConicalRevolution_FaceVector.push_back(EachFace);
                        RemoveDplicateFace(CylinderConicalRevolution_FaceVector);
                    }
                }
            }
        }
    }

    // Phase B: 验证种子面
    for (size_t i = 0; i != SeedFace.size(); i++)
    {
        NXOpen::Face* EachFace = SeedFace[i];
        std::vector<NXOpen::Face*> OutAdjacentFaces, InnerAdjacentFaces;
        get_External_Adjacent_Faces(EachFace, OutAdjacentFaces);
        get_Inner_Adjacent_Faces(EachFace, InnerAdjacentFaces);

        bool IsTopFace = true;

        for (size_t l = 0; l < OutAdjacentFaces.size(); l++)
        {
            NXOpen::Face* EachAdjFace = OutAdjacentFaces[l];
            if (EachAdjFace->Tag() == TopPlaneFace->Tag() ||
                EachAdjFace->Tag() == BottomPlaneFace->Tag() ||
                Is_Face_in_lpFace(EachAdjFace, ParallelPlaneFace) != -1)
            {
                IsTopFace = false;
                break;
            }
            else if (EachAdjFace->SolidFaceType() == Face::FaceTypePlanar)
            {
                int num = Get_FullCircle_Num1(EachAdjFace);
                if (num > 0)
                {
                    IsTopFace = false;
                    break;
                }
            }
        }

        for (size_t l = 0; l < InnerAdjacentFaces.size(); l++)
        {
            if (InnerAdjacentFaces[l]->SolidFaceType() == Face::FaceTypePlanar)
            {
                IsTopFace = false;
                break;
            }
        }

        if (IsTopFace)
        {
            SealFace_HookFaceRib.push_back(EachFace);
            RemoveDplicateFace(SealFace_HookFaceRib);
        }
    }

    // Phase C: 构建下陷槽
    for (size_t i = 0; i != CylinderConicalRevolution_FaceVector.size(); i++)
    {
        NXOpen::Face* EachFace = CylinderConicalRevolution_FaceVector[i];
        std::vector<NXOpen::Face*> AdjacentFaces;
        get_External_Adjacent_Faces(EachFace, AdjacentFaces);

        bool IsTopFace = true;
        for (size_t l = 0; l < AdjacentFaces.size(); l++)
        {
            if (AdjacentFaces[l]->SolidFaceType() == Face::FaceTypePlanar)
            {
                IsTopFace = false;
                break;
            }
        }

        if (IsTopFace && Is_Face_in_lpFace(EachFace, SealFace_HookFaceRib) == -1)
        {
            SealFace_SubsidenceGroove.push_back(EachFace);
        }
    }
}

//------------------------------------------------------------------------------
// CreateSubsidenceGrooveFeature: 下陷槽特征构建
// 来自 SpyFunctions.cpp 第3480-3594行
//------------------------------------------------------------------------------
void FeatureDected260702::CreateSubsidenceGrooveFeature(
    std::vector<NXOpen::Face*> SealFace_SubsidenceGroove,
    std::vector<SubsidenceGroove*>& Feature_SubsidenceGroove,
    NXOpen::Face* _ApproachFace)
{
    for (size_t i = 0; i != SealFace_SubsidenceGroove.size(); i++)
    {
        NXOpen::Face* EachFace = SealFace_SubsidenceGroove[i];
        SubsidenceGroove* Feature = new SubsidenceGroove;
        Feature->TopFaceVector.push_back(EachFace);

        std::vector<NXOpen::Face*> AdjacentFaces;
        get_External_Adjacent_Faces(EachFace, AdjacentFaces);

        for (size_t j = 0; j < AdjacentFaces.size(); j++)
        {
            NXOpen::Face* OtherFace = AdjacentFaces[j];
            if (OtherFace->SolidFaceType() == Face::FaceTypeConical ||
                OtherFace->SolidFaceType() == Face::FaceTypeCylindrical ||
                OtherFace->SolidFaceType() == Face::FaceTypeSurfaceOfRevolution)
            {
                double CornerRad;
                get_face_maxRad(OtherFace, CornerRad);
                if (CornerRad > 100)
                {
                    Feature->BottomCornerFaceVector.push_back(OtherFace);
                }
            }
        }

        Feature_SubsidenceGroove.push_back(Feature);
    }
}

//------------------------------------------------------------------------------
// PlanerBlindGrooveRecognition: 平面浅槽种子面识别
// 来自 SpyFunctions.cpp 第3595-3794行
//------------------------------------------------------------------------------
void FeatureDected260702::PlanerBlindGrooveRecognition(
    std::vector<NXOpen::Face*> AllFaces,
    NXOpen::Face* BottomPlaneFace,
    NXOpen::Face* TopPlaneFace,
    std::vector<NXOpen::Face*> ParallelPlaneFace,
    std::vector<NXOpen::Face*>& SealFace_PlanerBlindGroove)
{
    // Step 1: 收集所有平面（排除顶面、底面、中间侧面）
    std::vector<NXOpen::Face*> AllPlaneFaces;
    for (size_t i = 0; i != AllFaces.size(); i++)
    {
        NXOpen::Face* EachFace = AllFaces[i];
        if (EachFace->SolidFaceType() != Face::FaceTypePlanar) continue;

        if (EachFace->Tag() != TopPlaneFace->Tag() &&
            EachFace->Tag() != BottomPlaneFace->Tag() &&
            Is_Face_in_lpFace(EachFace, ParallelPlaneFace) == -1)
        {
            AllPlaneFaces.push_back(EachFace);
        }
    }

    // Step 2: 识别角面（圆角面，半径<10mm）
    std::vector<NXOpen::Face*> CornerFaces;
    for (size_t i = 0; i != AllPlaneFaces.size(); i++)
    {
        NXOpen::Face* EachPlaneFace = AllPlaneFaces[i];

        uf_list_p_t edge_list;
        UF_MODL_ask_face_edges(EachPlaneFace->Tag(), &edge_list);

        int count;
        UF_MODL_ask_list_count(edge_list, &count);

        for (int j = 0; j < count; j++)
        {
            tag_t EdgeTag = NULL_TAG;
            UF_MODL_ask_list_item(edge_list, j, &EdgeTag);

            logical is_smooth;
            UF_MODL_ask_edge_smoothness(EdgeTag, 0, &is_smooth);

            if (is_smooth == true)
            {
                uf_list_p_t face_list;
                UF_MODL_ask_edge_faces(EdgeTag, &face_list);

                int face_count;
                UF_MODL_ask_list_count(face_list, &face_count);

                for (int k = 0; k < face_count; k++)
                {
                    tag_t FaceTag = NULL_TAG;
                    UF_MODL_ask_list_item(face_list, k, &FaceTag);

                    if (EachPlaneFace->Tag() != FaceTag)
                    {
                        NXObjectManager ObjectManager;
                        TaggedObject* pTaggedObject = ObjectManager.Get(FaceTag);
                        NXOpen::Face* pFace = dynamic_cast<NXOpen::Face*>(pTaggedObject);

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

    // Step 3: 验证平面浅槽候选
    for (size_t i = 0; i != AllPlaneFaces.size(); i++)
    {
        NXOpen::Face* EachPlaneFace = AllPlaneFaces[i];
        std::vector<NXOpen::Face*> AdjacentFaces;
        get_External_Adjacent_Faces(EachPlaneFace, AdjacentFaces);

        bool Flag = true;
        int CornerNum = 0;
        for (size_t j = 0; j != AdjacentFaces.size(); j++)
        {
            if (Is_Face_in_lpFace(AdjacentFaces[j], CornerFaces) != -1)
            {
                CornerNum++;
            }
            else
            {
                Flag = false;
                break;
            }
        }

        if (Flag && CornerNum > 1)
        {
            SealFace_PlanerBlindGroove.push_back(EachPlaneFace);
        }
    }
}

//------------------------------------------------------------------------------
// CreatePlanerBlindGrooveFeature: 平面浅槽特征构建
// 来自 SpyFunctions.cpp 第3796-3971行
//------------------------------------------------------------------------------
void FeatureDected260702::CreatePlanerBlindGrooveFeature(
    std::vector<NXOpen::Face*> SealFace_PlanerBlindGroove,
    std::vector<PlanerBlindGroove*>& Feature_PlanerBlindGroove)
{
    for (size_t i = 0; i != SealFace_PlanerBlindGroove.size(); i++)
    {
        NXOpen::Face* EachFace = SealFace_PlanerBlindGroove[i];
        PlanerBlindGroove* Feature = new PlanerBlindGroove;
        Feature->BottomFaceVector.push_back(EachFace);

        // 找底角面
        uf_list_p_t edge_list;
        UF_MODL_ask_face_edges(EachFace->Tag(), &edge_list);

        int count;
        UF_MODL_ask_list_count(edge_list, &count);

        for (int j = 0; j < count; j++)
        {
            tag_t EdgeTag = NULL_TAG;
            UF_MODL_ask_list_item(edge_list, j, &EdgeTag);

            logical is_smooth;
            UF_MODL_ask_edge_smoothness(EdgeTag, 0, &is_smooth);

            if (is_smooth == true)
            {
                uf_list_p_t face_list;
                UF_MODL_ask_edge_faces(EdgeTag, &face_list);

                int face_count;
                UF_MODL_ask_list_count(face_list, &face_count);

                for (int k = 0; k < face_count; k++)
                {
                    tag_t FaceTag = NULL_TAG;
                    UF_MODL_ask_list_item(face_list, k, &FaceTag);

                    if (EachFace->Tag() != FaceTag)
                    {
                        NXObjectManager ObjectManager;
                        TaggedObject* pTaggedObject = ObjectManager.Get(FaceTag);
                        NXOpen::Face* pFace = dynamic_cast<NXOpen::Face*>(pTaggedObject);

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

        // 找侧面
        for (size_t t = 0; t != Feature->BottomCornerFaceVector.size(); t++)
        {
            NXOpen::Face* EachCornerFace = Feature->BottomCornerFaceVector[t];
            std::vector<NXOpen::Face*> ConnerAd_FaceVector;
            get_Adjacent_Faces(EachCornerFace, ConnerAd_FaceVector);

            for (size_t z = 0; z != ConnerAd_FaceVector.size(); z++)
            {
                NXOpen::Face* EachSideFace = ConnerAd_FaceVector[z];
                if (EachSideFace != EachFace &&
                    Is_Face_in_lpFace(EachSideFace, Feature->BottomCornerFaceVector) == -1 &&
                    Is_Face_in_lpFace(EachSideFace, Feature->SideFaceVector) == -1)
                {
                    Feature->SideFaceVector.push_back(EachSideFace);
                }
            }
        }

        Feature_PlanerBlindGroove.push_back(Feature);
    }
}

//------------------------------------------------------------------------------
// PassGrooveRecognition: 通槽种子面识别
// 来自 SpyFunctions.cpp 第3973-4084行
//------------------------------------------------------------------------------
void FeatureDected260702::PassGrooveRecognition(
    std::vector<NXOpen::Face*> AllFaces,
    std::vector<NXOpen::Face*>& SealFace_PassGroove)
{
    // 收集所有平面
    std::vector<NXOpen::Face*> PlanerFaces;
    for (size_t i = 0; i != AllFaces.size(); i++)
    {
        if (AllFaces[i]->SolidFaceType() == Face::FaceTypePlanar)
            PlanerFaces.push_back(AllFaces[i]);
    }

    for (size_t i = 0; i != PlanerFaces.size(); i++)
    {
        NXOpen::Face* EachFace = PlanerFaces[i];

        std::vector<NXOpen::Edge*> EachInner_EdgeVector;
        get_Inner_Edge(EachFace, EachInner_EdgeVector);

        std::vector<NXOpen::Edge*> NoComplete_EdgeVector;
        int CompleteEdgeNum = 0;

        // 区分内环边中的完整圆和非完整边
        for (size_t j = 0; j != EachInner_EdgeVector.size(); j++)
        {
            if (IsCircleEdgeComplete(EachInner_EdgeVector[j]) == false)
            {
                NoComplete_EdgeVector.push_back(EachInner_EdgeVector[j]);
            }
            else
            {
                CompleteEdgeNum++;
            }
        }

        if (CompleteEdgeNum == 0) continue; // 必须至少有一个完整圆（孔）

        // 通过非完整边找到下一个面
        std::vector<NXOpen::Face*> Next_FaceVector;
        for (size_t j = 0; j != NoComplete_EdgeVector.size(); j++)
        {
            NXOpen::Edge* EachEdge = NoComplete_EdgeVector[j];
            std::vector<NXOpen::Face*> Temp_FaceVector = EachEdge->GetFaces();

            for (size_t t = 0; t != Temp_FaceVector.size(); t++)
            {
                if (Temp_FaceVector[t] != EachFace &&
                    Is_Face_in_lpFace(Temp_FaceVector[t], Next_FaceVector) == -1)
                {
                    Next_FaceVector.push_back(Temp_FaceVector[t]);
                }
            }
        }

        if (Next_FaceVector.size() != 0)
        {
            SealFace_PassGroove.push_back(EachFace);
        }
    }
}

//------------------------------------------------------------------------------
// CreatePassGrooveFeature: 通槽特征构建
// 来自 SpyFunctions.cpp 第4086-4240行
//------------------------------------------------------------------------------
void FeatureDected260702::CreatePassGrooveFeature(
    std::vector<NXOpen::Face*> SealFace_PassGroove,
    std::vector<PassGroove*>& Feature_PassGroove)
{
    for (size_t i = 0; i != SealFace_PassGroove.size(); i++)
    {
        NXOpen::Face* EachFace = SealFace_PassGroove[i];
        PassGroove* Feature = new PassGroove;
        Feature->TopFaceVector.push_back(EachFace);

        std::vector<NXOpen::Edge*> EachInner_EdgeVector;
        get_Inner_Edge(EachFace, EachInner_EdgeVector);

        std::vector<NXOpen::Edge*> NoComplete_EdgeVector;
        for (size_t j = 0; j != EachInner_EdgeVector.size(); j++)
        {
            if (IsCircleEdgeComplete(EachInner_EdgeVector[j]) == false)
                NoComplete_EdgeVector.push_back(EachInner_EdgeVector[j]);
        }

        // 找顶面的邻面（通过非完整内边）
        for (size_t j = 0; j != NoComplete_EdgeVector.size(); j++)
        {
            NXOpen::Edge* EachEdge = NoComplete_EdgeVector[j];
            std::vector<NXOpen::Face*> Temp_FaceVector = EachEdge->GetFaces();

            for (size_t t = 0; t != Temp_FaceVector.size(); t++)
            {
                NXOpen::Face* TempFace = Temp_FaceVector[t];
                if (TempFace == EachFace) continue;

                std::vector<NXOpen::Edge*> CorEdge;
                GetEdge_From_TwoFaces(EachFace, TempFace, CorEdge);
                if (CorEdge.size() == 0) continue;

                double Angle;
                get_FaceAngle(EachFace, TempFace, CorEdge[0], Angle);
                Angle = Angle / PI * 180;

                if (Angle > 220 && Angle < 230) // 直侧面 ~225°
                {
                    if (Is_Face_in_lpFace(TempFace, Feature->RightFaceVector) == -1)
                        Feature->RightFaceVector.push_back(TempFace);
                }
                else if (Angle > 265 && Angle < 275) // 侧面 ~270°
                {
                    if (Is_Face_in_lpFace(TempFace, Feature->SideFaceVector) == -1)
                        Feature->SideFaceVector.push_back(TempFace);
                }
            }
        }

        // 如果没有直接找到侧面，从RightFace继续找
        if (Feature->SideFaceVector.size() == 0)
        {
            for (size_t j = 0; j != Feature->RightFaceVector.size(); j++)
            {
                NXOpen::Face* EachRightFace = Feature->RightFaceVector[j];
                std::vector<NXOpen::Face*> Side_FaceVector;
                get_Adjacent_Faces(EachRightFace, Side_FaceVector);

                for (size_t t = 0; t != Side_FaceVector.size(); t++)
                {
                    NXOpen::Face* EachSideFace = Side_FaceVector[t];
                    if (EachSideFace != EachFace &&
                        Is_Face_in_lpFace(EachSideFace, Feature->RightFaceVector) == -1 &&
                        Is_Face_in_lpFace(EachSideFace, Feature->SideFaceVector) == -1)
                    {
                        std::vector<NXOpen::Edge*> CorEdge;
                        GetEdge_From_TwoFaces(EachRightFace, EachSideFace, CorEdge);
                        if (CorEdge.size() == 0) continue;

                        double Angle;
                        get_FaceAngle(EachRightFace, EachSideFace, CorEdge[0], Angle);
                        Angle = Angle / PI * 180;

                        if (Angle > 220 && Angle < 230)
                            Feature->SideFaceVector.push_back(EachSideFace);
                    }
                }
            }
        }

        // 找底面
        for (size_t j = 0; j != Feature->SideFaceVector.size(); j++)
        {
            NXOpen::Face* EachSideFace = Feature->SideFaceVector[j];
            std::vector<NXOpen::Face*> Bottom_FaceVector;
            get_Adjacent_Faces(EachSideFace, Bottom_FaceVector);

            for (size_t t = 0; t != Bottom_FaceVector.size(); t++)
            {
                NXOpen::Face* EachBottomFace = Bottom_FaceVector[t];
                if (EachBottomFace != EachFace &&
                    Is_Face_in_lpFace(EachBottomFace, Feature->RightFaceVector) == -1 &&
                    Is_Face_in_lpFace(EachBottomFace, Feature->BottomFaceVector) == -1 &&
                    Is_Face_in_lpFace(EachBottomFace, Feature->SideFaceVector) == -1)
                {
                    Feature->BottomFaceVector.push_back(EachBottomFace);
                }
            }
        }

        Feature_PassGroove.push_back(Feature);
    }
}

//=============================================================================
// 孔特征识别（严格遵循 SpyFunctions.cpp）
//=============================================================================

//------------------------------------------------------------------------------
// DirectionMillRecognition: 轴向孔种子面识别
// 来自 SpyFunctions.cpp 第643-869行
//------------------------------------------------------------------------------
void FeatureDected260702::DirectionMillRecognition(
    std::vector<NXOpen::Face*> AllFaces,
    NXOpen::Face* BeginFace,
    NXOpen::Face* BottomPlaneFace,
    NXOpen::Face* TopPlaneFace,
    std::vector<NXOpen::Face*> ParallelPlaneFace,
    std::vector<NXOpen::Face*>& LeftSealHole_FaceVector,
    std::vector<NXOpen::Face*>& SealFace_DirectionNormalHole,
    std::vector<NXOpen::Face*>& SealFace_DirectionRightHole,
    std::vector<NXOpen::Face*>& SealFace_DirectionBlindHole)
{
    std::vector<NXOpen::Face*> seal_FaceVector;
    std::vector<NXOpen::Face*> sealdirection_FaceVector;

    Vector3d Normal_Vector;
    get_MainNormal_fr_plane(BeginFace, Normal_Vector);

    // 第1轮筛选：按几何条件筛选圆柱面
    for (size_t i = 0; i != AllFaces.size(); i++)
    {
        NXOpen::Face* EachFace = AllFaces[i];

        if (EachFace->SolidFaceType() != Face::FaceTypeCylindrical) continue;

        int type = 0, norm_dir = 0;
        double point[3], dir[3], box[6], radius, rad_data;
        UF_MODL_ask_face_data(EachFace->Tag(), &type, point, dir, box,
            &radius, &rad_data, &norm_dir);

        std::vector<NXOpen::Edge*> EachFace_EdgeVector = EachFace->GetEdges();

        // 筛选条件：半径<35mm，法向朝内(norm_dir==-1)，边数<20
        if (radius < 35 && norm_dir == -1 && EachFace_EdgeVector.size() < 20)
        {
            seal_FaceVector.push_back(EachFace);
        }
        else
        {
            continue;
        }

        Vector3d Each_Vector(dir[0], dir[1], dir[2]);
        double Angle = get_Angle_Two_vector3d(Normal_Vector, Each_Vector);
        Angle = Angle / PI * 180;

        if (Angle < 1 || Angle > 179) // 平行于进刀方向
        {
            sealdirection_FaceVector.push_back(EachFace);
        }
        else
        {
            LeftSealHole_FaceVector.push_back(EachFace); // 留给后续阶段
        }
    }

    // 第2轮分类：根据相邻面细分孔类型
    for (size_t i = 0; i != sealdirection_FaceVector.size(); i++)
    {
        NXOpen::Face* Each_Face = sealdirection_FaceVector[i];

        std::vector<NXOpen::Face*> EachAd_FaceVector;
        get_Adjacent_Faces(Each_Face, EachAd_FaceVector);

        if (EachAd_FaceVector.size() != 2) continue;

        // 判断孔类型
        if (EachAd_FaceVector[0]->Tag() == TopPlaneFace->Tag())
        {
            if (Is_Face_in_lpFace(EachAd_FaceVector[1], ParallelPlaneFace) != -1)
                SealFace_DirectionNormalHole.push_back(Each_Face);
            else
                SealFace_DirectionBlindHole.push_back(Each_Face);
        }
        else if (EachAd_FaceVector[1]->Tag() == TopPlaneFace->Tag())
        {
            if (Is_Face_in_lpFace(EachAd_FaceVector[0], ParallelPlaneFace) != -1)
                SealFace_DirectionNormalHole.push_back(Each_Face);
            else
                SealFace_DirectionBlindHole.push_back(Each_Face);
        }
        else if (EachAd_FaceVector[0]->Tag() == BottomPlaneFace->Tag() &&
                 Is_Face_in_lpFace(EachAd_FaceVector[1], ParallelPlaneFace) != -1)
        {
            SealFace_DirectionNormalHole.push_back(Each_Face);
        }
        else if (EachAd_FaceVector[1]->Tag() == BottomPlaneFace->Tag() &&
                 Is_Face_in_lpFace(EachAd_FaceVector[0], ParallelPlaneFace) != -1)
        {
            SealFace_DirectionNormalHole.push_back(Each_Face);
        }
        else if (Is_Face_in_lpFace(EachAd_FaceVector[0], ParallelPlaneFace) != -1 &&
                 EachAd_FaceVector[1]->SolidFaceType() == Face::FaceTypeConical)
        {
            SealFace_DirectionRightHole.push_back(Each_Face);
        }
        else if (Is_Face_in_lpFace(EachAd_FaceVector[1], ParallelPlaneFace) != -1 &&
                 EachAd_FaceVector[0]->SolidFaceType() == Face::FaceTypeConical)
        {
            SealFace_DirectionRightHole.push_back(Each_Face);
        }
    }

    // 排序
    OrderDirectionHole(SealFace_DirectionBlindHole, BeginFace);
    OrderDirectionHole(SealFace_DirectionNormalHole, BeginFace);
    OrderDirectionHole(SealFace_DirectionRightHole, BeginFace);
}

//------------------------------------------------------------------------------
// CreateDirectionMillFeature: 轴向孔特征构建
// 来自 SpyFunctions.cpp 第872-1035行
//------------------------------------------------------------------------------
void FeatureDected260702::CreateDirectionMillFeature(
    NXOpen::Face* BottomPlaneFace,
    NXOpen::Face* TopPlaneFace,
    std::vector<NXOpen::Face*> ParallelPlaneFace,
    std::vector<NXOpen::Face*> SealFace_DirectionNormalHole,
    std::vector<NXOpen::Face*> SealFace_DirectionRightHole,
    std::vector<NXOpen::Face*> SealFace_DirectionBlindHole,
    std::vector<DirectionNormalHole*>& Feature_DirectionNormalHole,
    std::vector<DirectionRightHole*>& Feature_DirectionRightHole,
    std::vector<DirectionBlindHole*>& Feature_DirectionBlindHole)
{
    // ---- 构建普通通孔 ----
    for (size_t i = 0; i != SealFace_DirectionNormalHole.size(); i++)
    {
        NXOpen::Face* EachFace = SealFace_DirectionNormalHole[i];
        std::vector<NXOpen::Face*> EachAd_FaceVector;
        get_Adjacent_Faces(EachFace, EachAd_FaceVector);

        if (EachAd_FaceVector.size() != 2) continue;

        NXOpen::Face* TopFace = NULL;
        NXOpen::Face* BottomFace = NULL;

        if (EachAd_FaceVector[0]->Tag() == TopPlaneFace->Tag() &&
            Is_Face_in_lpFace(EachAd_FaceVector[1], ParallelPlaneFace) != -1)
        {
            TopFace = EachAd_FaceVector[0];
            BottomFace = EachAd_FaceVector[1];
        }
        else
        {
            TopFace = EachAd_FaceVector[1];
            BottomFace = EachAd_FaceVector[0];
        }

        DirectionNormalHole* Feature = new DirectionNormalHole;
        Feature->TopFaceVector.push_back(TopFace);
        Feature->BottomFaceVector.push_back(BottomFace);
        Feature->InWallVector.push_back(EachFace);
        Feature->name = "普通通孔";
        Feature_DirectionNormalHole.push_back(Feature);
    }

    // ---- 构建倒角通孔 ----
    for (size_t i = 0; i != SealFace_DirectionRightHole.size(); i++)
    {
        NXOpen::Face* EachFace = SealFace_DirectionRightHole[i];
        std::vector<NXOpen::Face*> EachAd_FaceVector;
        get_Adjacent_Faces(EachFace, EachAd_FaceVector);

        if (EachAd_FaceVector.size() != 2) continue;

        NXOpen::Face* RightFace = NULL;
        NXOpen::Face* TopFace = NULL;
        NXOpen::Face* BottomFace = NULL;

        if (Is_Face_in_lpFace(EachAd_FaceVector[0], ParallelPlaneFace) != -1 &&
            EachAd_FaceVector[1]->SolidFaceType() == Face::FaceTypeConical)
        {
            RightFace = EachAd_FaceVector[1];
            BottomFace = EachAd_FaceVector[0];
        }
        else if (Is_Face_in_lpFace(EachAd_FaceVector[1], ParallelPlaneFace) != -1 &&
                 EachAd_FaceVector[0]->SolidFaceType() == Face::FaceTypeConical)
        {
            RightFace = EachAd_FaceVector[0];
            BottomFace = EachAd_FaceVector[1];
        }

        if (RightFace == NULL) continue;

        std::vector<NXOpen::Face*> RightAd_FaceVector;
        get_Adjacent_Faces(RightFace, RightAd_FaceVector);
        if (RightAd_FaceVector.size() == 2)
        {
            if (RightAd_FaceVector[0]->Tag() == TopPlaneFace->Tag())
                TopFace = RightAd_FaceVector[0];
            else if (RightAd_FaceVector[1]->Tag() == TopPlaneFace->Tag())
                TopFace = RightAd_FaceVector[1];
        }

        DirectionRightHole* Feature = new DirectionRightHole;
        Feature->TopFaceVector.push_back(TopFace);
        Feature->RightVector.push_back(RightFace);
        Feature->InWallVector.push_back(EachFace);
        Feature->BottomFaceVector.push_back(BottomFace);
        Feature->name = "倒角通孔";
        Feature_DirectionRightHole.push_back(Feature);
    }

    // ---- 构建盲孔 ----
    for (size_t i = 0; i != SealFace_DirectionBlindHole.size(); i++)
    {
        NXOpen::Face* EachFace = SealFace_DirectionBlindHole[i];
        std::vector<NXOpen::Face*> EachAd_FaceVector;
        get_Adjacent_Faces(EachFace, EachAd_FaceVector);

        if (EachAd_FaceVector.size() != 2) continue;

        NXOpen::Face* TopFace = NULL;
        NXOpen::Face* BottomFace = NULL;

        if (EachAd_FaceVector[0]->Tag() == TopPlaneFace->Tag() &&
            Is_Face_in_lpFace(EachAd_FaceVector[1], ParallelPlaneFace) == -1)
        {
            TopFace = EachAd_FaceVector[0];
            BottomFace = EachAd_FaceVector[1];
        }
        else if (EachAd_FaceVector[1] == TopPlaneFace &&
                 Is_Face_in_lpFace(EachAd_FaceVector[0], ParallelPlaneFace) == -1)
        {
            TopFace = EachAd_FaceVector[1];
            BottomFace = EachAd_FaceVector[0];
        }

        DirectionBlindHole* Feature = new DirectionBlindHole;
        Feature->TopFaceVector.push_back(TopFace);
        Feature->InWallVector.push_back(EachFace);
        Feature->BottomFaceVector.push_back(BottomFace);
        Feature->name = "盲孔";
        Feature_DirectionBlindHole.push_back(Feature);
    }
}
