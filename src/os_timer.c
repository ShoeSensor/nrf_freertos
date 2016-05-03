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

#include "os_timer.h"
#include "FreeRTOS.h"
#include "task.h"
#include "timers.h"

#if (configTICK_RATE_HZ != 1000)
#define ticksToMs(a)    (a * portTICK_PERIOD_MS)
#define msToTicks(a)    (a / portTICK_PERIOD_MS)
#else
#define ticksToMs(a)    a
#define msToTicks(a)    a
#endif

struct os_timerHandle {
    TimerHandle_t timerHandle;
    uint32_t initDelay;
};

uint32_t os_timerGetMs(void)
{
    return ticksToMs(xTaskGetTickCount());
}

uint32_t os_timerGetElapsed(uint32_t start)
{
    return os_timerGetMs() - start;
}

bool os_timerIsElapsed(uint32_t start, uint32_t amount)
{
    return os_timerGetElapsed(start) >= amount;
}

void os_timerDelay(uint32_t ms)
{
    vTaskDelay(msToTicks(ms));
}

os_timerHandle_t os_timerTaskNew(os_timerConfig_t* conf, uint16_t initDelay)
{
    bool ret = false;
    os_timerHandle_t handle = calloc(1, sizeof(struct os_timerHandle));
    handle->timerHandle = xTimerCreate(conf->name,
            msToTicks(conf->period),
            !conf->oneShot,
            NULL,
            conf->callback);
    if(handle->timerHandle && !conf->startLater) {
        ret = xTimerStart(handle->timerHandle, initDelay);
        if(ret) {
            return handle;
        } else {
            xTimerDelete(handle->timerHandle, 0);
            free(handle);
            return NULL;
        }
    } else if(conf->startLater) {
        handle->initDelay = initDelay;
        return handle;
    } else {
        free(handle);
        return NULL;
    }
}

bool os_timerTaskStart(os_timerHandle_t handle)
{
    return xTimerStart(handle->timerHandle, handle->initDelay);
}

bool os_timerTaskStop(os_timerHandle_t handle)
{
    return xTimerStop(handle->timerHandle, 0);
}

bool os_timerTaskRestart(os_timerHandle_t handle)
{
    return xTimerReset(handle->timerHandle, 0);
}

bool os_timerTaskDelete(os_timerHandle_t handle)
{
    bool ret;
    ret = xTimerDelete(handle->timerHandle, 0);
    if(ret)
        free(handle);
    return ret;
}
