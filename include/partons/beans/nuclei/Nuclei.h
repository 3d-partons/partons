/*
 * Nuclei.h
 *
 *  Created on: Mar 11, 2020
 *      Author: partons
 */

#ifndef INCLUDE_PARTONS_BEANS_NUCLEI_NUCLEI_H_
#define INCLUDE_PARTONS_BEANS_NUCLEI_NUCLEI_H_

#include <ElementaryUtils/parameters/Parameters.h>
#include <stddef.h>
#include <string>

namespace PARTONS {

class Nuclei {

public:

    static const std::string PARAMETER_NAME_SET_AZ; ///< Name of parameter to set A and Z numbers (should be separated by the pipe symbol).

    /**
     * Destructor.
     */
    virtual ~Nuclei();

    /**
     * Configure.
     * @param parameters Parameters to be set.
     */
    virtual void configure(const ElemUtils::Parameters &parameters);

    /**
     * Get mass number.
     */
    size_t getA() const;

    /**
     * Set mass number.
     */
    void setA(size_t A);

    /**
     * Get atomic number.
     */
    size_t getZ() const;

    /**
     * Set atomic number.
     */
    void setZ(size_t Z);

    /**
     * Get neutron number.
     */
    size_t getN() const;

    /**
     * Set neutron number.
     */
    void setN(size_t N);

protected:

    /**
     * Default constructor.
     */
    Nuclei(const std::string& className);

    /**
     * Assignment constructor.
     */
    Nuclei(const std::string& className, size_t A, size_t Z);

    /**
     * Copy constructor.
     */
    Nuclei(const Nuclei &other);

    size_t m_A; ///< Mass number.
    size_t m_Z; ///< Atomic number.

private:

    /**
     * Check if is well configured.
     */
    void isWellConfigured() const;

    std::string m_className; ///< Class name.
};

} /* namespace PARTONS */

#endif /* INCLUDE_PARTONS_BEANS_NUCLEI_NUCLEI_H_ */
