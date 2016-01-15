#include "../../../../include/partons/utils/thread/ThreadQueue.h"

#include <stddef.h>

ThreadQueue::ThreadQueue() {
    pthread_mutex_init(&m_mutex, NULL);
}

ThreadQueue::~ThreadQueue() {
    pthread_mutex_destroy(&m_mutex);
}

void ThreadQueue::push(const Packet& packet) {
    pthread_mutex_lock(&m_mutex);
    m_tasks.push(packet);
    pthread_mutex_unlock(&m_mutex);
}

void ThreadQueue::push(const List<Packet>& listOfPacket) {
    pthread_mutex_lock(&m_mutex);
    for (size_t i = 0; i != listOfPacket.size(); i++) {
        m_tasks.push(listOfPacket[i]);
    }
    pthread_mutex_unlock(&m_mutex);
}

Packet ThreadQueue::pop() {
    pthread_mutex_lock(&m_mutex);
    Packet packet = m_tasks.front();
    m_tasks.pop();
    pthread_mutex_unlock(&m_mutex);
    return packet;
}

bool ThreadQueue::empty() const {
    return m_tasks.empty();
}
