#ifndef CROSSSECTIONBEAMMINUSUU_H
#define CROSSSECTIONBEAMMINUSUU_H

#include <string>

#include "Observable.h"

class CrossSectionBeamMinusUU: public Observable {

public:

    static const unsigned int classId;

    CrossSectionBeamMinusUU(const std::string &className);
    virtual ~CrossSectionBeamMinusUU();

    virtual CrossSectionBeamMinusUU* clone() const;

    virtual double computePhiObservable(double phi);

protected:

    CrossSectionBeamMinusUU(const CrossSectionBeamMinusUU &other);

private:
};

#endif /* CROSSSECTIONBEAMMINUSUU_H */
