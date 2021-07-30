//
//  permutation_stream.hpp
//  spf
//
//  Created by Seong-Hwan Jun on 2018-04-17.
//  Copyright © 2018 Seong-Hwan Jun. All rights reserved.
//

#ifndef permutation_stream_h
#define permutation_stream_h

#include <vector>
#include <gsl/gsl_rng.h>
#include <gsl/gsl_randist.h>
#include <gsl/gsl_permutation.h>

#include "sampling_utils.hpp"

using namespace std;

class PermutationStream
{
    gsl_rng *random = 0;
    long seed;
    unsigned int *indices;
    unsigned int size;
    unsigned int num_calls;
public:
    PermutationStream(unsigned int size);
    unsigned int pop();
    void reset();
    void set_seed(long seed);
    inline gsl_rng *get_random() { return random; }
    ~PermutationStream();
};

#endif /* permutation_stream_h */
