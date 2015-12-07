#ifndef SERVICE_OBJECT_H
#define SERVICE_OBJECT_H

/**
 * @file ServiceObject.h
 * @author Bryan BERTHOU (SPhN / CEA Saclay)
 * @date 01 July 2015
 * @version 1.0
 *
 * @class ServiceObject
 *
 * @brief
 */

#include <string>

#include "BaseObject.h"

class Task;

class ServiceObject: public BaseObject {
public:

    ServiceObject(const std::string &className);

    /**
     * Default destructor
     */
    virtual ~ServiceObject();

    virtual void computeTask(Task &task) = 0;
};

#endif /* SERVICE_OBJECT_H */
