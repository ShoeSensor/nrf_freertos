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
 * @defgroup os_timer Timers
 * @{
 * @ingroup os
 *
 * @brief FreeRTOS timer abstraction layer
 *
 */

#ifndef OS_TIMER_H
#define OS_TIMER_H

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

#ifdef  __cplusplus
extern "C" {
#endif

typedef void(*os_timerCallback_t)(void *);
typedef struct os_timerHandle *os_timerHandle_t;

typedef struct {
    const char* name;               /**< Name of the timer task*/
    uint32_t period;                /**< Period of the task execution*/
    bool oneShot;                   /**< Whether the task should be executed once*/
    os_timerCallback_t callback;    /**< Function to call when the timer expires*/
    bool startLater;                /**< If the timer should start later*/
} os_timerConfig_t;

/**
 * @brief Get the system time from the scheduler.
 * @details Get the time in milliseconds from the task scheduler. The scheduler
 * must be started for to get the time.
 * @return Time in milliseconds.
 */
uint32_t os_timerGetMs(void);

/**
 * @brief Get the elapsed time since a starting point.
 * @param start Start time to use as reference.
 * @return The amount of milliseconds since start.
 */
uint32_t os_timerGetElapsed(uint32_t start);

/**
 * @brief Test if a certain time had elapsed.
 * @details Test is an amount of time has elapsed since a starting point in
 * milliseconds.
 * @param start Start time to use as reference
 * @param amount Amount of time to check.
 * @retval  true If the time was elapsed.
 * @retval  false If the time was not elapsed.
 */
bool os_timerIsElapsed(uint32_t start, uint32_t amount);

/**
 * @brief Delay the current thread for certain amount of milliseconds
 * @param ms The amount of milliseconds to delay.
 */
void os_timerDelay(uint32_t ms);

/**
 * @brief Create a new periodic task.
 * @details Creates a software timer that calls a callback every time it
 * expires.
 * @param conf Configuration for the timer task.
 * @param initDelay Initial delay before the task is executed.
 * @return A handle to timer task. If the timer task could not be created,
 * NULL is returned.
 * @note This function uses dynamic memory allocation.
 */
os_timerHandle_t os_timerTaskNew(os_timerConfig_t *conf, uint16_t initDelay);

/**
 * @brief Start a timer task that was previously created but not started.
 * @param handle Handle to the timertask
 * @retval  true if the timertask started successfully.
 * @retval  false if the device is out of memory
 */
bool os_timerTaskStart(os_timerHandle_t handle);

/**
 * @brief Stop a timer task.
 * @details Stop a running timer task previously created with os_timerTaskNew.
 * @param handle Handle to the timer task to stop.
 * @retval  true If the timer task was stopped successfully.
 * @retval  false If the timer task could not be stopped.
 */
bool os_timerTaskStop(os_timerHandle_t handle);

/**
 * @brief Restart a timer task.
 * @details Restart a timer task previously stopped with os_timertaskStop.
 * @param handle Handle to the timer task to restart.
 * @retval  true If the timer task was restarted successfully.
 * @retval  false If the timer task could not be restarted.
 */
bool os_timerTaskRestart(os_timerHandle_t handle);

/**
 * @brief Delete a timer task from the memory.
 * @details Stops and deletes a timer task. Do not call this function before
 * os_timerTaskNew.
 * @param handle Handle to the timer task to delete.
 * @retval  true If the timer task was deleted successfully.
 * @retval  false If the timer task could not be deleted.
 */
bool os_timerTaskDelete(os_timerHandle_t handle);

#ifdef  __cplusplus
}
#endif

#endif /* OS_TIMER_H */

/**
 *@}
 **/
