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

#include "os_semaphore.h"

#include "FreeRTOS.h"
#include "semphr.h"
#include "task.h"

os_semHandle_t os_semNew(os_semConfig_t *conf)
{
    if(conf->binary)
        return xSemaphoreCreateBinary();
    else
        return xSemaphoreCreateCounting(conf->maxCount, conf->initCount);
}

bool os_semWait(os_semHandle_t handle)
{
    return xSemaphoreTake(handle, portMAX_DELAY);
}

bool os_semTryWait(os_semHandle_t handle)
{
    return xSemaphoreTake(handle, 0);
}

bool os_semTimedWait(os_semHandle_t handle, uint32_t timeout)
{
    return xSemaphoreTake(handle, timeout);
}

bool os_semIsrWait(os_semHandle_t handle)
{
    bool hasWoken = false;
    bool ret;
    ret = xSemaphoreTakeFromISR(handle, (BaseType_t *)&hasWoken);
    portYIELD_FROM_ISR(hasWoken);
    return ret;
}

void os_semPost(os_semHandle_t handle)
{
    (void)xSemaphoreGive(handle);
}

bool os_semIsrPost(os_semHandle_t handle)
{
    bool hasWoken = false;
    bool ret = false;
    ret = xSemaphoreGiveFromISR(handle, (BaseType_t *)&hasWoken);
    portYIELD_FROM_ISR(hasWoken);
    return ret;
}

void os_semDelete(os_semHandle_t handle)
{
    vSemaphoreDelete(handle);
}
