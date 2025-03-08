#ifndef TCSCROSSSECTIONTOTAL_H
#define TCSCROSSSECTIONTOTAL_H

/**
 * @file TCSCrossSectionTotal.h
 * @author Pawel Sznajder (NCBJ)
 * @date May 17, 2020
 * @version 1.0
 */

#include <ElementaryUtils/parameters/Parameters.h>
#include <stddef.h>
#include <string>
#include <utility>

#include "../../../../beans/gpd/GPDType.h"
#include "../../../../beans/List.h"
#include "../../../../utils/type/PhysicalType.h"
#include "TCSCrossSectionUU.h"

namespace PARTONS {

/**
 * @class TCSCrossSectionTotal
 * @brief Unpolarized cross-section for electro-production integrated over \f$xB\f$, \f$Q^{2}\f$, \f$t\f$ and angles.
 *
 * Default ranges: \f$-1 < t < 0\f$, \f$1 < Q'^{2} < 1E3\f$ and \f$0 < y < 1\f$, \f$Q2min < Q^{2} < 0.1\f$. Full integration over angles.
 *
 * Unit: \f$\mathrm{nbarn}\f$.
 */
class TCSCrossSectionTotal: public TCSCrossSectionUU {

public:

    static const std::string TCS_CROSSSECTION_TOTAL_RANGET; ///< String used to set integration t range via XML scenario.
    static const std::string TCS_CROSSSECTION_TOTAL_RANGEQ2PRIM; ///< String used to set integration Q2Prim range via XML scenario.
    static const std::string TCS_CROSSSECTION_TOTAL_RANGEPHI; ///< String used to set integration phi range via XML scenario.
    static const std::string TCS_CROSSSECTION_TOTAL_RANGETHETA; ///< String used to set integration theta range via XML scenario.
    static const std::string TCS_CROSSSECTION_TOTAL_RANGEY; ///< String used to set integration y range via XML scenario.
    static const std::string TCS_CROSSSECTION_TOTAL_RANGEQ2; ///< String used to set integration Q2 range via XML scenario.
    static const std::string TCS_CROSSSECTION_TOTAL_N0; ///< String used to set number of MC integration iterations per cycle via XML scenario.
    static const std::string TCS_CROSSSECTION_TOTAL_N1; ///< String used to set number of MC integration cycles via XML scenario.

    static const std::string TCS_CROSSSECTION_TOTAL_PHIEPSILON; ///< String used to set epsilon skipped arroung phiL = PI via XML scenario.

    /**
     * Unique ID to automatically register the class in the registry.
     */
    static const unsigned int classId;

    /**
     * Function for the integration.
     */
    static double TCSCrossSectionTotalFunctionA(double* kin, size_t dim,
            void* par);

    /**
     * Function for the integration.
     */
    static double TCSCrossSectionTotalFunctionB(double x, void* params); 

   /**
     * Function for the integration.
     */
    static double TCSCrossSectionTotalFunctionC(double x, void* params); 

    /**
     * Constructor.
     * @param className Name of class.
     */
    TCSCrossSectionTotal(const std::string &className);

    /**
     * Destructor.
     */
    virtual ~TCSCrossSectionTotal();

    virtual TCSCrossSectionTotal* clone() const;
    virtual void configure(const ElemUtils::Parameters &parameters);

    size_t getNI0() const;
    void setNI0(size_t nI0);
    size_t getNI1() const;
    void setNI1(size_t nI1);
    const std::pair<double, double>& getRangeT() const;
    void setRangeT(const std::pair<double, double>& rangeT);
    const std::pair<double, double>& getRangeQ2Prim() const;
    void setRangeQ2Prim(const std::pair<double, double>& rangeQ2Prim);
    const std::pair<double, double>& getRangePhi() const;
    void setRangePhi(const std::pair<double, double>& rangePhi);
    const std::pair<double, double>& getRangeTheta() const;
    void setRangeTheta(const std::pair<double, double>& rangeTheta);
    const std::pair<double, double>& getRangeY() const;
    void setRangeY(const std::pair<double, double>& rangeY);
    const std::pair<double, double>& getRangeQ2() const;
    void setRangeQ2(const std::pair<double, double>& rangeQ2);
    double getPhiEpsilon() const;
    void setPhiEpsilon(double phiEpsilon);

protected:

    /**
     * Copy constructor.
     * @param other Object to be copied.
     */
    TCSCrossSectionTotal(const TCSCrossSectionTotal &other);

    virtual PhysicalType<double> computeObservable(
            const TCSObservableKinematic& kinematic,
            const List<GPDType>& gpdType);

private:

    /**
     * Print change of ranges.
     */
    void printChangeOfRange(const std::string& func, const std::string& name,
            const std::pair<double, double>& oldValues,
            const std::pair<double, double>& newValues) const;

    /**
     * Parse range.
     */
    std::pair<double, double> parseRange(const std::string& str) const;

    size_t m_nI0;   ///< Number of iteration in single cycle.
    size_t m_nI1;   ///< Number of cycles.

    std::pair<double, double> m_rangeT; ///< t integration range.
    std::pair<double, double> m_rangeQ2Prim; ///< Q2Prim integration range.
    std::pair<double, double> m_rangePhi; ///< phi integration range.
    std::pair<double, double> m_rangeTheta; ///< phi integration range.
    std::pair<double, double> m_rangeY; ///< y integration range.
    std::pair<double, double> m_rangeQ2; ///< Q2 integration range.

    double m_phiEpsilon; ///< Cut out phi = pi.
};

struct TCSCrossSectionTotalParameters {

    TCSCrossSectionTotal* m_pTCSCrossSectionTotal; ///< Pointer to TCSCrossSectionTotal.
    double m_Ee; ///< Beam energy.
    List<GPDType> m_gpdType; ///< GPD types.
    std::pair<double, double> m_phiCut;
    std::pair<double, double> m_thetaCut;

    double m_t, m_Q2Prim;
    double m_Egamma; 
    double m_phi;

    double m_phiEpsilon;
};

} /* namespace PARTONS */

#endif /* TCSCROSSSECTIONTOTAL_H */
