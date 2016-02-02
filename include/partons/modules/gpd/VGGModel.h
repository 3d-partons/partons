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

class c_mstwpdf;

class VGGModel: public GPDModule, public MathIntegratorModule {

public:

    static const unsigned int classId;

    VGGModel(const std::string &className);

    virtual ~VGGModel();

    virtual VGGModel* clone() const;

    virtual void init();

    virtual void configure(ParameterList parameters);

    virtual std::string toString();

protected:

    VGGModel(const VGGModel& other);

    virtual void isModuleWellConfigured();
    virtual void initModule();

    virtual PartonDistribution computeH();
    virtual PartonDistribution computeE();

private:

    enum flavour {
        undefined, upv, upsea, dnv, dnsea
    }; //< flavour

    static const double eps_doubleint; //< step to skip x = 0 singularity
    static const double kappa_u; //< anomalous magnetic moment of u quarks
    static const double kappa_d; //< anomalous magnetic moment of d quarks

    double b_profile_val; //< b slope for valence quarks
    double b_profile_sea; //< b slope for sea quarks
    double alphap_val; //< Ragge parameter for valence quarks
    double alphap_sea; //< Ragge parameter for sea quarks
    double eta_e_largex_u_s; //< Shape parameter for GPD E^uVal
    double eta_e_largex_d_s; //< Shape parameter for GPD E^dVal

    c_mstwpdf* m_Forward;   //< pdfs

    GPDType gpd_s5;
    flavour flavour_s5; //< flavour - internal variables used in integration
    double x_s5; //< x - internal variables used in integration

    double offforward_distr(); //< double distribution scheme
    double symm_profile_function(double beta, double alpha, double b_profile); //< profile function

    double symm_double_distr_reggeH(double beta, double alpha); //< double distribution function for GPD H
    double int_symm_double_distr_reggeH(std::vector<double> alpha,
            std::vector<double> par); //< wrapper for integration of double distribution at x for GPD H
    double int_symm_double_distr_reggeMxH(std::vector<double> alpha,
            std::vector<double> par); //< wrapper for integration of double distribution at -x for GPD H

    double symm_double_distr_reggeE(double beta, double alpha); //< double distribution function for GPD E
    double int_symm_double_distr_reggeE(std::vector<double> alpha,
            std::vector<double> par); //< wrapper for integration of double distribution at x for GPD E
    double int_symm_double_distr_reggeMxE(std::vector<double> alpha,
            std::vector<double> par); //< wrapper for integration of double distribution at -x for GPD E
    double int_mom2_up_valence_e(std::vector<double> x,
            std::vector<double> par);   //< normalization function for GPD E

    double test_pdf_down_val(double x); //< pdfs for cross-check purpose, parametrization of Martin, Roberts, Stirling, PRD 47 (1993) 867
    double test_pdf_up_val(double x);
    double test_pdf_sea(double x);
    double test_pdf_up_bar(double x);
    double test_pdf_down_bar(double x);
};

#endif /* VGGMODEL_H_ */
