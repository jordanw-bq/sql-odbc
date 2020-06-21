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

#include "es_result_queue.h"

#include <chrono>

#include "es_types.h"

ESResultQueue::ESResultQueue(size_t capacity)
    : m_full_slots(0), m_empty_slots(capacity) {
    // : m_push_semaphore(0), m_pop_semaphore(capacity) {
}

ESResultQueue::~ESResultQueue() {
    while (!m_queue.empty()) {
        delete m_queue.front();
        m_queue.pop();
    }
}

void ESResultQueue::clear() {
    std::scoped_lock lock(m_queue_mutex);
    while (!m_queue.empty()) {
        MYLOG(ES_WARNING, "%s", "0000 Clearing\n");
        delete m_queue.front();
        m_queue.pop();
        m_full_slots.release();
        m_empty_slots.lock();
    }
}

bool ESResultQueue::pop(size_t timeout_ms, ESResult*& result) {
    if (m_full_slots.try_lock_for(timeout_ms)) {
        std::scoped_lock lock(m_queue_mutex);
        MYLOG(ES_WARNING, "%s", "---- Popping\n");
        result = m_queue.front();
        MYLOG(ES_WARNING, "ii Size before: %zd\n", m_queue.size());
        m_queue.pop();
        MYLOG(ES_WARNING, "ii Size after: %zd\n", m_queue.size());
        m_empty_slots.release();
        return true;
    }

    MYLOG(ES_WARNING, "%s", "!!!! Couldn't pop\n");
    return false;
}

bool ESResultQueue::push(size_t timeout_ms, ESResult* result) {
    if (m_empty_slots.try_lock_for(timeout_ms)) {
        std::scoped_lock lock(m_queue_mutex);
        MYLOG(ES_WARNING, "%s", "++++ Pushing\n");
        MYLOG(ES_WARNING, "ii Size before: %zd\n", m_queue.size());
        m_queue.push(result);
        MYLOG(ES_WARNING, "ii Size after: %zd\n", m_queue.size());
        m_full_slots.release();
        return true;
    }

    MYLOG(ES_WARNING, "%s", "!!!! Couldn't push\n");
    return false;
}
