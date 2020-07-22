#ifndef DVMP_PROCESS_GK06_H
#define DVMP_PROCESS_GK06_H

/**
 * @file DVMPProcessGK06.h
 * @author Kemal Tezgin
 * @date October 21, 2019
 * @version 1.0
 */

#include <complex>
#include <string>

#include "../../../utils/type/PhysicalType.h"
#include "DVMPProcessModule.h"

namespace PARTONS {

/**
 * @class DVMPProcessGK06
 *
 * TODO
 */
class DVMPProcessGK06: public DVMPProcessModule {

public:

    static const unsigned int classId; ///< Unique ID to automatically register the class in the registry.

    /**
     * Constructor.
     * See BaseObject::BaseObject and ModuleObject::ModuleObject for more details.
     *
     * @param className name of child class.
     */
    DVMPProcessGK06(const std::string &className);

    /**
     * Default destructor.
     */
    virtual ~DVMPProcessGK06();
    virtual DVMPProcessGK06* clone() const;

protected:

    /** Copy constructor.
     @param other Object to be copied.
     */
    DVMPProcessGK06(const DVMPProcessGK06& other);

    virtual void initModule();
    virtual void isModuleWellConfigured();

    virtual PhysicalType<double> CrossSection();

private:

    double CrossSectionL(); ///< Partial cross-section L
    double CrossSectionT(); ///< Partial cross-section T
    double CrossSectionLT(); ///< Partial cross-section LT
    double CrossSectionTT(); ///< Partial cross-section TT

    double poleResidue(); ///< The residue of the pole in Pi+ production

    double poleAmplitude0p0p(); ///< Computation of the pion pole amplitude M_{0+0+}.
    double poleAmplitude0m0p(); ///< Computation of the pion pole amplitude M_{0-0+}.
    double poleAmplitude0mpp(); ///< Computation of the pion pole amplitude M_{0-++}.
    double poleAmplitude0ppp(); ///< Computation of the pion pole amplitude M_{0+++}.
    double poleAmplitude0pmp(); ///< Computation of the pion pole amplitude M_{0+-+}.
    double poleAmplitude0mmp(); ///< Computation of the pion pole amplitude M_{0--+}.

    std::complex<double> Amplitude0p0p(); ///< Computation of the amplitude M_{0+0+}.
    std::complex<double> Amplitude0m0p(); ///< Computation of the amplitude M_{0-0+}.
    std::complex<double> Amplitude0mpp(); ///< Computation of the amplitude M_{0-++}.
    std::complex<double> Amplitude0ppp(); ///< Computation of the amplitude M_{0+++}.
    std::complex<double> Amplitude0pmp(); ///< Computation of the amplitude M_{0+-+}.
    std::complex<double> Amplitude0mmp(); ///< Computation of the amplitude M_{0--+}.

    double m_W2; ///< W^2 variable.
    double m_xi; ///< xi variable.

    double lambdaFunction(double a, double b, double c) const; ///< Mandalstam function.
};

} /* namespace PARTONS */

#endif /* DVMP_PROCESS_GK06_H */
