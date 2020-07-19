#ifndef COLLINEAR_DISTRIBUTION_LHAPDF_H
#define COLLINEAR_DISTRIBUTION_LHAPDF_H

/**
 * @file CollinearDistributionLHAPDF.h
 * @author Valerio BERTONE (CEA Saclay)
 * @date July 18, 2020
 * @version 1.0
 */

#include <ElementaryUtils/parameters/Parameters.h>
#include <LHAPDF/LHAPDF.h>

#include "CollinearDistributionModule.h"

namespace PARTONS {

/**
 * @class CollinearDistributionLHAPDF
 *
 * @brief A module that encapsulates a set provided through the LHAPDF
 * interface.
 */
class CollinearDistributionLHAPDF: public CollinearDistributionModule {
public:

    static const std::string PARAM_NAME_SET_NAME;
    static const std::string PARAM_NAME_MEMBER;
    static const std::string PARAM_NAME_DIST_TYPE;

    static const unsigned int classId; ///< Unique ID to automatically register the class in the registry.

    /**
     * Constructor.
     * See BaseObject::BaseObject and ModuleObject::ModuleObject for more details.
     *
     * @param className name of child class.
     */
    CollinearDistributionLHAPDF(const std::string &className);

    /**
     * Default destructor.
     */
    virtual ~CollinearDistributionLHAPDF();

    virtual CollinearDistributionLHAPDF* clone() const;

    virtual void configure(const ElemUtils::Parameters &parameters);

    virtual std::string toString() const;

    // ##### GETTERS & SETTERS #####

    std::string getSetName() const;
    int getMember() const;
    CollinearDistributionType::Type getType() const;
    LHAPDF::PDF* getSet() const;

    void setSetName(const std::string &setname);
    void setMember(const int &member);
    void setType(const CollinearDistributionType::Type &type);

protected:

    /**
     * Copy constructor.
     *
     * Used by the factory.
     *
     * @param other
     */
    CollinearDistributionLHAPDF(const CollinearDistributionLHAPDF& other);

    virtual void isModuleWellConfigured();
    virtual void initModule();

    virtual PartonDistribution computeUnpolPDF();
    virtual PartonDistribution computePolPDF();
    virtual PartonDistribution computeUnpolFF();
    virtual PartonDistribution computePolFF();
 
protected:

    std::string m_setName;
    int m_member;
    CollinearDistributionType::Type m_type;
    LHAPDF::PDF* m_set;

};

} /* namespace PARTONS */

#endif /* COLLINEAR_DISTRIBUTION_LHAPDF_H */
