#define BOOST_TEST_MODULE PresentationContextAC
#include <boost/test/unit_test.hpp>

#include <sstream>
#include <string>
#include <vector>

#include "odil/Exception.h"
#include "odil/dul/PresentationContextAC.h"

std::string const data(
    "\x21\x00\x00\x17"
    "\x03\x00\x01\x00"
    "\x40\x00\x00\x0f""transfer_syntax",
    27
);

BOOST_AUTO_TEST_CASE(Constructor)
{
    odil::dul::PresentationContextAC const context(1, "transfer_syntax", 2);

    BOOST_REQUIRE_EQUAL(context.get_item_type(), 0x21);
    BOOST_REQUIRE_EQUAL(context.get_id(), 0x1);
    BOOST_REQUIRE_EQUAL(context.get_result_reason(), 0x2);
    BOOST_REQUIRE_EQUAL(context.get_transfer_syntax(), "transfer_syntax");
}

BOOST_AUTO_TEST_CASE(Id)
{
    odil::dul::PresentationContextAC context(1, "transfer_syntax", 1);
    context.set_id(123);
    BOOST_REQUIRE_EQUAL(context.get_id(), 123);
}

BOOST_AUTO_TEST_CASE(TransferSyntax)
{
    odil::dul::PresentationContextAC context(1, "transfer_syntax", 1);
    context.set_transfer_syntax("foo");
    BOOST_REQUIRE_EQUAL(context.get_transfer_syntax(), "foo");
}

BOOST_AUTO_TEST_CASE(Write)
{
    odil::dul::PresentationContextAC context(3, "transfer_syntax", 1);

    std::ostringstream stream;
    stream << context;

    BOOST_REQUIRE_EQUAL(stream.str(), data);
}

BOOST_AUTO_TEST_CASE(Read)
{
    std::istringstream stream(data);

    odil::dul::PresentationContextAC const context(stream);

    BOOST_REQUIRE_EQUAL(context.get_item_type(), 0x21);
    BOOST_REQUIRE_EQUAL(context.get_id(), 3);
    BOOST_REQUIRE_EQUAL(context.get_result_reason(), 1);
    BOOST_REQUIRE_EQUAL(context.get_transfer_syntax(), "transfer_syntax");
}
