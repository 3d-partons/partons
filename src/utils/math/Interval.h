#ifndef INTERVAL_H
#define INTERVAL_H

/**
 * @author Bryan BERTHOU (CEA Saclay)
 * @date 23/07/2014
 * @version 1.0
 */

template<typename T>
class Interval {
    T lowerBound;
    T upperBound;
    T step;

public:

    Interval(T lowerBound, T upperBound, T step = 1) :
            lowerBound(lowerBound), upperBound(upperBound), step(step) {
    }

    T getLowerBound() const {
        return lowerBound;
    }

    void setLowerBound(T lowerBound) {
        this->lowerBound = lowerBound;
    }

    T getStep() const {
        return step;
    }

    void setStep(T step) {
        this->step = step;
    }

    T getUpperBound() const {
        return upperBound;
    }

    void setUpperBound(T upperBound) {
        this->upperBound = upperBound;
    }
};

#endif /* INTERVAL_H */
