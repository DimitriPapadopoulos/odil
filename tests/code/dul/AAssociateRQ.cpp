#define BOOST_TEST_MODULE AAssociateRQ
#include <boost/test/unit_test.hpp>

#include <iomanip>
#include <sstream>
#include <string>
#include <vector>

#include "odil/dul/AAssociateRQ.h"
#include "odil/dul/ApplicationContext.h"
#include "odil/dul/PresentationContextRQ.h"
#include "odil/dul/UserIdentityRQ.h"
#include "odil/dul/UserInformation.h"
#include "odil/Exception.h"

struct Fixture
{
public:
    static std::string const read_data;
    static std::string const write_data;

    odil::dul::ApplicationContext application_context;
    std::vector<odil::dul::PresentationContextRQ> presentation_contexts;
    odil::dul::UserInformation user_information;

    Fixture()
    : application_context("foo")
    {
        odil::dul::PresentationContextRQ pc1(3, "abstract_syntax", {"ts1", "ts2"});
        this->presentation_contexts = {pc1};

        this->user_information.set_sub_items<odil::dul::MaximumLength>(
            { { 0x12345678 } });
        odil::dul::UserIdentityRQ user_identity;
        user_identity.set_username_and_passcode("foo", "bar");
        user_identity.set_positive_response_requested(true);
        this->user_information.set_sub_items<odil::dul::UserIdentityRQ>(
            { user_identity });
    }

    void check_application_context(
        odil::dul::ApplicationContext const & context) const
    {
        BOOST_REQUIRE_EQUAL(
            context.get_name(), this->application_context.get_name());
    }

    void check_presentation_contexts(
        std::vector<odil::dul::PresentationContextRQ> const & contexts)
    {
        BOOST_REQUIRE_EQUAL(contexts.size(), presentation_contexts.size());
        for(int i=0; i<contexts.size(); ++i)
        {
            BOOST_REQUIRE_EQUAL(
                contexts[i].get_item_type(),
                presentation_contexts[i].get_item_type());
            BOOST_REQUIRE_EQUAL(
                contexts[i].get_id(),
                presentation_contexts[i].get_id());
            BOOST_REQUIRE_EQUAL(
                contexts[i].get_abstract_syntax(),
                presentation_contexts[i].get_abstract_syntax());
        }
    }

    void check_user_information(
        odil::dul::UserInformation const & user_information) const
    {
        BOOST_REQUIRE(
            !user_information.get_sub_items<odil::dul::MaximumLength>().empty());
        BOOST_REQUIRE_EQUAL(
            user_information.get_sub_items<odil::dul::MaximumLength>()[0].get_maximum_length(),
            0x12345678);

        BOOST_REQUIRE(
            !user_information.get_sub_items<odil::dul::UserIdentityRQ>().empty());
        BOOST_REQUIRE_EQUAL(
            user_information.get_sub_items<odil::dul::UserIdentityRQ>()[0].get_type(),
            2);
        BOOST_REQUIRE_EQUAL(
            user_information.get_sub_items<odil::dul::UserIdentityRQ>()[0].get_positive_response_requested(),
            true);
        BOOST_REQUIRE_EQUAL(
            user_information.get_sub_items<odil::dul::UserIdentityRQ>()[0].get_primary_field(),
            "foo");
        BOOST_REQUIRE_EQUAL(
            user_information.get_sub_items<odil::dul::UserIdentityRQ>()[0].get_secondary_field(),
            "bar");
    }
};

std::string const
Fixture
::read_data(
    // Header, 6 bytes

    "\x01\x00"
    "\x00\x00\x00\x90"

    // Items: 68 bytes

    "\x00\x01\x00\x00"
    "       CALLED_AE"
    "CALLING_AE      "
    "\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00"
    "\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00"

    // Application Context, 7 bytes

    "\x10\x00\x00\x03"
    "foo"

    // Presentation Contexts, 41 bytes

    "\x20\x00\x00\x25"
    "\x03\x00\x00\x00"
    "\x30\x00\x00\x0f""abstract_syntax"
    "\x40\x00\x00\x03""ts1"
    "\x40\x00\x00\x03""ts2"

    // User Information, 28 bytes

    "\x50\x00\x00\x18"
    "\x51\x00\x00\x04"
    "\x12\x34\x56\x78"
    "\x58\x00\x00\x0c"
    "\x02\x01"
    "\x00\x03" "foo"
    "\x00\x03" "bar"
    ,
    150
);

std::string const
Fixture
::write_data(
    // Header, 6 bytes

    "\x01\x00"
    "\x00\x00\x00\x90"

    // Items: 68 bytes

    "\x00\x01\x00\x00"
    "CALLED_AE       "
    "CALLING_AE      "
    "\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00"
    "\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00"

    // Application Context, 7 bytes

    "\x10\x00\x00\x03"
    "foo"

    // Presentation Contexts, 41 bytes

    "\x20\x00\x00\x25"
    "\x03\x00\x00\x00"
    "\x30\x00\x00\x0f""abstract_syntax"
    "\x40\x00\x00\x03""ts1"
    "\x40\x00\x00\x03""ts2"

    // User Information, 28 bytes

    "\x50\x00\x00\x18"
    "\x51\x00\x00\x04"
    "\x12\x34\x56\x78"
    "\x58\x00\x00\x0c"
    "\x02\x01"
    "\x00\x03" "foo"
    "\x00\x03" "bar"
    ,
    150
);

