/*************************************************************************
 * odil - Copyright (C) Universite de Strasbourg
 * Distributed under the terms of the CeCILL-B license, as published by
 * the CEA-CNRS-INRIA. Refer to the LICENSE file or to
 * http://www.cecill.info/licences/Licence_CeCILL-B_V1-en.html
 * for details.
 ************************************************************************/

#include <pybind11/pybind11.h>
#include <pybind11/operators.h>

#include "odil/DataSet.h"

#include "opaque_types.h"
#include "type_casters.h"

void wrap_DataSet(pybind11::module & m)
{
    using namespace pybind11;
    using namespace odil;

    class_<DataSet, std::shared_ptr<DataSet>>(m, "DataSet")
        .def(init<>())
        .def(init<std::string const &>())
        .def("add", (void (DataSet::*)(Tag const &, Element const &)) &DataSet::add)
        .def(
            "add", (void (DataSet::*)(Tag const &, VR)) &DataSet::add,
            "tag"_a, "vr"_a=VR::UNKNOWN)
        .def(
            "add",
            [](DataSet & data_set, Tag const & tag, sequence source, VR vr)
            {
                if(vr == VR::UNKNOWN)
                {
                    vr = as_vr(tag);
                }
                if(len(source) > 0)
                {
                    data_set.add(tag, {convert_sequence<Value>(source), vr});
                }
                else
                {
                    data_set.add(tag);
                }
            },
            "self"_a, "tag"_a, "vr"_a=VR::UNKNOWN
        )
        .def("remove", &DataSet::remove)
        .def("has", &DataSet::has)
        .def("empty", (bool (DataSet::*)() const) &DataSet::empty)
        .def("size", (std::size_t (DataSet::*)() const) &DataSet::size)
        .def("get_vr", &DataSet::get_vr)
        .def("empty", (bool (DataSet::*)(Tag const &) const) &DataSet::empty)
        .def("size", (std::size_t (DataSet::*)(Tag const &) const) &DataSet::size)
        .def("is_int", &DataSet::is_int)
        .def(
            "as_int", (Value::Integers & (DataSet::*)(Tag const &)) &DataSet::as_int,
            return_value_policy::reference_internal)
        .def("is_real", &DataSet::is_real)
        .def(
            "as_real", (Value::Reals & (DataSet::*)(Tag const &)) &DataSet::as_real,
            return_value_policy::reference_internal)
        .def("is_string", &DataSet::is_string)
        .def(
            "as_string", (Value::Strings & (DataSet::*)(Tag const &)) &DataSet::as_string,
            return_value_policy::reference_internal)
        .def("is_data_set", &DataSet::is_data_set)
        .def(
            "as_data_set", (Value::DataSets & (DataSet::*)(Tag const &)) &DataSet::as_data_set,
            return_value_policy::reference_internal)
        .def("is_binary", &DataSet::is_binary)
        .def(
            "as_binary", (Value::Binary & (DataSet::*)(Tag const &)) &DataSet::as_binary,
            return_value_policy::reference_internal)
        .def(self == self)
        .def(self != self)
        .def("clear", (void (DataSet::*)()) &DataSet::clear)
        .def("clear", (void (DataSet::*) (Tag const &)) &DataSet::clear)
        .def("get_transfer_syntax", &DataSet::get_transfer_syntax)
        .def("set_transfer_syntax", &DataSet::set_transfer_syntax)
        .def("__len__", (std::size_t (DataSet::*)() const) &DataSet::size)
        .def(
            "__getitem__",
            [](DataSet const & d, Tag const & t) { return d[t]; })
        .def(
            "__setitem__",
            [](DataSet & d, Tag const & t, Element const & e) { d[t] = e; })
        .def(
            "__iter__",
            [](DataSet const & d)
            {
                return make_key_iterator(d.begin(), d.end());
            },
            keep_alive<0, 1>())
        .def(
            "keys",
            [](DataSet const & data_set)
            {
                list tags;
                for(auto const & item: data_set)
                {
                    tags.append(item.first);
                }
                return tags;
            })
        .def(
            "values",
            [](DataSet const & data_set)
            {
                list tags;
                for(auto const & item: data_set)
                {
                    tags.append(&item.second);
                }
                return tags;
            })
        .def(
            "items",
            [](DataSet const & data_set)
            {
                list tags;
                for(auto const & item: data_set)
                {
                    tags.append(std::make_pair(item.first, &item.second));
                }
                return tags;
            })
    ;

}
