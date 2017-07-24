#ifndef OBSERVABLE_H
#define OBSERVABLE_H

/**
 * @file Observable.h
 * @author Bryan BERTHOU (SPhN / CEA Saclay)
 * @date November 25, 2014
 * @version 1.0
 */

#include <ElementaryUtils/parameters/Parameters.h>
#include <NumA/linear_algebra/vector/Vector3D.h>
#include <pthread.h>
#include <map>
#include <string>

#include "../../beans/automation/BaseObjectData.h"
#include "../../beans/gpd/GPDType.h"
#include "../../beans/List.h"
#include "../../beans/observable/ObservableChannel.h"
#include "../../beans/observable/ObservableKinematic.h"
#include "../../beans/observable/ObservableType.h"
#include "../process/DVCS/DVCSModule.h"

class ObservableResult;

/**
 * @class Observable
 *
 * @brief
 */
class Observable: public ModuleObject {
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

    ObservableResult compute(const ObservableKinematic &kinematic,
            const List<GPDType> & gpdType = List<GPDType>());

    ObservableResult compute(double xB, double t, double Q2, double E,
            double phi, const List<GPDType> & gpdType = List<GPDType>());

    virtual double computePhiObservable(double phi);

    // TODO clean
//    virtual double Num(ProcessModule* pDVCSModule, double phi);
//    virtual double Den(ProcessModule* pDVCSModule, double phi);

    virtual void run();

    virtual void prepareSubModules(
            const std::map<std::string, BaseObjectData>& subModulesData);

// ##### GETTERS & SETTERS #####

    double getBeamCharge() const;
    void setBeamCharge(double beamCharge);
    double getBeamHelicity() const;
    void setBeamHelicity(double beamHelicity);
    const NumA::Vector3D& getTargetPolarization() const;
    void setTargetPolarization(const NumA::Vector3D& targetPolarization);
    ProcessModule* getProcessModule() const;
    virtual void setProcessModule(ProcessModule* pProcessModule);
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

    virtual double computeFourierObservable();

private:
    pthread_mutex_t m_mutex;
};

#endif /* OBSERVABLE_H */
