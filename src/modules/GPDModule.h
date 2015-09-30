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

#include "../beans/gpd/GPDType.h"
#include "../beans/parton_distribution/PartonDistribution.h"
#include "../ModuleObject.h"

class GPDEvolutionModule;
class GPDResult;

class GPDModule: public ModuleObject {
public:

    static const std::string GPD_TYPE;

    //TODO delete
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
    void configure(ParameterList parameters);

    /**
     * Virtual method, computes GPD with some input parameters.
     *
     * @param x Bjorken variable
     * @param xi longitudinal momentum
     * @param t momentum transfer (Mandelstam variable)
     * @param MuF2 Factorisation
     * @param MuR2 Re-normalisation
     * @param gpdComputeType H, Ht, E, Et, ... or ALL. See GPDComputeType for more details.
     *
     * @return Return results in an GPDOutputData class.
     * Contains GPD results for each flavor of quarks and for each GPDs (H, Ht, E, Et, ...) if computable.
     */
    virtual GPDResult compute(double x, double xi, double t, double MuF2,
            double MuR2, GPDType::Type gpdType);

    virtual PartonDistribution computeH();
    virtual PartonDistribution computeE();
    virtual PartonDistribution computeHt();
    virtual PartonDistribution computeEt();

    virtual std::string toString();

    // ##### GETTERS & SETTERS #####

    unsigned int getNf() const;
    void setNf(unsigned int nf);
    const GPDEvolutionModule* getEvolQcdModule() const;
    void setEvolQcdModule(GPDEvolutionModule* pEvolQcdModule);
    double getMuF2Ref() const;
    double getMuF2() const;
    void setMuF2(double muF2);
    double getMuR2() const;
    void setMuR2(double muR2);
    double getT() const;
    void setT(double t);
    double getX() const;
    void setX(double x);
    double getXi() const;
    void setXi(double xi);

protected:
    double m_x;
    double m_xi;
    double m_t;
    double m_MuF2;
    double m_MuR2;
    GPDType::Type m_gpdType;

    //TODO initialize
    double m_MuF2_ref;
    //TODO faire référence à la revue pour la notation (petit) nf
    unsigned int m_nf;

    //PDFModule* m_pPDFModule;
    GPDEvolutionModule* m_pGPDEvolutionModule;

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
     * @param MuF2
     * @param MuR2
     * @param gpdComputeType
     */
    void preCompute(double x, double xi, double t, double MuF2, double MuR2,
            GPDType::Type gpdType);

    std::map<GPDType::Type, PartonDistribution (GPDModule::*)()> m_listGPDComputeTypeAvailable;
    std::map<GPDType::Type, PartonDistribution (GPDModule::*)()>::iterator m_it;
};

#endif /* GPD_MODULE_H */
