/*!
 * \brief Test JSON parsing
 */

// Test framework
#include "gtest/gtest.h"

// Tested file
#include "TreeBinding/JSON.h"

#include <map>

/*!
 * \brief Enum type
 */
enum class EnumType1
{
    ENUM1,
    ENUM2,
    ENUM3
};

namespace TreeBinding
{

/*!
 * \brief \copydoc data_binding::Translator::fromString(std::string const &str,EnumType *value)
 */
template<>
EnumType1 Translator::fromString<EnumType1>(std::string const &str)
{
    static const std::map<std::string, EnumType1> stringToEnum =
    {
        {"ENUM1", EnumType1::ENUM1},
        {"ENUM2", EnumType1::ENUM2},
        {"ENUM3", EnumType1::ENUM3}
    };

    return stringToEnum.at(str);
}

/*!
 * \brief \copydoc std::string data_binding::Translator::toString(const EnumType* const value)
 */
template<>
std::string Translator::toString(const EnumType1& value)
{
    static const std::map<EnumType1, std::string> enumToString =
    {
        {EnumType1::ENUM1, "ENUM1"},
        {EnumType1::ENUM2, "ENUM2"},
        {EnumType1::ENUM3, "ENUM3"}
    };

    return enumToString.at(value);
}

} // namespace data_binding

/*!
 * \brief Most nested element
 */
JSON_ELEMENT(MostNestedJsonElement)
{
    JSON_CHILD("StringFieldName" , std::string                              ) strField ; /*!< String field  */
    JSON_CHILD("IntegerFieldName", int                                      ) intField ; /*!< Integer field */
    JSON_CHILD("EnumFieldName"   , EnumType1,  JSON::ItemsNum::NOT_SPECIFIED) enumField; /*!< Enum field (optional) */
};

/*!
 * \brief Intermediate element
 */
JSON_ELEMENT(NestedJsonElement)
{
    JSON_CHILD("IntegerFieldName"       , int)                      intField; /*!< Integer field */
    JSON_ARRAY("MostNestedElementsArray", MostNestedJsonElement, 2) childs  ; /*!< Child element (must contain 2 childs elements) */
};

/*!
 * \brief Root element
 */
JSON_ELEMENT(RootJsonElement)
{
    JSON_CHILD("StringFieldName"    , std::string                                  ) strField; /*!< String field */
    JSON_ARRAY("NestedElementsArray", NestedJsonElement, JSON::ItemsNum::MORE_THAN_0) childs  ; /*!< Child element (must contain more than 0 elements) */
};

static const std::string CORRECT_JSON_DATA =
R"(
{
    "RootElement" :
    {
        "StringFieldName": "StringValue",
        "NestedElementsArray":
        [
            {
                "IntegerFieldName": "22",
                "MostNestedElementsArray":
                [
                    {
                        "StringFieldName": "StringValue1",
                        "IntegerFieldName": "1",
                        "EnumFieldName": "ENUM2"
                    },
                    {
                        "StringFieldName": "StringValue5",
                        "IntegerFieldName": "11"
                    }
                ]
            }
        ]
    }
}
)";


TEST(common, test_json) {
    RootJsonElement rootJsonElement;

    boost::property_tree::ptree propertyTree;
    std::istringstream str(CORRECT_JSON_DATA);

    boost::property_tree::read_json(str, propertyTree);

    rootJsonElement.parsePtree(propertyTree);

    ASSERT_EQ(*rootJsonElement.strField, "StringValue");
    ASSERT_EQ(rootJsonElement.childs->size(), 1ul );

    auto& nested = rootJsonElement.childs->at(0);
    ASSERT_EQ(*nested.intField, 22);
    ASSERT_EQ(nested.childs->size(), 2ul);

    auto& mostNested0 = nested.childs->at(0);
    auto& mostNested1 = nested.childs->at(1);
    ASSERT_EQ(*mostNested0.strField , "StringValue1");
    ASSERT_EQ(*mostNested0.intField , 1);
    ASSERT_EQ(*mostNested0.enumField, EnumType1::ENUM2);

    ASSERT_EQ(*mostNested1.strField , "StringValue5");
    ASSERT_EQ(*mostNested1.intField , 11);
    ASSERT_EQ( mostNested1.enumField.validity, false);
}
