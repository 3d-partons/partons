#ifndef GPD_GK19_H
#define GPD_GK19_H

/**
 * @file GPDGK19.h
 * @author Kemal Tezgin
 * @date 2019
 * @version 1.0
 */

#include <string>
#include <vector>

#include "GPDGK16.h"

namespace PARTONS {

/**
 * @class GPDGK19
 *
 * @brief The module implements GPDs which appear in Goloskokov-Kroll (GK) model in pseudoscalar meson production
 *
 * This model is defined in the series of references:
 * - arxiv:0906.0460 \cite Goloskokov:2009ia ;
 * - arxiv:1106.4897 \cite Goloskokov:2011rd ;
 *
 * Available GPD types: H, E, \tilde{H}, \tilde{E}, H_T and \bar{E}_T
 *
 * We use the following parameters for GPDs (all GPDs have Regge-like profile function):
 *   1) \tilde{H}: Table 1 in arxiv:1901.11380 with \alpha(0) = 0.32
 *   2) \tilde{E}: Table 1 in arxiv:1901.11380
 *   3)       H_T: Table 1 in arxiv:1901.11380 with \alpha(0) = -0.17
 *   4) \bar{E}_T: Table 1 in arxiv:1901.11380
 *   with PDFs (forward-limits) parametrizations ABM: arXiv:1202.2281 and DSSV: arXiv:0904.3821
 *
 * Parameterization of GPDs H and E as in GPDGK16
 */

class GPDGK19: public GPDGK16 {

public:

    static const unsigned int classId; ///< Unique ID to automatically register the class in the registry.

    /**
     * Constructor.
     * See BaseObject::BaseObject and ModuleObject::ModuleObject for more details.
     *
     * @param className name of child class.
     */
    GPDGK19(const std::string &className);

    /**
     * Destructor.
     */
    virtual ~GPDGK19();

    virtual GPDGK19* clone() const;

protected:

    /**
     * Copy constructor.
     */
    GPDGK19(const GPDGK19& other);

    virtual void initModule();
    virtual void isModuleWellConfigured();

    virtual PartonDistribution computeHt();
    virtual PartonDistribution computeEt();
    virtual PartonDistribution computeHTrans();
    virtual PartonDistribution computeEbarTrans();

    void calculateHtCoefs();
    void calculateEtCoefs();
    void calculateHTransCoefs();
    void calculateETransCoefs();

private:

    double kHtgluon;                 ///< Exponent of correlated x-t dependence.
    double kHtsea;                   ///< Exponent of correlated x-t dependence.
    double kHtuval;                  ///< Exponent of correlated x-t dependence.
    double kHtdval;                  ///< Exponent of correlated x-t dependence.

    double kEtgluon;                 ///< Exponent of correlated x-t dependence.
    double kEtsea;                   ///< Exponent of correlated x-t dependence.
    double kEtuval;                  ///< Exponent of correlated x-t dependence.
    double kEtdval;                  ///< Exponent of correlated x-t dependence.

    double kHTransgluon;             ///< Exponent of correlated x-t dependence.
    double kHTranssea;               ///< Exponent of correlated x-t dependence.
    double kHTransuval;              ///< Exponent of correlated x-t dependence.
    double kHTransdval;              ///< Exponent of correlated x-t dependence.

    double kETransgluon;             ///< Exponent of correlated x-t dependence.
    double kETranssea;               ///< Exponent of correlated x-t dependence.
    double kETransuval;              ///< Exponent of correlated x-t dependence.
    double kETransdval;              ///< Exponent of correlated x-t dependence.

    std::vector<double> Htuval1tab;             ///< Htval1(i=0,1,2) for valence u
    std::vector<double> Htdval1tab;             ///< Htval1(i=0,1,2) for valence d
    std::vector<double> Htuval1mtab;            ///< Htval1(i=0,1,2) for valence u for -xb
    std::vector<double> Htdval1mtab;            ///< Htval1(i=0,1,2) for valence d for -xb

    std::vector<double> Etuval1tab;             ///< Etval1(i=0,1,2,3) for valence u
    std::vector<double> Etdval1tab;             ///< Etval1(i=0,1,2,3) for valence d
    std::vector<double> Etuval1mtab;            ///< Etval1(i=0,1,2,3) for valence u for -xb
    std::vector<double> Etdval1mtab;            ///< Etval1(i=0,1,2,3) for valence d for -xb

    std::vector<double> HTransuval1tab;         ///< HTransval1(i=0,1,2,3,4,5) for valence u
    std::vector<double> HTransdval1tab;         ///< HTransval1(i=0,1,2,3,4,5) for valence d
    std::vector<double> HTransuval1mtab;        ///< HTransval1(i=0,1,2,3,4,5) for valence u for -xb
    std::vector<double> HTransdval1mtab;        ///< HTransval1(i=0,1,2,3,4,5) for valence d for -xb

    std::vector<double> ETransuval1tab;         ///< ETransval1(i=0,1,2) for valence u
    std::vector<double> ETransdval1tab;         ///< ETransval1(i=0,1,2) for valence d
    std::vector<double> ETransuval1mtab;        ///< ETransval1(i=0,1,2) for valence u for -xb
    std::vector<double> ETransdval1mtab;        ///< ETransval1(i=0,1,2) for valence d for -xb

    void calculateHtKas();
    void calculateEtKas();
    void calculateHTransKas();
    void calculateETransKas();

    double ValenceExpansion(double x, double i, double k);
};

} /* namespace PARTONS */

#endif /* GPD_GK19_H */
