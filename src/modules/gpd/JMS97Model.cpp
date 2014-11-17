#include <cmath>
#include <iostream>

#include "../../beans/gpd/GPDOutputData.h"
#include "../../beans/gpd/GPDQuarkFlavorData.h"
#include "../../beans/gpd/GPDResultData.h"
#include "../../beans/QuarkFlavor.h"
#include "../../FundamentalPhysicalConstants.h"
#include "../../services/ModuleObjectFactory.h"
#include "../../utils/logger/LoggerManager.h"
#include "../../utils/stringUtils/Formatter.h"

// Initialise JMS97GPDModule::ID with a unique name.
const std::string JMS97Model::moduleID = "JMS97Model";

// Define a useless static boolean variable to enable registerModule() to be executed before the main() function.
// Because global variables have program scope, and are initialised before main() is called.
// !!! CARE !!! variable name must be unique (ex : isGK11ModelRegistered).
static bool isJMS97ModelRegistered =
        ModuleObjectFactory::getInstance()->registerModule(new JMS97Model());

JMS97Model::JMS97Model() :
        GPDModule(GK11Model::moduleID) {
    init();
    std::cerr << "[JMS97Model] default constructor | id = " << getId()
            << std::endl;
}

JMS97Model::JMS97Model(const JMS97Model& other) :
        GPDModule(other) {
    fMuF2 = other.getMuF2();

    std::cerr << "[JMS97Model] copy constructor| id = " << getId() << std::endl;
}

JMS97Model* JMS97Model::clone() const {
    return new JMS97Model(*this);
}

void GK11Model::init() {
    fQ0 = 4.;
}

JMS97Model::~JMS97Model() {
}

void JMS97Model::updateVariables(const double &_MuF) {

    fMuF2 = _MuF * _MuF;

    m_pLoggerManager->debug(getClassName(), __func__,
            Formatter() << "fMuF2 = " << fMuF2);

    m_pLoggerManager->debug(getClassName(), __func__,
            Formatter() << "fMuF2 = " << fMuF2);
    //    }
}

GPDOutputData JMS97Model::compute(const double &_x, const double &_xi,
        const double &_t, const double &_MuF, const double &_MuR,
        GPDComputeType::Type _gpdComputeType) {

    GPDOutputData gpdOutputData;

    // And after, update JMS97 variables before computing
    updateVariables(_MuF);

    switch (_gpdComputeType) {
    case GPDComputeType::H: {
        gpdOutputData.addGPDResultData(computeH(_x, _xi, _t));
        break;
    }
    case GPDComputeType::Ht: {
        gpdOutputData.addGPDResultData(computeHt(_x, _xi, _t));
        break;
    }
    case GPDComputeType::E: {
        gpdOutputData.addGPDResultData(computeE(_x, _xi, _t));
        break;
    }
    case GPDComputeType::Et: {
        gpdOutputData.addGPDResultData(computeEt(_x, _xi, _t));
        break;
    }
    default: {
        gpdOutputData.addGPDResultData(computeH(_x, _xi, _t));
        gpdOutputData.addGPDResultData(computeHt(_x, _xi, _t));
        gpdOutputData.addGPDResultData(computeE(_x, _xi, _t));
        gpdOutputData.addGPDResultData(computeEt(_x, _xi, _t));
    }
    }

    return gpdOutputData;
}

GPDResultData JMS97Model::computeH(const double &_x, const double &_xi,
        const double &_t) {
    GPDComputeType gpdComputeType(GPDComputeType::H);

    GPDResultData GPD_H(gpdComputeType);

    GPDQuarkFlavorData gpdQuarkFlavorData_u(gpdComputeType, QuarkFlavor::UP);
    GPDQuarkFlavorData gpdQuarkFlavorData_d(gpdComputeType, QuarkFlavor::DOWN);
    GPDQuarkFlavorData gpdQuarkFlavorData_s(gpdComputeType,
            QuarkFlavor::STRANGE);

    return GPD_H;
}

GPDResultData JMS97Model::computeHt(const double &_x, const double &_xi,
        const double &_t) {

    m_pLoggerManager->info(getClassName(), __func__, "Entered function ...");

    GPDComputeType gpdComputeType(GPDComputeType::Ht);

    GPDResultData GPD_Ht(gpdComputeType);

    GPDQuarkFlavorData gpdQuark_u(gpdComputeType, QuarkFlavor::UP);
    GPDQuarkFlavorData gpdQuark_d(gpdComputeType, QuarkFlavor::DOWN);
    GPDQuarkFlavorData gpdQuark_s(gpdComputeType, QuarkFlavor::STRANGE);

    return GPD_Ht;
}

GPDResultData JMS97Model::computeE(const double &_x, const double &_xi,
        const double &_t) {
    GPDComputeType gpdComputeType(GPDComputeType::E);

    GPDResultData GPD_E(gpdComputeType);

    GPDQuarkFlavorData gpdQuarkFlavorData_u(gpdComputeType, QuarkFlavor::UP);
    GPDQuarkFlavorData gpdQuarkFlavorData_d(gpdComputeType, QuarkFlavor::DOWN);
    GPDQuarkFlavorData gpdQuarkFlavorData_s(gpdComputeType,
            QuarkFlavor::STRANGE);

    return GPD_E;
}

GPDResultData JMS97Model::computeEt(const double &_x, const double &_xi,
        const double &_t) {
    GPDComputeType gpdComputeType(GPDComputeType::Et);

    GPDResultData GPD_Et(gpdComputeType);

    GPDQuarkFlavorData gpdQuarkFlavorData_u(gpdComputeType, QuarkFlavor::UP);
    GPDQuarkFlavorData gpdQuarkFlavorData_d(gpdComputeType, QuarkFlavor::DOWN);
    GPDQuarkFlavorData gpdQuarkFlavorData_s(gpdComputeType,
            QuarkFlavor::STRANGE);

    return GPD_Et;
}




/*---------------------------------- Constructors ----------------------------*/

TBag::TBag()
{

	fVerbose = kTRUE;

	cout << "TBag : TBag constructor called" << endl << endl;

    SetParameters( 8.7067, 0.35 );

} // end default TBag



TBag::TBag( Double_t R, Double_t eta )
{

	fVerbose = kFALSE;

    SetParameters(R, eta);

} // end TBag()



TBag::TBag( Double_t R, Double_t eta, Bool_t Print )
{

    fVerbose = Print;

    if ( fVerbose == kTRUE )
    {
    	cout << "TBag : TBag constructor called" << endl << endl;
    } // end if fVerbose

    SetParameters(R, eta);

} // end TBag()






/*----------------------------- Initialisations ------------------------------*/



/*------------- void TBag::SetParameters( Double_t R, Double_t eta ) ---------*/

/*!
 * \fn void TBag::SetParameters( Double_t R, Double_t eta )
 *
 * Initializes all kinematics-independent quantities.
 *
 * \param R : bag radius
 * \param eta : modelisation of effective momentum transfer
 *
 */
void TBag::SetParameters( Double_t R, Double_t eta )
{

    // constantes fondamentales

    kM = TComplex(0.938272029);                 // masse du proton en GeV ( P.J. Mohr, B.N. Taylor, RMP 77, 1 (2005) )
    kHBarC = TComplex(0.1973269631);            // constante de conversion en Gev.fm
    kPi = TComplex(TMath::Pi());
    I = TComplex::I();
    un = TComplex::One();


    // limite d'integration

    kper_max = 5.0;


    // modele du sac

    kOmega0 = TComplex(2.04);               // etat fondamental du quark dans le sac, sans dimension
    kOmega0_2 = TComplex::Power(kOmega0,2.);
    kJ0_Omega0 = J0(kOmega0);
    kJ1_Omega0 = J1 ( kOmega0 );
    kCos_Omega0 = TComplex::Cos( kOmega0 );
    kJ0_Omega0_fois_Omega0 = kJ0_Omega0 * kOmega0 ;
    fR = TComplex(R);                       // rayon du nucleon en Gev ( P.J. Mohr, B.N. Taylor, RMP 77, 1 (2005) )
    fEpsilon0 = kOmega0 / fR ;              // energie de l'etat fondamental
    fEta = TComplex(eta);                   // fEta * fDelta est le transfert d'impulsion effectif du quark actif


	// flags

    Parameters_init = kTRUE;

    kin_init = kFALSE;

    Flag_RecalInteg = kFALSE;

} // end def TBag::SetParameters



/*----------- TBag::SetKinematics(Double_t x, Double_t xi, Double_t t) -------*/

/*!
 * \fn void TBag::SetKinematics(Double_t x, Double_t xi, Double_t t)
 *
 * Fait les initialisations pour les constantes dependant de la cinematique
 *
 * \param x
 * \param xi
 * \param t
 *
 */
void TBag::SetKinematics(Double_t x, Double_t xi, Double_t t)
{

    // cinematique

    ft = TComplex(t);                   // carre du transfert en 4-impulsion
    fXi = TComplex(xi);                 // variation de la fraction d'impulsion longitudinale en energie du quark actif
                                        // 0 < fXi < sqrt(-ft)/fMBar/2
    f2Xi = TComplex(2)*fXi;
    fX = TComplex(x);                   // fraction d'impulsion moyenne du quark actif     -1 < fX < 1

    // I = TComplex::I();
    // un = TComplex::One();

    if(!Parameters_init)
    {
        cout << "TBag : Pas de parametres" << endl;
        exit(-1);
    } // end if Parameters_init


    if( fX.Re() > 1 || fX.Re() < -1)
    {
    	if ( fVerbose == kTRUE )
    	{
        	cout << "TBag : Warning !!!!!! Condition : -1 < x < 1 non respectee " << endl;
        	// exit (-1);
    	} // end if fVerbose
    } // end if fX.Re()


    if( ft.Re() > 0 )
    {
        cout << "TBag : Le tranfert doit être negatif ou nul " << endl;
        exit(-1);
    } // end if ft.Re()


    fMBar = TComplex::Sqrt( TComplex::Power(kM,2) - ft/TComplex(4.,0.));        // masse du proton en Gev dans le referentiel de Breit

    fMBar_over_kM = fMBar / kM;


    if( ft.Re() < 0 )
    {
        Double_t limite_xi = TMath::Sqrt( - ft.Re() ) / (fMBar.Re()) /2;

        if( fXi.Re() < 0 || fXi.Re() >  limite_xi )
        {
    		if ( fVerbose == kTRUE )
    		{
        		cout << "TBag : Warning !!!!!! Condition : 0 < xi < limite_xi = "<< limite_xi << " non respectee " << endl; // This bound is actually violated for some bins of JLab Hall A's measurements... Hence only warning...
           		// exit(-1);
    		} // end if fVerbose
    	} // end if fXi.Re()
        // TComplex factor = fMBar / ( un-(fMBar_over_kM -un)* fXi * fXi * fMBar * fMBar / ft );
    } // end if ft.Re()


    fSinh2 = (fMBar - kM) / (TComplex(2.,0.)*kM) ;                  // sinh^2(omega)

    TComplex fDeltaX = TComplex::Sqrt( - ft - TComplex::Conjugate( fMBar * f2Xi) * fMBar * f2Xi );    // composante en X de fDelta

    // fDelta[3]= { fDeltaX, TComplex(0.,0.) , - fXi * fMBar};          // 4-vecteur transfert d'impulsion dont la composante temporelle est nulle
    fDelta[0]=fDeltaX;
    fDelta[1]=TComplex(0.,0.);
    fDelta[2]=- f2Xi * fMBar;

    Double_t DeltaZ = fDelta[2].Re();
    Double_t M = kM.Re();
    Double_t Mbar = fMBar.Re();
    fA = ft.Re() / ( 4 * M * M);
    // fB = (TComplex(2,0) * ( un - f2Xi * f2Xi * fMBar/(TComplex(4.,0)*(kM+fMBar)))).Re();
    fB = 2 * ( 1 - DeltaZ*DeltaZ /(4.*Mbar*(M+Mbar)));
    // fC = (- f2Xi * f2Xi * fMBar / (2.*kM)).Re();
    fC = - DeltaZ*DeltaZ  / (2.*M* Mbar);
    // fD = (f2Xi * fDeltaX / (2.*(fMBar+kM))).Re();
    fD =  DeltaZ * fDeltaX / (2.*Mbar*(Mbar+M));
    // fE = (f2Xi * fDeltaX / (2.*kM)).Re();
    fE = DeltaZ * fDeltaX / (2.*Mbar*M);
    fD2 = fB * fE - fC * fD;

    if( fD2 == 0 )
    {
        cout << "TBag : Matrice de Et et Ht singuliere " << endl;
        exit(-1);
    } // end if fD2


    kin_init = kTRUE;

} // end def TBag::SetKinematics



/*-------------------------- TBag::J0 (TComplex x) ---------------------------*/

/*!
 * \fn TComplex J0 (TComplex x)
 * Premiere fonction de Bessel spherique
 * fait le calcul pour une variable complexe
 *
 * \param x : variable complexe
 * \return TComplex J0(x)
 */
TComplex TBag::J0 (TComplex x)
{
    return TComplex::Sin(x) / x;
} // end def TBag::J0



/*-------------------------- TBag::J1 (TComplex x) ---------------------------*/

/*!
 * \fn TComplex J1 (TComplex x)
 * Deuxieme fonction de Bessel spherique
 * fait le calcul pour une variable complexe
 *
 * \param x : variable complexe
 * \return TComplex J1(x)
 */
TComplex TBag::J1 (TComplex x)
{
    return TComplex::Sin(x) / (x*x) - TComplex::Cos(x)/x;
} // end def TBag::J1



/*------- Double_t TBag::TermeIntegreZ ( Double_t *var, Double_t *par ) ------*/

/*!
 * \fn Double_t TBag::TermeIntegreZ ( Double_t *var, Double_t *par )
 * Calcule le terme a integrer dans le calcul de Z
 * \f[ Z(t)\,=\,\frac{N^{2}}{\cosh\,\omega}\,\int_0^R \,dr\,r^{2}\,j_{0}\left(\epsilon _{0}\,\frac{\sqrt{-t}\,r}{\overline{M}}\right)\,\left(j_{0}^{2}(\epsilon _{0} r)\,+\,j_{1}^{2}(\epsilon _{0} r)\,\right) \f]
 *
 * \param var : tableau des variables = [u = r/R]
 * \param par : tableau des parametres = []
 *
 * \return integrande de Z
 */
