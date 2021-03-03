#ifndef GPDEVOLUTIONVINNIKOV_H
#define GPDEVOLUTIONVINNIKOV_H

/**
 * @file GPDEvolutionVinnikov.h
 * @author Pawel Sznajder (NCBJ)
 * @date June 01, 2017
 * @version 1.0
 */

#include <ElementaryUtils/parameters/Parameters.h>
#include <NumA/linear_algebra/matrix/MatrixD.h>
#include <NumA/integration/one_dimension/Integrator1D.h>
#include <stddef.h>
#include <map>
#include <string>
#include <vector>

#include "../../active_flavors_thresholds/ActiveFlavorsThresholdsModule.h"
#include "../../../beans/QuarkNonSingletCombination.h"
#include "../../MathIntegratorModule.h"
#include "GPDEvolutionModule.h"

namespace PARTONS {

  /**
   * @class GPDEvolutionVinnikov
   *
   * @brief GPD evolution according to Vinnikov routines.
   *
   * This class makes use of original Vinnikov routines for the GPD evolution. For the reference see Ref. \cite{Vinnikov:2006xw}.
   */
  class GPDEvolutionVinnikov: public GPDEvolutionModule, public MathIntegratorModule {

  public:

    static const std::string PARAM_NAME_GRID_SIZE;

    static const unsigned int classId;

    GPDEvolutionVinnikov(const std::string &className);
    virtual ~GPDEvolutionVinnikov();
    virtual GPDEvolutionVinnikov* clone() const;
    virtual void configure(const ElemUtils::Parameters &parameters);
    virtual void resolveObjectDependencies();

    size_t getGridSize() const;
    void setGridSize(size_t gridSize);

  protected:

    GPDEvolutionVinnikov(const GPDEvolutionVinnikov& other);

    virtual void isModuleWellConfigured();
    virtual void initModule();
    virtual PartonDistribution compute(GPDModule* pGPDModule, const GPDType::Type &type);

    virtual double nonSingletMu2FDerivative(double MuF2, QuarkNonSingletCombination::Type quarkNonSingletCombination);
    virtual double singletMuF2Derivative(double MuF2);
    virtual double gluonMuF2Derivative(double MuF2);

    void computeMuF2DerivativeNS(double MuF2, double MuF2Last, QuarkNonSingletCombination::Type quarkNonSingletCombination);
    void computeMuF2DerivativeSG(double MuF2, double MuF2Last);

  private:

    std::map<QuarkNonSingletCombination::Type, std::vector<double> > m_NS;
    std::vector<double> m_S;
    std::vector<double> m_G;

    std::map<QuarkNonSingletCombination::Type, std::vector<double> > m_NSDiff;
    std::vector<double> m_SDiff;
    std::vector<double> m_GDiff;

    std::map<QuarkNonSingletCombination::Type, std::vector<double> > m_NSEvolved;
    std::vector<double> m_SEvolved;
    std::vector<double> m_GEvolved;

    std::map<QuarkNonSingletCombination::Type, bool> m_NSEvoledForFirstTime;
    bool m_SEvoledForFirstTime;
    bool m_GEvoledForFirstTime;

    std::map<QuarkNonSingletCombination::Type, double> m_NSResultLast;
    double m_SResultLast;
    double m_GResultLast;

    std::map<QuarkNonSingletCombination::Type, double> m_NSMuF2Last;
    double m_SMuF2Last;
    double m_GMuF2Last;

    std::vector<double> m_NSXGrid;
    std::vector<double> m_SXGrid;

    void clearVectors(bool lastKinematicsDifferent);

    void checkIfGridsValid();
    void checkIfResultValid(const double x, double& value);
    void checkEvolutionDirection(const double MuF2, const double MuF2Last) const;

    size_t m_gridSize;
    double m_xiLast;
    double m_tLast;
    double m_MuR2Last;
    size_t m_nFlavors;
    size_t m_nFlavors_ref;

    GPDModule* m_pGPDModule;
    PartonDistribution m_PartonDistributionAtMuF2_ref;

    static const double N_C;
    static const double C_F;
    static const double N_F;
    static const double B_0;
    static const double C_A;
    static const double T_R;
    std::map<size_t, NumA::MatrixD> conversionMatrices;

    /**
     * Get constant iterator for given std::map<QuarkNonSingletCombination::Type, double> map and key.
     * @param map Map.
     * @param key Key.
     * @param mapName Map name.
     * @return Requested const iterator.
     */
    inline std::map<QuarkNonSingletCombination::Type, double>::const_iterator findInQuarkNonSingletCombinationMap(
            const std::map<QuarkNonSingletCombination::Type, double>& map,
            QuarkNonSingletCombination::Type key,
            const std::string& mapName) const;

