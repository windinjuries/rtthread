/*
 * Copyright (c) 2006-2022, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2018-11-22     Jesven       first version
 */

#include <rthw.h>
#include <rtthread.h>
#include <stdint.h>
#include "cp15.h"
#include "board.h"
#include "gtimer.h"

#define TIMER_IRQ   30

static rt_uint64_t timerStep = 0;

static void rt_hw_timer_isr(int vector, void *param)
{
    rt_hw_set_gtimer_val(timerStep);
    rt_tick_increase();
}

void rt_hw_timer_enable(void)
{
    rt_hw_set_gtimer_val(timerStep);
    rt_hw_interrupt_umask(TIMER_IRQ);
    rt_hw_gtimer_enable();
}

int rt_hw_timer_init(void)
{
    rt_hw_interrupt_install(TIMER_IRQ, rt_hw_timer_isr, RT_NULL, "tick");
    __ISB();
    timerStep = rt_hw_get_gtimer_frq();
    __DSB();
    timerStep /= RT_TICK_PER_SECOND;
    rt_hw_timer_enable();

    return 0;
}
INIT_BOARD_EXPORT(rt_hw_timer_init);
