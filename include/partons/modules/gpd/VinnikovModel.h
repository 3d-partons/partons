/*
 * VinnikovModel.h
 *
 *  Created on: Oct 1, 2015
 *      Author: Pawel Sznajder (NCBJ)
 */

#ifndef VINNIKOVMODEL_H_
#define VINNIKOVMODEL_H_

#include <string>

#include "../GPDModule.h"

class VinnikovModel: public GPDModule {

public:

    static const unsigned int classId;
    VinnikovModel(const std::string &className);
    virtual ~VinnikovModel();
    virtual VinnikovModel* clone() const;
    virtual void init();
    virtual void configure(const ElemUtils::Parameters &parameters);
    virtual std::string toString();

protected:

    VinnikovModel(const VinnikovModel& other);

    virtual void isModuleWellConfigured();
    virtual void initModule();

    virtual PartonDistribution computeH();
    virtual PartonDistribution computeHt();

private:

    const double EPS_BETR;
    const int SIMP_INT;

    double gpdh(int const i_part, double x, double const xi, double const t,
            int const n_par, double* const param);

    double gpdh_pol(int const i_part, double x, double const xi, double const t,
            int const n_par, double* const param);

    double dd1(int const i_part, double const beta, double const x,
            double const xi, double const t, int const n_par,
            double* const param);

    double dd2(int const i_part, double const beta, double const x,
            double const xi, double const t, int const n_par,
            double* const param);

    double dd3(int const i_part, double const beta, double const x,
            double const xi, double const t, int const n_par,
            double* const param);

    double dd1_pol(int const i_part, double const beta, double const x,
            double const xi, double const t, int const n_par,
            double* const param);

    double dd2_pol(int const i_part, double const beta, double const x,
            double const xi, double const t, int const n_par,
            double* const param);

    double dd3_pol(int const i_part, double const beta, double const x,
            double const xi, double const t, int const n_par,
            double* const param);

    double uval(double const x, double const t);

    double dval(double const x, double const t);

    double usea(double const x, double const t);

    double dsea(double const x, double const t);

    double ssea(double const x, double const t);

    double glu(double const x, double const t);

    double uval_pol(double const x, double const t);

    double dval_pol(double const x, double const t);

    double usea_pol(double const x, double const t);

    double dsea_pol(double const x, double const t);

    double ssea_pol(double const x, double const t);

    double glu_pol(double const x, double const t);

    double dd_int_simp(double const bmin, double const bmax,
            double (VinnikovModel::*dd)(int, double, double, double, double,
                    int, double*), int const i_part, double const x,
            double const xi, double const t, int const n_par, double* param);

    double gammf(double const x);

    double prof_funct(double const b, double const beta, double const alpha);

    double power(double const x, double const y);

    int n_par;
    double param[6];

};

#endif /* VINNIKOVMODEL_H_ */