    double conv_int_ns(int const ix, double const xi, int const n, const std::vector<double>& gpd, const std::vector<double>& x);

    double conv_int_qq(int const ix, double const xi, int const n, const std::vector<double>& gpd, const std::vector<double>& x);
    double conv_int_gg(int const ix, double const xi, int const n, const std::vector<double>& gpd, const std::vector<double>& x);
    double conv_int_gq(int const ix, double const xi, int const n, const std::vector<double>& gpd, const std::vector<double>& x);
    double conv_int_qg(int const ix, double const xi, int const n, const std::vector<double>& gpd, const std::vector<double>& x);

    double conv_int_qq_pol(int const ix, double const xi, int const n, const std::vector<double>& gpd, const std::vector<double>& x);
    double conv_int_gg_pol(int const ix, double const xi, int const n, const std::vector<double>& gpd, const std::vector<double>& x);
    double conv_int_gq_pol(int const ix, double const xi, int const n, const std::vector<double>& gpd, const std::vector<double>& x);
    double conv_int_qg_pol(int const ix, double const xi, int const n, const std::vector<double>& gpd, const std::vector<double>& x);

    int gpd_grid_ns(int const n, double const xi, std::vector<double>& x);
    int gpd_grid_s(int const n, double const xi, std::vector<double>& x);

    /**
     * Get map containing non-singlet values for given number of active flavors from PartonDistribution object.
     * @param nFlavors Number of active flavors.
     * @param partonDistribution Input PartonDistribution object.
     * @return Map containing non-singlet values.
     */
    std::map<QuarkNonSingletCombination::Type, double> getNS(const size_t nFlavors, const PartonDistribution& partonDistribution) const;

    /**
     * Get singlet value for given number of active flavors from PartonDistribution object.
     * @param nFlavors Number of active flavors.
     * @param partonDistribution Input PartonDistribution object.
     * @return Singlet value.
     */
    double getS(const size_t nFlavors,
            const PartonDistribution& partonDistribution) const;

    /**
     * Get value for gluon contribution from PartonDistribution object.
     * @param partonDistribution Input PartonDistribution object.
     * @return Value for gluon contribution.
     */
    double getG(const PartonDistribution& partonDistribution) const;

    /**
     * Get constant iterator for given std::map<QuarkFlavor::Type, QuarkDistribution> map and key.
     * @param map Map.
     * @param key Key.
     * @param mapName Map name.
     * @return Requested const iterator.
     */
    inline std::map<QuarkFlavor::Type, QuarkDistribution>::const_iterator findInQuarkFlavorMap(
            const std::map<QuarkFlavor::Type, QuarkDistribution>& map,
            QuarkFlavor::Type key, const std::string& mapName) const;

    /**
     * Get PartonDistribution object for given number of active flavors from non-singlet, singlet and gluon values.
     * @param nFlavors Number of active flavors.
     * @param NS Map containing non-singlet values.
     * @param S Singlet value.
     * @param G Value for gluon contribution.
     * @return PartonDistribution object.
     */
    PartonDistribution getPartonDistribution(const size_t nFlavors, const std::map<QuarkNonSingletCombination::Type, double>& NS, double S, double G) const;

    /**
     * Initialize functors for integration.
     */
    void initFunctorsForIntegrations();

    NumA::FunctionType1D* p_intIntegrateComputeOutputNS; ///< Functor to GPDEvolutionModule::integrateComputeOutputNS.
    NumA::FunctionType1D* p_intIntegrateComputeOutputS; ///< Functor to GPDEvolutionModule::integrateComputeOutputS.
    NumA::FunctionType1D* p_intIntegrateComputeOutputG; ///< Functor to GPDEvolutionModule::integrateComputeOutputG.

    /**
     * Wrapper to integrate GPDEvolutionModule::nonSingletMu2FDerivative.
     */
    double integrateComputeOutputNS(double MuF2, std::vector<double> par);

    /**
     * Wrapper to integrate GPDEvolutionModule::singletMuF2Derivative.
     */
    double integrateComputeOutputS(double MuF2, std::vector<double> par);

    /**
     * Wrapper to integrate GPDEvolutionModule::gluonMuF2Derivative.
     */
    double integrateComputeOutputG(double MuF2, std::vector<double> par);
  };

} /* namespace PARTONS */

#endif /* VINNIKOV_EVOLUTION_MODEL_H */
