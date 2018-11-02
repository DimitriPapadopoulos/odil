/*************************************************************************
 * odil - Copyright (C) Universite de Strasbourg
 * Distributed under the terms of the CeCILL-B license, as published by
 * the CEA-CNRS-INRIA. Refer to the LICENSE file or to
 * http://www.cecill.info/licences/Licence_CeCILL-B_V1-en.html
 * for details.
 ************************************************************************/

#ifndef _14da8fe6_71a3_4c0b_91bf_9c518393a641
#define _14da8fe6_71a3_4c0b_91bf_9c518393a641

#include <string>
#include <vector>

#include "odil/DataSet.h"
#include "odil/odil.h"
#include "odil/webservices/HTTPResponse.h"
#include "odil/webservices/Utils.h"

namespace odil
{

namespace webservices
{

/// @brief QIDO-RS response generator and parser.
class ODIL_API QIDORSResponse
{
public:
    /// @brief Constructor.
    QIDORSResponse();

    /// @brief Parse an HTTPResponse as a WADORSResponse.
    QIDORSResponse(HTTPResponse const & response);

    QIDORSResponse(QIDORSResponse const &) = default;
    QIDORSResponse(QIDORSResponse &&) = default;
    QIDORSResponse & operator=(QIDORSResponse const &) = default;
    QIDORSResponse & operator=(QIDORSResponse &&) = default;
    ~QIDORSResponse() = default;

    /// @brief Equality operator.
    bool operator==(QIDORSResponse const & other) const;

    /// @brief Difference operator.
    bool operator!=(QIDORSResponse const & other) const;

    /// @brief Get the response items
    Value::DataSets const & get_data_sets() const;

    /// @brief Modify the response items
    Value::DataSets & get_data_sets();

    /// @brief Set the response items
    void set_data_sets(Value::DataSets const & data_sets);

    /// @brief Return the representation
    Representation const & get_representation() const;

    /// @brief Set the representation
    void set_representation(Representation const & representation);

    /// @brief Return the media type
    std::string const & get_media_type() const;

    /// @brief Generate the associated HTTP response
    HTTPResponse get_http_response() const;

private:
    Value::DataSets _data_sets;
    Representation _representation;
    std::string _media_type;
};

}

}


#endif //_14da8fe6_71a3_4c0b_91bf_9c518393a641
