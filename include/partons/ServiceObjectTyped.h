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
#include <string>

#include "beans/automation/Scenario.h"
#include "beans/List.h"
#include "beans/ResultList.h"
#include "services/automation/AutomationService.h"
#include "ServiceObject.h"

template<typename KinematicType, typename ResultType>
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

    ResultList<ResultType> computeScenario(const std::string &scenarioFilePath,
            List<KinematicType> &kinematicList) {
        Scenario scenario = m_pAutomationService->parseXMLFile(
                scenarioFilePath);

        m_kinematicListBuffer.add(kinematicList);

        for (size_t i = 0; i != scenario.size(); i++) {
            computeTask(scenario.getTask(i));
        }

        return flushResultList();
    }

    //TODO How to handle random computation from thread ? How to index results for later compare ?
    void add(const ResultType &result) {
        sf::Lock lock(m_mutexResultListBuffer); // mutex.lock()

        m_resultListBuffer.add(result);
    } // mutex.unlock()

    void add(const ResultList<ResultType> &resultList) {
        sf::Lock lock(m_mutexResultListBuffer); // mutex.lock()

        for (size_t i = 0; i != resultList.size(); i++) {
            m_resultListBuffer.add(resultList[i]);
        }
    } // mutex.unlock()

    ResultList<ResultType> getResultList() {
        sf::Lock lock(m_mutexResultListBuffer); // mutex.lock()

        return m_resultListBuffer;
    } // mutex.unlock()

    void clearResultListBuffer() {
        sf::Lock lock(m_mutexResultListBuffer); // mutex.lock()

        m_resultListBuffer.clear();
    } // mutex.unlock()

    ResultList<ResultType> computeScenario(Scenario& scenario) {
        ResultList<ResultType> resultList;

        for (size_t i = 0; i != scenario.size(); i++) {
            computeTask(scenario.getTask(i));
        }

        resultList = getResultList();
        clearResultListBuffer();

        return resultList;
    }

    ResultList<ResultType> flushResultList() {
        sf::Lock lock(m_mutexResultListBuffer); // mutex.lock()

        ResultList<ResultType> resultList = m_resultListBuffer;
        m_resultListBuffer.clear();

        return resultList;
    } // mutex.unlock()

protected:
    sf::Mutex m_mutexKinematicList;
    sf::Mutex m_mutexResultListBuffer;

    List<KinematicType> m_kinematicListBuffer;
    ResultList<ResultType> m_resultListBuffer;

};

#endif /* SERVICE_OBJECT_TYPED_H */
