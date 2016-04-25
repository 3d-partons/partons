#ifndef SCENARIO_H
#define SCENARIO_H

/**
 * @file Scenario.h
 * @author Bryan BERTHOU (SPhN / CEA Saclay)
 * @date January 30, 2015
 * @version 1.0
 */

#include <stddef.h>
#include <string>
#include <vector>

#include "../../database/DatabaseFileObject.h"
#include "Task.h"

/**
 * @class Scenario
 *
 * @brief
 */
class Scenario: public DatabaseFileObject {
public:
    /**
     * Default constructor
     */
    Scenario();

    /**
     * Destructor
     */
    virtual ~Scenario();

    const Task& getTask(unsigned int i) const;
    Task& getTask(unsigned int i);

    void add(const Task &task);

    /**
     * Return the number of tasks in this scenario.
     *
     * @return
     */
    size_t size() const;

    const std::string& getDescription() const;
    void setDescription(const std::string& description);

    virtual std::string toString() const;

    const std::vector<Task>& getTasks() const;
    void setTasks(const std::vector<Task>& tasks);

private:
    std::string m_description; ///<
    std::vector<Task> m_tasks; ///<
};

#endif /* SCENARIO_H */
