/**
 * @file RandBFConstPW.h
 * @author Nabil Chouika (Irfu/SPhN, CEA Saclay)
 * @date 5 oct. 2016
 * @version 1.0
 */

#ifndef RANDBFCONSTPW_H_
#define RANDBFCONSTPW_H_

#include <cstddef>
#include <string>
#include <utility>
#include <vector>

#include "RadonInverseModule.h"

/**
 * @class RandBFConstPW
 * @brief Radon inversion with a brute-force randomly generated matrix and piece-wise constant basis functions.
 */

class RandBFConstPW: RadonInverseModule {
public:
    static const unsigned int classId; ///< Unique ID to automatically register the class in the registry.

    RandBFConstPW(const std::string &className);
    virtual ~RandBFConstPW();

    /**
     * Clone
     *
     * @return
     */
    virtual RandBFConstPW* clone() const;

    virtual void configure(const ElemUtils::Parameters &parameters);

    virtual void buildMatrix(size_t maxiter = 0);

    bool isAlphaEven() const;
    void setAlphaEven(bool alphaEven);
    bool isValence() const;
    void setValence(bool valence);
    bool isTriangular() const;
    void setTriangular(bool triangular);

protected:
    /**
     * Copy constructor
     */
    RandBFConstPW(const RandBFConstPW &other);

    virtual void initModule();
    virtual void isModuleWellConfigured();

    bool m_valence; ///< True if the GPD is valence.
    bool m_alphaEven; ///< True if the parity in alpha is constrained.
    bool m_triangular; ///< True if the mesh is triangular around beta=0.

    /**
     * Builds the mesh (cells) in the DD domain.
     */
    virtual void buildMesh();
    std::vector<double> m_nodes; ///< Nodes in the u and v directions.
    /**
     * Builds the indices used (or not), with respect to the constraints (parity, valence, etc).
     */
    virtual void buildIndices();
    std::vector<std::pair<size_t, size_t> > m_indicesUsed;
    std::vector<std::pair<size_t, size_t> > m_indicesSym;
    std::vector<std::pair<size_t, size_t> > m_indicesFixed;

    /**
     * Contribution to the GPD of a mesh cell indexed by (i,j). Can be either a triangle or a square.
     * @param x
     * @param xi
     * @param i
     * @param j
     * @return
     */
    virtual double GPDOfMeshElement(double x, double xi, size_t i, size_t j);

private:
};

#endif /* RANDBFCONSTPW_H_ */
