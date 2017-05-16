#ifndef GPD_MODULE_H
#define GPD_MODULE_H

/**
 * @file GPDModule.h
 * @author Bryan BERTHOU (SPhN / CEA Saclay)
 * @date August 06, 2014
 * @version 1.0
 */

#include <ElementaryUtils/parameters/Parameters.h>
#include <map>
#include <string>

#include "../beans/automation/BaseObjectData.h"
#include "../beans/gpd/GPDKinematic.h"
#include "../beans/gpd/GPDType.h"
#include "../beans/List.h"
#include "../beans/parton_distribution/PartonDistribution.h"
#include "../ModuleObject.h"

class GPDEvolutionModule;

/**
 * @class GPDModule
 * @brief Abstract class that provides a skeleton to implement a Generalized Parton Distributions (GPD) module.
 */
class GPDModule: public ModuleObject {
public:

    static const std::string GPD_MODULE_CLASS_NAME;
    static const std::string GPD_TYPE;

    //TODO delete
    enum Keys {
        e_MUF_REF_STRING_KEY
    };

    /**
     * Constructor.
     * See BaseObject::BaseObject and ModuleObject::ModuleObject for more details.
     *
     * @param className name of child class.
     */
    GPDModule(const std::string &className);

    /**
     * Default destructor
     */
    virtual ~GPDModule();

    /**
     * Clone. See BaseObject::clone for more details.
     *
     * @return GPDModule pointer.
     */
    virtual GPDModule* clone() const = 0;

    virtual void run();

    void virtual configure(const ElemUtils::Parameters &parameters);

    /**
     * Virtual method, computes GPD with some input parameters.
     *
     * @param kinematic GPD kinematics object.
     * @param gpdType H, Ht, E, Et, ... or ALL. See GPDType for more details.
     * @param evolution Boolean to use evolution. Default: false.
     *
     * @return PartonDistribution object.
     * Contains results for each flavor of partons.
     */
    virtual PartonDistribution compute(const GPDKinematic &kinematic,
            GPDType gpdType, bool evolution = false);

    /**
     * Virtual method, computes GPD with some input parameters.
     *
     * @param x Bjorken variable
     * @param xi longitudinal momentum
     * @param t momentum transfer (Mandelstam variable)
     * @param MuF2 Factorization scale.
     * @param MuR2 Renormalization scale.
     * @param gpdType H, Ht, E, Et, ... or ALL. See GPDType for more details.
     * @param evolution Boolean to use evolution. Default: false.
     *
     * @return PartonDistribution object.
     * Contains results for each flavor of partons.
     */
    virtual PartonDistribution compute(double x, double xi, double t,
            double MuF2, double MuR2, GPDType::Type gpdType, bool evolution =
                    false);

    /**
     * This method can be implemented in the child class if the GPD H is available to compute.
     *
     * @return PartonDistribution object.
     * Contains results for each flavor of partons.
     */
    virtual PartonDistribution computeH();
    /**
     * This method can be implemented in the child class if the GPD E is available to compute.
     *
     * @return PartonDistribution object.
     * Contains results for each flavor of partons.
     */
    virtual PartonDistribution computeE();
    /**
     * This method can be implemented in the child class if the GPD Ht is available to compute.
     *
     * @return PartonDistribution object.
     * Contains results for each flavor of partons.
     */
    virtual PartonDistribution computeHt();
    /**
     * This method can be implemented in the child class if the GPD Et is available to compute.
     *
     * @return PartonDistribution object.
     * Contains results for each flavor of partons.
     */
    virtual PartonDistribution computeEt();

    virtual std::string toString() const;

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

    /**
     *
     * @return List of available GPDTypes in the model considered. This list is set in the child class.
     */
    List<GPDType> getListOfAvailableGPDTypeForComputation() const;

//    void prepareComputationConfiguration(
//            const List<ElemUtils::Parameter> &moduleNameList,
//            const unsigned int level = 0);

    virtual void prepareSubModules(
            const std::map<std::string, BaseObjectData>& subModulesData);

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
    virtual void preCompute(double x, double xi, double t, double MuF2, double MuR2,
            GPDType::Type gpdType);

    std::map<GPDType::Type, PartonDistribution (GPDModule::*)()> m_listGPDComputeTypeAvailable;
    std::map<GPDType::Type, PartonDistribution (GPDModule::*)()>::iterator m_it;

    // std::vector<GPDType::Type> m_listOfAvailableGPDTypeForComputation;

private:
//    void prepareGPDEvolutionModule(
//            const List<ElemUtils::Parameter> &moduleNameList,
//            unsigned int level = 0);
};

#endif /* GPD_MODULE_H */
