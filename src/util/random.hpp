/*
 * bacteria-core, core for cellular automaton
 * Copyright (C) 2016 Pavel Dolgov
 *
 * See the LICENSE file for terms of use.
 */

#ifndef RANDOM_HPP_
#define RANDOM_HPP_

#include <cstdlib>

/** Return random number from interval [0, end).
\param end End of interval (not included)
*/
unsigned int random(unsigned int end);

#endif
