#ifndef DATABASE_OBJECT_H
#define DATABASE_OBJECT_H

/**
 * @file DatabaseObject.h
 * @author: Bryan BERTHOU (SPhN / CEA Saclay)
 * @date March 11, 2016
 * @version 1.0
 */

#include <string>

#include "../BaseObject.h"

/**
 * @class DatabaseObject
 *
 * @brief
 */
class DatabaseObject: public BaseObject {
public:
    /**
     * Constructor
     *
     * @param className
     */
    DatabaseObject(const std::string &className);

    /**
     * Constructor
     *
     * @param className
     * @param indexId
     */
    DatabaseObject(const std::string &className, int indexId);

    /**
     * Destructor
     */
    virtual ~DatabaseObject();

    // ##### GETTERS & SETTERS #####

    int getIndexId() const;
    void setIndexId(int indexId);

private:
    int m_indexId; ///<
};

#endif /* DATABASE_OBJECT_H */
