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

/**
 * @class MSTWPDF
 *
 * @brief // Class header file for MSTW PDFs.
 * Original C++ version by Jeppe Andersen.
 *  Modified by Graeme Watt <Graeme.Watt(at)cern.ch>.
 *  Updated 25/06/2010: Enlarge allowed range for m_c and m_b.
 *
 *  Usage:
 *  Initialising an instance of the class:
 *  c_mstwpdf pdf("filename.dat")
 *  with "filename.dat" being the data file to interpolate.
 *
 *  A call to the method c_mstwpdf::update(x,q)
 *  updates the parton content to the values at (x,q^2).
 *  The parton contents can then be accessed in c_mstwpdf::cont.upv etc.
 *  This method is faster if all the flavours need to be evaluated.
 *
 *  A call to the method c_mstwpdf::parton(f,x,q)
 *  will return the value of the PDF with flavour 'f' at (x,q^2).
 *  Now use PDG notation for parton flavour (apart from gluon=0, not 21):
 *  f =   -6,  -5,  -4,  -3,  -2,  -1,0,1,2,3,4,5,6 = tbar,bbar,cbar,sbar,ubar,dbar,g,d,u,s,c,b,t.
 *  Can also get valence quarks directly:
 *  f =  7, 8, 9,10,11,12 = dv,uv,sv,cv,bv,tv.
 *  Photon: f = 13.
 *  This method is faster if only one flavour needs to be evaluated.
 */
class MSTWPDF: public BaseObject {
public:
    MSTWPDF();
    virtual ~MSTWPDF();

    void init(const std::string &gridFilePath);
    void update(double x, double q); // update cont
    double parton(int flavour, double x, double q);

    const PartonContent& getPartonContent() const;
    void setPartonContent(const PartonContent& partonContent);

private:
    bool warn; // option to turn on warning if extrapolating.
    bool fatal; // option to return zero instead of terminating when invalid input values of x and q are used.
    int np;   // number of parton flavours
    int nx;   // number of x grid points
    int nq;   // number of q^2 grid points
    int nqc0; // number of q^2 bins below charm threshold
    int nqb0; // number of q^2 bins below bottom threshold
    std::vector<double> xx;    // grid points in x
    std::vector<double> qq;    // grid points in q^2
    //std::vector<std::vector<std::vector<std::vector<std::vector<double> > > > > c; // coefficients used for interpolation
    MDArray<double> c;
    double xmin;
    double xmax;
    double qsqmin;
    double qsqmax;
    double mCharm;
    double mBottom;
    double alphaSQ0;
    double alphaSMZ;
    double distance;
    double tolerance;
    int alphaSorder;
    int alphaSnfmax;

    PartonContent m_partonContent;

    double parton_interpolate(int flavour, double xxx, double qqq);
    double parton_extrapolate(int flavour, double xxx, double qqq);
    int locate(std::vector<double> xx, int n, double x);

    double polderivative1(double x1, double x2, double x3, double y1, double y2,
            double y3);
    double polderivative2(double x1, double x2, double x3, double y1, double y2,
            double y3);
    double polderivative3(double x1, double x2, double x3, double y1, double y2,
            double y3);
};

#endif /* MSTWPDF_H */
