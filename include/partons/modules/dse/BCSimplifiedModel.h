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

    // Various functions (virtual means model dependent and must be implemented in daughter class)
    virtual double F_A_func(double p2, double q2, double k2) const;
    virtual double F_M_func(double p2, double q2, double k2) const;
    virtual double H_A_func(double p2, double q2) const; ///< H_A function dependent on the iterated functions
    virtual double H_M_func(double p2, double q2) const; ///< H_M function dependent on the iterated functions
    virtual double H_A_deriv_a(double p2, double q2, unsigned int j) const;
    virtual double H_M_deriv_a(double p2, double q2, unsigned int j) const;
    virtual double H_A_deriv_b(double p2, double q2, unsigned int j) const;
    virtual double H_M_deriv_b(double p2, double q2, unsigned int j) const;
    virtual double H_A_func(double A_p2, double A_q2, double B_p2, double B_q2,
            double sigmaV_p2, double sigmaV_q2, double sigmaS_p2,
            double sigmaS_q2) const; ///< H_A function dependent on the iterated functions
    virtual double H_M_func(double A_p2, double A_q2, double B_p2, double B_q2,
            double sigmaV_p2, double sigmaV_q2, double sigmaS_p2,
            double sigmaS_q2) const; ///< H_M function dependent on the iterated functions
    virtual double H_A_deriv_a(double A_p2, double A_q2, double dA_p2,
            double dA_q2, double B_p2, double B_q2, double sigmaV_p2,
            double sigmaV_q2, double sigmaS_p2, double sigmaS_q2,
            double dsigmaV_a_p2, double dsigmaV_b_p2, double dsigmaV_a_q2,
            double dsigmaV_b_q2, double dsigmaS_a_p2, double dsigmaS_b_p2,
            double dsigmaS_a_q2, double dsigmaS_b_q2) const;
    virtual double H_M_deriv_a(double A_p2, double A_q2, double dA_p2,
            double dA_q2, double B_p2, double B_q2, double sigmaV_p2,
            double sigmaV_q2, double sigmaS_p2, double sigmaS_q2,
            double dsigmaV_a_p2, double dsigmaV_b_p2, double dsigmaV_a_q2,
            double dsigmaV_b_q2, double dsigmaS_a_p2, double dsigmaS_b_p2,
            double dsigmaS_a_q2, double dsigmaS_b_q2) const;
    virtual double H_A_deriv_b(double A_p2, double A_q2, double B_p2,
            double B_q2, double dB_p2, double dB_q2, double sigmaV_p2,
            double sigmaV_q2, double sigmaS_p2, double sigmaS_q2,
            double dsigmaV_a_p2, double dsigmaV_b_p2, double dsigmaV_a_q2,
            double dsigmaV_b_q2, double dsigmaS_a_p2, double dsigmaS_b_p2,
            double dsigmaS_a_q2, double dsigmaS_b_q2) const;
    virtual double H_M_deriv_b(double A_p2, double A_q2, double B_p2,
            double B_q2, double dB_p2, double dB_q2, double sigmaV_p2,
            double sigmaV_q2, double sigmaS_p2, double sigmaS_q2,
            double dsigmaV_a_p2, double dsigmaV_b_p2, double dsigmaV_a_q2,
            double dsigmaV_b_q2, double dsigmaS_a_p2, double dsigmaS_b_p2,
            double dsigmaS_a_q2, double dsigmaS_b_q2) const;
};

#endif /* BCSIMPLIFIEDMODEL_H_ */
