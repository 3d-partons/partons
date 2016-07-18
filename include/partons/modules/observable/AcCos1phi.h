#ifndef AC_COS_1_PHI_H
#define AC_COS_1_PHI_H

/**
 * @file AcCos1phi.h
 * @author Luca COLANERI (IPNO)
 * @date July 18, 2016
 * @version 1.0
 */

#include <string>
#include <vector>

#include "FourierObservable.h"


class AcCos1phi: public FourierObservable {
public:
    static const unsigned int classId; ///< Unique ID to automatically register the class in the registry.

    AcCos1phi(const std::string &className);
    virtual ~AcCos1phi();

    /**
     * Virtual clone function to allow factory to copy all derived members
     * @return
     */
    virtual AcCos1phi* clone() const;

    virtual double compute();

    virtual void resolveObjectDependencies();

protected:
    /**
     * Copy constructor
     */
    AcCos1phi(const AcCos1phi &other);

    NumA::FunctionType1D* m_pFunctionToIntegrateAcObservable;

    virtual double functionToIntegrateAcObservable(double x,
            std::vector<double> params);

    void initFunctorsForIntegrations();
};

#endif /* AC_COS_1_PHI_H */
