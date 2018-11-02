#define BOOST_TEST_MODULE QIDORSRequest
#include <boost/test/unit_test.hpp>

#include <set>

#include "odil/json_converter.h"

#include "odil/DataSet.h"
#include "odil/Tag.h"

#include "odil/webservices/HTTPRequest.h"
#include "odil/webservices/Utils.h"
#include "odil/webservices/QIDORSRequest.h"
#include "odil/webservices/Selector.h"
#include "odil/webservices/URL.h"

#include <iostream>

odil::webservices::URL const base_url_http{
    "http", "example.com", "/dicom", "", ""};

odil::webservices::Selector const instance_selector{
    {{"studies", "1.2"}, {"series", "3.4"}, {"instances", ""}}
};

odil::webservices::URL const full_url {
    "http",  "example.com", "/dicom/studies/1.2/instances",

    "PatientName=TOTO&"
    "SharedFunctionalGroupsSequence.EffectiveEchoTime=10.5&"
    "includefield=00200020&includefield=52009229.00200035",

    ""
};

BOOST_AUTO_TEST_CASE(ConstructorUrl)
{
    odil::webservices::QIDORSRequest const request(base_url_http);
    BOOST_REQUIRE(request.get_base_url() == base_url_http);
}

BOOST_AUTO_TEST_CASE(BaseUrl)
{
    odil::webservices::HTTPRequest http_request("GET", full_url);
    http_request.set_header("Accept", "application/dicom+json");
    odil::webservices::QIDORSRequest const qido_request(http_request);

    BOOST_REQUIRE(qido_request.get_base_url() == base_url_http);
}

BOOST_AUTO_TEST_CASE(SetBaseUrl)
{
    odil::webservices::QIDORSRequest request((odil::webservices::URL()));
    request.set_base_url(base_url_http);
    BOOST_REQUIRE(request.get_base_url() == base_url_http);
}

BOOST_AUTO_TEST_CASE(FullUrl)
{
    odil::webservices::HTTPRequest http_request("GET", full_url);
    http_request.set_header("Accept", "application/dicom+json");
    odil::webservices::QIDORSRequest const qido_request(http_request);

    odil::webservices::URL const full_url_alphabetic {
        "http", "example.com", "/dicom/studies/1.2/instances",

        "PatientName=TOTO&"
        "SharedFunctionalGroupsSequence.EffectiveEchoTime=10.5&"
        "includefield=PatientOrientation&"
        "includefield=SharedFunctionalGroupsSequence.ImageOrientation&"
        "fuzzymatching=false",

        ""
    };

    BOOST_REQUIRE(qido_request.get_url() == full_url_alphabetic);
}

BOOST_AUTO_TEST_CASE(Selector)
{
    odil::webservices::HTTPRequest http_request("GET", full_url);
    http_request.set_header("Accept", "application/dicom+json");
    odil::webservices::QIDORSRequest const qido_request(http_request);

    BOOST_REQUIRE_EQUAL(
        qido_request.get_selector().get_path(false), "/studies/1.2/instances");
}

BOOST_AUTO_TEST_CASE(QueryDataset)
{
    odil::webservices::HTTPRequest http_request("GET", full_url);
    http_request.set_header("Accept", "application/dicom+json");
    odil::webservices::QIDORSRequest const qido_request(http_request);

    auto dataset = std::make_shared<odil::DataSet>();
    dataset->add(odil::Tag("PatientName"), {"TOTO"});
    dataset->add(odil::Tag("00200020"));

    auto shared = std::make_shared<odil::DataSet>();
    shared->add(odil::Tag("EffectiveEchoTime"), {10.5});
    shared->add(odil::Tag("00200035"));
    dataset->add(odil::Tag("SharedFunctionalGroupsSequence"), {shared});

    BOOST_REQUIRE(*qido_request.get_query_data_set() == *dataset);
}

BOOST_AUTO_TEST_CASE(FuzzyLimitOffset)
{
    odil::webservices::HTTPRequest http_request("GET", full_url);
    http_request.set_header("Accept", "application/dicom+json");
    odil::webservices::QIDORSRequest const qido_request(http_request);

    BOOST_REQUIRE(!qido_request.get_fuzzymatching());
    BOOST_REQUIRE_EQUAL(qido_request.get_limit(), -1);
    BOOST_REQUIRE_EQUAL(qido_request.get_offset(), 0);
}

BOOST_AUTO_TEST_CASE(MediaTypeJson)
{
    odil::webservices::HTTPRequest http_request("GET", full_url);
    http_request.set_header("Accept", "application/dicom+json");
    odil::webservices::QIDORSRequest const qido_request(http_request);
    BOOST_REQUIRE_EQUAL(qido_request.get_media_type(), "application/dicom+json");
    BOOST_REQUIRE(
        qido_request.get_representation()
            == odil::webservices::Representation::DICOM_JSON);
}

