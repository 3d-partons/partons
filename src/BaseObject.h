#ifndef BASE_OBJECT_H
#define BASE_OBJECT_H

/**
 * @file BaseObject.h
 * @author Bryan BERTHOU (SPhN / CEA Saclay)
 * @date 09 September 2014
 * @version 1.0
 *
 * @class BaseObject
 *
 * @brief
 */

#include <string>

class LoggerManager;

class BaseObject {
public:

    /**
     * Constructor.
     * Takes a string parameter that represents the class's name of the child instance class.
     * Class's name value is set automatically by the child constructor with typeid(*this).name() method.
     *
     * @param className class's name of child class.
     */
    BaseObject(const std::string &className);

    /**
     * Default destructor
     */
    virtual ~BaseObject();

    /**
     * Virtual clone function to allow factory to copy all derived members
     * @return
     */
    virtual BaseObject* clone() const;

    //TODO voir si on en a besoin pour gérer la durée de vie des pointeurs membres des sous classes
    //virtual void destroy() = 0;

    /**
     * Return a pre-formatted characters string for output visualization of class member's values
     *
     * @return a pre-formatted characters string
     */
    virtual std::string toString();

    // ##### GETTERS & SETTERS #####

    unsigned int getObjectId() const;
    void setObjectId(unsigned int objectId);
    const std::string& getClassName() const;
    void setClassName(const std::string& className);

protected:
    LoggerManager* m_pLoggerManager; ///< Pointer to LoggerManager class

    /**
     * Copy constructor
     *
     * @param other
     */
    BaseObject(const BaseObject& other);

    void throwException(const std::string &functionName,
            const std::string &errorMessage);

private:
    static unsigned int uniqueID;

    unsigned int m_objectId;
    std::string m_className; ///< String that represents class's name used by the LoggerManager's class for know the source of the output trace

    /**
     * On call, returns an incremented by one uniqueID member value.
     *
     * @return incremented unsigned int ID.
     */
    unsigned int getUniqueID();
};

#endif /* BASE_OBJECT_H */
