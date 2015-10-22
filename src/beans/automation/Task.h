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

#include <map>
#include <string>
#include <vector>

#include "../../utils/ParameterList.h"

class Task: public BaseObject {
public:
    Task();
    virtual ~Task();

    void addParameterList(const std::string& className,
            const ParameterList &parameterList);
    const ParameterList& getParameterList(const std::string &className) const;

    const std::string& getFunctionName() const;
    void setFunctionName(const std::string& functionName);
    const std::string& getServiceName() const;
    void setServiceName(const std::string& serviceName);

    bool isAvailableParameterList(const std::string &className);
    ParameterList& getLastAvailableParameterList() const;

    std::vector<ParameterList> getListOfLastAvailableParameterList(
            const std::string &className);

private:
    std::string m_serviceName;
    std::string m_functionName;

    // std::string : className
    // ParameterList : parameters to create object from the class
    std::multimap<std::string, ParameterList> m_functionParameters;
    std::multimap<std::string, ParameterList>::iterator m_it;
};

#endif /* TASK_H */
