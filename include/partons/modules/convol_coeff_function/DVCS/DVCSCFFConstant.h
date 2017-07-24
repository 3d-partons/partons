#ifndef DVCS_CFF_CONSTANT_H
#define DVCS_CFF_CONSTANT_H

/**
 * @file DVCSCFFConstant.h
 * @author Nabil CHOUIKA (SPhN / CEA Saclay)
 * @author Pawel Sznajder (IPNO)
 * @date 09 November 2014
 * @version 1.0
 */

#include <ElementaryUtils/parameters/GenericType.h>
#include <ElementaryUtils/parameters/Parameters.h>
#include <complex>
#include <map>
#include <string>

#include "../../../beans/gpd/GPDType.h"
#include "DVCSConvolCoeffFunctionModule.h"

/**
 * @class DVCSCFFConstant
 *
 * Module to set CFF values via automation or via configure() function.
 */
class DVCSCFFConstant: public DVCSConvolCoeffFunctionModule {

public:

    static const unsigned int classId; ///< Unique ID to automatically register the class in the registry.

    static const std::string PARAMETER_NAME_CFF_VALUES; ///< Key name to set several CFFs via automation.

    static const std::string PARAMETER_NAME_CFF_H_Re; ///< Key name to set real part of CFF H via automation.
    static const std::string PARAMETER_NAME_CFF_H_Im; ///< Key name to set imaginary part of CFF H via automation.
    static const std::string PARAMETER_NAME_CFF_E_Re; ///< Key name to set real part of CFF E via automation.
    static const std::string PARAMETER_NAME_CFF_E_Im; ///< Key name to set imaginary part of CFF E via automation.
    static const std::string PARAMETER_NAME_CFF_Ht_Re; ///< Key name to set real part of CFF Ht via automation.
    static const std::string PARAMETER_NAME_CFF_Ht_Im; ///< Key name to set imaginary part of CFF Ht via automation.
    static const std::string PARAMETER_NAME_CFF_Et_Re; ///< Key name to set real part of CFF Et via automation.
    static const std::string PARAMETER_NAME_CFF_Et_Im; ///< Key name to set imaginary part of CFF Et via automation.

    /**
     * Constructor.
     * See BaseObject::BaseObject and ModuleObject::ModuleObject for more details.
     * @param className Name of last child class.
     */
    DVCSCFFConstant(const std::string &className);

    virtual DVCSCFFConstant* clone() const;

    /**
     * Default destructor.
     */
    virtual ~DVCSCFFConstant();

    virtual void configure(const ElemUtils::Parameters &parameters);
    virtual std::complex<double> computeCFF();

    /**
     * Get map containing CFFs.
     */
    const std::map<GPDType::Type, std::complex<double> >& getCFFs() const;

    /**
     * Set map containing CFFs.
     * @param cffs Map to be set.
     */
    void setCFFs(const std::map<GPDType::Type, std::complex<double> >& cffs);

    /**
     * Get specific CFF.
     @param gpdType Type of CFF to be get.
     */
    const std::complex<double>& getCFF(GPDType::Type gpdType) const;

    /**
     * Set specific CFF.
     @param gpdType Type of CFF to be set.
     @param cff CFF to be set.
     */
    void setCFF(GPDType::Type gpdType, const std::complex<double>& cff);

protected:

    /**
     * Copy constructor.
     @param other Object to be copied.
     */
    DVCSCFFConstant(const DVCSCFFConstant &other);

    virtual void initModule();
    virtual void isModuleWellConfigured();

private:

    /**
     * Print error in configure in case of setting CFFs via ambiguous keys.
     @param key Name of key to help localize error.
     */
    void printErrorInConfigure(const std::string& key) const;

    /**
     * Set single value in configure.
     @param gpdType CFF type to be set.
     @param isRealPart True if real part to be set.
     @param value Value to be set.
     */
    void configureValue(GPDType::Type gpdType, bool isRealPart,
            const ElemUtils::GenericType& value);

    /**
     * CFFs stored by type of GPD.
     * See GPDType.
     */
    std::map<GPDType::Type, std::complex<double> > m_CFFs;
};

#endif /* DVCS_CFF_CONSTANT_H */
