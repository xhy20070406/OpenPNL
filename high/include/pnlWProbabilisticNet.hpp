#ifndef __PNLWPROBABILISTICNET_HPP__
#define __PNLWPROBABILISTICNET_HPP__

#include <string>
#include "pnlHighConf.hpp"
#include "pnlTok.hpp"
#include "pnlWEvidence.hpp"

// FORWARDS
class WDistribFun;
class WGraph;
class TokenCover;
class WDistributions;
class WEvidence;
class NetCallback;

namespace pnl
{
    class CPNLBase;
    class CEvidence;
    class CGraphicalModel;
    class CInfEngine;
    class CBayesLearningEngine;
    class pnlString;
    template<typename Type> class CMatrix;
}

class PNLHIGH_API ProbabilisticNet: public pnl::CPNLBase
{
public:
    typedef Vector<int> IIMap;

    ProbabilisticNet();
    virtual ~ProbabilisticNet();

    void SetCallback(NetCallback *pCallback) { m_pCallback = pCallback; }
    void AddNode(TokArr nodes, TokArr subnodes);
    void DelNode(TokArr nodes);
    TokArr NodeType(TokArr nodes);
    void AddArc(TokArr from, TokArr to);
    void DelArc(TokArr from, TokArr to);
    void Evid(TokArr values = TokArr(), bool bPush = false);
    void PushEvid(TokArr const values[], int nValue);
    void ClearEvid();
    void ClearEvidHistory();

    int SaveLearnBuf(const char *filename, NetConst::ESavingType mode);
    int LoadLearnBuf(const char *filename, NetConst::ESavingType mode, TokArr columns);
    // other variants same as after SaveLearnBuf
    //add several evidences to learning buffer
    //nSample - number of evidences to generate
    //whatNodes - which nodes should be included. By default - all
    //ignoreCurrEvid - if 'false', then current evidence specifies some fixed values
    //                 if 'true',then no fixed values while generating evidences
    void GenerateEvidences( int nSample, bool ignoreCurrEvid = false, TokArr whatNodes = "");
    
    //This function hides nodes of current learning buffer with given probability
    //The function applies for all existing values (hidden and observed), 
    //i.e. observed may become hidden and hidden may become observed
    //By default this function unhides all hidden values 
    //If a node did not have sample then it can not be unhidden
    void MaskEvidences( TokArr whatNodes = "");

    void SetProperty(const char *name, const char *value);
    String Property(const char *name) const;

public:
    pnl::CEvidence *CreateEvidence(TokArr &aValue);
    int nNetNode() const;
    void MustBeNode(TokArr &nodes) const;
    bool IsNode(Tok &node) const;
    WEvidence *EvidenceBoard() { return &m_EvidenceBoard; }
    Vector<pnl::CEvidence *> *EvidenceBuf() { return &m_aEvidence; }

public:// Bayes node name (or TokIdNode) <-> index
    String NodeName(int iNode) const;
    int NodeIndex(const char *name) const;
    int NodeIndex(TokIdNode *name) const;
    TokIdNode *TokNodeByIndex(int i) const;
    String DiscreteValue(int iNode, int value) const;
    void ExtractTokArr(TokArr &aNode, Vector<int> *paiNode,
	Vector<int> *paiValue, IIMap *pMap = 0) const;
    static int GetInt(TokIdNode *node);

public:
    pnl::CNodeType pnlNodeType(int i);
    void Accumulate(TokArr *pResult, Vector<int> &aIndex,
	pnl::CMatrix<float> *mat, String &prtName, int prtValue) const;
    void SplitNodesByObservityFlag(Vector<int> *aiObserved, Vector<int> *aiUnobserved);
    TokArr CutReq( Vector<int>& queryNds, Vector<int>& queryVls, 
			const pnl::CMatrix<float> * mat ) const;

    int NodesClassification(TokArr &aValue) const;

public:// inlines for access to object fields
    WGraph *Graph() const { return m_pGraph; }
    TokenCover *Token() const { return m_pTokenCov; }
    WDistributions *Distributions() const { return m_paDistribution; }
    pnl::CGraphicalModel *Model();
    void SetModel(pnl::CGraphicalModel* pModel);

private:// DATA members
    // Tree for bnet:
    //
    //		   / categoric
    // bnet - nodes
    //		   \ continuous
    pnl::CGraphicalModel *m_Model;	    // model, if it exists
    WEvidence m_EvidenceBoard;		    // board for evidence (see diagram for evidence buffer)
    Vector<pnl::CEvidence *> m_aEvidence;   // buffer for evidences
    WDistributions *m_paDistribution;    // It holds all distributions
					    // It is moreover alters distribution as need
    TokenCover *m_pTokenCov;		    // token stuff
    WGraph *m_pGraph;			    // Stores graph and names of nodes

    typedef std::map<String, String> SSMap; // type for mapping string to string
    SSMap m_aPropertyValue;		    // Properties: value for every property

    NetCallback *m_pCallback;
};

#endif //__PNLWPROBABILISTICNET_HPP__