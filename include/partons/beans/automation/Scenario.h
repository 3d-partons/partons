#ifndef SCENARIO_H
#define SCENARIO_H

/**
 * @file Scenario.h
 * @author Bryan BERTHOU (SPhN / CEA Saclay)
 * @date January 30, 2015
 * @version 1.0
 */

#include <stddef.h>
#include <ctime>
#include <string>
#include <vector>

#include "../system/FileObject.h"
#include "Task.h"

namespace PARTONS {

/**
 * @class Scenario
 *
 * @brief Class representing single scenario.
  *
 * This class represents a single scenario, i.e. a set of tasks to be evaluated by services.
 * An object of this class is intended to be created by a xml parser during running PARTONS with a specific xml file, which can be local or can be retrieved from a database.
 * Therefore, this class may be seen as a representation of a single scenario encoded in a xml file, like:
 \code{.py}
 <scenario date="2016-03-25" description="Descriptions allow you to distinguish between many scenarios.">

     <task service="GPDService" method="computeGPDModel" storeInDB="0">

         <kinematics type="GPDKinematic">
             <param name="x" value="0.1" />
             <param name="xi" value="0.05" />
             <param name="t" value="-0.3" />
             <param name="MuF2" value="8." />
             <param name="MuR2" value="8." />
         </kinematics>

         <computation_configuration>
             <module type="GPDModule">
                 <param name="className" value="GK11Model" />
             </module>
         </computation_configuration>
     </task>
 </scenario>
 \endcode
 */
class Scenario: public FileObject {

public:

    /**
     * Default constructor.
     */
    Scenario();

    /**
     * Copy constructor.
     * @param other Object to be copied.
     */
    Scenario(const Scenario &other);

    /**
     * Assignment constructor.
     * @param description Description of this scenario.
     * @param filePath Path to file.
     * @param hashSum Hash sum of file content.
     * @param file String containing file content.
     */
    Scenario(const std::string &description,
            const std::string& filePath,
            const std::string &hashSum, const std::string &file);

    /**
     * Destructor.
     */
    virtual ~Scenario();

    /**
     * Get reference to task of a given id.
     * @param i Id of task to be retrieved.
     * @return Requested task.
     */
    const Task& getTask(unsigned int i) const;

    /**
     * Get reference to task of a given id.
     * @param i Id of task to be retrieved.
     * @return Requested task.
     */
    Task& getTask(unsigned int i);

    /**
     * Add a given task to this scenario.
     * @param task Task to be added.
     */
    void add(const Task &task);

    /**
     *  Return number of tasks in this scenario.
     */
    size_t size() const;

    virtual std::string toString() const;

    //********************************************************
    //*** SETTERS AND GETTERS ********************************
    //********************************************************

    /**
     * Get description of this scenario.
     */
    const std::string& getDescription() const;

    /**
     * Set description of this scenario.
     */
    void setDescription(const std::string& description);

    /**
     * Get vector containing tasks associated to this scenario.
     */
    const std::vector<Task>& getTasks() const;

    /**
     * Set vector containing tasks associated to this scenario.
     */
    void setTasks(const std::vector<Task>& tasks);

private:

    /**
     * Description of this scenario.
     */
    std::string m_description;

    /**
     * Vector containing tasks associated to this scenario.
     */
    std::vector<Task> m_tasks;
};

} /* namespace PARTONS */

#endif /* SCENARIO_H */
