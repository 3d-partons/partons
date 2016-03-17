#ifndef TASK_H
#define TASK_H

/**
 * @file Task.h
 * @author Bryan BERTHOU (SPhN / CEA Saclay)
 * @date 03 July 2015
 * @version 1.0
 *
 * @class Task
 * @brief
 */

#include <ElementaryUtils/parameters/Parameters.h>
#include <map>
#include <string>
#include <vector>

#include "../../BaseObject.h"

class Task: public BaseObject {
public:
    Task();
    virtual ~Task();

    void addParameters(const std::string& className,
            const ElemUtils::Parameters &parameters);
    const ElemUtils::Parameters& getParameters(
            const std::string &className) const;

    const std::string& getFunctionName() const;
    void setFunctionName(const std::string& functionName);
    const std::string& getServiceName() const;
    void setServiceName(const std::string& serviceName);

    bool isAvailableParameters(const std::string &className);
    ElemUtils::Parameters& getLastAvailableParameters() const;

    std::vector<ElemUtils::Parameters> getListOfLastAvailableParameters(
            const std::string &className);

    bool isStoreInDB() const;
    void setStoreInDB(bool storeInDb);

private:
    std::string m_serviceName;
    std::string m_functionName;
    bool m_storeInDB;

    // std::string : className
    // ParameterList : parameters to create object from the class
    std::multimap<std::string, ElemUtils::Parameters> m_functionParameters;
    mutable std::multimap<std::string, ElemUtils::Parameters>::iterator m_it;
};

#endif /* TASK_H */
