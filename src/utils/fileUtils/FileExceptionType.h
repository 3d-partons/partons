#ifndef FILE_EXCEPTION_TYPE_H
#define FILE_EXCEPTION_TYPE_H

/**
 * @file FileExceptionType.h
 * @author Bryan BERTHOU (CEA Saclay)
 * @date 15 September 2014
 * @version 1.0
 *
 * Last update : 15 September 2014
 *
 * @struct FileExceptionType
 * @brief
 */

#include <string>

#include "../logger/LoggerPrintMode.h"

struct FileExceptionType {
    //prevent automatic conversion for any other built-in types such as bool, int, etc
    template<typename T>
    operator T() const;

public:
    enum Type {
        DEFAULT, UNREADABLE
    };
    Type t_;
    FileExceptionType(Type t) :
            t_(t) {
    }
    operator Type() const {
        return t_;
    }

    std::string toString() {
        switch (t_) {
        case UNREADABLE:
            return "UNREADABLE";
            break;
        default:
            return "default";
        }
    }

    static FileExceptionType fromString(const std::string &type) {
        if (type == "UNREADABLE") {
            return UNREADABLE;
        }

        return DEFAULT;
    }
};

#endif /* FILE_EXCEPTION_TYPE_H */
