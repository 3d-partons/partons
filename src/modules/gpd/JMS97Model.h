#ifndef JMS97_MODEL_H
#define JMS97_MODEL_H

/**
 * @file JMS97Model.h
 * @author Bryan BERTHOU (CEA Saclay) - Herve MOUTARDE (CEA Saclay)
 * @date 2014
 * @version 1.0
 *
 * Last update : 9 October 2014
 *
 * @class JMS97Model
 * @brief MIT Bag model of GPDs after Ji, Melnitchouk and Song, Phys. Rev. D56 (1997) 5511.
 * (arXiv:hep-ph/9702379v2).
 *
 *
 * !!!! Important modification : here xi = -n.Delta /2 i.e. half the definition of
 * Ji, Melnitchouk and Song. Hence xi lives in [-1;+1] and no more in [-2;+2].
 *
 *
 * !!!! Use tgamma() instead of gamma() (see: http://stackoverflow.com/questions/18116376/what-is-the-definition-for-gammadouble-x-and-why-is-it-different-on-two-gcc-ve)
 */

#include <string>
#include <vector>

#include "../../beans/gpd/GPDComputeType.h"
#include "../GPDModule.h"

class GPDOutputData;
class GPDResultData;

//TODO Certainement plein de choses !

class JMS97: public GPDModule {
public:

    static const std::string moduleID; ///< Unique ID to automatically register the module in the factory.

    /**
     * Default constructor
     */
    JMS97Model();

    JMS97Model(const JMS97Model& other);

    /**
     * Default destructor
     */
    virtual ~JMS97Model();

    virtual JMS97Model* clone() const;

    /**
     * Call by default constructor to initialize all class attributes
     */
    void init();

    /**
     * Compute GPD described in Ji, Melnitchouk and Song model in 1997. \n
     * Compute GPD_H, GPD_Ht, GPD_E, GPD_Et for up and down. Set strange quark contribution to 0. \n
     * See GPDModule for more details.
     */
    virtual GPDOutputData compute(const double &_x, const double &_xi,
            const double &_t, const double &_MuF, const double &_MuR,
            GPDComputeType::Type _gpdComputeType);

    // ##### GETTERS & SETTERS #####

	void SetParameters( double R, double eta );                ///<  Initialisation of kinematics-independent quantities

	void Print_GPD ();		///< Screen print of GPD values
	void Print_CFF ();		///< Screen print of CFF values
	void Print_FF ();		///< Screen print of FF values
	void Print_Params ();	///< Screen print of current parameters values

	void GetGPD( double tab_GPD[], double x, double xi, double t ); ///< Bag model GPDs for u and d quarks
	void GetCFF( double tab_CFF[], double xi, double t ); ///< Corresponding real and imaginary parts of Compton Form Factors
	void GetFF( double tab_FF[], double xi, double t ); 	///< Corresponding (flavour contributions to) form factors

    double getEdValMx() const;
    double getEtdValMx() const;
    double getEtuValMx() const;
    double getEuValMx() const;
    double getHdValMx() const;
    double getHtdValMx() const;
    double getHtuValMx() const;
    double getHuValMx() const;
    double getMuF2() const;
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


	// "Universal" Constants (stored as complex numbers)

    TComplex kM;                ///<    Proton's mass  (= 0.938272029 GeV) ( P.J. Mohr, B.N. Taylor, RMP 77, 1 (2005) )
    TComplex kHBarC;            ///<    \f$ \hbar * c\f$, conversion GeV <-> fm
    TComplex kPi;               ///<    \f$ \pi \f$
    TComplex I;                 ///<    \f$ i=\sqrt{-1}\f$
    TComplex un;                ///<    1


    // Limits of integration domains

	double kper_max;          ///<    Upper limit (definition, integration) for \f$ k_{\perp} \f$


	// Bag model parameters

    TComplex kOmega0;           ///<    \f$ \omega_0 =2.04 \f$ : Bag fundamental mode
    TComplex fR;                ///<    Bag radius
    TComplex fEpsilon0;         ///<    \f$ \epsilon_0 = \frac{\omega_0}{R}\f$ : energy of a quark in the bag fundamental mode
    TComplex fEta;              ///<    \f$ \eta \f$ : fEta * fDelta is the effective 4-momentum transfer on the active quark

