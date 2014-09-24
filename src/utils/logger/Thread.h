#ifndef THREAD_H
#define THREAD_H

/**
 * @author Bryan BERTHOu (CEA Saclay)
 * @author <contibutor> http://vichargrave.com/java-style-thread-class-in-c/
 * @date 10 September 2014
 */

#include <pthread.h>

class Thread {
public:
    Thread();
    virtual ~Thread();

    int start();
    virtual void* run() = 0;

    pthread_t getThreadId();
private:

    pthread_t m_tid;
    int m_running;
    int m_detached;
};
#endif /* THREAD_H */
