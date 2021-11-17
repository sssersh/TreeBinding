/*!
 * \brief Test XML parsing
 */

// Test framework
#include "gtest/gtest.h"

// Tested file
#include "TreeBinding.h"



TEST(xml, test_default_element_name) {
    // Define element with default name
    XML_ELEMENT(XmlElement) {} element;

    const std::string XML_ELEMENT_DATA = R"(<XmlElement></XmlElement>)";

    boost::property_tree::ptree propertyTree;
    std::istringstream str(XML_ELEMENT_DATA);

    boost::property_tree::read_xml(str, propertyTree);

    ASSERT_NO_THROW(element.parsePtree(propertyTree));
}

