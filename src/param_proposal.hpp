//
//  param_proposal.h
//  spf
//
//  Created by Seong-Hwan Jun on 2018-06-15.
//  Copyright © 2018 Seong-Hwan Jun. All rights reserved.
//

#ifndef param_proposal_h
#define param_proposal_h

#include <gsl/gsl_rng.h>

template <class P>
class ParamProposal
{
public:
    virtual P *sample_from_prior(gsl_rng *random) = 0;
    virtual P *propose(gsl_rng *random, P *curr) = 0;
    virtual double log_proposal(P *curr, P *prev) = 0; // log p(curr | prev)
    virtual double log_prior(P *curr) = 0; // log p(curr)
    virtual void adapt(size_t num_accepts, size_t curr_iter) = 0;
};

#endif /* param_proposal_h */
