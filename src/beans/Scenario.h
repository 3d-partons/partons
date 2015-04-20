#ifndef SCENARIO_H_
#define SCENARIO_H_

/**
 * @file Scenario.h
 * @author Bryan BERTHOU (SPhN / CEA Saclay)
 * @date 30 January 2015
 * @version 1.0
 *
 * @class Scenario
 *
 * @brief
 */

#include <ctime>
#include <map>
#include <string>

class Scenario {
public:
    Scenario();
    virtual ~Scenario();

    void addFunctionArg(std::string argName, void* argValue);

    const std::map<std::string, void*>& getFunctionArgs() const;
    void setFunctionArgs(const std::map<std::string, void*>& functionArgs);
    const std::string& getFunctionName() const;
    void setFunctionName(const std::string& functionName);
    time_t getDate() const;
    void setDate(time_t date);
    const std::string& getDescription() const;
    void setDescription(const std::string& description);
    const std::string& getId() const;
    void setId(const std::string& id);
    const std::string& getServiceName() const;
    void setServiceName(const std::string& serviceName);

private:

    std::string m_id;
    time_t m_date;
    std::string m_description;

    std::string m_serviceName;
    std::string m_functionName;
    std::map<std::string, void*> m_functionArgs;
};

#endif /* SCENARIO_H_ */
