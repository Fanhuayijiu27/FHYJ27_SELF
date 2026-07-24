#undef CreateDialog
#ifndef XML_H
#define XML_H

#include "comutil.h"           //对COM组件进行初始化
#include "comdef.h"

#include <uf_defs.h>
#include <uf_ui.h>
#include <NXOpen/TaggedObject.hxx>
#include <NXOpen/Face.hxx>

#include "FeaturePublicClass.h"

#include <vector>
#include <cstdio>

using namespace std;
using namespace NXOpen;


class xml: public TaggedObject
{
public:
	// Standard constructors and destructors
	// -------------------------------------
	xml ();
	virtual ~xml ();

	// Copy constructor and equal operator
	// -----------------------------------
	xml (xml &);
	xml& operator=(xml&);

	//std::vector<CircularGroove*> CircularGroove0SpyResult, std::vector<CircularGroove*> CircularGroove1Result,
	//std::vector<SubsidenceGroove*> SubsidenceGrooveyResult,std::vector<PlanerBlindGroove*> PlanerBlindGrooveResult,std::vector<PassGroove*> PassGrooveResult,

public:
	std::vector<CircularGroove*> CircularGroove0SpyResult;
	std::vector<CircularGroove*> CircularGroove1SpyResult;
	std::vector<SubsidenceGroove*> SubsidenceGrooveSpyResult;
	std::vector<PlanerBlindGroove*> PlanerBlindGrooveSpyResult;
	std::vector<PassGroove*> PassGrooveSpyResult;
	/*std::vector<Boss*> BossSpyResult;*/
	std::vector<DoubleLayerBoss*> DoubleLayerBossSpyResult;
	std::vector<NormalBoss*> NormalBossSpyResult;
	std::vector<NoSideBoss*> NoSideBossSpyResult;
//-------------------------hole-----------------------------------//
	std::vector<DirectionNormalHole*> DirectionNormalHoleSpyResult;
	std::vector<DirectionRightHole*> DirectionRightHoleSpyResult;
	std::vector<DirectionBlindHole*> DirectionBlindHoleSpyResult;
	std::vector<WallNormalHole*> WallNormalHoleSpyResult;
	std::vector<WallCircleCombineHole*> WallCircleCombineHoleSpyResult;
	std::vector<RibCombineHole*> RibCombineHoleSpyResult;
	std::vector<BossNormalHole*> BossNormalHoleSpyResult;
	std::vector<BossOneRightHole*> BossOneRightHoleSpyResult;
	std::vector<BossOneCircleHole*> BossOneCircleHoleSpyResult;
	std::vector<BossDoubleRightHole*> BossDoubleRightHoleSpyResult;
	std::vector<BossDoubleCircleHole*> BossDoubleCircleHoleSpyResult;
	std::vector<BossOneRightOneCircleHole*> BossOneRightOneCircleHoleSpyResult;
	std::vector<BossManyLayerHole*> BossManyLayerHoleSpyResult;
	std::vector<BossOutsideWideCombineHole*> BossOutsideWideCombineHoleSpyResult;
	std::vector<BossInsideWideCombineHole*> BossInsideWideCombineHoleSpyResult;
	std::vector<BossMidCricleCombineHole*> BossMidCricleCombineHoleSpyResult;
	std::vector<BossNormalBlindHole*> BossNormalBlindHoleSpyResult;
	std::vector<BossRightBlindHole*> BossRightBlindHoleSpyResult;
	std::vector<Other3Hole*> otherHole3SpyResult;
//-------------------------hole-----------------------------------//
//-------------------------Rib------------------------------------//
	std::vector<DirectionRib*> DirectionRibSpyResult;
	std::vector<HookFaceRib*> HookFaceRibSpyResult;
//-------------------------outline------------------------------------//
	std::vector<Outline*> OutlineSpyResult;
	std::vector<Outline*> Outline1SpyResult;


	/*std::vector<Rib*> RibSpyResult;
	std::vector<Hole*> HoleSpyResult;
	std::vector<Residue*> ResidueSpyResult*/;

	NXOpen::Vector3d _Approach_Normal;
	//Feature_CircularGroove0, Feature_CircularGroove1, Feature_SubsidenceGroove,Feature_PlanerBlindGroove,Feature_PassGroove,
	void initial(std::vector<CircularGroove*> CircularGroove0Result, std::vector<CircularGroove*> CircularGroove1Result,
		std::vector<SubsidenceGroove*> SubsidenceGrooveResult,std::vector<PlanerBlindGroove*> PlanerBlindGrooveResult,std::vector<PassGroove*> PassGrooveResult,

		std::vector<DoubleLayerBoss*> DoubleLayerBossResult, std::vector<NormalBoss*> NormalBossResult,std::vector<NoSideBoss*> NoSideBossResult,

		std::vector<DirectionNormalHole*> DirectionNormalHoleResult,std::vector<DirectionRightHole*> DirectionRightHoleResult,std::vector<DirectionBlindHole*> DirectionBlindHoleResult,
		std::vector<WallNormalHole*> WallNormalHoleResult,std::vector<WallCircleCombineHole*> WallCircleCombineHoleResult,std::vector<RibCombineHole*> RibCombineHoleResult,
		std::vector<BossNormalHole*> BossNormalHoleResult,std::vector<BossOneRightHole*> BossOneRightHoleResult,std::vector<BossOneCircleHole*> BossOneCircleHoleResult,
		std::vector<BossDoubleRightHole*> BossDoubleRightHoleResult,std::vector<BossDoubleCircleHole*> BossDoubleCircleHoleResult,std::vector<BossOneRightOneCircleHole*> BossOneRightOneCircleHoleResult,
		std::vector<BossManyLayerHole*> BossManyLayerHoleResult,std::vector<BossOutsideWideCombineHole*> BossOutsideWideCombineHoleResult,std::vector<BossInsideWideCombineHole*> BossInsideWideCombineHoleResult,
		std::vector<BossMidCricleCombineHole*> BossMidCricleCombineHoleResult, std::vector<BossNormalBlindHole*> BossNormalBlindHoleResult, std::vector<BossRightBlindHole*> BossRightBlindHoleResult, std::vector<Other3Hole*> otherHole3Result,
		std::vector<DirectionRib*> DirectionRibResult, std::vector<HookFaceRib*> HookFaceRibResult,
		std::vector<Outline*> OutlineResult,std::vector<Outline*> Outline1Result,
		
		NXOpen::Vector3d _Normal);

	void CreateXml_for_CAM(IXMLDOMDocumentPtr &pDoc);
	void SaveXML(IXMLDOMDocumentPtr pDoc, NXString ifileName);

	void AppendChildToParent(IXMLDOMNode *pChild, IXMLDOMNode *pParent);
	BSTR ConvertToBSTR(NXString iString);
};

#endif