Double_t TBag::TermeIntegreZ ( Double_t *var, Double_t */*par*/ )
{
    if(!kin_init)
    {
        cout << "TBag : Pas de configuration cinematique " << endl;
        exit(-1);
    } // end if kin_init


    Double_t u = var[0];            // u est ma variable d'integration adimensionnee -> u = r/R;

    TComplex A = kOmega0 * TComplex::Sqrt( - ft ) * TComplex(u,0.) / fMBar;     // changement de variable
    TComplex B = kOmega0 * u;                           // idem = epsilon_0 * r

    TComplex N2 = kOmega0 / ( TComplex(2.,0.) * (kOmega0 - un) * TComplex::Power(kJ0_Omega0,2));

    return (N2 * TComplex(u,0.) * TComplex(u,0.) * J0(A) * ( J0(B) * J0(B) + J1(B) * J1(B) ) * kM / fMBar).Re();

} // end def TBag::TermeIntegreZ



/*----------------------- void TBag::Cal_IntegreZ () -------------------------*/

/*!
 * \fn void TBag::Cal_IntegreZ ()
 *  Integration de TermeIntegreZ pour avoir Z(t)
 *
 * Initialise fZ2
 *
 */
void TBag::Cal_IntegreZ ()
{

    TF1 *g1 = new TF1("g1",this,&TBag::TermeIntegreZ, 0, 1., 0, "TBag", "TermeIntegreZ");
    Double_t fZ = g1->Integral(0,1);

    fZ2 = fZ * fZ;
    delete g1;
} // end def TBag::Cal_IntegreZ



/*-- TComplex TBag::ComposanteComplex_kz ( TComplex x, TComplex xi, TComplex t, TComplex eta, TComplex delta[3], TComplex kper, TComplex phi) --*/

/*!
 * \fn TComplex TBag::ComposanteComplex_kz ( TComplex x, TComplex xi, TComplex t, TComplex eta, TComplex delta[3], TComplex kper, TComplex phi)
 *
 * equation 26, expression de la composante z complexe du 3-vecteur k
 *
 * \param x (fX)
 * \param xi (f2Xi)
 * \param t (ft)
 * \param eta (fEta)
 * \param delta 4-vecteur transfert d'impulsion dont la composante temporelle est nulle (fDelta)
 * \param kper
 * \param phi
 *
 * \return kz composante complexe z du 3-vecteur k
 */
TComplex TBag::ComposanteComplex_kz ( TComplex x, TComplex xi, TComplex t, TComplex eta, TComplex delta[3], TComplex kper, TComplex phi)
{

    if(!kin_init)
    {
        cout << "TBag : Pas de configuration cinematique " << endl;
        exit(-1);
    } // end if kin_init

    TComplex A = fMBar / ( un - ( ( fMBar_over_kM - un ) * TComplex::Power( xi * fMBar,2.) / t ) );
    TComplex B = x - (( ( TComplex(2.,0.) * fEpsilon0 - eta * xi * kM )*(fMBar_over_kM ) - eta * t / TComplex(2.,0.)/ fMBar + TComplex(2.,0.) * delta[0] * xi * kper * TComplex::Cos(phi) * fMBar*(fMBar_over_kM - un)/ t) /( TComplex(2.,0.)*fMBar ));

    return A * B ;

} // end def TBag::ComposanteComplex_kz



/*-------- TComplex TBag::PSComplex ( TComplex u[3], TComplex v[3]) ----------*/

/*!
 * \fn TComplex TBag::PSComplex ( TComplex u[3], TComplex v[3])
 *
 * Produit scalaire de 2 vecteurs complexes a 3 composantes
 *
 * \param u premier vecteur
 * \param v second vecteur
 *
 * \return PSComplex(u,v)
 */
TComplex TBag::PSComplex ( TComplex u[3], TComplex v[3])
{
    return  u[0] * v[0] + u[1] * v[1] + u[2] * v[2];
} // end def TBag::PSComplex



/*-------------- TComplex TBag::NormComplex ( TComplex u[3]) -----------------*/

/*!
 * \fn TComplex TBag::NormComplex ( TComplex u[3])
 *
 * Norme complexe d'un vecteur (produit scalaire par lui-même)
 *
 * \param u
 *
 * \return NormComplex(u)
 */
TComplex TBag::NormComplex ( TComplex u[3])
{
    return TComplex::Sqrt(PSComplex(u,u));
} // end def TBag::NormComplex



/*----------- TComplex TBag::T0 ( TComplex norme_k, TComplex R ) -------------*/

/*!
 * \fn TComplex TBag::T0 ( TComplex norme_k, TComplex R )
 *
 * Composition de fonctions de Bessel
 * Equation 23a, premiere fonction composee de Bessel
 *
 * \param norme_k
 * \param R (fR)
 *
 * \return T0(norme_k, R)
 */
TComplex TBag::T0 ( TComplex norme_k, TComplex R )
{

    TComplex K = norme_k * R ;              // norme_k designe la norme de k
                                            // Changement de variable
    TComplex A = kJ0_Omega0 * TComplex::Cos( K );
    TComplex B = J0 ( K ) * kCos_Omega0;

    return ( A - B ) / ( kOmega0_2 - TComplex::Power(K,2.) );
} // end def TBag::T0



/*----------- TComplex TBag::T1 ( TComplex norme_k, TComplex R ) -------------*/

/*!
 * \fn TComplex TBag::T1 ( TComplex norme_k, TComplex R )
 *
 * Composition de fonctions de Bessel
 * Equation 23b, seconde fonction composee de Bessel
 *
 * \param norme_k
 * \param R (fR)
 *
 * \return T1(norme_k, R)
 */
TComplex TBag::T1 ( TComplex norme_k, TComplex R )
{

    TComplex K = norme_k * R ;              // norme_k designe la norme de k
                                            // Changement de variable

    TComplex A = J0 ( K ) * kJ1_Omega0 * K ;
    TComplex B = J1 ( K ) * kJ0_Omega0_fois_Omega0;

    return ( A - B ) / ( kOmega0_2 - TComplex::Power(K,2) );
} // end def TBag::T1



/*------- TComplex TBag::FuncIntermed1( TComplex xi, TComplex t, TComplex kprime[3], TComplex k[3], TComplex delta[3], TComplex R) ------*/

/*!
 * \fn TComplex TBag::FuncIntermed1( TComplex xi, TComplex t, TComplex kprime[3], TComplex k[3], TComplex delta[3], TComplex R)
 *
 * Premiere fonction intermediaire utile pour le calcul de l'equation 27 ( jusqu'a la fin de la 2-ieme ligne )
 *
 * \param xi
 * \param t
 * \param kprime
 * \param k
 * \param delta
 * \param R
 *
 * \return FuncIntermed1( xi, t, kprime[3], k[3], delta[3], R)
 */
TComplex TBag::FuncIntermed1( TComplex xi, TComplex t, TComplex kprime[3], TComplex k[3], TComplex delta[3], TComplex R) {

    if(!kin_init)
    {
        cout << " Pas de configuration cinematique " << endl;
        exit(-1);
    } // end if kin_init

    TComplex norme_k = NormComplex(k);                  // norme de k
    TComplex norme_kprime = NormComplex(kprime);        // norme de k'

    TComplex ps = PSComplex(kprime, delta);         // produit scalaire de k' avec delta

    return ( kprime[2] * fMBar_over_kM - TComplex(2.,0.) * xi * fMBar * ps * fSinh2 / t ) * T0 (norme_k , R) * T1 (norme_kprime , R) / norme_kprime ;

} // end def TBag::FuncIntermed1



/*------- TComplex TBag::FuncIntermed2 ( TComplex xi, Double_t eta, TComplex k[3], TComplex kprime[3], TComplex delta[3], TComplex R ) ------*/

/*!
 * \fn TComplex TBag::FuncIntermed2 ( TComplex xi, Double_t xi, Double_t t, Double_t eta, TComplex k[3], TComplex kprime[3], TComplex delta[3], TComplex R )
 *
 * Deuxieme fonction intermediaire utile pour le calcul de l'equation 27 ( 5-ieme ligne )
 *
 * \param xi
 * \param eta
 * \param k
 * \param kprime
 * \param delta
 * \param R
 *
 * \return FuncIntermed2 ( xi, eta, k[3], kprime[3], delta[3], R ) = expression 5-ieme ligne.
 */
TComplex TBag::FuncIntermed2 ( TComplex xi, Double_t eta, TComplex k[3], TComplex kprime[3], TComplex delta[3], TComplex R )
{

    if(!kin_init)
    {
        cout << "TBag : Pas de configuration cinematique " << endl;
        exit(-1);
    } // end if kin_init


    TComplex norme_k = NormComplex(k);                  // norme de k
    TComplex norme_kprime = NormComplex(kprime);        // norme de k'

    TComplex v[3];                  // vecteur intermediaire
    for(Int_t i=0; i<=2; i++) {
        v[i] = - xi * k[i] - delta[i] * (k[2] / fMBar );
    }

    return ( PSComplex(k, kprime) - ( ( eta /TComplex(2.,0.)) * PSComplex(delta, v) ) ) * T1(norme_k,R) * T1(norme_kprime,R) / norme_k / norme_kprime;

} // end def TBag::FuncIntermed2



/*------- TComplex TBag::FuncIntermed3 (TComplex xi, TComplex t, TComplex k[3], TComplex kprime[3], TComplex delta[3], TComplex R) ------*/

/*!
 * \fn TComplex TBag::FuncIntermed3 (TComplex xi, TComplex t, TComplex k[3], TComplex kprime[3], TComplex delta[3], TComplex R)
 *
 * Troisieme fonction intermediaire utile pour le calcul de l'equation 30 ( last 1.5 lines )
 *
 * \param xi
 * \param t
 * \param k
 * \param kprime
 * \param delta
 * \param R
 *
 * \return FuncIntermed3 ( xi, t, k[3], kprime[3], delta[3], R ) = expression last 1.5 lines.
 */
TComplex TBag::FuncIntermed3 (TComplex xi, TComplex t, TComplex k[3], TComplex kprime[3], TComplex delta[3], TComplex R)
{

    if(!kin_init)
    {
        cout << "TBag : Pas de configuration cinematique " << endl;
        exit(-1);
    } // end if kin_init

    TComplex norme_k = NormComplex(k);                  // norme de k
    TComplex norme_kprime = NormComplex(kprime);        // norme de k'

    return ( (TComplex(2.,0.) * k[2] * kprime[2] - PSComplex(k,kprime)) * fMBar_over_kM - TComplex(2.,0) * xi * fMBar * fSinh2 * ( k[2] * ( PSComplex(k,delta) + PSComplex(kprime,delta) ) + xi*fMBar*PSComplex(k,k) ) / t ) * T1(norme_k, R) * T1(norme_kprime, R) / norme_k / norme_kprime;

} // end def TBag::FuncIntermed3



/*------- TComplex TBag::FuncIntermed4( TComplex k[3], TComplex kprime[3], TComplex delta[3], TComplex R) ------*/

/*!
 * \fn TComplex TBag::FuncIntermed4( TComplex k[3], TComplex kprime[3], TComplex delta[3], TComplex R)
 *
 * Quatrieme fonction intermediaire utile pour le calcul de l'equation 30 ( rhs till beginning of last 1.5 lines )
 *
 * \param k
 * \param kprime
 * \param delta
 * \param R
 *
 * \return FuncIntermed4( k[3], kprime[3], delta[3], R) = rhs till beginning of last 1.5 lines
 */
TComplex TBag::FuncIntermed4( TComplex k[3], TComplex kprime[3], TComplex delta[3], TComplex R)
{

    if(!kin_init)
    {
        cout << "TBag : Pas de configuration cinematique " << endl;
        exit(-1);
    } // end if kin_init


    TComplex norme_k = NormComplex(k);                      // norme de k
    TComplex norme_kprime = NormComplex(kprime);            // norme de k'


    return ( kprime[2]+ delta[0] * kprime[0] /TComplex(2.,0.)/kM ) * T0(norme_k,R) * T1(norme_kprime,R) / norme_kprime + ( k[2] - delta[0] * k[0] /TComplex(2.,0.)/kM ) * T1(norme_k,R) *T0(norme_kprime,R)/norme_k;

} // end def TBag::FuncIntermed4



/*------- TComplex TBag::FuncIntermed5 (TComplex k[3], TComplex kprime[3], TComplex delta[3], TComplex R) ------*/

/*!
 * \fn TComplex TBag::FuncIntermed5 (TComplex k[3], TComplex kprime[3], TComplex delta[3], TComplex R)
 *
 * Cinquieme fonction intermediaire utile pour le calcul de l'equation 31 ( 2-ieme et 3-ieme lignes )
 *
 * \param k
 * \param kprime
 * \param delta
 * \param R
 *
 * \return FuncIntermed5( k[3], kprime[3], delta[3], R) = expression 2-ieme et 3-ieme lignes.
 */
TComplex TBag::FuncIntermed5 (TComplex k[3], TComplex kprime[3], TComplex delta[3], TComplex R)
{

    if(!kin_init)
    {
        cout << "TBag : Pas de configuration cinematique " << endl;
        exit(-1);
    } // end if kin_init


    TComplex norme_k = NormComplex(k);                      // norme de k
    TComplex norme_kprime = NormComplex(kprime);            // norme de k'

    TComplex A = ( k[0] + delta[0] * k[2] /TComplex(2.,0.)/kM ) * T0(norme_kprime,R) * T1(norme_k,R) / norme_k;
    TComplex B = ( kprime[0] - delta[0] * kprime[2] /TComplex(2.,0.)/kM ) * T1(norme_kprime,R) * T0(norme_k,R) / norme_kprime;

    return  A + B;

} // end def TBag::FuncIntermed5



/*------- TComplex TBag::FuncIntermed6 (TComplex xi, TComplex t, TComplex k[3], TComplex kprime[3], TComplex delta[3], TComplex R ) ------*/

/*!
 * \fn TBag::FuncIntermed6 ( xi, t, k[3], kprime[3], delta[3], R )
 *
 * Sixieme fonction intermediaire utile pour le calcul de l'equation 31 ( 4-ieme et 5-ieme lignes )
 *
 * \param xi
 * \param t
 * \param k
 * \param kprime
 * \param delta
 * \param R
 *
 * \return FuncIntermed6 ( xi, t, k[3], kprime[3], delta[3], R ) = expression 4-ieme et 5-ieme lignes.
 */
TComplex TBag::FuncIntermed6 (TComplex xi, TComplex t, TComplex k[3], TComplex kprime[3], TComplex delta[3], TComplex R )
{

    if(!kin_init)
    {
        cout << "TBag : Pas de configuration cinematique " << endl;
        exit(-1);
    } // end if kin_init


    TComplex norme_k = NormComplex(k);                      // norme de k
    TComplex norme_kprime = NormComplex(kprime);            // norme de k'

    TComplex A = ( k[0] * kprime[2] + k[2] * kprime[0] ) * fMBar_over_kM;   // cinquieme ligne
    TComplex B = ( TComplex(-2.,0) * xi * fMBar * ( k[0] * (PSComplex(k,delta) + PSComplex(kprime,delta)) - delta[0] * PSComplex(k, k)) * fSinh2 / t); // sixieme ligne

    return ( A + B )* T1(norme_k,R) * T1(norme_kprime, R) / norme_k / norme_kprime ;

} // end def TBag::FuncIntermed6



