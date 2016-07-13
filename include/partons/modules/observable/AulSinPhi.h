#ifndef AUL_SIN_PHI_H
#define AUL_SIN_PHI_H

/**
 * @file AulSinPhi.h
 * @author Bryan BERTHOU (SPhN / CEA Saclay)
 * @date September 28, 2015
 * @version 1.0
 */

#include <string>
#include <vector>

#include "FourierObservable.h"

/**
 * @class AulSinPhi
 *
 * @brief
 */
class AulSinPhi: public FourierObservable {
public:
    static const unsigned int classId; ///< Unique ID to automatically register the class in the registry.

    AulSinPhi(const std::string &className);
    virtual ~AulSinPhi();

    /**
     * Virtual clone function to allow factory to copy all derived members
     * @return
     */
    virtual AulSinPhi* clone() const;

    virtual double compute();

    virtual void resolveObjectDependencies();

protected:
    /**
     * Copy constructor
     */
    AulSinPhi(const AulSinPhi &other);

    NumA::FunctionType1D* m_pFunctionToIntegrateObservable;

    virtual double functionToIntegrateObservable(double x,
            std::vector<double> params);

    void initFunctorsForIntegrations();
};

#endif /* AUL_SIN_PHI_H */
