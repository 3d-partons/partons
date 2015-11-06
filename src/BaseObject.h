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

    virtual void init();

    /**
     * Return a pre-formatted characters string for output visualization of class member's values
     *
     * @return a pre-formatted characters string
     */
    //TODO propagade const every daugther class
    virtual std::string toString() const;

    // ##### GETTERS & SETTERS #####

    const std::string& getClassName() const;
    unsigned int getObjectId() const;
    void setObjectId(unsigned int objectId);

protected:
    /**
     * Copy constructor
     *
     * @param other
     */
    BaseObject(const BaseObject& other);

    void throwException(const std::string functionName,
            const std::string errorMessage) const;

    void info(const std::string &functionName,
            const std::string &message) const;

    void debug(const std::string &functionName,
            const std::string &message) const;

    void warn(const std::string &functionName,
            const std::string &message) const;

    void error(const std::string &functionName,
            const std::string &message) const;

private:
    LoggerManager* m_pLoggerManager; ///< Pointer to LoggerManager class

    unsigned int m_objectId;
    std::string m_className; ///< String that represents class's name used by the LoggerManager's class for know the source of the output trace

    static unsigned int m_uniqueObjectIdCounter;

    /**
     * On call, returns an incremented by one uniqueID member value.
     *
     * @return incremented unsigned int ID.
     */
    unsigned int getUniqueObjectId();
};

#endif /* BASE_OBJECT_H */
