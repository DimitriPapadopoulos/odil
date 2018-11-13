/*************************************************************************
 * odil - Copyright (C) Universite de Strasbourg
 * Distributed under the terms of the CeCILL-B license, as published by
 * the CEA-CNRS-INRIA. Refer to the LICENSE file or to
 * http://www.cecill.info/licences/Licence_CeCILL-B_V1-en.html
 * for details.
 ************************************************************************/

#include "odil/dul/AAssociateAC.h"

#include <algorithm>
#include <sstream>
#include <vector>

#include "odil/Exception.h"
#include "odil/dul/AAssociate.h"
#include "odil/dul/PresentationContextAC.h"

namespace odil
{

namespace dul
{

AAssociateAC
::AAssociateAC()
: AAssociate(type)
{
    // Nothing else
}

AAssociateAC
::AAssociateAC(std::istream & stream)
: AAssociate(type, stream)
{
    // Nothing else
}

AAssociateAC
::~AAssociateAC()
{
    // Nothing to do.
}

std::vector<PresentationContextAC>
AAssociateAC
::get_presentation_contexts() const
{
    std::vector<PresentationContextAC> result;
    for(auto const & item: this->_item.as_items("Variable-items"))
    {
        if(item.as_unsigned_int_8("Item-type") == PresentationContextAC::type)
        {
            std::stringstream stream;
            stream << item;
            result.push_back(PresentationContextAC(stream));
        }
    }

    return result;
}

void
AAssociateAC
::set_presentation_contexts(std::vector<PresentationContextAC> const & value)
{
    auto const & old_items = this->_item.as_items("Variable-items");
    std::vector<Item> new_items;

    std::copy_if(
        old_items.begin(), old_items.end(), std::back_inserter(new_items),
        [](Item const & item)  {
            return item.as_unsigned_int_8("Item-type") == ApplicationContext::type; });

    std::transform(
        value.begin(), value.end(), std::back_inserter(new_items),
        [](PresentationContext const & x) { return x.get_item(); });

    std::copy_if(
        old_items.begin(), old_items.end(), std::back_inserter(new_items),
        [](Item const & item) {
            return item.as_unsigned_int_8("Item-type") == UserInformation::type; });

    this->_item.as_items("Variable-items") = new_items;
    this->_set_pdu_length(this->_compute_length());
}

}

}