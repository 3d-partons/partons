#ifndef SCENARIO_H
#define SCENARIO_H

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

#include <stddef.h>
#include <ctime>
#include <string>
#include <vector>

#include "Task.h"

class Scenario {
public:
    Scenario();
    virtual ~Scenario();

    const Task& getTask(unsigned int i) const;
    Task& getTask(unsigned int i);

    void add(const Task &task);

    size_t size() const;

    time_t getDate() const;
    void setDate(time_t date);
    const std::string& getDescription() const;
    void setDescription(const std::string& description);
    const std::string& getId() const;
    void setId(const std::string& id);
private:

    std::string m_id;
    time_t m_date;
    std::string m_description;

    std::vector<Task> m_tasks;
};

#endif /* SCENARIO_H */
