/*
 * Copyright <2019> Amazon.com, Inc. or its affiliates. All Rights Reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License").
 * You may not use this file except in compliance with the License.
 * A copy of the License is located at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * or in the "license" file accompanying this file. This file is distributed
 * on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either
 * express or implied. See the License for the specific language governing
 * permissions and limitations under the License.
 *
 */

#include "es_semaphore.h"

#include <chrono>

es_semaphore::es_semaphore(size_t capacity)
    :
#if defined(WIN32) || defined(WIN64)
      m_semaphore(CreateSemaphore(NULL, capacity, capacity, NULL))
#else
      m_semaphore(capacity)
#endif
{
}

es_semaphore::~es_semaphore() {
#if defined(WIN32) || defined(WIN64)
    CloseHandle(m_semaphore);
#endif
}

void es_semaphore::lock() {
#if defined(WIN32) || defined(WIN64)
    WaitForSingleObject(m_semaphore, INFINITE);
#else
    m_semaphore.lock();
#endif
}

void es_semaphore::release() {
#if defined(WIN32) || defined(WIN64)
    ReleaseSemaphore(m_semaphore, 1, NULL);
#else
    m_semaphore.release();
#endif
}

bool es_semaphore::try_lock_for(size_t timeout_ms) {
#if defined(WIN32) || defined(WIN64)
    return WaitForSingleObject(m_semaphore, timeout_ms) == WAIT_OBJECT_0;
#else
    return m_semaphore.try_lock_for(std::chrono::milliseconds(timeout_ms));
#endif
}
