#ifndef SERVICE_H
#define SERVICE_H

/**
 * @file Service.h
 * @author Bryan BERTHOU (SPhN / CEA Saclay)
 * @date 14 april 2015
 *
 * @class Service
 * @brief
 */

#include <string>

#include "../BaseObject.h"

class Scenario;

class Service: public BaseObject {
public:

    Service(const std::string &className);

    /**
     * Default destructor
     */
    virtual ~Service();

    virtual void computeScenario(Scenario scenario) = 0;

};

#endif /* SERVICE_H */
