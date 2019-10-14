#ifndef MESON_TYPE_H
#define MESON_TYPE_H

/**
 *
 * @file MesonType.h
 * @author Pawel Sznajder (NCBJ)
 * @date 23 April 2019
 * @version 1.0
 */

#include <string>

namespace PARTONS {

/**
 * @class MesonType
 *
 * @brief Definition of enumeration values for meson types.
 *
 * This class defines a set of enumeration values that are used to distinguish between meson types. In addition, a declared object of this class is always associated to one meson type (see MesonType::m_type), so member functions can act on it.
 */
class MesonType {

public:

    /**
     * Definition of enumerate values corresponding to quark flavors.
     */
    enum Type {
        UNDEFINED = 0,  //!< Undefined type.
        RHOMINUS = 1,   //!<  \f$\rho^{-}\f$
        RHO0 = 2,       //!<  \f$\rho^{0}\f$
        RHOPLUS = 3,    //!<  \f$\rho^{+}\f$
        OMEGA = 4,      //!<  \f$\omega\f$
        PHI = 5,        //!<  \f$\phi\f$
        JPSI = 6,       //!<  \f$J/\Psi\f$
        UPSILON = 7,    //!<  \f$\Upsilon\f$

        PIMINUS = 7,    //!<  \f$\pi^{-}\f$
        PI0 = 8,        //!<  \f$\pi^{0}\f$
        PIPLUS = 9      //!<  \f$\pi^{+}\f$
    };

    /**
     * Default constructor.
     */
    MesonType();

    /**
     * Copy constructor.
     * @param other Object to be copied.
     */
    MesonType(const MesonType &other);

    /**
     * Assignment constructor.
     * @param type Type to be assigned.
     */
    MesonType(Type type);

    /**
     * Destructor.
     */
    virtual ~MesonType();

    /**
     * Automatic cast to enum.
     */
    operator Type() const;

    /**
     * Get string representation of type being assigned to a declared object of this class.
     * @return String representation of assigned type, like "UP" for MesonType::UP.
     */
    std::string toString() const;

    /**
     * Get short name representation of type being assigned to a declared object of this class.
     * @return Short string representation of assigned type, like "u" for MesonType::UP.
     */
    std::string getShortName();

    //********************************************************
    //*** SETTERS AND GETTERS ********************************
    //********************************************************

    /**
     * Get type being assigned to a declared object of this class.
     */
    MesonType::Type getType() const;

    /**
     * Assign type to a declared object of this class.
     */
    void setType(Type type);

private:

    /**
     * Type associated to a declared object of this class.
     */
    MesonType::Type m_type;
};

} /* namespace PARTONS */

#endif /* MESON_TYPE_H */
