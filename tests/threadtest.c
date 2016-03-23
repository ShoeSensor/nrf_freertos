/*
 * Copyright 2016 Bart Monhemius.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <stdbool.h>
#include <stdint.h>

#include "FreeRTOS.h"
#include "task.h"
#include "timers.h"
#include "bsp.h"
#include "pca10028.h"
#include "nrf51.h"
#include "nordic_common.h"
#include "nrf_gpio.h"
#include "nrf_drv_clock.h"
#include "sdk_errors.h"
#include "app_error.h"
#include "nrf_drv_clock.h"

#include "os_mutex.h"
#include "os_thread.h"
#include "os_timer.h"


#define APP_TIMER_PRESCALER                  0  /**< Value of the RTC1 PRESCALER register. */
#define APP_TIMER_OP_QUEUE_SIZE              4  /**< Size of timer operation queues. */
#define DEAD_BEEF 0xDEADBEEF

os_threadHandle_t threadHandle;
os_threadHandle_t threadHandle2;
os_threadHandle_t threadHandle3;
os_threadHandle_t threadHandle4;
os_mutexHandle_t mutex;

static void testThread(void *args)
{
    uint32_t time = os_timerGetMs();
    while(1) {
        if(os_timerIsElapsed(time, 500)) {
            nrf_gpio_pin_toggle(LED_1);
            time = os_timerGetMs();
        }
    }
    os_threadExit(threadHandle);
}

static void testThread2(void *args)
{
    while(1) {
        nrf_gpio_pin_toggle(LED_2);
        os_timerDelay(750);
    }
    os_threadExit(threadHandle2);
}

void testThread3(void *args)
{
    while(1) {
        if(os_mutexLock(mutex)) {
            nrf_gpio_pin_set(LED_3);
            os_timerDelay(100);
            os_mutexUnlock(mutex);
            os_timerDelay(200);
        }
    }
    os_threadExit(threadHandle3);
}

void testThread4(void *args)
{
    while(1) {
        if(os_mutexLock(mutex)) {
            os_timerDelay(100);
            nrf_gpio_pin_clear(LED_3);
            os_mutexUnlock(mutex);
            os_timerDelay(200);
        }
    }
    os_threadExit(threadHandle4);
}

static void timerTask(void *args)
{
    nrf_gpio_pin_toggle(LED_4);
}

void assert_nrf_callback(uint16_t line_num, const uint8_t * p_file_name)
{
    app_error_handler(DEAD_BEEF, line_num, p_file_name);
}

int main(void)
{
    uint32_t errCode = 0;

    errCode = nrf_drv_clock_init(NULL);
    APP_ERROR_CHECK(errCode);

    nrf_gpio_cfg_output(BSP_LED_0);
    nrf_gpio_cfg_output(BSP_LED_1);
    nrf_gpio_cfg_output(BSP_LED_2);
    nrf_gpio_cfg_output(BSP_LED_3);
    nrf_gpio_pin_set(BSP_LED_0);
    nrf_gpio_pin_set(BSP_LED_1);
    nrf_gpio_pin_set(BSP_LED_2);
    nrf_gpio_pin_set(BSP_LED_3);

    os_threadConfig_t threadConfig = {
        .name = "thread1",
        .threadCallback = testThread,
        .threadArgs = NULL,
        .stackSize = configMINIMAL_STACK_SIZE + 100,
        .priority = THREAD_PRIO_NORM
    };

    os_threadConfig_t threadConfig2 = {
        .name = "thread2",
        .threadCallback = testThread2,
        .threadArgs = NULL,
        .stackSize = configMINIMAL_STACK_SIZE + 100,
        .priority = THREAD_PRIO_NORM
    };

    os_threadConfig_t threadConfig3 = {
        .name = "thread3",
        .threadCallback = testThread3,
        .threadArgs = NULL,
        .stackSize = configMINIMAL_STACK_SIZE + 100,
        .priority = THREAD_PRIO_LOW
    };

    os_threadConfig_t threadConfig4 = {
        .name = "thread4",
        .threadCallback = testThread4,
        .threadArgs = NULL,
        .stackSize = configMINIMAL_STACK_SIZE + 100,
        .priority = THREAD_PRIO_LOW
    };

    os_timerConfig_t timerConf = {
            .name = "task1",
            .period = 1000,
            .oneShot = false,
            .callback = timerTask
    };

    mutex = os_mutexNew();
    os_timerTaskNew(&timerConf, 250);
//    threadHandle = os_threadNew(&threadConfig);
    threadHandle2 = os_threadNew(&threadConfig2);
    threadHandle3 = os_threadNew(&threadConfig3);
    threadHandle4 = os_threadNew(&threadConfig4);
    os_startScheduler();
    while (1);
    return 0;
}
