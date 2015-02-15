#include "base_object.h"

ObjectContext BaseObjectWrapper::Functions[] = {
    ObjectContext("setV", &BaseObjectWrapper::setV),
    ObjectContext("getV", &BaseObjectWrapper::getV),
    ObjectContext("print", &BaseObjectWrapper::print)
};
