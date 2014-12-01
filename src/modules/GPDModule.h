#ifndef GPD_MODULE_H
#define GPD_MODULE_H

/**
 * @file GPDModule.h
 * @author Bryan BERTHOU (CEA Saclay)
 * @date 06 Aout 2014
 * @version 1.0
 *
 * Last update : 24 September 2014
 *
 * @class GPDModule
 * @brief Abstract class that provides skeleton to implement a Generalized Parton Distributions (GPD) module.
 */

#include <map>
#include <string>

#include "../beans/gpd/GPDComputeType.h"
#include "../beans/gpd/GPDResultData.h"
#include "ModuleObject.h"

class GPDResultData;
class GPDOutputData;
//class PDFModule;
class EvolQCDModule;

//typedef GPDResultData (GPDModule::*GPDComputeFunction)(GPDComputeType::Type);

class GPDModule: public ModuleObject {
public:
    /**
     * Default constructor
     */
    GPDModule(const std::string &moduleID);

    /**
     * Default destructor
     */
    virtual ~GPDModule();

    /**
     * Clone
     *
     * @return
     */
    virtual GPDModule* clone() const = 0;

    /**
     * Virtual method. \n
     * Compute GPD with some input parameters.
     *
     * @param _x : Bjorken variable
     * @param _xi : longitudinal momentum
     * @param _t : momentum transfer (Mandelstam variable)
     * @param _MuF : Factorisation
     * @param _MuR : Re-normalisation
     * @param _gpdComputeType : H, Ht, E, Et, ... or ALL. See GPDComputeType for more details.
     *
     * @return Return results in an GPDOutputData class. \n
     * Contains GPD results for each flavor of quarks and for each GPDs (H, Ht, E, Et, ...) if computable. \n
     */
    virtual GPDOutputData compute(const double &_x, const double &_xi,
            const double &_t, const double &_MuF, const double &_MuR,
            GPDComputeType::Type gpdComputeType);

    virtual GPDOutputData computeWithEvolution(const double &_x,
            const double &_xi, const double &_t, const double &_MuF,
            const double &_MuR, GPDComputeType::Type gpdComputeType);

    virtual GPDResultData computeH()=0;
    virtual GPDResultData computeE()=0;
    virtual GPDResultData computeHt()=0;
    virtual GPDResultData computeEt()=0;

    virtual std::string toString();

    // ##### GETTERS & SETTERS #####

    double getNf() const;
    void setNf(double nf);
    const EvolQCDModule* getEvolQcdModule() const;
    void setEvolQcdModule(EvolQCDModule* pEvolQcdModule);
    double getMuFRef() const;

protected:
    double m_x;
    double m_xi;
    double m_t;
    double m_MuF;
    double m_MuR;

    //TODO initialize
    double m_MuF_ref;
    //TODO faire référence à la revue pour la notation (petit) nf
    double m_nf;

    //PDFModule* m_pPDFModule;
    EvolQCDModule* m_pEvolQCDModule;

    /**
     * Copy constructor
     *
     * @param other
     */
    GPDModule(const GPDModule &other);

    virtual void initModule();
    virtual void isModuleWellConfigured();

    void preCompute(const double &x, const double &xi, const double &t,
            const double &MuF, const double &MuR);

    std::map<GPDComputeType::Type, GPDResultData (GPDModule::*)()> m_listGPDComputeTypeAvailable;
    std::map<GPDComputeType::Type, GPDResultData (GPDModule::*)()>::iterator m_it;

};

#endif /* GPD_MODULE_H */
