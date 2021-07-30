//
//  permutation_stream.cpp
//  SPF
//
//  Created by Seong-Hwan Jun on 2021-07-29.
//

#include "permutation_stream.hpp"

PermutationStream::PermutationStream(unsigned int size)
{
    this->size = size;
    this->indices = new unsigned int[size];
    this->num_calls = 0;
    this->random = generate_random_object(seed);
    for (int i = 0; i < size; i++) {
        indices[i] = i;
    }
}

void PermutationStream::reset()
{
    gsl_rng_set(random, seed);
    this->num_calls = 0;
    for (int i = 0; i < size; i++) {
        indices[i] = i;
    }
}

void PermutationStream::set_seed(long seed)
{
    this->seed = seed;
    reset();
}

unsigned int PermutationStream::pop()
{
    int idx = num_calls++ % size;
    if (idx == 0) {
        // shuffle
        gsl_ran_shuffle(random, indices, size, sizeof(unsigned int));
    }
    return indices[idx];
    
}

PermutationStream::~PermutationStream()
{
    if (!random)
        gsl_rng_free(random);
    delete indices;
}
