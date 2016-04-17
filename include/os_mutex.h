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
 * @defgroup os OSAL
 * @defgroup os_mutex Mutexes
 * @{
 * @ingroup os
 *
 * @brief FreeRTOS mutex abstraction layer
 *
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include "FreeRTOS.h"
#include "semphr.h"

#ifndef OS_MUTEX_H
#define OS_MUTEX_H

#ifdef  __cplusplus
extern "C" {
#endif

typedef SemaphoreHandle_t os_mutexHandle_t;

/**
 * @brief Create a new mutex object
 * @details Creates a mutex object and a handle to it later.
 * used for locking/unlocking.
 * @return Handle to the new mutex object.
 */
os_mutexHandle_t os_mutexNew(void);

/**
 * @brief Lock a mutex indefinitely.
 * @details Block a mutex without a timeout. This is a blocking function.
 * @param handle Handle to the mutex to lock.
 * @retval  true If the mutex was successfully lock.
 * @retval  false If the mutex could not be locked.
 */
bool os_mutexLock(os_mutexHandle_t handle);

/**
 * @brief Try to lock a mutex.
 * @details Non-blocking way of locking a mutex. If it cannot lock, it will
 * return immediately.
 * @param handle Handle to the mutex to lock.
 * @retval  true If the mutex was successfully lock.
 * @retval  false If the mutex could not be locked.
 */
bool os_mutexTryLock(os_mutexHandle_t handle);

/**
 * @brief Lock a mutex with a given timeout.
 * @details Tries to lock a mutex until a timeout expires.
 * @param handle Handle to the mutex to lock.
 * @param timeout Timeout to wait before bailing out.
 * @retval  true If the mutex was successfully lock.
 * @retval  false If the mutex could not be locked. Or the timeout expired.
 */
bool os_mutexTimedLock(os_mutexHandle_t handle, uint32_t timeout);

/**
 * @brief Lock a mutex from an interrupt service routine.
 * @details Use this function to lock a mutex from an ISR. This function is
 * save to call from an ISR. The other ones not.
 * @param handle Handle to the mutex to lock.
 * @retval  true If the mutex was successfully locked.
 * @retval  false If the mutex could not be locked. Or a higher priority task
 * has woken.
 */
bool os_mutexIsrLock(os_mutexHandle_t handle);

/**
 * @brief Unlock a mutex.
 * @param handle Handle to the mutex to unlock.
 */
void os_mutexUnlock(os_mutexHandle_t handle);

/**
 * @brief Unlock a mutex from an interrupt service routine.
 * @details Use this function to unlock a mutex from an ISR. This function is
 * save to call from an ISR, the other one not.
 * @param handle Handle to the mutex to unlock.
 * @retval  true If the mutex was successfully unlocked.
 * @retval  false If the mutex could not be unlocked. Or a higher priority task
 * has woken.
 */
bool os_mutexIsrUnLock(os_mutexHandle_t handle);

/**
 * @brief Delete a mutex object and the handle to it.
 * @details Do not delete a mutex while an object is still waiting for the mutex
 * to be unlocked.
 * @param handle Handle to the mutex to be deleted.
 */
void os_mutexDelete(os_mutexHandle_t handle);


#ifdef  __cplusplus
}
#endif

#endif /* OS_MUTEX_H */

/**
 *@}
 **/
