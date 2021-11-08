/*!
 * \brief Test XML parsing
 */


// Test framework
#include "gtest/gtest.h"

// Tested file
#include "TreeBinding.h"

#include <map>
#include <boost/property_tree/xml_parser.hpp>

/*!
 * \brief Enum type
 */
enum class EnumType
{
    ENUM1,
    ENUM2,
    ENUM3
};

namespace TreeBinding
{

/*!
 * \brief \copydoc TreeBinding::Translator::fromString(std::string const &str,EnumType *value)
 */
template<>
void Translator::fromString(std::string const &str, EnumType *value)
{
    static const std::map<std::string, EnumType> stringToEnum =
    {
        {"ENUM1", EnumType::ENUM1},
        {"ENUM2", EnumType::ENUM2},
        {"ENUM3", EnumType::ENUM3}
    };

    *value = stringToEnum.at(str);
}

/*!
 * \brief \copydoc std::string TreeBinding::Translator::toString(const EnumType* const value)
 */
template<>
std::string TreeBinding::Translator::toString(const EnumType* const value)
{
    static const std::map<EnumType, std::string> enumToString =
    {
        {EnumType::ENUM1, "ENUM1"},
        {EnumType::ENUM2, "ENUM2"},
        {EnumType::ENUM3, "ENUM3"}
    };

    return enumToString.at(*value);
}

} // namespace TreeBinding

/*!
 * \brief Most nested element
 */
XML_ELEMENT(MostNestedElement, "MostNestedElement")
{
  XML_ATTR("StringAttrName" , std::string                           ) strAttr ; /*!< String attribute  */
  XML_ATTR("IntegerAttrName", int                                   ) intAttr ; /*!< Integer attribute */
  XML_ATTR("EnumAttr"       , EnumType,  XML::ItemNum::NOT_SPECIFIED) enumAttr; /*!< Enum attribute (optional) */
};

/*!
 * \brief Intermediate element
 */
XML_ELEMENT(NestedElement, "NestedElement")
{
    XML_ATTR("IntegerAttrName", int)     intAttr; /*!< Integer attribute */
    XML_CHILD_ELEMENTS(MostNestedElement, 2) childs; /*!< Child element (must contain 2 childs elements) */
};

/*!
 * \brief Root element
 */
XML_ELEMENT(RootElement, "RootElement")
{
    XML_ATTR("StringAttrName", std::string) strAttr; /*!< String attribute */
    XML_CHILD_ELEMENTS(NestedElement, XML::ItemNum::MORE_THAN_0) childs; /*!< Child element (must contain more than 0 elements) */
};

static const std::string CORRECT_XML_DATA =
R"(
<RootElement StringAttrName="StringValue">
    <NestedElement IntegerAttrName="22">
        <MostNestedElement StringAttrName="StringValue1" IntegerAttrName="1"  EnumAttr="ENUM2"/>
        <MostNestedElement StringAttrName="StringValue5" IntegerAttrName="11"/>
    </NestedElement>
</RootElement>
)";

TEST(get_number_test, returns_correct_value) {
    RootElement rootXmlElement;

    boost::property_tree::ptree propertyTree;
    std::istringstream str(CORRECT_XML_DATA);

    boost::property_tree::read_xml(str, propertyTree);

    rootXmlElement.parsePtree(propertyTree);

    ASSERT_EQ(*rootXmlElement.strAttr, "StringValue");
    ASSERT_EQ(rootXmlElement.childs->size(), 1ul );

    auto& nested = rootXmlElement.childs->at(0);
    ASSERT_EQ(*nested.intAttr, 22);
    ASSERT_EQ(nested.childs->size(), 2ul);

    auto& mostNested0 = nested.childs->at(0);
    auto& mostNested1 = nested.childs->at(1);
    ASSERT_EQ(*mostNested0.strAttr , "StringValue1");
    ASSERT_EQ(*mostNested0.intAttr , 1);
    ASSERT_EQ(*mostNested0.enumAttr, EnumType::ENUM2);

    ASSERT_EQ(*mostNested1.strAttr , "StringValue5");
    ASSERT_EQ(*mostNested1.intAttr , 11);
    ASSERT_EQ( mostNested1.enumAttr.validity, false);
}

