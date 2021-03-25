#ifndef ALGEBRAICTOYMODEL_H
#define ALGEBRAICTOYMODEL_H

/**
 * @file algebraicToyModel.h
 * @author José Manuel Morgado Chavez (University Of Huelva)
 * @author Cédric Mezrag (CEA Saclay)
 * @date 3rd March 2021 
 * @version 1.0
 */


#include <ElementaryUtils/parameters/Parameters.h>
#include <string>
#include <vector>
#include <iostream>


#include "../GPDModule.h"

#include <NumA/linear_algebra/vector/VectorD.h>
#include <NumA/linear_algebra/matrix/MatrixD.h>
#include <NumA/triangulation/mesh.h>
#include <NumA/RadonTransform/RadonTransform.h>

#include <NumA/interpolation/CubicSpline.h>

namespace PARTONS {

class algebraicToyModel: public PARTONS::GPDModule 
{

public:

    /**
     * ID assigned by BaseObjectRegistry.
     */
    static const unsigned int classId;

    /**
     * Default constructor.
     * @param className Name of class.
     */
    algebraicToyModel(const std::string& className);

    /**
     * Destructor.
     */
    virtual ~algebraicToyModel();
    virtual algebraicToyModel* clone() const;
    virtual void resolveObjectDependencies();
    virtual void configure(const ElemUtils::Parameters &parameters);
    // virtual std::string toString() const;

protected:

    /**
     * Copy constructor.
     * @param other Object to be copied.
     * */

    algebraicToyModel(const algebraicToyModel& other);

    virtual void isModuleWellConfigured();
    virtual void initModule();

    virtual PARTONS::PartonDistribution computeH();

    /**
      * Computation of the double distribution.
      * 
      * Sets the DD attribute of the class.
      */
    void computeDD( bool t0 );

    /**
      * Mesh building.
      * 
      * Sets the mesh attribute of the class.
      */
    NumA::Mesh setMesh();

    /**
      * Computation of odd D-term from numerical GPD from the soft-pion theorem.
      * 
      * Returns Dminus as a interpolating object
      */
    std::vector<double> computeDminus( NumA::Mesh& mesh, const vector<double>& DD, vector<double>& x );

    /**
      * Computation of even D-term from numerical GPD from the soft-pion theorem.
      * 
      * Returns Dplus as a interpolating object
      */
    std::vector<double> computeDplus( NumA::Mesh& mesh, const vector<double>& DD, vector<double>& x );

    /**
      * Computation of D-terms.  
      * 
      * Updates DtermsVec.
      */
    std::vector<std::vector<double>> computeDterms();

private:

    double c;                                   // t-dependece of the GPD model.
    double cM;                                  // Auxilary t-dependece for the computation of the singlet distribution.
    double c1;                                  // Auxiliary t-dependence for the \xi->1 limit.
    double m2;                                  // Squared pion mass scale.

    double dplus;                               // Even D-term.
    double dminus;                              // Odd D-term.
    double dt;                                  // D-term t-dependence parametrized as a monopole: 1//(1-t/4m2)
    double alpha;                               // Kinematic variable for D-terms: \alpha = m_x/m_xi
    double m2D;                                 // D-term t-dependence (mass-scale). Fitting of Phys. Rev. D 97, 014020 (2018) gravitational FFs.

    // TODO: Implement computation of D-terms in RT.
    std::vector<std::vector<double>> DtermsVec; // Matrix containing the numerical evaluation for the D-terms: DtermsVec[0]: Dminus (Odd D-term)
                                                //                                                             DtermsVec[1]: Dplus (even D-term).
                                                //                                                             DtermsVec[2]: x: Evaluation points of the D-terms.

    NumA::CubicSpline* Dplus;                   // Interpolator for even D-term.
    NumA::CubicSpline* Dminus;                  // Interpolator for odd D-term.

    NumA::Mesh mesh;                            // Mesh over the double distribution domain.

    vector<double> DD;                          // Double distribution.
    vector<double> DDt0;                        // Double distribution at zero momentum transfer (it is necessary for the computation of D-terms.)
    NumA::MatrixD RTmatrix;                     // Radon transform matrix for uVal.

};


} //end of namespace

# endif