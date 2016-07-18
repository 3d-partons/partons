#ifndef ALL_COS_0_PHI_H
#define ALL_COS_0_PHI_H

/**
 * @file AllCos0phi.h
 * @author Luca COLANERI (IPNO)
 * @date July 18, 2015
 * @version 1.0
 */

#include <string>
#include <vector>

#include "FourierObservable.h"

/**
 * @class AllCos0phi
 *
 * @brief
 */
class AllCos0phi: public FourierObservable {
public:
    static const unsigned int classId; ///< Unique ID to automatically register the class in the registry.

    AllCos0phi(const std::string &className);
    virtual ~AllCos0phi();

    /**
     * Virtual clone function to allow factory to copy all derived members
     * @return
     */
    virtual AllCos0phi* clone() const;

    virtual double compute();

    virtual void resolveObjectDependencies();

protected:
    /**
     * Copy constructor
     */
    AllCos0phi(const AllCos0phi &other);

    NumA::FunctionType1D* m_pFunctionToIntegrateAllObservable;

    virtual double functionToIntegrateAllObservable(double x,
            std::vector<double> params);

    void initFunctorsForIntegrations();
};

#endif /* ALL_COS_0_PHI_H */
