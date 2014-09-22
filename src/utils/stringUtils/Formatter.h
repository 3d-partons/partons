#ifndef FORMATTER_H
#define FORMATTER_H

/**
 * @file Formatter.h
 * @author http://stackoverflow.com/questions/12261915/howto-throw-stdexceptions-with-variable-messages/12262626#12262626
 * @date 16 September 2014
 *
 * @class Formatter
 */

#include <sstream>
#include <string>

class Formatter {
public:
    Formatter() {
    }

    ~Formatter() {
    }

    template<typename Type>
    Formatter & operator <<(const Type & value) {
        stream_ << value;
        return *this;
    }

    std::string str() const {
        return stream_.str();
    }

    operator std::string() const {
        return stream_.str();
    }

    enum ConvertToString {
        to_str
    };

    std::string operator >>(ConvertToString) {
        return stream_.str();
    }

    void print();

private:
    std::stringstream stream_;

    Formatter(const Formatter &);
    Formatter & operator =(Formatter &);
};

#endif /* FORMATTER_H */
