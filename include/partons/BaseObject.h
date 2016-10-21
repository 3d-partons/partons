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
     * Virtual clone function to allow factory to clone all derived members object stored in the BaseObjectRegistry.
     * @return
     */
    virtual BaseObject* clone() const;

    /**
     * Because of the initialization step order of the program, objects are registered in a total random order and some objects depend others.
     * So to avoid pointer resolution order problem, this method is called by the BaseObjectRegistery after that all objects are well registered in it.
     * See BaseObjectRegistry class documentation for more details.
     */
    virtual void resolveObjectDependencies();

    /**
     * Return a pre-formatted characters string for output visualization of class member's values
     *
     * @return a pre-formatted characters string
     */
    //TODO propagade const every daugther class
    virtual std::string toString() const;

    /**
     * Use to split a complex C++ object into a concat of simple type.
     *
     * @param packet
     */
    void serialize(ElemUtils::Packet &packet) const;

    /**
     * Use to rebuild a complex C++ object from a concat of simple type.
     *
     * @param packet
     */
    void unserialize(ElemUtils::Packet &packet);

    /**
     * Overload of < operator to sort BaseObject object by its indexId value.
     *
     * @param other
     * @return
     */
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

    /**
     * Print info message into logger.
     *
     * @param functionName
     * @param message
     */
    void info(const std::string &functionName,
            const std::string &message) const;

    /**
     * Print debug message into logger.
     *
     * @param functionName
     * @param message
     */
    void debug(const std::string &functionName,
            const std::string &message) const;

    /**
     * Print warn message into logger.
     *
     * @param functionName
     * @param message
     */
    void warn(const std::string &functionName,
            const std::string &message) const;

    /**
     * Use in automation process to throw exception when a parameter is missing from the XML scenario file.
     *
     * @param parameterName
     */
    void errorMissingParameter(const std::string &parameterName) const;

private:
    unsigned int m_objectId;
    std::string m_className; ///< String that represents class's name used by the LoggerManager's class for know the source of the output trace

    int m_indexId; ///< Integer used when object is created from database. It is related to the id column value in the right database table.

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
