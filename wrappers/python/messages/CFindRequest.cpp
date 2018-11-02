/*************************************************************************
 * odil - Copyright (C) Universite de Strasbourg
 * Distributed under the terms of the CeCILL-B license, as published by
 * the CEA-CNRS-INRIA. Refer to the LICENSE file or to
 * http://www.cecill.info/licences/Licence_CeCILL-B_V1-en.html
 * for details.
 ************************************************************************/

#include <pybind11/pybind11.h>

#include "odil/message/CFindRequest.h"

void wrap_CFindRequest(pybind11::module & m)
{
    using namespace pybind11;
    using namespace odil;
    using namespace odil::message;

    class_<CFindRequest, std::shared_ptr<CFindRequest>, Request>(m, "CFindRequest")
        .def(init<
                Value::Integer, Value::String const &, Value::Integer,
                std::shared_ptr<DataSet>
            >())
        .def(init<std::shared_ptr<Message>>())
        .def(
            "get_affected_sop_class_uid",
            &CFindRequest::get_affected_sop_class_uid,
            return_value_policy::copy)
        .def(
            "set_affected_sop_class_uid",
            &CFindRequest::set_affected_sop_class_uid)
        .def(
            "get_priority", &CFindRequest::get_priority,
            return_value_policy::copy)
        .def("set_priority", &CFindRequest::set_priority)
    ;
}
