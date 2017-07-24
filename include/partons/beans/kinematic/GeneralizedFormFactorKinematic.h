#ifndef GENERALIZED_FORM_FACTOR_KINEMATIC_H
#define GENERALIZED_FORM_FACTOR_KINEMATIC_H

/**
 * @file GeneralizedFormFactorKinematic.h
 * @author: H. Moutarde (CEA/Irfu, Saclay)
 * @date 20 avr. 2015
 * @version 1.0
 */

#include <string>

namespace PARTONS {


/**
 * @class GeneralizedFormFactorKinematic
 *
 * @brief Class representing the kinematic variables for a Generalized Form Factor model.\n
 * The Generalized Form Factors are the coefficients of the polynomial\n
 * expansion of the Mellin moments of a GPD. They are t-dependent functions, and\n
 * also carry an implicit dependance on renormalization and factorization scales.\n
 * They are dimensionless functions.
 */
class GeneralizedFormFactorKinematic {
public:

    /**
     * Default constructor
     */
    GeneralizedFormFactorKinematic();

    /**
     * Constructor
     *
     * @param t Mandelstam variable, momentum transfer on the hadron target (in GeV^2)
     * @param MuF Factorization scale (in GeV)
     * @param MuR Renormalization scale (in GeV)
     */
    GeneralizedFormFactorKinematic(double t, double MuF, double MuR);

    /**
     * Default destructor
     */
    virtual ~GeneralizedFormFactorKinematic();

    /**
     * Return a pre-formatted characters string for output visualization of class member's values
     *
     * @return a pre-formatted characters string
     */
    virtual std::string toString() const;

    // ##### GETTERS & SETTERS #####

    double getMuF() const;
	void setMuF(double muF);
	double getMuR() const;
	void setMuR(double muR);
	double getT() const;
	void setT(double t);

private:

    double m_t;		///< Mandelstam variable, momentum transfer on the hadron target (in GeV^2)
    double m_MuF;	///< Factorization scale (in GeV)
    double m_MuR;	///< Renormalization scale (in GeV)
};

} /* namespace PARTONS */

#endif /* GENERALIZED_FORM_FACTOR_KINEMATIC_H */