/*------- TComplex TBag::CombLin1( TComplex kper, TComplex phi, TComplex x, TComplex xi, TComplex t, TComplex eta, TComplex R) ------*/

/*!
 * \fn TComplex TBag::CombLin1( TComplex kper, TComplex phi, TComplex x, TComplex xi, TComplex t, TComplex eta, TComplex R)
 * terme a integrer de l'equation 27
 *
 * \param kper
 * \param phi
 * \param x
 * \param xi
 * \param t
 * \param eta
 * \param R
 *
 * \return terme a integrer de l'equation 27
 */
TComplex TBag::CombLin1( TComplex kper, TComplex phi, TComplex x, TComplex xi, TComplex t, TComplex eta, TComplex R)
{

    if(!kin_init)
    {
        cout << "TBag : Pas de configuration cinematique " << endl;
        exit(-1);
    } // end if kin_init

    TComplex kx = kper * TComplex::Cos(phi);                // Formules de changement de base ( coordonnees cylindriques )
    TComplex ky = kper * TComplex::Sin(phi);
    TComplex kz = ComposanteComplex_kz (x, xi, t, eta, fDelta, kper, phi);

    TComplex k[3] = {kx, ky, kz};

    TComplex kprime[3];
    for(Int_t i=0; i<3;i++)
    {
        kprime[i] = k[i] + eta * (kM/fMBar) * fDelta[i];
    } // end for i


    TComplex norme_k = NormComplex(k);                              // norme de k
    TComplex norme_kprime = NormComplex(kprime);                    // norme de k'


    TComplex N2 = kOmega0 / ( TComplex(2.,0.) * TComplex::Power(R,3.) * (kOmega0 - un) * TComplex::Power(kJ0_Omega0,2));
    TComplex R6 = TComplex::Power(R,6.);
    TComplex factor = fMBar / (un - ( fMBar_over_kM - un) * xi * xi * fMBar * fMBar / t );

    return TComplex(4.,0.) * kPi * N2 * R6 * factor * ( kper * (T0 (norme_k,R) * T0( norme_kprime,R ) + FuncIntermed1( xi, t, kprime , k, fDelta, R ) + FuncIntermed1( xi, t, k , kprime, fDelta, R ) + FuncIntermed2( xi, eta, k , kprime, fDelta, R ) ) ) / TComplex::Power(TComplex(2.,0.)*kPi,3.);

} // end def TBag::CombLin1



/*----------- Double_t TBag::Integ1( Double_t *var, Double_t *par) -----------*/

/*!
 * \fn Double_t TBag::Integ1( Double_t *var, Double_t *par)
 * Extraction de la partie reelle de TBag::CombLin1
 *
 * \param var = [kper, phi]
 * \param par = [xi, t, eta, R, x]
 *
 * \return Re ( CombLin1 (kper,phi, x, xi, t, eta, R) )
 *
 */
Double_t TBag::Integ1( Double_t *var, Double_t *par)
{

    return (CombLin1( TComplex (var[0],0), TComplex (var[1],0), TComplex (par[4],0) , TComplex (par[0],0), TComplex (par[1],0), TComplex (par[2],0), TComplex (par[3],0) )).Re();

} // end def TBag::Integ1



/*------------------------- void TBag::Cal_Integ1() --------------------------*/

/*!
 * \fn void TBag::Cal_Integ1()
 *
 * Fait le calcul de l'integrale de la fonction TBag::Integ1
 * kper in [0,5], phi in [0,pi]
 * Multiplie par 2 le resultat pour faire l'integrale de 0 a 2pi sur phi (symetrie p/r a pi)
 *
 * H + a E = Integ1
 * H + E = Integ2
 *
 */
void TBag::Cal_Integ1()
{

    TF2 *i1 = new TF2("i1",this,&TBag::Integ1, 0, kper_max, 0, 2*TMath::Pi(), 5, "TBag","Integ1");
    //i1 : fonction de kper et phi, avec comme parametres [xi, t, eta, R, x]

    i1->SetParameters(f2Xi.Re(), ft.Re(), fEta.Re(), fR.Re(), fX.Re());
    i1->SetNpy(400);

    ValInteg1 = i1->Integral(0, kper_max, 0, TMath::Pi(), 0.0001)*2*fZ2;
    // Integrale sur kper de 0 a kper_max et sur phi de 0 a 2*Pi (en faisant 2* Integrale de 0 a Pi)


    delete i1;

} // end def TBag::Cal_Integ1()



/*----- TComplex TBag::CombLin1Contourplus (Double_t *var, Double_t *par) ----*/

/*!
 * \fn TComplex TBag::CombLin1Contourplus (Double_t *var, Double_t *par)
 * expression et integration de la fonction CombLin1 sur le demi-cercle \f$ \Gamma \f$ de rayon 1/2 et de centre (1/2,0)
 * Membre de droite de l'equation 27 sur gamma pour x = 0.5 + 0.5 exp(i*theta)
 *
 * z = 0.5 + 0.5 exp(i*theta)
 *
 * \param var = [ kper, phi,theta ]
 * \param par = [ xi, t, eta, R ]
 *
 * \return CombLin1(kper, phi, z, xi, t, eta, R)
 */
TComplex TBag::CombLin1Contourplus (Double_t *var, Double_t *par)
{

    TComplex x = TComplex (0.5,0) + TComplex(0.5,0) * TComplex::Exp(I * TComplex(var[2],0));

    return CombLin1( TComplex (var[0],0), TComplex (var[1],0), x , TComplex (par[0],0), TComplex (par[1],0), TComplex (par[2],0), TComplex (par[3],0) );

} // end def TBag::CombLin1Contourplus



/*---- TComplex TBag::CombLin1Contourmoins (Double_t *var, Double_t *par) ----*/

/*!
 * \fn TComplex TBag::CombLin1Contourmoins (Double_t *var, Double_t *par)
 * expression et integration de la fonction CombLin1 sur le demi-cercle \f$ \Gamma \f$ de rayon 1/2 et de centre (-1/2,0)
 * Membre de droite de l'equation 27 sur gamma pour x = -0.5 - 0.5 exp(i*theta)
 *
 * z = -0.5 - 0.5 exp(i*theta)
 *
 * \param var = [ kper, phi,theta ]
 * \param par = [ xi, t, eta, R ]
 *
 * \return CombLin1(kper, phi, z, xi, t, eta, R)
 */
TComplex TBag::CombLin1Contourmoins (Double_t *var, Double_t *par)
{

    TComplex x = - TComplex (0.5,0) - TComplex(0.5,0) * TComplex::Exp(I * TComplex(var[2],0));
    // var = [ kper, phi ]
    // par = [ x, xi, t, eta, R ]

    return CombLin1( TComplex (var[0],0), TComplex (var[1],0), x , TComplex (par[0],0), TComplex (par[1],0), TComplex (par[2],0), TComplex (par[3],0) );

} // end def TBag::CombLin1Contourmoins



/*---------- TComplex TBag::CombLin1plus(Double_t *var, Double_t *par) -------*/

/*!
 * \fn TComplex TBag::CombLin1plus(Double_t *var, Double_t *par)
 *
 * expression de CombLin1plus sur le contour gamma (demi-cercle autour de (-1/2,0) et demi-cercle autour de (1/2,0),
 *  i.e (CombLin1Contourplus - CombLin1Contourmoins) / (z - xi)
 *
 * \param var = [ kper, phi,theta ]
 * \param par = [ xi, t, eta, R ]
 *
 * \return (CombLin1Contourplus - CombLin1Contourmoins) / (z - xi)
 *
 */
TComplex TBag::CombLin1plus(Double_t *var, Double_t *par)
{

    TComplex z = TComplex(0.5,0) + TComplex(0.5,0)*TComplex::Exp(I * TComplex(var[2],0));

    return (CombLin1Contourplus (var, par) - CombLin1Contourmoins (var, par)) / (z - fXi);

} // end def TBag::CombLin1plus



/*---- Double_t TBag::RecalCombLin1Contour (Double_t *var, Double_t *par) ----*/

/*!
 * \fn Double_t TBag::RecalCombLin1Contour (Double_t *var, Double_t *par)
 *
 * cf formule 62 de Guillaume
 * Fonction necessaire au calcul de Re(calInteg1)
 *
 * \param var = [ kper, phi,theta ]
 * \param par = [ xi, t, eta, R ]
 *
 * \return - 0.5 * (Re(CombLin1plus) * Sin(theta) + Im(CombLin1plus) * Cos(theta));
 */
Double_t TBag::RecalCombLin1Contour (Double_t *var, Double_t *par)
{

    TComplex intermediaire = CombLin1plus (var, par);

    return - 0.5 * (intermediaire.Re() * TMath::Sin(var[2]) + intermediaire.Im() * TMath::Cos(var[2]));

} // end def TBag::RecalCombLin1Contour



/*------ Double_t TBag::RecalCombLin1axeR (Double_t *var, Double_t *par) -----*/

/*!
 * \fn Double_t TBag::RecalCombLin1axeR (Double_t *var, Double_t *par)
 *
 * Fonction donnant Re(CombLin1(x) - CombLin1(-x)) / (x+xi) sur l'axe [0;1]
 *
 * \param var = [ kper, phi,x ]
 * \param par = [ xi, t, eta, R ]
 *
 * \return Re(CombLin1(x) - CombLin1(-x)) / (x+xi)
 */
Double_t TBag::RecalCombLin1axeR (Double_t *var, Double_t *par)
{

    return (CombLin1( TComplex (var[0],0), TComplex (var[1],0), TComplex (var[2],0), TComplex (par[0],0), TComplex (par[1],0), TComplex (par[2],0), TComplex (par[3],0) )
        - (CombLin1( TComplex (var[0],0), TComplex (var[1],0), TComplex (- var[2],0), TComplex (par[0],0), TComplex (par[1],0), TComplex (par[2],0), TComplex (par[3],0)))).Re()
        / (var[2] + fXi.Re() );

} // end def TBag::RecalCombLin1axeR



/*------ Double_t TBag::ImcalCombLin1axeR (Double_t *var, Double_t *par) -----*/

/*!
 * \fn Double_t TBag::ImcalCombLin1axeR (Double_t *var, Double_t *par)
 * Fonction donnant CombLin1(x) - CombLin1(-x)
 * en l'evaluant en x = xi, on pourra acceder aux parties imaginaires des CFF
 *
 * \param var = [ kper, phi,x ]
 * \param par = [ xi, t, eta, R ]
 *
 * \return Re(CombLin1(x) - CombLin1(-x))
 */
Double_t TBag::ImcalCombLin1axeR (Double_t *var, Double_t *par)
{

    return ( CombLin1( TComplex (var[0],0), TComplex (var[1],0), TComplex (var[2],0), TComplex (par[0],0), TComplex (par[1],0), TComplex (par[2],0), TComplex (par[3],0))
        - CombLin1( TComplex (var[0],0), TComplex (var[1],0), TComplex (- var[2],0), TComplex (par[0],0), TComplex (par[1],0), TComplex (par[2],0), TComplex (par[3],0))).Re();

} // end def TBag::ImcalCombLin1axeR



/*--------------------- void TBag::Cal_RecalInteg1a () -----------------------*/

/*!
 * \fn void TBag::Cal_RecalInteg1a ()
 * calcule l'integrale de RecalCombLin1Contour pour kper de 0 a kper_max, phi de 0 a Pi (*2), theta de 0 a Pi
 * initialise RecalInteg1a
 *
 */
void TBag::Cal_RecalInteg1a ()
{

    TF3 *a1 = new TF3("a1", this, &TBag::RecalCombLin1Contour, 0., kper_max, 0., 2*TMath::Pi(), 0., TMath::Pi(), 4, "TBag", "RecalCombLin1Contour");
    // a1: TF3 fonction de kper, phi, et theta
    // parametres de a1: xi, t, eta, et R

    a1->SetParameters(f2Xi.Re(), ft.Re(), fEta.Re(), fR.Re());
    a1->SetNpy(200);

    Double_t va1 = a1->Integral(0., kper_max, 0., TMath::Pi(), 0., TMath::Pi(), 0.0001);
    //integrale de RecalCombLin1Contour pour kper de 0 a kper_max, phi de 0 a Pi (*2), theta de 0 a Pi

    RecalInteg1a = 2 * fZ2 * va1;
    delete a1;

} // end def TBag::Cal_RecalInteg1a



/*----------------------- void TBag::Cal_RecalInteg1b () ---------------------*/

/*!
 * \fn void TBag::Cal_RecalInteg1b ()
 * calcule l'integrale de RecalCombLin1axeR pour kper de 0 a kper_max, phi de 0 a Pi (*2), x de 0 a 1
 *
 */
void TBag::Cal_RecalInteg1b ()
{

    TF3 *b1 = new TF3 ("b1", this, &TBag::RecalCombLin1axeR, 0., kper_max, 0., 2.*TMath::Pi(), 0., 1., 4, "TBag", "RecalCombLin1axeR");
    // b1: TF3 fonction de kper, phi, et x
    // parametres de b1: xi, t, eta, et R

    b1-> SetParameters(f2Xi.Re(), ft.Re(), fEta.Re(), fR.Re());
    b1->SetNpy(500);
    Double_t vb1 = b1->Integral(0, kper_max, 0, TMath::Pi(), 0, 1., 0.0001);
    //integrale de RecalCombLin1Contour pour kper de 0 a kper_max, phi de 0 a Pi (*2), x de 0 a 1

    RecalInteg1b = 2 * fZ2 * vb1;
    delete b1;

} // end def TBag::Cal_RecalInteg1b



/*-------- Double_t TBag::IminterInteg1( Double_t *var, Double_t *par) -------*/

/*!
 * \fn Double_t TBag::IminterInteg1( Double_t *var, Double_t *par)
 * retourne ImcalCombLin1axeR(kper,phi,x=xi) * fZ2
 *
 * \param var = [ kper, phi, x]
 * \param par = [ xi ]
 *
 * \return ImcalCombLin1axeR(kper,phi,x=xi) * fZ2
 */
Double_t TBag::IminterInteg1( Double_t *var, Double_t *par)
{

    TF3 *c1 = new TF3 ("c1", this, &TBag::ImcalCombLin1axeR, 0, kper_max, 0, 2*TMath::Pi(), 0., 1., 4, "TBag", "ImcalCombLin1axeR");
    // c1: TF3 fonction de kper, phi, et x
    // parametres de c1: xi, t, eta, et R

    c1->SetParameters(f2Xi.Re(), ft.Re(), fEta.Re(), fR.Re());
    Double_t vc1 = c1->Eval(var[0], var[1], par[0]/2.);
    delete c1;
    return fZ2 * vc1;

} // end def TBag::IminterInteg1



