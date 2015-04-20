#ifndef OBSERVABLE_SERVICE_H
#define OBSERVABLE_SERVICE_H

/**
 * @file ObservableService.h
 * @author Bryan BERTHOU (CEA Saclay)
 * @date 20 November 2014
 * @version 1.0
 *
 * @class ObservableService
 * @brief \<singleton\> Use for handle and compute some pre-configured CFF modules.
 */

#include <map>
#include <string>

#include "../beans/cff/CFFOutputData.h"
#include "Service.h"

class ObservableKinematic;

class DVCSModule;
class Observable;
class ObservableModule;

class ObservableService: public Service {
public:
    static const std::string ID; ///< Unique ID to self-register in the registry

    /**
     * Share a unique pointer of this class
     */
    static ObservableService* getInstance();

    /**
     * Default destructor
     */
    virtual ~ObservableService();

    std::map<double, double> computeDVCSObservableWithPhiDependency(
            DVCSModule* pDVCSModule, Observable* pObservable,
            ObservableKinematic observableKinematic,
            CFFOutputData cffOutputData);

    virtual void computeScenario(Scenario scenario);

private:
    /**
     * Private pointer of this class for a unique instance
     */
    static ObservableService* m_pInstance;

    /**
     * Private default constructor for a unique instance
     */
    ObservableService();
};

#endif /* OBSERVABLE_SERVICE_H */
