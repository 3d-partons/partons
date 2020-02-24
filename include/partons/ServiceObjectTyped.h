#ifndef SERVICE_OBJECT_TYPED_H
#define SERVICE_OBJECT_TYPED_H

/**
 * @file ServiceObjectTyped.h
 * @author Bryan BERTHOU (SPhN / CEA Saclay)
 * @date February 26, 2016
 * @version 1.0
 */

#include <ElementaryUtils/string_utils/StringUtils.h>
#include <stddef.h>
#include <SFML/System/Lock.hpp>
#include <SFML/System/Mutex.hpp>
#include <string>

#include "beans/automation/Scenario.h"
#include "beans/automation/Task.h"
#include "beans/List.h"
#include "beans/system/ResultInfo.h"
#include "ResourceManager.h"
#include "ServiceObject.h"

//TODO How to handle random computation from thread ? How to index results for later compare ?

namespace PARTONS {

/** @class ServiceObjectTyped
 *
 * @brief Abstract class for a service.
 */
template<typename KinematicType, typename ResultType>
class ServiceObjectTyped: public ServiceObject {

public:

    static const std::string SERVICE_OBJECT_PRINT_RESULTS; ///< Name of the XML task used to print results via Logger.

    /**
     * Destructor.
     */
    virtual ~ServiceObjectTyped() {
    }

    virtual void resolveObjectDependencies() {
        ServiceObject::resolveObjectDependencies();
    }

    /**
     * Add single result to result list.
     */
    void add(const ResultType &result) {

        sf::Lock lock(m_mutexResultListBuffer); // mutex.lock()
        m_resultListBuffer.add(result);
    } // mutex.unlock()

    /**
     * Add many results to result list.
     */
    void add(const List<ResultType> &resultList) {

        sf::Lock lock(m_mutexResultListBuffer); // mutex.lock()

        for (size_t i = 0; i != resultList.size(); i++) {
            m_resultListBuffer.add(resultList[i]);
        }
    } // mutex.unlock()

    /**
     * Sort result list.
     */
    void sortResultList() {

        sf::Lock lock(m_mutexResultListBuffer); // mutex.lock()
        m_resultListBuffer.sort();
    } // mutex.unlock()

    /**
     * Get result list.
     */
    List<ResultType>& getResultList() {

        sf::Lock lock(m_mutexResultListBuffer); // mutex.lock()
        return m_resultListBuffer;
    } // mutex.unlock()

    /**
     * Clear result list.
     */
    void clearResultListBuffer() {

        sf::Lock lock(m_mutexResultListBuffer); // mutex.lock()
        m_resultListBuffer.clear();
    } // mutex.unlock()

    /**
     * Clear kinematic list.
     */
    void clearKinematicListBuffer() {

        sf::Lock lock(m_mutexKinematicList); // mutex.lock()
        m_kinematicListBuffer.clear();
    } // mutex.unlock()

    /**
     * Compute scenario.
     */
    List<ResultType> computeScenario(Scenario& scenario) {

        List<ResultType> resultList;

        for (size_t i = 0; i != scenario.size(); i++) {
            computeTask(scenario.getTask(i));
        }

        resultList = getResultList();
        clearResultListBuffer();

        return resultList;
    }

    /**
     * Flush result list.
     */
    List<ResultType> flushResultList() {

        sf::Lock lock(m_mutexResultListBuffer); // mutex.lock()

        List<ResultType> resultList = m_resultListBuffer;
        m_resultListBuffer.clear();

        return resultList;
    } // mutex.unlock()

    /**
     * Compute task.
     */
    virtual void computeTask(Task &task) {

        m_resultInfo = ResultInfo();

        m_resultInfo.setScenarioTaskIndexNumber(
                task.getScenarioTaskIndexNumber());

        Scenario * tempSenario =
                ResourceManager::getInstance()->registerScenario(
                        task.getScenario());

        if (tempSenario) {
            m_resultInfo.setScenarioHashSum(tempSenario->getHashSum());
        }
    }

protected:

    /**
     * Default constructor.
     */
    ServiceObjectTyped(const std::string &className) :
            ServiceObject(className), m_batchSize(1000) {
    }

    unsigned int m_batchSize; ///< Batch size.

    sf::Mutex m_mutexKinematicList; ///< Mutex for kinematic list.
    sf::Mutex m_mutexResultListBuffer; ///< Mutex for result list.

    List<KinematicType> m_kinematicListBuffer; ///< Kinematic list.
    List<ResultType> m_resultListBuffer; ///< Result list.

    ResultInfo m_resultInfo; ///< Result info.

    /**
     * General tasks.
     */
    bool computeGeneralTask(Task &task) {

        bool isEvaluated = false;

        if (ElemUtils::StringUtils::equals(task.getFunctionName(),
                SERVICE_OBJECT_PRINT_RESULTS)) {

            printResultListBuffer();
            isEvaluated = true;
        }

        return isEvaluated;
    }

    /**
     * Print results to buffer.
     */
    void printResultListBuffer() {
        for (unsigned int i = 0; i != m_resultListBuffer.size(); i++) {
            info(__func__, m_resultListBuffer[i].toString());
        }
    }

    /**
     * Update result info.
     */
    void updateResultInfo(ResultType &result, const ResultInfo &resultInfo) {
        result.setResultInfo(resultInfo);
    }

    /**
     * Update result info.
     */
    void updateResultInfo(List<ResultType> &resultList,
            const ResultInfo &resultInfo) {
        for (size_t i = 0; i != resultList.size(); i++) {
            updateResultInfo(resultList[i], resultInfo);
        }
    }
};

template<typename KinematicType, typename ResultType>
const std::string ServiceObjectTyped<KinematicType, ResultType>::SERVICE_OBJECT_PRINT_RESULTS =
        "printResults";

} /* namespace PARTONS */

#endif /* SERVICE_OBJECT_TYPED_H */
