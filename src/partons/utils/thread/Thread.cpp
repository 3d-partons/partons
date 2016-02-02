#include "/usr/local/sfml/v2.3.2/include/SFML/System/Thread.hpp"
#include "../../../../include/partons/utils/thread/Thread.h"

#include <SFML/System/Thread.hpp>

Thread::Thread() :
        m_pThread(new sf::Thread(&Thread::run, this)) {
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
}

void Thread::wait() {
    m_pThread->wait();
}

Thread::Thread(const Thread& other) {
    m_pThread = new sf::Thread(&Thread::run, this);
}