BOOST_AUTO_TEST_CASE(MediaTypeXml)
{
    odil::webservices::HTTPRequest http_request("GET", full_url);
    http_request.set_header("Accept", "multipart/related;type=application/dicom+xml");
    odil::webservices::QIDORSRequest const qido_request(http_request);
    BOOST_REQUIRE_EQUAL(qido_request.get_media_type(), "application/dicom+xml");
    BOOST_REQUIRE(
        qido_request.get_representation()
            == odil::webservices::Representation::DICOM_XML);
}

BOOST_AUTO_TEST_CASE(MediaTypeUnrecognized)
{
    odil::webservices::HTTPRequest http_request("GET", full_url);
    http_request.set_header("Accept", "multipart/related;type=application/dicom+json");
    BOOST_REQUIRE_THROW(
        odil::webservices::QIDORSRequest const request(http_request),
        odil::Exception);
}

BOOST_AUTO_TEST_CASE(MediaTypeUnrecognized2)
{
    odil::webservices::HTTPRequest http_request("GET", full_url);
    http_request.set_header("Accept", "application/dicom+xml");
    BOOST_REQUIRE_THROW(
        odil::webservices::QIDORSRequest const request(http_request),
        odil::Exception);
}

BOOST_AUTO_TEST_CASE(RequestDataset)
{
    odil::webservices::Selector const selector{
        {{"studies", "1.2"}, {"instances", ""}}
    };

    //-------------------- Query Dataset
    auto dataset = std::make_shared<odil::DataSet>();
    dataset->add(odil::Tag("PatientName"), {"TOTO"});
    dataset->add(odil::Tag("00200020"));

    auto shared = std::make_shared<odil::DataSet>();
    shared->add(odil::Tag("StudyDate"), {20130509});
    shared->add(odil::Tag("00200035"));
    dataset->add(odil::Tag("SharedFunctionalGroupsSequence"), {shared});

    odil::webservices::QIDORSRequest request(base_url_http);
    request.request_datasets(
        odil::webservices::Representation::DICOM_XML, selector, dataset);

    odil::webservices::URL const full_url_alphabetic_tags {
        "http", "example.com", "/dicom/studies/1.2/instances",

        "PatientName=TOTO&"
        "SharedFunctionalGroupsSequence.StudyDate=20130509&"
        "includefield=PatientOrientation&"
        "includefield=SharedFunctionalGroupsSequence.ImageOrientation&"
        "fuzzymatching=false",

        ""
    };

    BOOST_REQUIRE(
        request.get_http_request().get_target() == full_url_alphabetic_tags);
    BOOST_REQUIRE_EQUAL(request.get_http_request().get_method(), "GET");
}

BOOST_AUTO_TEST_CASE(Equality)
{
    odil::webservices::URL const full_url_cpy {
        "http", "example.com", "/dicom/studies/1.2/instances",

        "SharedFunctionalGroupsSequence.EffectiveEchoTime=10.5&"
        "includefield=00200020&includefield=52009229.00200035&"
        "PatientName=TOTO",

        ""
    };

    // The element's order within query should not be important,
    // this url and full_url must lead to the same QIDORSRequest

    odil::webservices::HTTPRequest http_request_base("GET", full_url);
    http_request_base.set_header("Accept", "application/dicom+json");

    odil::webservices::QIDORSRequest const qido_request(http_request_base);

    odil::webservices::HTTPRequest http_request_cpy("GET", full_url_cpy);
    http_request_cpy.set_header("Accept", "application/dicom+json");

    odil::webservices::QIDORSRequest const qido_request_cpy(http_request_cpy);

    BOOST_REQUIRE(qido_request == qido_request_cpy);
}

BOOST_AUTO_TEST_CASE(Difference)
{
    odil::webservices::URL const full_url_cpy {
        "http", "example.com", "/dicom/studies/1.2/instances",

        "SharedFunctionalGroupsSequence.EffectiveEchoTime=10.5&"
        "includefield=00200020&includefield=52009229.00200035&"
        "PatientName=TUTU",

        ""
    };

    odil::webservices::HTTPRequest http_request_base("GET", full_url);
    http_request_base.set_header("Accept", "application/dicom+json");

    odil::webservices::QIDORSRequest const qido_request(http_request_base);

    odil::webservices::HTTPRequest http_request_cpy("GET", full_url_cpy);
    http_request_cpy.set_header("Accept", "application/dicom+json");

    odil::webservices::QIDORSRequest const request_cpy(http_request_cpy);

    BOOST_REQUIRE(qido_request != request_cpy);
}
