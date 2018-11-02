/*************************************************************************
 * odil - Copyright (C) Universite de Strasbourg
 * Distributed under the terms of the CeCILL-B license, as published by
 * the CEA-CNRS-INRIA. Refer to the LICENSE file or to
 * http://www.cecill.info/licences/Licence_CeCILL-B_V1-en.html
 * for details.
 ************************************************************************/

#include "odil/webservices/STOWRSResponse.h"

#include <array>
#include <sstream>

#include <json/json.h>

#include <boost/lexical_cast.hpp>
#include <boost/property_tree/xml_parser.hpp>
#include <boost/version.hpp>

#include "odil/DataSet.h"
#include "odil/json_converter.h"
#include "odil/StringStream.h"
#include "odil/webservices/HTTPResponse.h"
#include "odil/webservices/multipart_related.h"
#include "odil/webservices/Utils.h"
#include "odil/xml_converter.h"

namespace odil
{

namespace webservices
{

STOWRSResponse
::STOWRSResponse()
: _store_instance_responses(std::make_shared<DataSet>()),
  _representation(Representation::DICOM_XML), _reason(""), _warning(false),
  _failure_code(0), _media_type("")
{
    // Nothing else.
}


STOWRSResponse
::STOWRSResponse(HTTPResponse const &response)
: _store_instance_responses(std::make_shared<DataSet>()),
  _representation(Representation::DICOM_XML), _reason(""), _warning(false),
  _failure_code(0), _media_type("")
{
    if(response.get_status() == 200)
    {
        this->set_reason("OK");
        // query is ok
    }
    else if(response.get_status() == 202)
    {
        this->set_reason("Accepted");
        this->_warning = true;
        // The Service stored some of the instances but warnings or failures exist for the others
    }
    else if(
        response.get_status() == 400 || response.get_status() == 401
        || response.get_status() == 403 || response.get_status() == 409
        || response.get_status() == 415 || response.get_status() == 503)
    {
        // Failure found
        this->set_failure_code(response.get_status());
        this->set_reason(response.get_reason());
        return;
    }
    else
    {
        throw Exception("Invalid status : "+ std::to_string(response.get_status()));
    }

    // Find the media type.
    auto const & content_type = as<ItemWithParameters>(
        response.get_header("Content-Type"));
    this->_media_type = content_type.name;

    if(content_type.name == "application/dicom+xml")
    {
        this->_representation = Representation::DICOM_XML;
    }
    else if (content_type.name == "application/dicom+json")
    {
        this->_representation = Representation::DICOM_JSON;
    }
    else
    {
        throw Exception("Unrecognized response representation");
    }

    if(this->_media_type == "application/dicom+xml")
    {
        IStringStream stream(
            &response.get_body()[0], response.get_body().size());
        boost::property_tree::ptree xml;
        boost::property_tree::read_xml(stream, xml);
        this->_store_instance_responses = as_dataset(xml);
    }
    else // if (this->_media_type == "application/dicom+json")
    {
        IStringStream stream(
            &response.get_body()[0], response.get_body().size());
        Json::Value array;
        stream >> array;
        if (!array.isArray())
        {
            throw Exception("Body must be an array");
        }
        this->_store_instance_responses  = as_dataset(array);
    }
}


bool
STOWRSResponse
::operator==(STOWRSResponse const & other) const
{
    return (
        this->_failure_code == other._failure_code
        && this->_media_type == other._media_type
        && this->_reason == other._reason
        && this->_representation == other._representation
        && this->_warning == other._warning
        && *this->_store_instance_responses == *other._store_instance_responses);
}

bool
STOWRSResponse
::operator!=(STOWRSResponse const & other) const
{
    return !(*this == other);
}

void
STOWRSResponse
::set_store_instance_responses(std::shared_ptr<DataSet> responses)
{
    this->_store_instance_responses = responses;
}

std::shared_ptr<DataSet const>
STOWRSResponse
::get_store_instance_responses() const
{
    return this->_store_instance_responses;
}

std::string const &
STOWRSResponse
::get_media_type() const
{
    return this->_media_type;
}

void
STOWRSResponse
::set_representation(Representation const &representation)
{
    this->_representation = representation;
    if(representation == Representation::DICOM_JSON)
    {
        this->_media_type = "application/dicom+json";
    }
    else if(representation == Representation::DICOM_XML)
    {
        this->_media_type = "application/dicom+xml";
    }
    else
    {
        throw Exception("Invalid representation");
    }
}

Representation const &
STOWRSResponse
::get_representation() const
{
    return this->_representation;
}

void
STOWRSResponse
::set_warning(bool warn)
{
    this->_warning = warn;
}

bool
STOWRSResponse
::is_status_warning() const
{
    return this->_warning;
}

void
STOWRSResponse
::set_failure_code(unsigned int failure_code)
{
    std::array<int,6> const failures = {400, 401, 403, 409, 415, 503};
    if(std::find(failures.begin(), failures.end(), failure_code) != failures.end())
    {
        this->_failure_code = failure_code;
    }
    else
    {
        throw Exception("Unknown reason");
    }
}

unsigned int
STOWRSResponse
::get_failure_code() const
{
    return this->_failure_code;
}

void
STOWRSResponse
::set_reason(std::string reason)
{
    std::array<std::string,8> const reasons = {
        "OK", "Bad Request", "Unauthorized", "Forbidden", "Conflict",
        "Unsupported Media Type", "Busy", "Accepted"};
    if (std::find(reasons.begin(), reasons.end(), reason) != reasons.end())
    {
        this->_reason = reason;
    }
    else
    {
        throw Exception("Unknown reason");
    }
}

std::string const &
STOWRSResponse
::get_reason() const
{
    return this->_reason;
}

HTTPResponse
STOWRSResponse
::get_http_response() const
{
    HTTPResponse response;
    response.set_status(
        this->_failure_code==0
        ?(this->is_status_warning()?202:200)
        :this->_failure_code);
    response.set_reason(this->_reason);

    if(this->_representation == Representation::DICOM_XML)
    {
        auto const xml = as_xml(this->get_store_instance_responses());
        std::ostringstream body;
#if BOOST_VERSION >= 105600
        typedef boost::property_tree::xml_writer_settings<std::string> SettingsType;
#else
        typedef boost::property_tree::xml_writer_settings<char> SettingsType;
#endif
        boost::property_tree::write_xml(body, xml, SettingsType());
        response.set_body(body.str());

        response.set_header("Content-Type","application/dicom+xml");
    }
    else if(this->_representation == Representation::DICOM_JSON)
    {
        Json::Value json;
        json.resize(1);
        json[0] = as_json(this->get_store_instance_responses());

        Json::FastWriter writer;
        response.set_body(writer.write(json));

        response.set_header("Content-Type","application/dicom+json");
    }
    else
    {
        throw Exception("Unknown representation");
    }
    return response;
}


}


}
