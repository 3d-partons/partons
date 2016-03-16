#ifndef DATABASE_OBJECT_H
#define DATABASE_OBJECT_H

/**
 * @file DatabaseObject.h
 * @author: Bryan BERTHOU (SPhN / CEA Saclay)
 * @date 11 March 2016
 * @version 1.0
 *
 * @class DatabaseObject
 *
 * @brief
 */

#include <string>

#include "../BaseObject.h"

class DatabaseObject: public BaseObject {
public:
    DatabaseObject(const std::string &className);
    DatabaseObject(const std::string &className, int indexId);
    virtual ~DatabaseObject();

    // ##### GETTERS & SETTERS #####

    int getIndexId() const;
    void setIndexId(int indexId);

private:
    int m_indexId;
};

#endif /* DATABASE_OBJECT_H */
