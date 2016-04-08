#ifndef AUL_SIN_PHI_H
#define AUL_SIN_PHI_H

/**
 * @file AulSinPhi.h
 * @author Bryan BERTHOU (CEA Saclay)
 * @date 28 September 2015
 * @version 1.0
 *
 * @class AulSinPhi
 *
 * @brief
 */

#include <string>
#include <vector>

#include "FourierObservable.h"

class Aul;

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

    Aul* m_pAulObservable;

    NumA::FunctionType1D* m_pFunctionToIntegrateNumObservable;
    NumA::FunctionType1D* m_pFunctionToIntegrateDenObservable;

    virtual double functionToIntegrateNumObservable(double x,
            std::vector<double> params);
    virtual double functionToIntegrateDenObservable(double x,
            std::vector<double> params);

    void initFunctorsForIntegrations();
};

#endif /* AUL_SIN_PHI_H */