/*------------------------ void TBag::Cal_ImcalInteg1 () ---------------------*/

/*!
 * \fn void TBag::Cal_ImcalInteg1 ()
 * calcule l'integrale de IminterInteg1 pour kper de 0 a kper_max, phi de 0 a Pi (*2)
 *
 */
void TBag::Cal_ImcalInteg1 ()
{

    TF2 *d1 = new TF2 ("d1", this, &TBag::IminterInteg1, 0., kper_max, 0., 2.* TMath::Pi(), 1, "TBag", "IminterInteg1");
    // d1: TF2 fonction de kper, phi
    // parametres de d1: xi

    d1->SetParameter(0,f2Xi.Re());
    d1->SetNpy(400);
    Double_t vd1 = d1->Integral(0., kper_max, 0., TMath::Pi(), 0.0001);
    //integrale de IminterInteg1 pour kper de 0 a kper_max, phi de 0 a Pi (*2)

    ImcalInteg1 = 2 * TMath::Pi() * vd1;
    // attention le fZ2 est deja dans IminterInteg1
    // le pi est la pour faire pi * (F(xi,xi) - F(-xi,xi))
    delete d1;

} // end def TBag::Cal_ImcalInteg1 ()



/*------ Double_t TBag::functionintegFF1( Double_t *var, Double_t *par) ------*/

/*!
 * \fn Double_t TBag::functionintegFF1( Double_t *var, Double_t *par)
 *
 * Fait le lien avec CombLin1 (qui prend des arguments complexes et rend une valeur complexe)
 * Rend la partie reelle de CombLin1
 *
 * \param var = [kper, theta, x]
 * \param par = [xi, t, eta, R]
 *
 * \return Re( CombLin1(kper, phi, x, xi, t, eta, R) )
 */
Double_t TBag::functionintegFF1( Double_t *var, Double_t *par)
{

    return ( CombLin1( TComplex (var[0],0), TComplex (var[1],0), TComplex (var[2],0), TComplex (par[0],0), TComplex (par[1],0), TComplex (par[2],0), TComplex (par[3],0))).Re();

} // end def TBag::functionintegFF1



/*------------------------ void TBag::Cal_integFF1 () ------------------------*/

/*!
 * \fn void TBag::Cal_integFF1 ()
 *
 * \f[ \int _{-1} ^1 dx \int_0 ^{k \perp _{max}} dk\perp \int _0 ^{2 \pi} d \theta CombLin1(k \perp, \theta, x) \f]
 *
 * calcule l'integrale de functionintegFF1 pour kper de 0 a kper_max, phi de 0 a Pi (*2), x de -1 a 1
 */
void TBag::Cal_integFF1 ()
{

    TF3 *d1bis = new TF3 ("d1bis", this, &TBag::functionintegFF1, 0., kper_max, 0., 2.* TMath::Pi(), -1, 1, 4, "TBag", "functionintegFF1");
    // d1bis : fonction de kper, phi, x
    // parametres: xi, t, eta, R

    d1bis->SetParameters(f2Xi.Re(), ft.Re(), fEta.Re(), fR.Re());
    d1bis->SetNpy(400);

    Double_t vd1bis = d1bis->Integral(0., kper_max, 0., TMath::Pi(), -1, 1, 0.0001);
    //integrale de functionintegFF1 pour kper de 0 a kper_max, phi de 0 a Pi (*2), x de -1 a 1

    integFF1 = 2 * fZ2 * vd1bis;
    delete d1bis;

} // end def TBag::Cal_integFF1



/*------- TComplex TBag::CombLin2 (TComplex kper, TComplex phi, TComplex x, TComplex xi, TComplex t, TComplex eta, TComplex R) ------*/

/*!
 * \fn TComplex TBag::CombLin2( TComplex kper, TComplex phi, TComplex x, TComplex xi, TComplex t, TComplex eta, TComplex R)
 * terme a integrer de l'equation 29
 *
 * \param kper
 * \param phi
 * \param x
 * \param xi
 * \param t
 * \param eta
 * \param R
 *
 * \return terme a integrer de l'equation 29
 */
TComplex TBag::CombLin2 (TComplex kper, TComplex phi, TComplex x, TComplex xi, TComplex t, TComplex eta, TComplex R)
{

    if(!kin_init)
    {
        cout << "TBag : Pas de configuration cinematique " << endl;
        exit(-1);
    } // end if kin_init

    TComplex kx = kper * TComplex::Cos(phi);    // Formules de changement de base ( coordonnees cylindriques )
    TComplex ky = kper * TComplex::Sin(phi);
    TComplex kz = ComposanteComplex_kz (x, xi, t, eta, fDelta, kper, phi);

    TComplex k[3] = {kx, ky, kz};

    TComplex kprime[3];
    for(Int_t i=0; i<3;i++)
    {
        kprime[i] = k[i] + eta * (kM/fMBar) * fDelta[i];
    } // end for i

    TComplex kCrossdelta[3] = { k[1]*fDelta[2] - k[2]*fDelta[1], k[2]*fDelta[0] - k[0]*fDelta[2], k[0]*fDelta[1] - k[1]*fDelta[0] };


    TComplex norme_k = NormComplex(k);                      // norme de k
    TComplex norme_kprime = NormComplex(kprime);                    // norme de k'

    // TComplex kJ0_Omega0 = J0 ( kOmega0 );                        // Calcul intermediaire
    TComplex N2 = kOmega0 / ( TComplex(2.,0.) * TComplex::Power(R,3.) * (kOmega0 - un) * TComplex::Power(kJ0_Omega0,2.));   // Jeu de notations
    TComplex R6 = TComplex::Power(R,6.);
    TComplex factor = fMBar / ( un - (fMBar_over_kM -un)* xi * xi * fMBar * fMBar / t );

    TComplex factor_2 = (2. * xi * fMBar * kCrossdelta[1] * fSinh2 / (fDelta[0] * t));
    TComplex factor_3 = ( (eta * kM * kCrossdelta[1] / (fMBar * fDelta[0])) - ( PSComplex(k,kprime) - 2. * k[1] * k[1] ) / 2. / kM) *T1(norme_k,R)* T1(norme_kprime,R) / norme_k/ norme_kprime;

    return TComplex(8.,0.) * kPi * N2 * R6 * kM * factor * (kper * ( T0(norme_k,R) * T0(norme_kprime,R)/ 2./ kM + ( kprime[0] * fMBar / ( kM * fDelta[0] ) + factor_2 ) * T0(norme_k,R) * T1(norme_kprime,R) / norme_kprime - ( k[0] * fMBar / ( kM * fDelta[0] ) + factor_2 ) * T1(norme_k,R) * T0(norme_kprime,R) / norme_k + factor_3 ) ) / TComplex::Power(2.*kPi,3.) ;

} // end def TBag::CombLin2



/*----------- Double_t TBag::Integ2( Double_t *var, Double_t *par) -----------*/

/*!
 * \fn Double_t TBag::Integ2( Double_t *var, Double_t *par)
 * Extraction de la partie reelle de TBag::CombLin2
 *
 * \param var = [kper, phi]
 * \param par = [xi, t, eta, R, x]
 *
 * \return Re ( CombLin2 (kper,phi, x, xi, t, eta, R) )
 */
Double_t TBag::Integ2( Double_t *var, Double_t *par)
{

    return (CombLin2( TComplex (var[0],0), TComplex (var[1],0), TComplex (par[4],0) , TComplex (par[0],0), TComplex (par[1],0), TComplex (par[2],0), TComplex (par[3],0) )).Re();

} // end def TBag::Integ2



/*-------------------------- void TBag::Cal_Integ2 () ------------------------*/

/*!
 * \fn void TBag::Cal_Integ2()
 *
 * Fait le calcul de l'integrale de la fonction TBag::Integ2
 * kper in [0,5], phi in [0,pi]
 * Multiplie par 2 le resultat pour faire l'integrale de 0 a 2pi sur phi (symetrie p/r a pi)
 *
 * H + a E = Integ1
 * H + E = Integ2
 *
 */
void TBag::Cal_Integ2 ()
{

    TF2 *i2 = new TF2("i2", this, &TBag::Integ2, 0, kper_max, 0, TMath::Pi(), 5, "TBag", "Integ2");
    //i2 : fonction de kper et phi, avec comme parametres [xi, t, eta, R, x]

    i2->SetParameters(f2Xi.Re(), ft.Re(), fEta.Re(), fR.Re(), fX.Re());
    i2->SetNpy(400);

    ValInteg2 = i2->Integral(0, kper_max, 0, TMath::Pi(), 0.0001)*2*fZ2;
    // Integrale sur kper de 0 a kper_max et sur phi de 0 a 2*Pi (en faisant 2* Integrale de 0 a Pi)

    delete i2;

} // end def TBag::Cal_Integ2



/*----- TComplex TBag::CombLin2Contourplus (Double_t *var, Double_t *par) ----*/

/*!
 * \fn TComplex TBag::CombLin2Contourplus (Double_t *var, Double_t *par)
 * expression et integration de la fonction CombLin2 sur le demi-cercle \f$ \Gamma \f$ de rayon 1/2 et de centre (1/2,0)
 * Membre de droite de l'equation 29 sur gamma pour x = 0.5 + 0.5 exp(i*theta)
 *
 * z = 0.5 + 0.5 exp(i*theta)
 *
 * \param var = [ kper, phi,theta ]
 * \param par = [ xi, t, eta, R ]
 *
 * \return CombLin2(kper, phi, z, xi, t, eta, R)
 */
TComplex TBag::CombLin2Contourplus (Double_t *var, Double_t *par)
{

    TComplex x = TComplex (0.5,0) + TComplex(0.5,0) * TComplex::Exp(I * TComplex(var[2],0));

    return CombLin2( TComplex (var[0],0), TComplex (var[1],0), x , TComplex (par[0],0), TComplex (par[1],0), TComplex (par[2],0), TComplex (par[3],0) );

} // end def TBag::CombLin2Contourplus



/*---- TComplex TBag::CombLin2Contourmoins (Double_t *var, Double_t *par) ----*/

/*!
 * \fn TComplex TBag::CombLin2Contourmoins (Double_t *var, Double_t *par)
 * expression et integration de la fonction CombLin2 sur le demi-cercle \f$ \Gamma \f$ de rayon 1/2 et de centre (-1/2,0)
 * Membre de droite de l'equation 29 sur gamma pour x = -0.5 - 0.5 exp(i*theta)
 *
 * z = -0.5 - 0.5 exp(i*theta)
 *
 * \param var = [ kper, phi,theta ]
 * \param par = [ xi, t, eta, R ]
 *
 * \return CombLin2(kper, phi, z, xi, t, eta, R)
 */
TComplex TBag::CombLin2Contourmoins (Double_t *var, Double_t *par)
{
    TComplex x = -TComplex (0.5,0) - TComplex(0.5,0) * TComplex::Exp(I * TComplex(var[2],0));
    return CombLin2( TComplex (var[0],0), TComplex (var[1],0), x , TComplex (par[0],0), TComplex (par[1],0), TComplex (par[2],0), TComplex (par[3],0) );

} // end def TBag::CombLin2Contourmoins



/*--------- TComplex TBag::CombLin2plus(Double_t *var, Double_t *par) --------*/

/*!
 * \fn TComplex TBag::CombLin2plus(Double_t *var, Double_t *par)
 *
 * expression de CombLin2plus sur le contour gamma (demi-cercle autour de (-1/2,0) et demi-cercle autour de (1/2,0),
 *  i.e (CombLin2Contourplus - CombLin2Contourmoins) / (z - xi)
 *
 * \param var = [ kper, phi,theta ]
 * \param par = [ xi, t, eta, R ]
 *
 * \return (CombLin2Contourplus - CombLin2Contourmoins) / (z - xi)
 *
 */
TComplex TBag::CombLin2plus(Double_t *var, Double_t *par)
{

    return (CombLin2Contourplus (var, par) - CombLin2Contourmoins (var, par)) / (TComplex(0.5,0) + TComplex(0.5,0)*TComplex::Exp(I * TComplex(var[2],0)) - fXi.Re());

} // end def TBag::CombLin2plus



/*----- Double_t TBag::RecalCombLin2Contour (Double_t *var, Double_t *par) ---*/

/*!
 * \fn Double_t TBag::RecalCombLin2Contour (Double_t *var, Double_t *par)
 *
 * cf formule 62 de Guillaume
 * Fonction necessaire au calcul de Re(calInteg2)
 *
 * \param var = [ kper, phi,theta ]
 * \param par = [ xi, t, eta, R ]
 *
 * \return - 0.5 * (Re(CombLin2plus) * Sin(theta) + Im(CombLin2plus) * Cos(theta));
 */
Double_t TBag::RecalCombLin2Contour (Double_t *var, Double_t *par)
{

    TComplex intermediaire = CombLin2plus (var, par);

    return - 0.5 * (intermediaire.Re() * TMath::Sin(var[2]) + intermediaire.Im() * TMath::Cos(var[2]));

} // end def TBag::RecalCombLin2Contour



/*------ Double_t TBag::RecalCombLin2axeR (Double_t *var, Double_t *par) -----*/

/*!
 * \fn Double_t TBag::RecalCombLin2axeR (Double_t *var, Double_t *par)
 *
 * Fonction donnant Re(CombLin2(x) - CombLin2(-x)) / (x+xi) sur l'axe [0;1]
 *
 * \param var = [ kper, phi,x ]
 * \param par = [ xi, t, eta, R ]
 *
 * \return Re(CombLin2(x) - CombLin2(-x)) / (x+xi)
 */
Double_t TBag::RecalCombLin2axeR (Double_t *var, Double_t *par)
{

    return (CombLin2( TComplex (var[0],0), TComplex (var[1],0), TComplex (var[2],0), TComplex (par[0],0), TComplex (par[1],0), TComplex (par[2],0), TComplex (par[3],0) )
            - (CombLin2( TComplex (var[0],0), TComplex (var[1],0), TComplex (- var[2],0), TComplex (par[0],0), TComplex (par[1],0), TComplex (par[2],0), TComplex (par[3],0)))).Re() / (var[2] + fXi.Re() );

} // end def TBag::RecalCombLin2axeR



/*------ Double_t TBag::ImcalCombLin2axeR (Double_t *var, Double_t *par) -----*/

