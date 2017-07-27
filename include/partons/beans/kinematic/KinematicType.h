#ifndef KINEMATIC_TYPE_H
#define KINEMATIC_TYPE_H

/**
 * @file KinematicType.h
 * @author: Bryan BERTHOU (SPhN / CEA Saclay)
 * @date 05 Mai 2015
 * @version 1.0
 */

#include <string>

namespace PARTONS {

/**
 * @class KinematicType
 *
 * @brief Definition of kinematics types.
 *
 * This class defines a set of enumeration values that are used to distinguish between kinematics type. In addition, a declared object of this class is always associated to one kinematics type (see KinematicType::m_type), so member functions can act on it. E.g.
 \code{.cpp}
 //this is single enum variable - nothing to play with
 KinematicType::Type enum_variable = KinematicType::THEO;

 //this is declared object
 KinematicType enum_object;

 //let us assign some type (default is KinematicType::UNDEFINED)
 enum_object.setType(enum_variable);

 //with objects you can use available functions, e.g. you can represent enumeration type by a corresponding string
 std::string enum_string_1 = enum_object.toString();

 //you can achieve some basic operations without the explicit declaration of objects by using the assignment constructor
 std::string enum_string_2 = KinematicType(KinematicType::EXP).toString();

 Partons::getInstance()->getLoggerManager()->info("example", __func__, ElemUtils::Formatter() << "Kinematics type is: " << enum_string_1);
 Partons::getInstance()->getLoggerManager()->info("example", __func__, ElemUtils::Formatter() << "Kinematics type is: " << enum_string_2);
 \endcode
 which gives via Logger:
 \code
 20-05-2017 12:13:02 [INFO] (example::main) Kinematics type is: THEO
 20-05-2017 12:13:02 [INFO] (example::main) Kinematics type is: EXP
 \endcode
 */
class KinematicType {

public:

    enum Type {
        UNDEFINED = 0,  //!< Undefined type.
        THEO = 1, //!< Kinematics related to to theory data
        EXP = 2 //!< Kinematics related to experimental data
    };

    /**
     * Default constructor.
     */
    KinematicType();

    /**
     * Assignment constructor.
     * @param type Type to be assigned.
     */
    KinematicType(Type type);

    /**
     * Copy constructor.
     * @param other Object to be copied.
     */
    KinematicType(const KinematicType &other);

    /**
     * Automatic cast to enum.
     */
    operator Type() const;

    /**
     * Get string representation of type being assigned to a declared object of this class.
     * @return String representation of assigned type, like "THEO" for KinematicType::THEO.
     */
    std::string toString() const;

    //********************************************************
    //*** SETTERS AND GETTERS ********************************
    //********************************************************

    /**
     * Get type being assigned to a declared object of this class.
     */
    KinematicType::Type getType() const;

    /**
     * Assign type to a declared object of this class.
     */
    void setType(Type type);

private:

    /**
     * Type associated to a declared object of this class.
     */
    KinematicType::Type m_type;
};

} /* namespace PARTONS */

#endif /* KINEMATIC_TYPE_H */
