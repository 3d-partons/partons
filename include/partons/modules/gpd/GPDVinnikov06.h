/*
 * GPDVinnikov06.h
 *
 *  Created on: Oct 1, 2015
 *      Author: Pawel Sznajder (NCBJ)
 */

#ifndef GPD_VINNIKOV06_H
#define GPD_VINNIKOV06_H

#include <ElementaryUtils/parameters/Parameters.h>
#include <string>

#include "GPDModule.h"

namespace PARTONS {

/**
 * @class GPDVinnikov06
 *
 * Module based on the original code from http://hepdata.cedar.ac.uk/pdfs
 *
 * For the reference, see arxiv:hep-ph/0604248 @cite Vinnikov:2006xw.
 *
 * Available GPD types: H, Ht.
 */
class GPDVinnikov06: public GPDModule {

public:

    static const unsigned int classId; ///< ID assigned by BaseObjectRegistry.

    /**
     * Constructor.
     * See BaseObject::BaseObject and ModuleObject::ModuleObject for more details.
     *
     * @param className name of child class.
     */
    GPDVinnikov06(const std::string &className);

    /**
     * Default destructor.
     */
    virtual ~GPDVinnikov06();
    virtual GPDVinnikov06* clone() const;
    virtual void resolveObjectDependencies();
    virtual void configure(const ElemUtils::Parameters &parameters);
    virtual std::string toString() const;

protected:

    /** Copy constructor.
     @param other Object to be copied.
     */
    GPDVinnikov06(const GPDVinnikov06& other);

    virtual void isModuleWellConfigured();
    virtual void initModule();

    virtual PartonDistribution computeH();
    virtual PartonDistribution computeHt();

private:

    const double EPS_BETR; ///< Epsilon to avoid the evaluation for x=xi.
    const int SIMP_INT; ///< Number of steps used in the integration.

    int n_par; ///< Number of model parameters.
    double param[6]; ///< Model parameters (double distribution profile function parameter for all partons).

    /** Evaluation of GPD H.
     @param i_part Parton selection (0 for g, -1/+1 for uSea/uVal, -2/+2 for dSea/dVal, 3 for s).
     @param x, xi, t GPD variables.
     @param n_par Number of input parameters.
     @param param Input parameters.
     \sa n_par, param
     */
    double gpdh(int const i_part, double x, double const xi, double const t,
            int const n_par, double* const param);

    /** Evaluation of GPD Ht
     @param i_part Parton selection (0 for g, -1/+1 for uSea/uVal, -2/+2 for dSea/dVal, 3 for s).
     @param x, xi, t GPD variables.
     @param n_par Number of input parameters.
     @param param Input parameters.
     \sa n_par, param
     */
    double gpdh_pol(int const i_part, double x, double const xi, double const t,
            int const n_par, double* const param);

    /** %Double distribution for GPD H and x < -xi part
     @param i_part Parton selection (0 for g, -1/+1 for uSea/uVal, -2/+2 for dSea/dVal, 3 for s)
     @param beta %Double distribution variable.
     @param x, xi, t GPD variables.
     @param n_par Number of input parameters.
     @param param Input parameters.
     \sa n_par, param
     */
    double dd1(int const i_part, double const beta, double const x,
            double const xi, double const t, int const n_par,
            double* const param);

    /** %Double distribution for GPD H and -xi < x < xi part.
     @param i_part Parton selection (0 for g, -1/+1 for uSea/uVal, -2/+2 for dSea/dVal, 3 for s).
     @param beta %Double distribution variable.
     @param x, xi, t GPD variables.
     @param n_par Number of input parameters.
     @param param Input parameters.
     \sa n_par, param
     */
    double dd2(int const i_part, double const beta, double const x,
            double const xi, double const t, int const n_par,
            double* const param);

    /** %Double distribution for GPD H and x > xi part.
     @param i_part Parton selection (0 for g, -1/+1 for uSea/uVal, -2/+2 for dSea/dVal, 3 for s)
     @param beta %Double distribution variable.
     @param x, xi, t GPD variables.
     @param n_par Number of input parameters.
     @param param Input parameters.
     \sa n_par, param
     */
    double dd3(int const i_part, double const beta, double const x,
            double const xi, double const t, int const n_par,
            double* const param);

