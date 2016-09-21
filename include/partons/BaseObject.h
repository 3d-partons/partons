#ifndef BASE_OBJECT_H
#define BASE_OBJECT_H

/**
 * @file BaseObject.h
 * @author Bryan BERTHOU (SPhN / CEA Saclay)
 * @date September 09, 2014
 * @version 1.0
 */

#include <string>

namespace ElemUtils {
class Packet;
} /* namespace ElemUtils */

/** @class BaseObject
 *
 * @brief BaseObject is the ”zeroth-level-object” of the architecture.
 *
 * Any C++ object in PARTONS can inherit of it.
 * It also carries information on the identity of a specific object to transmit an explicit message to the logger (i.e. understandable by a human being, not an address in memory).
 */
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

    virtual void resolveObjectDependencies();

    /**
     * Return a pre-formatted characters string for output visualization of class member's values
     *
     * @return a pre-formatted characters string
     */
    //TODO propagade const every daugther class
    virtual std::string toString() const;

    void serialize(ElemUtils::Packet &packet) const;
    void unserialize(ElemUtils::Packet &packet);

    bool operator <(const BaseObject& other) const;

    // ##### GETTERS & SETTERS #####

    const std::string& getClassName() const;

    unsigned int getObjectId() const;
    void setObjectId(unsigned int objectId);

    int getIndexId() const;
    void setIndexId(int indexId);

protected:
    /**
     * Copy constructor
     *
     * @param other
     */
    BaseObject(const BaseObject& other);

    void info(const std::string &functionName,
            const std::string &message) const;

    void debug(const std::string &functionName,
            const std::string &message) const;

    void warn(const std::string &functionName,
            const std::string &message) const;

    void errorMissingParameter(const std::string &parameterName) const;

private:
    unsigned int m_objectId;
    std::string m_className; ///< String that represents class's name used by the LoggerManager's class for know the source of the output trace

    int m_indexId;

    static unsigned int m_uniqueObjectIdCounter;

    /**
     * On call, returns an incremented by one uniqueID member value.
     *
     * @return incremented unsigned int ID.
     */
    unsigned int getUniqueObjectId();
};

ElemUtils::Packet& operator <<(ElemUtils::Packet& packet, BaseObject& object);
ElemUtils::Packet& operator >>(ElemUtils::Packet& packet, BaseObject& object);

#endif /* BASE_OBJECT_H */
