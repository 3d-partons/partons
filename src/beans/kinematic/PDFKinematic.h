#ifndef PDFKINEMATIC_H
#define PDFKINEMATIC_H

/**
 * @file PDFKinematic.h
 * @author: H. Moutarde (CEA/Irfu, Saclay)
 * @date 26 November 2014
 * @version 1.0
 *
 * @class GPDKinematic
 *
 * @brief Class representing the kinematic variables for a GPD model.
 */

#include <string>

class PDFKinematic {
public:

    /**
     * Default constructor
     */
    PDFKinematic();

    /**
     * Constructor
     *
     * @param x Longitudinal momentum fraction of the active parton
     * @param MuF Factorization scale (in GeV)
     * @param MuR Renormalization scale (in GeV)
     */
    PDFKinematic(double x, double MuF, double MuR);

    /**
     * Default destructor
     */
    virtual ~PDFKinematic();

    /**
     * Return a pre-formatted characters string for output visualization of class member's values
     *
     * @return a pre-formatted characters string
     */
    virtual std::string toString();

    // ##### GETTERS & SETTERS #####

	double getMuF() const;
	void setMuF(double muF);
	double getMuR() const;
	void setMuR(double muR);
	double getX() const;
	void setX(double x);

private:

    double m_x;		///< Longitudinal momentum fraction of the active parton
    double m_MuF;	///< Factorization scale (in GeV)
    double m_MuR;	///< Renormalization scale (in GeV)
};

#endif /* PDFKINEMATIC_H */
