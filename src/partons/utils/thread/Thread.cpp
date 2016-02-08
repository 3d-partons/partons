#include "../../../../include/partons/utils/thread/Thread.h"

#include <SFML/System/Thread.hpp>

Thread::Thread() :
        m_pThread(new sf::Thread(&Thread::run, this))/*, m_isRunning(false)*/{
}

Thread::~Thread() {
    if (m_pThread) {
        delete m_pThread;
        m_pThread = 0;
    }
}

Thread* Thread::clone() const {
    return new Thread(*this);
}

void Thread::run() {
    //TODO throw exception : must be implemented in daugther class
}

void Thread::launch() {
    m_pThread->launch();
    // m_isRunning = true;
}

void Thread::wait() {
    // if (m_isRunning) {
    // m_pThread->wait();
    // }

    m_pThread->wait();

}

Thread::Thread(const Thread& other) {
    m_pThread = new sf::Thread(&Thread::run, this);
}
