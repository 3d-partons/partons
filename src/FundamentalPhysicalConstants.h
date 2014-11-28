#ifndef FUNDAMENTAL_PHYSICAL_CONSTANTS_H
#define FUNDAMENTAL_PHYSICAL_CONSTANTS_H

/**
 * @file FundamentalPhysicalConstants.h
 * @date 09/07/2014
 * @authors Bryan BERTHOU
 * @authors Herve MOUTARDE
 * @version 1.0
 */

// Mathematical constants
const double PI = 3.14159265358979323846;
const double ZETA_3 = 1.2020569031595942;

// Physical constants

const double SPEED_OF_LIGHT = 299792458;            ///< PDG2008
const double PLANCK_CONSTANT = 6.62606896 * 1.e-34; ///< PDG2009
const double FINE_STRUCTURE_CONSTANT = 1. / 137.035999679; ///< PDG2008

// Conversions between GeV and fm

//TODO Finir de passer les constantes en MAJ
const double HBarC = 197.3269631;                 ///< PDG2008
const double HBarC2 = 0.389379304;                ///< PDG2008
const double CONV_GEVm1_TO_FM = 0.1973269631;
const double CONV_GEVm2_TO_NBARN = 0.389379304e+6;
const double CONV_GEVm2_TO_PBARN = 0.389379304e+9;

// Particle masses

const double PROTON_MASS = 0.938272013;            ///< PDG2010

const double ELECTRON_MASS = 0.510998910e-3;       ///< PDG2008
const double MUON_MASS = 105.658367e-3;            ///< PDG2008
const double TAU_MASS = 1.77684;                   ///< PDG2008

const double QUARK_UP_MASS = 0.0024;                ///< PDG2010
const double QUARK_DOWN_MASS = 0.0049;              ///< PDG2010
const double QUARK_STRANGE_MASS = 0.1;              ///< PDG2010
const double QUARK_CHARM_MASS = 1.29;               ///< PDG2010
const double QUARK_BOTTOM_MASS = 4.19;              ///< PDG2010
const double QUARK_TOP_MASS = 172.9;                ///< PDG2010

const double PI_ZERO_MASS = 0.1349766;              ///< PDG2010

const double Z_BOSON_MASS = 91.1876;                ///< PDG2010

// Particle electric charges

const double U_ELEC_CHARGE = 2. / 3.; ///< Electric charge of up quark (in units of e)
const double D_ELEC_CHARGE = -1. / 3.;
const double S_ELEC_CHARGE = -1. / 3.;
const double C_ELEC_CHARGE = 2. / 3.;
const double B_ELEC_CHARGE = -1. / 3.;
const double T_ELEC_CHARGE = 2. / 3.;

const double U2_ELEC_CHARGE = 4. / 9.; ///< Square of electric charge of up quark
const double D2_ELEC_CHARGE = 1. / 9.; ///< Square of electric charge of down quark
const double S2_ELEC_CHARGE = 1. / 9.; ///< Square of electric charge of strange quark
const double C2_ELEC_CHARGE = 4. / 9.; ///< Square of electric charge of charm quark
const double B2_ELEC_CHARGE = 1. / 9.; ///< Square of electric charge of bottom quark
const double T2_ELEC_CHARGE = 1. / 9.; ///< Square of electric charge of top quark

//TODO more explicit name
const double POSITRON_CHARGE = 0.30282211985966434;

#endif /* FUNDAMENTAL_PHYSICAL_CONSTANTS_H */