    TComplex fMBar;             ///<    \f$ \overline{M} \f$ : nucleon's energy in Breit's frame
    TComplex fMBar_over_kM;     ///<    \f$ \frac{\overline{M}}{M} = cosh(\omega) \f$ (\f$ \omega = tanh^{-1}(v) \f$: v is the velocity of the nucleon in the Breit frame)
    TComplex fSinh2;            ///<    \f$ \frac{\vert \vec{\Delta} \vert}{2M} = sinh(\omega) \f$ (doute sur le fait que ce soit le carré)


		// (Combinations of) constants appearing several times in calculations

    TComplex kOmega0_2;                    ///< \f$ \omega_0^2 \f$
    TComplex kJ0_Omega0_fois_Omega0;       ///< \f$ J_0(\omega_0) * \omega_0 \f$
    TComplex kCos_Omega0;                  ///< \f$ cos(\omega_0) \f$
    TComplex kJ0_Omega0;                   ///< \f$ J_0(\omega_0) \f$
    TComplex kJ1_Omega0;                   ///< \f$ J_1(\omega_0) \f$


    // Kinematics

    TComplex ft;            ///<    Square of 4-momentum transfer on the nucleon
    TComplex fXi;           ///<    Half-variation of the active quark "+" momentum
    TComplex f2Xi;			///<	2 times fXi
    TComplex fX;            ///<    "+" momentum fraction of the active quark


	// Misc.

    double fA;             ///< 	Coefficient of E in eq. (27)
    double fB;             ///< 	Coefficient of \f$ \tilde{H} \f$ in eq. (30)
    double fC;             ///< 	Coefficient of \f$ \tilde{E} \f$ in eq. (30)
    double fD;             ///< 	Coefficient of \f$ \tilde{H} \f$ in eq. (31)
    double fE;             ///< 	Coefficient of \f$ \tilde{E} \f$ in eq. (31)
    double fD2;            ///< 	Determinant of the 2x2 linear system defined by eq. (30) and (31) (\f$=fB*fE-fC*fD\f$)


    // 3-vectors

    TComplex fDelta[3];     ///<    3-Vector momentum transfer on nucleon


    // Reals

    double fZ2;           ///<    Contribution of spectator quarks. Square of rhs of eq. (28)

    double integFF1;      ///<    x-integration of rhs of eq. (27) for u and d quarks
    double integFF2;      ///<    x-integration of rhs of eq. (29) for u and d quarks
    double integFF3;      ///<    x-integration of rhs of eq. (30) for u and d quarks
    double integFF4;      ///<    x-integration of rhs of eq. (31) for u and d quarks

    double ValInteg1;     ///<    rhs of eq. (27) for u and d quarks
    double ValInteg2;     ///<    rhs of eq. (29) for u and d quarks
    double ValInteg3;     ///<    rhs of eq. (30) for u and d quarks
    double ValInteg4;     ///<    rhs of eq. (31) for u and d quarks

    double RecalInteg1a;  ///<    Complex analysis x-integration of [eq (27) /(x-xi)]
    double RecalInteg2a;  ///<    Complex analysis x-integration of [eq (29) /(x-xi)]
    double RecalInteg3a;  ///<    Complex analysis x-integration of [eq (30) /(x-xi)]
    double RecalInteg4a;  ///<    Complex analysis x-integration of [eq (31) /(x-xi)]

    double RecalInteg1b;  ///<    Integration of [eq (27) /(x+xi)]
    double RecalInteg2b;  ///<    Integration of [eq (29) /(x+xi)]
    double RecalInteg3b;  ///<    Integration of [eq (30) /(x+xi)]
    double RecalInteg4b;  ///<    Integration of [eq (31) /(x+xi)]

    double RecalInteg1;   ///<    RecalInteg1a - RecalInteg1b. See supra
    double RecalInteg2;   ///<    RecalInteg2a - RecalInteg2b. See supra
    double RecalInteg3;   ///<    RecalInteg3a + RecalInteg3b. See supra
    double RecalInteg4;   ///<    RecalInteg4a + RecalInteg4b. See supra

    double ImcalInteg1;   ///<    rhs of eq. (27) for u and d quarks for x = xi
    double ImcalInteg2;   ///<    rhs of eq. (29) for u and d quarks for x = xi
    double ImcalInteg3;   ///<    rhs of eq. (30) for u and d quarks for x = xi
    double ImcalInteg4;   ///<    rhs of eq. (31) for u and d quarks for x = xi



/*----------------------------- Tests variables ------------------------------*/

