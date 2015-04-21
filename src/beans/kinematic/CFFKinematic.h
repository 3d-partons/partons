#ifndef CFFKINEMATIC_H
#define CFFKINEMATIC_H

/**
 * @file CFFKinematic.h
 * @author: Bryan BERTHOU (SPhN / CEA Saclay)
 * @date 26 November 2014
 * @version 1.0
 *
 * @class CFFKinematic
 *
 * @brief Kinematic configuration of a Compton Form Factor.
 */

#include <string>

class CFFKinematic {
public:

    /**
     * Default constructor
     */
    CFFKinematic();

    /**
     * Constructor
     *
     * @param xi Skewness
     * @param t Mandelstam variable, momentum transfer on the hadron target (in GeV^2)
     * @param Q2 Virtuality of the photon in Born approximation (in GeV^2)
     * @param MuF Factorization scale (in GeV)
     * @param MuR Renormalization scale (in GeV)
     */
    CFFKinematic(double xi, double t, double Q2, double MuF, double MuR);

    /**
     * Constructor with a binId
     *
     * @param binId Identifier of a kinematic bin
     * @param xi Skewness
     * @param t Mandelstam variable, momentum transfer on the hadron target (in GeV^2)
     * @param Q2 Virtuality of the photon in Born approximation (in GeV^2)
     * @param MuF Factorization scale (in GeV)
     * @param MuR Renormalization scale (in GeV)
     */
    CFFKinematic(unsigned int binId, double xi, double t, double Q2, double MuF, double MuR);

    /**
     * Default destructor
     */
    virtual ~CFFKinematic();

    /**
     * Return a pre-formatted characters string for output visualization of class member's values
     *
     * @return a pre-formatted characters string
     */
    virtual std::string toString();

    // ##### GETTERS & SETTERS #####

    unsigned int getBinId() const;
    void setBinId(unsigned int binId);
    double getQ2() const;
    void setQ2(double q2);
	double getMuF() const;
	void setMuF(double muF);
	double getMuR() const;
	void setMuR(double muR);
	double getT() const;
	void setT(double t);
	double getXi() const;
	void setXi(double xi);

private:

    unsigned int m_binId;
    double m_xi;	///< Skewness
    double m_t;		///< Mandelstam variable, momentum transfer on the hadron target (in GeV^2)
    double m_Q2;	///< Virtuality of the photon in Born approximation (in GeV^2)
    double m_MuF;	///< Factorization scale (in GeV)
    double m_MuR;	///< Renormalization scale (in GeV)
};

#endif /* CFFKINEMATIC_H */
