#ifndef MSTWPDF_H
#define MSTWPDF_H

/**
 * @file MSTWPDF.h
 * @author: Bryan BERTHOU (SPhN / CEA Saclay)
 * @date June 21, 2016
 * @version 1.0
 */

#include <string>
#include <vector>

#include "math/MDArray.h"
#include "PartonContent.h"

namespace PARTONS {

/**
 * @class MSTWPDF
 *
 * @brief Class header file for MSTW PDFs.
 *
 * Original C++ version by Jeppe Andersen. Modified by Graeme Watt <Graeme.Watt(at)cern.ch>.
 * Updated 25/06/2010: Enlarge allowed range for m_c and m_b.
 *
 * Usage:
 * Initialising an instance of the class:
 \code{.cpp}
 c_mstwpdf pdf("filename.dat");
 \endcode
 * with "filename.dat" being the data file to interpolate.
 *
 * A call to the method
 \code{.cpp}
 c_mstwpdf::update(x, q)
 \endcode
 * updates the parton content to the values at (x, \f$q^2\f$). The parton contents can then be accessed in `c_mstwpdf::cont.upv` etc. This method is faster if all the flavors need to be evaluated.
 *
 * A call to the method
 \code{.cpp}
 c_mstwpdf::parton(f, x, q);
 \endcode
 * returns the value of the PDF with flavor `f` at (x, \f$q^2\f$). This method is faster if only one flavor needs to be evaluated.
 *
 * Use PDG notation for parton flavor (apart from gluon = 0, not 21):
 f  | parton
 ------------- | -------------
 -6  | tbar
 -5  | bbar
 -4  | cbar
 -3  | sbar
 -2  | ubar
 -1  | dbar
 0   | g
 1   | d
 2   | u
 3   | s
 4   | c
 5   | b
 6   | t

 * Can also get valence quarks directly:
 f  | parton
 ------------- | -------------
 7   | dv
 8   | uv
 9   | sv
 10  | cv
 11  | bv
 12  | tv

 * Photon: `f = 13`.
 */
class MSTWPDF: public BaseObject {

public:

    /**
     * Default constructor.
     */
    MSTWPDF();

    /**
     * Destructor.
     */
    virtual ~MSTWPDF();

    /**
     * Initialize by reading a specific PDF grid.
     * @param gridFilePath Path to the grid to be loaded.
     */
    void init(const std::string &gridFilePath);

    /**
     * Update internal variables for all flavors for given values of x and \f$q\f$.
     * @param x Fraction of proton's momentum carried by parton.
     * @param q Scale in GeV.
     */
    void update(double x, double q);

    /**
     * Get PDF for specific flavor and given values of x and \f$q\f$.
     * @param flavour Id of requested flavor.
     * @param x Fraction of proton's momentum carried by parton.
     * @param q Scale in GeV.
     * @return Requested value of PDF.
     */
    double parton(int flavour, double x, double q);

    //********************************************************
    //*** SETTERS AND GETTERS ********************************
    //********************************************************

    /**
     * Get PDF container.
     */
    const PartonContent& getPartonContent() const;

    /**
     * Set PDF container.
     */
    void setPartonContent(const PartonContent& partonContent);

private:

    bool warn;  ///< Option to turn on warning if extrapolating.
    bool fatal; ///< Option to return zero instead of terminating when invalid input values of x and \f$q\f$ are used.
    int np;     ///< Number of parton flavors.
    int nx;     ///< Number of x grid points.
    int nq;     ///< Number of \f$q^2\f$ grid points.
    int nqc0;   ///< Number of \f$q^2\f$ bins below charm threshold.
    int nqb0;   ///< Number of \f$q^2\f$ bins below bottom threshold.
    std::vector<double> xx;     ///< Grid points in x.
    std::vector<double> qq;     ///< Grid points in \f$q^2\f$.
    MDArray<double> c;          ///< Coefficients used for interpolation.
    double xmin;                ///< Minimum x grid point.
    double xmax;                ///< Maximum x grid point.
    double qsqmin;              ///< Minimum \f$q^2\f$ grid point.
    double qsqmax;              ///< Maximum \f$q^2\f$ grid point.
    double mCharm;              ///< Mass of charm quark.
    double mBottom;             ///< Mass of bottom quark.
    double alphaSQ0;            ///< \f$\alpha_{s}(Q_{0})\f$
    double alphaSMZ;            ///< \f$\alpha_{s}(M_{Z})\f$
    double distance;            ///< Distance.
    double tolerance;           ///< Tolerance.
    int alphaSorder;            ///< \f$\alpha_{s}\f$ order.
    int alphaSnfmax;            ///< \f$\alpha_{s}\f$ \f$n_{f}\f$ max.

    PartonContent m_partonContent;      ///< PDF container.

    /**
     * Interpolate to given x and \f$q\f$ for specific flavor.
     */
    double parton_interpolate(int flavour, double xxx, double qqq);

    /**
     * Extrapolate to given x and \f$q\f$ for specific flavor.
     */
    double parton_extrapolate(int flavour, double xxx, double qqq);

    /**
     * Returns an integer j such that x lies inbetween xx[j] and xx[j+1]. Unit offset of increasing ordered array xx assumed. n is the length of the array (xx[n] highest element).
     */
    int locate(std::vector<double> xx, int n, double x);

    /**
     * Returns the estimate of the derivative at x1 obtained by a polynomial interpolation using the three points (x_i,y_i).
     */
    double polderivative1(double x1, double x2, double x3, double y1, double y2,
            double y3);

    /**
     * Returns the estimate of the derivative at x2 obtained by a polynomial interpolation using the three points (x_i,y_i).
     */
    double polderivative2(double x1, double x2, double x3, double y1, double y2,
            double y3);

    /**
     * Returns the estimate of the derivative at x3 obtained by a polynomial interpolation using the three points (x_i,y_i).
     */
    double polderivative3(double x1, double x2, double x3, double y1, double y2,
            double y3);
};

} /* namespace PARTONS */

#endif /* MSTWPDF_H */
