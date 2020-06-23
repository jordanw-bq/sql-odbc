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

#ifndef ES_SEMAPHORE
#define ES_SEMAPHORE

#ifdef WIN32
#include <windows.h>
#endif
#include <mutex>
#include <thread>

class es_sem {
   public:
    es_sem(size_t initial_count, size_t capacity);
    ~es_sem();

    void lock();
    void release();
    bool try_lock_for(size_t timeout_ms);
    bool try_peek_for(size_t timeout_ms);

   private:
    size_t m_count;
    const size_t m_maxCount;
    std::mutex m_mutex;
    std::condition_variable m_signal;
};

#endif