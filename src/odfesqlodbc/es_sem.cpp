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

#include "es_sem.h"

#include <chrono>

es_sem::es_sem(size_t initialCount, size_t maxCount)
    : m_count(initialCount), m_maxCount(maxCount) {
}

es_sem::~es_sem() {
}

void es_sem::lock() {
    std::unique_lock< std::mutex > locker(m_mutex);
    if (0 == m_count) {
        m_signal.wait(locker, [this] { return m_count > 0; });
    }
    --m_count;
}

void es_sem::release() {
    std::lock_guard< std::mutex > locker(m_mutex);
    m_count = (std::min)(m_maxCount, m_count + 1);
    m_signal.notify_one();
}

bool es_sem::try_lock_for(size_t timeout_ms) {
    std::unique_lock< std::mutex > locker(m_mutex);
    if (0 == m_count) {
        if (m_signal.wait_for(locker, std::chrono::milliseconds(timeout_ms),
                              [this] { return m_count > 0; })) {
            --m_count;
            return true;
        } else {
            return false;
        }
    }
    --m_count;
    return true;
}

bool es_sem::try_peek_for(size_t timeout_ms) {
    std::unique_lock< std::mutex > locker(m_mutex);
    if (0 == m_count) {
        if (m_signal.wait_for(locker, std::chrono::milliseconds(timeout_ms),
                              [this] { return m_count > 0; })) {
            return true;
        } else {
            return false;
        }
    }
    return true;
}
