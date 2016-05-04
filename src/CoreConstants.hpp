/*
 * bacteria-core, core for cellular automaton
 * Copyright (C) 2016 Pavel Dolgov
 *
 * See the LICENSE file for terms of use.
 */

#ifndef CORE_CONSTANTS_HPP_
#define CORE_CONSTANTS_HPP_

/** Mass increase per one 'eat' command */
const int EAT_MASS = 1;

/** Mass increase per one 'go' command */
const int GO_MASS = -1;

/** Mass increase per one 'clon' command */
const int CLON_MASS = -10;

/** Mass increase per one 'str' command */
const int STR_MASS = -1;

/** Max additional damage per one 'str' command */
const int MAX_STR_DAMAGE = -2;

const int MAX_COMMANDS_PER_INSTRUCTION = 100;

const int MIN_COMMANDS_PER_INSTRUCTION = 1;

/** Maximum randomly generated number of actions (per instruction) */
const int RANDOM_MAX_ACTIONS = 5;

/** Maximum number of actions per move */
const int MAX_ACTIONS = 1;

/** Maximum number of pseudo actions per move */
const int MAX_PSEUDO_ACTIONS = 30;

/** Mass penalty (if pseudo actions >= MAX_PSEUDO_ACTIONS) */
const int PSEUDO_ACTIONS_EXCESS_PENALTY = -5;

/** Default mass of new bacteria */
const int DEFAULT_MASS = 5;

/** Default mass of cloned bacteria */
const int DEFAULT_CLON_MASS = 2;

/** Minimum width of board */
const int MIN_WIDTH = 5;

/** Maximum width of board */
const int MAX_WIDTH = 500;

/** Minimum height of board */
const int MIN_HEIGHT = 5;

/** Maximum height of board */
const int MAX_HEIGHT = 500;

#endif
