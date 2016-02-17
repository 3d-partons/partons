/**
 * @file RLModel.h
 * @author Nabil CHOUIKA (SPhN / CEA Saclay)
 * @date Jan 26, 2016
 * @version 1.0
 *
 * @class RLModel
 */

#ifndef RLMODEL_H_
#define RLMODEL_H_

#include <string>

#include "GapEquationSolverModule.h"

class RLModel: public GapEquationSolverModule {
public:
    static const unsigned int classId; ///< Unique ID to automatically register the class in the registry.

    RLModel(const std::string &className);
    virtual ~RLModel();

    virtual RLModel* clone() const;

protected:
    /**
     * Copy constructor
     *
     * Use by the factory
     *
     * @param other
     */
    RLModel(const RLModel& other);

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

#endif /* RLMODEL_H_ */
