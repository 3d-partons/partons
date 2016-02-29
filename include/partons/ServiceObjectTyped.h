#ifndef SERVICE_OBJECT_TYPED_H
#define SERVICE_OBJECT_TYPED_H

/**
 * @file ServiceObjectTyped.h
 * @author Bryan BERTHOU (SPhN / CEA Saclay)
 * @date 26 February 2016
 * @version 1.0
 *
 * @class ServiceObjectTyped
 *
 * @brief
 */

#include <stddef.h>
#include <SFML/System/Lock.hpp>
#include <SFML/System/Mutex.hpp>
#include <map>
#include <string>

#include "beans/ResultList.h"
#include "ServiceObject.h"
#include "utils/stringUtils/Formatter.h"

template<typename ResultType>
class ServiceObjectTyped: public ServiceObject {
public:
    /**
     * Default constructor
     *
     * @param className
     */
    ServiceObjectTyped(const std::string &className) :
            ServiceObject(className) {
    }

    /**
     * Default destructor
     */
    virtual ~ServiceObjectTyped() {
    }

    //TODO How to handle random computation from thread ? How to index results for later compare ?
    void add(const ResultType &result) {
        sf::Lock lock(m_mutexResultListBuffer); // mutex.lock()

        m_it = m_resultListBuffer.find(result.getComputationModuleName());

        if (m_it != m_resultListBuffer.end()) {
            (m_it->second).add(result);
        } else {
            ResultList<ResultType> tempObservableresultList;
            tempObservableresultList.add(result);
            m_resultListBuffer.insert(
                    std::make_pair(result.getComputationModuleName(),
                            tempObservableresultList));
        }
    } // mutex.unlock()

    void add(const ResultList<ResultType> &resultList) {
        sf::Lock lock(m_mutexResultListBuffer); // mutex.lock()

        for (size_t i = 0; i != resultList.size(); i++) {
            add(resultList[i]);
        }
    } // mutex.unlock()

    ResultList<ResultType> getResultList(const std::string &moduleClassName) {
        sf::Lock lock(m_mutexResultListBuffer); // mutex.lock()

        m_it = m_resultListBuffer.find(moduleClassName);

        if (m_it != m_resultListBuffer.end()) {
            return (m_it->second);
        }

        error(__func__,
                Formatter() << "Missing results for module class name = "
                        << moduleClassName);

    } // mutex.unlock()

    void clearResultListBuffer() {
        sf::Lock lock(m_mutexResultListBuffer); // mutex.lock()

        m_resultListBuffer.clear();
    } // mutex.unlock()

private:
    sf::Mutex m_mutexResultListBuffer;

    // see http://stackoverflow.com/q/22213260
    typename std::map<std::string, ResultList<ResultType> > m_resultListBuffer;
    typename std::map<std::string, ResultList<ResultType> >::iterator m_it;

};

#endif /* SERVICE_OBJECT_TYPED_H */
