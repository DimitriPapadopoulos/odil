#define BOOST_TEST_MODULE Selector
#include <boost/test/unit_test.hpp>

#include <vector>

#include "odil/webservices/Selector.h"

BOOST_AUTO_TEST_CASE(Constructor_empty)
{
    odil::webservices::Selector const selector;
    BOOST_REQUIRE(!selector.is_study_present());
    BOOST_REQUIRE(!selector.is_series_present());
    BOOST_REQUIRE(!selector.is_instance_present());
}

BOOST_AUTO_TEST_CASE(Constructor)
{
    odil::webservices::Selector const selector(
                std::map<std::string, std::string>
                    ({{"studies", "1.2"},
                      {"series", "3.4"}})
                );
    BOOST_REQUIRE(selector.is_study_present());
    BOOST_REQUIRE_EQUAL(selector.get_study(), "1.2");

    BOOST_REQUIRE(selector.is_series_present());
    BOOST_REQUIRE_EQUAL(selector.get_series(), "3.4");

    BOOST_REQUIRE(!selector.is_instance_present());
    BOOST_REQUIRE(selector.get_frames().empty());
}

BOOST_AUTO_TEST_CASE(FromPath)
{
    std::string service;
    odil::webservices::Selector selector;
    std::tie(service, selector) = odil::webservices::Selector::from_path(
        "/dicom/studies/1.2/instances");

    BOOST_REQUIRE_EQUAL(service, "/dicom");

    BOOST_REQUIRE(selector.is_study_present());
    BOOST_REQUIRE_EQUAL(selector.get_study(), "1.2");

    BOOST_REQUIRE(!selector.is_series_present());

    BOOST_REQUIRE(selector.is_instance_present());
    BOOST_REQUIRE_EQUAL(selector.get_instance(), "");

    BOOST_REQUIRE(selector.get_frames().empty());
}

BOOST_AUTO_TEST_CASE(FromPathFrames)
{
    std::string service;
    odil::webservices::Selector selector;
    std::tie(service, selector) = odil::webservices::Selector::from_path(
        "/dicom/studies/1.2/instances/3.4/frames/1,34");

    BOOST_REQUIRE_EQUAL(service, "/dicom");

    BOOST_REQUIRE(selector.is_study_present());
    BOOST_REQUIRE_EQUAL(selector.get_study(), "1.2");

    BOOST_REQUIRE(!selector.is_series_present());

    BOOST_REQUIRE(selector.is_instance_present());
    BOOST_REQUIRE_EQUAL(selector.get_instance(), "3.4");

    std::vector<int> const expected_frames{1,34};
    BOOST_REQUIRE(selector.get_frames() == expected_frames);
}

BOOST_AUTO_TEST_CASE(Equal)
{
    odil::webservices::Selector selector;
    selector.set_study("1.2");
    odil::webservices::Selector const selector_(
        std::map<std::string, std::string>
            ({{"studies", "1.2"}}));
    BOOST_REQUIRE(selector == selector_);
}

BOOST_AUTO_TEST_CASE(Different)
{
    odil::webservices::Selector selector;
    selector.set_study("1.2").set_series("3.3");
    odil::webservices::Selector const selector_(
        std::map<std::string, std::string>(
            {{"studies", "1.2"},
            {"series", "3.4"}}));
    BOOST_REQUIRE(selector != selector_);
}

BOOST_AUTO_TEST_CASE(GetPath)
{
    odil::webservices::Selector selector;
    selector.set_study("1.2").set_series("3.4").set_instance("");
    BOOST_REQUIRE_EQUAL(selector.get_path(false), "/studies/1.2/series/3.4/instances");

    odil::webservices::Selector selector_1;
    selector_1.set_study("1.2").set_series("");
    BOOST_REQUIRE_EQUAL(selector_1.get_path(false), "/studies/1.2/series");

    odil::webservices::Selector const selector_2(
        std::map<std::string, std::string>(
            {{"studies", "1.2"},
             {"series", "3.4"},
             {"instances", "5.6"}}),
        {1,2,3});
    BOOST_REQUIRE_EQUAL(selector_2.get_path(true), "/studies/1.2/series/3.4/instances/5.6/frames/1,2,3");

    odil::webservices::Selector const selector_3(
        std::map<std::string, std::string>(
            {{"studies", "1.2"},
             {"series", "3.4"},
             {"instances", ""}}));
    BOOST_REQUIRE_EQUAL(selector_3.get_path(false), "/studies/1.2/series/3.4/instances");
}

BOOST_AUTO_TEST_CASE(Is_XXX_present)
{
    odil::webservices::Selector selector;
    selector.set_instance("5.6");
    BOOST_REQUIRE(selector.is_instance_present());
}

BOOST_AUTO_TEST_CASE(Get_XXX)
{
    odil::webservices::Selector const selector(
        std::map<std::string, std::string>(
            {{"studies", "1.2"},
             {"series", "3.4"},
             {"instances", "5.6"}}),
        {1, 2, 3});
    BOOST_REQUIRE_EQUAL(selector.get_instance(), "5.6");
    std::vector<int> frames({1, 2, 3});
    BOOST_REQUIRE(selector.get_frames() == frames);
}

BOOST_AUTO_TEST_CASE(Set_XXX)
{
    odil::webservices::Selector selector;
    selector.set_frames({1, 2, 3});
    std::vector<int> frames({1, 2, 3});
    BOOST_REQUIRE(selector.get_frames() == frames);
}
