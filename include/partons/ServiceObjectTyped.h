#ifndef SERVICE_OBJECT_TYPED_H
#define SERVICE_OBJECT_TYPED_H

/**
 * @file ServiceObjectTyped.h
 * @author Bryan BERTHOU (SPhN / CEA Saclay)
 * @date February 26, 2016
 * @version 1.0
 */

#include <ElementaryUtils/PropertiesManager.h>
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

/** @class ServiceObjectTyped
 *
 * @brief
 */
template<typename KinematicType, typename ResultType>
class ServiceObjectTyped: public ServiceObject {
public:
    static const std::string SERVICE_OBJECT_PRINT_RESULTS;

    /**
     * Default constructor.
     *
     * @param className
     */
    ServiceObjectTyped(const std::string &className) :
            ServiceObject(className), m_batchSize(1000) {
    }

    /**
     * Default destructor.
     */
    virtual ~ServiceObjectTyped() {
    }

    //TODO How to handle random computation from thread ? How to index results for later compare ?
    void add(const ResultType &result) {
        sf::Lock lock(m_mutexResultListBuffer); // mutex.lock()

        m_resultListBuffer.add(result);
    } // mutex.unlock()

    void add(const List<ResultType> &resultList) {
        sf::Lock lock(m_mutexResultListBuffer); // mutex.lock()

        for (size_t i = 0; i != resultList.size(); i++) {
            m_resultListBuffer.add(resultList[i]);
        }
    } // mutex.unlock()

    void sortResultList() {
        sf::Lock lock(m_mutexResultListBuffer); // mutex.lock()

        m_resultListBuffer.sort();
    } // mutex.unlock()

    List<ResultType>& getResultList() {
        sf::Lock lock(m_mutexResultListBuffer); // mutex.lock()

        return m_resultListBuffer;
    } // mutex.unlock()

    void clearResultListBuffer() {
        sf::Lock lock(m_mutexResultListBuffer); // mutex.lock()

        m_resultListBuffer.clear();
    } // mutex.unlock()

    void clearKinematicListBuffer() {
        sf::Lock lock(m_mutexKinematicList); // mutex.lock()

        m_kinematicListBuffer.clear();
    } // mutex.unlock()

    List<ResultType> computeScenario(Scenario& scenario) {
        List<ResultType> resultList;

        for (size_t i = 0; i != scenario.size(); i++) {
            computeTask(scenario.getTask(i));
        }

        resultList = getResultList();
        clearResultListBuffer();

        return resultList;
    }

    List<ResultType> flushResultList() {
        sf::Lock lock(m_mutexResultListBuffer); // mutex.lock()

        List<ResultType> resultList = m_resultListBuffer;
        m_resultListBuffer.clear();

        return resultList;
    } // mutex.unlock()

    /**
     * Method used in automation to compute given tasks.
     * Method called when overwritten.
     * @param task Automation task to compute.
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
    unsigned int m_batchSize;

    sf::Mutex m_mutexKinematicList;
    sf::Mutex m_mutexResultListBuffer;

    List<KinematicType> m_kinematicListBuffer;
    List<ResultType> m_resultListBuffer;

    ResultInfo m_resultInfo;

    bool computeGeneralTask(Task &task) {
        bool isEvaluated = false;
        if (ElemUtils::StringUtils::equals(task.getFunctionName(),
                SERVICE_OBJECT_PRINT_RESULTS)) {
            printResultListBuffer();
            isEvaluated = true;
        }

        return isEvaluated;
    }

    void printResultListBuffer() {
        for (unsigned int i = 0; i != m_resultListBuffer.size(); i++) {
            info(__func__, m_resultListBuffer[i].toString());
        }
    }

    void updateResultInfo(ResultType &result, const ResultInfo &resultInfo) {
        result.setResultInfo(resultInfo);
    }

    void updateResultInfo(List<ResultType> &resultList,
            const ResultInfo &resultInfo) {
        debug(__func__, "Processing ...");

        for (size_t i = 0; i != resultList.size(); i++) {
            updateResultInfo(resultList[i], resultInfo);
        }
    }
};

template<typename KinematicType, typename ResultType>
const std::string ServiceObjectTyped<KinematicType, ResultType>::SERVICE_OBJECT_PRINT_RESULTS =
        "printResults";

#endif /* SERVICE_OBJECT_TYPED_H */
