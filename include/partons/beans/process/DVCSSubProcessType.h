#ifndef DVCS_SUB_PROCESS_TYPE_H
#define DVCS_SUB_PROCESS_TYPE_H

/**
 * @file DVCSSubProcessType.h
 * @author Pawel Sznajder (NCBJ, Warsaw)
 * @date July 27, 2017
 * @version 1.0
 */

#include <string>

#include "../../BaseObject.h"

namespace PARTONS {

/**
 * @class DVCSSubProcessType
 *
 * @brief Definition of enumeration values for DVCS subprocess types.
 *
 * This class defines a set of enumeration values that are used to distinguish between DVCS subprocess types. In addition, a declared object of this class is always associated to one DVCS subrpocess type (see DVCSSubProcessType::m_type), so member functions can act on it. E.g.
 \code{.cpp}
 //this is single enum variable - nothing to play with
 DVCSSubProcessType::Type enum_variable = DVCSSubProcessType::BH;

 //this is declared object
 DVCSSubProcessType enum_object;

 //let us assign some type (default is DVCSSubProcessType::UNDEFINED)
 enum_object.setType(enum_variable);

 //with objects you can use available functions, e.g. you can represent enumeration type by a corresponding string
 std::string enum_string_1 = enum_object.toString();

 //you can achieve some basic operations without the explicit declaration of objects by using the assignment constructor
 std::string enum_string_2 = DVCSSubProcessType(DVCSSubProcessType::DVCS).toString();

 Partons::getInstance()->getLoggerManager()->info("example", __func__, ElemUtils::Formatter() << "Subprocess is: " << enum_string_1);
 Partons::getInstance()->getLoggerManager()->info("example", __func__, ElemUtils::Formatter() << "Subprocess is: " << enum_string_2);
 \endcode
 which gives via Logger:
 \code
 20-05-2017 12:13:02 [INFO] (example::main) Subprocess is: BH
 20-05-2017 12:13:02 [INFO] (example::main) Subprocess is: DVCS
 \endcode
 */
class DVCSSubProcessType: public BaseObject {

public:

    enum Type {
        UNDEFINED = 0,  //!< Undefined type.
        ALL = 1,       //!< All subprocesses, i.e. DVCS, BH and INT.
        DVCS = 2,       //!< DVCS subprocess.
        BH = 3,         //!< Bethe-Heitler subrocess.
        INT = 4         //!< Interference between DVCS and Bethe-Heitler contribution.
    };

    /**
     * Default constructor.
     */
    DVCSSubProcessType();

    /**
     * Assignment constructor.
     * @param type Type to be assigned.
     */
    DVCSSubProcessType(Type type);

    /**
     * Assignment constructor trying to match subprocess type from given string. If unable to match set DVCSSubProcessType::UNDEFINED.
     * @param dvcsSubProcessTypeString String to be matched.
     */
    DVCSSubProcessType(const std::string &dvcsSubProcessTypeString);

    /**
     * Get string representation of type being assigned to a declared object of this class.
     * @return String representation of assigned type, like "LO" for DVCSSubProcessType::LO.
     */
    std::string toString() const;

    //********************************************************
    //*** SETTERS AND GETTERS ********************************
    //********************************************************

    /**
     * Get type being assigned to a declared object of this class.
     */
    DVCSSubProcessType::Type getType() const;

    /**
     * Assign type to a declared object of this class.
     */
    void setType(Type type);

private:

    /**
     * Type associated to a declared object of this class.
     */
    DVCSSubProcessType::Type m_type;
};

} /* namespace PARTONS */

#endif /* DVCS_SUB_PROCESS_TYPE_H */
