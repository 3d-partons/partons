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
 * Each module in PARTONS is referred to with a unique class name and a unique int used as ID.
 *
 * Any C++ object in PARTONS can inherit of it. It fulfills several needs:
 * 1. It contains a clone function to be used to the factory to provide the user an object identified by a simple key (a string).
 * 2. It carries information on the identity of a specific object among others to transmit an explicit message to the logger (i.e. understandable by a human being, not an address in memory).
 * 3. It handles Packets to allow systematic computations over lists of inputs e.g. with threads.
 * 4. It allows the registry to store pointers to all modules in a generic way.
 */
class BaseObject {
public:

    /**
     * Constructor.
     * Takes a string parameter that represents the class's name of the child instance class.
     *
     * @param className class's name of child class.
     */
    BaseObject(const std::string &className);

    /**
     * Default destructor.
     */
    virtual ~BaseObject();

    /**
     * Virtual clone function to allow the factory to clone all derived members object stored in the BaseObjectRegistry.
     * @return
     */
    virtual BaseObject* clone() const;

    /**
     * Because of the initialization step order of the program, objects are registered in a total random order and some objects depend on others.
     * To avoid the problem of pointer resolution order, this method is called by the BaseObjectRegistery after all objects are well registered in it.
     * See BaseObjectRegistry class documentation for more details.
     */
    virtual void resolveObjectDependencies();

    /**
     * Return a pre-formatted characters string for output visualization of class member's values.
     *
     * @return a pre-formatted characters string.
     */
    virtual std::string toString() const;

    /**
     * Used to split a complex C++ object into a concatenation of simple types.
     *
     * @param packet
     */
    void serialize(ElemUtils::Packet &packet) const;

    /**
     * Used to rebuild a complex C++ object from a concatenation of simple type.
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

    /**
     *
     * @return Name of last child class.
     */
    const std::string& getClassName() const;

    /**
     *
     * @return Unique id identifying each object.
     */
    unsigned int getObjectId() const;

    /**
     *
     * @return Integer used when object is created from database. It is related to the id column value in the right database table.
     */
    int getIndexId() const;
    /**
     *
     * @param indexId Integer used when object is created from database. It is related to the id column value in the right database table.
     */
    void setIndexId(int indexId);

protected:
    /**
     * Copy constructor.
     *
     * @param other
     */
    BaseObject(const BaseObject& other);

    /**
     * Print info message into logger.
     *
     * @param functionName Name of the function throwing an information. Use \_\_func\_\_ to set it automatically.
     * @param message Message to the logger.
     */
    void info(const std::string &functionName,
            const std::string &message) const;

    /**
     * Print debug message into logger.
     *
     * @param functionName ame of the function throwing an information. Use \_\_func\_\_ to set it automatically.
     * @param message Message to the logger.
     */
    void debug(const std::string &functionName,
            const std::string &message) const;

    /**
     * Print warning message into logger.
     *
     * @param functionName ame of the function throwing an information. Use \_\_func\_\_ to set it automatically.
     * @param message Message to the logger.
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
    unsigned int m_objectId; ///< Unique id identifying each object.
    std::string m_className; ///< String that represents class's name used by the LoggerManager's class to know the source of the output trace.

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
