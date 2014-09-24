#ifndef GK11_MODEL_H
#define GK11_MODEL_H

/**
 * @file GK11Model.h
 * @author Bryan BERTHOU (CEA Saclay) - Herve MOUTARDE (CEA Saclay)
 * @date 2014
 * @version 2.0
 *
 * Last update : 22 September 2014
 *
 * @class GK11Model
 * @brief A module that implements GPD as defined by Kroll-Goloskokov model in 2011.
 *
 * !!!! Use tgamma() instead of gamma() (see: http://stackoverflow.com/questions/18116376/what-is-the-definition-for-gammadouble-x-and-why-is-it-different-on-two-gcc-ve)
 */

#include <string>
#include <vector>

#include "../../beans/gpd/GPDComputeType.h"
#include "../GPDModule.h"

class GPDOutputData;
class GPDResultData;

//TODO finir les messages de debug dans toutes les fonctions
//TODO supprimer au maximum les pointers qui ne sont pas obligatoires

class GK11Model: public GPDModule {
public:

    static const std::string moduleID; ///< Unique ID to automatically register the module in the factory.

    /**
     * Default constructor
     */
    GK11Model();

    GK11Model(const GK11Model& other);

    /**
     * Default destructor
     */
    virtual ~GK11Model();

    virtual GK11Model* clone() const;

    /**
     * Call by default constructor to initialize all class attributes
     */
    void init();

    /**
     * Compute GPD describes by Kroll-Goloskokov model in 2011. \n
     * Compute GPD_H, GPD_Ht, GPD_E, GPD_Et for up, down and strange quarks. \n
     * See GPDModule for more details.
     */
    virtual GPDOutputData compute(const double &_x, const double &_xi,
            const double &_t, const double &_MuF, const double &_MuR,
            GPDComputeType::Type _gpdComputeType);

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
    double getMuF2() const;
    double getQ0() const;
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

private:

    double c1, c2, c3, c4, c5, c6, c7, c8; // Coefficients of the expansion of CTEQ PDF in terms of half-integer powers of beta (eq. (26))
    double b0; // Exponential t-dependence (eq. (22))
    double kappa_s; // Flavour symmetry breaking factor, eq. (36)
    double fL;
    double fQ0;
    double fMuF2;

    double fHuValMx;                                          ///< HuVal( - fx )
    double fHdValMx;                                          ///< HdVal( - fx )
    double fEuValMx;                                          ///< HuVal( - fx )
    double fEdValMx;                                          ///< HdVal( - fx )
    double fHtuValMx;                                         ///< HuVal( - fx )
    double fHtdValMx;                                         ///< HdVal( - fx )
    double fEtuValMx;                                         ///< HuVal( - fx )
    double fEtdValMx;                                         ///< HdVal( - fx )

    double kHgluon;                   ///< Exponent of correlated x-t dependence
    double kHsea;                     ///< Exponent of correlated x-t dependence
    double kHuval;                    ///< Exponent of correlated x-t dependence
    double kHdval;                    ///< Exponent of correlated x-t dependence

    double kEgluon;                   ///< Exponent of correlated x-t dependence
    double kEsea;                     ///< Exponent of correlated x-t dependence
    double kEuval;                    ///< Exponent of correlated x-t dependence
    double kEdval;                    ///< Exponent of correlated x-t dependence

    double kHtgluon;                  ///< Exponent of correlated x-t dependence
    double kHtsea;                    ///< Exponent of correlated x-t dependence
    double kHtuval;                   ///< Exponent of correlated x-t dependence
    double kHtdval;                   ///< Exponent of correlated x-t dependence

    double kEtgluon;                  ///< Exponent of correlated x-t dependence
    double kEtsea;                    ///< Exponent of correlated x-t dependence
    double kEtuval;                   ///< Exponent of correlated x-t dependence
    double kEtdval;                   ///< Exponent of correlated x-t dependence

    std::vector<double> Huval1tab;           ///< Hval1(i=0,0.5,1) for valence u
    std::vector<double> Hdval1tab;           ///< Hval1(i=0,0.5,1) for valence d
    std::vector<double> Huval1mtab;  ///< Hval1(i=0,0.5,1) for valence u for -xb
    std::vector<double> Hdval1mtab;  ///< Hval1(i=0,0.5,1) for valence d for -xb
    std::vector<double> Hs1tab; ///< Hs1(i=0,0.5,1,1.5) for sea, same for u, d, s
    std::vector<double> Hi1tab;               ///< Hi1(i=0,0.5,1,1.5) for gluons

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

    void calculateHCoefs(const double &_x, const double &_xi, const double &_t);
    void calculateECoefs(const double &_x, const double &_xi, const double &_t);
    void calculateHtCoefs(const double &_x, const double &_xi,
            const double &_t);
    void calculateEtCoefs(const double &_x, const double &_xi,
            const double &_t);

    void calculateHKas(const double &_t);
    void calculateEKas(const double &_t);
    void calculateHtKas(const double &_t);
    void calculateEtKas(const double &_t);

    double Et_pole(const double &_xi, const double &_t, double x); ///< Pion pole contribution to Et

    double Hi1(const double &_xi, double x, double i, double k);
    double Hs1(const double &_xi, double x, double i, double k);
    double Hval1(const double &_xi, double x, double i, double k);
    double Ei1(const double &_xi, double x, double i, double k);
    double Es1(const double &_xi, double x, double i, double k);
    double Eval1(const double &_xi, double x, double i, double k);
    double Hti1(const double &_xi, double x, double i, double k);
    double Htval1(const double &_xi, double x, double i, double k);
    double Etval1(const double &_xi, double x, double i, double k);

    double Hi1_alt(const double &_xi, double x, double i, double k);
    double Hs1_alt(const double &_xi, double x, double i, double k);
    double Hval1_alt(const double &_xi, double x, double i, double k);
    double Ei1_alt(const double &_xi, double x, double i, double k);
    double Es1_alt(const double &_xi, double x, double i, double k);
    double Eval1_alt(const double &_xi, double x, double i, double k);
    double Hti1_alt(const double &_xi, double x, double i, double k);
    double Htval1_alt(const double &_xi, double x, double i, double k);
    double Etval1_alt(const double &_xi, double x, double i, double k);

    void updateVariables(const double &_MuF);

    GPDResultData computeH(const double &_x, const double &_xi,
            const double &_t); ///< Compute GPD H at considered kinematics
    GPDResultData computeE(const double &_x, const double &_xi,
            const double &_t); ///< Compute GPD E at considered kinematics
    GPDResultData computeHt(const double &_x, const double &_xi,
            const double &_t); ///< Compute GPD Ht at considered kinematics
    GPDResultData computeEt(const double &_x, const double &_xi,
            const double &_t); ///< Compute GPD Et at considered kinematics
};

#endif /* GK11_MODEL_H */
