/**
 * @file BCSimplifiedModel.h
 * @author Nabil CHOUIKA (SPhN / CEA Saclay)
 * @date Feb 3, 2016
 * @version 1.0
 *
 * @class BCSimplifiedModel
 */

#ifndef BCSIMPLIFIEDMODEL_H_
#define BCSIMPLIFIEDMODEL_H_

#include <string>
#include <vector>

#include "GapEquationSolverModule.h"

class BCSimplifiedModel: public GapEquationSolverModule {
public:
    static const unsigned int classId; ///< Unique ID to automatically register the class in the registry.

    BCSimplifiedModel(const std::string &className);
    virtual ~BCSimplifiedModel();

    virtual BCSimplifiedModel* clone() const;

protected:
    /**
     * Copy constructor
     *
     * Use by the factory
     *
     * @param other
     */
    BCSimplifiedModel(const BCSimplifiedModel& other);

    virtual void initModule();
    virtual void isModuleWellConfigured();

    // Functions to integrate (virtual means model dependent and must be implemented in daughter class)
    // Angular integrals
    virtual double ThetaA_func(std::vector<double> z, std::vector<double> parameters);
    virtual double ThetaM_func(std::vector<double> z, std::vector<double> parameters);

    // Various functions (virtual means model dependent and must be implemented in daughter class)
    virtual double F1_func(double p2, double q2, double k2);
    virtual double F2_func(double p2, double q2, double k2);
};

#endif /* BCSIMPLIFIEDMODEL_H_ */
