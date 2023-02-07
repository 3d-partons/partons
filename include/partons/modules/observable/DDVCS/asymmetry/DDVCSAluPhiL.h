#ifndef DDVCSALUPHIL_H
#define DDVCSALUPHIL_H

/**
 * @file DDVCSAluPhiL.h
 * @author Victor Martinez-Fernandez (NCBJ)
 * @date 2023
 * @version 1.0
 */

#include <string>

#include "../../../../beans/gpd/GPDType.h"
#include "../../../../beans/List.h"
#include "../../../../utils/type/PhysicalType.h"
#include "../DDVCSObservable.h"

namespace PARTONS {

/**
 * @class DDVCSAluPhiL
 * @brief Longitudinally polarized beam asymmetry for negative beam charge. ALL subprocesses considered: DDVCS + BH + Interference.
 * @brief Asymmetry is left as a function of angle \phi^{\rm BDP}_\ell, azymuthal lepton angle as it is defined in BDP2001 paper.
 *
 * Definition:<br>
 *
 * \f$ \displaystyle
 * A_{LU}\left(  \right) =
 * TODO FORMULA \, ,
 * \f$
 *
 * where
 * \f$\sigma^{b_{h} b_{c}}\f$ is a single photon production cross-section (DDVCS, BH and Interference) for beam helicity denoted by \f$b_{h}\f$ and beam charge denoted by \f$b_{c}\f$.
 */
class DDVCSAluPhiL: public DDVCSObservable {

public:

    /**
     * Unique ID to automatically register the class in the registry.
     */
    static const unsigned int classId;

    /**
     * Function for the integration.
     */
    static double DDVCSAluPhiLFunction(double* kin, size_t dim, void* par);

    /**
     * Constructor.
     * @param className Name of class.
     */
    DDVCSAluPhiL(const std::string &className);

    /**
     * Destructor.
     */
    virtual ~DDVCSAluPhiL();

    virtual DDVCSAluPhiL* clone() const;

protected:

    /**
     * Copy constructor.
     * @param other Object to be copied.
     */

    DDVCSAluPhiL(const DDVCSAluPhiL &other);

    virtual PhysicalType<double> computeObservable(
            const DDVCSObservableKinematic& kinematic,
            const List<GPDType>& gpdType);
};

struct DDVCSAluPhiLParameters {

    DDVCSAluPhiL* m_pDDVCSAluPhiL; ///< Pointer to DDVCSAluPhiL.
    double m_xB, m_t, m_Q2, m_Q2Prim, m_E, m_phiLBDP, signAux;
    List<GPDType> m_gpdType; ///< GPD types.
};

} /* namespace PARTONS */

#endif /* DDVCSALUPHIL_H */
