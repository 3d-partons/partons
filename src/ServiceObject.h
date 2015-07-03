/*
 * ServiceObject.h
 *
 *  Created on: Jul 1, 2015
 *      Author: Bryan BERTHOU
 */

#ifndef SERVICE_OBJECT_H
#define SERVICE_OBJECT_H

#include <string>

#include "BaseObject.h"

class Scenario;

/*
 *
 */
class ServiceObject: public BaseObject {
public:

    ServiceObject(const std::string &className);

    /**
     * Default destructor
     */
    virtual ~ServiceObject();

    virtual void computeScenario(Scenario scenario) = 0;
};

#endif /* SERVICE_OBJECT_H */
