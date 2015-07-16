#define BOOST_TEST_MODULE ElementAccessor
#include <boost/test/unit_test.hpp>

#include <dcmtk/config/osconfig.h>
#include <dcmtk/dcmdata/dcdatset.h>
#include <dcmtk/dcmdata/dcdeftag.h>
#include <dcmtk/dcmnet/dimse.h>

#include "dcmtkpp/ElementAccessor.h"
#include "dcmtkpp/VRTraits.h"

struct Fixture
{
    DcmDataset dataset;

    Fixture()
    {
        OFCondition condition;
        condition = dataset.putAndInsertOFStringArray(DCM_PatientID, "DJ123");
        if(condition.bad())
        {
            throw dcmtkpp::Exception(condition);
        }

        condition = dataset.putAndInsertUint16(DCM_MessageID, 1234);
        if(condition.bad())
        {
            throw dcmtkpp::Exception(condition);
        }
    }
};

BOOST_FIXTURE_TEST_CASE(GetCorrectTypeString, Fixture)
{
    BOOST_CHECK_EQUAL(
        dcmtkpp::ElementAccessor<std::string>::get(this->dataset, DCM_PatientID),
        "DJ123");
}

BOOST_FIXTURE_TEST_CASE(GetCorrectVRString, Fixture)
{
    BOOST_CHECK_EQUAL(
        dcmtkpp::ElementAccessor<
            typename dcmtkpp::VRTraits<EVR_LO>::ValueType
        >::get(this->dataset, DCM_PatientID),
        "DJ123");
}

BOOST_FIXTURE_TEST_CASE(GetCorrectTypeNonString, Fixture)
{
    BOOST_CHECK_EQUAL(
        dcmtkpp::ElementAccessor<Uint16>::get(this->dataset, DCM_MessageID),
        1234);
}

BOOST_FIXTURE_TEST_CASE(GetCorrectVRNonString, Fixture)
{
    BOOST_CHECK_EQUAL(
        dcmtkpp::ElementAccessor<
            typename dcmtkpp::VRTraits<EVR_US>::ValueType
        >::get(this->dataset, DCM_MessageID),
        1234);
}

BOOST_FIXTURE_TEST_CASE(GetWrongTypeString, Fixture)
{
    BOOST_CHECK_THROW(
        dcmtkpp::ElementAccessor<Float32>::get(this->dataset, DCM_PatientID),
        dcmtkpp::Exception);
}

BOOST_FIXTURE_TEST_CASE(GetWrongTypeNonString, Fixture)
{
    BOOST_CHECK_THROW(
        dcmtkpp::ElementAccessor<Float32>::get(this->dataset, DCM_MessageID),
        dcmtkpp::Exception);
}

BOOST_FIXTURE_TEST_CASE(SetEmpty, Fixture)
{
    dcmtkpp::ElementAccessor<std::string>::set(this->dataset, DCM_PatientName, "FOO");
    OFString value;
    OFCondition const condition =
        this->dataset.findAndGetOFString(DCM_PatientName, value);
    BOOST_REQUIRE(condition.good());
    BOOST_CHECK_EQUAL(std::string(value.c_str()), "FOO");
}

BOOST_FIXTURE_TEST_CASE(SetWrongTypeString, Fixture)
{
    BOOST_CHECK_THROW(
        dcmtkpp::ElementAccessor<Float32>::set(
            this->dataset, DCM_PatientName, 1.234),
        dcmtkpp::Exception);
}

BOOST_FIXTURE_TEST_CASE(SetWrongTypeNonString, Fixture)
{
    BOOST_CHECK_THROW(
        dcmtkpp::ElementAccessor<Float64>::set(
            this->dataset, DCM_Status, STATUS_Success),
        dcmtkpp::Exception);
}

BOOST_FIXTURE_TEST_CASE(SetExisting, Fixture)
{
    dcmtkpp::ElementAccessor<std::string>::set(this->dataset, DCM_PatientID, "FOO");
    OFString value;
    OFCondition const condition =
        this->dataset.findAndGetOFString(DCM_PatientID, value);
    BOOST_REQUIRE(condition.good());
    BOOST_CHECK_EQUAL(std::string(value.c_str()), "FOO");
}

BOOST_FIXTURE_TEST_CASE(Has, Fixture)
{
    BOOST_CHECK(
        dcmtkpp::ElementAccessor<std::string>::has(this->dataset, DCM_PatientID));
}

BOOST_FIXTURE_TEST_CASE(HasNot, Fixture)
{
    BOOST_CHECK(
        !dcmtkpp::ElementAccessor<std::string>::has(this->dataset, DCM_PatientName));
}
