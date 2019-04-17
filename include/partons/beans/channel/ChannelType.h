#ifndef CHANNEL_TYPE_H
#define CHANNEL_TYPE_H

/**
 * @file ChannelType.h
 * @author: Bryan BERTHOU (SPhN / CEA Saclay)
 * @date November 26, 2014
 * @version 1.0
 */

#include <string>

namespace PARTONS {

/**
 * @class ChannelType
 *
 * @brief Definition of enumeration values for channels.
 *
 * This class defines a set of enumeration values that are used to distinguish between channels. In addition, a declared object of this class is always associated to one channel (see ChannelType::m_type), so member functions can act on it. E.g.
 \code{.cpp}
 //this is single enum variable - nothing to play with
 ChannelType::Type enum_variable = ChannelType::DVCS;

 //this is declared object
 ChannelType enum_object;

 //let us assign some type (default is ChannelType::UNDEFINED)
 enum_object.setType(enum_variable);

 //with objects you can use available functions, e.g. you can represent enumeration type by a corresponding string
 std::string enum_string_1 = enum_object.toString();

 //you can achieve some basic operations without the explicit declaration of objects by using the assignment constructor
 std::string enum_string_2 = ChannelType(ChannelType::DVMP).toString();

 Partons::getInstance()->getLoggerManager()->info("example", __func__, ElemUtils::Formatter() << "Channel is: " << enum_string_1);
 Partons::getInstance()->getLoggerManager()->info("example", __func__, ElemUtils::Formatter() << "Channel is: " << enum_string_2);
 \endcode
 which gives via Logger:
 \code
 20-05-2017 12:01:45 [INFO] (example::main) Channel is: DVCS
 20-05-2017 12:01:45 [INFO] (example::main) Channel is: DVMP
 \endcode
 */
class ChannelType {
public:

    /**
     * Definition of enumerate values corresponding to channels.
     */
    enum Type {
        UNDEFINED = 0, //!< Undefined type.
        DVCS = 1, //!< Deeply Virtual Compton Scattering
        DVMP = 2, //!< Deeply Virtual Meson Production
        TCS = 3 //!< Time-like Compton Scattering
    };

    /**
     * Default constructor.
     */
    ChannelType();

    /**
     * Assignment constructor.
     * @param type Type to be assigned.
     */
    ChannelType(Type type);

    /**
     * Destructor.
     */
    virtual ~ChannelType();

    /**
     * Automatic cast to enum.
     */
    operator Type() const;

    /**
     * Get string representation of type being assigned to a declared object of this class.
     * @return String representation of assigned type, like "DVCS" for ChannelType::DVCS.
     */
    std::string toString() const;

    /**
     * Get short name representation of type being assigned to a declared object of this class.
     * @return Short string representation of assigned type, like "DVCS" for ChannelType:DVCS.
     */
    std::string getShortName();

    //********************************************************
    //*** SETTERS AND GETTERS ********************************
    //********************************************************

    /**
     * Get type being assigned to a declared object of this class.
     */
    ChannelType::Type getType() const;

    /**
     * Assign type to a declared object of this class.
     */
    void setType(Type type);

private:

    /**
     * Type associated to a declared object of this class.
     */
    ChannelType::Type m_type;
};

} /* namespace PARTONS */

#endif /* CHANNEL_TYPE_H */
