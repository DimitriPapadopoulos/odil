/*************************************************************************
 * dcmtkpp - Copyright (C) Universite de Strasbourg
 * Distributed under the terms of the CeCILL-B license, as published by
 * the CEA-CNRS-INRIA. Refer to the LICENSE file or to
 * http://www.cecill.info/licences/Licence_CeCILL-B_V1-en.html
 * for details.
 ************************************************************************/

#ifndef _f82a15e2_fd13_44b5_af7d_c6983494c9c6
#define _f82a15e2_fd13_44b5_af7d_c6983494c9c6

#include "SCU.h"

#include <vector>

#include "dcmtkpp/CGetResponse.h"
#include "dcmtkpp/CStoreRequest.h"
#include "dcmtkpp/DataSet.h"
#include "dcmtkpp/StoreSCP.h"

namespace dcmtkpp
{

/// @brief SCU for C-GET services.
class GetSCU: public SCU
{
public:
    /// @brief Callback called when a response is received.
    typedef StoreSCP::Callback Callback;
    
    /// @brief Destructor.
    virtual ~GetSCU();
    
    /// @brief Perform the C-GET using an optional callback.
    void get(DataSet const & query, Callback callback) const;
    
    /**
     * @brief Return a list of datasets matching the query.
     */
    std::vector<DataSet> get(DataSet const & query) const;

private:
    bool _get_response(CGetResponse const & response) const;
    void _store_request(CStoreRequest const & request, Callback callback) const;
};

}

#endif // _f82a15e2_fd13_44b5_af7d_c6983494c9c6
