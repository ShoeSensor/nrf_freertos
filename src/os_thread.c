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

#include "os_thread.h"
#include "FreeRTOS.h"
#include  "task.h"

struct os_threadHandle {
    TaskHandle_t threadHandle;
    bool isRunning;
    bool isPaused;
};

os_threadHandle_t os_threadNew(os_threadConfig_t *conf)
{
    os_threadHandle_t handle = calloc(1, sizeof(struct os_threadHandle));
    if(xTaskCreate(conf->threadCallback,
            conf->name,
            conf->stackSize,
            conf->threadArgs,
            conf->priority,
            &handle->threadHandle) != pdPASS) {
        free(handle);
        return NULL;
    }
    handle->isPaused = false;
    handle->isRunning= true;
    return handle;
}

void os_startScheduler(void)
{
    vTaskStartScheduler();
}

bool os_threadStop(os_threadHandle_t handle)
{
    vTaskDelete(handle);
    return true;
}

bool os_threadPause(os_threadHandle_t handle)
{
    handle->isPaused = true;
    vTaskSuspend(handle->threadHandle);
    return true;
}

bool os_threadResume(os_threadHandle_t handle)
{
    handle->isPaused = false;
    vTaskResume(handle->threadHandle);
    return true;
}

bool os_threadIsRunning(os_threadHandle_t handle)
{
    return (eTaskGetState(handle->threadHandle) == eRunning);
}

bool os_threadIsPaused(os_threadHandle_t handle)
{
    return (eTaskGetState(handle->threadHandle) == eSuspended);
}

void os_threadExit(os_threadHandle_t handle)
{
    handle->isRunning = false;
    vTaskDelete(handle->threadHandle);
}

bool os_threadDelete(os_threadHandle_t handle)
{
    vTaskDelete(handle->threadHandle);
    free(handle);
    return true;
}
