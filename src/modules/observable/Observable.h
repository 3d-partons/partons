#ifndef OBSERVABLE_H
#define OBSERVABLE_H

/**
 * @file Observable.h
 * @author Bryan BERTHOU (CEA Saclay)
 * @date 25 November 2014
 * @version 1.0
 *
 * @class Observable
 *
 * @brief
 */

#include <string>
#include <vector>

#include "../../beans/observable/ObservableChannel.h"
#include "../../beans/observable/ObservableType.h"
#include "../../utils/vector/Vector3D.h"
#include "../process/DVCSModule.h"

class ObservableResultList;

class Observable: public BaseObject {

public:
    Observable(const std::string &className);

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

    ObservableResultList compute(double xB, double t, double Q2,
            std::vector<double> listOfPhi);

    virtual double compute(ProcessModule* pDVCSModule, double phi);

// ##### GETTERS & SETTERS #####

    double getBeamCharge() const;
    void setBeamCharge(double beamCharge);
    double getBeamHelicity() const;
    void setBeamHelicity(double beamHelicity);
    const Vector3D& getTargetPolarization() const;
    void setTargetPolarization(const Vector3D& targetPolarization);
    const ProcessModule* getDVCSModule() const;
    void setDVCSModule(ProcessModule* pDVCSModule);

protected:

    /**
     * Copy constructor
     *
     * Use by the factory
     *
     * @param other
     */
    Observable(const Observable& other);

    ProcessModule* m_pDVCSModule;

    //TODO doc
    ObservableChannel::Type m_channel;
    ObservableType::Type m_observableType;

    double m_beamHelicity;
    double m_beamCharge;

    Vector3D m_targetPolarization;

    virtual double compute();
};

#endif /* OBSERVABLE_H */
