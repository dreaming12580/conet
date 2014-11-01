/*
 * =====================================================================================
 *
 *       Filename:  timewheel.h
 *
 *    Description:
 *
 *        Version:  1.0
 *        Created:  2014年05月06日 06时22分35秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  piboyeliu
 *   Organization:
 *
 * =====================================================================================
 */

#ifndef __TIMEWHEEL_H_INCL__
#define __TIMEWHEEL_H_INCL__

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <time.h>
#include <sys/time.h>
#include "base/incl/list.h"
#include "base/incl/time_helper.h"

struct timewheel_t;

struct timeout_handle_t
{
    list_head link_to;
    uint64_t  timeout;
    void (*fn)(void *);
    void * arg;
    timewheel_t * tw;
    int interval;
};

void init_timeout_handle(timeout_handle_t * self,
                         void (*fn)(void *), void *arg, int timeout=-1);

#define DEFINE_TIMEOUT_HANDLE(name, func, arg1) \
        timeout_handle_t name; \
        init_timeout_handle(&name, func, arg1)


struct timewheel_t
{
    uint64_t pos;
    uint64_t prev_ms;
    int task_num;
    int slot_num;
    void * co;
    int stop;
    int timerfd;
    int update_timeofday_flag;
    struct timeval prev_tv;
    list_head *slots;
    uint64_t now_ms;
};

void init_timewheel(timewheel_t *self, int solt_num = 60*1000);
void fini_timewheel(timewheel_t *self);

//void free_timewheel(timewheel_t *tw);
//timewheel_t *alloc_timewheel();

void cancel_timeout(timeout_handle_t *self);
void stop_timewheel(timewheel_t *self);


bool set_interval(timewheel_t *tw, timeout_handle_t * obj, int interval);

bool set_timeout(timewheel_t *tw, timeout_handle_t * obj, int timeout);

int check_timewheel(timewheel_t *self, uint64_t cur_ms = 0);

// global timewheel_t
void set_timeout(timeout_handle_t *obj, int timeout /* ms*/);
void set_interval(timeout_handle_t * obj, int interval);


#endif
