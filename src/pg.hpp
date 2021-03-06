//
//  pg.hpp
//  spf-lib
//
//  Created by Seong-Hwan Jun on 2018-12-04.
//

#ifndef pg_h
#define pg_h

#include <cmath>
#include <vector>

#include <gsl/gsl_randist.h>
#include <gsl/gsl_rng.h>

#include "pg_proposal.hpp"
#include "pmcmc_options.hpp"
#include "csmc.hpp"

using namespace std;

template <class S, class P> class ParticleGibbs
{
    PMCMCOptions &options;
    ConditionalSMC<S, P> &csmc;
    PGProposal<S, P> &param_proposal;
    vector<shared_ptr<P> > parameters;
    vector<double > log_marginal_likelihoods;
    vector<shared_ptr<ParticleGenealogy<S> > > states; // store the genealogy along with the log weights

public:
    ParticleGibbs(PMCMCOptions &options, ConditionalSMC<S, P> &smc, PGProposal<S, P> &param_proposal);
    void run();
    vector<double > &get_log_marginal_likelihoods();
    vector<shared_ptr<P> > &get_parameters();
    vector<shared_ptr<ParticleGenealogy<S> > > &get_states();
    ~ParticleGibbs();
};

template <class S, class P>
ParticleGibbs<S,P>::ParticleGibbs(PMCMCOptions &options, ConditionalSMC<S, P> &csmc, PGProposal<S, P> &param_proposal) :
options(options), csmc(csmc), param_proposal(param_proposal)
{
}

template <class S, class P>
void ParticleGibbs<S,P>::run()
{
    // initialize the parameters
    shared_ptr<P> param = param_proposal.sample_from_prior(options.random);
    // initialize state
    shared_ptr<ParticleGenealogy<S> > genealogy;

    double log_Z = 0.0;
    double max_log_z = DOUBLE_NEG_INF;
    for (size_t i = 0; i < options.num_iterations; i++)
    {
        cout << "PG Iter: " << (i+1) << endl;
        if (i == 0) {
            genealogy = csmc.initialize(*param);
        } else {
            // run cSMC
            genealogy = csmc.run_csmc(*param, genealogy);
        }
        log_Z = csmc.get_log_marginal_likelihood();
        log_marginal_likelihoods.push_back(log_Z);
        if (options.verbose)
            cout << "logZ: " << log_Z << endl;

        if (log_Z > max_log_z) {
            if (options.verbose)
                cout << "new maximum log likelihood is found!" << endl;
            max_log_z = log_Z;
        }

        // propose new param: proposal can be quite general
        // as it can combine Gibbs sampling and form MH within MH
        // but it is always accepted
        param = param_proposal.propose(options.random, *param.get(), genealogy);

        parameters.push_back(param);
        states.push_back(genealogy);
    }
}

template <class S, class P>
vector<double > &ParticleGibbs<S,P>::get_log_marginal_likelihoods()
{
    return log_marginal_likelihoods;
}

template <class S, class P>
vector<shared_ptr<P> > &ParticleGibbs<S,P>::get_parameters()
{
    return parameters;
}

template <class S, class P>
vector<shared_ptr<ParticleGenealogy<S> > > &ParticleGibbs<S,P>::get_states()
{
    return states;
}

template <class S, class P>
ParticleGibbs<S,P>::~ParticleGibbs()
{
}

#endif /* pg_h */
