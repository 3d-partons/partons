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

#include <pthread.h>
#include <queue>

#include "../../beans/List.h"
#include "Packet.h"

class ThreadQueue {
public:
    ThreadQueue();
    virtual ~ThreadQueue();

    void push(const Packet &packet);
    void push(const List<Packet> &listOfPacket);

    Packet pop();

    bool empty() const;

private:
    std::queue<Packet> m_tasks;

    pthread_mutex_t m_mutex;
};

#endif /* THREAD_QUEUE_H */
