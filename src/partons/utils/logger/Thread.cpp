#include "../../../../include/partons/utils/logger/Thread.h"

#include <stddef.h>

static void* runThread(void* arg) {
    return ((Thread*) arg)->run();
}

Thread::Thread() :
        m_tid(0), m_running(0), m_detached(0) {
}

Thread::~Thread() {
    if (m_running == 1 && m_detached == 0) {
        pthread_detach(m_tid);
    }
    if (m_running == 1) {
        pthread_cancel(m_tid);
    }
}

int Thread::start() {
    // Initialize and set thread joinable
    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);

    int result = pthread_create(&m_tid, NULL, runThread, this);
    if (result == 0) {
        m_running = 1;
    }

    // free attribute
    pthread_attr_destroy(&attr);

    return result;
}

pthread_t Thread::getThreadId() {
    return m_tid;
}
