#ifndef OBSERVABLE_CHANNEL_H
#define OBSERVABLE_CHANNEL_H

/**
 * @file ObservableChannel.h
 * @author: Bryan BERTHOU (SPhN / CEA Saclay)
 * @date November 26, 2014
 * @version 1.0
 */

#include <string>

namespace PARTONS {

/**
 * @class ObservableChannel
 *
 * @brief Definition of enumeration values for observable channels.
 *
 * This class defines a set of enumeration values that are used to distinguish between observable channels. In addition, a declared object of this class is always associated to one observable channel (see ObservableChannel::m_type), so member functions can act on it. E.g.
 \code{.cpp}
 //this is single enum variable - nothing to play with
 ObservableChannel::Type enum_variable = ObservableChannel::DVCS;

 //this is declared object
 ObservableChannel enum_object;

 //let us assign some type (default is ObservableChannel::UNDEFINED)
 enum_object.setType(enum_variable);

 //with objects you can use available functions, e.g. you can represent enumeration type by a corresponding string
 std::string enum_string_1 = enum_object.toString();

 //you can achieve some basic operations without the explicit declaration of objects by using the assignment constructor
 std::string enum_string_2 = ObservableChannel(ObservableChannel::DVMP).toString();

 Partons::getInstance()->getLoggerManager()->info("example", __func__, ElemUtils::Formatter() << "Observable channel is: " << enum_string_1);
 Partons::getInstance()->getLoggerManager()->info("example", __func__, ElemUtils::Formatter() << "Observable channel is: " << enum_string_2);
 \endcode
 which gives via Logger:
 \code
 20-05-2017 12:01:45 [INFO] (example::main) Observable channel is: DVCS
 20-05-2017 12:01:45 [INFO] (example::main) Observable channel is: DVMP
 \endcode
 */
class ObservableChannel {
public:

    /**
     * Definition of enumerate values corresponding to observable channels.
     */
    enum Type {
        UNDEFINED, //!< Undefined type.
        DVCS, //!< Deeply Virtual Compton Scattering
        DVMP, //!< Deeply Virtual Meson Production
        TCS //!< Time-like Compton Scattering
    };

    /**
     * Default constructor.
     */
    ObservableChannel();

    /**
     * Assignment constructor.
     * @param type Type to be assigned.
     */
    ObservableChannel(Type type);

    /**
     * Destructor.
     */
    virtual ~ObservableChannel();

    /**
     * Automatic cast to enum.
     */
    operator Type() const;

    /**
     * Get string representation of type being assigned to a declared object of this class.
     * @return String representation of assigned type, like "DVCS" for ObservableChannel::DVCS.
     */
    std::string toString() const;

    /**
     * Get short name representation of type being assigned to a declared object of this class.
     * @return Short string representation of assigned type, like "DVCS" for ObservableChannel:DVCS.
     */
    std::string getShortName();

    //********************************************************
    //*** SETTERS AND GETTERS ********************************
    //********************************************************

    /**
     * Get type being assigned to a declared object of this class.
     */
    ObservableChannel::Type getType() const;

    /**
     * Assign type to a declared object of this class.
     */
    void setType(Type type);

private:

    /**
     * Type associated to a declared object of this class.
     */
    ObservableChannel::Type m_type;
};

} /* namespace PARTONS */

#endif /* OBSERVABLE_CHANNEL_H */