	// Printouts

	bool fVerbose;			///<	Screen prints from TBag


	// Initialisation flags

    bool Parameters_init;     ///<    Confirms parameters initialisation
    bool kin_init;            ///<    Confirms initialisation of kinematics dependent quantities


	// Check flags

    bool Flag_RecalInteg;     ///<    Confirms computation of linear combinations of integrals to evaluate CFF



/*--------------------------------- Methods ----------------------------------*/

    TComplex J0 (TComplex x);   //  Spherical Bessel function of the first kind \f$ j_0 \f$

    TComplex J1 (TComplex x);   //  Spherical Bessel function of the first kind \f$ j_1 \f$

    TComplex T0 (TComplex norme_k, TComplex R); //  (Proportional to) 3D Fourier transform of J0
    TComplex T1 (TComplex norme_k, TComplex R); //  (Proportional to) 3D Fourier transform of J1

    double TermeIntegreZ ( double *var, double *par ) ;
    void Cal_IntegreZ ();

    TComplex PSComplex ( TComplex u[3], TComplex v[3]);
    TComplex NormComplex ( TComplex u[3]);

    TComplex FuncIntermed1 ( TComplex xi, TComplex t, TComplex kprime[3], TComplex k[3], TComplex delta[3], TComplex R);
    TComplex FuncIntermed2 ( TComplex xi, double eta, TComplex k[3], TComplex kprime[3], TComplex delta[3], TComplex R );
    TComplex FuncIntermed3 ( TComplex xi, TComplex t, TComplex k[3], TComplex kprime[3], TComplex delta[3], TComplex R);
    TComplex FuncIntermed4 ( TComplex k[3], TComplex kprime[3], TComplex delta[3], TComplex R);
    TComplex FuncIntermed5 ( TComplex k[3], TComplex kprime[3], TComplex delta[3], TComplex R);
    TComplex FuncIntermed6 ( TComplex xi, TComplex t, TComplex k[3], TComplex kprime[3], TComplex delta[3], TComplex R );

    TComplex ComposanteComplex_kz ( TComplex x, TComplex xi, TComplex t, TComplex eta, TComplex delta[3], TComplex kper, TComplex phi); ///< rhs of eq. (26)

    TComplex CombLin1 (TComplex kper, TComplex phi, TComplex x, TComplex xi, TComplex t, TComplex eta, TComplex R); 	///< rhs of eq. (27) up to a multiplication by Z2
    TComplex CombLin2 (TComplex kper, TComplex phi, TComplex x, TComplex xi, TComplex t, TComplex eta, TComplex R); 	///< rhs of eq. (29) up to a multiplication by Z2
    TComplex CombLin3 (TComplex kper, TComplex phi, TComplex x, TComplex xi, TComplex t, TComplex eta, TComplex R);     ///< rhs of eq. (30) up to a multiplication by Z2
    TComplex CombLin4 (TComplex kper, TComplex phi, TComplex x, TComplex xi, TComplex t, TComplex eta, TComplex R);     ///< rhs of eq. (31) up to a multiplication by Z2

    TComplex CombLin1Contourplus (double *var, double *par);  ///< CombLin1 for x = 0.5 + 0.5 * exp(i * theta)
    TComplex CombLin2Contourplus (double *var, double *par);  ///< CombLin2 for x = 0.5 + 0.5 * exp(i * theta)
    TComplex CombLin3Contourplus (double *var, double *par);  ///< CombLin3 for x = 0.5 + 0.5 * exp(i * theta)
    TComplex CombLin4Contourplus (double *var, double *par);  ///< CombLin4 for x = 0.5 + 0.5 * exp(i * theta)

    TComplex CombLin1Contourmoins (double *var, double *par); ///< CombLin1 for x = - 0.5 - 0.5 * exp(i * theta)
    TComplex CombLin2Contourmoins (double *var, double *par); ///< CombLin2 for x = - 0.5 - 0.5 * exp(i * theta)
    TComplex CombLin3Contourmoins (double *var, double *par); ///< CombLin3 for x = - 0.5 - 0.5 * exp(i * theta)
    TComplex CombLin4Contourmoins (double *var, double *par); ///< CombLin4 for x = - 0.5 - 0.5 * exp(i * theta)

