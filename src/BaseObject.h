#ifndef BASE_OBJECT_H
#define BASE_OBJECT_H

/**
 * @author Bryan BERTHOU (CEA Saclay)
 * @date 09 September 2014
 * Last update : 23 September 2014
 */

#include <string>

class LoggerManager;

class BaseObject {
public:
    BaseObject(std::string className);
    /**
     * Copy constructor
     *
     * @param other
     */
    BaseObject(const BaseObject& other);

    /**
     * Default destructor
     */
    ~BaseObject();

    /**
     * Virtual clone function to allow factory to copy all derived members
     * @return
     */
    virtual BaseObject* clone() const;

    // ##### GETTERS & SETTERS #####

    const std::string& getClassName() const;
    void setClassName(const std::string& className);

protected:
    LoggerManager* m_pLoggerManager; ///< Logger's pointer

private:
    std::string m_className; ///< String that represents name of child class used by the logger for know the source of the trace
};

#endif /* BASE_OBJECT_H */