BOOST_AUTO_TEST_CASE(Constructor)
{
    odil::dul::AAssociateRQ const pdu;

    BOOST_REQUIRE_EQUAL(pdu.get_called_ae_title(), "");
    BOOST_REQUIRE_EQUAL(pdu.get_calling_ae_title(), "");
    BOOST_REQUIRE_EQUAL(pdu.get_protocol_version(), 0);
    BOOST_REQUIRE_THROW(pdu.get_application_context(), odil::Exception);
    BOOST_REQUIRE(pdu.get_presentation_contexts().empty());
    BOOST_REQUIRE_THROW(pdu.get_user_information(), odil::Exception);
}

BOOST_FIXTURE_TEST_CASE(ConstructorStream, Fixture)
{
    std::istringstream stream(read_data);
    odil::dul::AAssociateRQ const pdu(stream);

    BOOST_REQUIRE_EQUAL(pdu.get_called_ae_title(), "CALLED_AE");
    BOOST_REQUIRE_EQUAL(pdu.get_calling_ae_title(), "CALLING_AE");
    BOOST_REQUIRE_EQUAL(pdu.get_protocol_version(), 1);
    this->check_application_context(pdu.get_application_context());
    this->check_presentation_contexts(pdu.get_presentation_contexts());
    this->check_user_information(pdu.get_user_information());
}

BOOST_AUTO_TEST_CASE(ProtocolVersion)
{
    odil::dul::AAssociateRQ pdu;
    BOOST_REQUIRE_EQUAL(pdu.get_protocol_version(), 0);
    pdu.set_protocol_version(2);
    BOOST_REQUIRE_EQUAL(pdu.get_protocol_version(), 2);
}

BOOST_AUTO_TEST_CASE(CalledAETitle)
{
    odil::dul::AAssociateRQ pdu;
    BOOST_REQUIRE_EQUAL(pdu.get_called_ae_title(), "");
    pdu.set_called_ae_title("called");
    BOOST_REQUIRE_EQUAL(pdu.get_called_ae_title(), "called");
}

BOOST_AUTO_TEST_CASE(CallingAETitle)
{
    odil::dul::AAssociateRQ pdu;
    BOOST_REQUIRE_EQUAL(pdu.get_calling_ae_title(), "");
    pdu.set_calling_ae_title("calling");
    BOOST_REQUIRE_EQUAL(pdu.get_calling_ae_title(), "calling");
}

BOOST_FIXTURE_TEST_CASE(ApplicationContext, Fixture)
{
    odil::dul::AAssociateRQ pdu;
    BOOST_REQUIRE_THROW(pdu.get_application_context(), odil::Exception);
    pdu.set_application_context(application_context);
    this->check_application_context(pdu.get_application_context());
}

BOOST_FIXTURE_TEST_CASE(PresentationContexts, Fixture)
{
    odil::dul::AAssociateRQ pdu;
    BOOST_REQUIRE(pdu.get_presentation_contexts().empty());
    pdu.set_presentation_contexts(presentation_contexts);
    this->check_presentation_contexts(pdu.get_presentation_contexts());
}

BOOST_FIXTURE_TEST_CASE(UserInformation, Fixture)
{
    odil::dul::AAssociateRQ pdu;
    BOOST_REQUIRE_THROW(pdu.get_user_information(), odil::Exception);
    pdu.set_user_information(user_information);
    this->check_user_information(pdu.get_user_information());
}

BOOST_FIXTURE_TEST_CASE(Write, Fixture)
{
    odil::dul::AAssociateRQ pdu;
    pdu.set_protocol_version(1);
    pdu.set_called_ae_title("CALLED_AE");
    pdu.set_calling_ae_title("CALLING_AE");

    pdu.set_application_context(application_context);
    pdu.set_presentation_contexts(presentation_contexts);
    pdu.set_user_information(user_information);

    std::ostringstream stream;
    stream << pdu;

    BOOST_REQUIRE(stream.str() == write_data);
}

BOOST_AUTO_TEST_CASE(CalledAETitleEmpty)
{
    odil::dul::AAssociateRQ pdu;
    BOOST_REQUIRE_THROW(pdu.set_called_ae_title(""), odil::Exception);
}

BOOST_AUTO_TEST_CASE(CalledAETitleTooLong)
{
    odil::dul::AAssociateRQ pdu;
    BOOST_REQUIRE_THROW(
        pdu.set_called_ae_title("123456789abcdef01"), odil::Exception);
}

BOOST_AUTO_TEST_CASE(CallingAETitleEmpty)
{
    odil::dul::AAssociateRQ pdu;
    BOOST_REQUIRE_THROW(pdu.set_calling_ae_title(""), odil::Exception);
}

BOOST_AUTO_TEST_CASE(CallingAETitleTooLong)
{
    odil::dul::AAssociateRQ pdu;
    BOOST_REQUIRE_THROW(
        pdu.set_calling_ae_title("123456789abcdef01"), odil::Exception);
}