/*!
 * \fn Double_t TBag::ImcalCombLin2axeR (Double_t *var, Double_t *par)
 * Fonction donnant CombLin2(x) - CombLin2(-x)
 * en l'evaluant en x = xi, on pourra acceder aux parties imaginaires des CFF
 *
 * \param var = [ kper, phi,x ]
 * \param par = [ xi, t, eta, R ]
 *
 * \return Re(CombLin2(x) - CombLin2(-x))
 */
Double_t TBag::ImcalCombLin2axeR (Double_t *var, Double_t *par)
{

    return ( CombLin2( TComplex (var[0],0), TComplex (var[1],0), TComplex (var[2],0), TComplex (par[0],0), TComplex (par[1],0), TComplex (par[2],0), TComplex (par[3],0)) - CombLin2( TComplex (var[0],0), TComplex (var[1],0), TComplex (- var[2],0), TComplex (par[0],0), TComplex (par[1],0), TComplex (par[2],0), TComplex (par[3],0))).Re();

} // end def TBag::ImcalCombLin2axeR



/*---------------------- void TBag::Cal_RecalInteg2a () ----------------------*/

/*!
 * \fn void TBag::Cal_RecalInteg2a ()
 * calcule l'integrale de RecalCombLin2Contour pour kper de 0 a kper_max, phi de 0 a Pi (*2), theta de 0 a Pi
 * initialise RecalInteg2a
 *
 */
void TBag::Cal_RecalInteg2a ()
{

    TF3 *a2 = new TF3("a2", this, &TBag:: RecalCombLin2Contour, 0., kper_max, 0., 2*TMath::Pi(), 0., TMath::Pi(), 4, "TBag", "RecalCombLin2Contour");
    // a2: TF3 fonction de kper, phi, et theta
    // parametres de a2: xi, t, eta, et R

    a2->SetParameters(f2Xi.Re(), ft.Re(), fEta.Re(), fR.Re());
    a2->SetNpy(300);
    Double_t va2 = a2->Integral(0., kper_max, 0., TMath::Pi(), 0., TMath::Pi(), 0.0001);
    //integrale de RecalCombLin2Contour pour kper de 0 a kper_max, phi de 0 a Pi (*2), theta de 0 a Pi


    RecalInteg2a = 2 * fZ2 * va2;
    delete a2;

} // end def TBag::Cal_RecalInteg2a



/*---------------------- void TBag::Cal_RecalInteg2b () ----------------------*/

/*!
 * \fn void TBag::Cal_RecalInteg2b ()
 * calcule l'integrale de RecalCombLin2axeR pour kper de 0 a kper_max, phi de 0 a Pi (*2), x de 0 a 1
 *
 */
void TBag::Cal_RecalInteg2b ()
{

    TF3 *b2 = new TF3 ("b2", this, &TBag::RecalCombLin2axeR, 0., kper_max, 0., 2.*TMath::Pi(), 0., 1., 4, "TBag", "RecalCombLin2axeR");
    // b2: TF3 fonction de kper, phi, et x
    // parametres de b2: xi, t, eta, et R

    b2-> SetParameters(f2Xi.Re(), ft.Re(), fEta.Re(), fR.Re());
    b2->SetNpy(300);
    Double_t vb2 = b2->Integral(0, kper_max, 0, TMath::Pi(), 0, 1., 0.0001);
    //integrale de RecalCombLin2Contour pour kper de 0 a kper_max, phi de 0 a Pi (*2), x de 0 a 1


    RecalInteg2b = 2 * fZ2 * vb2;
    delete b2;

} // end def TBag::Cal_RecalInteg2b



/*-------- Double_t TBag::IminterInteg2( Double_t *var, Double_t *par) -------*/

/*!
 * \fn Double_t TBag::IminterInteg2( Double_t *var, Double_t *par)
 * retourne ImcalCombLin2axeR(xi,xi) * fZ2
 *
 * \param var = [ kper, phi, x]
 * \param par = [ xi ]
 *
 * \return ImcalCombLin2axeR(xi,xi) * fZ2
 */
Double_t TBag::IminterInteg2( Double_t *var, Double_t *par)
{

    TF3 *c2 = new TF3 ("c2", this, &TBag::ImcalCombLin2axeR, 0, kper_max, 0, 2*TMath::Pi(), 0., 1., 4, "TBag", "ImcalCombLin2axeR");
    c2->SetParameters(f2Xi.Re(), ft.Re(), fEta.Re(), fR.Re());
    //g2c->SetNpy(500);
    Double_t vc2 = c2->Eval(var[0], var[1], par[0]/2.);
    delete c2;

    return fZ2 * vc2;

} // end def TBag::IminterInteg2



/*---------------------- void TBag::Cal_ImcalInteg2 () -----------------------*/

/*!
 * \fn void TBag::Cal_ImcalInteg2 ()
 * calcule l'integrale de IminterInteg2 pour kper de 0 a kper_max, phi de 0 a Pi (*2)
 *
 */
void TBag::Cal_ImcalInteg2 ()
{

    TF2 *d2 = new TF2 ("d2", this, &TBag::IminterInteg2, 0., kper_max, 0., 2.* TMath::Pi(), 1, "TBag", "IminterInteg2");
    // d2: TF2 fonction de kper, phi
    // parametres de d2: xi

    d2->SetParameter(0,f2Xi.Re());
    d2->SetNpy(400);

    //integrale de IminterInteg2 pour kper de 0 a kper_max, phi de 0 a Pi (*2)
    Double_t vd2 = d2->Integral(0., kper_max, 0., TMath::Pi(), 0.0001);

    ImcalInteg2 = 2 * TMath::Pi() * vd2;
    // attention le fZ2 est deja dans IminterInteg2
    // le pi est la pour faire pi * (F(xi,xi) - F(-xi,xi))
    delete d2;

} // end def TBag::Cal_ImcalInteg2



/*------- Double_t TBag::functionintegFF2( Double_t *var, Double_t *par) -----*/

/*!
 * \fn Double_t TBag::functionintegFF2( Double_t *var, Double_t *par)
 *
 * Fait le lien avec CombLin2 (qui prend des arguments complexes et rend une valeur complexe)
 * Rend la partie reelle de CombLin2
 *
 * \param var = [kper, theta, x]
 * \param par = [xi, t, eta, R]
 *
 * \return Re( CombLin2(kper, phi, x, xi, t, eta, R) )
 */
Double_t TBag::functionintegFF2( Double_t *var, Double_t *par)
{

    return ( CombLin2( TComplex (var[0],0), TComplex (var[1],0), TComplex (var[2],0), TComplex (par[0],0), TComplex (par[1],0), TComplex (par[2],0), TComplex (par[3],0))).Re();

} // end def TBag::functionintegFF2



/*----------------------- void TBag::Cal_integFF2 () -------------------------*/

/*!
 * \fn void TBag::Cal_integFF2 ()
 *
 * \f[ \int _{-1} ^1 dx \int_0 ^{k \perp _{max}} dk\perp \int _0 ^{2 \pi} d \theta CombLin2(k \perp, \theta, x) \f]
 *
 * calcule l'integrale de functionintegFF2 pour kper de 0 a kper_max, phi de 0 a Pi (*2), x de -1 a 1
 */
void TBag::Cal_integFF2 ()
{

    TF3 *d2bis = new TF3 ("d2bis", this, &TBag::functionintegFF2, 0., kper_max, 0., 2.* TMath::Pi(), -1, 1, 4, "TBag", "functionintegFF2");
    // d2bis : fonction de kper, phi, x
    // parametres: xi, t, eta, R

    d2bis->SetParameters(f2Xi.Re(), ft.Re(), fEta.Re(), fR.Re());
    d2bis->SetNpy(400);

    Double_t vd2bis = d2bis->Integral(0., kper_max, 0., TMath::Pi(), -1, 1, 0.0001);
    //integrale de functionintegFF2 pour kper de 0 a kper_max, phi de 0 a Pi (*2), x de -1 a 1


    integFF2 = 2 * fZ2 * vd2bis;
    delete d2bis;

} // end def TBag::Cal_integFF2



/*------- TComplex TBag::CombLin3 (TComplex kper, TComplex phi, TComplex x, TComplex xi, TComplex t, TComplex eta, TComplex R) ------*/

/*!
 * \fn TComplex TBag::CombLin3( TComplex kper, TComplex phi, TComplex x, TComplex xi, TComplex t, TComplex eta, TComplex R)
 * terme a integrer de l'equation 30
 *
 * \param kper
 * \param phi
 * \param x
 * \param xi
 * \param t
 * \param eta
 * \param R
 *
 * \return terme a integrer de l'equation 30
 */
TComplex TBag::CombLin3 (TComplex kper, TComplex phi, TComplex x, TComplex xi, TComplex t, TComplex eta, TComplex R)
{

    if(!kin_init)
    {
        cout << "TBag : Pas de configuration cinematique " << endl;
        exit(-1);
    } // end if kin_init

    TComplex kx = kper * TComplex::Cos(phi);    // Formules de changement de base ( coordonnees cylindriques )
    TComplex ky = kper * TComplex::Sin(phi);
    TComplex kz = ComposanteComplex_kz (x, xi, t, eta, fDelta, kper,phi);

    TComplex k[3] = {kx, ky, kz};

    TComplex kprime[3];
    for(Int_t i=0; i<3;i++)
    {
        kprime[i] = k[i] + eta * (kM/fMBar) * fDelta[i];
    } // end for i

    TComplex norme_k = NormComplex(k);                      // norme de k
    TComplex norme_kprime = NormComplex(kprime);            // norme de k'

    TComplex N2 = kOmega0 / ( TComplex(2.,0.) * TComplex::Power(R,3) * ( kOmega0 - un) * TComplex::Power(kJ0_Omega0,2) );
    TComplex R6 = TComplex::Power(R,6.);
    TComplex factor = fMBar / ( un - ( fMBar_over_kM -un ) * xi * xi * fMBar * fMBar / t );

    return  TComplex(8.,0.) * R6 * N2 * kPi * factor * kM/fMBar * kper * ( ( fMBar_over_kM + 2.*TComplex::Power(fDelta[2],2.)*fSinh2/t ) * T0(norme_k,R) * T0(norme_kprime,R) + FuncIntermed4(k, kprime, fDelta, R) + FuncIntermed3(xi, t, k, kprime, fDelta, R) ) / TComplex::Power((2.*kPi),3.);

} // end def TBag::CombLin3



/*----------- Double_t TBag::Integ3 ( Double_t *var, Double_t *par) ----------*/

/*!
 * \fn Double_t TBag::Integ3( Double_t *var, Double_t *par)
 * Extraction de la partie reelle de TBag::CombLin3
 *
 * \param var = [kper, phi]
 * \param par = [xi, t, eta, R, x]
 *
 * \return Re ( CombLin3 (kper,phi, x, xi, t, eta, R) )
 */
Double_t TBag::Integ3 ( Double_t *var, Double_t *par)
{

    return (CombLin3( TComplex (var[0],0), TComplex (var[1],0), TComplex (par[4],0) , TComplex (par[0],0), TComplex (par[1],0), TComplex (par[2],0), TComplex (par[3],0) )).Re();

} // end def TBag::Integ3



/*------------------------- void TBag::Cal_Integ3 ()  ------------------------*/

/*!
 * \fn void TBag::Cal_Integ3()
 *
 * Fait le calcul de l'integrale de la fonction TBag::Integ3
 * kper in [0,5], phi in [0,pi]
 * Multiplie par 2 le resultat pour faire l'integrale de 0 a 2pi sur phi (symetrie p/r a pi)
 *
 * b Htilde + c Etilde = Integ3
 * d Htilde + e Etilde = Integ2
 *
 */
void TBag::Cal_Integ3 ()
{

    TF2 *i3 = new TF2("i3", this, &TBag::Integ3, 0, kper_max, 0, TMath::Pi(), 5, "TBag", "Integ3");
    //i3 : fonction de kper et phi, avec comme parametres [xi, t, eta, R, x]

    i3->SetParameters(f2Xi.Re(), ft.Re(), fEta.Re(), fR.Re(), fX.Re());
    i3->SetNpy(400);

    ValInteg3 = i3->Integral(0, kper_max, 0, TMath::Pi(), 0.0001)*2*fZ2;
    // Integrale sur kper de 0 a kper_max et sur phi de 0 a 2*Pi (en faisant 2* Integrale de 0 a Pi)

    delete i3;

} // end def TBag::Cal_Integ3



/*----- TComplex TBag::CombLin3Contourplus (Double_t *var, Double_t *par) ----*/

/*!
 * \fn TComplex TBag::CombLin3Contourplus (Double_t *var, Double_t *par)
 * expression et integration de la fonction CombLin3 sur le demi-cercle \f$ \Gamma \f$ de rayon 1/2 et de centre (1/2,0)
 * Membre de droite de l'equation 30 sur gamma pour x = 0.5 + 0.5 exp(i*theta)
 *
 * z = 0.5 + 0.5 exp(i*theta)
 *
 * \param var = [ kper, phi,theta ]
 * \param par = [ xi, t, eta, R ]
 *
 * \return CombLin3(kper, phi, z, xi, t, eta, R)
 */
TComplex TBag::CombLin3Contourplus (Double_t *var, Double_t *par)
{

    TComplex x = TComplex (0.5,0) + TComplex(0.5,0) * TComplex::Exp(I * TComplex(var[2],0));

    return CombLin3( TComplex (var[0],0), TComplex (var[1],0), x , TComplex (par[0],0), TComplex (par[1],0), TComplex (par[2],0), TComplex (par[3],0) );

} // end def TBag::CombLin3Contourplus



/*----- TComplex TBag::CombLin3Contourmoins (Double_t *var, Double_t *par) ---*/

/*!
 * \fn TComplex TBag::CombLin3Contourmoins (Double_t *var, Double_t *par)
 * expression et integration de la fonction CombLin3 sur le demi-cercle \f$ \Gamma \f$ de rayon 1/2 et de centre (-1/2,0)
 * Membre de droite de l'equation 30 sur gamma pour x = -0.5 - 0.5 exp(i*theta)
 *
 * z = -0.5 - 0.5 exp(i*theta)
 *
 * \param var = [ kper, phi,theta ]
 * \param par = [ xi, t, eta, R ]
 *
 * \return CombLin3(kper, phi, z, xi, t, eta, R)
 */
TComplex TBag::CombLin3Contourmoins (Double_t *var, Double_t *par)
{

    TComplex x = -TComplex (0.5,0) - TComplex(0.5,0) * TComplex::Exp(I * TComplex(var[2],0));
    return CombLin3( TComplex (var[0],0), TComplex (var[1],0), x , TComplex (par[0],0), TComplex (par[1],0), TComplex (par[2],0), TComplex (par[3],0) );

} // end def TBag::CombLin3Contourmoins



