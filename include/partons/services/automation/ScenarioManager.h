#ifndef SCENARIO_MANAGER_H
#define SCENARIO_MANAGER_H

/**
 * @file ScenarioManager.h
 * @author Bryan BERTHOU (SPhN / CEA Saclay)
 * @date January 30, 2014
 * @version 1.0
 */

#include "../../BaseObject.h"

namespace PARTONS {

class Scenario;

/**
 * @class ScenarioManager
 * @brief \<singleton\>
 */
class ScenarioManager: public BaseObject {
public:
    /**
     * A static function that provides a unique pointer of this class
     *
     * @return A unique pointer of this class
     */
    static ScenarioManager* getInstance();

    void delete_();

    void playScenario(const Scenario &scenario) const;

private:
    static ScenarioManager* pInstance; ///< Private pointer of this class for a unique instance

    /**
     * Private default constructor for a unique instance of this class
     */
    ScenarioManager();

    /**
     * Default destructor
     */
    virtual ~ScenarioManager();
};

} /* namespace PARTONS */

#endif /* SCENARIO_MANAGER_H */
