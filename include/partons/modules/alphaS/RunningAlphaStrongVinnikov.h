#ifndef RUNNING_ALPHA_STRONG_VINNIKOV_H
#define RUNNING_ALPHA_STRONG_VINNIKOV_H

#include <string>

#include "../RunningAlphaStrongModule.h"

class RunningAlphaStrongVinnikov: public RunningAlphaStrongModule {
public:

    static const unsigned int classId;

    RunningAlphaStrongVinnikov(const std::string &className);
    virtual RunningAlphaStrongVinnikov* clone() const;
    virtual ~RunningAlphaStrongVinnikov();
    virtual double compute();

protected:

    RunningAlphaStrongVinnikov(const RunningAlphaStrongVinnikov &other);

    virtual void initModule();
    virtual void isModuleWellConfigured();

private:

    double LOG_LAMBDA_5;
    double LOG_LAMBDA_4;
    double LOG_LAMBDA_3;
};

#endif /* RUNNING_ALPHA_STRONG_VINNIKOV_H */

