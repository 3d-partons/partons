#ifndef OBSERVABLE_CHANNEL_H
#define OBSERVABLE_CHANNEL_H

/**
 * @file ObservableChannel.h
 * @author: Bryan BERTHOU (SPhN / CEA Saclay)
 * @date November 26, 2014
 * @version 1.0
 */

#include <string>

/**
 * @class ObservableChannel
 *
 * @brief
 */
class ObservableChannel {
public:
    enum Type {
        UNDEFINED, DVCS, DVMP, TCS
    };

    ObservableChannel();

    ObservableChannel(Type type);

    virtual ~ObservableChannel();

    operator Type() const;

    std::string toString();

    std::string getShortName();

    ObservableChannel::Type getType() const;

    void setType(Type type);

private:
    ObservableChannel::Type m_type;
};

#endif /* OBSERVABLE_CHANNEL_H */
