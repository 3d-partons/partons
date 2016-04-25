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

#include <NumA/linear_algebra/vector/Vector3D.h>
#include <pthread.h>
#include <string>

#include "../../beans/observable/ObservableChannel.h"
#include "../../beans/observable/ObservableKinematic.h"
#include "../../beans/observable/ObservableType.h"
#include "../process/DVCSModule.h"

class ObservableResult;

class Observable: public ModuleObject /*, public Thread*/{

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

    virtual void initModule();

    virtual void isModuleWellConfigured();

    /**
     * Provides a generic method to configure all types of modules by passing a Parameters object.
     * (See ModuleObject class for more info).
     *
     * @param parameters
     */
    virtual void configure(const ElemUtils::Parameters &parameters);

    typedef double (DVCSModule::*computeCrossSection)(double beamHelicity,

    double beamCharge, NumA::Vector3D targetPolarization);

    ObservableResult compute(const ObservableKinematic &kinematic);

    ObservableResult compute(double xB, double t, double Q2, double phi);

    virtual double compute(ProcessModule* pDVCSModule, double phi);

    // TODO clean
//    virtual double Num(ProcessModule* pDVCSModule, double phi);
//    virtual double Den(ProcessModule* pDVCSModule, double phi);

    virtual void run();

// ##### GETTERS & SETTERS #####

    double getBeamCharge() const;
    void setBeamCharge(double beamCharge);
    double getBeamHelicity() const;
    void setBeamHelicity(double beamHelicity);
    const NumA::Vector3D& getTargetPolarization() const;
    void setTargetPolarization(const NumA::Vector3D& targetPolarization);
    const ProcessModule* getProcessModule() const;
    void setProcessModule(ProcessModule* pProcessModule);
    ObservableChannel::Type getChannel() const;
    void setChannel(ObservableChannel::Type channel);

protected:
    /**
     * Copy constructor
     *
     * Use by the factory
     *
     * @param other
     */
    Observable(const Observable& other);

    ProcessModule* m_pProcessModule;

    //TODO doc
    ObservableChannel::Type m_channel;
    ObservableType::Type m_observableType;

    double m_beamHelicity;
    double m_beamCharge;

    NumA::Vector3D m_targetPolarization;

    virtual double compute();

private:
    pthread_mutex_t m_mutex;
};

#endif /* OBSERVABLE_H */
