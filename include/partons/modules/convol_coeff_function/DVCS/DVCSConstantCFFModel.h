#ifndef DVCS_CONSTANT_CFF_MODULE_H
#define DVCS_CONSTANT_CFF_MODULE_H

/**
 * @file DVCSConstantCFFModel.h
 * @author Nabil CHOUIKA (SPhN / CEA Saclay)
 * @date 09 November 2014
 * @version 1.0
 *
 * @class DVCSConstantCFFModel
 *
 * @brief
 */

#include <complex>
#include <map>
#include <string>
//#include <vector>

#include "../../../beans/gpd/GPDType.h"
#include "DVCSConvolCoeffFunctionModule.h"

/**
 * Module to set CFF values via automation or via configure() function
 */
class DVCSConstantCFFModel: public DVCSConvolCoeffFunctionModule {

public:

    static const unsigned int classId; ///< Unique ID to automatically register the class in the registry.

    static const std::string CFF_VALUES; ///< Key name to set several CFFs via automation

    static const std::string CFF_H_Re; ///< Key name to set real part of CFF H via automation
    static const std::string CFF_H_Im; ///< Key name to set imaginary part of CFF H via automation
    static const std::string CFF_E_Re; ///< Key name to set real part of CFF E via automation
    static const std::string CFF_E_Im; ///< Key name to set imaginary part of CFF E via automation
    static const std::string CFF_Ht_Re; ///< Key name to set real part of CFF Ht via automation
    static const std::string CFF_Ht_Im; ///< Key name to set imaginary part of CFF Ht via automation
    static const std::string CFF_Et_Re; ///< Key name to set real part of CFF Et via automation
    static const std::string CFF_Et_Im; ///< Key name to set imaginary part of CFF Et via automation

    /**
     * Default constructor
     @param className Name of class
     */
    DVCSConstantCFFModel(const std::string &className);

    /**
     * Clone
     */
    virtual DVCSConstantCFFModel* clone() const;

    /**
     * Destructor
     */
    virtual ~DVCSConstantCFFModel();

    virtual void configure(const ElemUtils::Parameters &parameters);
    virtual std::complex<double> computeCFF();

protected:

    /**
     * Copy constructor
     @param other Object to be copied
     */
    DVCSConstantCFFModel(const DVCSConstantCFFModel &other);

    virtual void initModule();
    virtual void isModuleWellConfigured();

private:

    /**
     * Set single value in configure
     @param gpdType CFF type to be set
     @param isRealPart True if real part to be set
     @param value Value to be set
     */
    void configureValue(GPDType::Type gpdType, bool isRealPart, const ElemUtils::GenericType& value);

    /**
     * CFFs stored by type of GPD
     * See GPDType.h
     */
    std::map<GPDType::Type, std::complex<double> > m_CFF;
};

#endif /* DVCS_CONSTANT_CFF_MODULE_H */