/*--------- TComplex TBag::CombLin3plus(Double_t *var, Double_t *par) --------*/

/*!
 * \fn TComplex TBag::CombLin3plus(Double_t *var, Double_t *par)
 *
 * expression de CombLin3plus sur le contour gamma (demi-cercle autour de (-1/2,0) et demi-cercle autour de (1/2,0),
 *  i.e (CombLin3Contourplus + CombLin3Contourmoins) / (z - xi)
 *
 * \param var = [ kper, phi,theta ]
 * \param par = [ xi, t, eta, R ]
 *
 * \return (CombLin3Contourplus + CombLin3Contourmoins) / (z - xi)
 */
TComplex TBag::CombLin3plus(Double_t *var, Double_t *par)
{

    return (CombLin3Contourplus (var, par) + CombLin3Contourmoins (var, par)) / (TComplex(0.5,0) + TComplex(0.5,0)*TComplex::Exp(I * TComplex(var[2],0)) - fXi.Re());

} // end def TBag::CombLin3plus



/*---- Double_t TBag::RecalCombLin3Contour (Double_t *var, Double_t *par) ----*/

/*!
 * \fn Double_t TBag::RecalCombLin3Contour (Double_t *var, Double_t *par)
 *
 * cf formule 62 de Guillaume
 * Fonction necessaire au calcul de Re(calInteg3)
 *
 * \param var = [ kper, phi,theta ]
 * \param par = [ xi, t, eta, R ]
 *
 * \return - 0.5 * (Re(CombLin3plus) * Sin(theta) + Im(CombLin3plus) * Cos(theta));
 */
Double_t TBag::RecalCombLin3Contour (Double_t *var, Double_t *par)
{

    TComplex intermediaire = CombLin3plus (var, par);

    return - 0.5 * (intermediaire.Re() * TMath::Sin(var[2]) + intermediaire.Im() * TMath::Cos(var[2]));

} // end def TBag::RecalCombLin3Contour



/*------ Double_t TBag::RecalCombLin3axeR (Double_t *var, Double_t *par) -----*/

/*!
 * \fn Double_t TBag::RecalCombLin3axeR (Double_t *var, Double_t *par)
 *
 * Fonction donnant Re(CombLin3(x) - CombLin3(-x)) / (x+xi) sur l'axe [0;1]
 *
 * \param var = [ kper, phi,x ]
 * \param par = [ xi, t, eta, R ]
 *
 * \return Re(CombLin3(x) - CombLin3(-x)) / (x+xi)
 */
Double_t TBag::RecalCombLin3axeR (Double_t *var, Double_t *par)
{

    return (CombLin3( TComplex (var[0],0), TComplex (var[1],0), TComplex (var[2],0), TComplex (par[0],0), TComplex (par[1],0), TComplex (par[2],0), TComplex (par[3],0) ) + (CombLin3( TComplex (var[0],0), TComplex (var[1],0), TComplex (- var[2],0), TComplex (par[0],0), TComplex (par[1],0), TComplex (par[2],0), TComplex (par[3],0)))).Re() / (var[2] + fXi.Re() );

} // end def TBag::RecalCombLin3axeR



/*------ Double_t TBag::ImcalCombLin3axeR (Double_t *var, Double_t *par) -----*/

/*!
 * \fn Double_t TBag::ImcalCombLin3axeR (Double_t *var, Double_t *par)
 * Fonction donnant CombLin3(x) + CombLin3(-x)
 * en l'evaluant en x = xi, on pourra acceder aux parties imaginaires des CFF
 *
 * \param var = [ kper, phi,x ]
 * \param par = [ xi, t, eta, R ]
 *
 * \return Re(CombLin3(x) - CombLin3(-x))
 */
Double_t TBag::ImcalCombLin3axeR (Double_t *var, Double_t *par)
{

    return ( CombLin3( TComplex (var[0],0), TComplex (var[1],0), TComplex (var[2],0), TComplex (par[0],0), TComplex (par[1],0), TComplex (par[2],0), TComplex (par[3],0))
            + CombLin3( TComplex (var[0],0), TComplex (var[1],0), TComplex (- var[2],0), TComplex (par[0],0), TComplex (par[1],0), TComplex (par[2],0), TComplex (par[3],0))).Re();

} // end def TBag::ImcalCombLin3axeR



/*--------------------- void TBag::Cal_RecalInteg3a () -----------------------*/

/*!
 * \fn void TBag::Cal_RecalInteg3a ()
 * calcule l'integrale de RecalCombLin3Contour pour kper de 0 a kper_max, phi de 0 a Pi (*2), theta de 0 a Pi
 * initialise RecalInteg3a
 *
 *
 */
void TBag::Cal_RecalInteg3a ()
{

    TF3 *a3 = new TF3("a3", this, &TBag::RecalCombLin3Contour, 0., kper_max, 0., 2*TMath::Pi(), 0., TMath::Pi(), 4, "TBag", "RecalCombLin3Contour");
    // a3: TF3 fonction de kper, phi, et theta
    // parametres de a3: xi, t, eta, et R

    a3->SetParameters(f2Xi.Re(), ft.Re(), fEta.Re(), fR.Re());
    a3->SetNpy(200);
    Double_t va3 = a3->Integral(0., kper_max, 0., TMath::Pi(), 0., TMath::Pi(), 0.0001);
    //integrale de RecalCombLin3Contour pour kper de 0 a kper_max, phi de 0 a Pi (*2), theta de 0 a Pi

    RecalInteg3a = 2 * fZ2 * va3;
    delete a3;

} // end def TBag::Cal_RecalInteg3a



/*---------------------- void TBag::Cal_RecalInteg3b () ----------------------*/

/*!
 * \fn void TBag::Cal_RecalInteg3b ()
 * calcule l'integrale de RecalCombLin3axeR pour kper de 0 a kper_max, phi de 0 a Pi (*2), x de 0 a 1
 *
 */
void TBag::Cal_RecalInteg3b ()
{

    TF3 *b3 = new TF3 ("b3", this, &TBag::RecalCombLin3axeR, 0., kper_max, 0., 2.*TMath::Pi(), 0., 1., 4, "TBag", "RecalCombLin3axeR");
    // b3: TF3 fonction de kper, phi, et x
    // parametres de b3: xi, t, eta, et R

    b3-> SetParameters(f2Xi.Re(), ft.Re(), fEta.Re(), fR.Re());
    b3->SetNpy(500);
    Double_t vb3 = b3->Integral(0, kper_max, 0, TMath::Pi(), 0, 1., 0.0001);
    //integrale de RecalCombLin3Contour pour kper de 0 a kper_max, phi de 0 a Pi (*2), x de 0 a 1


    RecalInteg3b = 2 * fZ2 * vb3;
    delete b3;

} // end def TBag::Cal_RecalInteg3b



/*-------- Double_t TBag::IminterInteg3( Double_t *var, Double_t *par) -------*/

/*!
 * \fn Double_t TBag::IminterInteg3( Double_t *var, Double_t *par)
 * retourne ImcalCombLin3axeR(xi,xi) * fZ2
 *
 * \param var = [ kper, phi, x]
 * \param par = [ xi ]
 *
 * \return ImcalCombLin3axeR(xi,xi) * fZ2
 */
Double_t TBag::IminterInteg3( Double_t *var, Double_t *par)
{

    TF3 *c3 = new TF3 ("c3", this, &TBag::ImcalCombLin3axeR, 0, kper_max, 0, 2*TMath::Pi(), 0., 1., 4, "TBag", "ImcalCombLin3axeR");
    c3->SetParameters(f2Xi.Re(), ft.Re(), fEta.Re(), fR.Re());
    Double_t vc3 = c3->Eval(var[0], var[1], par[0]/3.);

    delete c3;
    return fZ2 * vc3;

} // end def TBag::IminterInteg3



/*--------------------- void TBag::Cal_ImcalInteg3 () ------------------------*/

/*!
 * \fn void TBag::Cal_ImcalInteg3 ()
 * calcule l'integrale de IminterInteg3 pour kper de 0 a kper_max, phi de 0 a Pi (*2)
 *
 */
void TBag::Cal_ImcalInteg3 ()
{

    TF2 *d3 = new TF2 ("d3", this, &TBag::IminterInteg3, 0., kper_max, 0., 2.* TMath::Pi(), 1, "TBag", "IminterInteg3");
    // d3: TF2 fonction de kper, phi
    // parametres de d3: x

    d3->SetParameter(0,f2Xi.Re());
    d3->SetNpy(400);
    Double_t vd3 = d3->Integral(0., kper_max, 0., TMath::Pi(), 0.0001);
    //integrale de IminterInteg3 pour kper de 0 a kper_max, phi de 0 a Pi (*2)


    ImcalInteg3 = 2 * TMath::Pi() * vd3;
    // attention le fZ2 est deja dans IminterInteg3
    // le pi est la pour faire pi * (F(xi,xi) - F(-xi,xi))
    delete d3;

} // end def TBag::Cal_ImcalInteg3



/*------ Double_t TBag::functionintegFF3( Double_t *var, Double_t *par) ------*/

/*!
 * \fn Double_t TBag::functionintegFF3( Double_t *var, Double_t *par)
 *
 * Fait le lien avec CombLin3 (qui prend des arguments complexes et rend une valeur complexe)
 * Rend la partie reelle de CombLin3
 *
 * \param var = [kper, theta, x]
 * \param par = [xi, t, eta, R]
 *
 * \return Re( CombLin3(kper, phi, x, xi, t, eta, R) )
 */
Double_t TBag::functionintegFF3( Double_t *var, Double_t *par)
{

    return ( CombLin3( TComplex (var[0],0), TComplex (var[1],0), TComplex (var[2],0), TComplex (par[0],0), TComplex (par[1],0), TComplex (par[2],0), TComplex (par[3],0))).Re();

} // end def TBag::functionintegFF3



/*------------------------ void TBag::Cal_integFF3 ---------------------------*/

/*!
 * \fn void TBag::Cal_integFF3 ()
 *
 * \f[ \int _{-1} ^1 dx \int_0 ^{k \perp _{max}} dk\perp \int _0 ^{2 \pi} d \theta CombLin3(k \perp, \theta, x) \f]
 *
 * calcule l'integrale de functionintegFF3 pour kper de 0 a kper_max, phi de 0 a Pi (*2), x de -1 a 1
 */
void TBag::Cal_integFF3 ()
{

    TF3 *d3bis = new TF3 ("d3bis", this, &TBag::functionintegFF3, 0., kper_max, 0., 2.* TMath::Pi(), -1, 1, 4, "TBag", "functionintegFF3");
    // d3bis : fonction de kper, phi, x
    // parametres: xi, t, eta, R

    d3bis->SetParameters(f2Xi.Re(), ft.Re(), fEta.Re(), fR.Re());
    d3bis->SetNpy(400);

    Double_t vd3bis = d3bis->Integral(0., kper_max, 0., TMath::Pi(), -1, 1, 0.0001);
    //integrale de functionintegFF3 pour kper de 0 a kper_max, phi de 0 a Pi (*2), x de -1 a 1


    integFF3 = 2 * fZ2 * vd3bis;
    delete d3bis;

} // end def TBag::Cal_integFF3



/*------- TComplex TBag::CombLin4 (TComplex kper, TComplex phi, TComplex x, TComplex xi, TComplex t, TComplex eta, TComplex R) ------*/

/*!
 * \fn TComplex TBag::CombLin4( TComplex kper, TComplex phi, TComplex x, TComplex xi, TComplex t, TComplex eta, TComplex R)
 * terme a integrer de l'equation 31
 *
 * \param kper
 * \param phi
 * \param x
 * \param xi
 * \param t
 * \param eta
 * \param R
 *
 * \return terme a integrer de l'equation 31
 */
TComplex TBag::CombLin4 (TComplex kper, TComplex phi, TComplex x, TComplex xi, TComplex t, TComplex eta, TComplex R)
{

    if(!kin_init)
    {
        cout << "TBag : Pas de configuration cinematique " << endl;
        exit(-1);
    } // end if kin_init

    TComplex kx = kper * TComplex::Cos(phi);    // Formules de changement de base ( coordonnees cylindriques )
    TComplex ky = kper * TComplex::Sin(phi);
    TComplex kz = ComposanteComplex_kz (x, xi, t, eta, fDelta, kper,phi);

    TComplex k[3] = {kx, ky, kz};

    TComplex kprime[3];
    for(Int_t i=0; i<3;i++) {
        kprime[i] = k[i] + eta * (kM/fMBar) * fDelta[i];
    } // end for i

    TComplex norme_k = NormComplex(k);                      // norme de k
    TComplex norme_kprime = NormComplex(kprime);            // norme de k'

    TComplex N2 = kOmega0 / ( TComplex(2.,0.) * TComplex::Power(R,3.) * (kOmega0 - un) * TComplex::Power(kJ0_Omega0,2));    // Jeu de notations
    TComplex R6 = TMath::Power(R,6.);
    TComplex factor = fMBar / ( un -( fMBar_over_kM - un )* xi * xi * fMBar * fMBar / t );

    return  8. * kPi * N2 * R6 * factor * kM / fMBar * kper * ( ( TComplex(-2.,0.) * fDelta[0] * xi * fMBar *fSinh2 / t) * T0(norme_k,R) * T0(norme_kprime,R) + FuncIntermed5 ( k, kprime ,fDelta, R) + FuncIntermed6( xi, t, k, kprime, fDelta, R) ) / TComplex::Power(2.*kPi, 3.);

} // end def TBag::CombLin4




/*----------- Double_t TBag::Integ4( Double_t *var, Double_t *par) -----------*/

/*!
 * \fn Double_t TBag::Integ4( Double_t *var, Double_t *par)
 * Extraction de la partie reelle de TBag::CombLin3
 *
 * \param var = [kper, phi]
 * \param par = [xi, t, eta, R, x]
 *
 * \return Re ( CombLin4 (kper,phi, x, xi, t, eta, R) )
 */
Double_t TBag::Integ4( Double_t *var, Double_t *par)
{

    return (CombLin4( TComplex (var[0],0), TComplex (var[1],0), TComplex (par[4],0) , TComplex (par[0],0), TComplex (par[1],0), TComplex (par[2],0), TComplex (par[3],0) )).Re();

} // end def TBag::Integ4



/*------------------------ void TBag::Cal_Integ4 () --------------------------*/

/*!
 * \fn void TBag::Cal_Integ4()
 *
 * Fait le calcul de l'integrale de la fonction TBag::Integ4
 * kper in [0,5], phi in [0,pi]
 * Multiplie par 2 le resultat pour faire l'integrale de 0 a 2pi sur phi (symetrie p/r a pi)
 *
 * b Htilde + c Etilde = Integ3
 * d Htilde + e Etilde = Integ2
 *
 */
