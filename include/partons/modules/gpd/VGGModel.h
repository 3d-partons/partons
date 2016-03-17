/*
 * VGGModel.h
 *
 *  Created on: Dec 10, 2015
 *      Author: Michel Guidal (IPNO) and Pawel Sznajder (IPNO)
 */

#ifndef VGGMODEL_H_
#define VGGMODEL_H_

#include <string>
#include <vector>

#include "../../beans/gpd/GPDType.h"
#include "../GPDModule.h"
#include "../MathIntegratorModule.h"

namespace NumA {
class FunctionType1D;
}
class c_mstwpdf;

class VGGModel: public GPDModule, public MathIntegratorModule {

public:

    static const unsigned int classId;

    VGGModel(const std::string &className);

    virtual ~VGGModel();

    virtual VGGModel* clone() const;

    virtual void init();

    virtual void configure(const ElemUtils::Parameters &parameters);

    virtual std::string toString();

protected:

    VGGModel(const VGGModel& other);

    virtual void isModuleWellConfigured();
    virtual void initModule();

    virtual PartonDistribution computeH();
    virtual PartonDistribution computeE();
    virtual PartonDistribution computeHt();
    virtual PartonDistribution computeEt();

private:

    enum flavour {
        UNDEFINED = 0,
        UP_VAL = 1,
        UP_SEA = 2,
        DOWN_VAL = 3,
        DOWN_SEA = 4,
        STRANGE = 5
    }; ///< flavour

    const double eps_doubleint; ///< step to skip x = 0 singularity
    const double kappa_u; ///< anomalous magnetic moment of u quarks
    const double kappa_d; ///< anomalous magnetic moment of d quarks

    const double b_profile_val; ///< b slope for valence quarks
    const double b_profile_sea; ///< b slope for sea quarks
    const double alphap_val; ///< Ragge parameter for valence quarks
    const double alphap_sea; ///< Ragge parameter for sea quarks
    const double eta_e_largex_u_s; ///< Shape parameter for GPD E^uVal
    const double eta_e_largex_d_s; ///< Shape parameter for GPD E^dVal
    const double g_AXIAL; ///< axial coupling constant of the nucleon

    c_mstwpdf* m_Forward;   ///< pdfs

    GPDType gpd_s5;
    flavour flavour_s5; ///< flavour - internal variables used in integration
    double x_s5; ///< x - internal variables used in integration

    double offforward_distr(); ///< double distribution scheme (unpolarised case)
    double offforward_pol_distr(); ///< double distribution scheme (polarised case)
    double symm_profile_function(double beta, double alpha, double b_profile); ///< profile function

    double symm_double_distr_reggeH(double beta, double alpha); ///< double distribution function for GPD H
    double int_symm_double_distr_reggeH(double alpha, std::vector<double> par); ///< wrapper for integration of double distribution at x for GPD H
    double int_symm_double_distr_reggeMxH(double alpha,
            std::vector<double> par); ///< wrapper for integration of double distribution at -x for GPD H

    double symm_double_distr_reggeE(double beta, double alpha); ///< double distribution function for GPD E
    double int_symm_double_distr_reggeE(double alpha, std::vector<double> par); ///< wrapper for integration of double distribution at x for GPD E
    double int_symm_double_distr_reggeMxE(double alpha,
            std::vector<double> par); ///< wrapper for integration of double distribution at -x for GPD E
    double int_mom2_up_valence_e(double x, std::vector<double> par); ///< normalization function for GPD E

    double symm_double_distr_reggeHt(double beta, double alpha); ///< double distribution function for GPD Ht
    double int_symm_double_distr_reggeHt(double alpha, std::vector<double> par); ///< wrapper for integration of double distribution at x for GPD Ht
    double int_symm_double_distr_reggeMxHt(double alpha,
            std::vector<double> par); ///< wrapper for integration of double distribution at -x for GPD Ht

    double test_pdf_down_val(double x); ///< unpolarized pdfs, MRS93 (for cross-check purpose)
    double test_pdf_up_val(double x);
    double test_pdf_sea(double x);
    double test_pdf_up_bar(double x);
    double test_pdf_down_bar(double x);

    double pol_up_valence(double x); ///< polarized pdfs, MRST98
    double pol_down_valence(double x);
    double pol_strange_bar(double x);
    double pol_up_bar(double x);
    double pol_down_bar(double x);

    double form_factor_G_A(double Q_sqr); ///< axial FF parameterisation
    double form_factor_G_P(double t);   ///< pseudoscalar FF parameterisation

    NumA::FunctionType1D* m_pint_symm_double_distr_reggeH;
    NumA::FunctionType1D* m_pint_symm_double_distr_reggeMxH;
    NumA::FunctionType1D* m_pint_symm_double_distr_reggeE;
    NumA::FunctionType1D* m_pint_symm_double_distr_reggeMxE;
    NumA::FunctionType1D* m_pint_symm_double_distr_reggeHt;
    NumA::FunctionType1D* m_pint_symm_double_distr_reggeMxHt;
    NumA::FunctionType1D* m_pint_mom2_up_valence_e;

    void initFunctorsForIntegrations();
};

#endif /* VGGMODEL_H_ */
