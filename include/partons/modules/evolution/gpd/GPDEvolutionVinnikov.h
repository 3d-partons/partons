#ifndef GPDEVOLUTIONVINNIKOV_H
#define GPDEVOLUTIONVINNIKOV_H

#include <ElementaryUtils/parameters/Parameters.h>
#include <stddef.h>
#include <map>
#include <string>
#include <vector>

#include "../GPDEvolutionModule.h"
#include "../QuarkNonSingletCombination.h"

namespace PARTONS {

class GPDEvolutionVinnikov: public GPDEvolutionModule {

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

    virtual double nonSingletMu2FDerivative(double MuF2,
            QuarkNonSingletCombination::Type quarkNonSingletCombination);
    virtual double singletMuF2Derivative(double MuF2);
    virtual double gluonMuF2Derivative(double MuF2);

    void computeMuF2DerivativeNS(double MuF2, double MuF2Last,
            QuarkNonSingletCombination::Type quarkNonSingletCombination);
    void computeMuF2DerivativeSG(double MuF2, double MuF2Last);

private:

    size_t m_gridSize;

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

    void checkEvolutionDirection(const double MuF2,
            const double MuF2Last) const;

    double m_xiLast;
    double m_tLast;
    double m_MuR2Last;
    double m_MuF2RefLast;

    static const double N_C;
    static const double C_F;
    static const double N_F;
    static const double B_0;
    static const double C_A;
    static const double T_R;

    double conv_int_ns(int const ix, double const xi, int const n,
            const std::vector<double>& gpd, const std::vector<double>& x);
    double conv_int_qq(int const ix, double const xi, int const n,
            const std::vector<double>& gpd, const std::vector<double>& x);
    double conv_int_gg(int const ix, double const xi, int const n,
            const std::vector<double>& gpd, const std::vector<double>& x);
    double conv_int_gq(int const ix, double const xi, int const n,
            const std::vector<double>& gpd, const std::vector<double>& x);
    double conv_int_qg(int const ix, double const xi, int const n,
            const std::vector<double>& gpd, const std::vector<double>& x);
    double conv_int_qq_pol(int const ix, double const xi, int const n,
            const std::vector<double>& gpd, const std::vector<double>& x);
    double conv_int_gg_pol(int const ix, double const xi, int const n,
            const std::vector<double>& gpd, const std::vector<double>& x);
    double conv_int_gq_pol(int const ix, double const xi, int const n,
            const std::vector<double>& gpd, const std::vector<double>& x);
    double conv_int_qg_pol(int const ix, double const xi, int const n,
            const std::vector<double>& gpd, const std::vector<double>& x);

    int gpd_grid_ns(int const n, double const xi, std::vector<double>& x);
    int gpd_grid_s(int const n, double const xi, std::vector<double>& x);
};

} /* namespace PARTONS */

#endif /* VINNIKOV_EVOLUTION_MODEL_H */