void TBag::Cal_Integ4 ()
{

    TF2 *i4 = new TF2("i4", this, &TBag::Integ4, 0, kper_max, 0, TMath::Pi(), 5, "TBag", "Integ4");
    //i4 : fonction de kper et phi, avec comme parametres [xi, t, eta, R, x]

    i4->SetParameters(f2Xi.Re(), ft.Re(), fEta.Re(), fR.Re(), fX.Re());
    i4->SetNpy(400);

    ValInteg4 = i4->Integral(0, kper_max, 0, TMath::Pi(), 0.0001)*2*fZ2;
    // Integrale sur kper de 0 a kper_max et sur phi de 0 a 2*Pi (en faisant 2* Integrale de 0 a Pi)

    delete i4;

} // end def TBag::Cal_Integ4



/*----- TComplex TBag::CombLin4Contourplus (Double_t *var, Double_t *par) ----*/

/*!
 * \fn TComplex TBag::CombLin4Contourplus (Double_t *var, Double_t *par)
 * expression et integration de la fonction CombLin1 sur le demi-cercle \f$ \Gamma \f$ de rayon 1/2 et de centre (1/2,0) (membre de droite de l'eq. (31))
 *
 * z = 0.5 + 0.5 exp(i*theta)
 *
 * \param var = [ kper, phi,theta ]
 * \param par = [ xi, t, eta, R ]
 *
 * \return CombLin4(kper, phi, z, xi, t, eta, R)
 */
TComplex TBag::CombLin4Contourplus (Double_t *var, Double_t *par)
{

    TComplex x = TComplex (0.5,0) + TComplex(0.5,0) * TComplex::Exp(I * TComplex(var[2],0));

    return CombLin4( TComplex (var[0],0), TComplex (var[1],0), x , TComplex (par[0],0), TComplex (par[1],0), TComplex (par[2],0), TComplex (par[3],0) );

} // end def TBag::CombLin4Contourplus



/*---- TComplex TBag::CombLin4Contourmoins (Double_t *var, Double_t *par) ----*/

/*!
 * \fn TComplex TBag::CombLin4Contourmoins (Double_t *var, Double_t *par)
 * expression et integration de la fonction CombLin4 sur le demi-cercle \f$ \Gamma \f$ de rayon 1/2 et de centre (-1/2,0) (membre de droite de l'eq. (31))
 *
 * z = -0.5 - 0.5 exp(i*theta)
 *
 * \param var = [ kper, phi,theta ]
 * \param par = [ xi, t, eta, R ]
 *
 * \return CombLin4(kper, phi, z, xi, t, eta, R)
 */
TComplex TBag::CombLin4Contourmoins (Double_t *var, Double_t *par)
{

    TComplex x = -TComplex (0.5,0) - TComplex(0.5,0) * TComplex::Exp(I * TComplex(var[2],0));

    return CombLin4( TComplex (var[0],0), TComplex (var[1],0), x , TComplex (par[0],0), TComplex (par[1],0), TComplex (par[2],0), TComplex (par[3],0) );

} // end def TBag::CombLin4Contourmoins



/*--------- TComplex TBag::CombLin4plus(Double_t *var, Double_t *par) --------*/

/*!
 * \fn TComplex TBag::CombLin4plus(Double_t *var, Double_t *par)
 *
 * expression de CombLin4plus sur le contour gamma (demi-cercle autour de (-1/2,0) et demi-cercle autour de (1/2,0),
 *  i.e (CombLin4Contourplus + CombLin4Contourmoins) / (z - xi)
 *
 * \param var = [ kper, phi,theta ]
 * \param par = [ xi, t, eta, R ]
 *
 * \return (CombLin4Contourplus + CombLin4Contourmoins) / (z - xi)
 */
TComplex TBag::CombLin4plus(Double_t *var, Double_t *par)
{

    return (CombLin4Contourplus (var, par) + CombLin4Contourmoins (var, par)) / (TComplex(0.5,0) + TComplex(0.5,0)*TComplex::Exp(I * TComplex(var[2],0)) - fXi.Re());

} // end def TBag::CombLin4plus



/*----- Double_t TBag::RecalCombLin4Contour (Double_t *var, Double_t *par) ---*/

/*!
 * \fn Double_t TBag::RecalCombLin4Contour (Double_t *var, Double_t *par)
 *
 * cf formule 62 de Guillaume
 * Fonction necessaire au calcul de Re(calInteg4)
 *
 * \param var = [ kper, phi,theta ]
 * \param par = [ xi, t, eta, R ]
 *
 * \return - 0.5 * (Re(CombLin4plus) * Sin(theta) + Im(CombLin4plus) * Cos(theta));
 */
Double_t TBag::RecalCombLin4Contour (Double_t *var, Double_t *par)
{

    TComplex intermediaire = CombLin4plus (var, par);

    return - 0.5 * (intermediaire.Re() * TMath::Sin(var[2]) + intermediaire.Im() * TMath::Cos(var[2]));

} // end def TBag::RecalCombLin4Contour



/*------ Double_t TBag::RecalCombLin4axeR (Double_t *var, Double_t *par) -----*/

/*!
 * \fn Double_t TBag::RecalCombLin4axeR (Double_t *var, Double_t *par)
 *
 * Fonction donnant Re(CombLin4(x) - CombLin4(-x)) / (x+xi) sur l'axe [0;1]
 *
 * \param var = [ kper, phi,x ]
 * \param par = [ xi, t, eta, R ]
 *
 * \return Re(CombLin4(x) - CombLin4(-x)) / (x+xi)
 */
Double_t TBag::RecalCombLin4axeR (Double_t *var, Double_t *par)
{

    return (CombLin4( TComplex (var[0],0), TComplex (var[1],0), TComplex (var[2],0), TComplex (par[0],0), TComplex (par[1],0), TComplex (par[2],0), TComplex (par[3],0) )
            + (CombLin4( TComplex (var[0],0), TComplex (var[1],0), TComplex (- var[2],0), TComplex (par[0],0), TComplex (par[1],0), TComplex (par[2],0), TComplex (par[3],0)))).Re() / (var[2] + fXi.Re() );

} // end def TBag::RecalCombLin4axeR



/*----- Double_t TBag::ImcalCombLin4axeR (Double_t *var, Double_t *par) ------*/

/*!
 * \fn Double_t TBag::ImcalCombLin4axeR (Double_t *var, Double_t *par)
 * Fonction donnant CombLin4(x) - CombLin4(-x)
 * en l'evaluant en x = xi, on pourra acceder aux parties imaginaires des CFF
 *
 * \param var = [ kper, phi,x ]
 * \param par = [ xi, t, eta, R ]
 *
 * \return Re(CombLin4(x) - CombLin4(-x))
 */
Double_t TBag::ImcalCombLin4axeR (Double_t *var, Double_t *par)
{

    return ( CombLin4( TComplex (var[0],0), TComplex (var[1],0), TComplex (var[2],0), TComplex (par[0],0), TComplex (par[1],0), TComplex (par[2],0), TComplex (par[3],0)) + CombLin4( TComplex (var[0],0), TComplex (var[1],0), TComplex (- var[2],0), TComplex (par[0],0), TComplex (par[1],0), TComplex (par[2],0), TComplex (par[3],0))).Re();

} // end def TBag::ImcalCombLin4axeR



/*--------------------- void TBag::Cal_RecalInteg4a () -----------------------*/

/*!
 * \fn void TBag::Cal_RecalInteg4a ()
 * calcule l'integrale de RecalCombLin4Contour pour kper de 0 a kper_max, phi de 0 a Pi (*2), theta de 0 a Pi
 * initialise RecalInteg4a
 *
 */
void TBag::Cal_RecalInteg4a ()
{

    TF3 *a4 = new TF3("a4", this, &TBag::RecalCombLin4Contour, 0., kper_max, 0., 2*TMath::Pi(), 0., TMath::Pi(), 4, "TBag", "RecalCombLin4Contour");
    // a4: TF3 fonction de kper, phi, et theta
    // parametres de a4: xi, t, eta, et R

    a4->SetParameters(f2Xi.Re(), ft.Re(), fEta.Re(), fR.Re());
    a4->SetNpy(300);
    Double_t va4 = a4->Integral(0., kper_max, 0., TMath::Pi(), 0., TMath::Pi(), 0.0001);
    //integrale de RecalCombLin4Contour pour kper de 0 a kper_max, phi de 0 a Pi (*2), theta de 0 a Pi

    RecalInteg4a = 2 * fZ2 * va4;
    delete a4;

} // end def TBag::Cal_RecalInteg4a



/*--------------------- void TBag::Cal_RecalInteg4b () -----------------------*/

/*!
 * \fn void TBag::Cal_RecalInteg4b ()
 * calcule l'integrale de RecalCombLin4axeR pour kper de 0 a kper_max, phi de 0 a Pi (*2), x de 0 a 1
 *
 */
void TBag::Cal_RecalInteg4b ()
{

    TF3 *b4 = new TF3 ("b4", this, &TBag::RecalCombLin4axeR, 0., kper_max, 0., 2.*TMath::Pi(), 0., 1., 4, "TBag", "RecalCombLin4axeR");
    // b4: TF3 fonction de kper, phi, et x
    // parametres de b4: xi, t, eta, et R

    b4-> SetParameters(f2Xi.Re(), ft.Re(), fEta.Re(), fR.Re());
    b4->SetNpy(500);
    Double_t vb4 = b4->Integral(0, kper_max, 0, TMath::Pi(), 0, 1., 0.0001);
    //integrale de RecalCombLin4Contour pour kper de 0 a kper_max, phi de 0 a Pi (*2), x de 0 a 1


    RecalInteg4b = 2 * fZ2 * vb4;
    delete b4;

} // end def TBag::Cal_RecalInteg4b



/*--------- Double_t TBag::IminterInteg4( Double_t *var, Double_t *par) ------*/

/*!
 * \fn Double_t TBag::IminterInteg4( Double_t *var, Double_t *par)
 * retourne ImcalCombLin4axeR(xi,xi) * fZ2
 *
 * \param var = [ kper, phi, x]
 * \param par = [ xi ]
 *
 * \return ImcalCombLin4axeR(xi,xi) * fZ2
 */
Double_t TBag::IminterInteg4( Double_t *var, Double_t *par)
{

    TF3 *c4 = new TF3 ("c4", this, &TBag::ImcalCombLin4axeR, 0, kper_max, 0, 2*TMath::Pi(), 0., 1., 4, "TBag", "ImcalCombLin4axeR");
    c4->SetParameters(f2Xi.Re(), ft.Re(), fEta.Re(), fR.Re());
    //g2c->SetNpy(500);
    Double_t vc4 = c4->Eval(var[0], var[1], par[0]/4.);

    delete c4;
    return fZ2 * vc4;

} // end def TBag::IminterInteg4



/*----------------------- void TBag::Cal_ImcalInteg4 () ----------------------*/

/*!
 * \fn void TBag::Cal_ImcalInteg4  ()
 * calcule l'integrale de IminterInteg4 pour kper de 0 a kper_max, phi de 0 a Pi (*2)
 *
 */
void TBag::Cal_ImcalInteg4 ()
{

    TF2 *d4 = new TF2 ("d4", this, &TBag::IminterInteg4, 0., kper_max, 0., 2.* TMath::Pi(), 1, "TBag", "IminterInteg4");
    // d4: TF2 fonction de kper, phi
    // parametres de d4: x

    d4->SetParameter(0,f2Xi.Re());
    d4->SetNpy(400);
    Double_t vd4 = d4->Integral(0., kper_max, 0., TMath::Pi(), 0.0001);
    //integrale de IminterInteg4 pour kper de 0 a kper_max, phi de 0 a Pi (*2)

    ImcalInteg4 = 2 * TMath::Pi() * vd4;
    // attention le fZ2 est deja dans IminterInteg4
    // le pi est la pour faire pi * (F(xi,xi) - F(-xi,xi))
    delete d4;

} // end def TBag::Cal_ImcalInteg4



/*------- Double_t TBag::functionintegFF4( Double_t *var, Double_t *par) -----*/

/*!
 * \fn Double_t TBag::functionintegFF4( Double_t *var, Double_t *par)
 *
 * Fait le lien avec CombLin4 (qui prend des arguments complexes et rend une valeur complexe)
 * Rend la partie reelle de CombLin4
 * Necessaire au calcul des facteurs de forme.
 *
 * \param var = [kper, theta, x]
 * \param par = [xi, t, eta, R]
 *
 * \return Re( CombLin4(kper, phi, x, xi, t, eta, R) )
 */
Double_t TBag::functionintegFF4( Double_t *var, Double_t *par)
{

    return ( CombLin4( TComplex (var[0],0), TComplex (var[1],0), TComplex (var[2],0), TComplex (par[0],0), TComplex (par[1],0), TComplex (par[2],0), TComplex (par[3],0))).Re();

} // end def TBag::functionintegFF4



/*------------------------ void TBag::Cal_integFF4 () ------------------------*/

/*!
 * \fn void TBag::Cal_integFF4 ()
 *
 * \f[ \int _{-1} ^1 dx \int_0 ^{k \perp _{max}} dk\perp \int _0 ^{2 \pi} d \theta CombLin4(k \perp, \theta, x) \f]
 *
 * calcule l'integrale de functionintegFF4 pour kper de 0 a kper_max, phi de 0 a Pi (*2), x de -1 a 1
 * Necessaire au calcul des facteurs de forme.
 */
void TBag::Cal_integFF4 ()
{

    TF3 *d4bis = new TF3 ("d3bis", this, &TBag::functionintegFF4, 0., kper_max, 0., 2.* TMath::Pi(), -1, 1, 4, "TBag", "functionintegFF4");
    // d4bis : fonction de kper, phi, x
    // parametres: xi, t, eta, R

    d4bis->SetParameters(f2Xi.Re(), ft.Re(), fEta.Re(), fR.Re());
    d4bis->SetNpy(400);

    Double_t vd4bis = d4bis->Integral(0., kper_max, 0., TMath::Pi(), -1, 1, 0.0001);
    //integrale de functionintegFF4 pour kper de 0 a kper_max, phi de 0 a Pi (*2), x de -1 a 1


    integFF4 = 2 * fZ2 * vd4bis;
    delete d4bis;

} // end def TBag::Cal_integFF4



/*------------------------ void TBag::Cal_RecalInteg () ----------------------*/

/*!
 * \fn void TBag::Cal_RecalInteg ()
 *
 * Fait les combinaisons lineaires des integrales precedemment calculees
 * Necessaire au calcul des CFF
 *
 */
