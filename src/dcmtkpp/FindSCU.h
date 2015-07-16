/*************************************************************************
 * dcmtkpp - Copyright (C) Universite de Strasbourg
 * Distributed under the terms of the CeCILL-B license, as published by
 * the CEA-CNRS-INRIA. Refer to the LICENSE file or to
 * http://www.cecill.info/licences/Licence_CeCILL-B_V1-en.html
 * for details.
 ************************************************************************/

#ifndef _0106eb3a_4e02_4d7c_93bf_4d53dcafbb05
#define _0106eb3a_4e02_4d7c_93bf_4d53dcafbb05

#include <vector>

#include "dcmtkpp/DataSet.h"
#include "dcmtkpp/SCU.h"

namespace dcmtkpp
{

/// @brief SCU for C-FIND services.
class FindSCU: public SCU
{
public:
    /// @brief Callback called when a response is received.
    typedef std::function<void(DataSet const &)> Callback;
    
    /// @brief Destructor.
    virtual ~FindSCU();
    
    /// @brief Perform the C-FIND using an optional callback.
    void find(DataSet const & query, Callback callback) const;
    
    /**
     * @brief Return a list of datasets matching the query. The user is 
     * responsible for the de-allocation of the matches.
     */
    std::vector<DataSet> find(DataSet const & query) const;
};

}

#endif // _0106eb3a_4e02_4d7c_93bf_4d53dcafbb05
