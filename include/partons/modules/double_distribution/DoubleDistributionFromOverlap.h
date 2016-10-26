/**
 * @file DoubleDistributionFromOverlap.h
 * @author Nabil Chouika (Irfu/SPhN, CEA Saclay)
 * @date 26 sept. 2016
 * @version 1.0
 */

#ifndef DOUBLEDISTRIBUTIONFROMOVERLAP_H_
#define DOUBLEDISTRIBUTIONFROMOVERLAP_H_

#include <string>

#include "../DoubleDistributionModule.h"

/**
 * @class DoubleDistributionFromOverlap
 * @brief 
 */

class DoubleDistributionFromOverlap: public DoubleDistributionModule {
public:
    static const unsigned int classId; ///< Unique ID to automatically register the class in the registry.

        DoubleDistributionFromOverlap(const std::string &className);
        virtual ~DoubleDistributionFromOverlap();

        virtual DoubleDistributionFromOverlap* clone() const;

        virtual void resolveObjectDependencies();

        virtual PartonDistribution computeF();
        virtual PartonDistribution computeG();
        virtual PartonDistribution computeK();

    protected:
        /**
         * Copy constructor
         *
         * @param other
         */
        DoubleDistributionFromOverlap(const DoubleDistributionFromOverlap &other);

        virtual void initModule();
        virtual void isModuleWellConfigured();

    private:
};

#endif /* DOUBLEDISTRIBUTIONFROMOVERLAP_H_ */