    TComplex CombLin1plus(double *var, double *par);    ///< CombLin1Contourplus - CombLin1Contourmoins
    TComplex CombLin2plus(double *var, double *par);  	///< CombLin2Contourplus - CombLin2Contourmoins
    TComplex CombLin3plus(double *var, double *par);    ///< CombLin3Contourplus + CombLin3Contourmoins
    TComplex CombLin4plus(double *var, double *par);  	///< CombLin4Contourplus + CombLin4Contourmoins

    double RecalCombLin1Contour (double *var, double *par); ///< x-integral kernel of [rhs of eq. (27)/(x-xi)]
    double RecalCombLin2Contour (double *var, double *par); ///< x-integral kernel of [rhs of eq. (29)/(x-xi)]
    double RecalCombLin3Contour (double *var, double *par); ///< x-integral kernel of [rhs of eq. (30)/(x-xi)]
    double RecalCombLin4Contour (double *var, double *par); ///< x-integral kernel of [rhs of eq. (31)/(x-xi)]

    double RecalCombLin1axeR (double *var, double *par); ///< x-integral kernel of [rhs of eq. (27)/(x+xi)]
    double RecalCombLin2axeR (double *var, double *par); ///< x-integral kernel of [rhs of eq. (29)/(x+xi)]
    double RecalCombLin3axeR (double *var, double *par); ///< x-integral kernel of [rhs of eq. (30)/(x+xi)]
    double RecalCombLin4axeR (double *var, double *par); ///< x-integral kernel of [rhs of eq. (31)/(x+xi)]

    double ImcalCombLin1axeR (double *var, double *par);
    double ImcalCombLin2axeR (double *var, double *par);
    double ImcalCombLin3axeR (double *var, double *par);
    double ImcalCombLin4axeR (double *var, double *par);

    double functionintegFF1 (double *var, double *par);  	///< x-integral kernel of [rhs of eq. (27)]
    double functionintegFF2 (double *var, double *par);  	///< x-integral kernel of [rhs of eq. (29)]
    double functionintegFF3 (double *var, double *par);  	///< x-integral kernel of [rhs of eq. (30)]
    double functionintegFF4 (double *var, double *par);  	///< x-integral kernel of [rhs of eq. (31)]

    double Integ1( double *var, double *par);	///< rho and phi-integral kernel of [rhs of eq. (27)]
    double Integ2( double *var, double *par);	///< rho and phi-integral kernel of [rhs of eq. (29)]
    double Integ3( double *var, double *par);	///< rho and phi-integral kernel of [rhs of eq. (30)]
    double Integ4( double *var, double *par);	///< rho and phi-integral kernel of [rhs of eq. (31)]

    void Cal_Integ1(); 	///< Evaluates the integral of the function Integ1
    void Cal_Integ2(); 	///< Evaluates the integral of the function Integ2
    void Cal_Integ3(); 	///< Evaluates the integral of the function Integ3
    void Cal_Integ4(); 	///< Evaluates the integral of the function Integ4

    void Cal_RecalInteg1a ();
    void Cal_RecalInteg1b ();
    void Cal_ImcalInteg1 ();
    void Cal_integFF1 ();

    void Cal_RecalInteg2a ();
    void Cal_RecalInteg2b ();
    void Cal_ImcalInteg2 ();
    void Cal_integFF2 ();

    void Cal_RecalInteg3a ();
    void Cal_RecalInteg3b ();
    void Cal_ImcalInteg3 ();
    void Cal_integFF3 ();

    void Cal_RecalInteg4a ();
    void Cal_RecalInteg4b ();
    void Cal_ImcalInteg4 ();
    void Cal_integFF4 ();

    double IminterInteg1( double *var, double *par);
    double IminterInteg2( double *var, double *par);
    double IminterInteg3( double *var, double *par);
    double IminterInteg4( double *var, double *par);

    void Cal_RecalInteg ();

	void SetKinematics( double x, double xi, double t );     ///<  Initialisation of kinematics-dependent quantities

    void FillGPD (double *TabGPD, Int_t flavor);	///< Fills an array with the Bag model values of GPDs
    void FillCFF ( double *Tab_Re_CFF, double *Tab_Im_CFF, Int_t flavor );	///< Fills an array with the Bag model values of CFFs
    void FillFF (double *TabFF, Int_t flavor);	///< Fills an array with the Bag model values of FFs

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

#endif /* JMS97_MODEL_H */
