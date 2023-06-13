#ifndef DVMPCROSSSECTIONTOTAL_H
#define DVMPCROSSSECTIONTOTAL_H

/**
 * @file DVMPCrossSectionTotal.h
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
#include "../../../../beans/MesonType.h"
#include "../../../../utils/type/PhysicalType.h"
#include "DVMPCrossSectionUUUMinus.h"

namespace PARTONS {

/**
 * @class DVMPCrossSectionTotal
 * @brief Unpolarized cross-section for electro-production integrated over \f$xB\f$, \f$Q^{2}\f$, \f$t\f$ and angles.
 *
 * Default ranges: \f$1E-4 < xB < 0.7\f$, \f$-1 < t < 0\f$, \f$1 < Q^{2} < 1E3\f$ and \f$0 < y < 1\f$. Full integration over angles.
 *
 * Unit: \f$\mathrm{nbarn}\f$.
 */
class DVMPCrossSectionTotal: public DVMPCrossSectionUUUMinus{

public:

    static const std::string DVMP_CROSSSECTION_TOTAL_RANGEXb; ///< String used to set integration xB range via XML scenario.
    static const std::string DVMP_CROSSSECTION_TOTAL_RANGET; ///< String used to set integration t range via XML scenario.
    static const std::string DVMP_CROSSSECTION_TOTAL_RANGEQ2; ///< String used to set integration Q2 range via XML scenario.
    static const std::string DVMP_CROSSSECTION_TOTAL_RANGEPHI; ///< String used to set integration phi range via XML scenario.
    static const std::string DVMP_CROSSSECTION_TOTAL_RANGEY; ///< String used to set integration y range via XML scenario.
    static const std::string DVMP_CROSSSECTION_TOTAL_RANGENu; ///< String used to set integration nu range via XML scenario.
    static const std::string DVMP_CROSSSECTION_TOTAL_N0; ///< String used to set number of MC integration iterations per cycle via XML scenario.
    static const std::string DVMP_CROSSSECTION_TOTAL_N1; ///< String used to set number of MC integration cycles via XML scenario.
    static const std::string DVMP_CROSSSECTION_TOTAL_VIRTUALPHOTOPRODUCTION; ///< String used to set if virtual photo-production should be evaluated.

    /**
     * Unique ID to automatically register the class in the registry.
     */
    static const unsigned int classId;

    /**
     * Function for the integration.
     */
    static double DVMPCrossSectionTotalFunctionA(double* kin, size_t dim,
            void* par);

    /**
     * Function for the integration.
     */
    static double DVMPCrossSectionTotalFunctionB(double x, void* params);

    /**
     * Constructor.
     * @param className Name of class.
     */
    DVMPCrossSectionTotal(const std::string &className);

    /**
     * Destructor.
     */
    virtual ~DVMPCrossSectionTotal();

    virtual DVMPCrossSectionTotal* clone() const;
    virtual void configure(const ElemUtils::Parameters &parameters);

    size_t getNI0() const;
    void setNI0(size_t nI0);
    size_t getNI1() const;
    void setNI1(size_t nI1);
    const std::pair<double, double>& getRangeQ2() const;
    void setRangeQ2(const std::pair<double, double>& rangeQ2);
    const std::pair<double, double>& getRangeT() const;
    void setRangeT(const std::pair<double, double>& rangeT);
    const std::pair<double, double>& getRangexB() const;
    void setRangexB(const std::pair<double, double>& rangexB);
    const std::pair<double, double>& getRangeY() const;
    void setRangeY(const std::pair<double, double>& rangeY);
    const std::pair<double, double>& getRangeNu() const;
    void setRangeNu(const std::pair<double, double>& rangeNu);
    const std::pair<double, double>& getRangePhi() const;
    void setRangePhi(const std::pair<double, double>& rangePhi);
    bool isEvaluatePhotoProduction() const;
    void setEvaluatePhotoProduction(bool evaluatePhotoProduction);

protected:

    /**
     * Copy constructor.
     * @param other Object to be copied.
     */
    DVMPCrossSectionTotal(const DVMPCrossSectionTotal &other);

    virtual PhysicalType<double> computeObservable(
            const DVMPObservableKinematic& kinematic,
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

    std::pair<double, double> m_rangexB; ///< xB integration range.
    std::pair<double, double> m_rangeT; ///< t integration range.
    std::pair<double, double> m_rangeQ2; ///< Q2 integration range.
    std::pair<double, double> m_rangePhi; ///< phi integration range.
    std::pair<double, double> m_rangeY; ///< y integration range.
    std::pair<double, double> m_rangeNu; ///< nu integration range.

    bool m_evaluatePhotoProduction; ///< If true, virtual photo-production will be evaluated.
};

struct DVMPCrossSectionTotalParameters {

    DVMPCrossSectionTotal* m_pDVMPCrossSectionTotal; ///< Pointer to DVMPCrossSectionTotal.
    double m_E; ///< Beam energy.
    MesonType::Type m_mesonType; ///< Meson type.
    List<GPDType> m_gpdType; ///< GPD types.
    std::pair<double, double> m_xBCut; ///< y cut.
    std::pair<double, double> m_nuCut; ///< nu cut.
    std::pair<double, double> m_phiCut; ///< phi cut.
    bool m_evaluatePhotoProduction; ///< If true, virtual photo-production will be evaluated.

    double m_y, m_t, m_Q2;
};

} /* namespace PARTONS */

#endif /* DVMPCROSSSECTIONTOTAL_H */
