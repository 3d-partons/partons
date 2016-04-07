#ifndef AC_COS_2_PHI_H
#define AC_COS_2_PHI_H

/**
 * @file AcCos2phi.h
 * @author Bryan BERTHOU (CEA Saclay)
 * @date 28 September 2015
 * @version 1.0
 *
 * @class AcCos2phi
 *
 * @brief
 */

#include <string>
#include <vector>

#include "FourierObservable.h"

class Observable;

class AcCos2phi: public FourierObservable {
public:
    static const unsigned int classId; ///< Unique ID to automatically register the class in the registry.

    AcCos2phi(const std::string &className);
    virtual ~AcCos2phi();

    /**
     * Virtual clone function to allow factory to copy all derived members
     * @return
     */
    virtual AcCos2phi* clone() const;

    virtual double compute();

protected:
    /**
     * Copy constructor
     */
    AcCos2phi(const AcCos2phi &other);

    Observable* m_pAcObservable;

    NumA::FunctionType1D* m_pFunctionToIntegrate;

    virtual double functionToIntegrate(double x, std::vector<double> params);

    void initFunctorsForIntegrations();
};

#endif /* AC_COS_2_PHI_H */
