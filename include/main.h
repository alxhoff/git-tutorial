/****************************************************************************
   ----------------------------------------------------------------------
    Copyright (C) Alexander Hoffman, 2019

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
   ----------------------------------------------------------------------
 ****************************************************************************/

/**
 * @file main.h
 * @author Alex Hoffman
 * @email alex.hoffman@tum.de
 * @website http://alexhoffman.info
 * @copyright GNU GPL v3
 * @mainpage
 * This basic implementation of a state machine using function pointers and
 * POSIX thread aims to show a simplified method for implementing a flexible
 * state machine with minimal code recycling required.
 *
 * The states.c and states.h files also demonstrates how mini API interfaces can
 * be implemented in a C project to segment your project into modules which only
 * expose clean API interfaces. This allows for the interfacing of multiple
 * components within a project to be done cleanly as well as allowing for easier
 * debugging through the black boxing of each module, which can be easily tested
 * separately.
 * */

#ifndef __MAIN_H__
#define __MAIN_H__

#define FIRST_STATE_NAME "Count up once"
#define SECOND_STATE_NAME "Count up twice"
#define THIRD_STATE_NAME "Count down once"

#define NANOS_IN_SEC 1000000000
#define NANOS_IN_MSEC 10000000
#define MSEC_IN_NANO(MSEC) MSEC *NANOS_IN_MSEC

#define DEFAULT_TICK 100

#endif
