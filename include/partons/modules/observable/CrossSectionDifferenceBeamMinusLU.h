#ifndef CROSSSECTIONDIFFERENCEBEAMMINUSLU_H
#define CROSSSECTIONDIFFERENCEBEAMMINUSLU_H

#include <string>

#include "Observable.h"

class CrossSectionDifferenceBeamMinusLU: public Observable {

public:

    static const unsigned int classId;

    CrossSectionDifferenceBeamMinusLU(const std::string &className);
    virtual ~CrossSectionDifferenceBeamMinusLU();

    virtual CrossSectionDifferenceBeamMinusLU* clone() const;

    virtual double computePhiObservable(double phi);

protected:

    CrossSectionDifferenceBeamMinusLU(const CrossSectionDifferenceBeamMinusLU &other);

private:
};

#endif /* CROSSSECTIONDIFFERENCEBEAMMINUSLU_H */
