/*!
 * TODO
 */


// Test framework
#include "gtest/gtest.h"

#include "TreeBinding.h"



XML_ELEMENT(NestedType1, "NestedType1Name")
{
  XML_ATTR("StringParamName" , std::string) stringParam;
  XML_ATTR("IntegerParamName", int        ) intParam   ;
};


TEST(get_number_test, returns_correct_value) {

}