void TBag::Cal_RecalInteg ()
{

    Flag_RecalInteg = kTRUE;

    RecalInteg1 = RecalInteg1a - RecalInteg1b;
    RecalInteg2 = RecalInteg2a - RecalInteg2b;
    RecalInteg3 = RecalInteg3a + RecalInteg3b;
    RecalInteg4 = RecalInteg4a + RecalInteg4b;

} // end def TBag::Cal_RecalInteg



/*----------- void TBag::FillGPD( Double_t *TabGPD, Int_t flavor ) -----------*/

/*!
 * \fn void TBag::FillGPD( Double_t *TabGPD, Int_t flavor )
 *
 * Fait l'inversion du systeme d'equations 27/29 ou 30/31
 * Remplit 1 tableau avec les GPD pour une saveur de quark donnee
 *
 * \param TabGPD : tableau de taille 4
 * \param flavor : 0 pour le quark d, 1 pour le quark u
 *
 */
void TBag::FillGPD( Double_t *TabGPD, Int_t flavor )
{

    Double_t coeff1=0;
    Double_t coeff234=0;

    switch(flavor)
    {

        /* ----- Definition des coefficients dans le cas du quark up ----- */

        case(1) :
            coeff1 = 2;
            coeff234 = 4./3.;

        break;

        /* ----- Definition des coefficients dans le cas du quark down ----- */

        case(0) :
            coeff1 = 1;
            coeff234 = -1./3.;

        break;
    } // end switch

                /* ----- Inversion des matrices ----- */

    // E
    TabGPD[0] = ( -coeff1 * ValInteg1 + coeff234 * ValInteg2 ) / ( 1. - fA);

    // H
    TabGPD[1] = (  coeff1 * ValInteg1 - fA * coeff234 * ValInteg2 ) / ( 1. - fA);

    // Et
    TabGPD[2] = (- fD * coeff234 * ValInteg3 + fB * coeff234 * ValInteg4 ) / fD2;

    // Ht
    TabGPD[3] = ( fE * coeff234 * ValInteg3 - fC * coeff234 * ValInteg4 ) / fD2;

} // end def TBag::FillGPD



/*- void TBag::FillCFF( Double_t *Tab_Re_CFF, Double_t *Tab_Im_CFF, Int_t flavor ) -*/

/*!
 * \fn void TBag::FillCFF( Double_t *Tab_Re_CFF, Double_t *Tab_Im_CFF, Int_t flavor )
 *
 * Fait l'inversion du systeme d'equations 27/29 ou 30/31
 * Remplit 2 tableaux avec les CFF pour une saveur de quark donnee
 *
 * \param Tab_Re_CFF : tableau de taille 4
 * \param Tab_Im_CFF : tableau de taille 4
 * \param flavor : 0 pour le quark d, 1 pour le quark u
 *
 */
void TBag::FillCFF( Double_t *Tab_Re_CFF, Double_t *Tab_Im_CFF, Int_t flavor )
{

    if (!Flag_RecalInteg)
    {
        cout << "TBag : Integrales non calculees, ou fonction Cal_RecalInteg pas appelee" << endl;
        exit(-1);
    } // end if Flag_RecalInteg

    Double_t coeff1=0;
    Double_t coeff234=0;

    switch(flavor)
    {

        /* ----- Definition des coefficients dans le cas du quark up ----- */

        case(1) :

            coeff1 = 2;
            coeff234 = 4./3.;

        break;

        /* ----- Definition des coefficients dans le cas du quark down ----- */

        case(0) :

            coeff1 = 1;
            coeff234 = -1./3.;

        break;
    } // end switch

                /* ----- Inversion des matrices ----- */

    // E
    Tab_Re_CFF[0] = ( -coeff1 * RecalInteg1 + coeff234 * RecalInteg2 ) / ( 1. - fA);
    Tab_Im_CFF[0] = ( -coeff1 * ImcalInteg1 + coeff234 * ImcalInteg2 ) / ( 1. - fA);

    // H
    Tab_Re_CFF[1] = (  coeff1 * RecalInteg1 - fA * coeff234 * RecalInteg2 ) / ( 1. - fA);
    Tab_Im_CFF[1] = (  coeff1 * ImcalInteg1 - fA * coeff234 * ImcalInteg2 ) / ( 1. - fA);

    // Et
    Tab_Re_CFF[2] = (- fD * coeff234 * RecalInteg3 + fB * coeff234 * RecalInteg4 ) / fD2;
    Tab_Im_CFF[2] = (- fD * coeff234 * ImcalInteg3 + fB * coeff234 * ImcalInteg4 ) / fD2;

    // Ht
    Tab_Re_CFF[3] = ( fE * coeff234 * RecalInteg3 - fC * coeff234 * RecalInteg4 ) / fD2;
    Tab_Im_CFF[3] = ( fE * coeff234 * ImcalInteg3 - fC * coeff234 * ImcalInteg4 ) / fD2;

} // end def TBag::FillCFF



/*-------------- TBag::FillFF( Double_t *TabFF, Int_t flavor ) ---------------*/

/*!
 * \fn void TBag::FillFF( Double_t *TabFF, Int_t flavor )
 *
 * Fait l'inversion du systeme d'equations 27/29 ou 30/31
 * Remplit 1 tableau avec les FF pour une saveur de quark donnee
 *
 * \param TabFF : tableau de taille 4
 * \param flavor : 0 pour le quark d, 1 pour le quark u
 *
 */
void TBag::FillFF( Double_t *TabFF, Int_t flavor )
{

    Double_t coeff1=0;
    Double_t coeff234=0;

    switch(flavor)
    {

        /* ----- Definition des coefficients dans le cas du quark up ----- */

        case(1) :

            coeff1 = 2;
            coeff234 = 4./3.;

        break;

        /* ----- Definition des coefficients dans le cas du quark down ----- */

        case(0) :

            coeff1 = 1;
            coeff234 = -1./3.;

        break;
    } // end switch

                /* ----- Inversion des matrices ----- */
    // E
    TabFF[0] = ( -coeff1 * integFF1 + coeff234 * integFF2 ) / ( 1. - fA);

    // H
    TabFF[1] = (  coeff1 * integFF1 - fA * coeff234 * integFF2 ) / ( 1. - fA);

    // Et
    TabFF[2] = (- fD * coeff234 * integFF3 + fB * coeff234 * integFF4 ) / fD2;

    // Ht
    TabFF[3] = ( fE * coeff234 * integFF3 - fC * coeff234 * integFF4 ) / fD2;

} // end def TBag::FillFF



/*-----------------------------TBag::Print_GPD()  ----------------------------*/

/*!
 * \fn void TBag::Print_GPD()
 *
 * affiche les GPD, en supposant que le calcul a ete fait
 *
 *
 */
void TBag::Print_GPD()
{

    Double_t tab[2][4]={{0,0,0,0},{0,0,0,0}};
    FillGPD(tab[0], 0); //down
    FillGPD(tab[1], 1); //up

    string colonnes[4] = {"E","H","Etilde","Htilde"};
    string lignes[2] = {"down", "up"};


	cout << "--------------" << endl;
	cout << "      GPD     " << endl;
	cout << "--------------" << endl;

	cout << "\t";
	for (Int_t j=0; j<4;j++)
	{
	    cout << " "<< colonnes[j] << "\t\t";
	} // end for j
	cout << endl;


	for (Int_t i=0; i<2;i++)
	{
	    cout << lignes[i] <<"\t";
	    for (Int_t j=0; j<4;j++)
	    {
	        printf("%.8e\t", tab[i][j]);
	    } // end for j
	    cout << endl;
	} // end for i
	cout << endl;

} // end def TBag::Print_GPD



/*------------------------- void TBag::Print_CFF() ---------------------------*/

/*!
 * \fn void TBag::Print_CFF()
 *
 * affiche les CFF, en supposant que le calcul a ete fait
 *
 *
 */
void TBag::Print_CFF()
{

    Double_t tab[4][4];
    FillCFF(tab[0], tab[1], 0);
    FillCFF(tab[2], tab[3], 1);

    string colonnes[4] = {"E cal     ","H cal     ","Etilde cal","Htilde cal"};
    string lignes[4] = {"down RE", "down IM", "up RE", "up IM"};

    cout << "----------------------" << endl;
	cout << " Compton Form Factors " << endl;
	cout << "----------------------" << endl;

	cout << "\t";
	for (Int_t j=0; j<4;j++)
	{
	    cout << " " << colonnes[j] << "\t";
	} // end for j
	cout << endl;


	for (Int_t i=0; i<4;i++)
	{
	    cout << lignes[i] <<"\t";
	    for (Int_t j=0; j<4;j++)
	    {
	        printf("%.8e\t", tab[i][j]);
	    } // end for j
	    cout << endl;
	} // end for i
	cout << endl;

} // end def TBag::Print_CFF



/*------------------------ void TBag::Print_FF() -----------------------------*/

/*!
 * \fn void TBag::Print_FF()
 *
 * affiche les FF, en supposant que le calcul a ete fait
 *
 *
 */
void TBag::Print_FF()
{

    Double_t tab[2][4];
    FillFF(tab[0], 0); //down
    FillFF(tab[1], 1); //up

    string colonnes[4] = {"FF E     ","FF H     ","FF Etilde","FF Htilde"};
    string lignes[2] = {"down", "up"};

    cout << "--------------" << endl;
	cout << " Form Factors " << endl;
	cout << "--------------" << endl;
	cout << "\t";
	for (Int_t j=0; j<4;j++)
	{
	    cout << " "<< colonnes[j] << "\t";
	} // end for j
	cout << endl;


	for (Int_t i=0; i<2;i++)
	{
	    cout << lignes[i] <<"\t";
	    for (Int_t j=0; j<4;j++)
	    {
	        printf("%.8e\t", tab[i][j]);
	    } // end for j
 	    cout << endl;
	} // end for i
    cout << endl;

} // end def TBag::Print_FF



/*---------------------- void TBag::Print_Params () --------------------------*/

/*!
 * \fn void TBag::Print_Params()
 *
 * affiche les parametres actuels
 *
 *
 */
void TBag::Print_Params ()
{

    cout << "-------------------" << endl <<  "   Parameters" << endl << "-------------------" << endl;

   	cout << "x\txi\tt\teta\tR" << endl;
    cout << fX.Re()<< "\t"<<fXi.Re()<< "\t"<<ft.Re()<< "\t"<<fEta.Re()<< "\t"<<fR.Re() << endl;

    cout<<endl;

} // end def TBag::Print_Params



/* void TBag::GetGPD(Double_t tab_GPD[], Double_t x, Double_t xi, Double_t t) */

/*!
 * \fn void TBag::GetGPD(Double_t tab_GPD[], Double_t x, Double_t xi, Double_t t)
 *
 * Lance TOUS les calculs necessaires a l'obtention des GPD
 *  * cinematique
 *  * calcul des integrales
 *  * remplissage du tableau de GPD
 *
 * Le contenu du tableau est le suivant:
 * \li E d  : 0
 * \li H d  : 1
 * \li Et d : 2
 * \li Ht d : 3
 * \li E u  : 4
 * \li H u  : 5
 * \li Et u : 6
 * \li Ht u : 7
 *
 *
 * \param tab_GPD : tableau de taille 8
 * \param x
 * \param xi
 * \param t
 *
 * \return
 */
void TBag::GetGPD(Double_t tab_GPD[], Double_t x, Double_t xi, Double_t t)
{

    SetKinematics(x, xi, t);
    Cal_IntegreZ();

    Cal_Integ1();
    Cal_Integ2();
    Cal_Integ3();
    Cal_Integ4();


    FillGPD(tab_GPD, 0);
    FillGPD(tab_GPD+4, 1);

    return;

} // end def TBag:GetGPD



/*------ void TBag::GetCFF(Double_t tab_CFF[], Double_t xi, Double_t t) ------*/

/*!
 * \fn void TBag::GetCFF(Double_t tab_CFF[], Double_t xi, Double_t t)
 *
 * Lance TOUS les calculs necessaires a l'obtention des Compton Form Factors
 *  * cinematique
 *  * calcul des integrales
 *  * remplissage du tableau de CFF
 *
 * Le contenu du tableau est le suivant:
 * \li Re E d : 0
 * \li Re H d : 1
 * \li Re Et d : 2
 * \li Re Ht d : 3
 * \li Im E d : 4
 * \li Im H d : 5
 * \li Im Et d : 6
 * \li Im Ht d : 7
 *
 * \li Re E u : 8
 * \li Re H u : 9
 * \li Re Et u : 10
 * \li Re Ht u : 11
 * \li Im E u : 12
 * \li Im H u : 13
 * \li Im Et u : 14
 * \li Im Ht u : 15
 *
 *
 * \param tab_CFF : tableau de taille 16
 * \param xi
 * \param t
 *
 * \return pointeur vers le tableau rempli
 */
void TBag::GetCFF(Double_t tab_CFF[], Double_t xi, Double_t t)
{

    SetKinematics(0., xi, t); // Integration over x in FillCFF
    Cal_IntegreZ();

    Cal_RecalInteg1a();
    Cal_RecalInteg1b();
    Cal_ImcalInteg1();

    Cal_RecalInteg2a();
    Cal_RecalInteg2b();
    Cal_ImcalInteg2();

    Cal_RecalInteg3a();
    Cal_RecalInteg3b();
    Cal_ImcalInteg3();

    Cal_RecalInteg4a();
    Cal_RecalInteg4b();
    Cal_ImcalInteg4();


    Cal_RecalInteg();


    FillCFF(tab_CFF,tab_CFF+4, 0);
    FillCFF(tab_CFF+8,tab_CFF+12, 1);

    return;

} // end def TBag:GetCFF



/*------- void TBag::GetFF(Double_t tab_FF[], Double_t xi, Double_t t) -------*/

/*!
 * \fn void TBag::GetFF(Double_t tab_FF[], Double_t xi, Double_t t)
 *
 * Lance TOUS les calculs necessaires a l'obtention des Form Factors
 *  * cinematique
 *  * calcul des integrales
 *  * remplissage du tableau de FF
 *
 * Le contenu du tableau est le suivant:
 * \li E d  : 0
 * \li H d  : 1
 * \li Et d : 2
 * \li Ht d : 3
 * \li E u  : 4
 * \li H u  : 5
 * \li Et u : 6
 * \li Ht u : 7
 *
 * \param tab_FF : tableau de taille 8
 * \param xi
 * \param t
 *
 * \return
 */
void TBag::GetFF(Double_t tab_FF[], Double_t xi, Double_t t)
{

    SetKinematics(0., xi, t); // Integration over x in FillFF
    Cal_IntegreZ();

    Cal_integFF1();
    Cal_integFF2();
    Cal_integFF3();
    Cal_integFF4();

    FillFF(tab_FF,0);
    FillFF(tab_FF+4,1);

    return;

} // end def TBag:GetFF


