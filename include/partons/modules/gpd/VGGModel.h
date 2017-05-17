#ifndef VGG_MODEL_H
#define VGG_MODEL_H

/**
 * @file VGGModel.h
 * @author  Michel Guidal (IPNO)
 * @author Pawel Sznajder (IPNO)
 * @date December 10, 2015
 * @version 1.0
 */

#include <ElementaryUtils/parameters/Parameters.h>
#include <string>
#include <vector>

#include "../../beans/gpd/GPDType.h"
#include "../GPDModule.h"
#include "../MathIntegratorModule.h"

class MSTWPDF;
namespace NumA {
class FunctionType1D;
} /* namespace NumA */

/**
 * @class VGGModel
 *
 * For the reference see:
 * - Prog. Part. Nucl. Phys. 47, 401 (2001)
 * - Phys. Rev. Lett. 80 5064 (1998).
 * - Phys. Rev. D 60, 094017 (1999).
 * - Phys. Rev. D 72, 054013 (2005).
 *
 * Module based on the original code received from M. Guidal as a private communication.
 */
class VGGModel: public GPDModule, public MathIntegratorModule {

public:

    static const unsigned int classId; ///< ID assigned by BaseObjectRegistry.

    /** Constructor.
     @param className Name of this class
     */
    VGGModel(const std::string &className);

    /**
     * Default destructor.
     */
    virtual ~VGGModel();
    virtual VGGModel* clone() const;
    virtual void resolveObjectDependencies();
    virtual void configure(const ElemUtils::Parameters &parameters);
    virtual std::string toString();

protected:

    /** Copy constructor
     @param other Object to be copied
     */
    VGGModel(const VGGModel& other);

    virtual void isModuleWellConfigured();
    virtual void initModule();

    virtual PartonDistribution computeH();
    virtual PartonDistribution computeE();
    virtual PartonDistribution computeHt();
    virtual PartonDistribution computeEt();

private:

    /** Quark flavors.
     */
    enum flavour {
        UNDEFINED = 0,
        UP_VAL = 1,
        UP_SEA = 2,
        DOWN_VAL = 3,
        DOWN_SEA = 4,
        STRANGE = 5
    };

    const double kappa_u; ///< Anomalous magnetic moment for quarks u.
    const double kappa_d; ///< Anomalous magnetic moment for quarks d.

    const double b_profile_val; ///< Parameter describing t dependence of the Regge residue for valence quarks.
    const double b_profile_sea; ///< Parameter describing t dependence of the Regge residue for see quarks.
    const double alphap_val; ///< Slope of Ragge trajectory for valence quarks.
    const double alphap_sea; ///< Slope of Ragge trajectory for for sea quarks.
    const double eta_e_largex_u_s; ///< Shape parameter for GPD E^uVal.
    const double eta_e_largex_d_s; ///< Shape parameter for GPD E^dVal.
    const double g_AXIAL; ///< Axial coupling constant of the nucleon.

    MSTWPDF* m_Forward; ///< Pointer to MSTW PDFs.

    GPDType gpd_s5; ///< GPD Type for the current computation (internal variables used for the integration).
    flavour flavour_s5; ///< Flavor for the current computation  (internal variables used for the integration).
    double x_s5; ///< Value of x for the current computation (internal variables used for the integration).

    /** Evaluation of %double distribution for unpolarized GPDs.
     */
    double offforward_distr();

    /** Evaluation of %double distribution for polarized GPDs.
     */
    double offforward_pol_distr();

    /** %Double distribution profile function.
     @param beta, alpha %Double distribution variables.
     @param b_profile Profile parameter.
     \sa b_profile_val, b_profile_sea.
     */
    double symm_profile_function(double beta, double alpha, double b_profile);

    /** %Double distribution function for GPD H.
     @param beta, alpha %Double distribution variables.
     */
    double symm_double_distr_reggeH(double beta, double alpha);

    /** Wrapper for integration of %double distribution for GPD H and x.
     @param alpha %Double distribution variable.
     @param par Additional parameters (not used).
     */
    double int_symm_double_distr_reggeH(double alpha, std::vector<double> par);

    /** Wrapper for integration of %double distribution for GPD H and -x.
     @param alpha %Double distribution variable.
     @param par Additional parameters (not used).
     */
    double int_symm_double_distr_reggeMxH(double alpha,
            std::vector<double> par);

