#ifndef BASE_OBJECT_H
#define BASE_OBJECT_H

/**
 * @author Bryan BERTHOU (CEA Saclay)
 * @date 09 September 2014
 */

#include <string>

class LoggerManager;

class BaseObject {
public:
    /**
     * Constructor
     *
     * @param className
     */
    BaseObject(std::string className);

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

    virtual std::string toString();

    // ##### GETTERS & SETTERS #####

    unsigned int getObjectId() const;
    void setObjectId(unsigned int objectId);
    const std::string& getClassName() const;
    void setClassName(const std::string& className);

protected:
    LoggerManager* m_pLoggerManager; ///< Logger's pointer

    /**
     * Copy constructor
     *
     * @param other
     */
    BaseObject(const BaseObject& other);

private:
    static unsigned int uniqueID;

    unsigned int m_objectId;
    std::string m_className; ///< String that represents class's name used by the logger for know the source of the trace

    unsigned int getUniqueID();
};

#endif /* BASE_OBJECT_H */
