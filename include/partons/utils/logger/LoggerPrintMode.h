#ifndef LOGGER_PRINT_MODE_H
#define LOGGER_PRINT_MODE_H

/**
 * @file LoggerPrintMode.h
 * @author Bryan BERTHOU (CEA Saclay)
 * @date 05 September 2014
 * @version 1.0
 *
 * @struct LoggerPrintMode
 * @brief
 */

#include <string>

#include "../stringUtils/StringUtils.h"

class LoggerPrintMode {
public:
    enum Type {
        COUT, FILE, BOTH, DEFAULT
    };

    LoggerPrintMode(Type type)
            : m_type(type) {
    }

    std::string toString() {
        switch (m_type) {
        case COUT:
            return "COUT";
            break;
        case FILE:
            return "FILE";
            break;
        case BOTH:
            return "BOTH";
            break;
        default:
            return "DEFAULT";
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

    LoggerPrintMode::Type getType() const {
        return m_type;
    }

    void setType(Type type) {
        m_type = type;
    }

private:
    LoggerPrintMode::Type m_type;
};

#endif /* LOGGER_PRINT_MODE_H */
