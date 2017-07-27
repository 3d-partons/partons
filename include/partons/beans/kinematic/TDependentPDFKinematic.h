#ifndef TDEPENDENTPDFKINEMATIC_H
#define TDEPENDENTPDFKINEMATIC_H

/**
 * @file TDependentPDFKinematic.h
 * @author: H. Moutarde (CEA/Irfu, Saclay)
 * @date 20 avr. 2015
 * @version 1.0
 */

#include <string>

namespace PARTONS {

/**
 * @class TDependentPDFKinematic
 *
 * @brief Class representing the kinematic variables for a t-dependent Parton Distribution Function model.
 */
class TDependentPDFKinematic {
public:

    /**
     * Default constructor
     */
    TDependentPDFKinematic();

    /**
     * Constructor
     *
     * @param x Longitudinal momentum fraction of the active parton
     * @param xi Skewness
     * @param t Mandelstam variable, momentum transfer on the hadron target (in GeV^2)
     * @param MuF Factorization scale (in GeV)
     * @param MuR Renormalization scale (in GeV)
     */
    TDependentPDFKinematic(double x, double t, double MuF, double MuR);

    /**
     * Default destructor
     */
    virtual ~TDependentPDFKinematic();

    /**
     * Return a pre-formatted characters string for output visualization of class member's values
     *
     * @return a pre-formatted characters string
     */

    // ##### GETTERS & SETTERS #####

    virtual std::string toString() const;
	double getMuF() const;
	void setMuF(double muF);
	double getMuR() const;
	void setMuR(double muR);
	double getT() const;
	void setT(double t);
	double getX() const;
	void setX(double x);

private:

    double m_x;		///< Longitudinal momentum fraction of the active parton
    double m_t;		///< Mandelstam variable, momentum transfer on the hadron target (in GeV^2)
    double m_MuF;	///< Factorization scale (in GeV)
    double m_MuR;	///< Renormalization scale (in GeV)
};

} /* namespace PARTONS */

#endif /* TDEPENDENTPDFKINEMATIC_H */
