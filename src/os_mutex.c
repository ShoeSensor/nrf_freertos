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

#include "os_mutex.h"
#include "FreeRTOS.h"
#include "semphr.h"
#include "task.h"

os_mutexHandle_t os_mutexNew(void)
{
    return xSemaphoreCreateMutex();
}

bool os_mutexLock(os_mutexHandle_t handle)
{
    return xSemaphoreTake(handle, portMAX_DELAY);
}

bool os_mutexTryLock(os_mutexHandle_t handle)
{
    return xSemaphoreTake(handle, 0);
}

bool os_mutexTimedLock(os_mutexHandle_t handle, uint32_t timeout)
{
    return xSemaphoreTake(handle, timeout);
}

bool os_mutexIsrLock(os_mutexHandle_t handle)
{
    bool hasWoken = false;
    bool ret = false;
    ret = xSemaphoreTakeFromISR(handle, (BaseType_t *)&hasWoken);
    portYIELD_FROM_ISR(hasWoken);
    return ret;
}

void os_mutexUnlock(os_mutexHandle_t handle)
{
    (void)xSemaphoreGive(handle);
}

bool os_mutexIsrUnLock(os_mutexHandle_t handle)
{
    bool hasWoken = false;
    bool ret = false;
    ret = xSemaphoreGiveFromISR(handle, (BaseType_t *)&hasWoken);
    portYIELD_FROM_ISR(hasWoken);
    return ret;
}

void os_mutexDelete(os_mutexHandle_t handle)
{
    vSemaphoreDelete(handle);
}
