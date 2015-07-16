/*************************************************************************
 * dcmtkpp - Copyright (C) Universite de Strasbourg
 * Distributed under the terms of the CeCILL-B license, as published by
 * the CEA-CNRS-INRIA. Refer to the LICENSE file or to
 * http://www.cecill.info/licences/Licence_CeCILL-B_V1-en.html
 * for details.
 ************************************************************************/

#include "CFindRequest.h"

#include <string>

#include "dcmtkpp/Exception.h"
#include "dcmtkpp/registry.h"
#include "dcmtkpp/Request.h"
#include "dcmtkpp/Value.h"

namespace dcmtkpp
{

CFindRequest
::CFindRequest(
    Value::Integer message_id, Value::String const & affected_sop_class_uid,
    Value::Integer priority, DataSet const & dataset)
: Request(message_id)
{
    this->set_command_field(Command::C_FIND_RQ);
    this->set_affected_sop_class_uid(affected_sop_class_uid);
    this->set_priority(priority);
    if(dataset.empty())
    {
        throw Exception("Data set is required");
    }
    this->set_data_set(dataset);
}

CFindRequest
::CFindRequest(Message const & message)
: Request(message)
{
    if(message.get_command_field() != Command::C_FIND_RQ)
    {
        throw Exception("Message is not a C-FIND-RQ");
    }
    this->set_command_field(message.get_command_field());

    this->set_affected_sop_class_uid(
        message.get_command_set().as_string(registry::AffectedSOPClassUID, 0));

    this->set_priority(message.get_command_set().as_int(registry::Priority, 0));

    if(!message.has_data_set() || message.get_data_set().empty())
    {
        throw Exception("Data set is required");
    }
    this->set_data_set(message.get_data_set());
}

CFindRequest
::~CFindRequest()
{
    // Nothing to do.
}

}
