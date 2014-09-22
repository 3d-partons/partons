#ifndef LOGGER_PRINT_MODE_H
#define LOGGER_PRINT_MODE_H

/**
 * @file LoggerPrintMode.h
 * @author Bryan BERTHOU (CEA Saclay)
 * @date 05 September 2014
 * @version 1.0
 *
 * Last update : 05 September 2014
 *
 * @struct LoggerPrintMode
 * @brief
 */

#include <string>

#include "../stringUtils/StringUtils.h"

struct LoggerPrintMode {
    //prevent automatic conversion for any other built-in types such as bool, int, etc
    template<typename T>
    operator T() const;

public:
    enum Type {
        COUT, FILE, BOTH, DEFAULT
    };
    Type t_;
    LoggerPrintMode(Type t) :
            t_(t) {
    }
    operator Type() const {
        return t_;
    }

    std::string toString() {
        switch (t_) {
        case COUT:
            return "COUT";
            break;
        case FILE:
            return "FILE";
            break;
        case BOTH:
            return "BOTH";
            break;
        case DEFAULT:
            return "DEFAULT";
            break;
        default:
            return "default";
        }
    }

    static LoggerPrintMode fromString(std::string printMode) {
        StringUtils::to_upperCase(printMode);
        if (printMode == "COUT") {
            return COUT;
        } else if (printMode == "FILE") {
            return FILE;
        } else if (printMode == "BOTH") {
            return BOTH;
        }

        return DEFAULT;
    }

};

#endif /* LOGGER_PRINT_MODE_H */
