#ifndef JETCROSSSECTIONTOTAL_H
#define JETCROSSSECTIONTOTAL_H

/**
 * @file JETCrossSectionTotal.h
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
#include "../JETObservable.h"

namespace PARTONS {

/**
 * @class JETCrossSectionTotal
 * @brief Unpolarized cross-section for electro-production integrated over \f$xB\f$, \f$t\f$, \f$z\f$, \f$qPerp2\f$, \f$Q^{2}\f$ and angles.
 *
 * Default ranges: \f$1E-4 < xB < 0.7\f$, \f$-1 < t < 0\f$, \f$0.1 < z < 0.9\f$, \f$1 < qPerp2 < 10\f$, \f$1 < Q^{2} < 1E3\f$ and \f$0 < y < 1\f$. Full integration over angles.
 *
 * Unit: \f$\mathrm{nbarn}\f$.
 */
class JETCrossSectionTotal: public JETObservable {

public:

    static const std::string JET_CROSSSECTION_TOTAL_RANGEXb; ///< String used to set integration xB range via XML scenario.
    static const std::string JET_CROSSSECTION_TOTAL_RANGET; ///< String used to set integration t range via XML scenario.
    static const std::string JET_CROSSSECTION_TOTAL_RANGEZ; ///< String used to set integration z range via XML scenario.
    static const std::string JET_CROSSSECTION_TOTAL_RANGEQPerp2; ///< String used to set integration qPerp2 range via XML scenario.
    static const std::string JET_CROSSSECTION_TOTAL_RANGEQ2; ///< String used to set integration Q2 range via XML scenario.
    static const std::string JET_CROSSSECTION_TOTAL_RANGEPHI; ///< String used to set integration phi range via XML scenario.
    static const std::string JET_CROSSSECTION_TOTAL_RANGEY; ///< String used to set integration y range via XML scenario.
    static const std::string JET_CROSSSECTION_TOTAL_N0; ///< String used to set number of MC integration iterations per cycle via XML scenario.
    static const std::string JET_CROSSSECTION_TOTAL_N1; ///< String used to set number of MC integration cycles via XML scenario.

    /**
     * Unique ID to automatically register the class in the registry.
     */
    static const unsigned int classId;

    /**
     * Function for the integration.
     */
    static double JETCrossSectionTotalFunctionA(double* kin, size_t dim,
            void* par);

    /**
     * Function for the integration.
     */
    static double JETCrossSectionTotalFunctionB(double x, void* params); 

    /**
     * Constructor.
     * @param className Name of class.
     */
    JETCrossSectionTotal(const std::string &className);

    /**
     * Destructor.
     */
    virtual ~JETCrossSectionTotal();

    virtual JETCrossSectionTotal* clone() const;
    virtual void configure(const ElemUtils::Parameters &parameters);

    size_t getNI0() const;
    void setNI0(size_t nI0);
    size_t getNI1() const;
    void setNI1(size_t nI1);
    const std::pair<double, double>& getRangexB() const;
    void setRangexB(const std::pair<double, double>& rangexB);
    const std::pair<double, double>& getRangeT() const;
    void setRangeT(const std::pair<double, double>& rangeT);
    const std::pair<double, double>& getRangeZ() const;
    void setRangeZ(const std::pair<double, double>& rangeZ);
    const std::pair<double, double>& getRangeQPerp2() const;
    void setRangeQPerp2(const std::pair<double, double>& rangeZ);
    const std::pair<double, double>& getRangeQ2() const;
    void setRangeQ2(const std::pair<double, double>& rangeQ2);
    const std::pair<double, double>& getRangePhi() const;
    void setRangePhi(const std::pair<double, double>& rangePhi);
    const std::pair<double, double>& getRangeY() const;
    void setRangeY(const std::pair<double, double>& rangeY);

protected:

    /**
     * Copy constructor.
     * @param other Object to be copied.
     */
    JETCrossSectionTotal(const JETCrossSectionTotal &other);

    virtual PhysicalType<double> computeObservable(
            const JETObservableKinematic& kinematic,
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
    std::pair<double, double> m_rangeZ; ///< z integration range.
    std::pair<double, double> m_rangeQPerp2; ///< qPerp2 integration range.
    std::pair<double, double> m_rangeQ2; ///< Q2 integration range.
    std::pair<double, double> m_rangePhi; ///< phi integration range.
    std::pair<double, double> m_rangeY; ///< y integration range.
};

struct JETCrossSectionTotalParameters {

    JETCrossSectionTotal* m_pJETCrossSectionTotal; ///< Pointer to JETCrossSectionTotal.
    double m_E; ///< Beam energy.
    JetType::Type m_jetType;
    List<GPDType> m_gpdType; ///< GPD types.
    std::pair<double, double> m_xBCut;
    std::pair<double, double> m_phiCut;

    double m_y, m_t, m_z, m_qPerp2, m_Q2;
};

} /* namespace PARTONS */

#endif /* JETCROSSSECTIONTOTAL_H */
