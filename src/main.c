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
 * @file main.c
 * @author Alex Hoffman
 * @email alex.hoffman@tum.de
 * @website http://alexhoffman.info
 * @copyright GNU GPL v3
 * */

#include <argp.h>
#include <errno.h>
#include <pthread.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "main.h"
#include "my_states.h"
#include "states.h"

const char *argp_program_version = "1.0";
const char *argp_program_bug_address = "alex.hoffman@tum.de";
static char doc[] =
    "A basic example to show the functionality of a state machine";
static char args_doc[] =
    "-n,  Specify the number the state machine should count to";
static struct argp_option options[] = {
    {"verbose", 'v', 0, 0, "Show verbose output"},
    {"tick", 't', "int", 0, "Set the state machine tick duration in mS"},
    {0}};

pthread_cond_t cond = PTHREAD_COND_INITIALIZER;    /** Condition variable */
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER; /** Self explanatory */
int count = 0;
int count_too = 0;


typedef struct {
  int args[1];
  int verbose;
  int tick;
} arguments_t;


void errno_abort(char* message) {
  perror(message);
  exit(EXIT_FAILURE);
}

void err_abort(int status, char *message);

static error_t parse_opt(int key, char *arg, struct argp_state *state) {
  arguments_t *args = state->input;

  switch (key) {
  case 'v':
    args->verbose = 1;
    break;
  case 't':
    args->tick = (int)strtol(arg, NULL, 10);
    break;
  case ARGP_KEY_ARG:
    if (state->arg_num > 1)
      argp_usage(state);
    args->args[state->arg_num] = (int)strtol(arg, NULL, 10);
    break;
  case ARGP_KEY_END:
    if (state->arg_num < 1)
      argp_usage(state);
  default:
    return ARGP_ERR_UNKNOWN;
  }

  return 0;
}

static struct argp argp = {options, parse_opt, args_doc, doc};

void timer_callback(union sigval arg) {
  int error;

  error = pthread_mutex_lock(&mutex);
  if (error != 0)
    errno_abort("Callback locking");

  states_run();

  if (count >= count_too) {
    error = pthread_cond_signal(&cond); /** Signal condition fulfilled */
    if (error != 0)
      err_abort(error, "Signal condition");
  }

  error = pthread_mutex_unlock(&mutex);
  if (error != 0)
    err_abort(error, "Callback unlocking");
}

void create_timer(int tick) {
  long long tick_nanos = MSEC_IN_NANO(tick); /** Tick in nanoseconds */
  timer_t our_timer;                         /** POSIX timer */
  int error;
  struct itimerspec timer_specs; /** Stores timer interval */
  struct sigevent se;            /** Describes timer expiration event */

  se.sigev_notify = SIGEV_THREAD;        /** New thread to handle event */
  se.sigev_value.sival_ptr = &our_timer; /** Timer responsible */
  se.sigev_notify_function = timer_callback;
  se.sigev_notify_attributes = NULL;

  timer_specs.it_interval.tv_nsec =
      tick_nanos % NANOS_IN_SEC; /** Interval nanoseconds */
  timer_specs.it_interval.tv_sec =
      tick_nanos / NANOS_IN_SEC; /** Interval seconds (tick) */
  timer_specs.it_value.tv_nsec =
      tick_nanos % NANOS_IN_SEC; /** Initial period nanoseconds */
  timer_specs.it_value.tv_sec =
      tick_nanos / NANOS_IN_SEC; /** Initial period seconds */

  error = timer_create(CLOCK_REALTIME, &se, &our_timer); /** Create timer */
  if (error == -1)
    errno_abort("Creating timer");

  error =
      timer_settime(our_timer, 0, &timer_specs, 0); /** Set timer interval */
  if (error == -1)
    errno_abort("Setting timer");
}

void statemachine_callback(void) {

  my_states_data *cur_data = states_get_data();

  int diff = cur_data->cur_val - cur_data->prev_val;

  count += diff;

  printf("%s\nTotal count: %d\nIn state %d, state count: %d\n---------\n",
         states_get_state_name(), count, states_get_state_id() + 1,
         cur_data->cur_val);

  states_set_state(rand() %
                   states_get_state_count()); /** Switch to random next state */
}


int main(int argc, char **argv) {
  int error;

  srand(time(NULL)); /** Init random numbers */

  /** Parse args */
  arguments_t arguments;

  arguments.verbose = 0; /** Default values */
  arguments.tick = DEFAULT_TICK;
  argp_parse(&argp, argc, argv, 0, 0, &arguments);

  count_too = arguments.args[0];

  printf("Count until = %d\nVerbose = %s\nTick = %dms\n", count_too,
         arguments.verbose ? "yes" : "no", arguments.tick);

  /** Initialize state machine */
  states_add(state_probe, NULL, state_one_run, NULL, state_first_e,
             FIRST_STATE_NAME);
  states_add(state_probe, state_two_enter, state_two_run, state_two_exit,
             state_second_e, SECOND_STATE_NAME);

  states_set_callback(statemachine_callback);

  states_init();

  printf("\n### Starting State Machine ###\n\n");

  /** Spawn a POSIX thread to block on the conditional count < count_out */
  create_timer(arguments.tick);

  error = pthread_mutex_lock(&mutex);

  while (count < count_too) {
    /** Blocked thread can be awakened by a call to pthread_cond_signal */
    error =
        pthread_cond_wait(&cond, &mutex); /** Release mutex and block on cond */
    if (error != 0)
      err_abort(error, "Wait on condition");
  }

  error = pthread_mutex_unlock(&mutex);
  if (error != 0)
    err_abort(error, "Unlock mutex");

  printf("Finshed\n");

}

void err_abort(int status, char *message) {
  fprintf(stderr, "%s\n", message);
  exit(status);
}