    /** %Double distribution for GPD Ht and x < -xi part.
     @param i_part Parton selection (0 for g, -1/+1 for uSea/uVal, -2/+2 for dSea/dVal, 3 for s).
     @param beta %Double distribution variable.
     @param x, xi, t GPD variables.
     @param n_par Number of input parameters.
     @param param Input parameters.
     \sa n_par, param
     */
    double dd1_pol(int const i_part, double const beta, double const x,
            double const xi, double const t, int const n_par,
            double* const param);

    /** %Double distribution for GPD Ht and x > xi part.
     @param i_part Parton selection (0 for g, -1/+1 for uSea/uVal, -2/+2 for dSea/dVal, 3 for s).
     @param beta %Double distribution variable.
     @param x, xi, t GPD variables.
     @param n_par Number of input parameters.
     @param param Input parameters.
     \sa n_par, param
     */
    double dd2_pol(int const i_part, double const beta, double const x,
            double const xi, double const t, int const n_par,
            double* const param);

    /** %Double distribution for GPD Ht and x > xi part.
     @param i_part Parton selection (0 for g, -1/+1 for uSea/uVal, -2/+2 for dSea/dVal, 3 for s).
     @param beta %Double distribution variable.
     @param x, xi, t GPD variables.
     @param n_par Number of input parameters.
     @param param Input parameters.
     \sa n_par, param
     */
    double dd3_pol(int const i_part, double const beta, double const x,
            double const xi, double const t, int const n_par,
            double* const param);

    /** Unpolarized PDF for quarks uVal convoluted with t-dependence.
     @param x, t GPD variables.
     */
    double uval(double const x, double const t);

    /** Unpolarized PDF for quarks dVal convoluted with t-dependence.
     @param x, t GPD variables.
     */
    double dval(double const x, double const t);

    /** Unpolarized PDF for quarks uSea convoluted with t-dependence.
     @param x, t GPD variables.
     */
    double usea(double const x, double const t);

    /** Unpolarized PDF for quarks dSea convoluted with t-dependence.
     @param x, t GPD variables.
     */
    double dsea(double const x, double const t);

    /** Unpolarized PDF for quarks s convoluted with t-dependence.
     @param x, t GPD variables.
     */
    double ssea(double const x, double const t);

    /** Unpolarized PDF for gluons convoluted with t-dependence.
     @param x, t GPD variables.
     */
    double glu(double const x, double const t);

    /** Polarized PDF for quarks uVal convoluted with t-dependence.
     @param x, t GPD variables.
     */
    double uval_pol(double const x, double const t);

    /** Polarized PDF for quarks dVal convoluted with t-dependence.
     @param x, t GPD variables.
     */
    double dval_pol(double const x, double const t);

    /** Polarized PDF for quarks uSea convoluted with t-dependence.
     @param x, t GPD variables.
     */
    double usea_pol(double const x, double const t);

    /** Polarized PDF for quarks dSea convoluted with t-dependence.
     @param x, t GPD variables.
     */
    double dsea_pol(double const x, double const t);

    /** Polarized PDF for quarks s convoluted with t-dependence.
     @param x, t GPD variables.
     */
    double ssea_pol(double const x, double const t);

    /** Polarized PDF for gluons convoluted with t-dependence.
     @param x, t GPD variables.
     */
    double glu_pol(double const x, double const t);

    /** Integration for %double distribution.
     @param bmin Integration limit min.
     @param bmax Integration limit max.
     @param dd %Double distribution function to be integrated.
     @param i_part Parton selection (0 for g, -1/+1 for uSea/uVal, -2/+2 for dSea/dVal, 3 for s).
     @param x, xi, t GPD variables.
     @param n_par Number of input parameters.
     @param param Input parameters.
     \sa n_par, param
     */
    double dd_int_simp(double const bmin, double const bmax,
            double (GPDVinnikov06::*dd)(int, double, double, double, double,
                    int, double*), int const i_part, double const x,
            double const xi, double const t, int const n_par, double* param);

    /** Gamma function.
     @param x Argument.
     */
    double gammf(double const x);

    /** %Double distribution profile function.
     @param b Profile parameter.
     @param beta, alpha %Double distribution variables.
     */
    double prof_funct(double const b, double const beta, double const alpha);

    /** Exponentiation function.
     @param x, y Exponentiation parameters x^y.
     */
    double power(double const x, double const y);
};

} /* namespace PARTONS */

#endif /* GPD_VINNIKOV06_H */
