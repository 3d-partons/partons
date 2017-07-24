#ifndef GPD_GK16_H
#define GPD_GK16_H

/**
 * @file GPDGK16.h
 * @author Bryan BERTHOU (CEA Saclay)
 * @author Herve MOUTARDE (CEA Saclay)
 * @date 2014
 * @version 1.0
 */

#include <ElementaryUtils/parameters/Parameters.h>
#include <string>
#include <vector>

#include "GPDModule.h"

//TODO finir les messages de debug dans toutes les fonctions
//TODO supprimer au maximum les pointers qui ne sont pas obligatoires
// !!!! Use tgamma() instead of gamma() (see: http://stackoverflow.com/questions/18116376/what-is-the-definition-for-gammadouble-x-and-why-is-it-different-on-two-gcc-ve)

/**
 * @class GPDGK16
 *
 * @brief A module that implements GPD as defined by Kroll-Goloskokov model in 2011.
 *
 * This model is defined in the series of references:
 * - arxiv:hep-ph/0611290 \cite Goloskokov:2006hr ;
 * - arxiv:0708.3569 \cite Goloskokov:2007nt ;
 * - arxiv:0809.4126 \cite Goloskokov:2008ib ;
 * - arxiv:0906.0460 \cite Goloskokov:2009ia ;
 * - arxiv:1106.4897 \cite Goloskokov:2011rd ;
 * - arxiv:1210.6975 \cite Kroll2012sm ;
 * - arxiv:1407.1141 \cite Goloskokov:2014ika.
 *
 * Has cln dependencies, which is not a thread-safe library. DO NOT USE THIS MODEL WITH THREADS!!
 *
 * Available GPD types: H, E, Ht, Et.
 *
 * August 2015 : Tested and approved by Pawel Sznajder.
 *
 * November 2016 : Update from GK11 to GK16 to correct pion pole issue in Et by Luca Colaneri and Pawel Sznajder.
 *
 */
class GPDGK16: public GPDModule {
public:

    static const unsigned int classId; ///< Unique ID to automatically register the class in the registry.

    /**
     * Constructor.
     * See BaseObject::BaseObject and ModuleObject::ModuleObject for more details.
     *
     * @param className name of child class.
     */
    GPDGK16(const std::string &className);

    /**
     * Default destructor.
     */
    virtual ~GPDGK16();

    virtual GPDGK16* clone() const;

    virtual void configure(const ElemUtils::Parameters &parameters);

    virtual std::string toString() const;

    // ##### GETTERS & SETTERS #####

    double getB0() const;
    double getC1() const;
    double getC2() const;
    double getC3() const;
    double getC4() const;
    double getC5() const;
    double getC6() const;
    double getC7() const;
    double getC8() const;
    const std::vector<double>& getEdval1mtab() const;
    const std::vector<double>& getEdval1tab() const;
    const std::vector<double>& getEi1tab() const;
    const std::vector<double>& getEs1tab() const;
    const std::vector<double>& getEtdval1mtab() const;
    const std::vector<double>& getEtdval1tab() const;
    const std::vector<double>& getEtuval1mtab() const;
    const std::vector<double>& getEtuval1tab() const;
    const std::vector<double>& getEuval1mtab() const;
    const std::vector<double>& getEuval1tab() const;
    double getEdValMx() const;
    double getEtdValMx() const;
    double getEtuValMx() const;
    double getEuValMx() const;
    double getHdValMx() const;
    double getHtdValMx() const;
    double getHtuValMx() const;
    double getHuValMx() const;
    double getL() const;
    const std::vector<double>& getHdval1mtab() const;
    const std::vector<double>& getHdval1tab() const;
    const std::vector<double>& getHi1tab() const;
    const std::vector<double>& getHs1tab() const;
    const std::vector<double>& getHtdval1mtab() const;
    const std::vector<double>& getHtdval1tab() const;
    const std::vector<double>& getHti1tab() const;
    const std::vector<double>& getHtuval1mtab() const;
    const std::vector<double>& getHtuval1tab() const;
    const std::vector<double>& getHuval1mtab() const;
    const std::vector<double>& getHuval1tab() const;
    double getKappaS() const;
    double getEdval() const;
    double getEgluon() const;
    double getEsea() const;
    double getEtdval() const;
    double getEtgluon() const;
    double getEtsea() const;
    double getEtuval() const;
    double getEuval() const;
    double getHdval() const;
    double getHgluon() const;
    double getHsea() const;
    double getHtdval() const;
    double getHtgluon() const;
    double getHtsea() const;
    double getHtuval() const;
    double getHuval() const;

protected:
    /**
     * Copy constructor.
     *
     * Used by the factory.
     *
     * @param other
     */
    GPDGK16(const GPDGK16& other);

    virtual void isModuleWellConfigured();
    virtual void initModule();

    //GPDResultData compute(GPDComputeType gpdComputeType);

    virtual PartonDistribution computeH();
    virtual PartonDistribution computeE();
    virtual PartonDistribution computeHt();
    virtual PartonDistribution computeEt();

//    double computeSinglet(const QuarkDistribution &quarkDistribution_u,
//            const QuarkDistribution &quarkDistribution_d,
//            const QuarkDistribution &quarkDistribution_s);

