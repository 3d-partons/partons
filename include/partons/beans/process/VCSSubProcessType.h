#ifndef VCS_SUB_PROCESS_TYPE_H
#define VCS_SUB_PROCESS_TYPE_H

/**
 * @file VCSSubProcessType.h
 * @author Pawel Sznajder (NCBJ, Warsaw)
 * @date July 27, 2017
 * @version 1.0
 */

#include <string>

#include "../../BaseObject.h"

namespace PARTONS {

/**
 * @class VCSSubProcessType
 *
 * @brief Definition of enumeration values for VCS subprocess types.
 *
 * This class defines a set of enumeration values that are used to distinguish between VCS subprocess types. In addition, a declared object of this class is always associated to one VCS subrpocess type (see VCSSubProcessType::m_type), so member functions can act on it. E.g.
 \code{.cpp}
 //this is single enum variable - nothing to play with
 VCSSubProcessType::Type enum_variable = VCSSubProcessType::BH;

 //this is declared object
 VCSSubProcessType enum_object;

 //let us assign some type (default is VCSSubProcessType::UNDEFINED)
 enum_object.setType(enum_variable);

 //with objects you can use available functions, e.g. you can represent enumeration type by a corresponding string
 std::string enum_string_1 = enum_object.toString();

 //you can achieve some basic operations without the explicit declaration of objects by using the assignment constructor
 std::string enum_string_2 = VCSSubProcessType(VCSSubProcessType::DVCS).toString();

 Partons::getInstance()->getLoggerManager()->info("example", __func__, ElemUtils::Formatter() << "Subprocess is: " << enum_string_1);
 Partons::getInstance()->getLoggerManager()->info("example", __func__, ElemUtils::Formatter() << "Subprocess is: " << enum_string_2);
 \endcode
 which gives via Logger:
 \code
 20-05-2017 12:13:02 [INFO] (example::main) Subprocess is: BH
 20-05-2017 12:13:02 [INFO] (example::main) Subprocess is: DVCS
 \endcode
 */
class VCSSubProcessType: public BaseObject {

public:

    enum Type {
        UNDEFINED = 0,  //!< Undefined type.
        ALL = 1,        //!< All subprocesses, i.e. VCS, BH and INT.
        DVCS = 2,       //!< DVCS subprocess.
        BH = 3,         //!< Bethe-Heitler subrocess.
        INT = 4,   //!< Interference between VCS and Bethe-Heitler contribution.
        TCS = 5,        //!< TCS subprocess.
        DDVCS = 6       //!< DDVCS subprocess.
    };

    /**
     * Default constructor.
     */
    VCSSubProcessType();

    /**
     * Assignment constructor.
     * @param type Type to be assigned.
     */
    VCSSubProcessType(Type type);

    /**
     * Assignment constructor trying to match subprocess type from given string. If unable to match set VCSSubProcessType::UNDEFINED.
     * @param dvcsSubProcessTypeString String to be matched.
     */
    VCSSubProcessType(const std::string &dvcsSubProcessTypeString);

    /**
     * Get string representation of type being assigned to a declared object of this class.
     * @return String representation of assigned type, like "LO" for VCSSubProcessType::LO.
     */
    std::string toString() const;

    //********************************************************
    //*** SETTERS AND GETTERS ********************************
    //********************************************************

    /**
     * Get type being assigned to a declared object of this class.
     */
    VCSSubProcessType::Type getType() const;

    /**
     * Assign type to a declared object of this class.
     */
    void setType(Type type);

private:

    /**
     * Type associated to a declared object of this class.
     */
    VCSSubProcessType::Type m_type;
};

} /* namespace PARTONS */

#endif /* VCS_SUB_PROCESS_TYPE_H */
