#ifndef __BNETCB_HPP__
#define __BNETCB_HPP__

#include <string>
#include "pnlHighConf.hpp"
#include "NetCallBack.hpp"

class PNLHIGH_API BayesNetCallback: public NetCallback
{
public:
    BayesNetCallback() {}

    virtual pnl::CGraphicalModel *CreateModel(ProbabilisticNet &net);

    virtual void GenerateSamples(ProbabilisticNet &net,
	pnl::pEvidencesVector *newSamples, int nSample, pnl::CEvidence *evid);
};

#endif //__BNETCB_HPP__