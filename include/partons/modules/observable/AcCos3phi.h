#ifndef AC_COS_3_PHI_H
#define AC_COS_3_PHI_H

/**
 * @file AcCos3phi.h
 * @author Luca COLANERI (IPNO)
 * @date July 18, 2016
 * @version 1.0
 */

#include <string>
#include <vector>

#include "FourierObservable.h"

/**
 * @class AcCos3phi
 *
 * @brief
 */
class AcCos3phi: public FourierObservable {
public:
    static const unsigned int classId; ///< Unique ID to automatically register the class in the registry.

    AcCos3phi(const std::string &className);
    virtual ~AcCos3phi();

    /**
     * Virtual clone function to allow factory to copy all derived members
     * @return
     */
    virtual AcCos3phi* clone() const;

    virtual double compute();

    virtual void resolveObjectDependencies();

protected:
    /**
     * Copy constructor
     */
    AcCos3phi(const AcCos3phi &other);

    NumA::FunctionType1D* m_pFunctionToIntegrateAcObservable;

    virtual double functionToIntegrateAcObservable(double x,
            std::vector<double> params);

    void initFunctorsForIntegrations();
};

#endif /* AC_COS_3_PHI_H */
