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
 * @file my_states.c
 * @author Alex Hoffman
 * @email alex.hoffman@tum.de
 * @website http://alexhoffman.info
 * @copyright GNU GPL v3
 * */

#include <stdio.h>
#include <stdlib.h>

#include "my_states.h"
#include "states.h"

void increment_state_data(int inc) {
  my_states_data *state_count = (my_states_data *)states_get_data();
  state_count->prev_val = state_count->cur_val;
  state_count->cur_val += inc;
}

void decrement_state_data(int dec) {
  my_states_data *state_count = (my_states_data *)states_get_data();
  state_count->prev_val = state_count->cur_val;
  state_count->cur_val -= dec;
}

void state_probe(void) {
  my_states_data *init_data = calloc(1, sizeof(my_states_data));
  if (!init_data) {
    printf("Failed to probe state %d\n", states_get_state_id);
    exit(-1);
  }

  states_set_data(init_data);

  printf("State %d probed\n", states_get_state_id());
}

void state_one_run(void) { increment_state_data(1); }

void state_two_enter(void) { printf("Entering 2\n"); }

void state_two_run(void) { increment_state_data(2); }

void state_two_exit(void) { printf("Exiting 2\n"); }

void state_three_run(void) { decrement_state_data(1); }
