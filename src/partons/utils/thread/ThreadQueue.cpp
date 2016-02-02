#include "../../../../include/partons/utils/thread/ThreadQueue.h"

#include <stddef.h>
#include <SFML/System/Lock.hpp>

ThreadQueue::ThreadQueue() {
}

ThreadQueue::~ThreadQueue() {
}

void ThreadQueue::push(const Packet& packet) {
    sf::Lock lock(m_mutex); // mutex.lock()

    m_tasks.push(packet);
} // mutex.unlock()

void ThreadQueue::push(const List<Packet>& listOfPacket) {
    sf::Lock lock(m_mutex); // mutex.lock()

    for (size_t i = 0; i != listOfPacket.size(); i++) {
        m_tasks.push(listOfPacket[i]);
    }
} // mutex.unlock()

Packet ThreadQueue::pop() {
    sf::Lock lock(m_mutex); // mutex.lock()

    Packet packet = m_tasks.front();
    m_tasks.pop();
    return packet;
} // mutex.unlock()

bool ThreadQueue::empty() {
    sf::Lock lock(m_mutex); // mutex.lock()

    return m_tasks.empty();
} // mutex.unlock()
