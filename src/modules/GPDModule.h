#ifndef GPD_MODULE_H
#define GPD_MODULE_H

/**
 * @file GPDModule.h
 * @author Bryan BERTHOU (SPhN / CEA Saclay)
 * @date 06 Aout 2014
 * @version 1.0
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

    enum Keys {
        e_MUF_REF_STRING_KEY
    };

    /**
     * Constructor.
     * See BaseObject class for more info about input parameter.
     *
     * @param className class's name of child class.
     */
    GPDModule(const std::string &className);

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
     * Provides a generic method to configure all types of modules by passing a Parameters object.
     * (See ModuleObject class for more info).
     *
     * @param parameters
     */
    void configure(Parameters parameters);

    /**
     * Virtual method, computes GPD with some input parameters.
     *
     * @param x Bjorken variable
     * @param xi longitudinal momentum
     * @param t momentum transfer (Mandelstam variable)
     * @param MuF Factorisation
     * @param MuR Re-normalisation
     * @param gpdComputeType H, Ht, E, Et, ... or ALL. See GPDComputeType for more details.
     *
     * @return Return results in an GPDOutputData class.
     * Contains GPD results for each flavor of quarks and for each GPDs (H, Ht, E, Et, ...) if computable.
     */
    virtual GPDOutputData compute(const double &x, const double &xi,
            const double &t, const double &MuF, const double &MuR,
            GPDComputeType::Type gpdComputeType);

    virtual GPDOutputData computeWithEvolution(const double &x,
            const double &xi, const double &t, const double &MuF,
            const double &MuR, GPDComputeType::Type gpdComputeType);

    virtual GPDResultData computeH();
    virtual GPDResultData computeE();
    virtual GPDResultData computeHt();
    virtual GPDResultData computeEt();

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
    GPDComputeType::Type m_gpdComputeType;

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

    /**
     *
     *
     * @param x
     * @param xi
     * @param t
     * @param MuF
     * @param MuR
     * @param gpdComputeType
     */
    void preCompute(const double &x, const double &xi, const double &t,
            const double &MuF, const double &MuR,
            GPDComputeType::Type gpdComputeType);

    std::map<GPDComputeType::Type, GPDResultData (GPDModule::*)()> m_listGPDComputeTypeAvailable;
    std::map<GPDComputeType::Type, GPDResultData (GPDModule::*)()>::iterator m_it;
private:

    GPDOutputData compute(bool evolution);
};

#endif /* GPD_MODULE_H */