    /** %Double distribution function for GPD E.
     @param beta, alpha %Double distribution variables.
     */
    double symm_double_distr_reggeE(double beta, double alpha);

    /** Wrapper for integration of %double distribution for GPD E and x.
     @param alpha %Double distribution variable.
     @param par Additional parameters (not used).
     */
    double int_symm_double_distr_reggeE(double alpha, std::vector<double> par);

    /** Wrapper for integration of %double distribution for GPD E and -x.
     @param alpha %Double distribution variable.
     @param par Additional parameters (not used).
     */
    double int_symm_double_distr_reggeMxE(double alpha,
            std::vector<double> par);

    /** Normalization function for GPD E.
     @param beta %Double distribution variable.
     @param par Additional parameters (not used).
     */
    double int_mom2_up_valence_e(double beta, std::vector<double> par);

    /** %Double distribution function for GPD Ht.
     @param beta, alpha %Double distribution variables.
     */
    double symm_double_distr_reggeHt(double beta, double alpha);

    /** Wrapper for integration of %double distribution for GPD Ht and x.
     @param alpha %Double distribution variable.
     @param par Additional parameters (not used).
     */
    double int_symm_double_distr_reggeHt(double alpha, std::vector<double> par);

    /** Wrapper for integration of %double distribution for GPD Ht and -x.
     @param alpha %Double distribution variable.
     @param par Additional parameters (not used).
     */
    double int_symm_double_distr_reggeMxHt(double alpha,
            std::vector<double> par);

    /** Unpolarized PDF for quarks uVal, MRS93 (used only for cross-check purpose).
     @param x Fractional momentum of parton.
     */
    double test_pdf_up_val(double x);

    /** Unpolarized PDF for quarks dVal, MRS93 (used only for cross-check purpose).
     @param x Fractional momentum of parton.
     */
    double test_pdf_down_val(double x);

    /** Unpolarized PDF for quarks uSea, MRS93 (used only for cross-check purpose).
     @param x Fractional momentum of parton.
     */
    double test_pdf_up_bar(double x);

    /** Unpolarized PDF for quarks dSea, MRS93 (used only for cross-check purpose).
     @param x Fractional momentum of parton.
     */
    double test_pdf_down_bar(double x);

    /** Unpolarized PDF for quarks s, MRS93 (used only for cross-check purpose).
     @param x Fractional momentum of parton.
     */
    double test_pdf_sea(double x);

    /** Polarized PDF for quarks uVal, MRST98.
     @param x Fractional momentum of parton.
     */
    double pol_up_valence(double x);

    /** Polarized PDF for quarks dVal, MRST98.
     @param x Fractional momentum of parton.
     */
    double pol_down_valence(double x);

    /** Polarized PDF for quarks uSea, MRST98.
     @param x Fractional momentum of parton.
     */
    double pol_up_bar(double x);

    /** Polarized PDF for quarks dSea, MRST98.
     @param x Fractional momentum of parton.
     */
    double pol_down_bar(double x);

    /** Polarized PDF for quarks s, MRST98.
     @param x Fractional momentum of parton.
     */
    double pol_strange_bar(double x);

    /** Axial form factor parameterization.
     @param Q_sqr FF variable.
     */
    double form_factor_G_A(double Q_sqr);

    /** Pseudoscalar form factor parameterization.
     @param t FF variable.
     */
    double form_factor_G_P(double t);

    NumA::FunctionType1D* m_pint_symm_double_distr_reggeH; ///< Functor related to symm_double_distr_reggeH()
    NumA::FunctionType1D* m_pint_symm_double_distr_reggeMxH; ///< Functor related to symm_double_distr_reggeMxH()
    NumA::FunctionType1D* m_pint_symm_double_distr_reggeE; ///< Functor related to symm_double_distr_reggeE()
    NumA::FunctionType1D* m_pint_symm_double_distr_reggeMxE; ///< Functor related to symm_double_distr_reggeMxE()
    NumA::FunctionType1D* m_pint_symm_double_distr_reggeHt; ///< Functor related to symm_double_distr_reggeHt()
    NumA::FunctionType1D* m_pint_symm_double_distr_reggeMxHt; ///< Functor related to symm_double_distr_reggeMxHt()
    NumA::FunctionType1D* m_pint_mom2_up_valence_e; ///< Functor related to mom2_up_valence_e()

    /** Initialize functors.
     */
    void initFunctorsForIntegrations();
};

#endif /* VGG_MODEL_H */
