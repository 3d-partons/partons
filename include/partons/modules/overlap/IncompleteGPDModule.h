/**
 * @file IncompleteGPDModule.h
 * @author Nabil Chouika (Irfu/SPhN, CEA Saclay)
 * @date 28 sept. 2016
 * @version 1.0
 */

#ifndef INCOMPLETEGPDMODULE_H_
#define INCOMPLETEGPDMODULE_H_

#include <ElementaryUtils/parameters/Parameters.h>
#include <map>
#include <string>

#include "../../beans/automation/BaseObjectData.h"
#include "../../beans/gpd/GPDKinematic.h"
#include "../../beans/gpd/GPDType.h"
#include "../../beans/List.h"
#include "../../beans/parton_distribution/PartonDistribution.h"
#include "../../ModuleObject.h"

/**
 * @class IncompleteGPDModule
 * @brief GPD in the DGLAP region obtained from an Overlap of Light-cone Wave-functions.
 */

class IncompleteGPDModule: public ModuleObject {
public:
    static const std::string DGLAP_REGION;
    static const std::string ERBL_REGION;

    /**
     * Constructor.
     * See BaseObject class for more info about input parameter.
     *
     * @param className class's name of child class.
     */
    IncompleteGPDModule(const std::string &className);

    /**
     * Default destructor
     */
    virtual ~IncompleteGPDModule();

    /**
     * Clone
     *
     * @return
     */
    virtual IncompleteGPDModule* clone() const = 0;

    /**
     * Provides a generic method to configure all types of modules by passing a Parameters object.
     * (See ModuleObject class for more info).
     *
     * @param parameters
     */
    void virtual configure(const ElemUtils::Parameters &parameters);

    virtual PartonDistribution compute(const GPDKinematic &kinematic,
            GPDType gpdType);

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
    virtual PartonDistribution compute(double x, double xi, double t,
            double MuF2, double MuR2, GPDType::Type gpdType);

    virtual PartonDistribution computeH();
    virtual PartonDistribution computeE();
    virtual PartonDistribution computeHt();
    virtual PartonDistribution computeEt();

    virtual std::string toString();

    void prepareSubModules(
            const std::map<std::string, BaseObjectData>& subModulesData);

    // ##### GETTERS & SETTERS #####

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

    List<GPDType> getListOfAvailableGPDTypeForComputation() const;

    // Kinematical region
    virtual bool isInKinematicRegion(double x, double xi) = 0; ///< Defines the limited kinematic region of the model
    const std::string& getKinematicRegion() const;
    void setKinematicRegion(const std::string& kinematicRegion);

protected:
    double m_x;
    double m_xi;
    double m_t;
    double m_MuF2;
    double m_MuR2;
    GPDType::Type m_gpdType;

    std::string m_kinematicRegion;

    /**
     * Copy constructor
     *
     * @param other
     */
    IncompleteGPDModule(const IncompleteGPDModule &other);

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

    std::map<GPDType::Type, PartonDistribution (IncompleteGPDModule::*)()> m_listGPDComputeTypeAvailable;
    std::map<GPDType::Type, PartonDistribution (IncompleteGPDModule::*)()>::iterator m_it;
};

#endif /* INCOMPLETEGPDMODULE_H_ */
