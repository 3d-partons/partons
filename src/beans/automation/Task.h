/*
 * Task.h
 *
 *  Created on: Jul 3, 2015
 *      Author: Bryan BERTHOU
 */

#ifndef TASK_H
#define TASK_H

#include <map>
#include <string>

#include "../../utils/ParameterList.h"

/*
 *
 */
class Task: public BaseObject {
public:
    Task();
    virtual ~Task();

    const ParameterList& getParameterList(const std::string &className) const;

    const std::string& getFunctionName() const;
    void setFunctionName(const std::string& functionName);
    const std::map<std::string, ParameterList>& getFunctionParameters() const;
    void setFunctionParameters(
            const std::map<std::string, ParameterList>& functionParameters);
    const std::string& getServiceName() const;
    void setServiceName(const std::string& serviceName);

private:
    std::string m_serviceName;
    std::string m_functionName;
    // std::string : className
    // ParameterList : parameters to create object from the class
    std::map<std::string, ParameterList> m_functionParameters;
};

#endif /* TASK_H */