    void calculateHCoefs();
    void calculateECoefs();
    void calculateHtCoefs();
    void calculateEtCoefs();

    double c1, c2, c3, c4, c5, c6, c7, c8; // Coefficients of the expansion of CTEQ PDF in terms of half-integer powers of beta (eq. (26))
    double b0; // Exponential t-dependence (eq. (22))
    double fL;

    std::vector<double> Hi1tab;               ///< Hi1(i=0,0.5,1,1.5) for gluons

private:

    double kappa_s; // Flavour symmetry breaking factor, eq. (36)

    double fHuValMx;                                          ///< HuVal( - fx )
    double fHdValMx;                                          ///< HdVal( - fx )
    double fEuValMx;                                          ///< HuVal( - fx )
    double fEdValMx;                                          ///< HdVal( - fx )
    double fHtuValMx;                                         ///< HuVal( - fx )
    double fHtdValMx;                                         ///< HdVal( - fx )
    double fEtuValMx;                                         ///< HuVal( - fx )
    double fEtdValMx;                                         ///< HdVal( - fx )

    double kHgluon;                   ///< Exponent of correlated x-t dependence.
    double kHsea;                     ///< Exponent of correlated x-t dependence.
    double kHuval;                    ///< Exponent of correlated x-t dependence.
    double kHdval;                    ///< Exponent of correlated x-t dependence.

    double kEgluon;                   ///< Exponent of correlated x-t dependence.
    double kEsea;                     ///< Exponent of correlated x-t dependence.
    double kEuval;                    ///< Exponent of correlated x-t dependence.
    double kEdval;                    ///< Exponent of correlated x-t dependence.

    double kHtgluon;                  ///< Exponent of correlated x-t dependence.
    double kHtsea;                    ///< Exponent of correlated x-t dependence.
    double kHtuval;                   ///< Exponent of correlated x-t dependence.
    double kHtdval;                   ///< Exponent of correlated x-t dependence.

    double kEtgluon;                  ///< Exponent of correlated x-t dependence.
    double kEtsea;                    ///< Exponent of correlated x-t dependence.
    double kEtuval;                   ///< Exponent of correlated x-t dependence.
    double kEtdval;                   ///< Exponent of correlated x-t dependence.

    std::vector<double> Huval1tab;           ///< Hval1(i=0,0.5,1) for valence u
    std::vector<double> Hdval1tab;           ///< Hval1(i=0,0.5,1) for valence d
    std::vector<double> Huval1mtab;  ///< Hval1(i=0,0.5,1) for valence u for -xb
    std::vector<double> Hdval1mtab;  ///< Hval1(i=0,0.5,1) for valence d for -xb
    std::vector<double> Hs1tab; ///< Hs1(i=0,0.5,1,1.5) for sea, same for u, d, s

    std::vector<double> Euval1tab;               ///< Eval1(i=0,1) for valence u
    std::vector<double> Edval1tab;            ///< Eval1(i=0,..,7) for valence d
    std::vector<double> Euval1mtab;      ///< Eval1(i=0,1) for valence u for -xb
    std::vector<double> Edval1mtab;   ///< Eval1(i=0,..,7) for valence d for -xb
    std::vector<double> Es1tab;      ///< Es1(i=0,1,2) for sea, same for u, d, s
    std::vector<double> Ei1tab;                       ///< Ei1(i=0,1) for gluons

    std::vector<double> Htuval1tab;           ///< Htval1(i=0,1,2) for valence u
    std::vector<double> Htdval1tab;           ///< Htval1(i=0,1,2) for valence d
    std::vector<double> Htuval1mtab;  ///< Htval1(i=0,1,2) for valence u for -xb
    std::vector<double> Htdval1mtab;  ///< Htval1(i=0,1,2) for valence d for -xb
    std::vector<double> Hti1tab;                 ///< Hti1(i=0,0.5,1) for gluons

    std::vector<double> Etuval1tab;         ///< Etval1(i=0,1,2,3) for valence u
    std::vector<double> Etdval1tab;         ///< Etval1(i=0,1,2,3) for valence d
    std::vector<double> Etuval1mtab; ///< Etval1(i=0,1,2,3) for valence u for -xb
    std::vector<double> Etdval1mtab; ///< Etval1(i=0,1,2,3) for valence d for -xb

    void calculateHKas();
    void calculateEKas();
    void calculateHtKas();
    void calculateEtKas();

    double Et_pole(double x); ///< Pion pole contribution to Et.

    double Hi1(double x, double i, double k);
    double Hs1(double x, double i, double k);
    double Hval1(double x, double i, double k);
    double Ei1(double x, double i, double k);
    double Es1(double x, double i, double k);
    double Eval1(double x, double i, double k);
    double Hti1(double x, double i, double k);
    double Htval1(double x, double i, double k);
    double Etval1(double x, double i, double k);

    double Hi1_alt(double x, double i, double k);
    double Hs1_alt(double x, double i, double k);
    double Hval1_alt(double x, double i, double k);
    double Ei1_alt(double x, double i, double k);
    double Es1_alt(double x, double i, double k);
    double Eval1_alt(double x, double i, double k);
    double Hti1_alt(double x, double i, double k);
    double Htval1_alt(double x, double i, double k);
    double Etval1_alt(double x, double i, double k);

};

#endif /* GPD_GK16_H */
