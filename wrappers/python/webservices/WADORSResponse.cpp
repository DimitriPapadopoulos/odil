/*************************************************************************
 * odil - Copyright (C) Universite de Strasbourg
 * Distributed under the terms of the CeCILL-B license, as published by
 * the CEA-CNRS-INRIA. Refer to the LICENSE file or to
 * http://www.cecill.info/licences/Licence_CeCILL-B_V1-en.html
 * for details.
 ************************************************************************/

#include <vector>

#include <pybind11/pybind11.h>
#include <pybind11/operators.h>
#include <pybind11/stl.h>

#include "odil/DataSet.h"
#include "odil/webservices/BulkData.h"
#include "odil/webservices/WADORSResponse.h"
#include "odil/Value.h"

void
set_data_sets(
    odil::webservices::WADORSResponse& self, pybind11::list data_sets)
{
    odil::Value::DataSets odil_ds;
    for(pybind11::size_t i = 0; i < pybind11::len(data_sets); ++i)
    {
        odil_ds.push_back(data_sets[i].cast<std::shared_ptr<odil::DataSet>>());
    }
    self.set_data_sets(odil_ds);
}

void
set_bulk_data(
    odil::webservices::WADORSResponse& self, pybind11::list bulk_data)
{
    std::vector<odil::webservices::BulkData> cpp_val;
    for (pybind11::size_t i = 0; i < pybind11::len(bulk_data); ++i)
    {
        cpp_val.push_back(bulk_data[i].cast<odil::webservices::BulkData>());
    }
    self.set_bulk_data(cpp_val);
}

void
wrap_webservices_WADORSResponse(pybind11::module & m)
{
    using namespace pybind11;
    using namespace odil;
    using namespace odil::webservices;

    class_<WADORSResponse>(m, "WADORSResponse")
        .def(init<>())
        .def(init<HTTPResponse>())
        .def(
            "get_data_sets",
            static_cast<
                Value::DataSets const &(WADORSResponse::*)() const
            >(&WADORSResponse::get_data_sets))
        .def("set_data_sets", &set_data_sets)
        .def(
            "get_bulk_data",
            static_cast<
                std::vector<BulkData> const &(WADORSResponse::*)() const
            >(&WADORSResponse::get_bulk_data))
        .def("set_bulk_data", set_bulk_data)
        .def("is_partial", &WADORSResponse::is_partial)
        .def("set_partial", &WADORSResponse::set_partial)
        .def("get_type", &WADORSResponse::get_type)
        .def("get_representation", &WADORSResponse::get_representation)
        .def("respond_dicom", &WADORSResponse::respond_dicom)
        .def("respond_bulk_data", &WADORSResponse::respond_bulk_data)
        .def("respond_pixel_data", &WADORSResponse::respond_pixel_data)
        .def("get_http_response", &WADORSResponse::get_http_response)
        .def(self == self)
        .def(self != self)
    ;
}
