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


/** @file
 * @defgroup os_thread Threading
 * @{
 * @ingroup os
 *
 * @brief FreeRTOS thread abstraction layer
 *
 */


#ifndef OS_THREAD_H
#define OS_THREAD_H

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

#ifdef  __cplusplus
extern "C" {
#endif

typedef void(*os_threadCallback_t)(void*);
typedef struct os_threadHandle *os_threadHandle_t;

typedef enum {
    STACK_SIZE_MINIMUM = 100,   /**< Minimum stacksize*/
    STACK_SIZE_DEFAULT = 150,   /**< Stack size for regular threads*/
    STACK_SIZE_BIG = 256        /**< Stack size for big threads*/
} os_stackSizes_t;

typedef enum {
    THREAD_PRIO_LOW = 0,    /**< Lowest thread priority*/
    THREAD_PRIO_NORM,       /**< Normal thread priority*/
    THREAD_PRIO_HIGH        /**< Highest thread priority*/
} os_threadPriorities_t;

typedef struct {
    const char* name;                   /**< Name of the thread*/
    os_threadCallback_t threadCallback; /**< Code to execute in the thread*/
    void* threadArgs;                   /**< Arguments to pass to the thread callback*/
    uint32_t stackSize;                 /**< Amount of stack memory the thread may use*/
    os_threadPriorities_t priority;     /**< The thread priority for the scheduler*/
} os_threadConfig_t;

/**
 * @brief Create and deploy a new thread. The thread will not run until
 * os_startSchedular() is called.
 * @param   conf  Configuration structure for the thread.
 * @return  A handle to the currently created thread. If something went wrong,
 * NULL is returned.
 * @note    This function uses dynamic memory allocation
 */
os_threadHandle_t os_threadNew(os_threadConfig_t *conf);

/**
 * @brief Start the OS task scheduler. This will cause the created threads to run.
 * @return false If the scheduler could not be started because of insufficient
 * ram. If the device runs out of ram after it started, this function will return
 * as well.
 * @note This function should never return.
 */
void os_startScheduler(void);

/**
 * @brief Stop a running thread.
 * @param handle    Handle to the thread to stop.
 * @retval  true    If the thread is stopped successfully.
 * @retval  false   If thread could not be stopped.
 */
bool os_threadStop(os_threadHandle_t handle);

/**
 * @brief Pause a running thread.
 * @details Suspend a thread to resume it later. The thread must be running
 * to do this.
 * @param handle Handle to the thread to suspend.
 * @retval  true If the thread was paused successfully.
 * @retval  false If the the thread could not be paused.
 */
bool os_threadPause(os_threadHandle_t handle);

/**
 * @brief Resume a thread.
 * @details Resume a thread that was previous paused.
 * @param handle Handle to the thread that has to be resumed.
 * @retval  true If the thread was successfully resumed.
 * @retval  false If the thread could not be resumed.
 */
bool os_threadResume(os_threadHandle_t handle);

/**
 * @brief Let a thread sleep until it's been notified by another task.
 * @details Light weight alternative for (binary) semaphores. This must be
 * called by the thread that needs to wait.
 * @return
 */
void os_threadWait(void);

/**
 * @brief Notify a waiting task.
 * @details Light weight alternative to (binary) semaphores. This wakes up
 * as task that is currently waiting.
 * @param handle Handle to the task to notify. Note that the task to notify
 * needs to be in a waiting state.
 */
void os_threadNotify(os_threadHandle_t handle);

/**
 * @brief Notify a waiting task from an interrupt.
 * @details Light weight alternative to (binary) semaphores. This wakes up
 * as task that is currently waiting. This function is interrupt safe.
 * @param handle Handle to the task to notify. Note that the task to notify
 * needs to be in a waiting state.
 */
void os_threadIsrNotify(os_threadHandle_t handle);

/**
 * @brief Test if a thread is running.
 * @param handle Handle to the thread to test.
 * @retval true If the thread is currently running.
 * @retval false If the thread is currently not running.
 */
bool os_threadIsRunning(os_threadHandle_t handle);

/**
 * @brief Test if a thread is paused.
 * @param handle Handle to the thread to test.
 * @retval  true If the thread is paused.
 * @retval  false If the thread is not paused.
 */
bool os_threadIsPaused(os_threadHandle_t handle);

/**
 * @brief Exit a thread.
 * @details This function must be called at the end of a thread callback. This
 * ensures that the thread is removed from the memory and schedulers task list.
 * This should be called by the task itself.
 */
void os_threadExit();

/**
 * @brief Delete a thread.
 * @details Stop and delete a thread from the memory.
 * DO NOT call this function before os_threadNew.
 * @param handle Handle to the thread that must be deleted.
 */
void os_threadDelete(os_threadHandle_t handle);


#ifdef  __cplusplus
}
#endif

#endif /* OS_THREAD_H */

/**
 *@}
 **/
