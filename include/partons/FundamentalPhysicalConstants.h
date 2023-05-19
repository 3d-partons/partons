#ifndef FUNDAMENTAL_PHYSICAL_CONSTANTS_H
#define FUNDAMENTAL_PHYSICAL_CONSTANTS_H

/**
 * @file FundamentalPhysicalConstants.h
 * @author Bryan BERTHOU (SPhN / CEA Saclay)
 * @author Herve MOUTARDE (SPhN / CEA Saclay)
 * @date 09 September 2014
 * @version 1.0
 */

#include <complex>

namespace PARTONS {

namespace Constant {

/**
* @namespace PARTONS::Constant
* @brief Definition of useful mathematical, physical, etc.\ constants.
*
* This header file defines a set of useful mathematical, physical, etc.\ constants to be used in PARTONS.
*/

// Mathematical constants

const double PI = 3.14159265358979323846;                   ///< Value of \f$\pi\f$.
const double ZETA_3 = 1.2020569031595942;                   ///< Apéry's constant (value of Riemann zeta function \f$\zeta(3)\f$).

// Physical constants

const double SPEED_OF_LIGHT = 299792458;                    ///< Speed of light \f$c\f$ in \f$\mathrm{m}/\mathrm{s}\f$ @cite Amsler:2008zzb.
const double PLANCK_CONSTANT = 6.62606896 * 1.e-34;         ///< Planck constant \f$h\f$ in \f$\mathrm{J}\cdot\mathrm{s}\f$ @cite Amsler:2008zzb, @cite Amsler::2009update.
const double PLANCK_CONSTANT_REDUCED = 6.58211899 * 1.e-16; ///< Planck constant reduced \f$h/2\pi\f$ in \f$\mathrm{eV}\cdot\mathrm{s}\f$ @cite Amsler:2008zzb.
const double FINE_STRUCTURE_CONSTANT = 1. / 137.035999679;  ///< Fine-structure constant \f$\alpha\f$ @cite Amsler:2008zzb.

// Conversions between GeV and fm

//TODO Finir de passer les constantes en MAJ
const double HBarC = 197.3269631;                           ///< Conversion constant \f$\hbar c\f$ in \f$\mathrm{MeV}\cdot\mathrm{fm}\f$ @cite Amsler:2008zzb
const double HBarC2 = 0.389379304;                          ///< Conversion constant \f$(\hbar c)^2\f$ in \f$\mathrm{GeV}^2\cdot\mathrm{mbarn}\f$ @cite Amsler:2008zzb
const double CONV_GEVm1_TO_FM = 0.1973269631;               ///< Conversion constant \f$\mathrm{GeV}^{-1} \rightarrow \mathrm{fm}\f$.
const double CONV_GEVm2_TO_NBARN = 0.389379304e+6;          ///< Conversion constant \f$\mathrm{GeV}^{-2} \rightarrow \mathrm{nbarn}\f$.
const double CONV_GEVm2_TO_PBARN = 0.389379304e+9;          ///< Conversion constant \f$\mathrm{GeV}^{-2} \rightarrow \mathrm{pbarn}\f$.

// Particle masses

const double PROTON_MASS = 0.938272013;                     ///< Proton mass \f$m_{p}\f$ in \f$\mathrm{GeV}\f$ @cite Nakamura:2010zzi

const double ELECTRON_MASS = 0.510998910e-3;                ///< Electron mass \f$m_{e}\f$ in \f$\mathrm{GeV}\f$ @cite Amsler:2008zzb
const double MUON_MASS = 105.658367e-3;                     ///< Muon mass \f$m_{\mu}\f$ in \f$\mathrm{GeV}\f$ @cite Amsler:2008zzb
const double TAU_MASS = 1.77684;                            ///< Tau mass \f$m_{\tau}\f$ in \f$\mathrm{GeV}\f$ @cite Amsler:2008zzb

const double QUARK_DOWN_MASS = 0.0049;                      ///< Down quark mass \f$m_{d}\f$ in \f$\mathrm{GeV}\f$ @cite Nakamura:2010zzi
const double QUARK_UP_MASS = 0.0024;                        ///< Up quark mass \f$m_{u}\f$ in \f$\mathrm{GeV}\f$ @cite Nakamura:2010zzi
const double QUARK_STRANGE_MASS = 0.1;                      ///< Strange quark mass \f$m_{s}\f$ in \f$\mathrm{GeV}\f$ @cite Nakamura:2010zzi
const double QUARK_CHARM_MASS = 1.29;                       ///< Charm quark mass \f$m_{c}\f$ in \f$\mathrm{GeV}\f$ @cite Nakamura:2010zzi
const double QUARK_BOTTOM_MASS = 4.19;                      ///< Bottom quark mass \f$m_{b}\f$ in \f$\mathrm{GeV}\f$ @cite Nakamura:2010zzi
const double QUARK_TOP_MASS = 172.9;                        ///< Top quark mass \f$m_{t}\f$ in \f$\mathrm{GeV}\f$ @cite Nakamura:2010zzi

const double PI_ZERO_MASS = 0.1349766;                      ///< Meson \f$\pi^0\f$ mass \f$m_{\pi^0}\f$ in \f$\mathrm{GeV}\f$ @cite Nakamura:2010zzi

const double Z_BOSON_MASS = 91.1876;                        ///< Boson \f$Z\f$ mass \f$m_{Z}\f$ in \f$\mathrm{GeV}\f$ @cite Nakamura:2010zzi

// Particle electric charges

const double D_ELEC_CHARGE = -1. / 3.;                      ///< Electric charge of down quark in units of \f$e\f$.
const double U_ELEC_CHARGE = 2. / 3.;                       ///< Electric charge of up quark in units of \f$e\f$.
const double S_ELEC_CHARGE = -1. / 3.;                      ///< Electric charge of strange quark in units of \f$e\f$.
const double C_ELEC_CHARGE = 2. / 3.;                       ///< Electric charge of charm quark in units of \f$e\f$.
const double B_ELEC_CHARGE = -1. / 3.;                      ///< Electric charge of bottom quark in units of \f$e\f$.
const double T_ELEC_CHARGE = 2. / 3.;                       ///< Electric charge of top quark in units of \f$e\f$.

const double D2_ELEC_CHARGE = 1. / 9.;                      ///< Square of electric charge of down quark in units of \f$e\f$.
const double U2_ELEC_CHARGE = 4. / 9.;                      ///< Square of electric charge of up quark in units of \f$e\f$.
const double S2_ELEC_CHARGE = 1. / 9.;                      ///< Square of electric charge of strange quark in units of \f$e\f$.
const double C2_ELEC_CHARGE = 4. / 9.;                      ///< Square of electric charge of charm quark in units of \f$e\f$.
const double B2_ELEC_CHARGE = 1. / 9.;                      ///< Square of electric charge of bottom quark in units of \f$e\f$.
const double T2_ELEC_CHARGE = 4. / 9.;                      ///< Square of electric charge of top quark in units of \f$e\f$.

//TODO more explicit name
const double POSITRON_CHARGE = 0.30282211985966434;         ///< Positron charge \f$e\f$ in natural units (\f$e^2 = 4\pi\alpha\f$).

// Complex numbers
const std::complex<double> COMPLEX_UNIT = std::complex<double>(0., 1.); ///< Complex unit.

// Mesons

const double MESON_RHOMINUS_MASS = 775.26e-3;               ///< \f$\rho^-\f$ mass in \f$\mathrm{GeV}\f$ Phys. 2020, 083C01 (2020).
const double MESON_RHO0_MASS = 775.26e-3;                   ///< \f$\rho^0\f$ mass in \f$\mathrm{GeV}\f$ Phys. 2020, 083C01 (2020).
const double MESON_RHOPLUS_MASS = 775.26e-3;                ///< \f$\rho^+\f$ mass in \f$\mathrm{GeV}\f$ Phys. 2020, 083C01 (2020).
const double MESON_OMEGA_MASS = 782.65e-3;                  ///< \f$\omega\f$ mass in \f$\mathrm{GeV}\f$ Phys. 2020, 083C01 (2020).
const double MESON_PHI_MASS = 1019.461e-3;                  ///< \f$\phi\f$ mass in \f$\mathrm{GeV}\f$ Phys. 2020, 083C01 (2020).
const double MESON_JPSI_MASS = 3096.900e-3;                 ///< \f$J/\Psi\f$ mass in \f$\mathrm{GeV}\f$ Phys. 2020, 083C01 (2020).
const double MESON_UPSILON_MASS = 9460.30e-3;               ///< \f$\Upsilon\f$ mass in \f$\mathrm{GeV}\f$ Phys. 2020, 083C01 (2020).

const double MESON_PIMINUS_MASS = 139.57039e-3;             ///< \f$\pi^-\f$ mass in \f$\mathrm{GeV}\f$ Phys. 2020, 083C01 (2020).
const double MESON_PI0_MASS = 134.9768e-3;                  ///< \f$\pi^0\f$ mass in \f$\mathrm{GeV}\f$ Phys. 2020, 083C01 (2020).
const double MESON_PIPLUS_MASS = 139.57039e-3;              ///< \f$\pi^+\f$ mass in \f$\mathrm{GeV}\f$ Phys. 2020, 083C01 (2020).

} /* namespace Constant */

} /* namespace PARTONS */

#endif /* FUNDAMENTAL_PHYSICAL_CONSTANTS_H */
