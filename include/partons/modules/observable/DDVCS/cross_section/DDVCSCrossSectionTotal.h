#ifndef DDVCSCROSSSECTIONTOTAL_H
#define DDVCSCROSSSECTIONTOTAL_H

/**
 * @file DDVCSCrossSectionTotal.h
 * @author Pawel Sznajder (NCBJ)
 * @date May 17, 2020
 * @version 1.0
 */

#include <ElementaryUtils/parameters/Parameters.h>
#include <gsl/gsl_rng.h>
#include <stddef.h>
#include <string>
#include <utility>

#include "../../../../beans/gpd/GPDType.h"
#include "../../../../beans/List.h"
#include "../../../../utils/type/PhysicalType.h"
#include "DDVCSCrossSectionUUMinus.h"

namespace PARTONS {

/**
 * @class DDVCSCrossSectionTotal
 * @brief Unpolarized cross-section for electro-production integrated over \f$xB\f$, \f$Q^{2}\f$, \f$Q'^{2}\f$, \f$t\f$ and angles.
 *
 * Unit: \f$\mathrm{nbarn}\f$.
 */
class DDVCSCrossSectionTotal: public DDVCSCrossSectionUUMinus {

public:

    static const std::string DDVCS_CROSSSECTION_TOTAL_RANGEXb; ///< String used to set integration xB range via XML scenario.
    static const std::string DDVCS_CROSSSECTION_TOTAL_RANGET; ///< String used to set integration t range via XML scenario.
    static const std::string DDVCS_CROSSSECTION_TOTAL_RANGEQ2; ///< String used to set integration Q2 range via XML scenario.
    static const std::string DDVCS_CROSSSECTION_TOTAL_RANGEQ2PRIM; ///< String used to set integration Q2 range via XML scenario.
    static const std::string DDVCS_CROSSSECTION_TOTAL_RANGEPHI; ///< String used to set integration Q2 range via XML scenario.
    static const std::string DDVCS_CROSSSECTION_TOTAL_RANGEPHIL; ///< String used to set integration Q2 range via XML scenario.
    static const std::string DDVCS_CROSSSECTION_TOTAL_RANGETHETAL; ///< String used to set integration Q2 range via XML scenario.

    static const std::string DDVCS_CROSSSECTION_TOTAL_RANGEY; ///< String used to set integration y range via XML scenario.

    static const std::string DDVCS_CROSSSECTION_TOTAL_N0; ///< String used to set number of MC integration iterations per cycle via XML scenario.
    static const std::string DDVCS_CROSSSECTION_TOTAL_N1; ///< String used to set number of MC integration cycles via XML scenario.
    static const std::string DDVCS_CROSSSECTION_TOTAL_N2; ///< String used to set number of MC integration iterations per cycle via XML scenario.
    static const std::string DDVCS_CROSSSECTION_TOTAL_N3; ///< String used to set number of MC integration cycles via XML scenario.

    /**
     * Unique ID to automatically register the class in the registry.
     */
    static const unsigned int classId;

    /**
     * Function for the integration.
     */
    static double DDVCSCrossSectionTotalFunction(double* kin, size_t dim,
            void* par);

    /**
      * Function for the integration over angles.
      */
     static double DDVCSCrossSectionTotalFunctionAngle(double* kin, size_t dim,
             void* par);

    /**
     * Constructor.
     * @param className Name of class.
     */
    DDVCSCrossSectionTotal(const std::string &className);

    /**
     * Destructor.
     */
    virtual ~DDVCSCrossSectionTotal();

    virtual DDVCSCrossSectionTotal* clone() const;
    virtual void configure(const ElemUtils::Parameters &parameters);

    size_t getNI0() const;
    void setNI0(size_t nI0);
    size_t getNI1() const;
    void setNI1(size_t nI1);
    size_t getNI2() const;
    void setNI2(size_t nI2);
    size_t getNI3() const;
    void setNI3(size_t nI3);

    const std::pair<double, double>& getRangexB() const;
    void setRangexB(const std::pair<double, double>& rangexB);
    const std::pair<double, double>& getRangeT() const;
    void setRangeT(const std::pair<double, double>& rangeT);
    const std::pair<double, double>& getRangeQ2() const;
    void setRangeQ2(const std::pair<double, double>& rangeQ2);
    const std::pair<double, double>& getRangeQ2Prim() const;
    void setRangeQ2Prim(const std::pair<double, double>& rangeQ2Prim);

    const std::pair<double, double>& getRangePhi() const;
    void setRangePhi(const std::pair<double, double>& rangePhi);
    const std::pair<double, double>& getRangePhiL() const;
    void setRangePhiL(const std::pair<double, double>& rangePhiL);
    const std::pair<double, double>& getRangeThetaL() const;
    void setRangeThetaL(const std::pair<double, double>& rangeThetaL);

    const std::pair<double, double>& getRangeY() const;
    void setRangeY(const std::pair<double, double>& rangeY);

    bool isMakeDxBDy() const;
    void setMakeDxBDy(bool makeDyDx);

protected:

    /**
     * Copy constructor.
     * @param other Object to be copied.
     */
    DDVCSCrossSectionTotal(const DDVCSCrossSectionTotal &other);

    virtual PhysicalType<double> computeObservable(
            const DDVCSObservableKinematic& kinematic,
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
    size_t m_nI2;   ///< Number of iteration in single cycle.
    size_t m_nI3;   ///< Number of cycles.

    std::pair<double, double> m_rangexB; ///< xB integration range.
    std::pair<double, double> m_rangeT; ///< t integration range.
    std::pair<double, double> m_rangeQ2; ///< Q2 integration range.
    std::pair<double, double> m_rangeQ2Prim; ///< Q2 integration range.

    std::pair<double, double> m_rangePhi; ///< phi integration range.
    std::pair<double, double> m_rangePhiL; ///< phiL integration range.
    std::pair<double, double> m_rangeThetaL; ///< thetaL integration range.

    std::pair<double, double> m_rangeY; ///< y integration range.

    bool m_makeDxBDy;
};

struct DDVCSCrossSectionTotalParameters {

    DDVCSCrossSectionTotal* m_pDDVCSCrossSectionTotal; ///< Pointer to DDVCSCrossSectionTotal.
    gsl_rng* m_r; ///< Pointer to random number generator.
    double m_xB, m_t, m_Q2, m_Q2Prim;
    double m_E; ///< Beam energy.
    List<GPDType> m_gpdType; ///< GPD types.
    std::pair<double, double> m_yCut;
    bool m_makeDxBDy;
    size_t m_nI2, m_nI3;
};

} /* namespace PARTONS */

#endif /* DDVCSCROSSSECTIONTOTAL_H */
