#ifndef THREAD_QUEUE_H
#define THREAD_QUEUE_H

/**
 * @file ThreadQueue.h
 * @author Bryan BERTHOU (CEA Saclay)
 * @date 14 January 2015
 * @version 1.0
 *
 * @class ThreadQueue
 *
 * @brief
 */

#include <ElementaryUtils/thread/Packet.h>
#include <SFML/System/Mutex.hpp>
#include <queue>

#include "../../beans/List.h"

class ThreadQueue {
public:
    ThreadQueue();
    virtual ~ThreadQueue();

    void push(const ElemUtils::Packet &packet);
    void push(const List<ElemUtils::Packet> &listOfPacket);

    ElemUtils::Packet pop();

    bool empty();

private:
    std::queue<ElemUtils::Packet> m_tasks;

    //pthread_mutex_t m_mutex;
    sf::Mutex m_mutex;
};

#endif /* THREAD_QUEUE_H */
