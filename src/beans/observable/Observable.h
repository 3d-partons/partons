#ifndef OBSERVABLE_H
#define OBSERVABLE_H

/**
 * @file Observable.h
 * @author Bryan BERTHOU (CEA Saclay)
 * @date 25 November 2014
 * @version 1.0
 *
 * @class Observable
 */

#include <string>

#include "../../modules/observable/DVCSModule.h"
#include "../../utils/vector/Vector3D.h"
#include "ObservableChannel.h"

class Observable: public BaseObject {

public:
    /**
     * Default destructor
     */
    virtual ~Observable();

    /**
     * Virtual clone function to allow factory to copy all derived members
     * @return
     */
    virtual Observable* clone() const = 0;

    typedef double (DVCSModule::*computeCrossSection)(double beamHelicity,
            double beamCharge, Vector3D targetPolarization);

    virtual double compute(DVCSModule* pDVCSModule) = 0;

// ##### GETTERS & SETTERS #####

    double getBeamCharge() const;
    void setBeamCharge(double beamCharge);
    double getBeamHelicity() const;
    void setBeamHelicity(double beamHelicity);
    const Vector3D& getTargetPolarization() const;
    void setTargetPolarization(const Vector3D& targetPolarization);

protected:
    Observable(const std::string &className, ObservableChannel::Type channel,
            double beamHelicity, double beamCharge,
            Vector3D targetPolarization);

    /**
     * Copy constructor
     *
     * Use by the factory
     *
     * @param other
     */
    Observable(const Observable& other);

private:
    //TODO doc
    ObservableChannel::Type m_channel;

    double m_beamHelicity;
    double m_beamCharge;
    Vector3D m_targetPolarization;
};

#endif /* OBSERVABLE_H */
