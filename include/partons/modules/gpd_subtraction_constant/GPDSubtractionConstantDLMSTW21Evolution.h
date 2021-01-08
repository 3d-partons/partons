/*
 * evolution.h
 *
 *  Created on: Aug 6, 2019
 *      Author: partons
 */

#ifndef INCLUDE_EVOLUTION_H_
#define INCLUDE_EVOLUTION_H_

#include <stddef.h>
#include <cmath>
#include <vector>

namespace PARTONS {
namespace GPDSubtractionConstantDLMSTW21Evolution {

/**
 * Color factors.
 */
const double c_cf = 4. / 3.;
const double c_tf = 0.5;
const double c_ca = 3.;

/**
 * Quark masses (squared).
 */
const double c_m_u2 = pow(2.2 / 1.E3, 2);
const double c_m_d2 = pow(4.7 / 1.E3, 2);
const double c_m_s2 = pow(96. / 1.E3, 2);
const double c_m_c2 = pow(1.28, 2);
const double c_m_b2 = pow(4.18, 2);
const double c_m_t2 = pow(173.1, 2);

/**
 * Number of active flavors.
 */
size_t getNActiveFlavors(double muF2);

/**
 * Threshold for a given number of active flavors.
 */
double getThreshold2(size_t nf);

/**
 * Lambda_QCD.
 */
double lambdaQCD(size_t nf);

/**
 * Lambda_QCD.
 */
double lambdaQCD(double muF2);

/**
 * Beta coefficient.
 */
double beta(size_t i, size_t nf);

/**
 * Running coupling.
 */
double alphaS(double muF2);

/**
 * Anomalous dimension.
 */
double evolutionGamma(double n, int sign, size_t nf);

/**
 * Gluon evolution coefficients denoted as a_n^\pm.
 */
double evolutionA(size_t n, int sign, size_t nf);

/**
 * Evolve quark non-singlet ignoring quark's thresholds.
 * dQuarkDiff is a diffrence of twho d^q.
 */
double evolveQuarkDiff(double muF2, double muF20, double dQuarkDiff, size_t n,
		size_t nf);
/**
 * Evolve quark singlet ignoring quark's thresholds.
 * dQuarkSum is a sum of d^q.
 */
double evolveQuarkSum(double muF2, double muF20, double dQuarkSum,
		double dGluon, size_t n, size_t nf);

/**
 * Evolve gluons ignoring quark's thresholds.
 * dQuarkSum is a sum of d^q.
 */
double evolveGluon(double muF2, double muF20, double dQuarkSum, double dGluon,
		size_t n, size_t nf);

/**
 * Evolve quarks and gluons ignoring quark's thresholds.
 * dPartons is a vector of dGluon, dQuark1, dQuark2, ...
 */
std::vector<double> evolveQuarkGluon(double mu2, double mu20,
		std::vector<double> dPartons, size_t n, size_t nf);

/**
 * Evolve quarks and gluons including quark's thresholds.
 * dPartons is a vector of dGluon, dQuark1, dQuark2, ...
 */
std::vector<double> evolveQuarkGluon(double mu2, double mu20,
		std::vector<double> dPartons, size_t n);
}
}

#endif /* INCLUDE_EVOLUTION_H_ */
