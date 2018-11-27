/*************************************************************************
 * odil - Copyright (C) Universite de Strasbourg
 * Distributed under the terms of the CeCILL-B license, as published by
 * the CEA-CNRS-INRIA. Refer to the LICENSE file or to
 * http://www.cecill.info/licences/Licence_CeCILL-B_V1-en.html
 * for details.
 ************************************************************************/

#include "AssociationReceiver.h"

#include <functional>
#include <mutex>
#include <vector>

#include <boost/asio.hpp>
#include <boost/signals2.hpp>

#include "odil/dul/Connection.h"
#include "odil/dul/PDU.h"
#include "odil/odil.h"

namespace odil
{

AssociationReceiver
::AssociationReceiver(
    boost::asio::io_service & service, dul::Connection & connection, 
    std::function<void(dul::PDU::Pointer)> success_handler, 
    std::function<void(dul::PDU::Pointer, boost::system::error_code)> error_handler)
: success_handler(success_handler), error_handler(error_handler),
    _service(service), _connection(connection), _acceptor(nullptr)
{
    // Nothing else.
}

void
AssociationReceiver
::operator()(boost::asio::ip::tcp::endpoint & endpoint)
{
    if(this->_connections.empty())
    {
        this->_connections.emplace_back(
            this->_connection.a_associate.response.connect(
                boost::bind(&AssociationReceiver::_on_negotiated, this, _1)));
        this->_connections.emplace_back(
            this->_connection.a_abort.indication.connect(
                boost::bind(
                    &AssociationReceiver::_on_error, 
                    this, _1, boost::system::error_code())));
        this->_connections.emplace_back(
            this->_connection.transport_closed.indication.connect(
                boost::bind(
                    &AssociationReceiver::_on_error, 
                    this, nullptr,
                    boost::system::errc::make_error_code(
                        boost::system::errc::connection_reset))));
        this->_connections.emplace_back(
            this->_connection.transport_error.indication.connect(
                boost::bind(&AssociationReceiver::_on_error, this, nullptr, _1)));
    }

    this->_acceptor = std::make_shared<boost::asio::ip::tcp::acceptor>(
        this->_service, endpoint);
    this->_acceptor->async_accept(
        this->_connection.socket,
        [&](boost::system::error_code error) {
            this->_connection.transport_connection.indication(error);
        }
    );
}

void
AssociationReceiver
::_on_negotiated(dul::PDU::Pointer pdu)
{
    std::lock_guard<std::mutex> lock(this->_mutex);
    if(this->_connections.empty())
    {
        return;
    }

    for(auto & connection: this->_connections)
    {
        connection.disconnect();
    }
    this->_connections.clear();

    this->success_handler(pdu);
}

void
AssociationReceiver
::_on_error(dul::PDU::Pointer pdu, boost::system::error_code error)
{
    std::lock_guard<std::mutex> lock(this->_mutex);
    if(this->_connections.empty())
    {
        return;
    }

    for(auto & connection: this->_connections)
    {
        connection.disconnect();
    }
    this->_connections.clear();
    
    this->error_handler(pdu, error);
}

}