#ifndef AUL_SIN_2_PHI_H
#define AUL_SIN_2_PHI_H

/**
 * @file AulSin2phi.h
 * @author Luca COLANERI (IPNO)
 * @date July 18, 2016
 * @version 1.0
 */

#include <string>
#include <vector>

#include "FourierObservable.h"

/**
 * @class AulSin2phi
 *
 * @brief
 */
class AulSin2phi: public FourierObservable {
public:
    static const unsigned int classId; ///< Unique ID to automatically register the class in the registry.

    AulSin2phi(const std::string &className);
    virtual ~AulSin2phi();

    /**
     * Virtual clone function to allow factory to copy all derived members
     * @return
     */
    virtual AulSin2phi* clone() const;

    virtual double compute();

    virtual void resolveObjectDependencies();

protected:
    /**
     * Copy constructor
     */
    AulSin2phi(const AulSin2phi &other);

    NumA::FunctionType1D* m_pFunctionToIntegrateObservable;

    virtual double functionToIntegrateObservable(double x,
            std::vector<double> params);

    void initFunctorsForIntegrations();
};

#endif /* AUL_SIN_2_PHI_H */
