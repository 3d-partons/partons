/*
 * BaseObjectFactory.h
 *
 *  Created on: Jun 26, 2015
 *      Author: Bryan BERTHOU
 */

#ifndef BASE_OBJECT_FACTORY_H_
#define BASE_OBJECT_FACTORY_H

#include <string>

class BaseObject;
class BaseObjectRegistry;

/*
 *
 */
class BaseObjectFactory {
public:

    static BaseObject* newBaseObject(unsigned int classId);
    static BaseObject* newBaseObject(const std::string &className);

private:

    static BaseObjectRegistry* m_pBaseObjectregistry;
};

#endif /* BASE_OBJECT_FACTORY_H */
