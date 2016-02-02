#ifndef THREAD_H
#define THREAD_H

/**
 * @file Thread.h
 * @author Bryan BERTHOU (CEA Saclay)
 * @author <contibutor> http://vichargrave.com/java-style-thread-class-in-c/
 * @date 10 September 2014
 *
 * @class Thread
 */

namespace sf {
class Thread;
} /* namespace sf */

class Thread {
public:
    /**
     * Default constructor
     */
    Thread();

    /**
     * Default destructor
     */
    virtual ~Thread();

    //TODO change it or suppress it
    virtual Thread* clone() const;

    /**
     * Main function executed by the thread
     */
    virtual void run();

    void launch();
    void wait();

protected:
    Thread(const Thread &other);

private:
    sf::Thread* m_pThread;
};
#endif /* THREAD_H */

