#ifndef GPD_BI05_H
#define GPD_BI05_H

/**
 * @file GPDBI05.h
 * @author Herve Moutarde (CEA/Irfu, Saclay)
 * @date November 11, 2014
 * @version 1.0
 */

#include <ElementaryUtils/parameters/Parameters.h>
#include <string>
#include <vector>
#include <LHAPDF/LHAPDF.h>

#include "GPDModule.h"

namespace NumA {
class FunctionType1D;
} /* namespace NumA */

namespace PARTONS {

/**
 * @class GPDBI05
 *
 * For the reference, see https://arxiv.org/pdf/hep-ph/0505263 
 *
 * Available GPD types: H.
 */
class GPDBI05: public GPDModule {

public:

    static const std::string PARAM_NAME_SET_NAME; ///< Tag to select PDF set name.
    static const std::string PARAM_NAME_MEMBER; ///< Tag to select PDF set member.

    static const unsigned int classId; ///< ID assigned by BaseObjectRegistry.

    ///< Integration parameters.
    struct IntegrationParameters{

        GPDBI05* m_pClass;
        double m_x, m_xi; 
        bool m_isGluon;
        QuarkFlavor::Type m_quarkType; bool m_isValence;
    };

    ///< Integration function.
    static double integrationFunction(double x, void* par);

    /**
     * Constructor.
     * See BaseObject::BaseObject and ModuleObject::ModuleObject for more details.
     *
     * @param className name of child class.
     */
    GPDBI05(const std::string &className);

    /**
     * Constructor.
     */
    virtual ~GPDBI05();
    virtual GPDBI05* clone() const;
    virtual void configure(const ElemUtils::Parameters &parameters);

    ///< Get PDF set name.
    const std::string& getSetName() const;

    ///< Set PDF set name.
    void setSetName(const std::string& setname);

    ///< Get PDF set member.
    int getMember() const;

    ///< Set PDF set member.
    void setMember(int member);

    double getQuarkForward(double x, QuarkFlavor::Type q, bool isValence) const;
    double getGluonForward(double x) const;
    double profile(double beta, double alpha, int n) const;
    double tDependace(double t) const;
    double integrate(IntegrationParameters* params) const;

protected:

    /** Copy constructor
     @param other Object to be copied
     */
    GPDBI05(const GPDBI05& other);

private:

    virtual void initModule();
    virtual PartonDistribution computeH();

    LHAPDF::PDF* m_Forward; ///< PDF set.
    std::string m_setName; ///< PDF set name.
    int m_member; ///< PDF set member.
};

} /* namespace PARTONS */

#endif /* GPD_BI05_H */
