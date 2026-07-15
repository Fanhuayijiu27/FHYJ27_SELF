#include "xml.h"
using namespace NXOpen;

//-----------------------------------------------------------------------------
// xml : constructor
//-----------------------------------------------------------------------------
xml::xml():
TaggedObject()
{
}

//-----------------------------------------------------------------------------
// xml : destructor
//-----------------------------------------------------------------------------
xml::~xml()
{
}

//-----------------------------------------------------------------------------
// xml : copy constructor
//-----------------------------------------------------------------------------
xml::xml(xml& original):
TaggedObject(original)
{
}

//-----------------------------------------------------------------------------
// xml : equal operator
//-----------------------------------------------------------------------------
xml& xml::operator=(xml& original)
{
	TaggedObject::operator=(original);
	return *this;
}

void xml::initial(std::vector<CircularGroove*> CircularGroove0Result, std::vector<CircularGroove*> CircularGroove1Result,
				  std::vector<SubsidenceGroove*> SubsidenceGrooveResult,std::vector<PlanerBlindGroove*> PlanerBlindGrooveResult,std::vector<PassGroove*> PassGrooveResult,

				  std::vector<DoubleLayerBoss*> DoubleLayerBossResult, std::vector<NormalBoss*> NormalBossResult, std::vector<NoSideBoss*> NoSideBossResult,

				  std::vector<DirectionNormalHole*> DirectionNormalHoleResult,std::vector<DirectionRightHole*> DirectionRightHoleResult,std::vector<DirectionBlindHole*> DirectionBlindHoleResult,
				  std::vector<WallNormalHole*> WallNormalHoleResult,std::vector<WallCircleCombineHole*> WallCircleCombineHoleResult,std::vector<RibCombineHole*> RibCombineHoleResult,
				  std::vector<BossNormalHole*> BossNormalHoleResult,std::vector<BossOneRightHole*> BossOneRightHoleResult,std::vector<BossOneCircleHole*> BossOneCircleHoleResult,
				  std::vector<BossDoubleRightHole*> BossDoubleRightHoleResult,std::vector<BossDoubleCircleHole*> BossDoubleCircleHoleResult,std::vector<BossOneRightOneCircleHole*> BossOneRightOneCircleHoleResult,
				  std::vector<BossManyLayerHole*> BossManyLayerHoleResult,std::vector<BossOutsideWideCombineHole*> BossOutsideWideCombineHoleResult,std::vector<BossInsideWideCombineHole*> BossInsideWideCombineHoleResult,
				  std::vector<BossMidCricleCombineHole*> BossMidCricleCombineHoleResult, std::vector<BossNormalBlindHole*> BossNormalBlindHoleResult, std::vector<BossRightBlindHole*> BossRightBlindHoleResult, std::vector<Other3Hole*> otherHole3Result,
				  std::vector<DirectionRib*> DirectionRibResult,std::vector<HookFaceRib*> HookFaceRibResult,
				  std::vector<Outline*> OutlineResult,std::vector<Outline*> Outline1Result,
				  NXOpen::Vector3d _Normal)
{
	CircularGroove0SpyResult = CircularGroove0Result;
	CircularGroove1SpyResult = CircularGroove1Result;
	SubsidenceGrooveSpyResult = SubsidenceGrooveResult;
	PlanerBlindGrooveSpyResult = PlanerBlindGrooveResult;
	PassGrooveSpyResult = PassGrooveResult;
//----------------------boss-------------------------------//
	DoubleLayerBossSpyResult = DoubleLayerBossResult;
	NormalBossSpyResult = NormalBossResult;
	NoSideBossSpyResult = NoSideBossResult;
//----------------------hole-------------------------------//
	DirectionNormalHoleSpyResult = DirectionNormalHoleResult;
	DirectionRightHoleSpyResult = DirectionRightHoleResult;
	DirectionBlindHoleSpyResult = DirectionBlindHoleResult;
	WallNormalHoleSpyResult = WallNormalHoleResult;
	WallCircleCombineHoleSpyResult = WallCircleCombineHoleResult;
	RibCombineHoleSpyResult = RibCombineHoleSpyResult;
	BossNormalHoleSpyResult = BossNormalHoleResult;
	BossOneRightHoleSpyResult = BossOneRightHoleResult;
	BossOneCircleHoleSpyResult = BossOneCircleHoleResult;
	BossDoubleRightHoleSpyResult = BossDoubleRightHoleResult;
	BossDoubleCircleHoleSpyResult = BossDoubleCircleHoleResult;
	BossOneRightOneCircleHoleSpyResult = BossOneRightOneCircleHoleResult;
	BossManyLayerHoleSpyResult = BossManyLayerHoleResult;
	BossOutsideWideCombineHoleSpyResult = BossOutsideWideCombineHoleResult;
	BossInsideWideCombineHoleSpyResult = BossInsideWideCombineHoleResult;
	BossMidCricleCombineHoleSpyResult = BossMidCricleCombineHoleResult;
	BossNormalBlindHoleSpyResult = BossNormalBlindHoleResult;
	BossRightBlindHoleSpyResult = BossRightBlindHoleResult;
	otherHole3SpyResult = otherHole3Result;
//----------------------Rib-------------------------------//
	DirectionRibSpyResult = DirectionRibResult;
	HookFaceRibSpyResult = HookFaceRibResult;
//-------------------------outline------------------------------------//
	OutlineSpyResult = OutlineResult;
	Outline1SpyResult = Outline1Result;

	/*RibSpyResult = RibResult;
	HoleSpyResult = HoleResult;
	ResidueSpyResult = ResidueResult;*/
	_Approach_Normal = _Normal;
}

void xml::CreateXml_for_CAM(IXMLDOMDocumentPtr &pDoc)
{
	/*uc1601("已进入程序",1);*/
	
	/*UF_UI_write_listing_window("进入程序测试2:");*/
	

	::CoInitialize(NULL);
	//IXMLDOMDocumentPtr pDoc;
	HRESULT hr =pDoc.CreateInstance(__uuidof(DOMDocument));

	// Create a processing instruction targeted for xml.
	IXMLDOMProcessingInstructionPtr pi;
	BSTR bstrTg;
	bstrTg = SysAllocString(L"xml");
	BSTR bstrDt;
	bstrDt = SysAllocString(L"version='1.0' encoding='utf-8'");
	pDoc->createProcessingInstruction(bstrTg, bstrDt,&pi);
	if (pi != NULL) 
	{
		AppendChildToParent(pi, pDoc);
		pi.Release();
	}
	/*UF_UI_write_listing_window("进入程序测试3:");*/
	BSTR bstr;
	NXString iString = "特征识别结果列表";

	bstr = ConvertToBSTR(iString);

	IXMLDOMElementPtr pDocElement;
	pDoc->createElement(bstr, &pDocElement);
	AppendChildToParent(pDocElement, pDoc);
	/*UF_UI_write_listing_window("进入程序测试4:");
	
	char mes[13];
	sprintf( mes, "%d\n",1111111);
	UF_UI_write_listing_window("环形槽中向量数:");
	UF_UI_write_listing_window(mes);

	UF_UI_write_listing_window("进入程序测试5:");*/
	////环形槽结点
	//NXString iStringAnnulusCavity = "环形槽";
	//bstr = ConvertToBSTR(iStringAnnulusCavity);

	//IXMLDOMElementPtr pElement_AnnulusCavity;
	//pDoc->createElement(bstr, &pElement_AnnulusCavity);
	//AppendChildToParent(pElement_AnnulusCavity, pDocElement);

	////环形槽特征输出
	//for (std::vector<AnnulusCavity*>::size_type i = 0; i != AnnulusCavitySpyResult.size(); i++)
	//{
	//	
	//	char message[13];
	//	sprintf( message, "%d\n",AnnulusCavitySpyResult.size());
	//	UF_UI_write_listing_window("环形槽中向量数:");
	//	UF_UI_write_listing_window(message);
	//	AnnulusCavity* EachAnnulusCavity = AnnulusCavitySpyResult[i];

	//	int indexI = i + 1;
	//	char* StringIndexI = new char;
	//	sprintf(StringIndexI, "%d", indexI);
	//	NXString NXStringIndexI = StringIndexI;

	//	BSTR bstr_AnnulusCavityFeatureNode;
	//	NXString iStringAnnulusCavityFeatureNode = "环形槽." + NXStringIndexI;

	//	bstr_AnnulusCavityFeatureNode = ConvertToBSTR(iStringAnnulusCavityFeatureNode);

	//	IXMLDOMElementPtr pElement_AnnulusCavityFeatureNode;
	//	pDoc->createElement(bstr_AnnulusCavityFeatureNode, &pElement_AnnulusCavityFeatureNode);
	//	AppendChildToParent(pElement_AnnulusCavityFeatureNode, pElement_AnnulusCavity);

	//	//层
	//	for (std::vector<AnnulusCavityLayer*>::size_type j = 0; j != EachAnnulusCavity->AnnulusCavityLayerVector.size(); j++)
	//	{
	//		AnnulusCavityLayer* EachAnnulusCavityLayer = EachAnnulusCavity->AnnulusCavityLayerVector[j];

	//		int indexJ = j + 1;
	//		char* StringIndexJ = new char;
	//		sprintf(StringIndexJ, "%d", indexJ);
	//		NXString NXStringIndexJ = StringIndexJ;

	//		BSTR bstr_AnnulusCavityLayerNumber;
	//		NXString iStringAnnulusCavityLayerNumber = "层." + NXStringIndexJ;

	//		bstr_AnnulusCavityLayerNumber = ConvertToBSTR(iStringAnnulusCavityLayerNumber);

	//		IXMLDOMElementPtr pElement_AnnulusCavityLayerNode;
	//		pDoc->createElement(bstr_AnnulusCavityLayerNumber, &pElement_AnnulusCavityLayerNode);
	//		AppendChildToParent(pElement_AnnulusCavityLayerNode, pElement_AnnulusCavityFeatureNode);

	//		//环形槽底面
	//		BSTR bstr_AnnulusCavityBottomFaceNode;
	//		NXString iStringAnnulusCavityBottomFaceNode = "底面";

	//		bstr_AnnulusCavityBottomFaceNode = ConvertToBSTR(iStringAnnulusCavityBottomFaceNode);

	//		IXMLDOMElementPtr pElement_AnnulusCavityBottomFaceNode;
	//		pDoc->createElement(bstr_AnnulusCavityBottomFaceNode, &pElement_AnnulusCavityBottomFaceNode);
	//		AppendChildToParent(pElement_AnnulusCavityBottomFaceNode, pElement_AnnulusCavityLayerNode);

	//		for (std::vector<Face*>::size_type k = 0; k != EachAnnulusCavityLayer->BottomFaceVector.size(); k++)
	//		{
	//			Face* EachBottomFace = EachAnnulusCavityLayer->BottomFaceVector[k];

	//			int indexK = k + 1;
	//			char* StringIndexK = new char;
	//			sprintf(StringIndexK, "%d", indexK);
	//			NXString NXStringIndexK = StringIndexK;

	//			BSTR bstr_AnnulusCavityBottomFace;
	//			NXString iStringAnnulusCavityBottomFace = "底面." + NXStringIndexK;

	//			bstr_AnnulusCavityBottomFace = ConvertToBSTR(iStringAnnulusCavityBottomFace);

	//			IXMLDOMElementPtr pElement_AnnulusCavityBottomFace;
	//			pDoc->createElement(bstr_AnnulusCavityBottomFace, &pElement_AnnulusCavityBottomFace);
	//			AppendChildToParent(pElement_AnnulusCavityBottomFace, pElement_AnnulusCavityBottomFaceNode);

	//			char* StringIndexBottomFace = new char;
	//			sprintf(StringIndexBottomFace, "%d", EachBottomFace->Tag());

	//			BSTR bstr_AnnulusCavityBottomFaceTag;
	//			NXString iStringAnnulusCavityBottomFaceTag = StringIndexBottomFace;

	//			bstr_AnnulusCavityBottomFaceTag = ConvertToBSTR(iStringAnnulusCavityBottomFaceTag);
	//			pElement_AnnulusCavityBottomFace->put_text(bstr_AnnulusCavityBottomFaceTag);
	//		}

	//		//环形槽底角面
	//		BSTR bstr_AnnulusCavityBottomCornerFaceNode;
	//		NXString iStringAnnulusCavityBottomCornerFaceNode = "底角面";

	//		bstr_AnnulusCavityBottomCornerFaceNode = ConvertToBSTR(iStringAnnulusCavityBottomCornerFaceNode);

	//		IXMLDOMElementPtr pElement_AnnulusCavityBottomCornerFaceNode;
	//		pDoc->createElement(bstr_AnnulusCavityBottomCornerFaceNode, &pElement_AnnulusCavityBottomCornerFaceNode);
	//		AppendChildToParent(pElement_AnnulusCavityBottomCornerFaceNode, pElement_AnnulusCavityLayerNode);

	//		for (std::vector<Face*>::size_type k = 0; k != EachAnnulusCavityLayer->BottomCornerFaceVector.size(); k++)
	//		{
	//			Face* EachBottomCornerFace = EachAnnulusCavityLayer->BottomCornerFaceVector[k];

	//			int indexK = k + 1;
	//			char* StringIndexK = new char;
	//			sprintf(StringIndexK, "%d", indexK);
	//			NXString NXStringIndexK = StringIndexK;

	//			BSTR bstr_AnnulusCavityBottomCornerFace;
	//			NXString iStringAnnulusCavityBottomCornerFace = "底角面." + NXStringIndexK;

	//			bstr_AnnulusCavityBottomCornerFace = ConvertToBSTR(iStringAnnulusCavityBottomCornerFace);

	//			IXMLDOMElementPtr pElement_AnnulusCavityBottomCornerFace;
	//			pDoc->createElement(bstr_AnnulusCavityBottomCornerFace, &pElement_AnnulusCavityBottomCornerFace);
	//			AppendChildToParent(pElement_AnnulusCavityBottomCornerFace, pElement_AnnulusCavityBottomCornerFaceNode);

	//			char* StringIndexBottomCornerFace = new char;
	//			sprintf(StringIndexBottomCornerFace, "%d", EachBottomCornerFace->Tag());

	//			BSTR bstr_AnnulusCavityBottomCornerFaceTag;
	//			NXString iStringAnnulusCavityBottomCornerFaceTag = StringIndexBottomCornerFace;

	//			bstr_AnnulusCavityBottomCornerFaceTag = ConvertToBSTR(iStringAnnulusCavityBottomCornerFaceTag);
	//			pElement_AnnulusCavityBottomCornerFace->put_text(bstr_AnnulusCavityBottomCornerFaceTag);
	//		}

	//		//环形槽侧面
	//		BSTR bstr_AnnulusCavitySideFaceNode;
	//		NXString iStringAnnulusCavitySideFaceNode = "侧面";

	//		bstr_AnnulusCavitySideFaceNode = ConvertToBSTR(iStringAnnulusCavitySideFaceNode);

	//		IXMLDOMElementPtr pElement_AnnulusCavitySideFaceNode;
	//		pDoc->createElement(bstr_AnnulusCavitySideFaceNode, &pElement_AnnulusCavitySideFaceNode);
	//		AppendChildToParent(pElement_AnnulusCavitySideFaceNode, pElement_AnnulusCavityLayerNode);

	//		for (std::vector<Face*>::size_type k = 0; k != EachAnnulusCavityLayer->SideFaceVector.size(); k++)
	//		{
	//			Face* EachSideFace = EachAnnulusCavityLayer->SideFaceVector[k];

	//			int indexK = k + 1;
	//			char* StringIndexK = new char;
	//			sprintf(StringIndexK, "%d", indexK);
	//			NXString NXStringIndexK = StringIndexK;

	//			BSTR bstr_AnnulusCavitySideFace;
	//			NXString iStringAnnulusCavitySideFace = "侧面." + NXStringIndexK;

	//			bstr_AnnulusCavitySideFace = ConvertToBSTR(iStringAnnulusCavitySideFace);

	//			IXMLDOMElementPtr pElement_AnnulusCavitySideFace;
	//			pDoc->createElement(bstr_AnnulusCavitySideFace, &pElement_AnnulusCavitySideFace);
	//			AppendChildToParent(pElement_AnnulusCavitySideFace, pElement_AnnulusCavitySideFaceNode);

	//			char* StringIndexSideFace = new char;
	//			sprintf(StringIndexSideFace, "%d", EachSideFace->Tag());

	//			BSTR bstr_AnnulusCavitySideFaceTag;
	//			NXString iStringAnnulusCavitySideFaceTag = StringIndexSideFace;

	//			bstr_AnnulusCavitySideFaceTag = ConvertToBSTR(iStringAnnulusCavitySideFaceTag);
	//			pElement_AnnulusCavitySideFace->put_text(bstr_AnnulusCavitySideFaceTag);
	//		}

	//		//环形槽顶面
	//		BSTR bstr_AnnulusCavityTopFaceNode;
	//		NXString iStringAnnulusCavityTopFaceNode = "顶面";

	//		bstr_AnnulusCavityTopFaceNode = ConvertToBSTR(iStringAnnulusCavityTopFaceNode);

	//		IXMLDOMElementPtr pElement_AnnulusCavityTopFaceNode;
	//		pDoc->createElement(bstr_AnnulusCavityTopFaceNode, &pElement_AnnulusCavityTopFaceNode);
	//		AppendChildToParent(pElement_AnnulusCavityTopFaceNode, pElement_AnnulusCavityLayerNode);

	//		for (std::vector<Face*>::size_type k = 0; k != EachAnnulusCavityLayer->TopFaceVector.size(); k++)
	//		{
	//			Face* EachTopFace = EachAnnulusCavityLayer->TopFaceVector[k];

	//			int indexK = k + 1;
	//			char* StringIndexK = new char;
	//			sprintf(StringIndexK, "%d", indexK);
	//			NXString NXStringIndexK = StringIndexK;

	//			BSTR bstr_AnnulusCavityTopFace;
	//			NXString iStringAnnulusCavityTopFace = "顶面." + NXStringIndexK;

	//			bstr_AnnulusCavityTopFace = ConvertToBSTR(iStringAnnulusCavityTopFace);

	//			IXMLDOMElementPtr pElement_AnnulusCavityTopFace;
	//			pDoc->createElement(bstr_AnnulusCavityTopFace, &pElement_AnnulusCavityTopFace);
	//			AppendChildToParent(pElement_AnnulusCavityTopFace, pElement_AnnulusCavityTopFaceNode);

	//			char* StringIndexTopFace = new char;
	//			sprintf(StringIndexTopFace, "%d", EachTopFace->Tag());

	//			BSTR bstr_AnnulusCavityTopFaceTag;
	//			NXString iStringAnnulusCavityTopFaceTag = StringIndexTopFace;

	//			bstr_AnnulusCavityTopFaceTag = ConvertToBSTR(iStringAnnulusCavityTopFaceTag);
	//			pElement_AnnulusCavityTopFace->put_text(bstr_AnnulusCavityTopFaceTag);
	//		}
	//	}
	//}
	////下陷槽结点
	//NXString iStringSubsidenceCavity = "下陷槽";
	//bstr = ConvertToBSTR(iStringSubsidenceCavity);

	//IXMLDOMElementPtr pElement_SubsidenceCavity;
	//pDoc->createElement(bstr, &pElement_SubsidenceCavity);
	//AppendChildToParent(pElement_SubsidenceCavity, pDocElement);

	////下陷槽特征输出
	//for (std::vector<SubsidenceCavity*>::size_type i = 0; i != SubsidenceCavitySpyResult.size(); i++)
	//{
	//	SubsidenceCavity* EachSubsidenceCavity = SubsidenceCavitySpyResult[i];

	//	int indexI = i + 1;
	//	char* StringIndexI = new char;
	//	sprintf(StringIndexI, "%d", indexI);
	//	NXString NXStringIndexI = StringIndexI;

	//	BSTR bstr_SubsidenceCavityFeatureNode;
	//	NXString iStringSubsidenceCavityFeatureNode = "下陷槽." + NXStringIndexI;

	//	bstr_SubsidenceCavityFeatureNode = ConvertToBSTR(iStringSubsidenceCavityFeatureNode);

	//	IXMLDOMElementPtr pElement_SubsidenceCavityFeatureNode;
	//	pDoc->createElement(bstr_SubsidenceCavityFeatureNode, &pElement_SubsidenceCavityFeatureNode);
	//	AppendChildToParent(pElement_SubsidenceCavityFeatureNode, pElement_SubsidenceCavity);

	//	//下陷槽底面
	//	BSTR bstr_SubsidenceCavityBottomFaceNode;
	//	NXString iStringSubsidenceCavityBottomFaceNode = "底面";

	//	bstr_SubsidenceCavityBottomFaceNode = ConvertToBSTR(iStringSubsidenceCavityBottomFaceNode);

	//	IXMLDOMElementPtr pElement_SubsidenceCavityBottomFaceNode;
	//	pDoc->createElement(bstr_SubsidenceCavityBottomFaceNode, &pElement_SubsidenceCavityBottomFaceNode);
	//	AppendChildToParent(pElement_SubsidenceCavityBottomFaceNode, pElement_SubsidenceCavityFeatureNode);

	//	for (std::vector<Face*>::size_type j = 0; j != EachSubsidenceCavity->BottomFaceVector.size(); j++)
	//	{
	//		Face* EachBottomFace = EachSubsidenceCavity->BottomFaceVector[j];

	//		int indexJ = j + 1;
	//		char* StringIndexJ = new char;
	//		sprintf(StringIndexJ, "%d", indexJ);
	//		NXString NXStringIndexJ = StringIndexJ;

	//		BSTR bstr_SubsidenceCavityBottomFace;
	//		NXString iStringSubsidenceCavityBottomFace = "底面." + NXStringIndexJ;

	//		bstr_SubsidenceCavityBottomFace = ConvertToBSTR(iStringSubsidenceCavityBottomFace);

	//		IXMLDOMElementPtr pElement_SubsidenceCavityBottomFace;
	//		pDoc->createElement(bstr_SubsidenceCavityBottomFace, &pElement_SubsidenceCavityBottomFace);
	//		AppendChildToParent(pElement_SubsidenceCavityBottomFace, pElement_SubsidenceCavityBottomFaceNode);

	//		char* StringIndexBottomFace = new char;
	//		sprintf(StringIndexBottomFace, "%d", EachBottomFace->Tag());

	//		BSTR bstr_SubsidenceCavityBottomFaceTag;
	//		NXString iStringSubsidenceCavityBottomFaceTag = StringIndexBottomFace;

	//		bstr_SubsidenceCavityBottomFaceTag = ConvertToBSTR(iStringSubsidenceCavityBottomFaceTag);
	//		pElement_SubsidenceCavityBottomFace->put_text(bstr_SubsidenceCavityBottomFaceTag);
	//	}

	//	//下陷槽底角面
	//	BSTR bstr_SubsidenceCavityBottomCornerFaceNode;
	//	NXString iStringSubsidenceCavityBottomCornerFaceNode = "底角面";

	//	bstr_SubsidenceCavityBottomCornerFaceNode = ConvertToBSTR(iStringSubsidenceCavityBottomCornerFaceNode);

	//	IXMLDOMElementPtr pElement_SubsidenceCavityBottomCornerFaceNode;
	//	pDoc->createElement(bstr_SubsidenceCavityBottomCornerFaceNode, &pElement_SubsidenceCavityBottomCornerFaceNode);
	//	AppendChildToParent(pElement_SubsidenceCavityBottomCornerFaceNode, pElement_SubsidenceCavityFeatureNode);

	//	for (std::vector<Face*>::size_type j = 0; j != EachSubsidenceCavity->BottomCornerFaceVector.size(); j++)
	//	{
	//		Face* EachBottomCornerFace = EachSubsidenceCavity->BottomCornerFaceVector[j];

	//		int indexJ = j + 1;
	//		char* StringIndexJ = new char;
	//		sprintf(StringIndexJ, "%d", indexJ);
	//		NXString NXStringIndexJ = StringIndexJ;

	//		BSTR bstr_SubsidenceCavityBottomCornerFace;
	//		NXString iStringSubsidenceCavityBottomCornerFace = "底角面." + NXStringIndexJ;

	//		bstr_SubsidenceCavityBottomCornerFace = ConvertToBSTR(iStringSubsidenceCavityBottomCornerFace);

	//		IXMLDOMElementPtr pElement_SubsidenceCavityBottomCornerFace;
	//		pDoc->createElement(bstr_SubsidenceCavityBottomCornerFace, &pElement_SubsidenceCavityBottomCornerFace);
	//		AppendChildToParent(pElement_SubsidenceCavityBottomCornerFace, pElement_SubsidenceCavityBottomCornerFaceNode);

	//		char* StringIndexBottomCornerFace = new char;
	//		sprintf(StringIndexBottomCornerFace, "%d", EachBottomCornerFace->Tag());

	//		BSTR bstr_SubsidenceCavityBottomCornerFaceTag;
	//		NXString iStringSubsidenceCavityBottomCornerFaceTag = StringIndexBottomCornerFace;

	//		bstr_SubsidenceCavityBottomCornerFaceTag = ConvertToBSTR(iStringSubsidenceCavityBottomCornerFaceTag);
	//		pElement_SubsidenceCavityBottomCornerFace->put_text(bstr_SubsidenceCavityBottomCornerFaceTag);
	//	}
	//}

	////弧形槽结点
	//NXString iStringArcCavity = "弧形槽";
	//bstr = ConvertToBSTR(iStringArcCavity);

	//IXMLDOMElementPtr pElement_ArcCavity;
	//pDoc->createElement(bstr, &pElement_ArcCavity);
	//AppendChildToParent(pElement_ArcCavity, pDocElement);

	////弧形槽特征输出
	//for (std::vector<ArcCavity*>::size_type i = 0; i != ArcCavitySpyResult.size(); i++)
	//{
	//	ArcCavity* EachSubsidenceCavity = ArcCavitySpyResult[i];

	//	int indexI = i + 1;
	//	char* StringIndexI = new char;
	//	sprintf(StringIndexI, "%d", indexI);
	//	NXString NXStringIndexI = StringIndexI;

	//	BSTR bstr_SubsidenceCavityFeatureNode;
	//	NXString iStringSubsidenceCavityFeatureNode = "弧形槽." + NXStringIndexI;

	//	bstr_SubsidenceCavityFeatureNode = ConvertToBSTR(iStringSubsidenceCavityFeatureNode);

	//	IXMLDOMElementPtr pElement_SubsidenceCavityFeatureNode;
	//	pDoc->createElement(bstr_SubsidenceCavityFeatureNode, &pElement_SubsidenceCavityFeatureNode);
	//	AppendChildToParent(pElement_SubsidenceCavityFeatureNode, pElement_ArcCavity);

	//	//弧形槽底面
	//	BSTR bstr_SubsidenceCavityBottomFaceNode;
	//	NXString iStringSubsidenceCavityBottomFaceNode = "底面";

	//	bstr_SubsidenceCavityBottomFaceNode = ConvertToBSTR(iStringSubsidenceCavityBottomFaceNode);

	//	IXMLDOMElementPtr pElement_SubsidenceCavityBottomFaceNode;
	//	pDoc->createElement(bstr_SubsidenceCavityBottomFaceNode, &pElement_SubsidenceCavityBottomFaceNode);
	//	AppendChildToParent(pElement_SubsidenceCavityBottomFaceNode, pElement_SubsidenceCavityFeatureNode);

	//	for (std::vector<Face*>::size_type j = 0; j != EachSubsidenceCavity->BottomFaceVector.size(); j++)
	//	{
	//		Face* EachBottomFace = EachSubsidenceCavity->BottomFaceVector[j];

	//		int indexJ = j + 1;
	//		char* StringIndexJ = new char;
	//		sprintf(StringIndexJ, "%d", indexJ);
	//		NXString NXStringIndexJ = StringIndexJ;

	//		BSTR bstr_SubsidenceCavityBottomFace;
	//		NXString iStringSubsidenceCavityBottomFace = "底面." + NXStringIndexJ;

	//		bstr_SubsidenceCavityBottomFace = ConvertToBSTR(iStringSubsidenceCavityBottomFace);

	//		IXMLDOMElementPtr pElement_SubsidenceCavityBottomFace;
	//		pDoc->createElement(bstr_SubsidenceCavityBottomFace, &pElement_SubsidenceCavityBottomFace);
	//		AppendChildToParent(pElement_SubsidenceCavityBottomFace, pElement_SubsidenceCavityBottomFaceNode);

	//		char* StringIndexBottomFace = new char;
	//		sprintf(StringIndexBottomFace, "%d", EachBottomFace->Tag());

	//		BSTR bstr_SubsidenceCavityBottomFaceTag;
	//		NXString iStringSubsidenceCavityBottomFaceTag = StringIndexBottomFace;

	//		bstr_SubsidenceCavityBottomFaceTag = ConvertToBSTR(iStringSubsidenceCavityBottomFaceTag);
	//		pElement_SubsidenceCavityBottomFace->put_text(bstr_SubsidenceCavityBottomFaceTag);
	//	}

	//	//弧形槽底角面
	//	BSTR bstr_SubsidenceCavityBottomCornerFaceNode;
	//	NXString iStringSubsidenceCavityBottomCornerFaceNode = "底角面";

	//	bstr_SubsidenceCavityBottomCornerFaceNode = ConvertToBSTR(iStringSubsidenceCavityBottomCornerFaceNode);

	//	IXMLDOMElementPtr pElement_SubsidenceCavityBottomCornerFaceNode;
	//	pDoc->createElement(bstr_SubsidenceCavityBottomCornerFaceNode, &pElement_SubsidenceCavityBottomCornerFaceNode);
	//	AppendChildToParent(pElement_SubsidenceCavityBottomCornerFaceNode, pElement_SubsidenceCavityFeatureNode);

	//	for (std::vector<Face*>::size_type j = 0; j != EachSubsidenceCavity->BottomCornerFaceVector.size(); j++)
	//	{
	//		Face* EachBottomCornerFace = EachSubsidenceCavity->BottomCornerFaceVector[j];

	//		int indexJ = j + 1;
	//		char* StringIndexJ = new char;
	//		sprintf(StringIndexJ, "%d", indexJ);
	//		NXString NXStringIndexJ = StringIndexJ;

	//		BSTR bstr_SubsidenceCavityBottomCornerFace;
	//		NXString iStringSubsidenceCavityBottomCornerFace = "底角面." + NXStringIndexJ;

	//		bstr_SubsidenceCavityBottomCornerFace = ConvertToBSTR(iStringSubsidenceCavityBottomCornerFace);

	//		IXMLDOMElementPtr pElement_SubsidenceCavityBottomCornerFace;
	//		pDoc->createElement(bstr_SubsidenceCavityBottomCornerFace, &pElement_SubsidenceCavityBottomCornerFace);
	//		AppendChildToParent(pElement_SubsidenceCavityBottomCornerFace, pElement_SubsidenceCavityBottomCornerFaceNode);

	//		char* StringIndexBottomCornerFace = new char;
	//		sprintf(StringIndexBottomCornerFace, "%d", EachBottomCornerFace->Tag());

	//		BSTR bstr_SubsidenceCavityBottomCornerFaceTag;
	//		NXString iStringSubsidenceCavityBottomCornerFaceTag = StringIndexBottomCornerFace;

	//		bstr_SubsidenceCavityBottomCornerFaceTag = ConvertToBSTR(iStringSubsidenceCavityBottomCornerFaceTag);
	//		pElement_SubsidenceCavityBottomCornerFace->put_text(bstr_SubsidenceCavityBottomCornerFaceTag);
	//	}
	//}
	//筋结点
	NXString iStringRib = "筋";
	bstr = ConvertToBSTR(iStringRib);

	IXMLDOMElementPtr pElement_Rib;
	pDoc->createElement(bstr, &pElement_Rib);
	AppendChildToParent(pElement_Rib, pDocElement);

	//主轴规则回转筋
	for (std::vector<DirectionRib*>::size_type i = 0; i != DirectionRibSpyResult.size(); i++) //G
	{
		DirectionRib* EachHole = DirectionRibSpyResult[i];//G

		int indexI = i + 1;
		char* StringIndexI = new char;
		sprintf(StringIndexI, "%d", indexI);
		NXString NXStringIndexI = StringIndexI;

		BSTR bstr_HoleFeatureNode;
		NXString iStringHoleFeatureNode = "主轴规则回转筋." + NXStringIndexI;//g

		bstr_HoleFeatureNode = ConvertToBSTR(iStringHoleFeatureNode);

		IXMLDOMElementPtr pElement_HoleFeatureNode;
		pDoc->createElement(bstr_HoleFeatureNode, &pElement_HoleFeatureNode);
		AppendChildToParent(pElement_HoleFeatureNode, pElement_Rib);//接

		//顶面
		BSTR bstr_HoleTopFaceNode;
		NXString iStringHoleTopFaceNode = "顶面";//g

		bstr_HoleTopFaceNode = ConvertToBSTR(iStringHoleTopFaceNode);

		IXMLDOMElementPtr pElement_HoleTopFaceNode;
		pDoc->createElement(bstr_HoleTopFaceNode, &pElement_HoleTopFaceNode);
		AppendChildToParent(pElement_HoleTopFaceNode, pElement_HoleFeatureNode);//接

		for (std::vector<Face*>::size_type j = 0; j != EachHole->TopFaceVector.size(); j++)//g
		{
			Face* EachTopFace = EachHole->TopFaceVector[j];//g

			int indexJ = j + 1;
			char* StringIndexJ = new char;
			sprintf(StringIndexJ, "%d", indexJ);
			NXString NXStringIndexJ = StringIndexJ;

			BSTR bstr_HoleTopFace;
			NXString iStringHoleTopFace = "顶面." + NXStringIndexJ;//g

			bstr_HoleTopFace = ConvertToBSTR(iStringHoleTopFace);

			IXMLDOMElementPtr pElement_HoleTopFace;
			pDoc->createElement(bstr_HoleTopFace, &pElement_HoleTopFace);
			AppendChildToParent(pElement_HoleTopFace, pElement_HoleTopFaceNode);

			char* StringIndexTopFace = new char;
			sprintf(StringIndexTopFace, "%d", EachTopFace->Tag());

			BSTR bstr_HoleTopFaceTag;
			NXString iStringHoleTopFaceTag = StringIndexTopFace;

			bstr_HoleTopFaceTag = ConvertToBSTR(iStringHoleTopFaceTag);
			pElement_HoleTopFace->put_text(bstr_HoleTopFaceTag);
		}

		//约束面
		BSTR bstr_HoleSideFaceNode;
		NXString iStringHoleSideFaceNode = "约束面";//g

		bstr_HoleSideFaceNode = ConvertToBSTR(iStringHoleSideFaceNode);

		IXMLDOMElementPtr pElement_HoleSideFaceNode;
		pDoc->createElement(bstr_HoleSideFaceNode, &pElement_HoleSideFaceNode);
		AppendChildToParent(pElement_HoleSideFaceNode, pElement_HoleFeatureNode);

		for (std::vector<Face*>::size_type j = 0; j != EachHole->LimitFaceVector.size(); j++)//g
		{
			Face* EachSideFace = EachHole->LimitFaceVector[j];//g

			int indexJ = j + 1;
			char* StringIndexJ = new char;
			sprintf(StringIndexJ, "%d", indexJ);
			NXString NXStringIndexJ = StringIndexJ;

			BSTR bstr_HoleSideFace;
			NXString iStringHoleSideFace = "约束面." + NXStringIndexJ;//g

			bstr_HoleSideFace = ConvertToBSTR(iStringHoleSideFace);

			IXMLDOMElementPtr pElement_HoleSideFace;
			pDoc->createElement(bstr_HoleSideFace, &pElement_HoleSideFace);
			AppendChildToParent(pElement_HoleSideFace, pElement_HoleSideFaceNode);//jie for shang

			char* StringIndexSideFace = new char;
			sprintf(StringIndexSideFace, "%d", EachSideFace->Tag());

			BSTR bstr_HoleSideFaceTag;
			NXString iStringHoleSideFaceTag = StringIndexSideFace;

			bstr_HoleSideFaceTag = ConvertToBSTR(iStringHoleSideFaceTag);
			pElement_HoleSideFace->put_text(bstr_HoleSideFaceTag);
		}
	    
		//孔倒角
		/*BSTR bstr_HoleCornerFaceNode;
		NXString iStringHoleCornerFaceNode = "倒角面";

		bstr_HoleCornerFaceNode = ConvertToBSTR(iStringHoleCornerFaceNode);

		IXMLDOMElementPtr pElement_HoleCornerFaceNode;
		pDoc->createElement(bstr_HoleCornerFaceNode, &pElement_HoleCornerFaceNode);
		AppendChildToParent(pElement_HoleCornerFaceNode, pElement_HoleFeatureNode);

		for (std::vector<Face*>::size_type j = 0; j != EachHole->CornerVector.size(); j++)
		{
			Face* EachCornerFace = EachHole->CornerVector[j];

			int indexJ = j + 1;
			char* StringIndexJ = new char;
			sprintf(StringIndexJ, "%d", indexJ);
			NXString NXStringIndexJ = StringIndexJ;

			BSTR bstr_HoleCornerFace;
			NXString iStringHoleCornerFace = "倒角面." + NXStringIndexJ;

			bstr_HoleCornerFace = ConvertToBSTR(iStringHoleCornerFace);

			IXMLDOMElementPtr pElement_HoleCornerFace;
			pDoc->createElement(bstr_HoleCornerFace, &pElement_HoleCornerFace);
			AppendChildToParent(pElement_HoleCornerFace, pElement_HoleCornerFaceNode);

			char* StringIndexCornerFace = new char;
			sprintf(StringIndexCornerFace, "%d", EachCornerFace->Tag());

			BSTR bstr_HoleCornerFaceTag;
			NXString iStringHoleCornerFaceTag = StringIndexCornerFace;

			bstr_HoleCornerFaceTag = ConvertToBSTR(iStringHoleCornerFaceTag);
			pElement_HoleCornerFace->put_text(bstr_HoleCornerFaceTag);
		}*/

		//底角面
		BSTR bstr_HoleBottomFaceNode;
		NXString iStringHoleBottomFaceNode = "底角面";

		bstr_HoleBottomFaceNode = ConvertToBSTR(iStringHoleBottomFaceNode);

		IXMLDOMElementPtr pElement_HoleBottomFaceNode;
		pDoc->createElement(bstr_HoleBottomFaceNode, &pElement_HoleBottomFaceNode);
		AppendChildToParent(pElement_HoleBottomFaceNode, pElement_HoleFeatureNode);

		for (std::vector<Face*>::size_type j = 0; j != EachHole->BottomCornerFaceVector.size(); j++)
		{
			Face* EachBottomFace = EachHole->BottomCornerFaceVector[j];

			int indexJ = j + 1;
			char* StringIndexJ = new char;
			sprintf(StringIndexJ, "%d", indexJ);
			NXString NXStringIndexJ = StringIndexJ;

			BSTR bstr_HoleBottomFace;
			NXString iStringHoleBottomFace = "底角面." + NXStringIndexJ;

			bstr_HoleBottomFace = ConvertToBSTR(iStringHoleBottomFace);

			IXMLDOMElementPtr pElement_HoleBottomFace;
			pDoc->createElement(bstr_HoleBottomFace, &pElement_HoleBottomFace);
			AppendChildToParent(pElement_HoleBottomFace, pElement_HoleBottomFaceNode);

			char* StringIndexBottomFace = new char;

			

			sprintf(StringIndexBottomFace, "%d", EachBottomFace->Tag());

			
			BSTR bstr_HoleBottomFaceTag;
			NXString iStringHoleBottomFaceTag = StringIndexBottomFace;

			bstr_HoleBottomFaceTag = ConvertToBSTR(iStringHoleBottomFaceTag);
			pElement_HoleBottomFace->put_text(bstr_HoleBottomFaceTag);
		}
	}

	//曲面槽筋
	for (std::vector<HookFaceRib*>::size_type i = 0; i != HookFaceRibSpyResult.size(); i++) //G
	{
		HookFaceRib* EachHole = HookFaceRibSpyResult[i];//G

		int indexI = i + 1;
		char* StringIndexI = new char;
		sprintf(StringIndexI, "%d", indexI);
		NXString NXStringIndexI = StringIndexI;

		BSTR bstr_HoleFeatureNode;
		NXString iStringHoleFeatureNode = "曲面槽筋." + NXStringIndexI;//g

		bstr_HoleFeatureNode = ConvertToBSTR(iStringHoleFeatureNode);

		IXMLDOMElementPtr pElement_HoleFeatureNode;
		pDoc->createElement(bstr_HoleFeatureNode, &pElement_HoleFeatureNode);
		AppendChildToParent(pElement_HoleFeatureNode, pElement_Rib);//接

		//顶面
		BSTR bstr_HoleTopFaceNode;
		NXString iStringHoleTopFaceNode = "顶面";//g

		bstr_HoleTopFaceNode = ConvertToBSTR(iStringHoleTopFaceNode);

		IXMLDOMElementPtr pElement_HoleTopFaceNode;
		pDoc->createElement(bstr_HoleTopFaceNode, &pElement_HoleTopFaceNode);
		AppendChildToParent(pElement_HoleTopFaceNode, pElement_HoleFeatureNode);//接

		for (std::vector<Face*>::size_type j = 0; j != EachHole->TopFaceVector.size(); j++)//g
		{
			Face* EachTopFace = EachHole->TopFaceVector[j];//g

			int indexJ = j + 1;
			char* StringIndexJ = new char;
			sprintf(StringIndexJ, "%d", indexJ);
			NXString NXStringIndexJ = StringIndexJ;

			BSTR bstr_HoleTopFace;
			NXString iStringHoleTopFace = "顶面." + NXStringIndexJ;//g

			bstr_HoleTopFace = ConvertToBSTR(iStringHoleTopFace);

			IXMLDOMElementPtr pElement_HoleTopFace;
			pDoc->createElement(bstr_HoleTopFace, &pElement_HoleTopFace);
			AppendChildToParent(pElement_HoleTopFace, pElement_HoleTopFaceNode);

			char* StringIndexTopFace = new char;
			sprintf(StringIndexTopFace, "%d", EachTopFace->Tag());

			BSTR bstr_HoleTopFaceTag;
			NXString iStringHoleTopFaceTag = StringIndexTopFace;

			bstr_HoleTopFaceTag = ConvertToBSTR(iStringHoleTopFaceTag);
			pElement_HoleTopFace->put_text(bstr_HoleTopFaceTag);
		}

		//约束面
		BSTR bstr_HoleSideFaceNode;
		NXString iStringHoleSideFaceNode = "约束面";//g

		bstr_HoleSideFaceNode = ConvertToBSTR(iStringHoleSideFaceNode);

		IXMLDOMElementPtr pElement_HoleSideFaceNode;
		pDoc->createElement(bstr_HoleSideFaceNode, &pElement_HoleSideFaceNode);
		AppendChildToParent(pElement_HoleSideFaceNode, pElement_HoleFeatureNode);

		for (std::vector<Face*>::size_type j = 0; j != EachHole->LimitFaceVector.size(); j++)//g
		{
			Face* EachSideFace = EachHole->LimitFaceVector[j];//g

			int indexJ = j + 1;
			char* StringIndexJ = new char;
			sprintf(StringIndexJ, "%d", indexJ);
			NXString NXStringIndexJ = StringIndexJ;

			BSTR bstr_HoleSideFace;
			NXString iStringHoleSideFace = "约束面." + NXStringIndexJ;//g

			bstr_HoleSideFace = ConvertToBSTR(iStringHoleSideFace);

			IXMLDOMElementPtr pElement_HoleSideFace;
			pDoc->createElement(bstr_HoleSideFace, &pElement_HoleSideFace);
			AppendChildToParent(pElement_HoleSideFace, pElement_HoleSideFaceNode);//jie for shang

			char* StringIndexSideFace = new char;
			sprintf(StringIndexSideFace, "%d", EachSideFace->Tag());

			BSTR bstr_HoleSideFaceTag;
			NXString iStringHoleSideFaceTag = StringIndexSideFace;

			bstr_HoleSideFaceTag = ConvertToBSTR(iStringHoleSideFaceTag);
			pElement_HoleSideFace->put_text(bstr_HoleSideFaceTag);
		}
	    
		//侧面
		BSTR bstr_HoleCornerFaceNode;
		NXString iStringHoleCornerFaceNode = "侧面";

		bstr_HoleCornerFaceNode = ConvertToBSTR(iStringHoleCornerFaceNode);

		IXMLDOMElementPtr pElement_HoleCornerFaceNode;
		pDoc->createElement(bstr_HoleCornerFaceNode, &pElement_HoleCornerFaceNode);
		AppendChildToParent(pElement_HoleCornerFaceNode, pElement_HoleFeatureNode);

		for (std::vector<Face*>::size_type j = 0; j != EachHole->SideFaceVector.size(); j++)
		{
			Face* EachCornerFace = EachHole->SideFaceVector[j];

			int indexJ = j + 1;
			char* StringIndexJ = new char;
			sprintf(StringIndexJ, "%d", indexJ);
			NXString NXStringIndexJ = StringIndexJ;

			BSTR bstr_HoleCornerFace;
			NXString iStringHoleCornerFace = "侧面." + NXStringIndexJ;

			bstr_HoleCornerFace = ConvertToBSTR(iStringHoleCornerFace);

			IXMLDOMElementPtr pElement_HoleCornerFace;
			pDoc->createElement(bstr_HoleCornerFace, &pElement_HoleCornerFace);
			AppendChildToParent(pElement_HoleCornerFace, pElement_HoleCornerFaceNode);

			char* StringIndexCornerFace = new char;
			sprintf(StringIndexCornerFace, "%d", EachCornerFace->Tag());

			BSTR bstr_HoleCornerFaceTag;
			NXString iStringHoleCornerFaceTag = StringIndexCornerFace;

			bstr_HoleCornerFaceTag = ConvertToBSTR(iStringHoleCornerFaceTag);
			pElement_HoleCornerFace->put_text(bstr_HoleCornerFaceTag);
		}

		//底角面
		BSTR bstr_HoleBottomFaceNode;
		NXString iStringHoleBottomFaceNode = "底角面";

		bstr_HoleBottomFaceNode = ConvertToBSTR(iStringHoleBottomFaceNode);

		IXMLDOMElementPtr pElement_HoleBottomFaceNode;
		pDoc->createElement(bstr_HoleBottomFaceNode, &pElement_HoleBottomFaceNode);
		AppendChildToParent(pElement_HoleBottomFaceNode, pElement_HoleFeatureNode);

		for (std::vector<Face*>::size_type j = 0; j != EachHole->BottomCornerFaceVector.size(); j++)
		{
			Face* EachBottomFace = EachHole->BottomCornerFaceVector[j];

			int indexJ = j + 1;
			char* StringIndexJ = new char;
			sprintf(StringIndexJ, "%d", indexJ);
			NXString NXStringIndexJ = StringIndexJ;

			BSTR bstr_HoleBottomFace;
			NXString iStringHoleBottomFace = "底角面." + NXStringIndexJ;

			bstr_HoleBottomFace = ConvertToBSTR(iStringHoleBottomFace);

			IXMLDOMElementPtr pElement_HoleBottomFace;
			pDoc->createElement(bstr_HoleBottomFace, &pElement_HoleBottomFace);
			AppendChildToParent(pElement_HoleBottomFace, pElement_HoleBottomFaceNode);

			char* StringIndexBottomFace = new char;

			

			sprintf(StringIndexBottomFace, "%d", EachBottomFace->Tag());

			
			BSTR bstr_HoleBottomFaceTag;
			NXString iStringHoleBottomFaceTag = StringIndexBottomFace;

			bstr_HoleBottomFaceTag = ConvertToBSTR(iStringHoleBottomFaceTag);
			pElement_HoleBottomFace->put_text(bstr_HoleBottomFaceTag);
		}
	}

//--------------

	//轮廓结点
	NXString iStringOutline = "轮廓";
	bstr = ConvertToBSTR(iStringOutline);

	IXMLDOMElementPtr pElement_Outline;
	pDoc->createElement(bstr, &pElement_Outline);
	AppendChildToParent(pElement_Outline, pDocElement);

	//外轮廓特征
	for (std::vector<Outline*>::size_type i = 0; i != OutlineSpyResult.size(); i++) //G
	{
		Outline* EachHole = OutlineSpyResult[i];//G

		int indexI = i + 1;
		char* StringIndexI = new char;
		sprintf(StringIndexI, "%d", indexI);
		NXString NXStringIndexI = StringIndexI;

		BSTR bstr_HoleFeatureNode;
		NXString iStringHoleFeatureNode = "外轮廓特征." + NXStringIndexI;//g

		bstr_HoleFeatureNode = ConvertToBSTR(iStringHoleFeatureNode);

		IXMLDOMElementPtr pElement_HoleFeatureNode;
		pDoc->createElement(bstr_HoleFeatureNode, &pElement_HoleFeatureNode);
		AppendChildToParent(pElement_HoleFeatureNode, pElement_Outline);//接

		//顶面
		BSTR bstr_HoleTopFaceNode;
		NXString iStringHoleTopFaceNode = "底角面";//g

		bstr_HoleTopFaceNode = ConvertToBSTR(iStringHoleTopFaceNode);

		IXMLDOMElementPtr pElement_HoleTopFaceNode;
		pDoc->createElement(bstr_HoleTopFaceNode, &pElement_HoleTopFaceNode);
		AppendChildToParent(pElement_HoleTopFaceNode, pElement_HoleFeatureNode);//接

		for (std::vector<Face*>::size_type j = 0; j != EachHole->BottomCornerFaceVector.size(); j++)//g
		{
			Face* EachTopFace = EachHole->BottomCornerFaceVector[j];//g

			int indexJ = j + 1;
			char* StringIndexJ = new char;
			sprintf(StringIndexJ, "%d", indexJ);
			NXString NXStringIndexJ = StringIndexJ;

			BSTR bstr_HoleTopFace;
			NXString iStringHoleTopFace = "底角面." + NXStringIndexJ;//g

			bstr_HoleTopFace = ConvertToBSTR(iStringHoleTopFace);

			IXMLDOMElementPtr pElement_HoleTopFace;
			pDoc->createElement(bstr_HoleTopFace, &pElement_HoleTopFace);
			AppendChildToParent(pElement_HoleTopFace, pElement_HoleTopFaceNode);

			char* StringIndexTopFace = new char;
			sprintf(StringIndexTopFace, "%d", EachTopFace->Tag());

			BSTR bstr_HoleTopFaceTag;
			NXString iStringHoleTopFaceTag = StringIndexTopFace;

			bstr_HoleTopFaceTag = ConvertToBSTR(iStringHoleTopFaceTag);
			pElement_HoleTopFace->put_text(bstr_HoleTopFaceTag);
		}

		//平面
		BSTR bstr_HoleSideFaceNode;
		NXString iStringHoleSideFaceNode = "平面";//g

		bstr_HoleSideFaceNode = ConvertToBSTR(iStringHoleSideFaceNode);

		IXMLDOMElementPtr pElement_HoleSideFaceNode;
		pDoc->createElement(bstr_HoleSideFaceNode, &pElement_HoleSideFaceNode);
		AppendChildToParent(pElement_HoleSideFaceNode, pElement_HoleFeatureNode);

		for (std::vector<Face*>::size_type j = 0; j != EachHole->PlanerFaceVector.size(); j++)//g
		{
			Face* EachSideFace = EachHole->PlanerFaceVector[j];//g

			int indexJ = j + 1;
			char* StringIndexJ = new char;
			sprintf(StringIndexJ, "%d", indexJ);
			NXString NXStringIndexJ = StringIndexJ;

			BSTR bstr_HoleSideFace;
			NXString iStringHoleSideFace = "平面." + NXStringIndexJ;//g

			bstr_HoleSideFace = ConvertToBSTR(iStringHoleSideFace);

			IXMLDOMElementPtr pElement_HoleSideFace;
			pDoc->createElement(bstr_HoleSideFace, &pElement_HoleSideFace);
			AppendChildToParent(pElement_HoleSideFace, pElement_HoleSideFaceNode);//jie for shang

			char* StringIndexSideFace = new char;
			sprintf(StringIndexSideFace, "%d", EachSideFace->Tag());

			BSTR bstr_HoleSideFaceTag;
			NXString iStringHoleSideFaceTag = StringIndexSideFace;

			bstr_HoleSideFaceTag = ConvertToBSTR(iStringHoleSideFaceTag);
			pElement_HoleSideFace->put_text(bstr_HoleSideFaceTag);
		}
	    
		//孔倒角
		/*BSTR bstr_HoleCornerFaceNode;
		NXString iStringHoleCornerFaceNode = "倒角面";

		bstr_HoleCornerFaceNode = ConvertToBSTR(iStringHoleCornerFaceNode);

		IXMLDOMElementPtr pElement_HoleCornerFaceNode;
		pDoc->createElement(bstr_HoleCornerFaceNode, &pElement_HoleCornerFaceNode);
		AppendChildToParent(pElement_HoleCornerFaceNode, pElement_HoleFeatureNode);

		for (std::vector<Face*>::size_type j = 0; j != EachHole->CornerVector.size(); j++)
		{
			Face* EachCornerFace = EachHole->CornerVector[j];

			int indexJ = j + 1;
			char* StringIndexJ = new char;
			sprintf(StringIndexJ, "%d", indexJ);
			NXString NXStringIndexJ = StringIndexJ;

			BSTR bstr_HoleCornerFace;
			NXString iStringHoleCornerFace = "倒角面." + NXStringIndexJ;

			bstr_HoleCornerFace = ConvertToBSTR(iStringHoleCornerFace);

			IXMLDOMElementPtr pElement_HoleCornerFace;
			pDoc->createElement(bstr_HoleCornerFace, &pElement_HoleCornerFace);
			AppendChildToParent(pElement_HoleCornerFace, pElement_HoleCornerFaceNode);

			char* StringIndexCornerFace = new char;
			sprintf(StringIndexCornerFace, "%d", EachCornerFace->Tag());

			BSTR bstr_HoleCornerFaceTag;
			NXString iStringHoleCornerFaceTag = StringIndexCornerFace;

			bstr_HoleCornerFaceTag = ConvertToBSTR(iStringHoleCornerFaceTag);
			pElement_HoleCornerFace->put_text(bstr_HoleCornerFaceTag);
		}*/

		//轮廓锥面
		BSTR bstr_HoleBottomFaceNode;
		NXString iStringHoleBottomFaceNode = "轮廓锥面";

		bstr_HoleBottomFaceNode = ConvertToBSTR(iStringHoleBottomFaceNode);

		IXMLDOMElementPtr pElement_HoleBottomFaceNode;
		pDoc->createElement(bstr_HoleBottomFaceNode, &pElement_HoleBottomFaceNode);
		AppendChildToParent(pElement_HoleBottomFaceNode, pElement_HoleFeatureNode);

		for (std::vector<Face*>::size_type j = 0; j != EachHole->ConicalFaceVector.size(); j++)//g
		{
			Face* EachBottomFace = EachHole->ConicalFaceVector[j];//g

			int indexJ = j + 1;
			char* StringIndexJ = new char;
			sprintf(StringIndexJ, "%d", indexJ);
			NXString NXStringIndexJ = StringIndexJ;

			BSTR bstr_HoleBottomFace;
			NXString iStringHoleBottomFace = "轮廓锥面." + NXStringIndexJ;

			bstr_HoleBottomFace = ConvertToBSTR(iStringHoleBottomFace);

			IXMLDOMElementPtr pElement_HoleBottomFace;
			pDoc->createElement(bstr_HoleBottomFace, &pElement_HoleBottomFace);
			AppendChildToParent(pElement_HoleBottomFace, pElement_HoleBottomFaceNode);

			char* StringIndexBottomFace = new char;

			

			sprintf(StringIndexBottomFace, "%d", EachBottomFace->Tag());

			
			BSTR bstr_HoleBottomFaceTag;
			NXString iStringHoleBottomFaceTag = StringIndexBottomFace;

			bstr_HoleBottomFaceTag = ConvertToBSTR(iStringHoleBottomFaceTag);
			pElement_HoleBottomFace->put_text(bstr_HoleBottomFaceTag);
		}
	}
	//内轮廓特征
	for (std::vector<Outline*>::size_type i = 0; i != Outline1SpyResult.size(); i++) //G
	{
		Outline* EachHole = Outline1SpyResult[i];//G

		int indexI = i + 1;
		char* StringIndexI = new char;
		sprintf(StringIndexI, "%d", indexI);
		NXString NXStringIndexI = StringIndexI;

		BSTR bstr_HoleFeatureNode;
		NXString iStringHoleFeatureNode = "内轮廓特征." + NXStringIndexI;//g

		bstr_HoleFeatureNode = ConvertToBSTR(iStringHoleFeatureNode);

		IXMLDOMElementPtr pElement_HoleFeatureNode;
		pDoc->createElement(bstr_HoleFeatureNode, &pElement_HoleFeatureNode);
		AppendChildToParent(pElement_HoleFeatureNode, pElement_Outline);//接

		//顶面
		BSTR bstr_HoleTopFaceNode;
		NXString iStringHoleTopFaceNode = "顶面";//g

		bstr_HoleTopFaceNode = ConvertToBSTR(iStringHoleTopFaceNode);

		IXMLDOMElementPtr pElement_HoleTopFaceNode;
		pDoc->createElement(bstr_HoleTopFaceNode, &pElement_HoleTopFaceNode);
		AppendChildToParent(pElement_HoleTopFaceNode, pElement_HoleFeatureNode);//接

		for (std::vector<Face*>::size_type j = 0; j != EachHole->TopFaceVector.size(); j++)//g
		{
			Face* EachTopFace = EachHole->TopFaceVector[j];//g

			int indexJ = j + 1;
			char* StringIndexJ = new char;
			sprintf(StringIndexJ, "%d", indexJ);
			NXString NXStringIndexJ = StringIndexJ;

			BSTR bstr_HoleTopFace;
			NXString iStringHoleTopFace = "顶面." + NXStringIndexJ;//g

			bstr_HoleTopFace = ConvertToBSTR(iStringHoleTopFace);

			IXMLDOMElementPtr pElement_HoleTopFace;
			pDoc->createElement(bstr_HoleTopFace, &pElement_HoleTopFace);
			AppendChildToParent(pElement_HoleTopFace, pElement_HoleTopFaceNode);

			char* StringIndexTopFace = new char;
			sprintf(StringIndexTopFace, "%d", EachTopFace->Tag());

			BSTR bstr_HoleTopFaceTag;
			NXString iStringHoleTopFaceTag = StringIndexTopFace;

			bstr_HoleTopFaceTag = ConvertToBSTR(iStringHoleTopFaceTag);
			pElement_HoleTopFace->put_text(bstr_HoleTopFaceTag);
		}

		//倒角面
		BSTR bstr_HoleSideFaceNode;
		NXString iStringHoleSideFaceNode = "倒角面";//g

		bstr_HoleSideFaceNode = ConvertToBSTR(iStringHoleSideFaceNode);

		IXMLDOMElementPtr pElement_HoleSideFaceNode;
		pDoc->createElement(bstr_HoleSideFaceNode, &pElement_HoleSideFaceNode);
		AppendChildToParent(pElement_HoleSideFaceNode, pElement_HoleFeatureNode);

		for (std::vector<Face*>::size_type j = 0; j != EachHole->RightFaceVector.size(); j++)//g
		{
			Face* EachSideFace = EachHole->RightFaceVector[j];//g

			int indexJ = j + 1;
			char* StringIndexJ = new char;
			sprintf(StringIndexJ, "%d", indexJ);
			NXString NXStringIndexJ = StringIndexJ;

			BSTR bstr_HoleSideFace;
			NXString iStringHoleSideFace = "倒角面." + NXStringIndexJ;//g

			bstr_HoleSideFace = ConvertToBSTR(iStringHoleSideFace);

			IXMLDOMElementPtr pElement_HoleSideFace;
			pDoc->createElement(bstr_HoleSideFace, &pElement_HoleSideFace);
			AppendChildToParent(pElement_HoleSideFace, pElement_HoleSideFaceNode);//jie for shang

			char* StringIndexSideFace = new char;
			sprintf(StringIndexSideFace, "%d", EachSideFace->Tag());

			BSTR bstr_HoleSideFaceTag;
			NXString iStringHoleSideFaceTag = StringIndexSideFace;

			bstr_HoleSideFaceTag = ConvertToBSTR(iStringHoleSideFaceTag);
			pElement_HoleSideFace->put_text(bstr_HoleSideFaceTag);
		}
	    
		//侧面
		BSTR bstr_HoleCornerFaceNode;
		NXString iStringHoleCornerFaceNode = "侧面";

		bstr_HoleCornerFaceNode = ConvertToBSTR(iStringHoleCornerFaceNode);

		IXMLDOMElementPtr pElement_HoleCornerFaceNode;
		pDoc->createElement(bstr_HoleCornerFaceNode, &pElement_HoleCornerFaceNode);
		AppendChildToParent(pElement_HoleCornerFaceNode, pElement_HoleFeatureNode);

		for (std::vector<Face*>::size_type j = 0; j != EachHole->SideFaceVector.size(); j++)
		{
			Face* EachCornerFace = EachHole->SideFaceVector[j];

			int indexJ = j + 1;
			char* StringIndexJ = new char;
			sprintf(StringIndexJ, "%d", indexJ);
			NXString NXStringIndexJ = StringIndexJ;

			BSTR bstr_HoleCornerFace;
			NXString iStringHoleCornerFace = "侧面." + NXStringIndexJ;

			bstr_HoleCornerFace = ConvertToBSTR(iStringHoleCornerFace);

			IXMLDOMElementPtr pElement_HoleCornerFace;
			pDoc->createElement(bstr_HoleCornerFace, &pElement_HoleCornerFace);
			AppendChildToParent(pElement_HoleCornerFace, pElement_HoleCornerFaceNode);

			char* StringIndexCornerFace = new char;
			sprintf(StringIndexCornerFace, "%d", EachCornerFace->Tag());

			BSTR bstr_HoleCornerFaceTag;
			NXString iStringHoleCornerFaceTag = StringIndexCornerFace;

			bstr_HoleCornerFaceTag = ConvertToBSTR(iStringHoleCornerFaceTag);
			pElement_HoleCornerFace->put_text(bstr_HoleCornerFaceTag);
		}

		//底角面
		BSTR bstr_HoleBottomFaceNode;
		NXString iStringHoleBottomFaceNode = "底角面";

		bstr_HoleBottomFaceNode = ConvertToBSTR(iStringHoleBottomFaceNode);

		IXMLDOMElementPtr pElement_HoleBottomFaceNode;
		pDoc->createElement(bstr_HoleBottomFaceNode, &pElement_HoleBottomFaceNode);
		AppendChildToParent(pElement_HoleBottomFaceNode, pElement_HoleFeatureNode);

		for (std::vector<Face*>::size_type j = 0; j != EachHole->BottomCornerFaceVector.size(); j++)//g
		{
			Face* EachBottomFace = EachHole->BottomCornerFaceVector[j];//g

			int indexJ = j + 1;
			char* StringIndexJ = new char;
			sprintf(StringIndexJ, "%d", indexJ);
			NXString NXStringIndexJ = StringIndexJ;

			BSTR bstr_HoleBottomFace;
			NXString iStringHoleBottomFace = "底角面." + NXStringIndexJ;

			bstr_HoleBottomFace = ConvertToBSTR(iStringHoleBottomFace);

			IXMLDOMElementPtr pElement_HoleBottomFace;
			pDoc->createElement(bstr_HoleBottomFace, &pElement_HoleBottomFace);
			AppendChildToParent(pElement_HoleBottomFace, pElement_HoleBottomFaceNode);

			char* StringIndexBottomFace = new char;

			

			sprintf(StringIndexBottomFace, "%d", EachBottomFace->Tag());

			
			BSTR bstr_HoleBottomFaceTag;
			NXString iStringHoleBottomFaceTag = StringIndexBottomFace;

			bstr_HoleBottomFaceTag = ConvertToBSTR(iStringHoleBottomFaceTag);
			pElement_HoleBottomFace->put_text(bstr_HoleBottomFaceTag);
		}
	}


//--------------

	//凸台结点
	NXString iStringBoss = "凸台";
	bstr = ConvertToBSTR(iStringBoss);

	IXMLDOMElementPtr pElement_Boss;
	pDoc->createElement(bstr, &pElement_Boss);
	AppendChildToParent(pElement_Boss, pDocElement);
	//多层凸台特征输出										
	for (std::vector<DoubleLayerBoss*>::size_type i = 0; i != DoubleLayerBossSpyResult.size(); i++)
	{
		DoubleLayerBoss* EachBoss = DoubleLayerBossSpyResult[i];
		int indexI = i + 1;
		char* StringIndexI = new char;
		sprintf(StringIndexI, "%d", indexI);
		NXString NXStringIndexI = StringIndexI;

		BSTR bstr_BossFeatureNode;
		NXString iStringAnnulusCavityFeatureNode;
		if(EachBoss->OutOrIn==0)
		{
			iStringAnnulusCavityFeatureNode = "内侧-多层凸台." + NXStringIndexI;
		}
		if(EachBoss->OutOrIn==1)
		{
			iStringAnnulusCavityFeatureNode = "外侧-多层凸台." + NXStringIndexI;
		}

		bstr_BossFeatureNode = ConvertToBSTR(iStringAnnulusCavityFeatureNode);

		IXMLDOMElementPtr pElement_BossFeatureNode;
		pDoc->createElement(bstr_BossFeatureNode, &pElement_BossFeatureNode);
		AppendChildToParent(pElement_BossFeatureNode, pElement_Boss);


		//凸台底角面
		BSTR bstr_BossBottomCornerFaceNode;
		NXString iStringBossBottomCornerFaceNode = "底角面";

		bstr_BossBottomCornerFaceNode = ConvertToBSTR(iStringBossBottomCornerFaceNode);

		IXMLDOMElementPtr pElement_BossBottomCornerFaceNode;

		pDoc->createElement(bstr_BossBottomCornerFaceNode, &pElement_BossBottomCornerFaceNode);

		AppendChildToParent(pElement_BossBottomCornerFaceNode, pElement_BossFeatureNode);

		for (std::vector<Face*>::size_type j = 0; j != EachBoss->BottomCornerFaceVector.size(); j++)
		{

			Face* EachBottomCornerFace = EachBoss->BottomCornerFaceVector[j];

			int indexJ = j + 1;
			char* StringIndexJ = new char;
			sprintf(StringIndexJ, "%d", indexJ);
			NXString NXStringIndexJ = StringIndexJ;

			BSTR bstr_BossBottomCornerFace;
			NXString iStringBossBottomCornerFace = "底角面." + NXStringIndexJ;

			bstr_BossBottomCornerFace = ConvertToBSTR(iStringBossBottomCornerFace);

			IXMLDOMElementPtr pElement_BossBottomCornerFace;
			pDoc->createElement(bstr_BossBottomCornerFace, &pElement_BossBottomCornerFace);
			AppendChildToParent(pElement_BossBottomCornerFace, pElement_BossBottomCornerFaceNode);

			char* StringIndexBottomCornerFace = new char;
			sprintf(StringIndexBottomCornerFace, "%d", EachBottomCornerFace->Tag());

			BSTR bstr_BossBottomCornerFaceTag;
			NXString iStringBossBottomCornerFaceTag = StringIndexBottomCornerFace;

			bstr_BossBottomCornerFaceTag = ConvertToBSTR(iStringBossBottomCornerFaceTag);
			pElement_BossBottomCornerFace->put_text(bstr_BossBottomCornerFaceTag);
		}

		//凸台侧面
		BSTR bstr_BossSideFaceNode;
		NXString iStringBossSideFaceNode = "侧面";

		bstr_BossSideFaceNode = ConvertToBSTR(iStringBossSideFaceNode);

		IXMLDOMElementPtr pElement_BossSideFaceNode;
		pDoc->createElement(bstr_BossSideFaceNode, &pElement_BossSideFaceNode);
		AppendChildToParent(pElement_BossSideFaceNode, pElement_BossFeatureNode);

		for (std::vector<Face*>::size_type j = 0; j != EachBoss->SideFaceVector.size(); j++)
		{
			Face* EachSideFace = EachBoss->SideFaceVector[j];

			int indexJ = j + 1;
			char* StringIndexJ = new char;
			sprintf(StringIndexJ, "%d", indexJ);
			NXString NXStringIndexJ = StringIndexJ;

			BSTR bstr_BossSideFace;
			NXString iStringBossSideFace = "侧面." + NXStringIndexJ;

			bstr_BossSideFace = ConvertToBSTR(iStringBossSideFace);

			IXMLDOMElementPtr pElement_BossSideFace;
			pDoc->createElement(bstr_BossSideFace, &pElement_BossSideFace);
			AppendChildToParent(pElement_BossSideFace, pElement_BossSideFaceNode);

			char* StringIndexSideFace = new char;
			sprintf(StringIndexSideFace, "%d", EachSideFace->Tag());

			BSTR bstr_BossSideFaceTag;
			NXString iStringBossSideFaceTag = StringIndexSideFace;

			bstr_BossSideFaceTag = ConvertToBSTR(iStringBossSideFaceTag);
			pElement_BossSideFace->put_text(bstr_BossSideFaceTag);
		}

		//凸台顶面
		BSTR bstr_BossTopFaceNode;
		NXString iStringBossTopFaceNode = "顶面";

		bstr_BossTopFaceNode = ConvertToBSTR(iStringBossTopFaceNode);

		IXMLDOMElementPtr pElement_BossTopFaceNode;
		pDoc->createElement(bstr_BossTopFaceNode, &pElement_BossTopFaceNode);
		AppendChildToParent(pElement_BossTopFaceNode, pElement_BossFeatureNode);

		for (std::vector<Face*>::size_type j = 0; j != EachBoss->TopFaceVector.size(); j++)
		{
			Face* EachTopFace = EachBoss->TopFaceVector[j];

			int indexJ = j + 1;
			char* StringIndexJ = new char;
			sprintf(StringIndexJ, "%d", indexJ);
			NXString NXStringIndexJ = StringIndexJ;

			BSTR bstr_BossTopFace;
			NXString iStringBossTopFace = "顶面." + NXStringIndexJ;

			bstr_BossTopFace = ConvertToBSTR(iStringBossTopFace);

			IXMLDOMElementPtr pElement_BossTopFace;
			pDoc->createElement(bstr_BossTopFace, &pElement_BossTopFace);
			AppendChildToParent(pElement_BossTopFace, pElement_BossTopFaceNode);

			char* StringIndexTopFace = new char;
			sprintf(StringIndexTopFace, "%d", EachTopFace->Tag());

			BSTR bstr_BossTopFaceTag;
			NXString iStringBossTopFaceTag = StringIndexTopFace;

			bstr_BossTopFaceTag = ConvertToBSTR(iStringBossTopFaceTag);
			pElement_BossTopFace->put_text(bstr_BossTopFaceTag);
		}

		//凸台次顶面
		BSTR bstr_BossNTopFaceNode;
		NXString iStringBossNTopFaceNode = "次顶面";

		bstr_BossNTopFaceNode = ConvertToBSTR(iStringBossNTopFaceNode);

		IXMLDOMElementPtr pElement_BossNTopFaceNode;
		pDoc->createElement(bstr_BossNTopFaceNode, &pElement_BossNTopFaceNode);
		AppendChildToParent(pElement_BossNTopFaceNode, pElement_BossFeatureNode);

		for (std::vector<Face*>::size_type j = 0; j != EachBoss->NTopFaceVector.size(); j++)
		{
			Face* EachTopFace = EachBoss->NTopFaceVector[j];

			int indexJ = j + 1;
			char* StringIndexJ = new char;
			sprintf(StringIndexJ, "%d", indexJ);
			NXString NXStringIndexJ = StringIndexJ;

			BSTR bstr_BossTopFace;
			NXString iStringBossTopFace = "次顶面." + NXStringIndexJ;

			bstr_BossTopFace = ConvertToBSTR(iStringBossTopFace);

			IXMLDOMElementPtr pElement_BossTopFace;
			pDoc->createElement(bstr_BossTopFace, &pElement_BossTopFace);
			AppendChildToParent(pElement_BossTopFace, pElement_BossNTopFaceNode);//结点连接处要改

			char* StringIndexTopFace = new char;
			sprintf(StringIndexTopFace, "%d", EachTopFace->Tag());

			BSTR bstr_BossTopFaceTag;
			NXString iStringBossTopFaceTag = StringIndexTopFace;

			bstr_BossTopFaceTag = ConvertToBSTR(iStringBossTopFaceTag);
			pElement_BossTopFace->put_text(bstr_BossTopFaceTag);
		}
		//凸台次侧面
		BSTR bstr_BossNSideFaceNode;
		NXString iStringBossNSideFaceNode = "次侧面";

		bstr_BossNSideFaceNode = ConvertToBSTR(iStringBossNSideFaceNode);

		IXMLDOMElementPtr pElement_BossNSideFaceNode;
		pDoc->createElement(bstr_BossNSideFaceNode, &pElement_BossNSideFaceNode);
		AppendChildToParent(pElement_BossNSideFaceNode, pElement_BossFeatureNode);

		for (std::vector<Face*>::size_type j = 0; j != EachBoss->NSideFaceVector.size(); j++)
		{
			Face* EachTopFace = EachBoss->NSideFaceVector[j];

			int indexJ = j + 1;
			char* StringIndexJ = new char;
			sprintf(StringIndexJ, "%d", indexJ);
			NXString NXStringIndexJ = StringIndexJ;

			BSTR bstr_BossTopFace;
			NXString iStringBossTopFace = "次侧面." + NXStringIndexJ;

			bstr_BossTopFace = ConvertToBSTR(iStringBossTopFace);

			IXMLDOMElementPtr pElement_BossTopFace;
			pDoc->createElement(bstr_BossTopFace, &pElement_BossTopFace);
			AppendChildToParent(pElement_BossTopFace, pElement_BossNSideFaceNode);

			char* StringIndexTopFace = new char;
			sprintf(StringIndexTopFace, "%d", EachTopFace->Tag());

			BSTR bstr_BossTopFaceTag;
			NXString iStringBossTopFaceTag = StringIndexTopFace;

			bstr_BossTopFaceTag = ConvertToBSTR(iStringBossTopFaceTag);
			pElement_BossTopFace->put_text(bstr_BossTopFaceTag);
		}

		//凸台连接面
		BSTR bstr_BossConnectFaceNode;
		NXString iStringBossConnectFaceNode = "连接面";

		bstr_BossConnectFaceNode = ConvertToBSTR(iStringBossConnectFaceNode);

		IXMLDOMElementPtr pElement_BossConnectFaceNode;
		pDoc->createElement(bstr_BossConnectFaceNode, &pElement_BossConnectFaceNode);
		AppendChildToParent(pElement_BossConnectFaceNode, pElement_BossFeatureNode);

		for (std::vector<Face*>::size_type j = 0; j != EachBoss->ConnectFaceVector.size(); j++)
		{
			Face* EachTopFace = EachBoss->ConnectFaceVector[j];

			int indexJ = j + 1;
			char* StringIndexJ = new char;
			sprintf(StringIndexJ, "%d", indexJ);
			NXString NXStringIndexJ = StringIndexJ;

			BSTR bstr_BossTopFace;
			NXString iStringBossTopFace = "连接面." + NXStringIndexJ;

			bstr_BossTopFace = ConvertToBSTR(iStringBossTopFace);

			IXMLDOMElementPtr pElement_BossTopFace;
			pDoc->createElement(bstr_BossTopFace, &pElement_BossTopFace);
			AppendChildToParent(pElement_BossTopFace, pElement_BossConnectFaceNode);

			char* StringIndexTopFace = new char;
			sprintf(StringIndexTopFace, "%d", EachTopFace->Tag());

			BSTR bstr_BossTopFaceTag;
			NXString iStringBossTopFaceTag = StringIndexTopFace;

			bstr_BossTopFaceTag = ConvertToBSTR(iStringBossTopFaceTag);
			pElement_BossTopFace->put_text(bstr_BossTopFaceTag);
		}
		
	}

	//无侧面凸台特征输出
	for (std::vector<NoSideBoss*>::size_type i = 0; i != NoSideBossSpyResult.size(); i++)
	{
		NoSideBoss* EachBoss = NoSideBossSpyResult[i];
		int indexI = i + 1;
		char* StringIndexI = new char;
		sprintf(StringIndexI, "%d", indexI);
		NXString NXStringIndexI = StringIndexI;

		BSTR bstr_BossFeatureNode;
		NXString iStringAnnulusCavityFeatureNode;
		if(EachBoss->OutOrIn==0)
		{
			iStringAnnulusCavityFeatureNode = "内侧-无侧面凸台." + NXStringIndexI;
		}
		if(EachBoss->OutOrIn==1)
		{
			iStringAnnulusCavityFeatureNode = "外侧-无侧面凸台." + NXStringIndexI;
		}

		bstr_BossFeatureNode = ConvertToBSTR(iStringAnnulusCavityFeatureNode);

		IXMLDOMElementPtr pElement_BossFeatureNode;
		pDoc->createElement(bstr_BossFeatureNode, &pElement_BossFeatureNode);
		AppendChildToParent(pElement_BossFeatureNode, pElement_Boss);


		//凸台底角面
		BSTR bstr_BossBottomCornerFaceNode;
		NXString iStringBossBottomCornerFaceNode = "底角面";

		bstr_BossBottomCornerFaceNode = ConvertToBSTR(iStringBossBottomCornerFaceNode);

		IXMLDOMElementPtr pElement_BossBottomCornerFaceNode;

		pDoc->createElement(bstr_BossBottomCornerFaceNode, &pElement_BossBottomCornerFaceNode);

		AppendChildToParent(pElement_BossBottomCornerFaceNode, pElement_BossFeatureNode);

		for (std::vector<Face*>::size_type j = 0; j != EachBoss->BottomCornerFaceVector.size(); j++)
		{

			Face* EachBottomCornerFace = EachBoss->BottomCornerFaceVector[j];

			int indexJ = j + 1;
			char* StringIndexJ = new char;
			sprintf(StringIndexJ, "%d", indexJ);
			NXString NXStringIndexJ = StringIndexJ;

			BSTR bstr_BossBottomCornerFace;
			NXString iStringBossBottomCornerFace = "底角面." + NXStringIndexJ;

			bstr_BossBottomCornerFace = ConvertToBSTR(iStringBossBottomCornerFace);

			IXMLDOMElementPtr pElement_BossBottomCornerFace;
			pDoc->createElement(bstr_BossBottomCornerFace, &pElement_BossBottomCornerFace);
			AppendChildToParent(pElement_BossBottomCornerFace, pElement_BossBottomCornerFaceNode);

			char* StringIndexBottomCornerFace = new char;
			sprintf(StringIndexBottomCornerFace, "%d", EachBottomCornerFace->Tag());

			BSTR bstr_BossBottomCornerFaceTag;
			NXString iStringBossBottomCornerFaceTag = StringIndexBottomCornerFace;

			bstr_BossBottomCornerFaceTag = ConvertToBSTR(iStringBossBottomCornerFaceTag);
			pElement_BossBottomCornerFace->put_text(bstr_BossBottomCornerFaceTag);
		}

		//凸台顶面
		BSTR bstr_BossTopFaceNode;
		NXString iStringBossTopFaceNode = "顶面";

		bstr_BossTopFaceNode = ConvertToBSTR(iStringBossTopFaceNode);

		IXMLDOMElementPtr pElement_BossTopFaceNode;
		pDoc->createElement(bstr_BossTopFaceNode, &pElement_BossTopFaceNode);
		AppendChildToParent(pElement_BossTopFaceNode, pElement_BossFeatureNode);

		for (std::vector<Face*>::size_type j = 0; j != EachBoss->TopFaceVector.size(); j++)
		{
			Face* EachTopFace = EachBoss->TopFaceVector[j];

			int indexJ = j + 1;
			char* StringIndexJ = new char;
			sprintf(StringIndexJ, "%d", indexJ);
			NXString NXStringIndexJ = StringIndexJ;

			BSTR bstr_BossTopFace;
			NXString iStringBossTopFace = "顶面." + NXStringIndexJ;

			bstr_BossTopFace = ConvertToBSTR(iStringBossTopFace);

			IXMLDOMElementPtr pElement_BossTopFace;
			pDoc->createElement(bstr_BossTopFace, &pElement_BossTopFace);
			AppendChildToParent(pElement_BossTopFace, pElement_BossTopFaceNode);

			char* StringIndexTopFace = new char;
			sprintf(StringIndexTopFace, "%d", EachTopFace->Tag());

			BSTR bstr_BossTopFaceTag;
			NXString iStringBossTopFaceTag = StringIndexTopFace;

			bstr_BossTopFaceTag = ConvertToBSTR(iStringBossTopFaceTag);
			pElement_BossTopFace->put_text(bstr_BossTopFaceTag);
		}
	}


	//含侧面凸台特征输出
	for (std::vector<NormalBoss*>::size_type i = 0; i != NormalBossSpyResult.size(); i++)
	{
		NormalBoss* EachBoss = NormalBossSpyResult[i];
		int indexI = i + 1;
		char* StringIndexI = new char;
		sprintf(StringIndexI, "%d", indexI);
		NXString NXStringIndexI = StringIndexI;

		BSTR bstr_BossFeatureNode;
		NXString iStringAnnulusCavityFeatureNode;
		if(EachBoss->OutOrIn==0)
		{
			iStringAnnulusCavityFeatureNode = "内侧-含侧面凸台." + NXStringIndexI;
		}
		if(EachBoss->OutOrIn==1)
		{
			iStringAnnulusCavityFeatureNode = "外侧-含侧面凸台." + NXStringIndexI;
		}
		
		bstr_BossFeatureNode = ConvertToBSTR(iStringAnnulusCavityFeatureNode);

		IXMLDOMElementPtr pElement_BossFeatureNode;
		pDoc->createElement(bstr_BossFeatureNode, &pElement_BossFeatureNode);
		AppendChildToParent(pElement_BossFeatureNode, pElement_Boss);
		

		//凸台底角面
		BSTR bstr_BossBottomCornerFaceNode;
		NXString iStringBossBottomCornerFaceNode = "底角面";
		
		bstr_BossBottomCornerFaceNode = ConvertToBSTR(iStringBossBottomCornerFaceNode);
		
		IXMLDOMElementPtr pElement_BossBottomCornerFaceNode;
		
		pDoc->createElement(bstr_BossBottomCornerFaceNode, &pElement_BossBottomCornerFaceNode);
		
		AppendChildToParent(pElement_BossBottomCornerFaceNode, pElement_BossFeatureNode);
		
		for (std::vector<Face*>::size_type j = 0; j != EachBoss->BottomCornerFaceVector.size(); j++)
		{
			
			Face* EachBottomCornerFace = EachBoss->BottomCornerFaceVector[j];

			int indexJ = j + 1;
			char* StringIndexJ = new char;
			sprintf(StringIndexJ, "%d", indexJ);
			NXString NXStringIndexJ = StringIndexJ;

			BSTR bstr_BossBottomCornerFace;
			NXString iStringBossBottomCornerFace = "底角面." + NXStringIndexJ;

			bstr_BossBottomCornerFace = ConvertToBSTR(iStringBossBottomCornerFace);

			IXMLDOMElementPtr pElement_BossBottomCornerFace;
			pDoc->createElement(bstr_BossBottomCornerFace, &pElement_BossBottomCornerFace);
			AppendChildToParent(pElement_BossBottomCornerFace, pElement_BossBottomCornerFaceNode);

			char* StringIndexBottomCornerFace = new char;
			sprintf(StringIndexBottomCornerFace, "%d", EachBottomCornerFace->Tag());

			BSTR bstr_BossBottomCornerFaceTag;
			NXString iStringBossBottomCornerFaceTag = StringIndexBottomCornerFace;

			bstr_BossBottomCornerFaceTag = ConvertToBSTR(iStringBossBottomCornerFaceTag);
			pElement_BossBottomCornerFace->put_text(bstr_BossBottomCornerFaceTag);
		}

		//凸台侧面
		BSTR bstr_BossSideFaceNode;
		NXString iStringBossSideFaceNode = "侧面";

		bstr_BossSideFaceNode = ConvertToBSTR(iStringBossSideFaceNode);

		IXMLDOMElementPtr pElement_BossSideFaceNode;
		pDoc->createElement(bstr_BossSideFaceNode, &pElement_BossSideFaceNode);
		AppendChildToParent(pElement_BossSideFaceNode, pElement_BossFeatureNode);

		for (std::vector<Face*>::size_type j = 0; j != EachBoss->SideFaceVector.size(); j++)
		{
			Face* EachSideFace = EachBoss->SideFaceVector[j];

			int indexJ = j + 1;
			char* StringIndexJ = new char;
			sprintf(StringIndexJ, "%d", indexJ);
			NXString NXStringIndexJ = StringIndexJ;

			BSTR bstr_BossSideFace;
			NXString iStringBossSideFace = "侧面." + NXStringIndexJ;

			bstr_BossSideFace = ConvertToBSTR(iStringBossSideFace);

			IXMLDOMElementPtr pElement_BossSideFace;
			pDoc->createElement(bstr_BossSideFace, &pElement_BossSideFace);
			AppendChildToParent(pElement_BossSideFace, pElement_BossSideFaceNode);

			char* StringIndexSideFace = new char;
			sprintf(StringIndexSideFace, "%d", EachSideFace->Tag());

			BSTR bstr_BossSideFaceTag;
			NXString iStringBossSideFaceTag = StringIndexSideFace;

			bstr_BossSideFaceTag = ConvertToBSTR(iStringBossSideFaceTag);
			pElement_BossSideFace->put_text(bstr_BossSideFaceTag);
		}

		//凸台顶面
		BSTR bstr_BossTopFaceNode;
		NXString iStringBossTopFaceNode = "顶面";

		bstr_BossTopFaceNode = ConvertToBSTR(iStringBossTopFaceNode);

		IXMLDOMElementPtr pElement_BossTopFaceNode;
		pDoc->createElement(bstr_BossTopFaceNode, &pElement_BossTopFaceNode);
		AppendChildToParent(pElement_BossTopFaceNode, pElement_BossFeatureNode);

		for (std::vector<Face*>::size_type j = 0; j != EachBoss->TopFaceVector.size(); j++)
		{
			Face* EachTopFace = EachBoss->TopFaceVector[j];

			int indexJ = j + 1;
			char* StringIndexJ = new char;
			sprintf(StringIndexJ, "%d", indexJ);
			NXString NXStringIndexJ = StringIndexJ;

			BSTR bstr_BossTopFace;
			NXString iStringBossTopFace = "顶面." + NXStringIndexJ;

			bstr_BossTopFace = ConvertToBSTR(iStringBossTopFace);

			IXMLDOMElementPtr pElement_BossTopFace;
			pDoc->createElement(bstr_BossTopFace, &pElement_BossTopFace);
			AppendChildToParent(pElement_BossTopFace, pElement_BossTopFaceNode);

			char* StringIndexTopFace = new char;
			sprintf(StringIndexTopFace, "%d", EachTopFace->Tag());

			BSTR bstr_BossTopFaceTag;
			NXString iStringBossTopFaceTag = StringIndexTopFace;

			bstr_BossTopFaceTag = ConvertToBSTR(iStringBossTopFaceTag);
			pElement_BossTopFace->put_text(bstr_BossTopFaceTag);
		}
	}
	/*UF_UI_write_listing_window("离开凸台:");*/

	//筋结点
	//NXString iStringRib = "筋";
	//bstr = ConvertToBSTR(iStringRib);

	//IXMLDOMElementPtr pElement_Rib;
	//pDoc->createElement(bstr, &pElement_Rib);
	//AppendChildToParent(pElement_Rib, pDocElement);

	////筋特征输出
	//for (std::vector<Rib*>::size_type i = 0; i != RibSpyResult.size(); i++)
	//{
	//	Rib* EachRib = RibSpyResult[i];

	//	int indexI = i + 1;
	//	char* StringIndexI = new char;
	//	sprintf(StringIndexI, "%d", indexI);
	//	NXString NXStringIndexI = StringIndexI;

	//	BSTR bstr_RibFeatureNode;
	//	NXString iStringRibFeatureNode = "筋." + NXStringIndexI;

	//	bstr_RibFeatureNode = ConvertToBSTR(iStringRibFeatureNode);

	//	IXMLDOMElementPtr pElement_RibFeatureNode;
	//	pDoc->createElement(bstr_RibFeatureNode, &pElement_RibFeatureNode);
	//	AppendChildToParent(pElement_RibFeatureNode, pElement_Rib);

	//	//筋顶面
	//	BSTR bstr_RibTopFaceNode;
	//	NXString iStringRibTopFaceNode = "顶面";

	//	bstr_RibTopFaceNode = ConvertToBSTR(iStringRibTopFaceNode);

	//	IXMLDOMElementPtr pElement_RibTopFaceNode;
	//	pDoc->createElement(bstr_RibTopFaceNode, &pElement_RibTopFaceNode);
	//	AppendChildToParent(pElement_RibTopFaceNode, pElement_RibFeatureNode);

	//	for (std::vector<Face*>::size_type j = 0; j != EachRib->TopFaceVector.size(); j++)
	//	{
	//		Face* EachTopFace = EachRib->TopFaceVector[j];

	//		int indexJ = j + 1;
	//		char* StringIndexJ = new char;
	//		sprintf(StringIndexJ, "%d", indexJ);
	//		NXString NXStringIndexJ = StringIndexJ;

	//		BSTR bstr_RibTopFace;
	//		NXString iStringRibTopFace = "顶面." + NXStringIndexJ;

	//		bstr_RibTopFace = ConvertToBSTR(iStringRibTopFace);

	//		IXMLDOMElementPtr pElement_RibTopFace;
	//		pDoc->createElement(bstr_RibTopFace, &pElement_RibTopFace);
	//		AppendChildToParent(pElement_RibTopFace, pElement_RibTopFaceNode);

	//		char* StringIndexTopFace = new char;
	//		sprintf(StringIndexTopFace, "%d", EachTopFace->Tag());

	//		BSTR bstr_RibTopFaceTag;
	//		NXString iStringRibTopFaceTag = StringIndexTopFace;

	//		bstr_RibTopFaceTag = ConvertToBSTR(iStringRibTopFaceTag);
	//		pElement_RibTopFace->put_text(bstr_RibTopFaceTag);
	//	}

	//	//筋侧面
	//	BSTR bstr_RibSideFaceNode;
	//	NXString iStringRibSideFaceNode = "侧面";

	//	bstr_RibSideFaceNode = ConvertToBSTR(iStringRibSideFaceNode);

	//	IXMLDOMElementPtr pElement_RibSideFaceNode;
	//	pDoc->createElement(bstr_RibSideFaceNode, &pElement_RibSideFaceNode);
	//	AppendChildToParent(pElement_RibSideFaceNode, pElement_RibFeatureNode);

	//	for (std::vector<Face*>::size_type j = 0; j != EachRib->SideFaceVector.size(); j++)
	//	{
	//		Face* EachSideFace = EachRib->SideFaceVector[j];

	//		int indexJ = j + 1;
	//		char* StringIndexJ = new char;
	//		sprintf(StringIndexJ, "%d", indexJ);
	//		NXString NXStringIndexJ = StringIndexJ;

	//		BSTR bstr_RibSideFace;
	//		NXString iStringRibSideFace = "侧面." + NXStringIndexJ;

	//		bstr_RibSideFace = ConvertToBSTR(iStringRibSideFace);

	//		IXMLDOMElementPtr pElement_RibSideFace;
	//		pDoc->createElement(bstr_RibSideFace, &pElement_RibSideFace);
	//		AppendChildToParent(pElement_RibSideFace, pElement_RibSideFaceNode);

	//		char* StringIndexSideFace = new char;
	//		sprintf(StringIndexSideFace, "%d", EachSideFace->Tag());

	//		BSTR bstr_RibSideFaceTag;
	//		NXString iStringRibSideFaceTag = StringIndexSideFace;

	//		bstr_RibSideFaceTag = ConvertToBSTR(iStringRibSideFaceTag);
	//		pElement_RibSideFace->put_text(bstr_RibSideFaceTag);
	//	}
	//}
	//槽结点
	NXString iStringGroove = "槽";
	bstr = ConvertToBSTR(iStringGroove);

	IXMLDOMElementPtr pElement_Groove;
	pDoc->createElement(bstr, &pElement_Groove);
	AppendChildToParent(pElement_Groove, pDocElement);

	//内侧环形槽
	for (std::vector<CircularGroove*>::size_type i = 0; i != CircularGroove0SpyResult.size(); i++) //G
	{
		CircularGroove* EachHole = CircularGroove0SpyResult[i];//G

		int indexI = i + 1;
		char* StringIndexI = new char;
		sprintf(StringIndexI, "%d", indexI);
		NXString NXStringIndexI = StringIndexI;

		BSTR bstr_HoleFeatureNode;
		NXString iStringHoleFeatureNode = "内侧环形槽." + NXStringIndexI;//g

		bstr_HoleFeatureNode = ConvertToBSTR(iStringHoleFeatureNode);

		IXMLDOMElementPtr pElement_HoleFeatureNode;
		pDoc->createElement(bstr_HoleFeatureNode, &pElement_HoleFeatureNode);
		AppendChildToParent(pElement_HoleFeatureNode, pElement_Groove);

		//槽底面
		BSTR bstr_HoleTopFaceNode;
		NXString iStringHoleTopFaceNode = "底面";//g

		bstr_HoleTopFaceNode = ConvertToBSTR(iStringHoleTopFaceNode);

		IXMLDOMElementPtr pElement_HoleTopFaceNode;
		pDoc->createElement(bstr_HoleTopFaceNode, &pElement_HoleTopFaceNode);
		AppendChildToParent(pElement_HoleTopFaceNode, pElement_HoleFeatureNode);

		for (std::vector<Face*>::size_type j = 0; j != EachHole->TopFaceVector.size(); j++)//g
		{
			Face* EachTopFace = EachHole->TopFaceVector[j];//g

			int indexJ = j + 1;
			char* StringIndexJ = new char;
			sprintf(StringIndexJ, "%d", indexJ);
			NXString NXStringIndexJ = StringIndexJ;

			BSTR bstr_HoleTopFace;
			NXString iStringHoleTopFace = "底面." + NXStringIndexJ;//g

			bstr_HoleTopFace = ConvertToBSTR(iStringHoleTopFace);

			IXMLDOMElementPtr pElement_HoleTopFace;
			pDoc->createElement(bstr_HoleTopFace, &pElement_HoleTopFace);
			AppendChildToParent(pElement_HoleTopFace, pElement_HoleTopFaceNode);

			char* StringIndexTopFace = new char;
			sprintf(StringIndexTopFace, "%d", EachTopFace->Tag());

			BSTR bstr_HoleTopFaceTag;
			NXString iStringHoleTopFaceTag = StringIndexTopFace;

			bstr_HoleTopFaceTag = ConvertToBSTR(iStringHoleTopFaceTag);
			pElement_HoleTopFace->put_text(bstr_HoleTopFaceTag);
		}

		//底角面
		BSTR bstr_HoleSideFaceNode;
		NXString iStringHoleSideFaceNode = "底角面";//g

		bstr_HoleSideFaceNode = ConvertToBSTR(iStringHoleSideFaceNode);

		IXMLDOMElementPtr pElement_HoleSideFaceNode;
		pDoc->createElement(bstr_HoleSideFaceNode, &pElement_HoleSideFaceNode);
		AppendChildToParent(pElement_HoleSideFaceNode, pElement_HoleFeatureNode);

		for (std::vector<Face*>::size_type j = 0; j != EachHole->BottomCornerFaceVector.size(); j++)//g
		{
			Face* EachSideFace = EachHole->BottomCornerFaceVector[j];//g

			int indexJ = j + 1;
			char* StringIndexJ = new char;
			sprintf(StringIndexJ, "%d", indexJ);
			NXString NXStringIndexJ = StringIndexJ;

			BSTR bstr_HoleSideFace;
			NXString iStringHoleSideFace = "底角面." + NXStringIndexJ;//g

			bstr_HoleSideFace = ConvertToBSTR(iStringHoleSideFace);

			IXMLDOMElementPtr pElement_HoleSideFace;
			pDoc->createElement(bstr_HoleSideFace, &pElement_HoleSideFace);
			AppendChildToParent(pElement_HoleSideFace, pElement_HoleSideFaceNode);//jie for shang

			char* StringIndexSideFace = new char;
			sprintf(StringIndexSideFace, "%d", EachSideFace->Tag());

			BSTR bstr_HoleSideFaceTag;
			NXString iStringHoleSideFaceTag = StringIndexSideFace;

			bstr_HoleSideFaceTag = ConvertToBSTR(iStringHoleSideFaceTag);
			pElement_HoleSideFace->put_text(bstr_HoleSideFaceTag);
		}
	    
		//孔倒角
		/*BSTR bstr_HoleCornerFaceNode;
		NXString iStringHoleCornerFaceNode = "倒角面";

		bstr_HoleCornerFaceNode = ConvertToBSTR(iStringHoleCornerFaceNode);

		IXMLDOMElementPtr pElement_HoleCornerFaceNode;
		pDoc->createElement(bstr_HoleCornerFaceNode, &pElement_HoleCornerFaceNode);
		AppendChildToParent(pElement_HoleCornerFaceNode, pElement_HoleFeatureNode);

		for (std::vector<Face*>::size_type j = 0; j != EachHole->CornerVector.size(); j++)
		{
			Face* EachCornerFace = EachHole->CornerVector[j];

			int indexJ = j + 1;
			char* StringIndexJ = new char;
			sprintf(StringIndexJ, "%d", indexJ);
			NXString NXStringIndexJ = StringIndexJ;

			BSTR bstr_HoleCornerFace;
			NXString iStringHoleCornerFace = "倒角面." + NXStringIndexJ;

			bstr_HoleCornerFace = ConvertToBSTR(iStringHoleCornerFace);

			IXMLDOMElementPtr pElement_HoleCornerFace;
			pDoc->createElement(bstr_HoleCornerFace, &pElement_HoleCornerFace);
			AppendChildToParent(pElement_HoleCornerFace, pElement_HoleCornerFaceNode);

			char* StringIndexCornerFace = new char;
			sprintf(StringIndexCornerFace, "%d", EachCornerFace->Tag());

			BSTR bstr_HoleCornerFaceTag;
			NXString iStringHoleCornerFaceTag = StringIndexCornerFace;

			bstr_HoleCornerFaceTag = ConvertToBSTR(iStringHoleCornerFaceTag);
			pElement_HoleCornerFace->put_text(bstr_HoleCornerFaceTag);
		}*/

		//约束面
		BSTR bstr_HoleBottomFaceNode;
		NXString iStringHoleBottomFaceNode = "约束面";

		bstr_HoleBottomFaceNode = ConvertToBSTR(iStringHoleBottomFaceNode);

		IXMLDOMElementPtr pElement_HoleBottomFaceNode;
		pDoc->createElement(bstr_HoleBottomFaceNode, &pElement_HoleBottomFaceNode);
		AppendChildToParent(pElement_HoleBottomFaceNode, pElement_HoleFeatureNode);

		for (std::vector<Face*>::size_type j = 0; j != EachHole->LimitFaceVector.size(); j++)
		{
			Face* EachBottomFace = EachHole->LimitFaceVector[j];

			int indexJ = j + 1;
			char* StringIndexJ = new char;
			sprintf(StringIndexJ, "%d", indexJ);
			NXString NXStringIndexJ = StringIndexJ;

			BSTR bstr_HoleBottomFace;
			NXString iStringHoleBottomFace = "约束面." + NXStringIndexJ;

			bstr_HoleBottomFace = ConvertToBSTR(iStringHoleBottomFace);

			IXMLDOMElementPtr pElement_HoleBottomFace;
			pDoc->createElement(bstr_HoleBottomFace, &pElement_HoleBottomFace);
			AppendChildToParent(pElement_HoleBottomFace, pElement_HoleBottomFaceNode);

			char* StringIndexBottomFace = new char;

			

			sprintf(StringIndexBottomFace, "%d", EachBottomFace->Tag());

			
			BSTR bstr_HoleBottomFaceTag;
			NXString iStringHoleBottomFaceTag = StringIndexBottomFace;

			bstr_HoleBottomFaceTag = ConvertToBSTR(iStringHoleBottomFaceTag);
			pElement_HoleBottomFace->put_text(bstr_HoleBottomFaceTag);
		}
	}
	//外侧环形槽
	for (std::vector<CircularGroove*>::size_type i = 0; i != CircularGroove1SpyResult.size(); i++) //G
	{
		CircularGroove* EachHole = CircularGroove1SpyResult[i];//G

		int indexI = i + 1;
		char* StringIndexI = new char;
		sprintf(StringIndexI, "%d", indexI);
		NXString NXStringIndexI = StringIndexI;

		BSTR bstr_HoleFeatureNode;
		NXString iStringHoleFeatureNode = "外侧环形槽." + NXStringIndexI;//g

		bstr_HoleFeatureNode = ConvertToBSTR(iStringHoleFeatureNode);

		IXMLDOMElementPtr pElement_HoleFeatureNode;
		pDoc->createElement(bstr_HoleFeatureNode, &pElement_HoleFeatureNode);
		AppendChildToParent(pElement_HoleFeatureNode, pElement_Groove);

		//槽底面
		BSTR bstr_HoleTopFaceNode;
		NXString iStringHoleTopFaceNode = "底面";//g

		bstr_HoleTopFaceNode = ConvertToBSTR(iStringHoleTopFaceNode);

		IXMLDOMElementPtr pElement_HoleTopFaceNode;
		pDoc->createElement(bstr_HoleTopFaceNode, &pElement_HoleTopFaceNode);
		AppendChildToParent(pElement_HoleTopFaceNode, pElement_HoleFeatureNode);

		for (std::vector<Face*>::size_type j = 0; j != EachHole->TopFaceVector.size(); j++)//g
		{
			Face* EachTopFace = EachHole->TopFaceVector[j];//g

			int indexJ = j + 1;
			char* StringIndexJ = new char;
			sprintf(StringIndexJ, "%d", indexJ);
			NXString NXStringIndexJ = StringIndexJ;

			BSTR bstr_HoleTopFace;
			NXString iStringHoleTopFace = "底面." + NXStringIndexJ;//g

			bstr_HoleTopFace = ConvertToBSTR(iStringHoleTopFace);

			IXMLDOMElementPtr pElement_HoleTopFace;
			pDoc->createElement(bstr_HoleTopFace, &pElement_HoleTopFace);
			AppendChildToParent(pElement_HoleTopFace, pElement_HoleTopFaceNode);

			char* StringIndexTopFace = new char;
			sprintf(StringIndexTopFace, "%d", EachTopFace->Tag());

			BSTR bstr_HoleTopFaceTag;
			NXString iStringHoleTopFaceTag = StringIndexTopFace;

			bstr_HoleTopFaceTag = ConvertToBSTR(iStringHoleTopFaceTag);
			pElement_HoleTopFace->put_text(bstr_HoleTopFaceTag);
		}

		//底角面
		BSTR bstr_HoleSideFaceNode;
		NXString iStringHoleSideFaceNode = "底角面";//g

		bstr_HoleSideFaceNode = ConvertToBSTR(iStringHoleSideFaceNode);

		IXMLDOMElementPtr pElement_HoleSideFaceNode;
		pDoc->createElement(bstr_HoleSideFaceNode, &pElement_HoleSideFaceNode);
		AppendChildToParent(pElement_HoleSideFaceNode, pElement_HoleFeatureNode);

		for (std::vector<Face*>::size_type j = 0; j != EachHole->BottomCornerFaceVector.size(); j++)//g
		{
			Face* EachSideFace = EachHole->BottomCornerFaceVector[j];//g

			int indexJ = j + 1;
			char* StringIndexJ = new char;
			sprintf(StringIndexJ, "%d", indexJ);
			NXString NXStringIndexJ = StringIndexJ;

			BSTR bstr_HoleSideFace;
			NXString iStringHoleSideFace = "底角面." + NXStringIndexJ;//g

			bstr_HoleSideFace = ConvertToBSTR(iStringHoleSideFace);

			IXMLDOMElementPtr pElement_HoleSideFace;
			pDoc->createElement(bstr_HoleSideFace, &pElement_HoleSideFace);
			AppendChildToParent(pElement_HoleSideFace, pElement_HoleSideFaceNode);//jie for shang

			char* StringIndexSideFace = new char;
			sprintf(StringIndexSideFace, "%d", EachSideFace->Tag());

			BSTR bstr_HoleSideFaceTag;
			NXString iStringHoleSideFaceTag = StringIndexSideFace;

			bstr_HoleSideFaceTag = ConvertToBSTR(iStringHoleSideFaceTag);
			pElement_HoleSideFace->put_text(bstr_HoleSideFaceTag);
		}
	    
		//孔倒角
		/*BSTR bstr_HoleCornerFaceNode;
		NXString iStringHoleCornerFaceNode = "倒角面";

		bstr_HoleCornerFaceNode = ConvertToBSTR(iStringHoleCornerFaceNode);

		IXMLDOMElementPtr pElement_HoleCornerFaceNode;
		pDoc->createElement(bstr_HoleCornerFaceNode, &pElement_HoleCornerFaceNode);
		AppendChildToParent(pElement_HoleCornerFaceNode, pElement_HoleFeatureNode);

		for (std::vector<Face*>::size_type j = 0; j != EachHole->CornerVector.size(); j++)
		{
			Face* EachCornerFace = EachHole->CornerVector[j];

			int indexJ = j + 1;
			char* StringIndexJ = new char;
			sprintf(StringIndexJ, "%d", indexJ);
			NXString NXStringIndexJ = StringIndexJ;

			BSTR bstr_HoleCornerFace;
			NXString iStringHoleCornerFace = "倒角面." + NXStringIndexJ;

			bstr_HoleCornerFace = ConvertToBSTR(iStringHoleCornerFace);

			IXMLDOMElementPtr pElement_HoleCornerFace;
			pDoc->createElement(bstr_HoleCornerFace, &pElement_HoleCornerFace);
			AppendChildToParent(pElement_HoleCornerFace, pElement_HoleCornerFaceNode);

			char* StringIndexCornerFace = new char;
			sprintf(StringIndexCornerFace, "%d", EachCornerFace->Tag());

			BSTR bstr_HoleCornerFaceTag;
			NXString iStringHoleCornerFaceTag = StringIndexCornerFace;

			bstr_HoleCornerFaceTag = ConvertToBSTR(iStringHoleCornerFaceTag);
			pElement_HoleCornerFace->put_text(bstr_HoleCornerFaceTag);
		}*/

		//约束面
		BSTR bstr_HoleBottomFaceNode;
		NXString iStringHoleBottomFaceNode = "约束面";

		bstr_HoleBottomFaceNode = ConvertToBSTR(iStringHoleBottomFaceNode);

		IXMLDOMElementPtr pElement_HoleBottomFaceNode;
		pDoc->createElement(bstr_HoleBottomFaceNode, &pElement_HoleBottomFaceNode);
		AppendChildToParent(pElement_HoleBottomFaceNode, pElement_HoleFeatureNode);

		for (std::vector<Face*>::size_type j = 0; j != EachHole->LimitFaceVector.size(); j++)
		{
			Face* EachBottomFace = EachHole->LimitFaceVector[j];

			int indexJ = j + 1;
			char* StringIndexJ = new char;
			sprintf(StringIndexJ, "%d", indexJ);
			NXString NXStringIndexJ = StringIndexJ;

			BSTR bstr_HoleBottomFace;
			NXString iStringHoleBottomFace = "约束面." + NXStringIndexJ;

			bstr_HoleBottomFace = ConvertToBSTR(iStringHoleBottomFace);

			IXMLDOMElementPtr pElement_HoleBottomFace;
			pDoc->createElement(bstr_HoleBottomFace, &pElement_HoleBottomFace);
			AppendChildToParent(pElement_HoleBottomFace, pElement_HoleBottomFaceNode);

			char* StringIndexBottomFace = new char;

			

			sprintf(StringIndexBottomFace, "%d", EachBottomFace->Tag());

			
			BSTR bstr_HoleBottomFaceTag;
			NXString iStringHoleBottomFaceTag = StringIndexBottomFace;

			bstr_HoleBottomFaceTag = ConvertToBSTR(iStringHoleBottomFaceTag);
			pElement_HoleBottomFace->put_text(bstr_HoleBottomFaceTag);
		}
	}
	//下陷槽
	for (std::vector<SubsidenceGroove*>::size_type i = 0; i != SubsidenceGrooveSpyResult.size(); i++) //G
	{
		SubsidenceGroove* EachHole = SubsidenceGrooveSpyResult[i];//G

		int indexI = i + 1;
		char* StringIndexI = new char;
		sprintf(StringIndexI, "%d", indexI);
		NXString NXStringIndexI = StringIndexI;

		BSTR bstr_HoleFeatureNode;
		NXString iStringHoleFeatureNode = "下陷槽." + NXStringIndexI;//g

		bstr_HoleFeatureNode = ConvertToBSTR(iStringHoleFeatureNode);

		IXMLDOMElementPtr pElement_HoleFeatureNode;
		pDoc->createElement(bstr_HoleFeatureNode, &pElement_HoleFeatureNode);
		AppendChildToParent(pElement_HoleFeatureNode, pElement_Groove);

		//槽底面
		BSTR bstr_HoleTopFaceNode;
		NXString iStringHoleTopFaceNode = "底面";//g

		bstr_HoleTopFaceNode = ConvertToBSTR(iStringHoleTopFaceNode);

		IXMLDOMElementPtr pElement_HoleTopFaceNode;
		pDoc->createElement(bstr_HoleTopFaceNode, &pElement_HoleTopFaceNode);
		AppendChildToParent(pElement_HoleTopFaceNode, pElement_HoleFeatureNode);

		for (std::vector<Face*>::size_type j = 0; j != EachHole->TopFaceVector.size(); j++)//g
		{
			Face* EachTopFace = EachHole->TopFaceVector[j];//g

			int indexJ = j + 1;
			char* StringIndexJ = new char;
			sprintf(StringIndexJ, "%d", indexJ);
			NXString NXStringIndexJ = StringIndexJ;

			BSTR bstr_HoleTopFace;
			NXString iStringHoleTopFace = "底面." + NXStringIndexJ;//g

			bstr_HoleTopFace = ConvertToBSTR(iStringHoleTopFace);

			IXMLDOMElementPtr pElement_HoleTopFace;
			pDoc->createElement(bstr_HoleTopFace, &pElement_HoleTopFace);
			AppendChildToParent(pElement_HoleTopFace, pElement_HoleTopFaceNode);

			char* StringIndexTopFace = new char;
			sprintf(StringIndexTopFace, "%d", EachTopFace->Tag());

			BSTR bstr_HoleTopFaceTag;
			NXString iStringHoleTopFaceTag = StringIndexTopFace;

			bstr_HoleTopFaceTag = ConvertToBSTR(iStringHoleTopFaceTag);
			pElement_HoleTopFace->put_text(bstr_HoleTopFaceTag);
		}

		//底角面
		BSTR bstr_HoleSideFaceNode;
		NXString iStringHoleSideFaceNode = "底角面";//g

		bstr_HoleSideFaceNode = ConvertToBSTR(iStringHoleSideFaceNode);

		IXMLDOMElementPtr pElement_HoleSideFaceNode;
		pDoc->createElement(bstr_HoleSideFaceNode, &pElement_HoleSideFaceNode);
		AppendChildToParent(pElement_HoleSideFaceNode, pElement_HoleFeatureNode);

		for (std::vector<Face*>::size_type j = 0; j != EachHole->BottomCornerFaceVector.size(); j++)//g
		{
			Face* EachSideFace = EachHole->BottomCornerFaceVector[j];//g

			int indexJ = j + 1;
			char* StringIndexJ = new char;
			sprintf(StringIndexJ, "%d", indexJ);
			NXString NXStringIndexJ = StringIndexJ;

			BSTR bstr_HoleSideFace;
			NXString iStringHoleSideFace = "底角面." + NXStringIndexJ;//g

			bstr_HoleSideFace = ConvertToBSTR(iStringHoleSideFace);

			IXMLDOMElementPtr pElement_HoleSideFace;
			pDoc->createElement(bstr_HoleSideFace, &pElement_HoleSideFace);
			AppendChildToParent(pElement_HoleSideFace, pElement_HoleSideFaceNode);//jie for shang

			char* StringIndexSideFace = new char;
			sprintf(StringIndexSideFace, "%d", EachSideFace->Tag());

			BSTR bstr_HoleSideFaceTag;
			NXString iStringHoleSideFaceTag = StringIndexSideFace;

			bstr_HoleSideFaceTag = ConvertToBSTR(iStringHoleSideFaceTag);
			pElement_HoleSideFace->put_text(bstr_HoleSideFaceTag);
		}
	    
		//孔倒角
		/*BSTR bstr_HoleCornerFaceNode;
		NXString iStringHoleCornerFaceNode = "倒角面";

		bstr_HoleCornerFaceNode = ConvertToBSTR(iStringHoleCornerFaceNode);

		IXMLDOMElementPtr pElement_HoleCornerFaceNode;
		pDoc->createElement(bstr_HoleCornerFaceNode, &pElement_HoleCornerFaceNode);
		AppendChildToParent(pElement_HoleCornerFaceNode, pElement_HoleFeatureNode);

		for (std::vector<Face*>::size_type j = 0; j != EachHole->CornerVector.size(); j++)
		{
			Face* EachCornerFace = EachHole->CornerVector[j];

			int indexJ = j + 1;
			char* StringIndexJ = new char;
			sprintf(StringIndexJ, "%d", indexJ);
			NXString NXStringIndexJ = StringIndexJ;

			BSTR bstr_HoleCornerFace;
			NXString iStringHoleCornerFace = "倒角面." + NXStringIndexJ;

			bstr_HoleCornerFace = ConvertToBSTR(iStringHoleCornerFace);

			IXMLDOMElementPtr pElement_HoleCornerFace;
			pDoc->createElement(bstr_HoleCornerFace, &pElement_HoleCornerFace);
			AppendChildToParent(pElement_HoleCornerFace, pElement_HoleCornerFaceNode);

			char* StringIndexCornerFace = new char;
			sprintf(StringIndexCornerFace, "%d", EachCornerFace->Tag());

			BSTR bstr_HoleCornerFaceTag;
			NXString iStringHoleCornerFaceTag = StringIndexCornerFace;

			bstr_HoleCornerFaceTag = ConvertToBSTR(iStringHoleCornerFaceTag);
			pElement_HoleCornerFace->put_text(bstr_HoleCornerFaceTag);
		}*/

		//约束面
		/*BSTR bstr_HoleBottomFaceNode;
		NXString iStringHoleBottomFaceNode = "约束面";

		bstr_HoleBottomFaceNode = ConvertToBSTR(iStringHoleBottomFaceNode);

		IXMLDOMElementPtr pElement_HoleBottomFaceNode;
		pDoc->createElement(bstr_HoleBottomFaceNode, &pElement_HoleBottomFaceNode);
		AppendChildToParent(pElement_HoleBottomFaceNode, pElement_HoleFeatureNode);

		for (std::vector<Face*>::size_type j = 0; j != EachHole->LimitFaceVector.size(); j++)
		{
			Face* EachBottomFace = EachHole->LimitFaceVector[j];

			int indexJ = j + 1;
			char* StringIndexJ = new char;
			sprintf(StringIndexJ, "%d", indexJ);
			NXString NXStringIndexJ = StringIndexJ;

			BSTR bstr_HoleBottomFace;
			NXString iStringHoleBottomFace = "约束面." + NXStringIndexJ;

			bstr_HoleBottomFace = ConvertToBSTR(iStringHoleBottomFace);

			IXMLDOMElementPtr pElement_HoleBottomFace;
			pDoc->createElement(bstr_HoleBottomFace, &pElement_HoleBottomFace);
			AppendChildToParent(pElement_HoleBottomFace, pElement_HoleBottomFaceNode);

			char* StringIndexBottomFace = new char;

			

			sprintf(StringIndexBottomFace, "%d", EachBottomFace->Tag());

			
			BSTR bstr_HoleBottomFaceTag;
			NXString iStringHoleBottomFaceTag = StringIndexBottomFace;

			bstr_HoleBottomFaceTag = ConvertToBSTR(iStringHoleBottomFaceTag);
			pElement_HoleBottomFace->put_text(bstr_HoleBottomFaceTag);
		}*/
	}
	//平面浅槽
	for (std::vector<PlanerBlindGroove*>::size_type i = 0; i != PlanerBlindGrooveSpyResult.size(); i++) //G
	{
		PlanerBlindGroove* EachHole = PlanerBlindGrooveSpyResult[i];//G

		int indexI = i + 1;
		char* StringIndexI = new char;
		sprintf(StringIndexI, "%d", indexI);
		NXString NXStringIndexI = StringIndexI;

		BSTR bstr_HoleFeatureNode;
		NXString iStringHoleFeatureNode = "平面浅槽." + NXStringIndexI;//g

		bstr_HoleFeatureNode = ConvertToBSTR(iStringHoleFeatureNode);

		IXMLDOMElementPtr pElement_HoleFeatureNode;
		pDoc->createElement(bstr_HoleFeatureNode, &pElement_HoleFeatureNode);
		AppendChildToParent(pElement_HoleFeatureNode, pElement_Groove);

		//槽底面
		BSTR bstr_HoleTopFaceNode;
		NXString iStringHoleTopFaceNode = "底面";//g

		bstr_HoleTopFaceNode = ConvertToBSTR(iStringHoleTopFaceNode);

		IXMLDOMElementPtr pElement_HoleTopFaceNode;
		pDoc->createElement(bstr_HoleTopFaceNode, &pElement_HoleTopFaceNode);
		AppendChildToParent(pElement_HoleTopFaceNode, pElement_HoleFeatureNode);

		for (std::vector<Face*>::size_type j = 0; j != EachHole->BottomFaceVector.size(); j++)//g
		{
			Face* EachTopFace = EachHole->BottomFaceVector[j];//g

			int indexJ = j + 1;
			char* StringIndexJ = new char;
			sprintf(StringIndexJ, "%d", indexJ);
			NXString NXStringIndexJ = StringIndexJ;

			BSTR bstr_HoleTopFace;
			NXString iStringHoleTopFace = "底面." + NXStringIndexJ;//g

			bstr_HoleTopFace = ConvertToBSTR(iStringHoleTopFace);

			IXMLDOMElementPtr pElement_HoleTopFace;
			pDoc->createElement(bstr_HoleTopFace, &pElement_HoleTopFace);
			AppendChildToParent(pElement_HoleTopFace, pElement_HoleTopFaceNode);

			char* StringIndexTopFace = new char;
			sprintf(StringIndexTopFace, "%d", EachTopFace->Tag());

			BSTR bstr_HoleTopFaceTag;
			NXString iStringHoleTopFaceTag = StringIndexTopFace;

			bstr_HoleTopFaceTag = ConvertToBSTR(iStringHoleTopFaceTag);
			pElement_HoleTopFace->put_text(bstr_HoleTopFaceTag);
		}
		//侧面
		BSTR bstr_HoleCornerFaceNode;
		NXString iStringHoleCornerFaceNode = "侧面";

		bstr_HoleCornerFaceNode = ConvertToBSTR(iStringHoleCornerFaceNode);

		IXMLDOMElementPtr pElement_HoleCornerFaceNode;
		pDoc->createElement(bstr_HoleCornerFaceNode, &pElement_HoleCornerFaceNode);
		AppendChildToParent(pElement_HoleCornerFaceNode, pElement_HoleFeatureNode);

		for (std::vector<Face*>::size_type j = 0; j != EachHole->SideFaceVector.size(); j++)
		{
			Face* EachCornerFace = EachHole->SideFaceVector[j];

			int indexJ = j + 1;
			char* StringIndexJ = new char;
			sprintf(StringIndexJ, "%d", indexJ);
			NXString NXStringIndexJ = StringIndexJ;

			BSTR bstr_HoleCornerFace;
			NXString iStringHoleCornerFace = "侧面." + NXStringIndexJ;

			bstr_HoleCornerFace = ConvertToBSTR(iStringHoleCornerFace);

			IXMLDOMElementPtr pElement_HoleCornerFace;
			pDoc->createElement(bstr_HoleCornerFace, &pElement_HoleCornerFace);
			AppendChildToParent(pElement_HoleCornerFace, pElement_HoleCornerFaceNode);

			char* StringIndexCornerFace = new char;
			sprintf(StringIndexCornerFace, "%d", EachCornerFace->Tag());

			BSTR bstr_HoleCornerFaceTag;
			NXString iStringHoleCornerFaceTag = StringIndexCornerFace;

			bstr_HoleCornerFaceTag = ConvertToBSTR(iStringHoleCornerFaceTag);
			pElement_HoleCornerFace->put_text(bstr_HoleCornerFaceTag);
		}

		//底角面
		BSTR bstr_HoleSideFaceNode;
		NXString iStringHoleSideFaceNode = "底角面";//g

		bstr_HoleSideFaceNode = ConvertToBSTR(iStringHoleSideFaceNode);

		IXMLDOMElementPtr pElement_HoleSideFaceNode;
		pDoc->createElement(bstr_HoleSideFaceNode, &pElement_HoleSideFaceNode);
		AppendChildToParent(pElement_HoleSideFaceNode, pElement_HoleFeatureNode);

		for (std::vector<Face*>::size_type j = 0; j != EachHole->BottomCornerFaceVector.size(); j++)//g
		{
			Face* EachSideFace = EachHole->BottomCornerFaceVector[j];//g

			int indexJ = j + 1;
			char* StringIndexJ = new char;
			sprintf(StringIndexJ, "%d", indexJ);
			NXString NXStringIndexJ = StringIndexJ;

			BSTR bstr_HoleSideFace;
			NXString iStringHoleSideFace = "底角面." + NXStringIndexJ;//g

			bstr_HoleSideFace = ConvertToBSTR(iStringHoleSideFace);

			IXMLDOMElementPtr pElement_HoleSideFace;
			pDoc->createElement(bstr_HoleSideFace, &pElement_HoleSideFace);
			AppendChildToParent(pElement_HoleSideFace, pElement_HoleSideFaceNode);//jie for shang

			char* StringIndexSideFace = new char;
			sprintf(StringIndexSideFace, "%d", EachSideFace->Tag());

			BSTR bstr_HoleSideFaceTag;
			NXString iStringHoleSideFaceTag = StringIndexSideFace;

			bstr_HoleSideFaceTag = ConvertToBSTR(iStringHoleSideFaceTag);
			pElement_HoleSideFace->put_text(bstr_HoleSideFaceTag);
		}
	    
		

		//约束面
		/*BSTR bstr_HoleBottomFaceNode;
		NXString iStringHoleBottomFaceNode = "约束面";

		bstr_HoleBottomFaceNode = ConvertToBSTR(iStringHoleBottomFaceNode);

		IXMLDOMElementPtr pElement_HoleBottomFaceNode;
		pDoc->createElement(bstr_HoleBottomFaceNode, &pElement_HoleBottomFaceNode);
		AppendChildToParent(pElement_HoleBottomFaceNode, pElement_HoleFeatureNode);

		for (std::vector<Face*>::size_type j = 0; j != EachHole->LimitFaceVector.size(); j++)
		{
			Face* EachBottomFace = EachHole->LimitFaceVector[j];

			int indexJ = j + 1;
			char* StringIndexJ = new char;
			sprintf(StringIndexJ, "%d", indexJ);
			NXString NXStringIndexJ = StringIndexJ;

			BSTR bstr_HoleBottomFace;
			NXString iStringHoleBottomFace = "约束面." + NXStringIndexJ;

			bstr_HoleBottomFace = ConvertToBSTR(iStringHoleBottomFace);

			IXMLDOMElementPtr pElement_HoleBottomFace;
			pDoc->createElement(bstr_HoleBottomFace, &pElement_HoleBottomFace);
			AppendChildToParent(pElement_HoleBottomFace, pElement_HoleBottomFaceNode);

			char* StringIndexBottomFace = new char;

			

			sprintf(StringIndexBottomFace, "%d", EachBottomFace->Tag());

			
			BSTR bstr_HoleBottomFaceTag;
			NXString iStringHoleBottomFaceTag = StringIndexBottomFace;

			bstr_HoleBottomFaceTag = ConvertToBSTR(iStringHoleBottomFaceTag);
			pElement_HoleBottomFace->put_text(bstr_HoleBottomFaceTag);
		}*/
	}
	//通槽
	for (std::vector<PassGroove*>::size_type i = 0; i != PassGrooveSpyResult.size(); i++) //G
	{
		PassGroove* EachHole = PassGrooveSpyResult[i];//G

		int indexI = i + 1;
		char* StringIndexI = new char;
		sprintf(StringIndexI, "%d", indexI);
		NXString NXStringIndexI = StringIndexI;

		BSTR bstr_HoleFeatureNode;
		NXString iStringHoleFeatureNode = "通槽." + NXStringIndexI;//g

		bstr_HoleFeatureNode = ConvertToBSTR(iStringHoleFeatureNode);

		IXMLDOMElementPtr pElement_HoleFeatureNode;
		pDoc->createElement(bstr_HoleFeatureNode, &pElement_HoleFeatureNode);
		AppendChildToParent(pElement_HoleFeatureNode, pElement_Groove);

		//槽底面
		BSTR bstr_HoleTopFaceNode;
		NXString iStringHoleTopFaceNode = "顶面";//g

		bstr_HoleTopFaceNode = ConvertToBSTR(iStringHoleTopFaceNode);

		IXMLDOMElementPtr pElement_HoleTopFaceNode;
		pDoc->createElement(bstr_HoleTopFaceNode, &pElement_HoleTopFaceNode);
		AppendChildToParent(pElement_HoleTopFaceNode, pElement_HoleFeatureNode);

		for (std::vector<Face*>::size_type j = 0; j != EachHole->TopFaceVector.size(); j++)//g
		{
			Face* EachTopFace = EachHole->TopFaceVector[j];//g

			int indexJ = j + 1;
			char* StringIndexJ = new char;
			sprintf(StringIndexJ, "%d", indexJ);
			NXString NXStringIndexJ = StringIndexJ;

			BSTR bstr_HoleTopFace;
			NXString iStringHoleTopFace = "顶面." + NXStringIndexJ;//g

			bstr_HoleTopFace = ConvertToBSTR(iStringHoleTopFace);

			IXMLDOMElementPtr pElement_HoleTopFace;
			pDoc->createElement(bstr_HoleTopFace, &pElement_HoleTopFace);
			AppendChildToParent(pElement_HoleTopFace, pElement_HoleTopFaceNode);

			char* StringIndexTopFace = new char;
			sprintf(StringIndexTopFace, "%d", EachTopFace->Tag());

			BSTR bstr_HoleTopFaceTag;
			NXString iStringHoleTopFaceTag = StringIndexTopFace;

			bstr_HoleTopFaceTag = ConvertToBSTR(iStringHoleTopFaceTag);
			pElement_HoleTopFace->put_text(bstr_HoleTopFaceTag);
		}
		//底面
		BSTR bstr_HoleCornerFaceNode;
		NXString iStringHoleCornerFaceNode = "底面";

		bstr_HoleCornerFaceNode = ConvertToBSTR(iStringHoleCornerFaceNode);

		IXMLDOMElementPtr pElement_HoleCornerFaceNode;
		pDoc->createElement(bstr_HoleCornerFaceNode, &pElement_HoleCornerFaceNode);
		AppendChildToParent(pElement_HoleCornerFaceNode, pElement_HoleFeatureNode);

		for (std::vector<Face*>::size_type j = 0; j != EachHole->BottomFaceVector.size(); j++)
		{
			Face* EachCornerFace = EachHole->BottomFaceVector[j];

			int indexJ = j + 1;
			char* StringIndexJ = new char;
			sprintf(StringIndexJ, "%d", indexJ);
			NXString NXStringIndexJ = StringIndexJ;

			BSTR bstr_HoleCornerFace;
			NXString iStringHoleCornerFace = "底面." + NXStringIndexJ;

			bstr_HoleCornerFace = ConvertToBSTR(iStringHoleCornerFace);

			IXMLDOMElementPtr pElement_HoleCornerFace;
			pDoc->createElement(bstr_HoleCornerFace, &pElement_HoleCornerFace);
			AppendChildToParent(pElement_HoleCornerFace, pElement_HoleCornerFaceNode);

			char* StringIndexCornerFace = new char;
			sprintf(StringIndexCornerFace, "%d", EachCornerFace->Tag());

			BSTR bstr_HoleCornerFaceTag;
			NXString iStringHoleCornerFaceTag = StringIndexCornerFace;

			bstr_HoleCornerFaceTag = ConvertToBSTR(iStringHoleCornerFaceTag);
			pElement_HoleCornerFace->put_text(bstr_HoleCornerFaceTag);
		}

		//倒斜角面
		BSTR bstr_HoleSideFaceNode;
		NXString iStringHoleSideFaceNode = "倒斜角面";//g

		bstr_HoleSideFaceNode = ConvertToBSTR(iStringHoleSideFaceNode);

		IXMLDOMElementPtr pElement_HoleSideFaceNode;
		pDoc->createElement(bstr_HoleSideFaceNode, &pElement_HoleSideFaceNode);
		AppendChildToParent(pElement_HoleSideFaceNode, pElement_HoleFeatureNode);

		for (std::vector<Face*>::size_type j = 0; j != EachHole->RightFaceVector.size(); j++)//g
		{
			Face* EachSideFace = EachHole->RightFaceVector[j];//g

			int indexJ = j + 1;
			char* StringIndexJ = new char;
			sprintf(StringIndexJ, "%d", indexJ);
			NXString NXStringIndexJ = StringIndexJ;

			BSTR bstr_HoleSideFace;
			NXString iStringHoleSideFace = "倒斜角面." + NXStringIndexJ;//g

			bstr_HoleSideFace = ConvertToBSTR(iStringHoleSideFace);

			IXMLDOMElementPtr pElement_HoleSideFace;
			pDoc->createElement(bstr_HoleSideFace, &pElement_HoleSideFace);
			AppendChildToParent(pElement_HoleSideFace, pElement_HoleSideFaceNode);//jie for shang

			char* StringIndexSideFace = new char;
			sprintf(StringIndexSideFace, "%d", EachSideFace->Tag());

			BSTR bstr_HoleSideFaceTag;
			NXString iStringHoleSideFaceTag = StringIndexSideFace;

			bstr_HoleSideFaceTag = ConvertToBSTR(iStringHoleSideFaceTag);
			pElement_HoleSideFace->put_text(bstr_HoleSideFaceTag);
		}
	    
		

		//侧面
		BSTR bstr_HoleBottomFaceNode;
		NXString iStringHoleBottomFaceNode = "侧面";

		bstr_HoleBottomFaceNode = ConvertToBSTR(iStringHoleBottomFaceNode);

		IXMLDOMElementPtr pElement_HoleBottomFaceNode;
		pDoc->createElement(bstr_HoleBottomFaceNode, &pElement_HoleBottomFaceNode);
		AppendChildToParent(pElement_HoleBottomFaceNode, pElement_HoleFeatureNode);

		for (std::vector<Face*>::size_type j = 0; j != EachHole->SideFaceVector.size(); j++)
		{
			Face* EachBottomFace = EachHole->SideFaceVector[j];

			int indexJ = j + 1;
			char* StringIndexJ = new char;
			sprintf(StringIndexJ, "%d", indexJ);
			NXString NXStringIndexJ = StringIndexJ;

			BSTR bstr_HoleBottomFace;
			NXString iStringHoleBottomFace = "侧面." + NXStringIndexJ;

			bstr_HoleBottomFace = ConvertToBSTR(iStringHoleBottomFace);

			IXMLDOMElementPtr pElement_HoleBottomFace;
			pDoc->createElement(bstr_HoleBottomFace, &pElement_HoleBottomFace);
			AppendChildToParent(pElement_HoleBottomFace, pElement_HoleBottomFaceNode);

			char* StringIndexBottomFace = new char;

			

			sprintf(StringIndexBottomFace, "%d", EachBottomFace->Tag());

			
			BSTR bstr_HoleBottomFaceTag;
			NXString iStringHoleBottomFaceTag = StringIndexBottomFace;

			bstr_HoleBottomFaceTag = ConvertToBSTR(iStringHoleBottomFaceTag);
			pElement_HoleBottomFace->put_text(bstr_HoleBottomFaceTag);
		}
	}

	//孔结点
	NXString iStringHole = "孔";
	bstr = ConvertToBSTR(iStringHole);

	IXMLDOMElementPtr pElement_Hole;
	pDoc->createElement(bstr, &pElement_Hole);
	AppendChildToParent(pElement_Hole, pDocElement);

	//其它孔
	for (std::vector<Other3Hole*>::size_type i = 0; i != otherHole3SpyResult.size(); i++) //G
	{
		Other3Hole* EachHole = otherHole3SpyResult[i];//G

		int indexI = i + 1;
		char* StringIndexI = new char;
		sprintf(StringIndexI, "%d", indexI);
		NXString NXStringIndexI = StringIndexI;

		BSTR bstr_HoleFeatureNode;
		NXString iStringHoleFeatureNode = "其它孔." + NXStringIndexI;//g

		bstr_HoleFeatureNode = ConvertToBSTR(iStringHoleFeatureNode);

		IXMLDOMElementPtr pElement_HoleFeatureNode;
		pDoc->createElement(bstr_HoleFeatureNode, &pElement_HoleFeatureNode);
		AppendChildToParent(pElement_HoleFeatureNode, pElement_Hole);

		//孔顶面
		BSTR bstr_HoleTopFaceNode;
		NXString iStringHoleTopFaceNode = "顶面";

		bstr_HoleTopFaceNode = ConvertToBSTR(iStringHoleTopFaceNode);

		IXMLDOMElementPtr pElement_HoleTopFaceNode;
		pDoc->createElement(bstr_HoleTopFaceNode, &pElement_HoleTopFaceNode);
		AppendChildToParent(pElement_HoleTopFaceNode, pElement_HoleFeatureNode);

		for (std::vector<Face*>::size_type j = 0; j != EachHole->OuterVector.size(); j++)
		{
			Face* EachTopFace = EachHole->OuterVector[j];

			int indexJ = j + 1;
			char* StringIndexJ = new char;
			sprintf(StringIndexJ, "%d", indexJ);
			NXString NXStringIndexJ = StringIndexJ;

			BSTR bstr_HoleTopFace;
			NXString iStringHoleTopFace = "顶面." + NXStringIndexJ;

			bstr_HoleTopFace = ConvertToBSTR(iStringHoleTopFace);

			IXMLDOMElementPtr pElement_HoleTopFace;
			pDoc->createElement(bstr_HoleTopFace, &pElement_HoleTopFace);
			AppendChildToParent(pElement_HoleTopFace, pElement_HoleTopFaceNode);

			char* StringIndexTopFace = new char;
			sprintf(StringIndexTopFace, "%d", EachTopFace->Tag());

			BSTR bstr_HoleTopFaceTag;
			NXString iStringHoleTopFaceTag = StringIndexTopFace;

			bstr_HoleTopFaceTag = ConvertToBSTR(iStringHoleTopFaceTag);
			pElement_HoleTopFace->put_text(bstr_HoleTopFaceTag);
		}

		//孔壁
		BSTR bstr_HoleSideFaceNode;
		NXString iStringHoleSideFaceNode = "内壁面";//g

		bstr_HoleSideFaceNode = ConvertToBSTR(iStringHoleSideFaceNode);

		IXMLDOMElementPtr pElement_HoleSideFaceNode;
		pDoc->createElement(bstr_HoleSideFaceNode, &pElement_HoleSideFaceNode);
		AppendChildToParent(pElement_HoleSideFaceNode, pElement_HoleFeatureNode);

		for (std::vector<Face*>::size_type j = 0; j != EachHole->InWallVector.size(); j++)//g
		{
			Face* EachSideFace = EachHole->InWallVector[j];//g

			int indexJ = j + 1;
			char* StringIndexJ = new char;
			sprintf(StringIndexJ, "%d", indexJ);
			NXString NXStringIndexJ = StringIndexJ;

			BSTR bstr_HoleSideFace;
			NXString iStringHoleSideFace = "侧面." + NXStringIndexJ;//g

			bstr_HoleSideFace = ConvertToBSTR(iStringHoleSideFace);

			IXMLDOMElementPtr pElement_HoleSideFace;
			pDoc->createElement(bstr_HoleSideFace, &pElement_HoleSideFace);
			AppendChildToParent(pElement_HoleSideFace, pElement_HoleSideFaceNode);//jie for shang

			char* StringIndexSideFace = new char;
			sprintf(StringIndexSideFace, "%d", EachSideFace->Tag());

			BSTR bstr_HoleSideFaceTag;
			NXString iStringHoleSideFaceTag = StringIndexSideFace;

			bstr_HoleSideFaceTag = ConvertToBSTR(iStringHoleSideFaceTag);
			pElement_HoleSideFace->put_text(bstr_HoleSideFaceTag);
		}
	}

	//普通通孔
	for (std::vector<DirectionNormalHole*>::size_type i = 0; i != DirectionNormalHoleSpyResult.size(); i++) //G
	{
		DirectionNormalHole* EachHole = DirectionNormalHoleSpyResult[i];//G

		int indexI = i + 1;
		char* StringIndexI = new char;
		sprintf(StringIndexI, "%d", indexI);
		NXString NXStringIndexI = StringIndexI;

		BSTR bstr_HoleFeatureNode;
		NXString iStringHoleFeatureNode = "轴向普通孔." + NXStringIndexI;//g

		bstr_HoleFeatureNode = ConvertToBSTR(iStringHoleFeatureNode);

		IXMLDOMElementPtr pElement_HoleFeatureNode;
		pDoc->createElement(bstr_HoleFeatureNode, &pElement_HoleFeatureNode);
		AppendChildToParent(pElement_HoleFeatureNode, pElement_Hole);

		//孔顶面
		BSTR bstr_HoleTopFaceNode;
		NXString iStringHoleTopFaceNode = "顶面";

		bstr_HoleTopFaceNode = ConvertToBSTR(iStringHoleTopFaceNode);

		IXMLDOMElementPtr pElement_HoleTopFaceNode;
		pDoc->createElement(bstr_HoleTopFaceNode, &pElement_HoleTopFaceNode);
		AppendChildToParent(pElement_HoleTopFaceNode, pElement_HoleFeatureNode);

		for (std::vector<Face*>::size_type j = 0; j != EachHole->TopFaceVector.size(); j++)
		{
			Face* EachTopFace = EachHole->TopFaceVector[j];

			int indexJ = j + 1;
			char* StringIndexJ = new char;
			sprintf(StringIndexJ, "%d", indexJ);
			NXString NXStringIndexJ = StringIndexJ;

			BSTR bstr_HoleTopFace;
			NXString iStringHoleTopFace = "顶面." + NXStringIndexJ;

			bstr_HoleTopFace = ConvertToBSTR(iStringHoleTopFace);

			IXMLDOMElementPtr pElement_HoleTopFace;
			pDoc->createElement(bstr_HoleTopFace, &pElement_HoleTopFace);
			AppendChildToParent(pElement_HoleTopFace, pElement_HoleTopFaceNode);

			char* StringIndexTopFace = new char;
			sprintf(StringIndexTopFace, "%d", EachTopFace->Tag());

			BSTR bstr_HoleTopFaceTag;
			NXString iStringHoleTopFaceTag = StringIndexTopFace;

			bstr_HoleTopFaceTag = ConvertToBSTR(iStringHoleTopFaceTag);
			pElement_HoleTopFace->put_text(bstr_HoleTopFaceTag);
		}

		//孔壁
		BSTR bstr_HoleSideFaceNode;
		NXString iStringHoleSideFaceNode = "侧面";//g

		bstr_HoleSideFaceNode = ConvertToBSTR(iStringHoleSideFaceNode);

		IXMLDOMElementPtr pElement_HoleSideFaceNode;
		pDoc->createElement(bstr_HoleSideFaceNode, &pElement_HoleSideFaceNode);
		AppendChildToParent(pElement_HoleSideFaceNode, pElement_HoleFeatureNode);

		for (std::vector<Face*>::size_type j = 0; j != EachHole->InWallVector.size(); j++)//g
		{
			Face* EachSideFace = EachHole->InWallVector[j];//g

			int indexJ = j + 1;
			char* StringIndexJ = new char;
			sprintf(StringIndexJ, "%d", indexJ);
			NXString NXStringIndexJ = StringIndexJ;

			BSTR bstr_HoleSideFace;
			NXString iStringHoleSideFace = "侧面." + NXStringIndexJ;//g

			bstr_HoleSideFace = ConvertToBSTR(iStringHoleSideFace);

			IXMLDOMElementPtr pElement_HoleSideFace;
			pDoc->createElement(bstr_HoleSideFace, &pElement_HoleSideFace);
			AppendChildToParent(pElement_HoleSideFace, pElement_HoleSideFaceNode);//jie for shang

			char* StringIndexSideFace = new char;
			sprintf(StringIndexSideFace, "%d", EachSideFace->Tag());

			BSTR bstr_HoleSideFaceTag;
			NXString iStringHoleSideFaceTag = StringIndexSideFace;

			bstr_HoleSideFaceTag = ConvertToBSTR(iStringHoleSideFaceTag);
			pElement_HoleSideFace->put_text(bstr_HoleSideFaceTag);
		}
	    
		//孔倒角
		/*BSTR bstr_HoleCornerFaceNode;
		NXString iStringHoleCornerFaceNode = "倒角面";

		bstr_HoleCornerFaceNode = ConvertToBSTR(iStringHoleCornerFaceNode);

		IXMLDOMElementPtr pElement_HoleCornerFaceNode;
		pDoc->createElement(bstr_HoleCornerFaceNode, &pElement_HoleCornerFaceNode);
		AppendChildToParent(pElement_HoleCornerFaceNode, pElement_HoleFeatureNode);

		for (std::vector<Face*>::size_type j = 0; j != EachHole->CornerVector.size(); j++)
		{
			Face* EachCornerFace = EachHole->CornerVector[j];

			int indexJ = j + 1;
			char* StringIndexJ = new char;
			sprintf(StringIndexJ, "%d", indexJ);
			NXString NXStringIndexJ = StringIndexJ;

			BSTR bstr_HoleCornerFace;
			NXString iStringHoleCornerFace = "倒角面." + NXStringIndexJ;

			bstr_HoleCornerFace = ConvertToBSTR(iStringHoleCornerFace);

			IXMLDOMElementPtr pElement_HoleCornerFace;
			pDoc->createElement(bstr_HoleCornerFace, &pElement_HoleCornerFace);
			AppendChildToParent(pElement_HoleCornerFace, pElement_HoleCornerFaceNode);

			char* StringIndexCornerFace = new char;
			sprintf(StringIndexCornerFace, "%d", EachCornerFace->Tag());

			BSTR bstr_HoleCornerFaceTag;
			NXString iStringHoleCornerFaceTag = StringIndexCornerFace;

			bstr_HoleCornerFaceTag = ConvertToBSTR(iStringHoleCornerFaceTag);
			pElement_HoleCornerFace->put_text(bstr_HoleCornerFaceTag);
		}*/

		//孔底面
		BSTR bstr_HoleBottomFaceNode;
		NXString iStringHoleBottomFaceNode = "底面";

		bstr_HoleBottomFaceNode = ConvertToBSTR(iStringHoleBottomFaceNode);

		IXMLDOMElementPtr pElement_HoleBottomFaceNode;
		pDoc->createElement(bstr_HoleBottomFaceNode, &pElement_HoleBottomFaceNode);
		AppendChildToParent(pElement_HoleBottomFaceNode, pElement_HoleFeatureNode);

		for (std::vector<Face*>::size_type j = 0; j != EachHole->BottomFaceVector.size(); j++)
		{
			Face* EachBottomFace = EachHole->BottomFaceVector[j];

			int indexJ = j + 1;
			char* StringIndexJ = new char;
			sprintf(StringIndexJ, "%d", indexJ);
			NXString NXStringIndexJ = StringIndexJ;

			BSTR bstr_HoleBottomFace;
			NXString iStringHoleBottomFace = "底面." + NXStringIndexJ;

			bstr_HoleBottomFace = ConvertToBSTR(iStringHoleBottomFace);

			IXMLDOMElementPtr pElement_HoleBottomFace;
			pDoc->createElement(bstr_HoleBottomFace, &pElement_HoleBottomFace);
			AppendChildToParent(pElement_HoleBottomFace, pElement_HoleBottomFaceNode);

			char* StringIndexBottomFace = new char;

			

			sprintf(StringIndexBottomFace, "%d", EachBottomFace->Tag());

			
			BSTR bstr_HoleBottomFaceTag;
			NXString iStringHoleBottomFaceTag = StringIndexBottomFace;

			bstr_HoleBottomFaceTag = ConvertToBSTR(iStringHoleBottomFaceTag);
			pElement_HoleBottomFace->put_text(bstr_HoleBottomFaceTag);
		}
	}
	//轴向单侧斜角通孔
	for (std::vector<DirectionRightHole*>::size_type i = 0; i != DirectionRightHoleSpyResult.size(); i++) //G
	{
		DirectionRightHole* EachHole = DirectionRightHoleSpyResult[i];//G

		int indexI = i + 1;
		char* StringIndexI = new char;
		sprintf(StringIndexI, "%d", indexI);
		NXString NXStringIndexI = StringIndexI;

		BSTR bstr_HoleFeatureNode;
		NXString iStringHoleFeatureNode = "轴向单侧斜角通孔." + NXStringIndexI;//g

		bstr_HoleFeatureNode = ConvertToBSTR(iStringHoleFeatureNode);

		IXMLDOMElementPtr pElement_HoleFeatureNode;
		pDoc->createElement(bstr_HoleFeatureNode, &pElement_HoleFeatureNode);
		AppendChildToParent(pElement_HoleFeatureNode, pElement_Hole);

		//孔顶面
		BSTR bstr_HoleTopFaceNode;
		NXString iStringHoleTopFaceNode = "顶面";

		bstr_HoleTopFaceNode = ConvertToBSTR(iStringHoleTopFaceNode);

		IXMLDOMElementPtr pElement_HoleTopFaceNode;
		pDoc->createElement(bstr_HoleTopFaceNode, &pElement_HoleTopFaceNode);
		AppendChildToParent(pElement_HoleTopFaceNode, pElement_HoleFeatureNode);

		for (std::vector<Face*>::size_type j = 0; j != EachHole->TopFaceVector.size(); j++)
		{
			Face* EachTopFace = EachHole->TopFaceVector[j];

			int indexJ = j + 1;
			char* StringIndexJ = new char;
			sprintf(StringIndexJ, "%d", indexJ);
			NXString NXStringIndexJ = StringIndexJ;

			BSTR bstr_HoleTopFace;
			NXString iStringHoleTopFace = "顶面." + NXStringIndexJ;

			bstr_HoleTopFace = ConvertToBSTR(iStringHoleTopFace);

			IXMLDOMElementPtr pElement_HoleTopFace;
			pDoc->createElement(bstr_HoleTopFace, &pElement_HoleTopFace);
			AppendChildToParent(pElement_HoleTopFace, pElement_HoleTopFaceNode);

			char* StringIndexTopFace = new char;
			sprintf(StringIndexTopFace, "%d", EachTopFace->Tag());

			BSTR bstr_HoleTopFaceTag;
			NXString iStringHoleTopFaceTag = StringIndexTopFace;

			bstr_HoleTopFaceTag = ConvertToBSTR(iStringHoleTopFaceTag);
			pElement_HoleTopFace->put_text(bstr_HoleTopFaceTag);
		}

		//倒斜角面
		BSTR bstr_BossTopFaceNode;
		NXString iStringBossTopFaceNode = "倒斜角面";

		bstr_BossTopFaceNode = ConvertToBSTR(iStringBossTopFaceNode);

		IXMLDOMElementPtr pElement_BossTopFaceNode;
		pDoc->createElement(bstr_BossTopFaceNode, &pElement_BossTopFaceNode);
		AppendChildToParent(pElement_BossTopFaceNode, pElement_HoleFeatureNode);//g

		for (std::vector<Face*>::size_type j = 0; j != EachHole->RightVector.size(); j++)//g
		{
			Face* EachTopFace = EachHole->RightVector[j];//g

			int indexJ = j + 1;
			char* StringIndexJ = new char;
			sprintf(StringIndexJ, "%d", indexJ);
			NXString NXStringIndexJ = StringIndexJ;

			BSTR bstr_BossTopFace;
			NXString iStringBossTopFace = "倒斜角面." + NXStringIndexJ;

			bstr_BossTopFace = ConvertToBSTR(iStringBossTopFace);

			IXMLDOMElementPtr pElement_BossTopFace;
			pDoc->createElement(bstr_BossTopFace, &pElement_BossTopFace);
			AppendChildToParent(pElement_BossTopFace, pElement_BossTopFaceNode);

			char* StringIndexTopFace = new char;
			sprintf(StringIndexTopFace, "%d", EachTopFace->Tag());

			BSTR bstr_BossTopFaceTag;
			NXString iStringBossTopFaceTag = StringIndexTopFace;

			bstr_BossTopFaceTag = ConvertToBSTR(iStringBossTopFaceTag);
			pElement_BossTopFace->put_text(bstr_BossTopFaceTag);
		}

		//孔壁
		BSTR bstr_HoleSideFaceNode;
		NXString iStringHoleSideFaceNode = "侧面";//g

		bstr_HoleSideFaceNode = ConvertToBSTR(iStringHoleSideFaceNode);

		IXMLDOMElementPtr pElement_HoleSideFaceNode;
		pDoc->createElement(bstr_HoleSideFaceNode, &pElement_HoleSideFaceNode);
		AppendChildToParent(pElement_HoleSideFaceNode, pElement_HoleFeatureNode);

		for (std::vector<Face*>::size_type j = 0; j != EachHole->InWallVector.size(); j++)//g
		{
			Face* EachSideFace = EachHole->InWallVector[j];//g

			int indexJ = j + 1;
			char* StringIndexJ = new char;
			sprintf(StringIndexJ, "%d", indexJ);
			NXString NXStringIndexJ = StringIndexJ;

			BSTR bstr_HoleSideFace;
			NXString iStringHoleSideFace = "侧面." + NXStringIndexJ;//g

			bstr_HoleSideFace = ConvertToBSTR(iStringHoleSideFace);

			IXMLDOMElementPtr pElement_HoleSideFace;
			pDoc->createElement(bstr_HoleSideFace, &pElement_HoleSideFace);
			AppendChildToParent(pElement_HoleSideFace, pElement_HoleSideFaceNode);//jie for shang

			char* StringIndexSideFace = new char;
			sprintf(StringIndexSideFace, "%d", EachSideFace->Tag());

			BSTR bstr_HoleSideFaceTag;
			NXString iStringHoleSideFaceTag = StringIndexSideFace;

			bstr_HoleSideFaceTag = ConvertToBSTR(iStringHoleSideFaceTag);
			pElement_HoleSideFace->put_text(bstr_HoleSideFaceTag);
		}
	    
		//孔倒角
		/*BSTR bstr_HoleCornerFaceNode;
		NXString iStringHoleCornerFaceNode = "倒角面";

		bstr_HoleCornerFaceNode = ConvertToBSTR(iStringHoleCornerFaceNode);

		IXMLDOMElementPtr pElement_HoleCornerFaceNode;
		pDoc->createElement(bstr_HoleCornerFaceNode, &pElement_HoleCornerFaceNode);
		AppendChildToParent(pElement_HoleCornerFaceNode, pElement_HoleFeatureNode);

		for (std::vector<Face*>::size_type j = 0; j != EachHole->CornerVector.size(); j++)
		{
			Face* EachCornerFace = EachHole->CornerVector[j];

			int indexJ = j + 1;
			char* StringIndexJ = new char;
			sprintf(StringIndexJ, "%d", indexJ);
			NXString NXStringIndexJ = StringIndexJ;

			BSTR bstr_HoleCornerFace;
			NXString iStringHoleCornerFace = "倒角面." + NXStringIndexJ;

			bstr_HoleCornerFace = ConvertToBSTR(iStringHoleCornerFace);

			IXMLDOMElementPtr pElement_HoleCornerFace;
			pDoc->createElement(bstr_HoleCornerFace, &pElement_HoleCornerFace);
			AppendChildToParent(pElement_HoleCornerFace, pElement_HoleCornerFaceNode);

			char* StringIndexCornerFace = new char;
			sprintf(StringIndexCornerFace, "%d", EachCornerFace->Tag());

			BSTR bstr_HoleCornerFaceTag;
			NXString iStringHoleCornerFaceTag = StringIndexCornerFace;

			bstr_HoleCornerFaceTag = ConvertToBSTR(iStringHoleCornerFaceTag);
			pElement_HoleCornerFace->put_text(bstr_HoleCornerFaceTag);
		}*/

		//孔底面
		BSTR bstr_HoleBottomFaceNode;
		NXString iStringHoleBottomFaceNode = "底面";

		bstr_HoleBottomFaceNode = ConvertToBSTR(iStringHoleBottomFaceNode);

		IXMLDOMElementPtr pElement_HoleBottomFaceNode;
		pDoc->createElement(bstr_HoleBottomFaceNode, &pElement_HoleBottomFaceNode);
		AppendChildToParent(pElement_HoleBottomFaceNode, pElement_HoleFeatureNode);

		for (std::vector<Face*>::size_type j = 0; j != EachHole->BottomFaceVector.size(); j++)
		{
			Face* EachBottomFace = EachHole->BottomFaceVector[j];

			int indexJ = j + 1;
			char* StringIndexJ = new char;
			sprintf(StringIndexJ, "%d", indexJ);
			NXString NXStringIndexJ = StringIndexJ;

			BSTR bstr_HoleBottomFace;
			NXString iStringHoleBottomFace = "底面." + NXStringIndexJ;

			bstr_HoleBottomFace = ConvertToBSTR(iStringHoleBottomFace);

			IXMLDOMElementPtr pElement_HoleBottomFace;
			pDoc->createElement(bstr_HoleBottomFace, &pElement_HoleBottomFace);
			AppendChildToParent(pElement_HoleBottomFace, pElement_HoleBottomFaceNode);

			char* StringIndexBottomFace = new char;
			sprintf(StringIndexBottomFace, "%d", EachBottomFace->Tag());

			BSTR bstr_HoleBottomFaceTag;
			NXString iStringHoleBottomFaceTag = StringIndexBottomFace;

			bstr_HoleBottomFaceTag = ConvertToBSTR(iStringHoleBottomFaceTag);
			pElement_HoleBottomFace->put_text(bstr_HoleBottomFaceTag);
		}
	}
	//盲孔
	for (std::vector<DirectionBlindHole*>::size_type i = 0; i != DirectionBlindHoleSpyResult.size(); i++) //G
	{
		DirectionBlindHole* EachHole = DirectionBlindHoleSpyResult[i];//G

		int indexI = i + 1;
		char* StringIndexI = new char;
		sprintf(StringIndexI, "%d", indexI);
		NXString NXStringIndexI = StringIndexI;

		BSTR bstr_HoleFeatureNode;
		NXString iStringHoleFeatureNode = "轴向无倒角盲孔." + NXStringIndexI;//g

		bstr_HoleFeatureNode = ConvertToBSTR(iStringHoleFeatureNode);

		IXMLDOMElementPtr pElement_HoleFeatureNode;
		pDoc->createElement(bstr_HoleFeatureNode, &pElement_HoleFeatureNode);
		AppendChildToParent(pElement_HoleFeatureNode, pElement_Hole);

		//孔顶面
		BSTR bstr_HoleTopFaceNode;
		NXString iStringHoleTopFaceNode = "顶面";

		bstr_HoleTopFaceNode = ConvertToBSTR(iStringHoleTopFaceNode);

		IXMLDOMElementPtr pElement_HoleTopFaceNode;
		pDoc->createElement(bstr_HoleTopFaceNode, &pElement_HoleTopFaceNode);
		AppendChildToParent(pElement_HoleTopFaceNode, pElement_HoleFeatureNode);

		for (std::vector<Face*>::size_type j = 0; j != EachHole->TopFaceVector.size(); j++)
		{
			Face* EachTopFace = EachHole->TopFaceVector[j];

			int indexJ = j + 1;
			char* StringIndexJ = new char;
			sprintf(StringIndexJ, "%d", indexJ);
			NXString NXStringIndexJ = StringIndexJ;

			BSTR bstr_HoleTopFace;
			NXString iStringHoleTopFace = "顶面." + NXStringIndexJ;

			bstr_HoleTopFace = ConvertToBSTR(iStringHoleTopFace);

			IXMLDOMElementPtr pElement_HoleTopFace;
			pDoc->createElement(bstr_HoleTopFace, &pElement_HoleTopFace);
			AppendChildToParent(pElement_HoleTopFace, pElement_HoleTopFaceNode);

			char* StringIndexTopFace = new char;
			sprintf(StringIndexTopFace, "%d", EachTopFace->Tag());

			BSTR bstr_HoleTopFaceTag;
			NXString iStringHoleTopFaceTag = StringIndexTopFace;

			bstr_HoleTopFaceTag = ConvertToBSTR(iStringHoleTopFaceTag);
			pElement_HoleTopFace->put_text(bstr_HoleTopFaceTag);
		}
		////倒斜角面
		//BSTR bstr_BossTopFaceNode;
		//NXString iStringBossTopFaceNode = "倒斜角面";

		//bstr_BossTopFaceNode = ConvertToBSTR(iStringBossTopFaceNode);

		//IXMLDOMElementPtr pElement_BossTopFaceNode;
		//pDoc->createElement(bstr_BossTopFaceNode, &pElement_BossTopFaceNode);
		//AppendChildToParent(pElement_BossTopFaceNode, pElement_HoleFeatureNode);//g

		//for (std::vector<Face*>::size_type j = 0; j != EachHole->RightVector.size(); j++)//g
		//{
		//	Face* EachTopFace = EachHole->RightVector[j];//g

		//	int indexJ = j + 1;
		//	char* StringIndexJ = new char;
		//	sprintf(StringIndexJ, "%d", indexJ);
		//	NXString NXStringIndexJ = StringIndexJ;

		//	BSTR bstr_BossTopFace;
		//	NXString iStringBossTopFace = "倒斜角面." + NXStringIndexJ;

		//	bstr_BossTopFace = ConvertToBSTR(iStringBossTopFace);

		//	IXMLDOMElementPtr pElement_BossTopFace;
		//	pDoc->createElement(bstr_BossTopFace, &pElement_BossTopFace);
		//	AppendChildToParent(pElement_BossTopFace, pElement_BossTopFaceNode);

		//	char* StringIndexTopFace = new char;
		//	sprintf(StringIndexTopFace, "%d", EachTopFace->Tag());

		//	BSTR bstr_BossTopFaceTag;
		//	NXString iStringBossTopFaceTag = StringIndexTopFace;

		//	bstr_BossTopFaceTag = ConvertToBSTR(iStringBossTopFaceTag);
		//	pElement_BossTopFace->put_text(bstr_BossTopFaceTag);
		//}

		//孔壁
		BSTR bstr_HoleSideFaceNode;
		NXString iStringHoleSideFaceNode = "侧面";//g

		bstr_HoleSideFaceNode = ConvertToBSTR(iStringHoleSideFaceNode);

		IXMLDOMElementPtr pElement_HoleSideFaceNode;
		pDoc->createElement(bstr_HoleSideFaceNode, &pElement_HoleSideFaceNode);
		AppendChildToParent(pElement_HoleSideFaceNode, pElement_HoleFeatureNode);

		for (std::vector<Face*>::size_type j = 0; j != EachHole->InWallVector.size(); j++)//g
		{
			Face* EachSideFace = EachHole->InWallVector[j];//g

			int indexJ = j + 1;
			char* StringIndexJ = new char;
			sprintf(StringIndexJ, "%d", indexJ);
			NXString NXStringIndexJ = StringIndexJ;

			BSTR bstr_HoleSideFace;
			NXString iStringHoleSideFace = "侧面." + NXStringIndexJ;//g

			bstr_HoleSideFace = ConvertToBSTR(iStringHoleSideFace);

			IXMLDOMElementPtr pElement_HoleSideFace;
			pDoc->createElement(bstr_HoleSideFace, &pElement_HoleSideFace);
			AppendChildToParent(pElement_HoleSideFace, pElement_HoleSideFaceNode);//jie for shang

			char* StringIndexSideFace = new char;
			sprintf(StringIndexSideFace, "%d", EachSideFace->Tag());

			BSTR bstr_HoleSideFaceTag;
			NXString iStringHoleSideFaceTag = StringIndexSideFace;

			bstr_HoleSideFaceTag = ConvertToBSTR(iStringHoleSideFaceTag);
			pElement_HoleSideFace->put_text(bstr_HoleSideFaceTag);
		}
	    
		//孔倒角
		/*BSTR bstr_HoleCornerFaceNode;
		NXString iStringHoleCornerFaceNode = "倒角面";

		bstr_HoleCornerFaceNode = ConvertToBSTR(iStringHoleCornerFaceNode);

		IXMLDOMElementPtr pElement_HoleCornerFaceNode;
		pDoc->createElement(bstr_HoleCornerFaceNode, &pElement_HoleCornerFaceNode);
		AppendChildToParent(pElement_HoleCornerFaceNode, pElement_HoleFeatureNode);

		for (std::vector<Face*>::size_type j = 0; j != EachHole->CornerVector.size(); j++)
		{
			Face* EachCornerFace = EachHole->CornerVector[j];

			int indexJ = j + 1;
			char* StringIndexJ = new char;
			sprintf(StringIndexJ, "%d", indexJ);
			NXString NXStringIndexJ = StringIndexJ;

			BSTR bstr_HoleCornerFace;
			NXString iStringHoleCornerFace = "倒角面." + NXStringIndexJ;

			bstr_HoleCornerFace = ConvertToBSTR(iStringHoleCornerFace);

			IXMLDOMElementPtr pElement_HoleCornerFace;
			pDoc->createElement(bstr_HoleCornerFace, &pElement_HoleCornerFace);
			AppendChildToParent(pElement_HoleCornerFace, pElement_HoleCornerFaceNode);

			char* StringIndexCornerFace = new char;
			sprintf(StringIndexCornerFace, "%d", EachCornerFace->Tag());

			BSTR bstr_HoleCornerFaceTag;
			NXString iStringHoleCornerFaceTag = StringIndexCornerFace;

			bstr_HoleCornerFaceTag = ConvertToBSTR(iStringHoleCornerFaceTag);
			pElement_HoleCornerFace->put_text(bstr_HoleCornerFaceTag);
		}*/

		//孔底面
		BSTR bstr_HoleBottomFaceNode;
		NXString iStringHoleBottomFaceNode = "底面";

		bstr_HoleBottomFaceNode = ConvertToBSTR(iStringHoleBottomFaceNode);

		IXMLDOMElementPtr pElement_HoleBottomFaceNode;
		pDoc->createElement(bstr_HoleBottomFaceNode, &pElement_HoleBottomFaceNode);
		AppendChildToParent(pElement_HoleBottomFaceNode, pElement_HoleFeatureNode);

		for (std::vector<Face*>::size_type j = 0; j != EachHole->BottomFaceVector.size(); j++)
		{
			Face* EachBottomFace = EachHole->BottomFaceVector[j];

			int indexJ = j + 1;
			char* StringIndexJ = new char;
			sprintf(StringIndexJ, "%d", indexJ);
			NXString NXStringIndexJ = StringIndexJ;

			BSTR bstr_HoleBottomFace;
			NXString iStringHoleBottomFace = "底面." + NXStringIndexJ;

			bstr_HoleBottomFace = ConvertToBSTR(iStringHoleBottomFace);

			IXMLDOMElementPtr pElement_HoleBottomFace;
			pDoc->createElement(bstr_HoleBottomFace, &pElement_HoleBottomFace);
			AppendChildToParent(pElement_HoleBottomFace, pElement_HoleBottomFaceNode);

			char* StringIndexBottomFace = new char;
			sprintf(StringIndexBottomFace, "%d", EachBottomFace->Tag());

			BSTR bstr_HoleBottomFaceTag;
			NXString iStringHoleBottomFaceTag = StringIndexBottomFace;

			bstr_HoleBottomFaceTag = ConvertToBSTR(iStringHoleBottomFaceTag);
			pElement_HoleBottomFace->put_text(bstr_HoleBottomFaceTag);
		}
	}
	//壁面通孔
	for (std::vector<WallNormalHole*>::size_type i = 0; i != WallNormalHoleSpyResult.size(); i++) //G
	{
		WallNormalHole* EachHole = WallNormalHoleSpyResult[i];//G

		int indexI = i + 1;
		char* StringIndexI = new char;
		sprintf(StringIndexI, "%d", indexI);
		NXString NXStringIndexI = StringIndexI;

		BSTR bstr_HoleFeatureNode;
		NXString iStringHoleFeatureNode = "壁面通孔." + NXStringIndexI;//g

		bstr_HoleFeatureNode = ConvertToBSTR(iStringHoleFeatureNode);

		IXMLDOMElementPtr pElement_HoleFeatureNode;
		pDoc->createElement(bstr_HoleFeatureNode, &pElement_HoleFeatureNode);
		AppendChildToParent(pElement_HoleFeatureNode, pElement_Hole);

		//孔顶面
		BSTR bstr_HoleTopFaceNode;
		NXString iStringHoleTopFaceNode = "顶面";

		bstr_HoleTopFaceNode = ConvertToBSTR(iStringHoleTopFaceNode);

		IXMLDOMElementPtr pElement_HoleTopFaceNode;
		pDoc->createElement(bstr_HoleTopFaceNode, &pElement_HoleTopFaceNode);
		AppendChildToParent(pElement_HoleTopFaceNode, pElement_HoleFeatureNode);

		for (std::vector<Face*>::size_type j = 0; j != EachHole->TopFaceVector.size(); j++)
		{
			Face* EachTopFace = EachHole->TopFaceVector[j];

			int indexJ = j + 1;
			char* StringIndexJ = new char;
			sprintf(StringIndexJ, "%d", indexJ);
			NXString NXStringIndexJ = StringIndexJ;

			BSTR bstr_HoleTopFace;
			NXString iStringHoleTopFace = "顶面." + NXStringIndexJ;

			bstr_HoleTopFace = ConvertToBSTR(iStringHoleTopFace);

			IXMLDOMElementPtr pElement_HoleTopFace;
			pDoc->createElement(bstr_HoleTopFace, &pElement_HoleTopFace);
			AppendChildToParent(pElement_HoleTopFace, pElement_HoleTopFaceNode);

			char* StringIndexTopFace = new char;
			sprintf(StringIndexTopFace, "%d", EachTopFace->Tag());

			BSTR bstr_HoleTopFaceTag;
			NXString iStringHoleTopFaceTag = StringIndexTopFace;

			bstr_HoleTopFaceTag = ConvertToBSTR(iStringHoleTopFaceTag);
			pElement_HoleTopFace->put_text(bstr_HoleTopFaceTag);
		}
		////倒斜角面
		//BSTR bstr_BossTopFaceNode;
		//NXString iStringBossTopFaceNode = "倒斜角面";

		//bstr_BossTopFaceNode = ConvertToBSTR(iStringBossTopFaceNode);

		//IXMLDOMElementPtr pElement_BossTopFaceNode;
		//pDoc->createElement(bstr_BossTopFaceNode, &pElement_BossTopFaceNode);
		//AppendChildToParent(pElement_BossTopFaceNode, pElement_HoleFeatureNode);//g

		//for (std::vector<Face*>::size_type j = 0; j != EachHole->RightVector.size(); j++)//g
		//{
		//	Face* EachTopFace = EachHole->RightVector[j];//g

		//	int indexJ = j + 1;
		//	char* StringIndexJ = new char;
		//	sprintf(StringIndexJ, "%d", indexJ);
		//	NXString NXStringIndexJ = StringIndexJ;

		//	BSTR bstr_BossTopFace;
		//	NXString iStringBossTopFace = "倒斜角面." + NXStringIndexJ;

		//	bstr_BossTopFace = ConvertToBSTR(iStringBossTopFace);

		//	IXMLDOMElementPtr pElement_BossTopFace;
		//	pDoc->createElement(bstr_BossTopFace, &pElement_BossTopFace);
		//	AppendChildToParent(pElement_BossTopFace, pElement_BossTopFaceNode);

		//	char* StringIndexTopFace = new char;
		//	sprintf(StringIndexTopFace, "%d", EachTopFace->Tag());

		//	BSTR bstr_BossTopFaceTag;
		//	NXString iStringBossTopFaceTag = StringIndexTopFace;

		//	bstr_BossTopFaceTag = ConvertToBSTR(iStringBossTopFaceTag);
		//	pElement_BossTopFace->put_text(bstr_BossTopFaceTag);
		//}

		//孔壁
		BSTR bstr_HoleSideFaceNode;
		NXString iStringHoleSideFaceNode = "侧面";//g

		bstr_HoleSideFaceNode = ConvertToBSTR(iStringHoleSideFaceNode);

		IXMLDOMElementPtr pElement_HoleSideFaceNode;
		pDoc->createElement(bstr_HoleSideFaceNode, &pElement_HoleSideFaceNode);
		AppendChildToParent(pElement_HoleSideFaceNode, pElement_HoleFeatureNode);

		for (std::vector<Face*>::size_type j = 0; j != EachHole->InWallVector.size(); j++)//g
		{
			Face* EachSideFace = EachHole->InWallVector[j];//g

			int indexJ = j + 1;
			char* StringIndexJ = new char;
			sprintf(StringIndexJ, "%d", indexJ);
			NXString NXStringIndexJ = StringIndexJ;

			BSTR bstr_HoleSideFace;
			NXString iStringHoleSideFace = "侧面." + NXStringIndexJ;//g

			bstr_HoleSideFace = ConvertToBSTR(iStringHoleSideFace);

			IXMLDOMElementPtr pElement_HoleSideFace;
			pDoc->createElement(bstr_HoleSideFace, &pElement_HoleSideFace);
			AppendChildToParent(pElement_HoleSideFace, pElement_HoleSideFaceNode);//jie for shang

			char* StringIndexSideFace = new char;
			sprintf(StringIndexSideFace, "%d", EachSideFace->Tag());

			BSTR bstr_HoleSideFaceTag;
			NXString iStringHoleSideFaceTag = StringIndexSideFace;

			bstr_HoleSideFaceTag = ConvertToBSTR(iStringHoleSideFaceTag);
			pElement_HoleSideFace->put_text(bstr_HoleSideFaceTag);
		}
	    
		//孔倒角
		/*BSTR bstr_HoleCornerFaceNode;
		NXString iStringHoleCornerFaceNode = "倒角面";

		bstr_HoleCornerFaceNode = ConvertToBSTR(iStringHoleCornerFaceNode);

		IXMLDOMElementPtr pElement_HoleCornerFaceNode;
		pDoc->createElement(bstr_HoleCornerFaceNode, &pElement_HoleCornerFaceNode);
		AppendChildToParent(pElement_HoleCornerFaceNode, pElement_HoleFeatureNode);

		for (std::vector<Face*>::size_type j = 0; j != EachHole->CornerVector.size(); j++)
		{
			Face* EachCornerFace = EachHole->CornerVector[j];

			int indexJ = j + 1;
			char* StringIndexJ = new char;
			sprintf(StringIndexJ, "%d", indexJ);
			NXString NXStringIndexJ = StringIndexJ;

			BSTR bstr_HoleCornerFace;
			NXString iStringHoleCornerFace = "倒角面." + NXStringIndexJ;

			bstr_HoleCornerFace = ConvertToBSTR(iStringHoleCornerFace);

			IXMLDOMElementPtr pElement_HoleCornerFace;
			pDoc->createElement(bstr_HoleCornerFace, &pElement_HoleCornerFace);
			AppendChildToParent(pElement_HoleCornerFace, pElement_HoleCornerFaceNode);

			char* StringIndexCornerFace = new char;
			sprintf(StringIndexCornerFace, "%d", EachCornerFace->Tag());

			BSTR bstr_HoleCornerFaceTag;
			NXString iStringHoleCornerFaceTag = StringIndexCornerFace;

			bstr_HoleCornerFaceTag = ConvertToBSTR(iStringHoleCornerFaceTag);
			pElement_HoleCornerFace->put_text(bstr_HoleCornerFaceTag);
		}*/

		//孔底面
		BSTR bstr_HoleBottomFaceNode;
		NXString iStringHoleBottomFaceNode = "底面";

		bstr_HoleBottomFaceNode = ConvertToBSTR(iStringHoleBottomFaceNode);

		IXMLDOMElementPtr pElement_HoleBottomFaceNode;
		pDoc->createElement(bstr_HoleBottomFaceNode, &pElement_HoleBottomFaceNode);
		AppendChildToParent(pElement_HoleBottomFaceNode, pElement_HoleFeatureNode);

		for (std::vector<Face*>::size_type j = 0; j != EachHole->BottomFaceVector.size(); j++)
		{
			Face* EachBottomFace = EachHole->BottomFaceVector[j];

			int indexJ = j + 1;
			char* StringIndexJ = new char;
			sprintf(StringIndexJ, "%d", indexJ);
			NXString NXStringIndexJ = StringIndexJ;

			BSTR bstr_HoleBottomFace;
			NXString iStringHoleBottomFace = "底面." + NXStringIndexJ;

			bstr_HoleBottomFace = ConvertToBSTR(iStringHoleBottomFace);

			IXMLDOMElementPtr pElement_HoleBottomFace;
			pDoc->createElement(bstr_HoleBottomFace, &pElement_HoleBottomFace);
			AppendChildToParent(pElement_HoleBottomFace, pElement_HoleBottomFaceNode);

			char* StringIndexBottomFace = new char;
			sprintf(StringIndexBottomFace, "%d", EachBottomFace->Tag());

			BSTR bstr_HoleBottomFaceTag;
			NXString iStringHoleBottomFaceTag = StringIndexBottomFace;

			bstr_HoleBottomFaceTag = ConvertToBSTR(iStringHoleBottomFaceTag);
			pElement_HoleBottomFace->put_text(bstr_HoleBottomFaceTag);
		}
	}
	//壁面倒斜角复合通孔
	for (std::vector<WallCircleCombineHole*>::size_type i = 0; i != WallCircleCombineHoleSpyResult.size(); i++) //G
	{
		WallCircleCombineHole* EachHole = WallCircleCombineHoleSpyResult[i];//G

		int indexI = i + 1;
		char* StringIndexI = new char;
		sprintf(StringIndexI, "%d", indexI);
		NXString NXStringIndexI = StringIndexI;

		BSTR bstr_HoleFeatureNode;
		NXString iStringHoleFeatureNode = "壁面倒斜角复合通孔." + NXStringIndexI;//g

		bstr_HoleFeatureNode = ConvertToBSTR(iStringHoleFeatureNode);

		IXMLDOMElementPtr pElement_HoleFeatureNode;
		pDoc->createElement(bstr_HoleFeatureNode, &pElement_HoleFeatureNode);
		AppendChildToParent(pElement_HoleFeatureNode, pElement_Hole);

		//孔顶面
		BSTR bstr_HoleTopFaceNode;
		NXString iStringHoleTopFaceNode = "顶面";

		bstr_HoleTopFaceNode = ConvertToBSTR(iStringHoleTopFaceNode);

		IXMLDOMElementPtr pElement_HoleTopFaceNode;
		pDoc->createElement(bstr_HoleTopFaceNode, &pElement_HoleTopFaceNode);
		AppendChildToParent(pElement_HoleTopFaceNode, pElement_HoleFeatureNode);

		for (std::vector<Face*>::size_type j = 0; j != EachHole->TopFaceVector.size(); j++)
		{
			Face* EachTopFace = EachHole->TopFaceVector[j];

			int indexJ = j + 1;
			char* StringIndexJ = new char;
			sprintf(StringIndexJ, "%d", indexJ);
			NXString NXStringIndexJ = StringIndexJ;

			BSTR bstr_HoleTopFace;
			NXString iStringHoleTopFace = "顶面." + NXStringIndexJ;

			bstr_HoleTopFace = ConvertToBSTR(iStringHoleTopFace);

			IXMLDOMElementPtr pElement_HoleTopFace;
			pDoc->createElement(bstr_HoleTopFace, &pElement_HoleTopFace);
			AppendChildToParent(pElement_HoleTopFace, pElement_HoleTopFaceNode);

			char* StringIndexTopFace = new char;
			sprintf(StringIndexTopFace, "%d", EachTopFace->Tag());

			BSTR bstr_HoleTopFaceTag;
			NXString iStringHoleTopFaceTag = StringIndexTopFace;

			bstr_HoleTopFaceTag = ConvertToBSTR(iStringHoleTopFaceTag);
			pElement_HoleTopFace->put_text(bstr_HoleTopFaceTag);
		}
		//上侧面
		BSTR bstr_BossTopFaceNode;
		NXString iStringBossTopFaceNode = "上侧面";//g

		bstr_BossTopFaceNode = ConvertToBSTR(iStringBossTopFaceNode);

		IXMLDOMElementPtr pElement_BossTopFaceNode;
		pDoc->createElement(bstr_BossTopFaceNode, &pElement_BossTopFaceNode);
		AppendChildToParent(pElement_BossTopFaceNode, pElement_HoleFeatureNode);//g

		for (std::vector<Face*>::size_type j = 0; j != EachHole->TopInWallVector.size(); j++)//g
		{
			Face* EachTopFace = EachHole->TopInWallVector[j];//g

			int indexJ = j + 1;
			char* StringIndexJ = new char;
			sprintf(StringIndexJ, "%d", indexJ);
			NXString NXStringIndexJ = StringIndexJ;

			BSTR bstr_BossTopFace;
			NXString iStringBossTopFace = "上侧面." + NXStringIndexJ;//g

			bstr_BossTopFace = ConvertToBSTR(iStringBossTopFace);

			IXMLDOMElementPtr pElement_BossTopFace;
			pDoc->createElement(bstr_BossTopFace, &pElement_BossTopFace);
			AppendChildToParent(pElement_BossTopFace, pElement_BossTopFaceNode);

			char* StringIndexTopFace = new char;
			sprintf(StringIndexTopFace, "%d", EachTopFace->Tag());

			BSTR bstr_BossTopFaceTag;
			NXString iStringBossTopFaceTag = StringIndexTopFace;

			bstr_BossTopFaceTag = ConvertToBSTR(iStringBossTopFaceTag);
			pElement_BossTopFace->put_text(bstr_BossTopFaceTag);
		}

		/*倒圆角面*/
		BSTR bstr_HoleSideFaceNode;
		NXString iStringHoleSideFaceNode = "倒圆角面";//g

		bstr_HoleSideFaceNode = ConvertToBSTR(iStringHoleSideFaceNode);

		IXMLDOMElementPtr pElement_HoleSideFaceNode;
		pDoc->createElement(bstr_HoleSideFaceNode, &pElement_HoleSideFaceNode);
		AppendChildToParent(pElement_HoleSideFaceNode, pElement_HoleFeatureNode);

		for (std::vector<Face*>::size_type j = 0; j != EachHole->CircleVector.size(); j++)//g
		{
			Face* EachSideFace = EachHole->CircleVector[j];//g

			int indexJ = j + 1;
			char* StringIndexJ = new char;
			sprintf(StringIndexJ, "%d", indexJ);
			NXString NXStringIndexJ = StringIndexJ;

			BSTR bstr_HoleSideFace;
			NXString iStringHoleSideFace = "倒圆角面." + NXStringIndexJ;//g

			bstr_HoleSideFace = ConvertToBSTR(iStringHoleSideFace);

			IXMLDOMElementPtr pElement_HoleSideFace;
			pDoc->createElement(bstr_HoleSideFace, &pElement_HoleSideFace);
			AppendChildToParent(pElement_HoleSideFace, pElement_HoleSideFaceNode);//jie for shang

			char* StringIndexSideFace = new char;
			sprintf(StringIndexSideFace, "%d", EachSideFace->Tag());

			BSTR bstr_HoleSideFaceTag;
			NXString iStringHoleSideFaceTag = StringIndexSideFace;

			bstr_HoleSideFaceTag = ConvertToBSTR(iStringHoleSideFaceTag);
			pElement_HoleSideFace->put_text(bstr_HoleSideFaceTag);
		}
	    
		//过渡平面
		BSTR bstr_HoleCornerFaceNode;
		NXString iStringHoleCornerFaceNode = "过渡平面";

		bstr_HoleCornerFaceNode = ConvertToBSTR(iStringHoleCornerFaceNode);

		IXMLDOMElementPtr pElement_HoleCornerFaceNode;
		pDoc->createElement(bstr_HoleCornerFaceNode, &pElement_HoleCornerFaceNode);
		AppendChildToParent(pElement_HoleCornerFaceNode, pElement_HoleFeatureNode);

		for (std::vector<Face*>::size_type j = 0; j != EachHole->PlanerVector.size(); j++)
		{
			Face* EachCornerFace = EachHole->PlanerVector[j];

			int indexJ = j + 1;
			char* StringIndexJ = new char;
			sprintf(StringIndexJ, "%d", indexJ);
			NXString NXStringIndexJ = StringIndexJ;

			BSTR bstr_HoleCornerFace;
			NXString iStringHoleCornerFace = "过渡平面." + NXStringIndexJ;

			bstr_HoleCornerFace = ConvertToBSTR(iStringHoleCornerFace);

			IXMLDOMElementPtr pElement_HoleCornerFace;
			pDoc->createElement(bstr_HoleCornerFace, &pElement_HoleCornerFace);
			AppendChildToParent(pElement_HoleCornerFace, pElement_HoleCornerFaceNode);

			char* StringIndexCornerFace = new char;
			sprintf(StringIndexCornerFace, "%d", EachCornerFace->Tag());

			BSTR bstr_HoleCornerFaceTag;
			NXString iStringHoleCornerFaceTag = StringIndexCornerFace;

			bstr_HoleCornerFaceTag = ConvertToBSTR(iStringHoleCornerFaceTag);
			pElement_HoleCornerFace->put_text(bstr_HoleCornerFaceTag);
		}

		//下侧面
		BSTR bstr_BossSideFaceNode;
		NXString iStringBossSideFaceNode = "下侧面";

		bstr_BossSideFaceNode = ConvertToBSTR(iStringBossSideFaceNode);

		IXMLDOMElementPtr pElement_BossSideFaceNode;
		pDoc->createElement(bstr_BossSideFaceNode, &pElement_BossSideFaceNode);
		AppendChildToParent(pElement_BossSideFaceNode, pElement_HoleFeatureNode);

		for (std::vector<Face*>::size_type j = 0; j != EachHole->BottomInWallVector.size(); j++)
		{
			Face* EachSideFace = EachHole->BottomInWallVector[j];

			int indexJ = j + 1;
			char* StringIndexJ = new char;
			sprintf(StringIndexJ, "%d", indexJ);
			NXString NXStringIndexJ = StringIndexJ;

			BSTR bstr_BossSideFace;
			NXString iStringBossSideFace = "下侧面." + NXStringIndexJ;

			bstr_BossSideFace = ConvertToBSTR(iStringBossSideFace);

			IXMLDOMElementPtr pElement_BossSideFace;
			pDoc->createElement(bstr_BossSideFace, &pElement_BossSideFace);
			AppendChildToParent(pElement_BossSideFace, pElement_BossSideFaceNode);

			char* StringIndexSideFace = new char;
			sprintf(StringIndexSideFace, "%d", EachSideFace->Tag());

			BSTR bstr_BossSideFaceTag;
			NXString iStringBossSideFaceTag = StringIndexSideFace;

			bstr_BossSideFaceTag = ConvertToBSTR(iStringBossSideFaceTag);
			pElement_BossSideFace->put_text(bstr_BossSideFaceTag);
		}


		//孔底面
		BSTR bstr_HoleBottomFaceNode;
		NXString iStringHoleBottomFaceNode = "底面";

		bstr_HoleBottomFaceNode = ConvertToBSTR(iStringHoleBottomFaceNode);

		IXMLDOMElementPtr pElement_HoleBottomFaceNode;
		pDoc->createElement(bstr_HoleBottomFaceNode, &pElement_HoleBottomFaceNode);
		AppendChildToParent(pElement_HoleBottomFaceNode, pElement_HoleFeatureNode);

		for (std::vector<Face*>::size_type j = 0; j != EachHole->BottomFaceVector.size(); j++)
		{
			Face* EachBottomFace = EachHole->BottomFaceVector[j];

			int indexJ = j + 1;
			char* StringIndexJ = new char;
			sprintf(StringIndexJ, "%d", indexJ);
			NXString NXStringIndexJ = StringIndexJ;

			BSTR bstr_HoleBottomFace;
			NXString iStringHoleBottomFace = "底面." + NXStringIndexJ;

			bstr_HoleBottomFace = ConvertToBSTR(iStringHoleBottomFace);

			IXMLDOMElementPtr pElement_HoleBottomFace;
			pDoc->createElement(bstr_HoleBottomFace, &pElement_HoleBottomFace);
			AppendChildToParent(pElement_HoleBottomFace, pElement_HoleBottomFaceNode);

			char* StringIndexBottomFace = new char;
			sprintf(StringIndexBottomFace, "%d", EachBottomFace->Tag());

			BSTR bstr_HoleBottomFaceTag;
			NXString iStringHoleBottomFaceTag = StringIndexBottomFace;

			bstr_HoleBottomFaceTag = ConvertToBSTR(iStringHoleBottomFaceTag);
			pElement_HoleBottomFace->put_text(bstr_HoleBottomFaceTag);
		}
	}

	//筋面复合孔
	for (std::vector<RibCombineHole*>::size_type i = 0; i != RibCombineHoleSpyResult.size(); i++) //G
	{
		RibCombineHole* EachHole = RibCombineHoleSpyResult[i];//G

		int indexI = i + 1;
		char* StringIndexI = new char;
		sprintf(StringIndexI, "%d", indexI);
		NXString NXStringIndexI = StringIndexI;

		BSTR bstr_HoleFeatureNode;
		NXString iStringHoleFeatureNode = "筋面复合孔." + NXStringIndexI;//g

		bstr_HoleFeatureNode = ConvertToBSTR(iStringHoleFeatureNode);

		IXMLDOMElementPtr pElement_HoleFeatureNode;
		pDoc->createElement(bstr_HoleFeatureNode, &pElement_HoleFeatureNode);
		AppendChildToParent(pElement_HoleFeatureNode, pElement_Hole);

		//孔顶面
		BSTR bstr_HoleTopFaceNode;
		NXString iStringHoleTopFaceNode = "顶面";

		bstr_HoleTopFaceNode = ConvertToBSTR(iStringHoleTopFaceNode);

		IXMLDOMElementPtr pElement_HoleTopFaceNode;
		pDoc->createElement(bstr_HoleTopFaceNode, &pElement_HoleTopFaceNode);
		AppendChildToParent(pElement_HoleTopFaceNode, pElement_HoleFeatureNode);

		for (std::vector<Face*>::size_type j = 0; j != EachHole->TopFaceVector.size(); j++)
		{
			Face* EachTopFace = EachHole->TopFaceVector[j];

			int indexJ = j + 1;
			char* StringIndexJ = new char;
			sprintf(StringIndexJ, "%d", indexJ);
			NXString NXStringIndexJ = StringIndexJ;

			BSTR bstr_HoleTopFace;
			NXString iStringHoleTopFace = "顶面." + NXStringIndexJ;

			bstr_HoleTopFace = ConvertToBSTR(iStringHoleTopFace);

			IXMLDOMElementPtr pElement_HoleTopFace;
			pDoc->createElement(bstr_HoleTopFace, &pElement_HoleTopFace);
			AppendChildToParent(pElement_HoleTopFace, pElement_HoleTopFaceNode);

			char* StringIndexTopFace = new char;
			sprintf(StringIndexTopFace, "%d", EachTopFace->Tag());

			BSTR bstr_HoleTopFaceTag;
			NXString iStringHoleTopFaceTag = StringIndexTopFace;

			bstr_HoleTopFaceTag = ConvertToBSTR(iStringHoleTopFaceTag);
			pElement_HoleTopFace->put_text(bstr_HoleTopFaceTag);
		}
		//倒圆角面
		BSTR bstr_BossTopFaceNode;
		NXString iStringBossTopFaceNode = "倒圆角面";//g

		bstr_BossTopFaceNode = ConvertToBSTR(iStringBossTopFaceNode);

		IXMLDOMElementPtr pElement_BossTopFaceNode;
		pDoc->createElement(bstr_BossTopFaceNode, &pElement_BossTopFaceNode);
		AppendChildToParent(pElement_BossTopFaceNode, pElement_HoleFeatureNode);//g

		for (std::vector<Face*>::size_type j = 0; j != EachHole->CircleVector.size(); j++)//g
		{
			Face* EachTopFace = EachHole->CircleVector[j];//g

			int indexJ = j + 1;
			char* StringIndexJ = new char;
			sprintf(StringIndexJ, "%d", indexJ);
			NXString NXStringIndexJ = StringIndexJ;

			BSTR bstr_BossTopFace;
			NXString iStringBossTopFace = "倒圆角面." + NXStringIndexJ;//g

			bstr_BossTopFace = ConvertToBSTR(iStringBossTopFace);

			IXMLDOMElementPtr pElement_BossTopFace;
			pDoc->createElement(bstr_BossTopFace, &pElement_BossTopFace);
			AppendChildToParent(pElement_BossTopFace, pElement_BossTopFaceNode);

			char* StringIndexTopFace = new char;
			sprintf(StringIndexTopFace, "%d", EachTopFace->Tag());

			BSTR bstr_BossTopFaceTag;
			NXString iStringBossTopFaceTag = StringIndexTopFace;

			bstr_BossTopFaceTag = ConvertToBSTR(iStringBossTopFaceTag);
			pElement_BossTopFace->put_text(bstr_BossTopFaceTag);
		}

		/*过渡平面*/
		BSTR bstr_HoleSideFaceNode;
		NXString iStringHoleSideFaceNode = "过渡平面";//g

		bstr_HoleSideFaceNode = ConvertToBSTR(iStringHoleSideFaceNode);

		IXMLDOMElementPtr pElement_HoleSideFaceNode;
		pDoc->createElement(bstr_HoleSideFaceNode, &pElement_HoleSideFaceNode);
		AppendChildToParent(pElement_HoleSideFaceNode, pElement_HoleFeatureNode);

		for (std::vector<Face*>::size_type j = 0; j != EachHole->PlanerVector.size(); j++)//g
		{
			Face* EachSideFace = EachHole->PlanerVector[j];//g

			int indexJ = j + 1;
			char* StringIndexJ = new char;
			sprintf(StringIndexJ, "%d", indexJ);
			NXString NXStringIndexJ = StringIndexJ;

			BSTR bstr_HoleSideFace;
			NXString iStringHoleSideFace = "过渡平面." + NXStringIndexJ;//g

			bstr_HoleSideFace = ConvertToBSTR(iStringHoleSideFace);

			IXMLDOMElementPtr pElement_HoleSideFace;
			pDoc->createElement(bstr_HoleSideFace, &pElement_HoleSideFace);
			AppendChildToParent(pElement_HoleSideFace, pElement_HoleSideFaceNode);//jie for shang

			char* StringIndexSideFace = new char;
			sprintf(StringIndexSideFace, "%d", EachSideFace->Tag());

			BSTR bstr_HoleSideFaceTag;
			NXString iStringHoleSideFaceTag = StringIndexSideFace;

			bstr_HoleSideFaceTag = ConvertToBSTR(iStringHoleSideFaceTag);
			pElement_HoleSideFace->put_text(bstr_HoleSideFaceTag);
		}

		//侧面
		BSTR bstr_HoleCornerFaceNode;
		NXString iStringHoleCornerFaceNode = "侧面";

		bstr_HoleCornerFaceNode = ConvertToBSTR(iStringHoleCornerFaceNode);

		IXMLDOMElementPtr pElement_HoleCornerFaceNode;
		pDoc->createElement(bstr_HoleCornerFaceNode, &pElement_HoleCornerFaceNode);
		AppendChildToParent(pElement_HoleCornerFaceNode, pElement_HoleFeatureNode);

		for (std::vector<Face*>::size_type j = 0; j != EachHole->InWallVector.size(); j++)
		{
			Face* EachCornerFace = EachHole->InWallVector[j];

			int indexJ = j + 1;
			char* StringIndexJ = new char;
			sprintf(StringIndexJ, "%d", indexJ);
			NXString NXStringIndexJ = StringIndexJ;

			BSTR bstr_HoleCornerFace;
			NXString iStringHoleCornerFace = "侧面." + NXStringIndexJ;

			bstr_HoleCornerFace = ConvertToBSTR(iStringHoleCornerFace);

			IXMLDOMElementPtr pElement_HoleCornerFace;
			pDoc->createElement(bstr_HoleCornerFace, &pElement_HoleCornerFace);
			AppendChildToParent(pElement_HoleCornerFace, pElement_HoleCornerFaceNode);

			char* StringIndexCornerFace = new char;
			sprintf(StringIndexCornerFace, "%d", EachCornerFace->Tag());

			BSTR bstr_HoleCornerFaceTag;
			NXString iStringHoleCornerFaceTag = StringIndexCornerFace;

			bstr_HoleCornerFaceTag = ConvertToBSTR(iStringHoleCornerFaceTag);
			pElement_HoleCornerFace->put_text(bstr_HoleCornerFaceTag);
		}

		//倒斜角面
		BSTR bstr_BossSideFaceNode;
		NXString iStringBossSideFaceNode = "倒斜角面";

		bstr_BossSideFaceNode = ConvertToBSTR(iStringBossSideFaceNode);

		IXMLDOMElementPtr pElement_BossSideFaceNode;
		pDoc->createElement(bstr_BossSideFaceNode, &pElement_BossSideFaceNode);
		AppendChildToParent(pElement_BossSideFaceNode, pElement_HoleFeatureNode);

		for (std::vector<Face*>::size_type j = 0; j != EachHole->RightVector.size(); j++)
		{
			Face* EachSideFace = EachHole->RightVector[j];

			int indexJ = j + 1;
			char* StringIndexJ = new char;
			sprintf(StringIndexJ, "%d", indexJ);
			NXString NXStringIndexJ = StringIndexJ;

			BSTR bstr_BossSideFace;
			NXString iStringBossSideFace = "倒斜角面." + NXStringIndexJ;

			bstr_BossSideFace = ConvertToBSTR(iStringBossSideFace);

			IXMLDOMElementPtr pElement_BossSideFace;
			pDoc->createElement(bstr_BossSideFace, &pElement_BossSideFace);
			AppendChildToParent(pElement_BossSideFace, pElement_BossSideFaceNode);

			char* StringIndexSideFace = new char;
			sprintf(StringIndexSideFace, "%d", EachSideFace->Tag());

			BSTR bstr_BossSideFaceTag;
			NXString iStringBossSideFaceTag = StringIndexSideFace;

			bstr_BossSideFaceTag = ConvertToBSTR(iStringBossSideFaceTag);
			pElement_BossSideFace->put_text(bstr_BossSideFaceTag);
		}


		//孔底面
		BSTR bstr_HoleBottomFaceNode;
		NXString iStringHoleBottomFaceNode = "底面";

		bstr_HoleBottomFaceNode = ConvertToBSTR(iStringHoleBottomFaceNode);

		IXMLDOMElementPtr pElement_HoleBottomFaceNode;
		pDoc->createElement(bstr_HoleBottomFaceNode, &pElement_HoleBottomFaceNode);
		AppendChildToParent(pElement_HoleBottomFaceNode, pElement_HoleFeatureNode);

		for (std::vector<Face*>::size_type j = 0; j != EachHole->BottomFaceVector.size(); j++)
		{
			Face* EachBottomFace = EachHole->BottomFaceVector[j];

			int indexJ = j + 1;
			char* StringIndexJ = new char;
			sprintf(StringIndexJ, "%d", indexJ);
			NXString NXStringIndexJ = StringIndexJ;

			BSTR bstr_HoleBottomFace;
			NXString iStringHoleBottomFace = "底面." + NXStringIndexJ;

			bstr_HoleBottomFace = ConvertToBSTR(iStringHoleBottomFace);

			IXMLDOMElementPtr pElement_HoleBottomFace;
			pDoc->createElement(bstr_HoleBottomFace, &pElement_HoleBottomFace);
			AppendChildToParent(pElement_HoleBottomFace, pElement_HoleBottomFaceNode);

			char* StringIndexBottomFace = new char;
			sprintf(StringIndexBottomFace, "%d", EachBottomFace->Tag());

			BSTR bstr_HoleBottomFaceTag;
			NXString iStringHoleBottomFaceTag = StringIndexBottomFace;

			bstr_HoleBottomFaceTag = ConvertToBSTR(iStringHoleBottomFaceTag);
			pElement_HoleBottomFace->put_text(bstr_HoleBottomFaceTag);
		}
	}

	//凸台通孔
	for (std::vector<BossNormalHole*>::size_type i = 0; i != BossNormalHoleSpyResult.size(); i++) //G
	{
		BossNormalHole* EachHole = BossNormalHoleSpyResult[i];//G

		int indexI = i + 1;
		char* StringIndexI = new char;
		sprintf(StringIndexI, "%d", indexI);
		NXString NXStringIndexI = StringIndexI;

		BSTR bstr_HoleFeatureNode;
		NXString iStringHoleFeatureNode = "凸台通孔." + NXStringIndexI;//g

		bstr_HoleFeatureNode = ConvertToBSTR(iStringHoleFeatureNode);

		IXMLDOMElementPtr pElement_HoleFeatureNode;
		pDoc->createElement(bstr_HoleFeatureNode, &pElement_HoleFeatureNode);
		AppendChildToParent(pElement_HoleFeatureNode, pElement_Hole);

		//孔顶面
		BSTR bstr_HoleTopFaceNode;
		NXString iStringHoleTopFaceNode = "顶面";

		bstr_HoleTopFaceNode = ConvertToBSTR(iStringHoleTopFaceNode);

		IXMLDOMElementPtr pElement_HoleTopFaceNode;
		pDoc->createElement(bstr_HoleTopFaceNode, &pElement_HoleTopFaceNode);
		AppendChildToParent(pElement_HoleTopFaceNode, pElement_HoleFeatureNode);

		for (std::vector<Face*>::size_type j = 0; j != EachHole->TopFaceVector.size(); j++)
		{
			Face* EachTopFace = EachHole->TopFaceVector[j];

			int indexJ = j + 1;
			char* StringIndexJ = new char;
			sprintf(StringIndexJ, "%d", indexJ);
			NXString NXStringIndexJ = StringIndexJ;

			BSTR bstr_HoleTopFace;
			NXString iStringHoleTopFace = "顶面." + NXStringIndexJ;

			bstr_HoleTopFace = ConvertToBSTR(iStringHoleTopFace);

			IXMLDOMElementPtr pElement_HoleTopFace;
			pDoc->createElement(bstr_HoleTopFace, &pElement_HoleTopFace);
			AppendChildToParent(pElement_HoleTopFace, pElement_HoleTopFaceNode);

			char* StringIndexTopFace = new char;
			sprintf(StringIndexTopFace, "%d", EachTopFace->Tag());

			BSTR bstr_HoleTopFaceTag;
			NXString iStringHoleTopFaceTag = StringIndexTopFace;

			bstr_HoleTopFaceTag = ConvertToBSTR(iStringHoleTopFaceTag);
			pElement_HoleTopFace->put_text(bstr_HoleTopFaceTag);
		}
		//倒圆角面
		//BSTR bstr_BossTopFaceNode;
		//NXString iStringBossTopFaceNode = "倒圆角面";//g

		//bstr_BossTopFaceNode = ConvertToBSTR(iStringBossTopFaceNode);

		//IXMLDOMElementPtr pElement_BossTopFaceNode;
		//pDoc->createElement(bstr_BossTopFaceNode, &pElement_BossTopFaceNode);
		//AppendChildToParent(pElement_BossTopFaceNode, pElement_HoleFeatureNode);//g

		//for (std::vector<Face*>::size_type j = 0; j != EachHole->CircleVector.size(); j++)//g
		//{
		//	Face* EachTopFace = EachHole->CircleVector[j];//g

		//	int indexJ = j + 1;
		//	char* StringIndexJ = new char;
		//	sprintf(StringIndexJ, "%d", indexJ);
		//	NXString NXStringIndexJ = StringIndexJ;

		//	BSTR bstr_BossTopFace;
		//	NXString iStringBossTopFace = "倒圆角面." + NXStringIndexJ;//g

		//	bstr_BossTopFace = ConvertToBSTR(iStringBossTopFace);

		//	IXMLDOMElementPtr pElement_BossTopFace;
		//	pDoc->createElement(bstr_BossTopFace, &pElement_BossTopFace);
		//	AppendChildToParent(pElement_BossTopFace, pElement_BossTopFaceNode);

		//	char* StringIndexTopFace = new char;
		//	sprintf(StringIndexTopFace, "%d", EachTopFace->Tag());

		//	BSTR bstr_BossTopFaceTag;
		//	NXString iStringBossTopFaceTag = StringIndexTopFace;

		//	bstr_BossTopFaceTag = ConvertToBSTR(iStringBossTopFaceTag);
		//	pElement_BossTopFace->put_text(bstr_BossTopFaceTag);
		//}

		/*过渡平面*/
		//BSTR bstr_HoleSideFaceNode;
		//NXString iStringHoleSideFaceNode = "过渡平面";//g

		//bstr_HoleSideFaceNode = ConvertToBSTR(iStringHoleSideFaceNode);

		//IXMLDOMElementPtr pElement_HoleSideFaceNode;
		//pDoc->createElement(bstr_HoleSideFaceNode, &pElement_HoleSideFaceNode);
		//AppendChildToParent(pElement_HoleSideFaceNode, pElement_HoleFeatureNode);

		//for (std::vector<Face*>::size_type j = 0; j != EachHole->PlanerVector.size(); j++)//g
		//{
		//	Face* EachSideFace = EachHole->PlanerVector[j];//g

		//	int indexJ = j + 1;
		//	char* StringIndexJ = new char;
		//	sprintf(StringIndexJ, "%d", indexJ);
		//	NXString NXStringIndexJ = StringIndexJ;

		//	BSTR bstr_HoleSideFace;
		//	NXString iStringHoleSideFace = "过渡平面." + NXStringIndexJ;//g

		//	bstr_HoleSideFace = ConvertToBSTR(iStringHoleSideFace);

		//	IXMLDOMElementPtr pElement_HoleSideFace;
		//	pDoc->createElement(bstr_HoleSideFace, &pElement_HoleSideFace);
		//	AppendChildToParent(pElement_HoleSideFace, pElement_HoleSideFaceNode);//jie for shang

		//	char* StringIndexSideFace = new char;
		//	sprintf(StringIndexSideFace, "%d", EachSideFace->Tag());

		//	BSTR bstr_HoleSideFaceTag;
		//	NXString iStringHoleSideFaceTag = StringIndexSideFace;

		//	bstr_HoleSideFaceTag = ConvertToBSTR(iStringHoleSideFaceTag);
		//	pElement_HoleSideFace->put_text(bstr_HoleSideFaceTag);
		//}

		//侧面
		BSTR bstr_HoleCornerFaceNode;
		NXString iStringHoleCornerFaceNode = "侧面";

		bstr_HoleCornerFaceNode = ConvertToBSTR(iStringHoleCornerFaceNode);

		IXMLDOMElementPtr pElement_HoleCornerFaceNode;
		pDoc->createElement(bstr_HoleCornerFaceNode, &pElement_HoleCornerFaceNode);
		AppendChildToParent(pElement_HoleCornerFaceNode, pElement_HoleFeatureNode);

		for (std::vector<Face*>::size_type j = 0; j != EachHole->InWallVector.size(); j++)
		{
			Face* EachCornerFace = EachHole->InWallVector[j];

			int indexJ = j + 1;
			char* StringIndexJ = new char;
			sprintf(StringIndexJ, "%d", indexJ);
			NXString NXStringIndexJ = StringIndexJ;

			BSTR bstr_HoleCornerFace;
			NXString iStringHoleCornerFace = "侧面." + NXStringIndexJ;

			bstr_HoleCornerFace = ConvertToBSTR(iStringHoleCornerFace);

			IXMLDOMElementPtr pElement_HoleCornerFace;
			pDoc->createElement(bstr_HoleCornerFace, &pElement_HoleCornerFace);
			AppendChildToParent(pElement_HoleCornerFace, pElement_HoleCornerFaceNode);

			char* StringIndexCornerFace = new char;
			sprintf(StringIndexCornerFace, "%d", EachCornerFace->Tag());

			BSTR bstr_HoleCornerFaceTag;
			NXString iStringHoleCornerFaceTag = StringIndexCornerFace;

			bstr_HoleCornerFaceTag = ConvertToBSTR(iStringHoleCornerFaceTag);
			pElement_HoleCornerFace->put_text(bstr_HoleCornerFaceTag);
		}

		//倒斜角面
		/*BSTR bstr_BossSideFaceNode;
		NXString iStringBossSideFaceNode = "倒斜角面";

		bstr_BossSideFaceNode = ConvertToBSTR(iStringBossSideFaceNode);

		IXMLDOMElementPtr pElement_BossSideFaceNode;
		pDoc->createElement(bstr_BossSideFaceNode, &pElement_BossSideFaceNode);
		AppendChildToParent(pElement_BossSideFaceNode, pElement_HoleFeatureNode);

		for (std::vector<Face*>::size_type j = 0; j != EachHole->RightVector.size(); j++)
		{
			Face* EachSideFace = EachHole->RightVector[j];

			int indexJ = j + 1;
			char* StringIndexJ = new char;
			sprintf(StringIndexJ, "%d", indexJ);
			NXString NXStringIndexJ = StringIndexJ;

			BSTR bstr_BossSideFace;
			NXString iStringBossSideFace = "倒斜角面." + NXStringIndexJ;

			bstr_BossSideFace = ConvertToBSTR(iStringBossSideFace);

			IXMLDOMElementPtr pElement_BossSideFace;
			pDoc->createElement(bstr_BossSideFace, &pElement_BossSideFace);
			AppendChildToParent(pElement_BossSideFace, pElement_BossSideFaceNode);

			char* StringIndexSideFace = new char;
			sprintf(StringIndexSideFace, "%d", EachSideFace->Tag());

			BSTR bstr_BossSideFaceTag;
			NXString iStringBossSideFaceTag = StringIndexSideFace;

			bstr_BossSideFaceTag = ConvertToBSTR(iStringBossSideFaceTag);
			pElement_BossSideFace->put_text(bstr_BossSideFaceTag);
		}*/


		//孔底面
		BSTR bstr_HoleBottomFaceNode;
		NXString iStringHoleBottomFaceNode = "底面";

		bstr_HoleBottomFaceNode = ConvertToBSTR(iStringHoleBottomFaceNode);

		IXMLDOMElementPtr pElement_HoleBottomFaceNode;
		pDoc->createElement(bstr_HoleBottomFaceNode, &pElement_HoleBottomFaceNode);
		AppendChildToParent(pElement_HoleBottomFaceNode, pElement_HoleFeatureNode);

		for (std::vector<Face*>::size_type j = 0; j != EachHole->BottomFaceVector.size(); j++)
		{
			Face* EachBottomFace = EachHole->BottomFaceVector[j];

			int indexJ = j + 1;
			char* StringIndexJ = new char;
			sprintf(StringIndexJ, "%d", indexJ);
			NXString NXStringIndexJ = StringIndexJ;

			BSTR bstr_HoleBottomFace;
			NXString iStringHoleBottomFace = "底面." + NXStringIndexJ;

			bstr_HoleBottomFace = ConvertToBSTR(iStringHoleBottomFace);

			IXMLDOMElementPtr pElement_HoleBottomFace;
			pDoc->createElement(bstr_HoleBottomFace, &pElement_HoleBottomFace);
			AppendChildToParent(pElement_HoleBottomFace, pElement_HoleBottomFaceNode);

			char* StringIndexBottomFace = new char;
			sprintf(StringIndexBottomFace, "%d", EachBottomFace->Tag());

			BSTR bstr_HoleBottomFaceTag;
			NXString iStringHoleBottomFaceTag = StringIndexBottomFace;

			bstr_HoleBottomFaceTag = ConvertToBSTR(iStringHoleBottomFaceTag);
			pElement_HoleBottomFace->put_text(bstr_HoleBottomFaceTag);
		}
	}

	//凸台单侧斜角通孔
	for (std::vector<BossOneRightHole*>::size_type i = 0; i != BossOneRightHoleSpyResult.size(); i++) //G
	{
		BossOneRightHole* EachHole = BossOneRightHoleSpyResult[i];//G

		int indexI = i + 1;
		char* StringIndexI = new char;
		sprintf(StringIndexI, "%d", indexI);
		NXString NXStringIndexI = StringIndexI;

		BSTR bstr_HoleFeatureNode;
		NXString iStringHoleFeatureNode = "凸台单侧斜角通孔." + NXStringIndexI;//g

		bstr_HoleFeatureNode = ConvertToBSTR(iStringHoleFeatureNode);

		IXMLDOMElementPtr pElement_HoleFeatureNode;
		pDoc->createElement(bstr_HoleFeatureNode, &pElement_HoleFeatureNode);
		AppendChildToParent(pElement_HoleFeatureNode, pElement_Hole);

		//孔顶面
		BSTR bstr_HoleTopFaceNode;
		NXString iStringHoleTopFaceNode = "顶面";

		bstr_HoleTopFaceNode = ConvertToBSTR(iStringHoleTopFaceNode);

		IXMLDOMElementPtr pElement_HoleTopFaceNode;
		pDoc->createElement(bstr_HoleTopFaceNode, &pElement_HoleTopFaceNode);
		AppendChildToParent(pElement_HoleTopFaceNode, pElement_HoleFeatureNode);

		for (std::vector<Face*>::size_type j = 0; j != EachHole->TopFaceVector.size(); j++)
		{
			Face* EachTopFace = EachHole->TopFaceVector[j];

			int indexJ = j + 1;
			char* StringIndexJ = new char;
			sprintf(StringIndexJ, "%d", indexJ);
			NXString NXStringIndexJ = StringIndexJ;

			BSTR bstr_HoleTopFace;
			NXString iStringHoleTopFace = "顶面." + NXStringIndexJ;

			bstr_HoleTopFace = ConvertToBSTR(iStringHoleTopFace);

			IXMLDOMElementPtr pElement_HoleTopFace;
			pDoc->createElement(bstr_HoleTopFace, &pElement_HoleTopFace);
			AppendChildToParent(pElement_HoleTopFace, pElement_HoleTopFaceNode);

			char* StringIndexTopFace = new char;
			sprintf(StringIndexTopFace, "%d", EachTopFace->Tag());

			BSTR bstr_HoleTopFaceTag;
			NXString iStringHoleTopFaceTag = StringIndexTopFace;

			bstr_HoleTopFaceTag = ConvertToBSTR(iStringHoleTopFaceTag);
			pElement_HoleTopFace->put_text(bstr_HoleTopFaceTag);
		}
		//倒圆角面
		//BSTR bstr_BossTopFaceNode;
		//NXString iStringBossTopFaceNode = "倒圆角面";//g

		//bstr_BossTopFaceNode = ConvertToBSTR(iStringBossTopFaceNode);

		//IXMLDOMElementPtr pElement_BossTopFaceNode;
		//pDoc->createElement(bstr_BossTopFaceNode, &pElement_BossTopFaceNode);
		//AppendChildToParent(pElement_BossTopFaceNode, pElement_HoleFeatureNode);//g

		//for (std::vector<Face*>::size_type j = 0; j != EachHole->CircleVector.size(); j++)//g
		//{
		//	Face* EachTopFace = EachHole->CircleVector[j];//g

		//	int indexJ = j + 1;
		//	char* StringIndexJ = new char;
		//	sprintf(StringIndexJ, "%d", indexJ);
		//	NXString NXStringIndexJ = StringIndexJ;

		//	BSTR bstr_BossTopFace;
		//	NXString iStringBossTopFace = "倒圆角面." + NXStringIndexJ;//g

		//	bstr_BossTopFace = ConvertToBSTR(iStringBossTopFace);

		//	IXMLDOMElementPtr pElement_BossTopFace;
		//	pDoc->createElement(bstr_BossTopFace, &pElement_BossTopFace);
		//	AppendChildToParent(pElement_BossTopFace, pElement_BossTopFaceNode);

		//	char* StringIndexTopFace = new char;
		//	sprintf(StringIndexTopFace, "%d", EachTopFace->Tag());

		//	BSTR bstr_BossTopFaceTag;
		//	NXString iStringBossTopFaceTag = StringIndexTopFace;

		//	bstr_BossTopFaceTag = ConvertToBSTR(iStringBossTopFaceTag);
		//	pElement_BossTopFace->put_text(bstr_BossTopFaceTag);
		//}

		/*过渡平面*/
		//BSTR bstr_HoleSideFaceNode;
		//NXString iStringHoleSideFaceNode = "过渡平面";//g

		//bstr_HoleSideFaceNode = ConvertToBSTR(iStringHoleSideFaceNode);

		//IXMLDOMElementPtr pElement_HoleSideFaceNode;
		//pDoc->createElement(bstr_HoleSideFaceNode, &pElement_HoleSideFaceNode);
		//AppendChildToParent(pElement_HoleSideFaceNode, pElement_HoleFeatureNode);

		//for (std::vector<Face*>::size_type j = 0; j != EachHole->PlanerVector.size(); j++)//g
		//{
		//	Face* EachSideFace = EachHole->PlanerVector[j];//g

		//	int indexJ = j + 1;
		//	char* StringIndexJ = new char;
		//	sprintf(StringIndexJ, "%d", indexJ);
		//	NXString NXStringIndexJ = StringIndexJ;

		//	BSTR bstr_HoleSideFace;
		//	NXString iStringHoleSideFace = "过渡平面." + NXStringIndexJ;//g

		//	bstr_HoleSideFace = ConvertToBSTR(iStringHoleSideFace);

		//	IXMLDOMElementPtr pElement_HoleSideFace;
		//	pDoc->createElement(bstr_HoleSideFace, &pElement_HoleSideFace);
		//	AppendChildToParent(pElement_HoleSideFace, pElement_HoleSideFaceNode);//jie for shang

		//	char* StringIndexSideFace = new char;
		//	sprintf(StringIndexSideFace, "%d", EachSideFace->Tag());

		//	BSTR bstr_HoleSideFaceTag;
		//	NXString iStringHoleSideFaceTag = StringIndexSideFace;

		//	bstr_HoleSideFaceTag = ConvertToBSTR(iStringHoleSideFaceTag);
		//	pElement_HoleSideFace->put_text(bstr_HoleSideFaceTag);
		//}

		//侧面
		BSTR bstr_HoleCornerFaceNode;
		NXString iStringHoleCornerFaceNode = "侧面";

		bstr_HoleCornerFaceNode = ConvertToBSTR(iStringHoleCornerFaceNode);

		IXMLDOMElementPtr pElement_HoleCornerFaceNode;
		pDoc->createElement(bstr_HoleCornerFaceNode, &pElement_HoleCornerFaceNode);
		AppendChildToParent(pElement_HoleCornerFaceNode, pElement_HoleFeatureNode);

		for (std::vector<Face*>::size_type j = 0; j != EachHole->InWallVector.size(); j++)
		{
			Face* EachCornerFace = EachHole->InWallVector[j];

			int indexJ = j + 1;
			char* StringIndexJ = new char;
			sprintf(StringIndexJ, "%d", indexJ);
			NXString NXStringIndexJ = StringIndexJ;

			BSTR bstr_HoleCornerFace;
			NXString iStringHoleCornerFace = "侧面." + NXStringIndexJ;

			bstr_HoleCornerFace = ConvertToBSTR(iStringHoleCornerFace);

			IXMLDOMElementPtr pElement_HoleCornerFace;
			pDoc->createElement(bstr_HoleCornerFace, &pElement_HoleCornerFace);
			AppendChildToParent(pElement_HoleCornerFace, pElement_HoleCornerFaceNode);

			char* StringIndexCornerFace = new char;
			sprintf(StringIndexCornerFace, "%d", EachCornerFace->Tag());

			BSTR bstr_HoleCornerFaceTag;
			NXString iStringHoleCornerFaceTag = StringIndexCornerFace;

			bstr_HoleCornerFaceTag = ConvertToBSTR(iStringHoleCornerFaceTag);
			pElement_HoleCornerFace->put_text(bstr_HoleCornerFaceTag);
		}

		//倒斜角面
		BSTR bstr_BossSideFaceNode;
		NXString iStringBossSideFaceNode = "倒斜角面";

		bstr_BossSideFaceNode = ConvertToBSTR(iStringBossSideFaceNode);

		IXMLDOMElementPtr pElement_BossSideFaceNode;
		pDoc->createElement(bstr_BossSideFaceNode, &pElement_BossSideFaceNode);
		AppendChildToParent(pElement_BossSideFaceNode, pElement_HoleFeatureNode);

		for (std::vector<Face*>::size_type j = 0; j != EachHole->RightVector.size(); j++)
		{
			Face* EachSideFace = EachHole->RightVector[j];

			int indexJ = j + 1;
			char* StringIndexJ = new char;
			sprintf(StringIndexJ, "%d", indexJ);
			NXString NXStringIndexJ = StringIndexJ;

			BSTR bstr_BossSideFace;
			NXString iStringBossSideFace = "倒斜角面." + NXStringIndexJ;

			bstr_BossSideFace = ConvertToBSTR(iStringBossSideFace);

			IXMLDOMElementPtr pElement_BossSideFace;
			pDoc->createElement(bstr_BossSideFace, &pElement_BossSideFace);
			AppendChildToParent(pElement_BossSideFace, pElement_BossSideFaceNode);

			char* StringIndexSideFace = new char;
			sprintf(StringIndexSideFace, "%d", EachSideFace->Tag());

			BSTR bstr_BossSideFaceTag;
			NXString iStringBossSideFaceTag = StringIndexSideFace;

			bstr_BossSideFaceTag = ConvertToBSTR(iStringBossSideFaceTag);
			pElement_BossSideFace->put_text(bstr_BossSideFaceTag);
		}


		//孔底面
		BSTR bstr_HoleBottomFaceNode;
		NXString iStringHoleBottomFaceNode = "底面";

		bstr_HoleBottomFaceNode = ConvertToBSTR(iStringHoleBottomFaceNode);

		IXMLDOMElementPtr pElement_HoleBottomFaceNode;
		pDoc->createElement(bstr_HoleBottomFaceNode, &pElement_HoleBottomFaceNode);
		AppendChildToParent(pElement_HoleBottomFaceNode, pElement_HoleFeatureNode);

		for (std::vector<Face*>::size_type j = 0; j != EachHole->BottomFaceVector.size(); j++)
		{
			Face* EachBottomFace = EachHole->BottomFaceVector[j];

			int indexJ = j + 1;
			char* StringIndexJ = new char;
			sprintf(StringIndexJ, "%d", indexJ);
			NXString NXStringIndexJ = StringIndexJ;

			BSTR bstr_HoleBottomFace;
			NXString iStringHoleBottomFace = "底面." + NXStringIndexJ;

			bstr_HoleBottomFace = ConvertToBSTR(iStringHoleBottomFace);

			IXMLDOMElementPtr pElement_HoleBottomFace;
			pDoc->createElement(bstr_HoleBottomFace, &pElement_HoleBottomFace);
			AppendChildToParent(pElement_HoleBottomFace, pElement_HoleBottomFaceNode);

			char* StringIndexBottomFace = new char;
			sprintf(StringIndexBottomFace, "%d", EachBottomFace->Tag());

			BSTR bstr_HoleBottomFaceTag;
			NXString iStringHoleBottomFaceTag = StringIndexBottomFace;

			bstr_HoleBottomFaceTag = ConvertToBSTR(iStringHoleBottomFaceTag);
			pElement_HoleBottomFace->put_text(bstr_HoleBottomFaceTag);
		}
	}

	//凸台单侧圆角通孔
	for (std::vector<BossOneCircleHole*>::size_type i = 0; i != BossOneCircleHoleSpyResult.size(); i++) //G
	{
		BossOneCircleHole* EachHole = BossOneCircleHoleSpyResult[i];//G

		int indexI = i + 1;
		char* StringIndexI = new char;
		sprintf(StringIndexI, "%d", indexI);
		NXString NXStringIndexI = StringIndexI;

		BSTR bstr_HoleFeatureNode;
		NXString iStringHoleFeatureNode = "凸台单侧圆角通孔." + NXStringIndexI;//g

		bstr_HoleFeatureNode = ConvertToBSTR(iStringHoleFeatureNode);

		IXMLDOMElementPtr pElement_HoleFeatureNode;
		pDoc->createElement(bstr_HoleFeatureNode, &pElement_HoleFeatureNode);
		AppendChildToParent(pElement_HoleFeatureNode, pElement_Hole);

		//孔顶面
		BSTR bstr_HoleTopFaceNode;
		NXString iStringHoleTopFaceNode = "顶面";

		bstr_HoleTopFaceNode = ConvertToBSTR(iStringHoleTopFaceNode);

		IXMLDOMElementPtr pElement_HoleTopFaceNode;
		pDoc->createElement(bstr_HoleTopFaceNode, &pElement_HoleTopFaceNode);
		AppendChildToParent(pElement_HoleTopFaceNode, pElement_HoleFeatureNode);

		for (std::vector<Face*>::size_type j = 0; j != EachHole->TopFaceVector.size(); j++)
		{
			Face* EachTopFace = EachHole->TopFaceVector[j];

			int indexJ = j + 1;
			char* StringIndexJ = new char;
			sprintf(StringIndexJ, "%d", indexJ);
			NXString NXStringIndexJ = StringIndexJ;

			BSTR bstr_HoleTopFace;
			NXString iStringHoleTopFace = "顶面." + NXStringIndexJ;

			bstr_HoleTopFace = ConvertToBSTR(iStringHoleTopFace);

			IXMLDOMElementPtr pElement_HoleTopFace;
			pDoc->createElement(bstr_HoleTopFace, &pElement_HoleTopFace);
			AppendChildToParent(pElement_HoleTopFace, pElement_HoleTopFaceNode);

			char* StringIndexTopFace = new char;
			sprintf(StringIndexTopFace, "%d", EachTopFace->Tag());

			BSTR bstr_HoleTopFaceTag;
			NXString iStringHoleTopFaceTag = StringIndexTopFace;

			bstr_HoleTopFaceTag = ConvertToBSTR(iStringHoleTopFaceTag);
			pElement_HoleTopFace->put_text(bstr_HoleTopFaceTag);
		}
		//倒圆角面
		BSTR bstr_BossTopFaceNode;
		NXString iStringBossTopFaceNode = "倒圆角面";//g

		bstr_BossTopFaceNode = ConvertToBSTR(iStringBossTopFaceNode);

		IXMLDOMElementPtr pElement_BossTopFaceNode;
		pDoc->createElement(bstr_BossTopFaceNode, &pElement_BossTopFaceNode);
		AppendChildToParent(pElement_BossTopFaceNode, pElement_HoleFeatureNode);//g

		for (std::vector<Face*>::size_type j = 0; j != EachHole->CircleVector.size(); j++)//g
		{
			Face* EachTopFace = EachHole->CircleVector[j];//g

			int indexJ = j + 1;
			char* StringIndexJ = new char;
			sprintf(StringIndexJ, "%d", indexJ);
			NXString NXStringIndexJ = StringIndexJ;

			BSTR bstr_BossTopFace;
			NXString iStringBossTopFace = "倒圆角面." + NXStringIndexJ;//g

			bstr_BossTopFace = ConvertToBSTR(iStringBossTopFace);

			IXMLDOMElementPtr pElement_BossTopFace;
			pDoc->createElement(bstr_BossTopFace, &pElement_BossTopFace);
			AppendChildToParent(pElement_BossTopFace, pElement_BossTopFaceNode);

			char* StringIndexTopFace = new char;
			sprintf(StringIndexTopFace, "%d", EachTopFace->Tag());

			BSTR bstr_BossTopFaceTag;
			NXString iStringBossTopFaceTag = StringIndexTopFace;

			bstr_BossTopFaceTag = ConvertToBSTR(iStringBossTopFaceTag);
			pElement_BossTopFace->put_text(bstr_BossTopFaceTag);
		}

		/*过渡平面*/
		//BSTR bstr_HoleSideFaceNode;
		//NXString iStringHoleSideFaceNode = "过渡平面";//g

		//bstr_HoleSideFaceNode = ConvertToBSTR(iStringHoleSideFaceNode);

		//IXMLDOMElementPtr pElement_HoleSideFaceNode;
		//pDoc->createElement(bstr_HoleSideFaceNode, &pElement_HoleSideFaceNode);
		//AppendChildToParent(pElement_HoleSideFaceNode, pElement_HoleFeatureNode);

		//for (std::vector<Face*>::size_type j = 0; j != EachHole->PlanerVector.size(); j++)//g
		//{
		//	Face* EachSideFace = EachHole->PlanerVector[j];//g

		//	int indexJ = j + 1;
		//	char* StringIndexJ = new char;
		//	sprintf(StringIndexJ, "%d", indexJ);
		//	NXString NXStringIndexJ = StringIndexJ;

		//	BSTR bstr_HoleSideFace;
		//	NXString iStringHoleSideFace = "过渡平面." + NXStringIndexJ;//g

		//	bstr_HoleSideFace = ConvertToBSTR(iStringHoleSideFace);

		//	IXMLDOMElementPtr pElement_HoleSideFace;
		//	pDoc->createElement(bstr_HoleSideFace, &pElement_HoleSideFace);
		//	AppendChildToParent(pElement_HoleSideFace, pElement_HoleSideFaceNode);//jie for shang

		//	char* StringIndexSideFace = new char;
		//	sprintf(StringIndexSideFace, "%d", EachSideFace->Tag());

		//	BSTR bstr_HoleSideFaceTag;
		//	NXString iStringHoleSideFaceTag = StringIndexSideFace;

		//	bstr_HoleSideFaceTag = ConvertToBSTR(iStringHoleSideFaceTag);
		//	pElement_HoleSideFace->put_text(bstr_HoleSideFaceTag);
		//}

		//侧面
		BSTR bstr_HoleCornerFaceNode;
		NXString iStringHoleCornerFaceNode = "侧面";

		bstr_HoleCornerFaceNode = ConvertToBSTR(iStringHoleCornerFaceNode);

		IXMLDOMElementPtr pElement_HoleCornerFaceNode;
		pDoc->createElement(bstr_HoleCornerFaceNode, &pElement_HoleCornerFaceNode);
		AppendChildToParent(pElement_HoleCornerFaceNode, pElement_HoleFeatureNode);

		for (std::vector<Face*>::size_type j = 0; j != EachHole->InWallVector.size(); j++)
		{
			Face* EachCornerFace = EachHole->InWallVector[j];

			int indexJ = j + 1;
			char* StringIndexJ = new char;
			sprintf(StringIndexJ, "%d", indexJ);
			NXString NXStringIndexJ = StringIndexJ;

			BSTR bstr_HoleCornerFace;
			NXString iStringHoleCornerFace = "侧面." + NXStringIndexJ;

			bstr_HoleCornerFace = ConvertToBSTR(iStringHoleCornerFace);

			IXMLDOMElementPtr pElement_HoleCornerFace;
			pDoc->createElement(bstr_HoleCornerFace, &pElement_HoleCornerFace);
			AppendChildToParent(pElement_HoleCornerFace, pElement_HoleCornerFaceNode);

			char* StringIndexCornerFace = new char;
			sprintf(StringIndexCornerFace, "%d", EachCornerFace->Tag());

			BSTR bstr_HoleCornerFaceTag;
			NXString iStringHoleCornerFaceTag = StringIndexCornerFace;

			bstr_HoleCornerFaceTag = ConvertToBSTR(iStringHoleCornerFaceTag);
			pElement_HoleCornerFace->put_text(bstr_HoleCornerFaceTag);
		}

		//倒斜角面
		/*BSTR bstr_BossSideFaceNode;
		NXString iStringBossSideFaceNode = "倒斜角面";

		bstr_BossSideFaceNode = ConvertToBSTR(iStringBossSideFaceNode);

		IXMLDOMElementPtr pElement_BossSideFaceNode;
		pDoc->createElement(bstr_BossSideFaceNode, &pElement_BossSideFaceNode);
		AppendChildToParent(pElement_BossSideFaceNode, pElement_HoleFeatureNode);

		for (std::vector<Face*>::size_type j = 0; j != EachHole->RightVector.size(); j++)
		{
			Face* EachSideFace = EachHole->RightVector[j];

			int indexJ = j + 1;
			char* StringIndexJ = new char;
			sprintf(StringIndexJ, "%d", indexJ);
			NXString NXStringIndexJ = StringIndexJ;

			BSTR bstr_BossSideFace;
			NXString iStringBossSideFace = "倒斜角面." + NXStringIndexJ;

			bstr_BossSideFace = ConvertToBSTR(iStringBossSideFace);

			IXMLDOMElementPtr pElement_BossSideFace;
			pDoc->createElement(bstr_BossSideFace, &pElement_BossSideFace);
			AppendChildToParent(pElement_BossSideFace, pElement_BossSideFaceNode);

			char* StringIndexSideFace = new char;
			sprintf(StringIndexSideFace, "%d", EachSideFace->Tag());

			BSTR bstr_BossSideFaceTag;
			NXString iStringBossSideFaceTag = StringIndexSideFace;

			bstr_BossSideFaceTag = ConvertToBSTR(iStringBossSideFaceTag);
			pElement_BossSideFace->put_text(bstr_BossSideFaceTag);
		}*/


		//孔底面
		BSTR bstr_HoleBottomFaceNode;
		NXString iStringHoleBottomFaceNode = "底面";

		bstr_HoleBottomFaceNode = ConvertToBSTR(iStringHoleBottomFaceNode);

		IXMLDOMElementPtr pElement_HoleBottomFaceNode;
		pDoc->createElement(bstr_HoleBottomFaceNode, &pElement_HoleBottomFaceNode);
		AppendChildToParent(pElement_HoleBottomFaceNode, pElement_HoleFeatureNode);

		for (std::vector<Face*>::size_type j = 0; j != EachHole->BottomFaceVector.size(); j++)
		{
			Face* EachBottomFace = EachHole->BottomFaceVector[j];

			int indexJ = j + 1;
			char* StringIndexJ = new char;
			sprintf(StringIndexJ, "%d", indexJ);
			NXString NXStringIndexJ = StringIndexJ;

			BSTR bstr_HoleBottomFace;
			NXString iStringHoleBottomFace = "底面." + NXStringIndexJ;

			bstr_HoleBottomFace = ConvertToBSTR(iStringHoleBottomFace);

			IXMLDOMElementPtr pElement_HoleBottomFace;
			pDoc->createElement(bstr_HoleBottomFace, &pElement_HoleBottomFace);
			AppendChildToParent(pElement_HoleBottomFace, pElement_HoleBottomFaceNode);

			char* StringIndexBottomFace = new char;
			sprintf(StringIndexBottomFace, "%d", EachBottomFace->Tag());

			BSTR bstr_HoleBottomFaceTag;
			NXString iStringHoleBottomFaceTag = StringIndexBottomFace;

			bstr_HoleBottomFaceTag = ConvertToBSTR(iStringHoleBottomFaceTag);
			pElement_HoleBottomFace->put_text(bstr_HoleBottomFaceTag);
		}
	}

	//凸台双侧斜角通孔
	for (std::vector<BossDoubleRightHole*>::size_type i = 0; i != BossDoubleRightHoleSpyResult.size(); i++) //G
	{
		BossDoubleRightHole* EachHole = BossDoubleRightHoleSpyResult[i];//G

		int indexI = i + 1;
		char* StringIndexI = new char;
		sprintf(StringIndexI, "%d", indexI);
		NXString NXStringIndexI = StringIndexI;

		BSTR bstr_HoleFeatureNode;
		NXString iStringHoleFeatureNode = "凸台双侧斜角通孔." + NXStringIndexI;//g

		bstr_HoleFeatureNode = ConvertToBSTR(iStringHoleFeatureNode);

		IXMLDOMElementPtr pElement_HoleFeatureNode;
		pDoc->createElement(bstr_HoleFeatureNode, &pElement_HoleFeatureNode);
		AppendChildToParent(pElement_HoleFeatureNode, pElement_Hole);

		//孔顶面
		BSTR bstr_HoleTopFaceNode;
		NXString iStringHoleTopFaceNode = "顶面";

		bstr_HoleTopFaceNode = ConvertToBSTR(iStringHoleTopFaceNode);

		IXMLDOMElementPtr pElement_HoleTopFaceNode;
		pDoc->createElement(bstr_HoleTopFaceNode, &pElement_HoleTopFaceNode);
		AppendChildToParent(pElement_HoleTopFaceNode, pElement_HoleFeatureNode);

		for (std::vector<Face*>::size_type j = 0; j != EachHole->TopFaceVector.size(); j++)
		{
			Face* EachTopFace = EachHole->TopFaceVector[j];

			int indexJ = j + 1;
			char* StringIndexJ = new char;
			sprintf(StringIndexJ, "%d", indexJ);
			NXString NXStringIndexJ = StringIndexJ;

			BSTR bstr_HoleTopFace;
			NXString iStringHoleTopFace = "顶面." + NXStringIndexJ;

			bstr_HoleTopFace = ConvertToBSTR(iStringHoleTopFace);

			IXMLDOMElementPtr pElement_HoleTopFace;
			pDoc->createElement(bstr_HoleTopFace, &pElement_HoleTopFace);
			AppendChildToParent(pElement_HoleTopFace, pElement_HoleTopFaceNode);

			char* StringIndexTopFace = new char;
			sprintf(StringIndexTopFace, "%d", EachTopFace->Tag());

			BSTR bstr_HoleTopFaceTag;
			NXString iStringHoleTopFaceTag = StringIndexTopFace;

			bstr_HoleTopFaceTag = ConvertToBSTR(iStringHoleTopFaceTag);
			pElement_HoleTopFace->put_text(bstr_HoleTopFaceTag);
		}
		//上侧倒斜角面
		BSTR bstr_BossTopFaceNode;
		NXString iStringBossTopFaceNode = "上侧倒斜角面";//g

		bstr_BossTopFaceNode = ConvertToBSTR(iStringBossTopFaceNode);

		IXMLDOMElementPtr pElement_BossTopFaceNode;
		pDoc->createElement(bstr_BossTopFaceNode, &pElement_BossTopFaceNode);
		AppendChildToParent(pElement_BossTopFaceNode, pElement_HoleFeatureNode);//g

		for (std::vector<Face*>::size_type j = 0; j != EachHole->TopRightVector.size(); j++)//g
		{
			Face* EachTopFace = EachHole->TopRightVector[j];//g

			int indexJ = j + 1;
			char* StringIndexJ = new char;
			sprintf(StringIndexJ, "%d", indexJ);
			NXString NXStringIndexJ = StringIndexJ;

			BSTR bstr_BossTopFace;
			NXString iStringBossTopFace = "上侧倒斜角面." + NXStringIndexJ;//g

			bstr_BossTopFace = ConvertToBSTR(iStringBossTopFace);

			IXMLDOMElementPtr pElement_BossTopFace;
			pDoc->createElement(bstr_BossTopFace, &pElement_BossTopFace);
			AppendChildToParent(pElement_BossTopFace, pElement_BossTopFaceNode);

			char* StringIndexTopFace = new char;
			sprintf(StringIndexTopFace, "%d", EachTopFace->Tag());

			BSTR bstr_BossTopFaceTag;
			NXString iStringBossTopFaceTag = StringIndexTopFace;

			bstr_BossTopFaceTag = ConvertToBSTR(iStringBossTopFaceTag);
			pElement_BossTopFace->put_text(bstr_BossTopFaceTag);
		}

		/*下侧倒斜角面*/
		BSTR bstr_HoleSideFaceNode;
		NXString iStringHoleSideFaceNode = "下侧倒斜角面";//g

		bstr_HoleSideFaceNode = ConvertToBSTR(iStringHoleSideFaceNode);

		IXMLDOMElementPtr pElement_HoleSideFaceNode;
		pDoc->createElement(bstr_HoleSideFaceNode, &pElement_HoleSideFaceNode);
		AppendChildToParent(pElement_HoleSideFaceNode, pElement_HoleFeatureNode);

		for (std::vector<Face*>::size_type j = 0; j != EachHole->BottomRightVector.size(); j++)//g
		{
			Face* EachSideFace = EachHole->BottomRightVector[j];//g

			int indexJ = j + 1;
			char* StringIndexJ = new char;
			sprintf(StringIndexJ, "%d", indexJ);
			NXString NXStringIndexJ = StringIndexJ;

			BSTR bstr_HoleSideFace;
			NXString iStringHoleSideFace = "下侧倒斜角面." + NXStringIndexJ;//g

			bstr_HoleSideFace = ConvertToBSTR(iStringHoleSideFace);

			IXMLDOMElementPtr pElement_HoleSideFace;
			pDoc->createElement(bstr_HoleSideFace, &pElement_HoleSideFace);
			AppendChildToParent(pElement_HoleSideFace, pElement_HoleSideFaceNode);//jie for shang

			char* StringIndexSideFace = new char;
			sprintf(StringIndexSideFace, "%d", EachSideFace->Tag());

			BSTR bstr_HoleSideFaceTag;
			NXString iStringHoleSideFaceTag = StringIndexSideFace;

			bstr_HoleSideFaceTag = ConvertToBSTR(iStringHoleSideFaceTag);
			pElement_HoleSideFace->put_text(bstr_HoleSideFaceTag);
		}

		//侧面
		BSTR bstr_HoleCornerFaceNode;
		NXString iStringHoleCornerFaceNode = "侧面";

		bstr_HoleCornerFaceNode = ConvertToBSTR(iStringHoleCornerFaceNode);

		IXMLDOMElementPtr pElement_HoleCornerFaceNode;
		pDoc->createElement(bstr_HoleCornerFaceNode, &pElement_HoleCornerFaceNode);
		AppendChildToParent(pElement_HoleCornerFaceNode, pElement_HoleFeatureNode);

		for (std::vector<Face*>::size_type j = 0; j != EachHole->InWallVector.size(); j++)
		{
			Face* EachCornerFace = EachHole->InWallVector[j];

			int indexJ = j + 1;
			char* StringIndexJ = new char;
			sprintf(StringIndexJ, "%d", indexJ);
			NXString NXStringIndexJ = StringIndexJ;

			BSTR bstr_HoleCornerFace;
			NXString iStringHoleCornerFace = "侧面." + NXStringIndexJ;

			bstr_HoleCornerFace = ConvertToBSTR(iStringHoleCornerFace);

			IXMLDOMElementPtr pElement_HoleCornerFace;
			pDoc->createElement(bstr_HoleCornerFace, &pElement_HoleCornerFace);
			AppendChildToParent(pElement_HoleCornerFace, pElement_HoleCornerFaceNode);

			char* StringIndexCornerFace = new char;
			sprintf(StringIndexCornerFace, "%d", EachCornerFace->Tag());

			BSTR bstr_HoleCornerFaceTag;
			NXString iStringHoleCornerFaceTag = StringIndexCornerFace;

			bstr_HoleCornerFaceTag = ConvertToBSTR(iStringHoleCornerFaceTag);
			pElement_HoleCornerFace->put_text(bstr_HoleCornerFaceTag);
		}

		//倒斜角面
		/*BSTR bstr_BossSideFaceNode;
		NXString iStringBossSideFaceNode = "倒斜角面";

		bstr_BossSideFaceNode = ConvertToBSTR(iStringBossSideFaceNode);

		IXMLDOMElementPtr pElement_BossSideFaceNode;
		pDoc->createElement(bstr_BossSideFaceNode, &pElement_BossSideFaceNode);
		AppendChildToParent(pElement_BossSideFaceNode, pElement_HoleFeatureNode);

		for (std::vector<Face*>::size_type j = 0; j != EachHole->RightVector.size(); j++)
		{
			Face* EachSideFace = EachHole->RightVector[j];

			int indexJ = j + 1;
			char* StringIndexJ = new char;
			sprintf(StringIndexJ, "%d", indexJ);
			NXString NXStringIndexJ = StringIndexJ;

			BSTR bstr_BossSideFace;
			NXString iStringBossSideFace = "倒斜角面." + NXStringIndexJ;

			bstr_BossSideFace = ConvertToBSTR(iStringBossSideFace);

			IXMLDOMElementPtr pElement_BossSideFace;
			pDoc->createElement(bstr_BossSideFace, &pElement_BossSideFace);
			AppendChildToParent(pElement_BossSideFace, pElement_BossSideFaceNode);

			char* StringIndexSideFace = new char;
			sprintf(StringIndexSideFace, "%d", EachSideFace->Tag());

			BSTR bstr_BossSideFaceTag;
			NXString iStringBossSideFaceTag = StringIndexSideFace;

			bstr_BossSideFaceTag = ConvertToBSTR(iStringBossSideFaceTag);
			pElement_BossSideFace->put_text(bstr_BossSideFaceTag);
		}*/


		//孔底面
		BSTR bstr_HoleBottomFaceNode;
		NXString iStringHoleBottomFaceNode = "底面";

		bstr_HoleBottomFaceNode = ConvertToBSTR(iStringHoleBottomFaceNode);

		IXMLDOMElementPtr pElement_HoleBottomFaceNode;
		pDoc->createElement(bstr_HoleBottomFaceNode, &pElement_HoleBottomFaceNode);
		AppendChildToParent(pElement_HoleBottomFaceNode, pElement_HoleFeatureNode);

		for (std::vector<Face*>::size_type j = 0; j != EachHole->BottomFaceVector.size(); j++)
		{
			Face* EachBottomFace = EachHole->BottomFaceVector[j];

			int indexJ = j + 1;
			char* StringIndexJ = new char;
			sprintf(StringIndexJ, "%d", indexJ);
			NXString NXStringIndexJ = StringIndexJ;

			BSTR bstr_HoleBottomFace;
			NXString iStringHoleBottomFace = "底面." + NXStringIndexJ;

			bstr_HoleBottomFace = ConvertToBSTR(iStringHoleBottomFace);

			IXMLDOMElementPtr pElement_HoleBottomFace;
			pDoc->createElement(bstr_HoleBottomFace, &pElement_HoleBottomFace);
			AppendChildToParent(pElement_HoleBottomFace, pElement_HoleBottomFaceNode);

			char* StringIndexBottomFace = new char;
			sprintf(StringIndexBottomFace, "%d", EachBottomFace->Tag());

			BSTR bstr_HoleBottomFaceTag;
			NXString iStringHoleBottomFaceTag = StringIndexBottomFace;

			bstr_HoleBottomFaceTag = ConvertToBSTR(iStringHoleBottomFaceTag);
			pElement_HoleBottomFace->put_text(bstr_HoleBottomFaceTag);
		}
	}

	//凸台双侧圆角通孔
	for (std::vector<BossDoubleCircleHole*>::size_type i = 0; i != BossDoubleCircleHoleSpyResult.size(); i++) //G
	{
		BossDoubleCircleHole* EachHole = BossDoubleCircleHoleSpyResult[i];//G

		int indexI = i + 1;
		char* StringIndexI = new char;
		sprintf(StringIndexI, "%d", indexI);
		NXString NXStringIndexI = StringIndexI;

		BSTR bstr_HoleFeatureNode;
		NXString iStringHoleFeatureNode = "凸台双侧圆角通孔." + NXStringIndexI;//g

		bstr_HoleFeatureNode = ConvertToBSTR(iStringHoleFeatureNode);

		IXMLDOMElementPtr pElement_HoleFeatureNode;
		pDoc->createElement(bstr_HoleFeatureNode, &pElement_HoleFeatureNode);
		AppendChildToParent(pElement_HoleFeatureNode, pElement_Hole);

		//孔顶面
		BSTR bstr_HoleTopFaceNode;
		NXString iStringHoleTopFaceNode = "顶面";

		bstr_HoleTopFaceNode = ConvertToBSTR(iStringHoleTopFaceNode);

		IXMLDOMElementPtr pElement_HoleTopFaceNode;
		pDoc->createElement(bstr_HoleTopFaceNode, &pElement_HoleTopFaceNode);
		AppendChildToParent(pElement_HoleTopFaceNode, pElement_HoleFeatureNode);

		for (std::vector<Face*>::size_type j = 0; j != EachHole->TopFaceVector.size(); j++)
		{
			Face* EachTopFace = EachHole->TopFaceVector[j];

			int indexJ = j + 1;
			char* StringIndexJ = new char;
			sprintf(StringIndexJ, "%d", indexJ);
			NXString NXStringIndexJ = StringIndexJ;

			BSTR bstr_HoleTopFace;
			NXString iStringHoleTopFace = "顶面." + NXStringIndexJ;

			bstr_HoleTopFace = ConvertToBSTR(iStringHoleTopFace);

			IXMLDOMElementPtr pElement_HoleTopFace;
			pDoc->createElement(bstr_HoleTopFace, &pElement_HoleTopFace);
			AppendChildToParent(pElement_HoleTopFace, pElement_HoleTopFaceNode);

			char* StringIndexTopFace = new char;
			sprintf(StringIndexTopFace, "%d", EachTopFace->Tag());

			BSTR bstr_HoleTopFaceTag;
			NXString iStringHoleTopFaceTag = StringIndexTopFace;

			bstr_HoleTopFaceTag = ConvertToBSTR(iStringHoleTopFaceTag);
			pElement_HoleTopFace->put_text(bstr_HoleTopFaceTag);
		}
		//上侧倒圆角面
		BSTR bstr_BossTopFaceNode;
		NXString iStringBossTopFaceNode = "上侧倒圆角面";//g

		bstr_BossTopFaceNode = ConvertToBSTR(iStringBossTopFaceNode);

		IXMLDOMElementPtr pElement_BossTopFaceNode;
		pDoc->createElement(bstr_BossTopFaceNode, &pElement_BossTopFaceNode);
		AppendChildToParent(pElement_BossTopFaceNode, pElement_HoleFeatureNode);//g

		for (std::vector<Face*>::size_type j = 0; j != EachHole->TopCircleVector.size(); j++)//g
		{
			Face* EachTopFace = EachHole->TopCircleVector[j];//g

			int indexJ = j + 1;
			char* StringIndexJ = new char;
			sprintf(StringIndexJ, "%d", indexJ);
			NXString NXStringIndexJ = StringIndexJ;

			BSTR bstr_BossTopFace;
			NXString iStringBossTopFace = "上侧倒圆角面." + NXStringIndexJ;//g

			bstr_BossTopFace = ConvertToBSTR(iStringBossTopFace);

			IXMLDOMElementPtr pElement_BossTopFace;
			pDoc->createElement(bstr_BossTopFace, &pElement_BossTopFace);
			AppendChildToParent(pElement_BossTopFace, pElement_BossTopFaceNode);

			char* StringIndexTopFace = new char;
			sprintf(StringIndexTopFace, "%d", EachTopFace->Tag());

			BSTR bstr_BossTopFaceTag;
			NXString iStringBossTopFaceTag = StringIndexTopFace;

			bstr_BossTopFaceTag = ConvertToBSTR(iStringBossTopFaceTag);
			pElement_BossTopFace->put_text(bstr_BossTopFaceTag);
		}

		/*下侧倒圆角面*/
		BSTR bstr_HoleSideFaceNode;
		NXString iStringHoleSideFaceNode = "下侧倒圆角面";//g

		bstr_HoleSideFaceNode = ConvertToBSTR(iStringHoleSideFaceNode);

		IXMLDOMElementPtr pElement_HoleSideFaceNode;
		pDoc->createElement(bstr_HoleSideFaceNode, &pElement_HoleSideFaceNode);
		AppendChildToParent(pElement_HoleSideFaceNode, pElement_HoleFeatureNode);

		for (std::vector<Face*>::size_type j = 0; j != EachHole->BottomCircleVector.size(); j++)//g
		{
			Face* EachSideFace = EachHole->BottomCircleVector[j];//g

			int indexJ = j + 1;
			char* StringIndexJ = new char;
			sprintf(StringIndexJ, "%d", indexJ);
			NXString NXStringIndexJ = StringIndexJ;

			BSTR bstr_HoleSideFace;
			NXString iStringHoleSideFace = "下侧倒圆角面." + NXStringIndexJ;//g

			bstr_HoleSideFace = ConvertToBSTR(iStringHoleSideFace);

			IXMLDOMElementPtr pElement_HoleSideFace;
			pDoc->createElement(bstr_HoleSideFace, &pElement_HoleSideFace);
			AppendChildToParent(pElement_HoleSideFace, pElement_HoleSideFaceNode);//jie for shang

			char* StringIndexSideFace = new char;
			sprintf(StringIndexSideFace, "%d", EachSideFace->Tag());

			BSTR bstr_HoleSideFaceTag;
			NXString iStringHoleSideFaceTag = StringIndexSideFace;

			bstr_HoleSideFaceTag = ConvertToBSTR(iStringHoleSideFaceTag);
			pElement_HoleSideFace->put_text(bstr_HoleSideFaceTag);
		}

		//侧面
		BSTR bstr_HoleCornerFaceNode;
		NXString iStringHoleCornerFaceNode = "侧面";

		bstr_HoleCornerFaceNode = ConvertToBSTR(iStringHoleCornerFaceNode);

		IXMLDOMElementPtr pElement_HoleCornerFaceNode;
		pDoc->createElement(bstr_HoleCornerFaceNode, &pElement_HoleCornerFaceNode);
		AppendChildToParent(pElement_HoleCornerFaceNode, pElement_HoleFeatureNode);

		for (std::vector<Face*>::size_type j = 0; j != EachHole->InWallVector.size(); j++)
		{
			Face* EachCornerFace = EachHole->InWallVector[j];

			int indexJ = j + 1;
			char* StringIndexJ = new char;
			sprintf(StringIndexJ, "%d", indexJ);
			NXString NXStringIndexJ = StringIndexJ;

			BSTR bstr_HoleCornerFace;
			NXString iStringHoleCornerFace = "侧面." + NXStringIndexJ;

			bstr_HoleCornerFace = ConvertToBSTR(iStringHoleCornerFace);

			IXMLDOMElementPtr pElement_HoleCornerFace;
			pDoc->createElement(bstr_HoleCornerFace, &pElement_HoleCornerFace);
			AppendChildToParent(pElement_HoleCornerFace, pElement_HoleCornerFaceNode);

			char* StringIndexCornerFace = new char;
			sprintf(StringIndexCornerFace, "%d", EachCornerFace->Tag());

			BSTR bstr_HoleCornerFaceTag;
			NXString iStringHoleCornerFaceTag = StringIndexCornerFace;

			bstr_HoleCornerFaceTag = ConvertToBSTR(iStringHoleCornerFaceTag);
			pElement_HoleCornerFace->put_text(bstr_HoleCornerFaceTag);
		}

		//倒斜角面
		/*BSTR bstr_BossSideFaceNode;
		NXString iStringBossSideFaceNode = "倒斜角面";

		bstr_BossSideFaceNode = ConvertToBSTR(iStringBossSideFaceNode);

		IXMLDOMElementPtr pElement_BossSideFaceNode;
		pDoc->createElement(bstr_BossSideFaceNode, &pElement_BossSideFaceNode);
		AppendChildToParent(pElement_BossSideFaceNode, pElement_HoleFeatureNode);

		for (std::vector<Face*>::size_type j = 0; j != EachHole->RightVector.size(); j++)
		{
			Face* EachSideFace = EachHole->RightVector[j];

			int indexJ = j + 1;
			char* StringIndexJ = new char;
			sprintf(StringIndexJ, "%d", indexJ);
			NXString NXStringIndexJ = StringIndexJ;

			BSTR bstr_BossSideFace;
			NXString iStringBossSideFace = "倒斜角面." + NXStringIndexJ;

			bstr_BossSideFace = ConvertToBSTR(iStringBossSideFace);

			IXMLDOMElementPtr pElement_BossSideFace;
			pDoc->createElement(bstr_BossSideFace, &pElement_BossSideFace);
			AppendChildToParent(pElement_BossSideFace, pElement_BossSideFaceNode);

			char* StringIndexSideFace = new char;
			sprintf(StringIndexSideFace, "%d", EachSideFace->Tag());

			BSTR bstr_BossSideFaceTag;
			NXString iStringBossSideFaceTag = StringIndexSideFace;

			bstr_BossSideFaceTag = ConvertToBSTR(iStringBossSideFaceTag);
			pElement_BossSideFace->put_text(bstr_BossSideFaceTag);
		}*/


		//孔底面
		BSTR bstr_HoleBottomFaceNode;
		NXString iStringHoleBottomFaceNode = "底面";

		bstr_HoleBottomFaceNode = ConvertToBSTR(iStringHoleBottomFaceNode);

		IXMLDOMElementPtr pElement_HoleBottomFaceNode;
		pDoc->createElement(bstr_HoleBottomFaceNode, &pElement_HoleBottomFaceNode);
		AppendChildToParent(pElement_HoleBottomFaceNode, pElement_HoleFeatureNode);

		for (std::vector<Face*>::size_type j = 0; j != EachHole->BottomFaceVector.size(); j++)
		{
			Face* EachBottomFace = EachHole->BottomFaceVector[j];

			int indexJ = j + 1;
			char* StringIndexJ = new char;
			sprintf(StringIndexJ, "%d", indexJ);
			NXString NXStringIndexJ = StringIndexJ;

			BSTR bstr_HoleBottomFace;
			NXString iStringHoleBottomFace = "底面." + NXStringIndexJ;

			bstr_HoleBottomFace = ConvertToBSTR(iStringHoleBottomFace);

			IXMLDOMElementPtr pElement_HoleBottomFace;
			pDoc->createElement(bstr_HoleBottomFace, &pElement_HoleBottomFace);
			AppendChildToParent(pElement_HoleBottomFace, pElement_HoleBottomFaceNode);

			char* StringIndexBottomFace = new char;
			sprintf(StringIndexBottomFace, "%d", EachBottomFace->Tag());

			BSTR bstr_HoleBottomFaceTag;
			NXString iStringHoleBottomFaceTag = StringIndexBottomFace;

			bstr_HoleBottomFaceTag = ConvertToBSTR(iStringHoleBottomFaceTag);
			pElement_HoleBottomFace->put_text(bstr_HoleBottomFaceTag);
		}
	}

	//凸台单侧直角单侧圆角通孔
	for (std::vector<BossOneRightOneCircleHole*>::size_type i = 0; i != BossOneRightOneCircleHoleSpyResult.size(); i++) //G
	{
		BossOneRightOneCircleHole* EachHole = BossOneRightOneCircleHoleSpyResult[i];//G

		int indexI = i + 1;
		char* StringIndexI = new char;
		sprintf(StringIndexI, "%d", indexI);
		NXString NXStringIndexI = StringIndexI;

		BSTR bstr_HoleFeatureNode;
		NXString iStringHoleFeatureNode = "凸台单侧直角单侧圆角通孔." + NXStringIndexI;//g

		bstr_HoleFeatureNode = ConvertToBSTR(iStringHoleFeatureNode);

		IXMLDOMElementPtr pElement_HoleFeatureNode;
		pDoc->createElement(bstr_HoleFeatureNode, &pElement_HoleFeatureNode);
		AppendChildToParent(pElement_HoleFeatureNode, pElement_Hole);

		//孔顶面
		BSTR bstr_HoleTopFaceNode;
		NXString iStringHoleTopFaceNode = "顶面";

		bstr_HoleTopFaceNode = ConvertToBSTR(iStringHoleTopFaceNode);

		IXMLDOMElementPtr pElement_HoleTopFaceNode;
		pDoc->createElement(bstr_HoleTopFaceNode, &pElement_HoleTopFaceNode);
		AppendChildToParent(pElement_HoleTopFaceNode, pElement_HoleFeatureNode);

		for (std::vector<Face*>::size_type j = 0; j != EachHole->TopFaceVector.size(); j++)
		{
			Face* EachTopFace = EachHole->TopFaceVector[j];

			int indexJ = j + 1;
			char* StringIndexJ = new char;
			sprintf(StringIndexJ, "%d", indexJ);
			NXString NXStringIndexJ = StringIndexJ;

			BSTR bstr_HoleTopFace;
			NXString iStringHoleTopFace = "顶面." + NXStringIndexJ;

			bstr_HoleTopFace = ConvertToBSTR(iStringHoleTopFace);

			IXMLDOMElementPtr pElement_HoleTopFace;
			pDoc->createElement(bstr_HoleTopFace, &pElement_HoleTopFace);
			AppendChildToParent(pElement_HoleTopFace, pElement_HoleTopFaceNode);

			char* StringIndexTopFace = new char;
			sprintf(StringIndexTopFace, "%d", EachTopFace->Tag());

			BSTR bstr_HoleTopFaceTag;
			NXString iStringHoleTopFaceTag = StringIndexTopFace;

			bstr_HoleTopFaceTag = ConvertToBSTR(iStringHoleTopFaceTag);
			pElement_HoleTopFace->put_text(bstr_HoleTopFaceTag);
		}
		//上侧倒圆角面
		BSTR bstr_BossTopFaceNode;
		NXString iStringBossTopFaceNode = "上侧倒圆角面";//g

		bstr_BossTopFaceNode = ConvertToBSTR(iStringBossTopFaceNode);

		IXMLDOMElementPtr pElement_BossTopFaceNode;
		pDoc->createElement(bstr_BossTopFaceNode, &pElement_BossTopFaceNode);
		AppendChildToParent(pElement_BossTopFaceNode, pElement_HoleFeatureNode);//g

		for (std::vector<Face*>::size_type j = 0; j != EachHole->CircleVector.size(); j++)//g
		{
			Face* EachTopFace = EachHole->CircleVector[j];//g

			int indexJ = j + 1;
			char* StringIndexJ = new char;
			sprintf(StringIndexJ, "%d", indexJ);
			NXString NXStringIndexJ = StringIndexJ;

			BSTR bstr_BossTopFace;
			NXString iStringBossTopFace = "上侧倒圆角面." + NXStringIndexJ;//g

			bstr_BossTopFace = ConvertToBSTR(iStringBossTopFace);

			IXMLDOMElementPtr pElement_BossTopFace;
			pDoc->createElement(bstr_BossTopFace, &pElement_BossTopFace);
			AppendChildToParent(pElement_BossTopFace, pElement_BossTopFaceNode);

			char* StringIndexTopFace = new char;
			sprintf(StringIndexTopFace, "%d", EachTopFace->Tag());

			BSTR bstr_BossTopFaceTag;
			NXString iStringBossTopFaceTag = StringIndexTopFace;

			bstr_BossTopFaceTag = ConvertToBSTR(iStringBossTopFaceTag);
			pElement_BossTopFace->put_text(bstr_BossTopFaceTag);
		}

		/*下侧倒斜角面*/
		BSTR bstr_HoleSideFaceNode;
		NXString iStringHoleSideFaceNode = "下侧倒斜角面";//g

		bstr_HoleSideFaceNode = ConvertToBSTR(iStringHoleSideFaceNode);

		IXMLDOMElementPtr pElement_HoleSideFaceNode;
		pDoc->createElement(bstr_HoleSideFaceNode, &pElement_HoleSideFaceNode);
		AppendChildToParent(pElement_HoleSideFaceNode, pElement_HoleFeatureNode);

		for (std::vector<Face*>::size_type j = 0; j != EachHole->RightVector.size(); j++)//g
		{
			Face* EachSideFace = EachHole->RightVector[j];//g

			int indexJ = j + 1;
			char* StringIndexJ = new char;
			sprintf(StringIndexJ, "%d", indexJ);
			NXString NXStringIndexJ = StringIndexJ;

			BSTR bstr_HoleSideFace;
			NXString iStringHoleSideFace = "下侧倒斜角面." + NXStringIndexJ;//g

			bstr_HoleSideFace = ConvertToBSTR(iStringHoleSideFace);

			IXMLDOMElementPtr pElement_HoleSideFace;
			pDoc->createElement(bstr_HoleSideFace, &pElement_HoleSideFace);
			AppendChildToParent(pElement_HoleSideFace, pElement_HoleSideFaceNode);//jie for shang

			char* StringIndexSideFace = new char;
			sprintf(StringIndexSideFace, "%d", EachSideFace->Tag());

			BSTR bstr_HoleSideFaceTag;
			NXString iStringHoleSideFaceTag = StringIndexSideFace;

			bstr_HoleSideFaceTag = ConvertToBSTR(iStringHoleSideFaceTag);
			pElement_HoleSideFace->put_text(bstr_HoleSideFaceTag);
		}

		//侧面
		BSTR bstr_HoleCornerFaceNode;
		NXString iStringHoleCornerFaceNode = "侧面";

		bstr_HoleCornerFaceNode = ConvertToBSTR(iStringHoleCornerFaceNode);

		IXMLDOMElementPtr pElement_HoleCornerFaceNode;
		pDoc->createElement(bstr_HoleCornerFaceNode, &pElement_HoleCornerFaceNode);
		AppendChildToParent(pElement_HoleCornerFaceNode, pElement_HoleFeatureNode);

		for (std::vector<Face*>::size_type j = 0; j != EachHole->InWallVector.size(); j++)
		{
			Face* EachCornerFace = EachHole->InWallVector[j];

			int indexJ = j + 1;
			char* StringIndexJ = new char;
			sprintf(StringIndexJ, "%d", indexJ);
			NXString NXStringIndexJ = StringIndexJ;

			BSTR bstr_HoleCornerFace;
			NXString iStringHoleCornerFace = "侧面." + NXStringIndexJ;

			bstr_HoleCornerFace = ConvertToBSTR(iStringHoleCornerFace);

			IXMLDOMElementPtr pElement_HoleCornerFace;
			pDoc->createElement(bstr_HoleCornerFace, &pElement_HoleCornerFace);
			AppendChildToParent(pElement_HoleCornerFace, pElement_HoleCornerFaceNode);

			char* StringIndexCornerFace = new char;
			sprintf(StringIndexCornerFace, "%d", EachCornerFace->Tag());

			BSTR bstr_HoleCornerFaceTag;
			NXString iStringHoleCornerFaceTag = StringIndexCornerFace;

			bstr_HoleCornerFaceTag = ConvertToBSTR(iStringHoleCornerFaceTag);
			pElement_HoleCornerFace->put_text(bstr_HoleCornerFaceTag);
		}

		//倒斜角面
		/*BSTR bstr_BossSideFaceNode;
		NXString iStringBossSideFaceNode = "倒斜角面";

		bstr_BossSideFaceNode = ConvertToBSTR(iStringBossSideFaceNode);

		IXMLDOMElementPtr pElement_BossSideFaceNode;
		pDoc->createElement(bstr_BossSideFaceNode, &pElement_BossSideFaceNode);
		AppendChildToParent(pElement_BossSideFaceNode, pElement_HoleFeatureNode);

		for (std::vector<Face*>::size_type j = 0; j != EachHole->RightVector.size(); j++)
		{
			Face* EachSideFace = EachHole->RightVector[j];

			int indexJ = j + 1;
			char* StringIndexJ = new char;
			sprintf(StringIndexJ, "%d", indexJ);
			NXString NXStringIndexJ = StringIndexJ;

			BSTR bstr_BossSideFace;
			NXString iStringBossSideFace = "倒斜角面." + NXStringIndexJ;

			bstr_BossSideFace = ConvertToBSTR(iStringBossSideFace);

			IXMLDOMElementPtr pElement_BossSideFace;
			pDoc->createElement(bstr_BossSideFace, &pElement_BossSideFace);
			AppendChildToParent(pElement_BossSideFace, pElement_BossSideFaceNode);

			char* StringIndexSideFace = new char;
			sprintf(StringIndexSideFace, "%d", EachSideFace->Tag());

			BSTR bstr_BossSideFaceTag;
			NXString iStringBossSideFaceTag = StringIndexSideFace;

			bstr_BossSideFaceTag = ConvertToBSTR(iStringBossSideFaceTag);
			pElement_BossSideFace->put_text(bstr_BossSideFaceTag);
		}*/


		//孔底面
		BSTR bstr_HoleBottomFaceNode;
		NXString iStringHoleBottomFaceNode = "底面";

		bstr_HoleBottomFaceNode = ConvertToBSTR(iStringHoleBottomFaceNode);

		IXMLDOMElementPtr pElement_HoleBottomFaceNode;
		pDoc->createElement(bstr_HoleBottomFaceNode, &pElement_HoleBottomFaceNode);
		AppendChildToParent(pElement_HoleBottomFaceNode, pElement_HoleFeatureNode);

		for (std::vector<Face*>::size_type j = 0; j != EachHole->BottomFaceVector.size(); j++)
		{
			Face* EachBottomFace = EachHole->BottomFaceVector[j];

			int indexJ = j + 1;
			char* StringIndexJ = new char;
			sprintf(StringIndexJ, "%d", indexJ);
			NXString NXStringIndexJ = StringIndexJ;

			BSTR bstr_HoleBottomFace;
			NXString iStringHoleBottomFace = "底面." + NXStringIndexJ;

			bstr_HoleBottomFace = ConvertToBSTR(iStringHoleBottomFace);

			IXMLDOMElementPtr pElement_HoleBottomFace;
			pDoc->createElement(bstr_HoleBottomFace, &pElement_HoleBottomFace);
			AppendChildToParent(pElement_HoleBottomFace, pElement_HoleBottomFaceNode);

			char* StringIndexBottomFace = new char;
			sprintf(StringIndexBottomFace, "%d", EachBottomFace->Tag());

			BSTR bstr_HoleBottomFaceTag;
			NXString iStringHoleBottomFaceTag = StringIndexBottomFace;

			bstr_HoleBottomFaceTag = ConvertToBSTR(iStringHoleBottomFaceTag);
			pElement_HoleBottomFace->put_text(bstr_HoleBottomFaceTag);
		}
	}

	//凸台多层通孔
	for (std::vector<BossManyLayerHole*>::size_type i = 0; i != BossManyLayerHoleSpyResult.size(); i++) //G
	{
		BossManyLayerHole* EachHole = BossManyLayerHoleSpyResult[i];//G

		int indexI = i + 1;
		char* StringIndexI = new char;
		sprintf(StringIndexI, "%d", indexI);
		NXString NXStringIndexI = StringIndexI;

		BSTR bstr_HoleFeatureNode;
		NXString iStringHoleFeatureNode = "凸台多层通孔." + NXStringIndexI;//g

		bstr_HoleFeatureNode = ConvertToBSTR(iStringHoleFeatureNode);

		IXMLDOMElementPtr pElement_HoleFeatureNode;
		pDoc->createElement(bstr_HoleFeatureNode, &pElement_HoleFeatureNode);
		AppendChildToParent(pElement_HoleFeatureNode, pElement_Hole);

		//孔顶面
		BSTR bstr_HoleTopFaceNode;
		NXString iStringHoleTopFaceNode = "顶面";

		bstr_HoleTopFaceNode = ConvertToBSTR(iStringHoleTopFaceNode);

		IXMLDOMElementPtr pElement_HoleTopFaceNode;
		pDoc->createElement(bstr_HoleTopFaceNode, &pElement_HoleTopFaceNode);
		AppendChildToParent(pElement_HoleTopFaceNode, pElement_HoleFeatureNode);

		for (std::vector<Face*>::size_type j = 0; j != EachHole->TopFaceVector.size(); j++)
		{
			Face* EachTopFace = EachHole->TopFaceVector[j];

			int indexJ = j + 1;
			char* StringIndexJ = new char;
			sprintf(StringIndexJ, "%d", indexJ);
			NXString NXStringIndexJ = StringIndexJ;

			BSTR bstr_HoleTopFace;
			NXString iStringHoleTopFace = "顶面." + NXStringIndexJ;

			bstr_HoleTopFace = ConvertToBSTR(iStringHoleTopFace);

			IXMLDOMElementPtr pElement_HoleTopFace;
			pDoc->createElement(bstr_HoleTopFace, &pElement_HoleTopFace);
			AppendChildToParent(pElement_HoleTopFace, pElement_HoleTopFaceNode);

			char* StringIndexTopFace = new char;
			sprintf(StringIndexTopFace, "%d", EachTopFace->Tag());

			BSTR bstr_HoleTopFaceTag;
			NXString iStringHoleTopFaceTag = StringIndexTopFace;

			bstr_HoleTopFaceTag = ConvertToBSTR(iStringHoleTopFaceTag);
			pElement_HoleTopFace->put_text(bstr_HoleTopFaceTag);
		}
		//上侧倒圆角面
		BSTR bstr_BossTopFaceNode;
		NXString iStringBossTopFaceNode = "上侧倒圆角面";//g

		bstr_BossTopFaceNode = ConvertToBSTR(iStringBossTopFaceNode);

		IXMLDOMElementPtr pElement_BossTopFaceNode;
		pDoc->createElement(bstr_BossTopFaceNode, &pElement_BossTopFaceNode);
		AppendChildToParent(pElement_BossTopFaceNode, pElement_HoleFeatureNode);//g

		for (std::vector<Face*>::size_type j = 0; j != EachHole->CircleVector.size(); j++)//g
		{
			Face* EachTopFace = EachHole->CircleVector[j];//g

			int indexJ = j + 1;
			char* StringIndexJ = new char;
			sprintf(StringIndexJ, "%d", indexJ);
			NXString NXStringIndexJ = StringIndexJ;

			BSTR bstr_BossTopFace;
			NXString iStringBossTopFace = "上侧倒圆角面." + NXStringIndexJ;//g

			bstr_BossTopFace = ConvertToBSTR(iStringBossTopFace);

			IXMLDOMElementPtr pElement_BossTopFace;
			pDoc->createElement(bstr_BossTopFace, &pElement_BossTopFace);
			AppendChildToParent(pElement_BossTopFace, pElement_BossTopFaceNode);

			char* StringIndexTopFace = new char;
			sprintf(StringIndexTopFace, "%d", EachTopFace->Tag());

			BSTR bstr_BossTopFaceTag;
			NXString iStringBossTopFaceTag = StringIndexTopFace;

			bstr_BossTopFaceTag = ConvertToBSTR(iStringBossTopFaceTag);
			pElement_BossTopFace->put_text(bstr_BossTopFaceTag);
		}

		/*上侧倒斜角面*/
		BSTR bstr_HoleSideFaceNode;
		NXString iStringHoleSideFaceNode = "上侧倒斜角面";//g

		bstr_HoleSideFaceNode = ConvertToBSTR(iStringHoleSideFaceNode);

		IXMLDOMElementPtr pElement_HoleSideFaceNode;
		pDoc->createElement(bstr_HoleSideFaceNode, &pElement_HoleSideFaceNode);
		AppendChildToParent(pElement_HoleSideFaceNode, pElement_HoleFeatureNode);

		for (std::vector<Face*>::size_type j = 0; j != EachHole->RightVector.size(); j++)//g
		{
			Face* EachSideFace = EachHole->RightVector[j];//g

			int indexJ = j + 1;
			char* StringIndexJ = new char;
			sprintf(StringIndexJ, "%d", indexJ);
			NXString NXStringIndexJ = StringIndexJ;

			BSTR bstr_HoleSideFace;
			NXString iStringHoleSideFace = "上侧倒斜角面." + NXStringIndexJ;//g

			bstr_HoleSideFace = ConvertToBSTR(iStringHoleSideFace);

			IXMLDOMElementPtr pElement_HoleSideFace;
			pDoc->createElement(bstr_HoleSideFace, &pElement_HoleSideFace);
			AppendChildToParent(pElement_HoleSideFace, pElement_HoleSideFaceNode);//jie for shang

			char* StringIndexSideFace = new char;
			sprintf(StringIndexSideFace, "%d", EachSideFace->Tag());

			BSTR bstr_HoleSideFaceTag;
			NXString iStringHoleSideFaceTag = StringIndexSideFace;

			bstr_HoleSideFaceTag = ConvertToBSTR(iStringHoleSideFaceTag);
			pElement_HoleSideFace->put_text(bstr_HoleSideFaceTag);
		}

		//侧面
		BSTR bstr_HoleCornerFaceNode;
		NXString iStringHoleCornerFaceNode = "侧面";

		bstr_HoleCornerFaceNode = ConvertToBSTR(iStringHoleCornerFaceNode);

		IXMLDOMElementPtr pElement_HoleCornerFaceNode;
		pDoc->createElement(bstr_HoleCornerFaceNode, &pElement_HoleCornerFaceNode);
		AppendChildToParent(pElement_HoleCornerFaceNode, pElement_HoleFeatureNode);

		for (std::vector<Face*>::size_type j = 0; j != EachHole->InWallVector.size(); j++)
		{
			Face* EachCornerFace = EachHole->InWallVector[j];

			int indexJ = j + 1;
			char* StringIndexJ = new char;
			sprintf(StringIndexJ, "%d", indexJ);
			NXString NXStringIndexJ = StringIndexJ;

			BSTR bstr_HoleCornerFace;
			NXString iStringHoleCornerFace = "侧面." + NXStringIndexJ;

			bstr_HoleCornerFace = ConvertToBSTR(iStringHoleCornerFace);

			IXMLDOMElementPtr pElement_HoleCornerFace;
			pDoc->createElement(bstr_HoleCornerFace, &pElement_HoleCornerFace);
			AppendChildToParent(pElement_HoleCornerFace, pElement_HoleCornerFaceNode);

			char* StringIndexCornerFace = new char;
			sprintf(StringIndexCornerFace, "%d", EachCornerFace->Tag());

			BSTR bstr_HoleCornerFaceTag;
			NXString iStringHoleCornerFaceTag = StringIndexCornerFace;

			bstr_HoleCornerFaceTag = ConvertToBSTR(iStringHoleCornerFaceTag);
			pElement_HoleCornerFace->put_text(bstr_HoleCornerFaceTag);
		}

		//过渡平面
		BSTR bstr_BossSideFaceNode;
		NXString iStringBossSideFaceNode = "过渡平面";

		bstr_BossSideFaceNode = ConvertToBSTR(iStringBossSideFaceNode);

		IXMLDOMElementPtr pElement_BossSideFaceNode;
		pDoc->createElement(bstr_BossSideFaceNode, &pElement_BossSideFaceNode);
		AppendChildToParent(pElement_BossSideFaceNode, pElement_HoleFeatureNode);

		for (std::vector<Face*>::size_type j = 0; j != EachHole->PlanerVector.size(); j++)
		{
			Face* EachSideFace = EachHole->PlanerVector[j];

			int indexJ = j + 1;
			char* StringIndexJ = new char;
			sprintf(StringIndexJ, "%d", indexJ);
			NXString NXStringIndexJ = StringIndexJ;

			BSTR bstr_BossSideFace;
			NXString iStringBossSideFace = "过渡平面." + NXStringIndexJ;

			bstr_BossSideFace = ConvertToBSTR(iStringBossSideFace);

			IXMLDOMElementPtr pElement_BossSideFace;
			pDoc->createElement(bstr_BossSideFace, &pElement_BossSideFace);
			AppendChildToParent(pElement_BossSideFace, pElement_BossSideFaceNode);

			char* StringIndexSideFace = new char;
			sprintf(StringIndexSideFace, "%d", EachSideFace->Tag());

			BSTR bstr_BossSideFaceTag;
			NXString iStringBossSideFaceTag = StringIndexSideFace;

			bstr_BossSideFaceTag = ConvertToBSTR(iStringBossSideFaceTag);
			pElement_BossSideFace->put_text(bstr_BossSideFaceTag);
		}


		//孔底面
		BSTR bstr_HoleBottomFaceNode;
		NXString iStringHoleBottomFaceNode = "底面";

		bstr_HoleBottomFaceNode = ConvertToBSTR(iStringHoleBottomFaceNode);

		IXMLDOMElementPtr pElement_HoleBottomFaceNode;
		pDoc->createElement(bstr_HoleBottomFaceNode, &pElement_HoleBottomFaceNode);
		AppendChildToParent(pElement_HoleBottomFaceNode, pElement_HoleFeatureNode);

		for (std::vector<Face*>::size_type j = 0; j != EachHole->BottomFaceVector.size(); j++)
		{
			Face* EachBottomFace = EachHole->BottomFaceVector[j];

			int indexJ = j + 1;
			char* StringIndexJ = new char;
			sprintf(StringIndexJ, "%d", indexJ);
			NXString NXStringIndexJ = StringIndexJ;

			BSTR bstr_HoleBottomFace;
			NXString iStringHoleBottomFace = "底面." + NXStringIndexJ;

			bstr_HoleBottomFace = ConvertToBSTR(iStringHoleBottomFace);

			IXMLDOMElementPtr pElement_HoleBottomFace;
			pDoc->createElement(bstr_HoleBottomFace, &pElement_HoleBottomFace);
			AppendChildToParent(pElement_HoleBottomFace, pElement_HoleBottomFaceNode);

			char* StringIndexBottomFace = new char;
			sprintf(StringIndexBottomFace, "%d", EachBottomFace->Tag());

			BSTR bstr_HoleBottomFaceTag;
			NXString iStringHoleBottomFaceTag = StringIndexBottomFace;

			bstr_HoleBottomFaceTag = ConvertToBSTR(iStringHoleBottomFaceTag);
			pElement_HoleBottomFace->put_text(bstr_HoleBottomFaceTag);
		}
	}

	//凸台外宽内窄复合通孔
	for (std::vector<BossOutsideWideCombineHole*>::size_type i = 0; i != BossOutsideWideCombineHoleSpyResult.size(); i++) //G
	{
		BossOutsideWideCombineHole* EachHole = BossOutsideWideCombineHoleSpyResult[i];//G

		int indexI = i + 1;
		char* StringIndexI = new char;
		sprintf(StringIndexI, "%d", indexI);
		NXString NXStringIndexI = StringIndexI;

		BSTR bstr_HoleFeatureNode;
		NXString iStringHoleFeatureNode = "凸台外宽内窄复合通孔." + NXStringIndexI;//g

		bstr_HoleFeatureNode = ConvertToBSTR(iStringHoleFeatureNode);

		IXMLDOMElementPtr pElement_HoleFeatureNode;
		pDoc->createElement(bstr_HoleFeatureNode, &pElement_HoleFeatureNode);
		AppendChildToParent(pElement_HoleFeatureNode, pElement_Hole);

		//孔顶面
		BSTR bstr_HoleTopFaceNode;
		NXString iStringHoleTopFaceNode = "顶面";

		bstr_HoleTopFaceNode = ConvertToBSTR(iStringHoleTopFaceNode);

		IXMLDOMElementPtr pElement_HoleTopFaceNode;
		pDoc->createElement(bstr_HoleTopFaceNode, &pElement_HoleTopFaceNode);
		AppendChildToParent(pElement_HoleTopFaceNode, pElement_HoleFeatureNode);

		for (std::vector<Face*>::size_type j = 0; j != EachHole->TopFaceVector.size(); j++)
		{
			Face* EachTopFace = EachHole->TopFaceVector[j];

			int indexJ = j + 1;
			char* StringIndexJ = new char;
			sprintf(StringIndexJ, "%d", indexJ);
			NXString NXStringIndexJ = StringIndexJ;

			BSTR bstr_HoleTopFace;
			NXString iStringHoleTopFace = "顶面." + NXStringIndexJ;

			bstr_HoleTopFace = ConvertToBSTR(iStringHoleTopFace);

			IXMLDOMElementPtr pElement_HoleTopFace;
			pDoc->createElement(bstr_HoleTopFace, &pElement_HoleTopFace);
			AppendChildToParent(pElement_HoleTopFace, pElement_HoleTopFaceNode);

			char* StringIndexTopFace = new char;
			sprintf(StringIndexTopFace, "%d", EachTopFace->Tag());

			BSTR bstr_HoleTopFaceTag;
			NXString iStringHoleTopFaceTag = StringIndexTopFace;

			bstr_HoleTopFaceTag = ConvertToBSTR(iStringHoleTopFaceTag);
			pElement_HoleTopFace->put_text(bstr_HoleTopFaceTag);
		}
		//上侧面
		BSTR bstr_BossTopFaceNode;
		NXString iStringBossTopFaceNode = "上侧面";//g

		bstr_BossTopFaceNode = ConvertToBSTR(iStringBossTopFaceNode);

		IXMLDOMElementPtr pElement_BossTopFaceNode;
		pDoc->createElement(bstr_BossTopFaceNode, &pElement_BossTopFaceNode);
		AppendChildToParent(pElement_BossTopFaceNode, pElement_HoleFeatureNode);//g

		for (std::vector<Face*>::size_type j = 0; j != EachHole->TopWallVector.size(); j++)//g
		{
			Face* EachTopFace = EachHole->TopWallVector[j];//g

			int indexJ = j + 1;
			char* StringIndexJ = new char;
			sprintf(StringIndexJ, "%d", indexJ);
			NXString NXStringIndexJ = StringIndexJ;

			BSTR bstr_BossTopFace;
			NXString iStringBossTopFace = "上侧面." + NXStringIndexJ;//g

			bstr_BossTopFace = ConvertToBSTR(iStringBossTopFace);

			IXMLDOMElementPtr pElement_BossTopFace;
			pDoc->createElement(bstr_BossTopFace, &pElement_BossTopFace);
			AppendChildToParent(pElement_BossTopFace, pElement_BossTopFaceNode);

			char* StringIndexTopFace = new char;
			sprintf(StringIndexTopFace, "%d", EachTopFace->Tag());

			BSTR bstr_BossTopFaceTag;
			NXString iStringBossTopFaceTag = StringIndexTopFace;

			bstr_BossTopFaceTag = ConvertToBSTR(iStringBossTopFaceTag);
			pElement_BossTopFace->put_text(bstr_BossTopFaceTag);
		}

		//过渡平面
		BSTR bstr_BossSideFaceNode;
		NXString iStringBossSideFaceNode = "过渡平面";

		bstr_BossSideFaceNode = ConvertToBSTR(iStringBossSideFaceNode);

		IXMLDOMElementPtr pElement_BossSideFaceNode;
		pDoc->createElement(bstr_BossSideFaceNode, &pElement_BossSideFaceNode);
		AppendChildToParent(pElement_BossSideFaceNode, pElement_HoleFeatureNode);

		for (std::vector<Face*>::size_type j = 0; j != EachHole->PlanerVector.size(); j++)
		{
			Face* EachSideFace = EachHole->PlanerVector[j];

			int indexJ = j + 1;
			char* StringIndexJ = new char;
			sprintf(StringIndexJ, "%d", indexJ);
			NXString NXStringIndexJ = StringIndexJ;

			BSTR bstr_BossSideFace;
			NXString iStringBossSideFace = "过渡平面." + NXStringIndexJ;

			bstr_BossSideFace = ConvertToBSTR(iStringBossSideFace);

			IXMLDOMElementPtr pElement_BossSideFace;
			pDoc->createElement(bstr_BossSideFace, &pElement_BossSideFace);
			AppendChildToParent(pElement_BossSideFace, pElement_BossSideFaceNode);

			char* StringIndexSideFace = new char;
			sprintf(StringIndexSideFace, "%d", EachSideFace->Tag());

			BSTR bstr_BossSideFaceTag;
			NXString iStringBossSideFaceTag = StringIndexSideFace;

			bstr_BossSideFaceTag = ConvertToBSTR(iStringBossSideFaceTag);
			pElement_BossSideFace->put_text(bstr_BossSideFaceTag);
		}


		/*下侧面*/
		BSTR bstr_HoleSideFaceNode;
		NXString iStringHoleSideFaceNode = "下侧面";//g

		bstr_HoleSideFaceNode = ConvertToBSTR(iStringHoleSideFaceNode);

		IXMLDOMElementPtr pElement_HoleSideFaceNode;
		pDoc->createElement(bstr_HoleSideFaceNode, &pElement_HoleSideFaceNode);
		AppendChildToParent(pElement_HoleSideFaceNode, pElement_HoleFeatureNode);

		for (std::vector<Face*>::size_type j = 0; j != EachHole->BottomWallVector.size(); j++)//g
		{
			Face* EachSideFace = EachHole->BottomWallVector[j];//g

			int indexJ = j + 1;
			char* StringIndexJ = new char;
			sprintf(StringIndexJ, "%d", indexJ);
			NXString NXStringIndexJ = StringIndexJ;

			BSTR bstr_HoleSideFace;
			NXString iStringHoleSideFace = "下侧面." + NXStringIndexJ;//g

			bstr_HoleSideFace = ConvertToBSTR(iStringHoleSideFace);

			IXMLDOMElementPtr pElement_HoleSideFace;
			pDoc->createElement(bstr_HoleSideFace, &pElement_HoleSideFace);
			AppendChildToParent(pElement_HoleSideFace, pElement_HoleSideFaceNode);//jie for shang

			char* StringIndexSideFace = new char;
			sprintf(StringIndexSideFace, "%d", EachSideFace->Tag());

			BSTR bstr_HoleSideFaceTag;
			NXString iStringHoleSideFaceTag = StringIndexSideFace;

			bstr_HoleSideFaceTag = ConvertToBSTR(iStringHoleSideFaceTag);
			pElement_HoleSideFace->put_text(bstr_HoleSideFaceTag);
		}

		//侧面
		/*BSTR bstr_HoleCornerFaceNode;
		NXString iStringHoleCornerFaceNode = "侧面";

		bstr_HoleCornerFaceNode = ConvertToBSTR(iStringHoleCornerFaceNode);

		IXMLDOMElementPtr pElement_HoleCornerFaceNode;
		pDoc->createElement(bstr_HoleCornerFaceNode, &pElement_HoleCornerFaceNode);
		AppendChildToParent(pElement_HoleCornerFaceNode, pElement_HoleFeatureNode);

		for (std::vector<Face*>::size_type j = 0; j != EachHole->InWallVector.size(); j++)
		{
			Face* EachCornerFace = EachHole->InWallVector[j];

			int indexJ = j + 1;
			char* StringIndexJ = new char;
			sprintf(StringIndexJ, "%d", indexJ);
			NXString NXStringIndexJ = StringIndexJ;

			BSTR bstr_HoleCornerFace;
			NXString iStringHoleCornerFace = "侧面." + NXStringIndexJ;

			bstr_HoleCornerFace = ConvertToBSTR(iStringHoleCornerFace);

			IXMLDOMElementPtr pElement_HoleCornerFace;
			pDoc->createElement(bstr_HoleCornerFace, &pElement_HoleCornerFace);
			AppendChildToParent(pElement_HoleCornerFace, pElement_HoleCornerFaceNode);

			char* StringIndexCornerFace = new char;
			sprintf(StringIndexCornerFace, "%d", EachCornerFace->Tag());

			BSTR bstr_HoleCornerFaceTag;
			NXString iStringHoleCornerFaceTag = StringIndexCornerFace;

			bstr_HoleCornerFaceTag = ConvertToBSTR(iStringHoleCornerFaceTag);
			pElement_HoleCornerFace->put_text(bstr_HoleCornerFaceTag);
		}*/

		

		//孔底面
		BSTR bstr_HoleBottomFaceNode;
		NXString iStringHoleBottomFaceNode = "底面";

		bstr_HoleBottomFaceNode = ConvertToBSTR(iStringHoleBottomFaceNode);

		IXMLDOMElementPtr pElement_HoleBottomFaceNode;
		pDoc->createElement(bstr_HoleBottomFaceNode, &pElement_HoleBottomFaceNode);
		AppendChildToParent(pElement_HoleBottomFaceNode, pElement_HoleFeatureNode);

		for (std::vector<Face*>::size_type j = 0; j != EachHole->BottomFaceVector.size(); j++)
		{
			Face* EachBottomFace = EachHole->BottomFaceVector[j];

			int indexJ = j + 1;
			char* StringIndexJ = new char;
			sprintf(StringIndexJ, "%d", indexJ);
			NXString NXStringIndexJ = StringIndexJ;

			BSTR bstr_HoleBottomFace;
			NXString iStringHoleBottomFace = "底面." + NXStringIndexJ;

			bstr_HoleBottomFace = ConvertToBSTR(iStringHoleBottomFace);

			IXMLDOMElementPtr pElement_HoleBottomFace;
			pDoc->createElement(bstr_HoleBottomFace, &pElement_HoleBottomFace);
			AppendChildToParent(pElement_HoleBottomFace, pElement_HoleBottomFaceNode);

			char* StringIndexBottomFace = new char;
			sprintf(StringIndexBottomFace, "%d", EachBottomFace->Tag());

			BSTR bstr_HoleBottomFaceTag;
			NXString iStringHoleBottomFaceTag = StringIndexBottomFace;

			bstr_HoleBottomFaceTag = ConvertToBSTR(iStringHoleBottomFaceTag);
			pElement_HoleBottomFace->put_text(bstr_HoleBottomFaceTag);
		}
	}

	//凸台内宽外窄复合通孔
	for (std::vector<BossInsideWideCombineHole*>::size_type i = 0; i != BossInsideWideCombineHoleSpyResult.size(); i++) //G
	{
		BossInsideWideCombineHole* EachHole = BossInsideWideCombineHoleSpyResult[i];//G

		int indexI = i + 1;
		char* StringIndexI = new char;
		sprintf(StringIndexI, "%d", indexI);
		NXString NXStringIndexI = StringIndexI;

		BSTR bstr_HoleFeatureNode;
		NXString iStringHoleFeatureNode = "凸台内宽外窄复合通孔." + NXStringIndexI;//g

		bstr_HoleFeatureNode = ConvertToBSTR(iStringHoleFeatureNode);

		IXMLDOMElementPtr pElement_HoleFeatureNode;
		pDoc->createElement(bstr_HoleFeatureNode, &pElement_HoleFeatureNode);
		AppendChildToParent(pElement_HoleFeatureNode, pElement_Hole);

		//孔顶面
		BSTR bstr_HoleTopFaceNode;
		NXString iStringHoleTopFaceNode = "顶面";

		bstr_HoleTopFaceNode = ConvertToBSTR(iStringHoleTopFaceNode);

		IXMLDOMElementPtr pElement_HoleTopFaceNode;
		pDoc->createElement(bstr_HoleTopFaceNode, &pElement_HoleTopFaceNode);
		AppendChildToParent(pElement_HoleTopFaceNode, pElement_HoleFeatureNode);

		for (std::vector<Face*>::size_type j = 0; j != EachHole->TopFaceVector.size(); j++)
		{
			Face* EachTopFace = EachHole->TopFaceVector[j];

			int indexJ = j + 1;
			char* StringIndexJ = new char;
			sprintf(StringIndexJ, "%d", indexJ);
			NXString NXStringIndexJ = StringIndexJ;

			BSTR bstr_HoleTopFace;
			NXString iStringHoleTopFace = "顶面." + NXStringIndexJ;

			bstr_HoleTopFace = ConvertToBSTR(iStringHoleTopFace);

			IXMLDOMElementPtr pElement_HoleTopFace;
			pDoc->createElement(bstr_HoleTopFace, &pElement_HoleTopFace);
			AppendChildToParent(pElement_HoleTopFace, pElement_HoleTopFaceNode);

			char* StringIndexTopFace = new char;
			sprintf(StringIndexTopFace, "%d", EachTopFace->Tag());

			BSTR bstr_HoleTopFaceTag;
			NXString iStringHoleTopFaceTag = StringIndexTopFace;

			bstr_HoleTopFaceTag = ConvertToBSTR(iStringHoleTopFaceTag);
			pElement_HoleTopFace->put_text(bstr_HoleTopFaceTag);
		}
		//上侧面
		BSTR bstr_BossTopFaceNode;
		NXString iStringBossTopFaceNode = "上侧面";//g

		bstr_BossTopFaceNode = ConvertToBSTR(iStringBossTopFaceNode);

		IXMLDOMElementPtr pElement_BossTopFaceNode;
		pDoc->createElement(bstr_BossTopFaceNode, &pElement_BossTopFaceNode);
		AppendChildToParent(pElement_BossTopFaceNode, pElement_HoleFeatureNode);//g

		for (std::vector<Face*>::size_type j = 0; j != EachHole->TopWallVector.size(); j++)//g
		{
			Face* EachTopFace = EachHole->TopWallVector[j];//g

			int indexJ = j + 1;
			char* StringIndexJ = new char;
			sprintf(StringIndexJ, "%d", indexJ);
			NXString NXStringIndexJ = StringIndexJ;

			BSTR bstr_BossTopFace;
			NXString iStringBossTopFace = "上侧面." + NXStringIndexJ;//g

			bstr_BossTopFace = ConvertToBSTR(iStringBossTopFace);

			IXMLDOMElementPtr pElement_BossTopFace;
			pDoc->createElement(bstr_BossTopFace, &pElement_BossTopFace);
			AppendChildToParent(pElement_BossTopFace, pElement_BossTopFaceNode);

			char* StringIndexTopFace = new char;
			sprintf(StringIndexTopFace, "%d", EachTopFace->Tag());

			BSTR bstr_BossTopFaceTag;
			NXString iStringBossTopFaceTag = StringIndexTopFace;

			bstr_BossTopFaceTag = ConvertToBSTR(iStringBossTopFaceTag);
			pElement_BossTopFace->put_text(bstr_BossTopFaceTag);
		}

		//过渡平面
		BSTR bstr_BossSideFaceNode;
		NXString iStringBossSideFaceNode = "过渡平面";

		bstr_BossSideFaceNode = ConvertToBSTR(iStringBossSideFaceNode);

		IXMLDOMElementPtr pElement_BossSideFaceNode;
		pDoc->createElement(bstr_BossSideFaceNode, &pElement_BossSideFaceNode);
		AppendChildToParent(pElement_BossSideFaceNode, pElement_HoleFeatureNode);

		for (std::vector<Face*>::size_type j = 0; j != EachHole->PlanerVector.size(); j++)
		{
			Face* EachSideFace = EachHole->PlanerVector[j];

			int indexJ = j + 1;
			char* StringIndexJ = new char;
			sprintf(StringIndexJ, "%d", indexJ);
			NXString NXStringIndexJ = StringIndexJ;

			BSTR bstr_BossSideFace;
			NXString iStringBossSideFace = "过渡平面." + NXStringIndexJ;

			bstr_BossSideFace = ConvertToBSTR(iStringBossSideFace);

			IXMLDOMElementPtr pElement_BossSideFace;
			pDoc->createElement(bstr_BossSideFace, &pElement_BossSideFace);
			AppendChildToParent(pElement_BossSideFace, pElement_BossSideFaceNode);

			char* StringIndexSideFace = new char;
			sprintf(StringIndexSideFace, "%d", EachSideFace->Tag());

			BSTR bstr_BossSideFaceTag;
			NXString iStringBossSideFaceTag = StringIndexSideFace;

			bstr_BossSideFaceTag = ConvertToBSTR(iStringBossSideFaceTag);
			pElement_BossSideFace->put_text(bstr_BossSideFaceTag);
		}


		/*下侧面*/
		BSTR bstr_HoleSideFaceNode;
		NXString iStringHoleSideFaceNode = "下侧面";//g

		bstr_HoleSideFaceNode = ConvertToBSTR(iStringHoleSideFaceNode);

		IXMLDOMElementPtr pElement_HoleSideFaceNode;
		pDoc->createElement(bstr_HoleSideFaceNode, &pElement_HoleSideFaceNode);
		AppendChildToParent(pElement_HoleSideFaceNode, pElement_HoleFeatureNode);

		for (std::vector<Face*>::size_type j = 0; j != EachHole->BottomWallVector.size(); j++)//g
		{
			Face* EachSideFace = EachHole->BottomWallVector[j];//g

			int indexJ = j + 1;
			char* StringIndexJ = new char;
			sprintf(StringIndexJ, "%d", indexJ);
			NXString NXStringIndexJ = StringIndexJ;

			BSTR bstr_HoleSideFace;
			NXString iStringHoleSideFace = "下侧面." + NXStringIndexJ;//g

			bstr_HoleSideFace = ConvertToBSTR(iStringHoleSideFace);

			IXMLDOMElementPtr pElement_HoleSideFace;
			pDoc->createElement(bstr_HoleSideFace, &pElement_HoleSideFace);
			AppendChildToParent(pElement_HoleSideFace, pElement_HoleSideFaceNode);//jie for shang

			char* StringIndexSideFace = new char;
			sprintf(StringIndexSideFace, "%d", EachSideFace->Tag());

			BSTR bstr_HoleSideFaceTag;
			NXString iStringHoleSideFaceTag = StringIndexSideFace;

			bstr_HoleSideFaceTag = ConvertToBSTR(iStringHoleSideFaceTag);
			pElement_HoleSideFace->put_text(bstr_HoleSideFaceTag);
		}

		//侧面
		/*BSTR bstr_HoleCornerFaceNode;
		NXString iStringHoleCornerFaceNode = "侧面";

		bstr_HoleCornerFaceNode = ConvertToBSTR(iStringHoleCornerFaceNode);

		IXMLDOMElementPtr pElement_HoleCornerFaceNode;
		pDoc->createElement(bstr_HoleCornerFaceNode, &pElement_HoleCornerFaceNode);
		AppendChildToParent(pElement_HoleCornerFaceNode, pElement_HoleFeatureNode);

		for (std::vector<Face*>::size_type j = 0; j != EachHole->InWallVector.size(); j++)
		{
			Face* EachCornerFace = EachHole->InWallVector[j];

			int indexJ = j + 1;
			char* StringIndexJ = new char;
			sprintf(StringIndexJ, "%d", indexJ);
			NXString NXStringIndexJ = StringIndexJ;

			BSTR bstr_HoleCornerFace;
			NXString iStringHoleCornerFace = "侧面." + NXStringIndexJ;

			bstr_HoleCornerFace = ConvertToBSTR(iStringHoleCornerFace);

			IXMLDOMElementPtr pElement_HoleCornerFace;
			pDoc->createElement(bstr_HoleCornerFace, &pElement_HoleCornerFace);
			AppendChildToParent(pElement_HoleCornerFace, pElement_HoleCornerFaceNode);

			char* StringIndexCornerFace = new char;
			sprintf(StringIndexCornerFace, "%d", EachCornerFace->Tag());

			BSTR bstr_HoleCornerFaceTag;
			NXString iStringHoleCornerFaceTag = StringIndexCornerFace;

			bstr_HoleCornerFaceTag = ConvertToBSTR(iStringHoleCornerFaceTag);
			pElement_HoleCornerFace->put_text(bstr_HoleCornerFaceTag);
		}*/

		

		//孔底面
		BSTR bstr_HoleBottomFaceNode;
		NXString iStringHoleBottomFaceNode = "底面";

		bstr_HoleBottomFaceNode = ConvertToBSTR(iStringHoleBottomFaceNode);

		IXMLDOMElementPtr pElement_HoleBottomFaceNode;
		pDoc->createElement(bstr_HoleBottomFaceNode, &pElement_HoleBottomFaceNode);
		AppendChildToParent(pElement_HoleBottomFaceNode, pElement_HoleFeatureNode);

		for (std::vector<Face*>::size_type j = 0; j != EachHole->BottomFaceVector.size(); j++)
		{
			Face* EachBottomFace = EachHole->BottomFaceVector[j];

			int indexJ = j + 1;
			char* StringIndexJ = new char;
			sprintf(StringIndexJ, "%d", indexJ);
			NXString NXStringIndexJ = StringIndexJ;

			BSTR bstr_HoleBottomFace;
			NXString iStringHoleBottomFace = "底面." + NXStringIndexJ;

			bstr_HoleBottomFace = ConvertToBSTR(iStringHoleBottomFace);

			IXMLDOMElementPtr pElement_HoleBottomFace;
			pDoc->createElement(bstr_HoleBottomFace, &pElement_HoleBottomFace);
			AppendChildToParent(pElement_HoleBottomFace, pElement_HoleBottomFaceNode);

			char* StringIndexBottomFace = new char;
			sprintf(StringIndexBottomFace, "%d", EachBottomFace->Tag());

			BSTR bstr_HoleBottomFaceTag;
			NXString iStringHoleBottomFaceTag = StringIndexBottomFace;

			bstr_HoleBottomFaceTag = ConvertToBSTR(iStringHoleBottomFaceTag);
			pElement_HoleBottomFace->put_text(bstr_HoleBottomFaceTag);
		}
	}

	//凸台中间倒圆角复合通孔
	for (std::vector<BossMidCricleCombineHole*>::size_type i = 0; i != BossMidCricleCombineHoleSpyResult.size(); i++) //G
	{
		BossMidCricleCombineHole* EachHole = BossMidCricleCombineHoleSpyResult[i];//G

		int indexI = i + 1;
		char* StringIndexI = new char;
		sprintf(StringIndexI, "%d", indexI);
		NXString NXStringIndexI = StringIndexI;

		BSTR bstr_HoleFeatureNode;
		NXString iStringHoleFeatureNode = "凸台中间倒圆角复合通孔." + NXStringIndexI;//g

		bstr_HoleFeatureNode = ConvertToBSTR(iStringHoleFeatureNode);

		IXMLDOMElementPtr pElement_HoleFeatureNode;
		pDoc->createElement(bstr_HoleFeatureNode, &pElement_HoleFeatureNode);
		AppendChildToParent(pElement_HoleFeatureNode, pElement_Hole);

		//孔顶面
		BSTR bstr_HoleTopFaceNode;
		NXString iStringHoleTopFaceNode = "顶面";

		bstr_HoleTopFaceNode = ConvertToBSTR(iStringHoleTopFaceNode);

		IXMLDOMElementPtr pElement_HoleTopFaceNode;
		pDoc->createElement(bstr_HoleTopFaceNode, &pElement_HoleTopFaceNode);
		AppendChildToParent(pElement_HoleTopFaceNode, pElement_HoleFeatureNode);

		for (std::vector<Face*>::size_type j = 0; j != EachHole->TopFaceVector.size(); j++)
		{
			Face* EachTopFace = EachHole->TopFaceVector[j];

			int indexJ = j + 1;
			char* StringIndexJ = new char;
			sprintf(StringIndexJ, "%d", indexJ);
			NXString NXStringIndexJ = StringIndexJ;

			BSTR bstr_HoleTopFace;
			NXString iStringHoleTopFace = "顶面." + NXStringIndexJ;

			bstr_HoleTopFace = ConvertToBSTR(iStringHoleTopFace);

			IXMLDOMElementPtr pElement_HoleTopFace;
			pDoc->createElement(bstr_HoleTopFace, &pElement_HoleTopFace);
			AppendChildToParent(pElement_HoleTopFace, pElement_HoleTopFaceNode);

			char* StringIndexTopFace = new char;
			sprintf(StringIndexTopFace, "%d", EachTopFace->Tag());

			BSTR bstr_HoleTopFaceTag;
			NXString iStringHoleTopFaceTag = StringIndexTopFace;

			bstr_HoleTopFaceTag = ConvertToBSTR(iStringHoleTopFaceTag);
			pElement_HoleTopFace->put_text(bstr_HoleTopFaceTag);
		}
		//上侧面
		BSTR bstr_BossTopFaceNode;
		NXString iStringBossTopFaceNode = "上侧面";//g

		bstr_BossTopFaceNode = ConvertToBSTR(iStringBossTopFaceNode);

		IXMLDOMElementPtr pElement_BossTopFaceNode;
		pDoc->createElement(bstr_BossTopFaceNode, &pElement_BossTopFaceNode);
		AppendChildToParent(pElement_BossTopFaceNode, pElement_HoleFeatureNode);//g

		for (std::vector<Face*>::size_type j = 0; j != EachHole->TopWallVector.size(); j++)//g
		{
			Face* EachTopFace = EachHole->TopWallVector[j];//g

			int indexJ = j + 1;
			char* StringIndexJ = new char;
			sprintf(StringIndexJ, "%d", indexJ);
			NXString NXStringIndexJ = StringIndexJ;

			BSTR bstr_BossTopFace;
			NXString iStringBossTopFace = "上侧面." + NXStringIndexJ;//g

			bstr_BossTopFace = ConvertToBSTR(iStringBossTopFace);

			IXMLDOMElementPtr pElement_BossTopFace;
			pDoc->createElement(bstr_BossTopFace, &pElement_BossTopFace);
			AppendChildToParent(pElement_BossTopFace, pElement_BossTopFaceNode);

			char* StringIndexTopFace = new char;
			sprintf(StringIndexTopFace, "%d", EachTopFace->Tag());

			BSTR bstr_BossTopFaceTag;
			NXString iStringBossTopFaceTag = StringIndexTopFace;

			bstr_BossTopFaceTag = ConvertToBSTR(iStringBossTopFaceTag);
			pElement_BossTopFace->put_text(bstr_BossTopFaceTag);
		}

		//倒圆角面
		BSTR bstr_HoleCornerFaceNode;
		NXString iStringHoleCornerFaceNode = "倒圆角面";

		bstr_HoleCornerFaceNode = ConvertToBSTR(iStringHoleCornerFaceNode);

		IXMLDOMElementPtr pElement_HoleCornerFaceNode;
		pDoc->createElement(bstr_HoleCornerFaceNode, &pElement_HoleCornerFaceNode);
		AppendChildToParent(pElement_HoleCornerFaceNode, pElement_HoleFeatureNode);

		for (std::vector<Face*>::size_type j = 0; j != EachHole->CircleVector.size(); j++)
		{
			Face* EachCornerFace = EachHole->CircleVector[j];

			int indexJ = j + 1;
			char* StringIndexJ = new char;
			sprintf(StringIndexJ, "%d", indexJ);
			NXString NXStringIndexJ = StringIndexJ;

			BSTR bstr_HoleCornerFace;
			NXString iStringHoleCornerFace = "倒圆角面." + NXStringIndexJ;

			bstr_HoleCornerFace = ConvertToBSTR(iStringHoleCornerFace);

			IXMLDOMElementPtr pElement_HoleCornerFace;
			pDoc->createElement(bstr_HoleCornerFace, &pElement_HoleCornerFace);
			AppendChildToParent(pElement_HoleCornerFace, pElement_HoleCornerFaceNode);

			char* StringIndexCornerFace = new char;
			sprintf(StringIndexCornerFace, "%d", EachCornerFace->Tag());

			BSTR bstr_HoleCornerFaceTag;
			NXString iStringHoleCornerFaceTag = StringIndexCornerFace;

			bstr_HoleCornerFaceTag = ConvertToBSTR(iStringHoleCornerFaceTag);
			pElement_HoleCornerFace->put_text(bstr_HoleCornerFaceTag);
		}


		//过渡平面
		BSTR bstr_BossSideFaceNode;
		NXString iStringBossSideFaceNode = "过渡平面";

		bstr_BossSideFaceNode = ConvertToBSTR(iStringBossSideFaceNode);

		IXMLDOMElementPtr pElement_BossSideFaceNode;
		pDoc->createElement(bstr_BossSideFaceNode, &pElement_BossSideFaceNode);
		AppendChildToParent(pElement_BossSideFaceNode, pElement_HoleFeatureNode);

		for (std::vector<Face*>::size_type j = 0; j != EachHole->PlanerVector.size(); j++)
		{
			Face* EachSideFace = EachHole->PlanerVector[j];

			int indexJ = j + 1;
			char* StringIndexJ = new char;
			sprintf(StringIndexJ, "%d", indexJ);
			NXString NXStringIndexJ = StringIndexJ;

			BSTR bstr_BossSideFace;
			NXString iStringBossSideFace = "过渡平面." + NXStringIndexJ;

			bstr_BossSideFace = ConvertToBSTR(iStringBossSideFace);

			IXMLDOMElementPtr pElement_BossSideFace;
			pDoc->createElement(bstr_BossSideFace, &pElement_BossSideFace);
			AppendChildToParent(pElement_BossSideFace, pElement_BossSideFaceNode);

			char* StringIndexSideFace = new char;
			sprintf(StringIndexSideFace, "%d", EachSideFace->Tag());

			BSTR bstr_BossSideFaceTag;
			NXString iStringBossSideFaceTag = StringIndexSideFace;

			bstr_BossSideFaceTag = ConvertToBSTR(iStringBossSideFaceTag);
			pElement_BossSideFace->put_text(bstr_BossSideFaceTag);
		}


		/*下侧面*/
		BSTR bstr_HoleSideFaceNode;
		NXString iStringHoleSideFaceNode = "下侧面";//g

		bstr_HoleSideFaceNode = ConvertToBSTR(iStringHoleSideFaceNode);

		IXMLDOMElementPtr pElement_HoleSideFaceNode;
		pDoc->createElement(bstr_HoleSideFaceNode, &pElement_HoleSideFaceNode);
		AppendChildToParent(pElement_HoleSideFaceNode, pElement_HoleFeatureNode);

		for (std::vector<Face*>::size_type j = 0; j != EachHole->BottomWallVector.size(); j++)//g
		{
			Face* EachSideFace = EachHole->BottomWallVector[j];//g

			int indexJ = j + 1;
			char* StringIndexJ = new char;
			sprintf(StringIndexJ, "%d", indexJ);
			NXString NXStringIndexJ = StringIndexJ;

			BSTR bstr_HoleSideFace;
			NXString iStringHoleSideFace = "下侧面." + NXStringIndexJ;//g

			bstr_HoleSideFace = ConvertToBSTR(iStringHoleSideFace);

			IXMLDOMElementPtr pElement_HoleSideFace;
			pDoc->createElement(bstr_HoleSideFace, &pElement_HoleSideFace);
			AppendChildToParent(pElement_HoleSideFace, pElement_HoleSideFaceNode);//jie for shang

			char* StringIndexSideFace = new char;
			sprintf(StringIndexSideFace, "%d", EachSideFace->Tag());

			BSTR bstr_HoleSideFaceTag;
			NXString iStringHoleSideFaceTag = StringIndexSideFace;

			bstr_HoleSideFaceTag = ConvertToBSTR(iStringHoleSideFaceTag);
			pElement_HoleSideFace->put_text(bstr_HoleSideFaceTag);
		}

		
		

		//孔底面
		BSTR bstr_HoleBottomFaceNode;
		NXString iStringHoleBottomFaceNode = "底面";

		bstr_HoleBottomFaceNode = ConvertToBSTR(iStringHoleBottomFaceNode);

		IXMLDOMElementPtr pElement_HoleBottomFaceNode;
		pDoc->createElement(bstr_HoleBottomFaceNode, &pElement_HoleBottomFaceNode);
		AppendChildToParent(pElement_HoleBottomFaceNode, pElement_HoleFeatureNode);

		for (std::vector<Face*>::size_type j = 0; j != EachHole->BottomFaceVector.size(); j++)
		{
			Face* EachBottomFace = EachHole->BottomFaceVector[j];

			int indexJ = j + 1;
			char* StringIndexJ = new char;
			sprintf(StringIndexJ, "%d", indexJ);
			NXString NXStringIndexJ = StringIndexJ;

			BSTR bstr_HoleBottomFace;
			NXString iStringHoleBottomFace = "底面." + NXStringIndexJ;

			bstr_HoleBottomFace = ConvertToBSTR(iStringHoleBottomFace);

			IXMLDOMElementPtr pElement_HoleBottomFace;
			pDoc->createElement(bstr_HoleBottomFace, &pElement_HoleBottomFace);
			AppendChildToParent(pElement_HoleBottomFace, pElement_HoleBottomFaceNode);

			char* StringIndexBottomFace = new char;
			sprintf(StringIndexBottomFace, "%d", EachBottomFace->Tag());

			BSTR bstr_HoleBottomFaceTag;
			NXString iStringHoleBottomFaceTag = StringIndexBottomFace;

			bstr_HoleBottomFaceTag = ConvertToBSTR(iStringHoleBottomFaceTag);
			pElement_HoleBottomFace->put_text(bstr_HoleBottomFaceTag);
		}
	}

	//凸台无倒角盲孔
	for (std::vector<BossNormalBlindHole*>::size_type i = 0; i != BossNormalBlindHoleSpyResult.size(); i++) //G
	{
		BossNormalBlindHole* EachHole = BossNormalBlindHoleSpyResult[i];//G

		int indexI = i + 1;
		char* StringIndexI = new char;
		sprintf(StringIndexI, "%d", indexI);
		NXString NXStringIndexI = StringIndexI;

		BSTR bstr_HoleFeatureNode;
		NXString iStringHoleFeatureNode = "凸台无倒角盲孔." + NXStringIndexI;//g

		bstr_HoleFeatureNode = ConvertToBSTR(iStringHoleFeatureNode);

		IXMLDOMElementPtr pElement_HoleFeatureNode;
		pDoc->createElement(bstr_HoleFeatureNode, &pElement_HoleFeatureNode);
		AppendChildToParent(pElement_HoleFeatureNode, pElement_Hole);

		//孔顶面
		BSTR bstr_HoleTopFaceNode;
		NXString iStringHoleTopFaceNode = "顶面";

		bstr_HoleTopFaceNode = ConvertToBSTR(iStringHoleTopFaceNode);

		IXMLDOMElementPtr pElement_HoleTopFaceNode;
		pDoc->createElement(bstr_HoleTopFaceNode, &pElement_HoleTopFaceNode);
		AppendChildToParent(pElement_HoleTopFaceNode, pElement_HoleFeatureNode);

		for (std::vector<Face*>::size_type j = 0; j != EachHole->TopFaceVector.size(); j++)
		{
			Face* EachTopFace = EachHole->TopFaceVector[j];

			int indexJ = j + 1;
			char* StringIndexJ = new char;
			sprintf(StringIndexJ, "%d", indexJ);
			NXString NXStringIndexJ = StringIndexJ;

			BSTR bstr_HoleTopFace;
			NXString iStringHoleTopFace = "顶面." + NXStringIndexJ;

			bstr_HoleTopFace = ConvertToBSTR(iStringHoleTopFace);

			IXMLDOMElementPtr pElement_HoleTopFace;
			pDoc->createElement(bstr_HoleTopFace, &pElement_HoleTopFace);
			AppendChildToParent(pElement_HoleTopFace, pElement_HoleTopFaceNode);

			char* StringIndexTopFace = new char;
			sprintf(StringIndexTopFace, "%d", EachTopFace->Tag());

			BSTR bstr_HoleTopFaceTag;
			NXString iStringHoleTopFaceTag = StringIndexTopFace;

			bstr_HoleTopFaceTag = ConvertToBSTR(iStringHoleTopFaceTag);
			pElement_HoleTopFace->put_text(bstr_HoleTopFaceTag);
		}
		//侧面
		BSTR bstr_BossTopFaceNode;
		NXString iStringBossTopFaceNode = "侧面";//g

		bstr_BossTopFaceNode = ConvertToBSTR(iStringBossTopFaceNode);

		IXMLDOMElementPtr pElement_BossTopFaceNode;
		pDoc->createElement(bstr_BossTopFaceNode, &pElement_BossTopFaceNode);
		AppendChildToParent(pElement_BossTopFaceNode, pElement_HoleFeatureNode);//g

		for (std::vector<Face*>::size_type j = 0; j != EachHole->InWallVector.size(); j++)//g
		{
			Face* EachTopFace = EachHole->InWallVector[j];//g

			int indexJ = j + 1;
			char* StringIndexJ = new char;
			sprintf(StringIndexJ, "%d", indexJ);
			NXString NXStringIndexJ = StringIndexJ;

			BSTR bstr_BossTopFace;
			NXString iStringBossTopFace = "侧面." + NXStringIndexJ;//g

			bstr_BossTopFace = ConvertToBSTR(iStringBossTopFace);

			IXMLDOMElementPtr pElement_BossTopFace;
			pDoc->createElement(bstr_BossTopFace, &pElement_BossTopFace);
			AppendChildToParent(pElement_BossTopFace, pElement_BossTopFaceNode);

			char* StringIndexTopFace = new char;
			sprintf(StringIndexTopFace, "%d", EachTopFace->Tag());

			BSTR bstr_BossTopFaceTag;
			NXString iStringBossTopFaceTag = StringIndexTopFace;

			bstr_BossTopFaceTag = ConvertToBSTR(iStringBossTopFaceTag);
			pElement_BossTopFace->put_text(bstr_BossTopFaceTag);
		}

		//倒圆角面
		/*BSTR bstr_HoleCornerFaceNode;
		NXString iStringHoleCornerFaceNode = "倒圆角面";

		bstr_HoleCornerFaceNode = ConvertToBSTR(iStringHoleCornerFaceNode);

		IXMLDOMElementPtr pElement_HoleCornerFaceNode;
		pDoc->createElement(bstr_HoleCornerFaceNode, &pElement_HoleCornerFaceNode);
		AppendChildToParent(pElement_HoleCornerFaceNode, pElement_HoleFeatureNode);

		for (std::vector<Face*>::size_type j = 0; j != EachHole->CircleVector.size(); j++)
		{
			Face* EachCornerFace = EachHole->CircleVector[j];

			int indexJ = j + 1;
			char* StringIndexJ = new char;
			sprintf(StringIndexJ, "%d", indexJ);
			NXString NXStringIndexJ = StringIndexJ;

			BSTR bstr_HoleCornerFace;
			NXString iStringHoleCornerFace = "倒圆角面." + NXStringIndexJ;

			bstr_HoleCornerFace = ConvertToBSTR(iStringHoleCornerFace);

			IXMLDOMElementPtr pElement_HoleCornerFace;
			pDoc->createElement(bstr_HoleCornerFace, &pElement_HoleCornerFace);
			AppendChildToParent(pElement_HoleCornerFace, pElement_HoleCornerFaceNode);

			char* StringIndexCornerFace = new char;
			sprintf(StringIndexCornerFace, "%d", EachCornerFace->Tag());

			BSTR bstr_HoleCornerFaceTag;
			NXString iStringHoleCornerFaceTag = StringIndexCornerFace;

			bstr_HoleCornerFaceTag = ConvertToBSTR(iStringHoleCornerFaceTag);
			pElement_HoleCornerFace->put_text(bstr_HoleCornerFaceTag);
		}*/


		//过渡平面
		/*BSTR bstr_BossSideFaceNode;
		NXString iStringBossSideFaceNode = "过渡平面";

		bstr_BossSideFaceNode = ConvertToBSTR(iStringBossSideFaceNode);

		IXMLDOMElementPtr pElement_BossSideFaceNode;
		pDoc->createElement(bstr_BossSideFaceNode, &pElement_BossSideFaceNode);
		AppendChildToParent(pElement_BossSideFaceNode, pElement_HoleFeatureNode);

		for (std::vector<Face*>::size_type j = 0; j != EachHole->PlanerVector.size(); j++)
		{
			Face* EachSideFace = EachHole->PlanerVector[j];

			int indexJ = j + 1;
			char* StringIndexJ = new char;
			sprintf(StringIndexJ, "%d", indexJ);
			NXString NXStringIndexJ = StringIndexJ;

			BSTR bstr_BossSideFace;
			NXString iStringBossSideFace = "过渡平面." + NXStringIndexJ;

			bstr_BossSideFace = ConvertToBSTR(iStringBossSideFace);

			IXMLDOMElementPtr pElement_BossSideFace;
			pDoc->createElement(bstr_BossSideFace, &pElement_BossSideFace);
			AppendChildToParent(pElement_BossSideFace, pElement_BossSideFaceNode);

			char* StringIndexSideFace = new char;
			sprintf(StringIndexSideFace, "%d", EachSideFace->Tag());

			BSTR bstr_BossSideFaceTag;
			NXString iStringBossSideFaceTag = StringIndexSideFace;

			bstr_BossSideFaceTag = ConvertToBSTR(iStringBossSideFaceTag);
			pElement_BossSideFace->put_text(bstr_BossSideFaceTag);
		}*/


		/*下侧面*/
		//BSTR bstr_HoleSideFaceNode;
		//NXString iStringHoleSideFaceNode = "下侧面";//g

		//bstr_HoleSideFaceNode = ConvertToBSTR(iStringHoleSideFaceNode);

		//IXMLDOMElementPtr pElement_HoleSideFaceNode;
		//pDoc->createElement(bstr_HoleSideFaceNode, &pElement_HoleSideFaceNode);
		//AppendChildToParent(pElement_HoleSideFaceNode, pElement_HoleFeatureNode);

		//for (std::vector<Face*>::size_type j = 0; j != EachHole->BottomWallVector.size(); j++)//g
		//{
		//	Face* EachSideFace = EachHole->BottomWallVector[j];//g

		//	int indexJ = j + 1;
		//	char* StringIndexJ = new char;
		//	sprintf(StringIndexJ, "%d", indexJ);
		//	NXString NXStringIndexJ = StringIndexJ;

		//	BSTR bstr_HoleSideFace;
		//	NXString iStringHoleSideFace = "下侧面." + NXStringIndexJ;//g

		//	bstr_HoleSideFace = ConvertToBSTR(iStringHoleSideFace);

		//	IXMLDOMElementPtr pElement_HoleSideFace;
		//	pDoc->createElement(bstr_HoleSideFace, &pElement_HoleSideFace);
		//	AppendChildToParent(pElement_HoleSideFace, pElement_HoleSideFaceNode);//jie for shang

		//	char* StringIndexSideFace = new char;
		//	sprintf(StringIndexSideFace, "%d", EachSideFace->Tag());

		//	BSTR bstr_HoleSideFaceTag;
		//	NXString iStringHoleSideFaceTag = StringIndexSideFace;

		//	bstr_HoleSideFaceTag = ConvertToBSTR(iStringHoleSideFaceTag);
		//	pElement_HoleSideFace->put_text(bstr_HoleSideFaceTag);
		//}




		//孔底面
		BSTR bstr_HoleBottomFaceNode;
		NXString iStringHoleBottomFaceNode = "底面";

		bstr_HoleBottomFaceNode = ConvertToBSTR(iStringHoleBottomFaceNode);

		IXMLDOMElementPtr pElement_HoleBottomFaceNode;
		pDoc->createElement(bstr_HoleBottomFaceNode, &pElement_HoleBottomFaceNode);
		AppendChildToParent(pElement_HoleBottomFaceNode, pElement_HoleFeatureNode);

		for (std::vector<Face*>::size_type j = 0; j != EachHole->BottomFaceVector.size(); j++)
		{
			Face* EachBottomFace = EachHole->BottomFaceVector[j];

			int indexJ = j + 1;
			char* StringIndexJ = new char;
			sprintf(StringIndexJ, "%d", indexJ);
			NXString NXStringIndexJ = StringIndexJ;

			BSTR bstr_HoleBottomFace;
			NXString iStringHoleBottomFace = "底面." + NXStringIndexJ;

			bstr_HoleBottomFace = ConvertToBSTR(iStringHoleBottomFace);

			IXMLDOMElementPtr pElement_HoleBottomFace;
			pDoc->createElement(bstr_HoleBottomFace, &pElement_HoleBottomFace);
			AppendChildToParent(pElement_HoleBottomFace, pElement_HoleBottomFaceNode);

			char* StringIndexBottomFace = new char;
			sprintf(StringIndexBottomFace, "%d", EachBottomFace->Tag());

			BSTR bstr_HoleBottomFaceTag;
			NXString iStringHoleBottomFaceTag = StringIndexBottomFace;

			bstr_HoleBottomFaceTag = ConvertToBSTR(iStringHoleBottomFaceTag);
			pElement_HoleBottomFace->put_text(bstr_HoleBottomFaceTag);
		}
	}

	//凸台倒斜角盲孔
	for (std::vector<BossRightBlindHole*>::size_type i = 0; i != BossRightBlindHoleSpyResult.size(); i++) //G
	{
		BossRightBlindHole* EachHole = BossRightBlindHoleSpyResult[i];//G

		int indexI = i + 1;
		char* StringIndexI = new char;
		sprintf(StringIndexI, "%d", indexI);
		NXString NXStringIndexI = StringIndexI;

		BSTR bstr_HoleFeatureNode;
		NXString iStringHoleFeatureNode = "凸台倒斜角盲孔." + NXStringIndexI;//g

		bstr_HoleFeatureNode = ConvertToBSTR(iStringHoleFeatureNode);

		IXMLDOMElementPtr pElement_HoleFeatureNode;
		pDoc->createElement(bstr_HoleFeatureNode, &pElement_HoleFeatureNode);
		AppendChildToParent(pElement_HoleFeatureNode, pElement_Hole);

		//孔顶面
		BSTR bstr_HoleTopFaceNode;
		NXString iStringHoleTopFaceNode = "顶面";

		bstr_HoleTopFaceNode = ConvertToBSTR(iStringHoleTopFaceNode);

		IXMLDOMElementPtr pElement_HoleTopFaceNode;
		pDoc->createElement(bstr_HoleTopFaceNode, &pElement_HoleTopFaceNode);
		AppendChildToParent(pElement_HoleTopFaceNode, pElement_HoleFeatureNode);

		for (std::vector<Face*>::size_type j = 0; j != EachHole->TopFaceVector.size(); j++)
		{
			Face* EachTopFace = EachHole->TopFaceVector[j];

			int indexJ = j + 1;
			char* StringIndexJ = new char;
			sprintf(StringIndexJ, "%d", indexJ);
			NXString NXStringIndexJ = StringIndexJ;

			BSTR bstr_HoleTopFace;
			NXString iStringHoleTopFace = "顶面." + NXStringIndexJ;

			bstr_HoleTopFace = ConvertToBSTR(iStringHoleTopFace);

			IXMLDOMElementPtr pElement_HoleTopFace;
			pDoc->createElement(bstr_HoleTopFace, &pElement_HoleTopFace);
			AppendChildToParent(pElement_HoleTopFace, pElement_HoleTopFaceNode);

			char* StringIndexTopFace = new char;
			sprintf(StringIndexTopFace, "%d", EachTopFace->Tag());

			BSTR bstr_HoleTopFaceTag;
			NXString iStringHoleTopFaceTag = StringIndexTopFace;

			bstr_HoleTopFaceTag = ConvertToBSTR(iStringHoleTopFaceTag);
			pElement_HoleTopFace->put_text(bstr_HoleTopFaceTag);
		}

		//倒斜角面
		BSTR bstr_HoleCornerFaceNode;
		NXString iStringHoleCornerFaceNode = "倒斜角面";

		bstr_HoleCornerFaceNode = ConvertToBSTR(iStringHoleCornerFaceNode);

		IXMLDOMElementPtr pElement_HoleCornerFaceNode;
		pDoc->createElement(bstr_HoleCornerFaceNode, &pElement_HoleCornerFaceNode);
		AppendChildToParent(pElement_HoleCornerFaceNode, pElement_HoleFeatureNode);

		for (std::vector<Face*>::size_type j = 0; j != EachHole->RightVector.size(); j++)
		{
			Face* EachCornerFace = EachHole->RightVector[j];

			int indexJ = j + 1;
			char* StringIndexJ = new char;
			sprintf(StringIndexJ, "%d", indexJ);
			NXString NXStringIndexJ = StringIndexJ;

			BSTR bstr_HoleCornerFace;
			NXString iStringHoleCornerFace = "倒斜角面." + NXStringIndexJ;

			bstr_HoleCornerFace = ConvertToBSTR(iStringHoleCornerFace);

			IXMLDOMElementPtr pElement_HoleCornerFace;
			pDoc->createElement(bstr_HoleCornerFace, &pElement_HoleCornerFace);
			AppendChildToParent(pElement_HoleCornerFace, pElement_HoleCornerFaceNode);

			char* StringIndexCornerFace = new char;
			sprintf(StringIndexCornerFace, "%d", EachCornerFace->Tag());

			BSTR bstr_HoleCornerFaceTag;
			NXString iStringHoleCornerFaceTag = StringIndexCornerFace;

			bstr_HoleCornerFaceTag = ConvertToBSTR(iStringHoleCornerFaceTag);
			pElement_HoleCornerFace->put_text(bstr_HoleCornerFaceTag);
		}


		//侧面
		BSTR bstr_BossTopFaceNode;
		NXString iStringBossTopFaceNode = "侧面";//g

		bstr_BossTopFaceNode = ConvertToBSTR(iStringBossTopFaceNode);

		IXMLDOMElementPtr pElement_BossTopFaceNode;
		pDoc->createElement(bstr_BossTopFaceNode, &pElement_BossTopFaceNode);
		AppendChildToParent(pElement_BossTopFaceNode, pElement_HoleFeatureNode);//g

		for (std::vector<Face*>::size_type j = 0; j != EachHole->InWallVector.size(); j++)//g
		{
			Face* EachTopFace = EachHole->InWallVector[j];//g

			int indexJ = j + 1;
			char* StringIndexJ = new char;
			sprintf(StringIndexJ, "%d", indexJ);
			NXString NXStringIndexJ = StringIndexJ;

			BSTR bstr_BossTopFace;
			NXString iStringBossTopFace = "侧面." + NXStringIndexJ;//g

			bstr_BossTopFace = ConvertToBSTR(iStringBossTopFace);

			IXMLDOMElementPtr pElement_BossTopFace;
			pDoc->createElement(bstr_BossTopFace, &pElement_BossTopFace);
			AppendChildToParent(pElement_BossTopFace, pElement_BossTopFaceNode);

			char* StringIndexTopFace = new char;
			sprintf(StringIndexTopFace, "%d", EachTopFace->Tag());

			BSTR bstr_BossTopFaceTag;
			NXString iStringBossTopFaceTag = StringIndexTopFace;

			bstr_BossTopFaceTag = ConvertToBSTR(iStringBossTopFaceTag);
			pElement_BossTopFace->put_text(bstr_BossTopFaceTag);
		}

		


		//过渡平面
		/*BSTR bstr_BossSideFaceNode;
		NXString iStringBossSideFaceNode = "过渡平面";

		bstr_BossSideFaceNode = ConvertToBSTR(iStringBossSideFaceNode);

		IXMLDOMElementPtr pElement_BossSideFaceNode;
		pDoc->createElement(bstr_BossSideFaceNode, &pElement_BossSideFaceNode);
		AppendChildToParent(pElement_BossSideFaceNode, pElement_HoleFeatureNode);

		for (std::vector<Face*>::size_type j = 0; j != EachHole->PlanerVector.size(); j++)
		{
			Face* EachSideFace = EachHole->PlanerVector[j];

			int indexJ = j + 1;
			char* StringIndexJ = new char;
			sprintf(StringIndexJ, "%d", indexJ);
			NXString NXStringIndexJ = StringIndexJ;

			BSTR bstr_BossSideFace;
			NXString iStringBossSideFace = "过渡平面." + NXStringIndexJ;

			bstr_BossSideFace = ConvertToBSTR(iStringBossSideFace);

			IXMLDOMElementPtr pElement_BossSideFace;
			pDoc->createElement(bstr_BossSideFace, &pElement_BossSideFace);
			AppendChildToParent(pElement_BossSideFace, pElement_BossSideFaceNode);

			char* StringIndexSideFace = new char;
			sprintf(StringIndexSideFace, "%d", EachSideFace->Tag());

			BSTR bstr_BossSideFaceTag;
			NXString iStringBossSideFaceTag = StringIndexSideFace;

			bstr_BossSideFaceTag = ConvertToBSTR(iStringBossSideFaceTag);
			pElement_BossSideFace->put_text(bstr_BossSideFaceTag);
		}*/


		/*下侧面*/
		//BSTR bstr_HoleSideFaceNode;
		//NXString iStringHoleSideFaceNode = "下侧面";//g

		//bstr_HoleSideFaceNode = ConvertToBSTR(iStringHoleSideFaceNode);

		//IXMLDOMElementPtr pElement_HoleSideFaceNode;
		//pDoc->createElement(bstr_HoleSideFaceNode, &pElement_HoleSideFaceNode);
		//AppendChildToParent(pElement_HoleSideFaceNode, pElement_HoleFeatureNode);

		//for (std::vector<Face*>::size_type j = 0; j != EachHole->BottomWallVector.size(); j++)//g
		//{
		//	Face* EachSideFace = EachHole->BottomWallVector[j];//g

		//	int indexJ = j + 1;
		//	char* StringIndexJ = new char;
		//	sprintf(StringIndexJ, "%d", indexJ);
		//	NXString NXStringIndexJ = StringIndexJ;

		//	BSTR bstr_HoleSideFace;
		//	NXString iStringHoleSideFace = "下侧面." + NXStringIndexJ;//g

		//	bstr_HoleSideFace = ConvertToBSTR(iStringHoleSideFace);

		//	IXMLDOMElementPtr pElement_HoleSideFace;
		//	pDoc->createElement(bstr_HoleSideFace, &pElement_HoleSideFace);
		//	AppendChildToParent(pElement_HoleSideFace, pElement_HoleSideFaceNode);//jie for shang

		//	char* StringIndexSideFace = new char;
		//	sprintf(StringIndexSideFace, "%d", EachSideFace->Tag());

		//	BSTR bstr_HoleSideFaceTag;
		//	NXString iStringHoleSideFaceTag = StringIndexSideFace;

		//	bstr_HoleSideFaceTag = ConvertToBSTR(iStringHoleSideFaceTag);
		//	pElement_HoleSideFace->put_text(bstr_HoleSideFaceTag);
		//}




		//孔底面
		BSTR bstr_HoleBottomFaceNode;
		NXString iStringHoleBottomFaceNode = "底面";

		bstr_HoleBottomFaceNode = ConvertToBSTR(iStringHoleBottomFaceNode);

		IXMLDOMElementPtr pElement_HoleBottomFaceNode;
		pDoc->createElement(bstr_HoleBottomFaceNode, &pElement_HoleBottomFaceNode);
		AppendChildToParent(pElement_HoleBottomFaceNode, pElement_HoleFeatureNode);

		for (std::vector<Face*>::size_type j = 0; j != EachHole->BottomFaceVector.size(); j++)
		{
			Face* EachBottomFace = EachHole->BottomFaceVector[j];

			int indexJ = j + 1;
			char* StringIndexJ = new char;
			sprintf(StringIndexJ, "%d", indexJ);
			NXString NXStringIndexJ = StringIndexJ;

			BSTR bstr_HoleBottomFace;
			NXString iStringHoleBottomFace = "底面." + NXStringIndexJ;

			bstr_HoleBottomFace = ConvertToBSTR(iStringHoleBottomFace);

			IXMLDOMElementPtr pElement_HoleBottomFace;
			pDoc->createElement(bstr_HoleBottomFace, &pElement_HoleBottomFace);
			AppendChildToParent(pElement_HoleBottomFace, pElement_HoleBottomFaceNode);

			char* StringIndexBottomFace = new char;
			sprintf(StringIndexBottomFace, "%d", EachBottomFace->Tag());

			BSTR bstr_HoleBottomFaceTag;
			NXString iStringHoleBottomFaceTag = StringIndexBottomFace;

			bstr_HoleBottomFaceTag = ConvertToBSTR(iStringHoleBottomFaceTag);
			pElement_HoleBottomFace->put_text(bstr_HoleBottomFaceTag);
		}
	}

	////残余特征结点//筋结点
	//NXString iStringResidue = "残余特征";
	//bstr = ConvertToBSTR(iStringResidue);

	//IXMLDOMElementPtr pElement_Residue;
	//pDoc->createElement(bstr, &pElement_Residue);
	//AppendChildToParent(pElement_Residue, pDocElement);

	////残余特征输出
	//for (std::vector<Residue*>::size_type i = 0; i != ResidueSpyResult.size(); i++)
	//{
	//	Residue* EachResidue = ResidueSpyResult[i];

	//	int indexI = i + 1;
	//	char* StringIndexI = new char;
	//	sprintf(StringIndexI, "%d", indexI);
	//	NXString NXStringIndexI = StringIndexI;

	//	BSTR bstr_ResidueFeatureNode;
	//	NXString iStringResidueFeatureNode = "残余特征." + NXStringIndexI;

	//	bstr_ResidueFeatureNode = ConvertToBSTR(iStringResidueFeatureNode);

	//	IXMLDOMElementPtr pElement_ResidueFeatureNode;
	//	pDoc->createElement(bstr_ResidueFeatureNode, &pElement_ResidueFeatureNode);
	//	AppendChildToParent(pElement_ResidueFeatureNode, pElement_Residue);

	//	//种子面A
	//	BSTR bstr_ResidueASeedFaceNode;
	//	NXString iStringResidueASeedFaceNode = "种子面A";

	//	bstr_ResidueASeedFaceNode = ConvertToBSTR(iStringResidueASeedFaceNode);

	//	IXMLDOMElementPtr pElement_ResidueASeedFaceNode;
	//	pDoc->createElement(bstr_ResidueASeedFaceNode, &pElement_ResidueASeedFaceNode);
	//	AppendChildToParent(pElement_ResidueASeedFaceNode, pElement_ResidueFeatureNode);

	//	Face* EachASeedFace = EachResidue->ASeedFace;

	//	BSTR bstr_ResidueASeedFace;
	//	NXString iStringResidueASeedFace = "种子面A" ;

	//	bstr_ResidueASeedFace = ConvertToBSTR(iStringResidueASeedFace);

	//	IXMLDOMElementPtr pElement_ResidueASeedFace;
	//	pDoc->createElement(bstr_ResidueASeedFace, &pElement_ResidueASeedFace);
	//	AppendChildToParent(pElement_ResidueASeedFace, pElement_ResidueASeedFaceNode);

	//	char* StringIndexASeedFace = new char;
	//	sprintf(StringIndexASeedFace, "%d", EachASeedFace->Tag());

	//	BSTR bstr_ASeedFaceTag;
	//	NXString iStringASeedFaceTag = StringIndexASeedFace;

	//	bstr_ASeedFaceTag = ConvertToBSTR(iStringASeedFaceTag);
	//	pElement_ResidueASeedFace->put_text(bstr_ASeedFaceTag);
	//	

	//	//种子面B
	//	BSTR bstr_ResidueBSeedFaceNode;
	//	NXString iStringResidueBSeedFaceNode = "种子面B";

	//	bstr_ResidueBSeedFaceNode = ConvertToBSTR(iStringResidueBSeedFaceNode);

	//	IXMLDOMElementPtr pElement_ResidueBSeedFaceNode;
	//	pDoc->createElement(bstr_ResidueBSeedFaceNode, &pElement_ResidueBSeedFaceNode);
	//	AppendChildToParent(pElement_ResidueBSeedFaceNode, pElement_ResidueFeatureNode);

	//	Face* EachBSeedFace = EachResidue->BSeedFace;

	//	BSTR bstr_ResidueBSeedFace;
	//	NXString iStringResidueBSeedFace = "种子面B" ;

	//	bstr_ResidueBSeedFace = ConvertToBSTR(iStringResidueBSeedFace);

	//	IXMLDOMElementPtr pElement_ResidueBSeedFace;
	//	pDoc->createElement(bstr_ResidueBSeedFace, &pElement_ResidueBSeedFace);
	//	AppendChildToParent(pElement_ResidueBSeedFace, pElement_ResidueBSeedFaceNode);

	//	char* StringIndexBSeedFace = new char;
	//	sprintf(StringIndexBSeedFace, "%d", EachBSeedFace->Tag());

	//	BSTR bstr_BSeedFaceTag;
	//	NXString iStringBSeedFaceTag = StringIndexBSeedFace;

	//	bstr_BSeedFaceTag = ConvertToBSTR(iStringBSeedFaceTag);
	//	pElement_ResidueBSeedFace->put_text(bstr_BSeedFaceTag);

	//	//侧面A
	//	BSTR bstr_ResidueASideFaceNode;
	//	NXString iStringResidueASideFaceNode = "侧面A";

	//	bstr_ResidueASideFaceNode = ConvertToBSTR(iStringResidueASideFaceNode);

	//	IXMLDOMElementPtr pElement_ResidueASideFaceNode;
	//	pDoc->createElement(bstr_ResidueASideFaceNode, &pElement_ResidueASideFaceNode);
	//	AppendChildToParent(pElement_ResidueASideFaceNode, pElement_ResidueFeatureNode);

	//	for (std::vector<Face*>::size_type j=0; j!=EachResidue->AFaceVector.size(); j++)
	//	{
	//		Face* EachASideFace = EachResidue->AFaceVector[j];

	//		int indexJ = j + 1;
	//		char* StringIndexJ = new char;
	//		sprintf(StringIndexJ, "%d", indexJ);
	//		NXString NXStringIndexJ = StringIndexJ;

	//		BSTR bstr_ResidueASideFace;
	//		NXString iStringResidueASideFace = "侧面A." + NXStringIndexJ;

	//		bstr_ResidueASideFace = ConvertToBSTR(iStringResidueASideFace);

	//		IXMLDOMElementPtr pElement_ResidueASideFace ;
	//		pDoc->createElement(bstr_ResidueASideFace, &pElement_ResidueASideFace);
	//		AppendChildToParent(pElement_ResidueASideFace, pElement_ResidueASideFaceNode);

	//		char* StringIndexResidueASideFace = new char;
	//		sprintf(StringIndexResidueASideFace, "%d", EachASideFace->Tag());

	//		BSTR bstr_ResidueASideFaceTag;
	//		NXString iStringResidueASideFaceTag = StringIndexResidueASideFace;

	//		bstr_ResidueASideFaceTag = ConvertToBSTR(iStringResidueASideFaceTag);
	//		pElement_ResidueASideFace->put_text(bstr_ResidueASideFaceTag);
	//	}

	//	//侧面B
	//	BSTR bstr_ResidueBSideFaceNode;
	//	NXString iStringResidueBSideFaceNode = "侧面B";

	//	bstr_ResidueBSideFaceNode = ConvertToBSTR(iStringResidueBSideFaceNode);

	//	IXMLDOMElementPtr pElement_ResidueBSideFaceNode;
	//	pDoc->createElement(bstr_ResidueBSideFaceNode, &pElement_ResidueBSideFaceNode);
	//	AppendChildToParent(pElement_ResidueBSideFaceNode, pElement_ResidueFeatureNode);

	//	for (std::vector<Face*>::size_type j=0; j!=EachResidue->BFaceVector.size(); j++)
	//	{
	//		Face* EachBSideFace = EachResidue->BFaceVector[j];

	//		int indexJ = j + 1;
	//		char* StringIndexJ = new char;
	//		sprintf(StringIndexJ, "%d", indexJ);
	//		NXString NXStringIndexJ = StringIndexJ;

	//		BSTR bstr_ResidueBSideFace;
	//		NXString iStringResidueBSideFace = "侧面B." + NXStringIndexJ;

	//		bstr_ResidueBSideFace = ConvertToBSTR(iStringResidueBSideFace);

	//		IXMLDOMElementPtr pElement_ResidueBSideFace ;
	//		pDoc->createElement(bstr_ResidueBSideFace, &pElement_ResidueBSideFace);
	//		AppendChildToParent(pElement_ResidueBSideFace, pElement_ResidueBSideFaceNode);

	//		char* StringIndexResidueBSideFace = new char;
	//		sprintf(StringIndexResidueBSideFace, "%d", EachBSideFace->Tag());

	//		BSTR bstr_ResidueBSideFaceTag;
	//		NXString iStringResidueBSideFaceTag = StringIndexResidueBSideFace;

	//		bstr_ResidueBSideFaceTag = ConvertToBSTR(iStringResidueBSideFaceTag);
	//		pElement_ResidueBSideFace->put_text(bstr_ResidueBSideFaceTag);
	//	}
	//}


	//法矢量
	NXString iStringRotate = "旋转矢量";
	bstr = ConvertToBSTR(iStringRotate);

	IXMLDOMElementPtr pElement_Rotate;
	pDoc->createElement(bstr, &pElement_Rotate);
	AppendChildToParent(pElement_Rotate, pDocElement);

	char buffer[200];
	std::string bufferString;
	sprintf_s(buffer," %f", _Approach_Normal.X);
	bufferString = buffer;
	NXString X_NXString = (NXString)bufferString;
	sprintf_s(buffer," %f", _Approach_Normal.Y);
	bufferString = buffer;
	NXString Y_NXString = (NXString)bufferString;
	sprintf_s(buffer," %f", _Approach_Normal.Z);
	bufferString = buffer;
	NXString Z_NXString = (NXString)bufferString;

	NXString NXString_Normal = X_NXString + "," + Y_NXString + "," + Z_NXString;
	BSTR bstr_Normal;
	bstr_Normal = ConvertToBSTR(NXString_Normal);
	pElement_Rotate->put_text(bstr_Normal);

	////----X
	//BSTR bstrX;
	//NXString iStringX = "X";
	//bstrX = ConvertToBSTR(iStringX);
	//IXMLDOMElementPtr pElementX ;
	//pDoc->createElement(bstrX, &pElementX);
	//AppendChildToParent(pElementX, pElement_Rotate);

	//char buffer[200];
	//sprintf_s(buffer," %f", _Approach_Normal.X);
	//std::string Xbb = buffer;
	//NXString X_NXString = (NXString)Xbb;
	//BSTR bstr_Xvalue;
	//bstr_Xvalue = ConvertToBSTR(X_NXString);
	//pElementX->put_text(bstr_Xvalue);   


}
void xml::SaveXML(IXMLDOMDocumentPtr pDoc, NXString ifileName)
{
	BSTR FileName = ConvertToBSTR(ifileName);

	//BSTR FileName = ifileName.AllocSysString();

	//const char* ww;
	//WCHAR* pp(ww);
	VARIANT var;
	VariantInit(&var);
	V_BSTR(&var) = SysAllocString(FileName);
	V_VT(&var) = VT_BSTR;
	pDoc->save(var);
	CoUninitialize();

	//Show cue message
	NXString information = "XML File save in " + ifileName + " succeed!!";
	std::string cuemessage(information.GetUTF8Text());
	char* cuemess = new char[cuemessage.length()+1];
	strcpy(cuemess,cuemessage.c_str());
	uc1601(cuemess,1);
}

void xml::AppendChildToParent(IXMLDOMNode *pChild, IXMLDOMNode *pParent)
{
	IXMLDOMNode *pNode=NULL;
	pParent->appendChild(pChild, &pNode);

	if (pNode) pNode->Release();
	pNode=NULL;
}

BSTR xml::ConvertToBSTR(NXString iString)
{
	char* istr = (char*)iString.GetText();
	BSTR bstr = _com_util::ConvertStringToBSTR(istr);
	return bstr;
}