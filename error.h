#ifndef ERROR_H
#define ERROR_H

#include <QList>
#include <QVariant>

namespace Error {

enum Value {
    NoError, ErrorOptionRequireArgument
};

}

inline int error(Error::Value value, const QList<const QVariant&> &var) {
    switch(value) {
    case Error::NoError:
        return 0;
    case Error::ErrorOptionRequireArgument:
        std::cout << "Option " << var[0].toString().toStdString() << " requires an argument" << std::endl;
    }
}




#endif // ERROR_H
