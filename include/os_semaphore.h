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
 * @defgroup os_semaphore Semaphores
 * @{
 * @ingroup os
 *
 * @brief FreeRTOS semaphore abstraction layer
 *
 */

#ifndef OS_SEMAPHORE_H
#define OS_SEMAPHORE_H

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "FreeRTOS.h"
#include "semphr.h"

#ifdef  __cplusplus
extern "C" {
#endif

typedef struct {
    uint8_t initCount;      /**<Initial value of a counting semaphore*/
    uint8_t maxCount;       /**<Maximum value of a counting semaphore*/
    bool binary;            /**<If the semaphore should be binary or not*/
} os_semConfig_t;

typedef SemaphoreHandle_t os_semHandle_t;

/**
 * @brief Create a new semaphore object
 * @details Creates a semaphore object and a handle to it later
 * used for waiting/posting.
 * @param conf Configuration struct for the new semaphore
 * @return Handle to the new semaphore object.
 */
os_semHandle_t os_semNew(os_semConfig_t *conf);

/**
 * @brief Decrement the value of a semaphore.
 * @details This function blocks forever until it can decrement
 * the value of a semaphore.
 * @param handle Handle to the semaphore to decrement.
 * @retval  true    If the semaphore was decremented.
 * @retval  false   If the semaphore could not be decremented.
 */
bool os_semWait(os_semHandle_t handle);

/**
 * @brief Try to decrement the value of a semaphore.
 * @details This is a non-blocking function to decrement the value of a semaphore.
 * If the semaphore could not be decremented, it returns immediately.
 * @param handle Handle to the semaphore to decrement.
 * @retval  true If the semaphore was decremented.
 * @retval  false  If the semaphore could not be decremented.
 */
bool os_semTryWait(os_semHandle_t handle);

/**
 * @brief Try to decrement the value of a semaphore with a timeout.
 * @details Use this function to decrement the value of a semaphore with a timeout.
 * @param handle Handle to the semaphore to decrement.
 * @param timeout Time to block until the semaphore can decrement the value.
 * @retval  true If the value was successfully decremented.
 * @retval  false If the value could not be decremented or the semaphore timed out.
 */
bool os_semTimedWait(os_semHandle_t handle, uint32_t timeout);

/**
 * @brief Try to decrement the value of a semaphore from an interrupt service 
 * routine.
 * @details Use this function to decrement the value of a semaphore from an ISR
 * context. This function is promised to be ISR safe, the other ones not.
 * @param handle Handle to the semaphore to decrement.
 * @retval  true If the semaphore was successfully decremented.
 * @retval  false If the value could not be decremented or a higher priority
 * task has woken.
 */
bool os_semIsrWait(os_semHandle_t handle);

/**
 * @brief Increment the value of a semaphore.
 * @param handle Handle to the semaphore to increment.
 */
void os_semPost(os_semHandle_t handle);

/**
 * @brief Increase the value of a semaphore from an interrupt service routine.
 * @details Use this function to increment the value of a semaphore from an ISR
 * context. This function is promised to be ISR safe, the other ones not.
 * @param handle Handle to the semaphore to increment.
 * @retval  true If the semaphore was successfully incremented.
 * @retval  false If the value could not be incremented or a higher priority
 * task has woken.
 */
bool os_semIsrPost(os_semHandle_t handle);

/**
 * @brief Delete a semaphore object.
 * @details Delete a semaphore object created with os_semNew.
 * @param handle Handle to the semaphore object to delete.
 */
void os_semDelete(os_semHandle_t handle);

#ifdef  __cplusplus
}
#endif

#endif /* OS_SEMAPHORE_H */

/**
 *@}
 **/
