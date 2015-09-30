#ifndef OBSERVABLE_MODULE_H
#define OBSERVABLE_MODULE_H

/**
 * @file ObservableModule
 * @author Bryan BERTHOU (SPhN / CEA Saclay)
 * @date 19 November 2014
 * @version 1.0
 *
 * @class ObservableModule
 *
 * @brief
 */

#include <string>

#include "../ModuleObject.h"

class Vector3D;

class ObservableModule: public ModuleObject {
public:
    /**
     * Default constructor
     */
    ObservableModule(const std::string &className);

    virtual ObservableModule* clone() const = 0;

    /**
     * Default destructor
     */
    virtual ~ObservableModule();

    virtual void computeConvolCoeffFunction(double xB, double t, double Q2,
            double MuF2, double MuR2) = 0;

    virtual double computeCrossSection(double beamHelicity, double beamCharge,
            Vector3D targetPolarization, double phi) = 0;

protected:
    /**
     * Copy constructor
     *
     * @param other
     */
    ObservableModule(const ObservableModule &other);

    // Invariant scalars
    double m_xB;
    double m_t;
    double m_Q2;

    // Frame dependent scalars

    // 4-vectors defined in the CM frame

private:

};

#endif /* OBSERVABLE_MODULE_H */
