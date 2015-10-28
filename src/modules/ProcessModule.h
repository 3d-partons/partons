#ifndef PROCESS_MODULE_H
#define PROCESS_MODULE_H

/**
 * @file ProcessModule.h
 * @author Bryan BERTHOU (SPhN / CEA Saclay)
 * @date 19 November 2014
 * @version 1.0
 *
 * @class ProcessModule
 *
 * @brief
 */

#include <string>

#include "../ModuleObject.h"

namespace NumA {
class Vector3D;
} /* namespace NumA */

class Vector3D;

class ProcessModule: public ModuleObject {
public:
    /**
     * Default constructor
     */
    ProcessModule(const std::string &className);

    virtual ProcessModule* clone() const = 0;

    /**
     * Default destructor
     */
    virtual ~ProcessModule();

    virtual void computeConvolCoeffFunction(double xB, double t, double Q2) = 0;

    virtual double computeCrossSection(double beamHelicity, double beamCharge,
            NumA::Vector3D targetPolarization, double phi) = 0;

protected:
    /**
     * Copy constructor
     *
     * @param other
     */
    ProcessModule(const ProcessModule &other);

    // Invariant scalars
    double m_xB;        ///< Bjorken variable
    double m_t;     ///< Mandelstam variable (square of the 4-momentum transfer)
    double m_Q2;    ///< Virtuality of the photon

    double m_E;     ///< Beam energy in target rest frame

    // Frame dependent scalars

    // 4-vectors defined in the CM frame

private:

};

#endif /* PROCESS_MODULE_H */
