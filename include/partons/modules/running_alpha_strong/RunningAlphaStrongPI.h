/**
  * @file    RunningAlphaStrongPI.h
  * @author  Jose Manuel Morgado Chávez (University of Huelva),
  * @date    January 18th 2021
  * 
  * @brief   Evaluation of the process independent strong running coupling constant presented in e.g.: Eur. Phys. J. C (2020) 80:1064
  * @version 1.0
  *
  */

#ifndef RUNNING_ALPHA_STRONG_PI_H
#define RUNNING_ALPHA_STRONG_PI_H

#include <string>
#include <vector>

#include "RunningAlphaStrongModule.h"

namespace PARTONS {

/*!
 * \class RunningAlphaStrongPI
 */
class RunningAlphaStrongPI: public RunningAlphaStrongModule {

public:

    /**
     * Unique ID to automatically register the class in the registry.
     */
    static const unsigned int classId;

    /**
     * Constructor.
     * @param className Name of class.
     */
    RunningAlphaStrongPI(const std::string &className);

    virtual RunningAlphaStrongPI* clone() const;

    /**
     * Destructor.
     */
    virtual ~RunningAlphaStrongPI();

    virtual double compute();

    /**
     * Get number of colors (SU(\f$n_{c}\f$))
     */
    unsigned int GetColourNumber() const;

protected:

    /**
     * Copy constructor.
     * @param other Object to be copied.
     */
    RunningAlphaStrongPI(const RunningAlphaStrongPI &other);

    virtual void initModule();
    virtual void isModuleWellConfigured();

private:

    /**
     * Evaluate beta function coefficients for a fixed number of flavors.
     * @param NFlavour Number of flavors.
     */
    void ComputeExpansionCoefficients(unsigned int NFlavour);

    /**
     *  Evaluate \f$\Lambda_{QCD}\f$ for different number of flavors.
     */
    void ComputeLambdaQCD();

    /**
     *  Evaluate \f$\alpha_s\f$ at four loops.
     */
    void Running(double Mu, double Lambda, unsigned int NFlavour);

    /**
     *  Returns \f$\Lambda_{QCD}\f$ from \f$\alpha_s\f$ at given scale.
     */
    double FindLambda(double Lambda, std::vector<double> Parameters);

    /**
     * Number of colors (SU(\f$n_{c}\f$)).
     */
    unsigned int fNc;

    double fBeta0;              ///< 1st coefficient of beta function expansion.

    double fLambdaQCD4;         ///< \f$\Lambda_{QCD}(n_{f} = 4)\f$.
    double fLambdaQCD5;         ///< \f$\Lambda_{QCD}(n_{f} = 5)\f$.
    double fLambdaQCD6;         ///< \f$\Lambda_{QCD}(n_{f} = 6)\f$.
    double fAlphaS;             ///< Current value of strong coupling.

    double fa0;                 // Parameters for K2(k2)
    double fa1;
    double fb0;
};

} /* namespace PARTONS */

#endif /* RUNNING_ALPHA_STRONG_PI_H */