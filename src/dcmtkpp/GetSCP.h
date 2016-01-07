/*************************************************************************
 * dcmtkpp - Copyright (C) Universite de Strasbourg
 * Distributed under the terms of the CeCILL-B license, as published by
 * the CEA-CNRS-INRIA. Refer to the LICENSE file or to
 * http://www.cecill.info/licences/Licence_CeCILL-B_V1-en.html
 * for details.
 ************************************************************************/

#ifndef _2f0ad1fd_8779_4ab3_b7e8_6d37fdc0c018
#define _2f0ad1fd_8779_4ab3_b7e8_6d37fdc0c018

#include <memory>

#include "dcmtkpp/Association.h"
#include "dcmtkpp/SCP.h"
#include "dcmtkpp/message/Message.h"

namespace dcmtkpp
{

/// @brief SCP for C-Get services.
class GetSCP: public SCP
{
public:

    class DataSetGenerator: public SCP::DataSetGenerator
    {
    public:
        virtual unsigned int count() const =0;
    };

    /// @brief Constructor.
    GetSCP(Association & association);

    /// @brief Constructor.
    GetSCP(
        Association & association,
        std::shared_ptr<DataSetGenerator> const & generator);

    /// @brief Destructor.
    virtual ~GetSCP();

    /// @brief Return the generator.
    DataSetGenerator const & get_generator() const;

    /// @brief Set the generator.
    void set_generator(std::shared_ptr<DataSetGenerator> const & generator);

    /// @brief Process a C-Get request.
    virtual void operator()(message::Message const & message);

private:
    std::shared_ptr<DataSetGenerator> _generator;
};

}

#endif // _2f0ad1fd_8779_4ab3_b7e8_6d37fdc0c018
