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
#include <string>
#include <vector>

#include "DVCSConvolCoeffFunctionModule.h"

class DVCSConstantCFFModel: public DVCSConvolCoeffFunctionModule {
public:

    static const unsigned int classId; ///< Unique ID to automatically register the class in the registry.

    static const std::string CFF_VALUES;

    DVCSConstantCFFModel(const std::string &className);

    virtual DVCSConstantCFFModel* clone() const;

    virtual ~DVCSConstantCFFModel();

    virtual void configure(const ElemUtils::Parameters &parameters);

    virtual std::complex<double> computeCFF();

protected:
    /**
     * Copy constructor
     * @param other
     */
    DVCSConstantCFFModel(const DVCSConstantCFFModel &other);

    virtual void initModule();
    virtual void isModuleWellConfigured();

private:
    /**
     * CFFs stored by type of GPD
     * [2]=H, [3]=E, [4]=Ht, etc.
     * See GPDType.h
     */
    std::vector<std::complex<double> > m_CFF;
};

#endif /* DVCS_CONSTANT_CFF_MODULE_H */
