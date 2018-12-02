//
//  particle_population.hpp
//  spf
//
//  Created by Seong-Hwan Jun on 2018-04-05.
//  Copyright © 2018 Seong-Hwan Jun. All rights reserved.
//

#ifndef particle_population_h
#define particle_population_h

#include <gsl/gsl_rng.h>
#include <math.h>

#include "numerical_utils.hpp"

template <class P>
class ParticlePopulation
{
    vector<P> *particles;
    vector<double> *log_weights = 0;
    vector<double> *normalized_weights = 0;
    unsigned long num_particles;
    double ess = NaN;
    double sum_weights;
    double sum_weights_squared;
    double log_norm = NaN;
    bool resampled = false;

public:
    ParticlePopulation(vector<P> *particles);
    ParticlePopulation(vector<P> *particles, vector<double> *log_weights);
    ParticlePopulation(vector<P> *particles, vector<double> *log_weights, double log_norm);
    //ParticlePopulation(vector<P> *particles, vector<double> *log_weights, vector<double> *normalized_weights);
    inline vector<double> *get_log_weights() { return log_weights; }
    inline vector<P> *get_particles() { return particles; }
    vector<double> *get_normalized_weights();
    double get_ess();
    double get_log_norm();
    bool is_resampled();
    static ParticlePopulation<P>* construct_equally_weighted_population(int num_particles);
    static ParticlePopulation<P>* construct_equally_weighted_population(vector<P> *particles);
    ~ParticlePopulation();
};

template <class P>
ParticlePopulation<P>::ParticlePopulation(vector<P> *particles, vector<double> *log_weights)
{
    this->particles = particles;
    this->log_weights = log_weights;
    num_particles = particles->size();
}

template <class P>
ParticlePopulation<P>::ParticlePopulation(vector<P> *particles, vector<double> *log_weights, double log_norm) :
ParticlePopulation(particles, log_weights)
{
    this->log_norm = log_norm;
}

template <class P>
ParticlePopulation<P>::ParticlePopulation(vector<P> *particles)
{
    this->particles = particles;
    this->num_particles = particles->size();
    this->resampled = true;
}

//template <class P>
//ParticlePopulation<P>::ParticlePopulation(vector<P> *particles, vector<double> *log_weights, vector<double> *normalized_weights)
//{
//    this->particles = particles;
//    this->log_weights = log_weights;
//    this->normalized_weights = normalized_weights;
//    num_particles = particles->size();
//    this->get_ess();
//}

template <class P>
double ParticlePopulation<P>::get_ess()
{
    if (ess == NaN) {
        // compute ESS
        ess = 0.0;
        sum_weights = 0.0;
        sum_weights_squared = 0.0;
        if (normalized_weights == 0) {
            get_normalized_weights();
        }
        for (int i = 0; i < num_particles; i++) {
            ess += (*normalized_weights)[i] * (*normalized_weights)[i];
        }
        ess = 1.0/ess;
        ess /= num_particles;
    }
    return ess;
}

template <class P>
double ParticlePopulation<P>::get_log_norm()
{
    if (log_norm == NaN) {
        // compute ESS
        get_ess();
    }
    return log_norm;
}

template <class P>
vector<double> *ParticlePopulation<P>::get_normalized_weights()
{
    if (normalized_weights == 0) {
        normalized_weights = new vector<double>(num_particles);
        if (is_resampled()) {
            double w = 1./num_particles;
            for (size_t k = 0; k < num_particles; k++) {
                (*normalized_weights)[k] = w;
            }
        } else {
            if (log_norm == NaN) {
                // normalize the log weights, compute log_norm
                log_norm = normalize(*log_weights, *normalized_weights);
            } else {
                normalize(*log_weights, *normalized_weights, log_norm);
            }
        }
    }
    return normalized_weights;
}

template <class P>
bool ParticlePopulation<P>::is_resampled()
{
    return resampled;
}

template<class P>
ParticlePopulation<P>* ParticlePopulation<P>::construct_equally_weighted_population(int num_particles)
{
    vector<P> *particles = new vector<P>();
    vector<double> *log_weights = new vector<double>();
    double log_w = -log(num_particles);
    for (int n = 0; n < num_particles; n++)
    {
        particles->push_back(0);
        log_weights->push_back(log_w);
    }
    ParticlePopulation<P> *pop = new ParticlePopulation<P>(particles, log_weights);
    return pop;
}

template<class P>
ParticlePopulation<P>* ParticlePopulation<P>::construct_equally_weighted_population(vector<P> *particles)
{
    unsigned int num_particles = *particles->size();
    vector<double> *log_weights = new vector<double>();
    double log_w = -log(num_particles);
    for (int n = 0; n < num_particles; n++)
    {
        particles->push_back(0);
        log_weights->push_back(log_w);
    }
    ParticlePopulation<P> *pop = new ParticlePopulation<P>(particles, log_weights);
    return pop;
}

template <class P>
ParticlePopulation<P>::~ParticlePopulation()
{
    if (!particles)
        delete particles;
    if (!log_weights)
        delete log_weights;
    if (!normalized_weights)
        delete normalized_weights;
}

#endif /* particle_population_h */
