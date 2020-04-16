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
 * @file my_states.h
 * @author Alex Hoffman
 * @email alex.hoffman@tum.de
 * @website http://alexhoffman.info
 * @copyright GNU GPL v3
 * */

#ifndef __MY_STATES__
#define __MY_STATES__

typedef struct {
  int cur_val;
  int prev_val;
} my_states_data;

typedef enum {
  state_first_e = 0,
  state_second_e = 1,
  state_third_e = 2,
} my_states_id;

void state_probe(void);
void state_one_run(void);
void state_two_enter(void);
void state_two_run(void);
void state_two_exit(void);
void state_three_run(void);

#endif
