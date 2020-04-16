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
 * @file states.c
 * @author Alex Hoffman
 * @copyright GNU GPL v3
 * */

#include "error.h"
#include "states.h"

#include <stdlib.h>
#include <string.h>

typedef struct state state_t;
/**
 * @struct state
 * @brief Data object to store a single state's properties and functions
 */
struct state {
  unsigned char id; /**< The state's ID number */

  void *data; /**< Data stored inside the state */

  char *name; /**< String representation of the state's name */

  unsigned char initd; /**< If the state's probe function has been run yet */

  void (*probe)(void); /**< The states init function */

  void (*enter)(void); /**< Function that is called when going into the state */
  void (*run)(void);   /**< Run function that executes while the state is the
                          current state*/
  void (*exit)(void);  /**< Function run when the state is being moved out of*/
};

/**
 *
 * @struct state_machine
 * @brief The state machine
 */
typedef struct state_machine {
  state_t *current_state; /**< State currently executing in the SM */
  state_t *next_state;    /**< State to be moved to */

  unsigned char input; /**< 8 bit input vector */

  void (*callback)(void); /**< SM callback, executed at the end of each cycle */

  state_t **states; /**< Array of all states in the SM */
  int count;        /**< Number of states in the SM */
} state_machine_t;

state_machine_t state_machine_dev = {0};

void states_set_callback(void (*callback)(void)) {
  (state_machine_dev.callback) = callback;
}

void states_set_data(void *data) {
  if (state_machine_dev.current_state->data)
    free(state_machine_dev.current_state->data);
  state_machine_dev.current_state->data = data;
}

void states_set_input(unsigned char input) { state_machine_dev.input = input; }

unsigned char states_set_state(unsigned int state_id) {
  for (unsigned char i = 0; i < state_machine_dev.count; i++) {
    if (state_machine_dev.states[i]->id == state_id) {
      state_machine_dev.next_state = state_machine_dev.states[i];
      return 0;
    }
  }
  return -EINVAL;
}

void *states_get_data(void) { return state_machine_dev.current_state->data; }

char *states_get_state_name(void) {
  return state_machine_dev.current_state->name;
}

unsigned char states_get_input(void) { return state_machine_dev.input; }

unsigned int states_get_state_id(void) {
  return state_machine_dev.current_state->id;
}

unsigned int states_get_state_count(void) { return state_machine_dev.count; }

void states_clear_input(void) { state_machine_dev.input = 0; }

unsigned char states_run(void) {
  if (state_machine_dev.next_state->id != state_machine_dev.current_state->id) {
    if (state_machine_dev.current_state->exit) /* Exit current state */
      (state_machine_dev.current_state->exit)();
    if (state_machine_dev.next_state->enter) /* Enter next state */
      (state_machine_dev.next_state->enter)();
    state_machine_dev.current_state =
        state_machine_dev.next_state; /* Change states */
  }

  if (state_machine_dev.current_state->run) /* Run current state */
    (state_machine_dev.current_state->run)();

  if (state_machine_dev.callback) /** SM callback if set */
    (state_machine_dev.callback)();

  return 0;
}

unsigned char states_add(void (*probe)(void), void (*enter)(void),
                         void (*run)(void), void (*exit)(void), int ID,
                         char *name) {
  unsigned char error = 0;
  state_t *ret = calloc(1, sizeof(state_t));
  if (!ret)
    return -ENOMEM;

  (ret->enter) = enter;
  (ret->run) = run;
  (ret->exit) = exit;
  (ret->probe) = probe;

  ret->id = ID;

  ret->name = malloc(sizeof(char) * (strlen(name) + 1));
  if (!ret->name)
    return -ENOMEM;
  strcpy(ret->name, name);

  state_machine_dev.states =
      realloc(state_machine_dev.states,
              sizeof(state_t *) * (state_machine_dev.count + 1));
  state_machine_dev.states[state_machine_dev.count] = ret;
  state_machine_dev.count++;

  return 0;
}

unsigned char states_init(void) {
  state_t *current_state = state_machine_dev.current_state;
  state_t *next_state = state_machine_dev.next_state;

  /** All states who's probe has not been run are probed, if possible */
  for (int i = 0; i < state_machine_dev.count; i++)
    if (!state_machine_dev.states[i]->initd) {
      if (state_machine_dev.states[i]->probe) {
        state_machine_dev.current_state = state_machine_dev.states[i];
        (state_machine_dev.states[i]->probe)();
      }
      state_machine_dev.states[i]->initd = 1;
    }

  /** Restore original states */
  state_machine_dev.current_state = current_state;
  state_machine_dev.next_state = next_state;

  /** First added state is the initial state */
  if (state_machine_dev.current_state == NULL)
    if (state_machine_dev.count >= 1) {
      state_machine_dev.current_state = state_machine_dev.states[0];
      state_machine_dev.next_state = state_machine_dev.states[0];
    } else
      return -ENOENT;
  else
    return -ENOENT;
  return 0;
}
