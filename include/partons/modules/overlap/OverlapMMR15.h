/**
 * @file OverlapMMR2015.h
 * @author Nabil Chouika (Irfu/SPhN, CEA Saclay)
 * @date 5 oct. 2016
 * @version 1.0
 */

#ifndef OVERLAPMMR2015_H_
#define OVERLAPMMR2015_H_

#include <string>

#include "IncompleteGPDModule.h"

/**
 * @class OverlapMMR2015
 * @brief Pion GPD from Cédric's thesis [arXiv:1602.07722]
 */

class OverlapMMR15: public IncompleteGPDModule {
public:

    static const unsigned int classId; ///< Unique ID to automatically register the class in the registry.

    /**
     * Default constructor
     */
    OverlapMMR15(const std::string &className);

    /**
     * Default destructor
     */
    virtual ~OverlapMMR15();

    virtual OverlapMMR15* clone() const;

    virtual void configure(const ElemUtils::Parameters &parameters);

    virtual std::string toString();

    virtual PartonDistribution computeH(); ///< Compute GPD H

    virtual bool isInKinematicRegion(double x, double xi); ///< Defines the limited kinematic region of the model

protected:
    /**
     * Copy constructor
     *
     * Use by the factory
     *
     * @param other
     */
    OverlapMMR15(const OverlapMMR15& other);

    virtual void isModuleWellConfigured();
    virtual void initModule();

};

#endif /* OVERLAPMMR2015_H_ */
