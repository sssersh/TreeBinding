# Tree Binding: library for binding Boost property tree and data structures

Master:
[![Build Status](https://app.travis-ci.com/sssersh/creolization.svg?branch=master)]travis1]

Develop:
[![Build Status](https://app.travis-ci.com/sssersh/creolization.svg?branch=develop)][travis2]

[![Coverage Status](https://coveralls.io/repos/github/deftio/travis-ci-cpp-example/badge.svg?branch=master)](https://coveralls.io/github/deftio/travis-ci-cpp-example?branch=master)
[![License](https://img.shields.io/badge/License-BSD%202--Clause-blue.svg)](https://opensource.org/licenses/BSD-2-Clause)


## Goals

__Main goal of this libaray__  — .... [Boost][boost]



## Structure of project

- `include` — derectory with one header include file
- `samples` — examples of using library
- `creolization` — Tree Binding source files
    - `creolization.h` — ...
    - `CMakeLists.txt` — ...
- `tests` — tests

## Usage

```C
XML_ELEMENT(MostNestedElement, "MostNestedElement")
{
  XML_ATTR("StringAttrName" , std::string                           ) strAttr ; /*!< String attribute  */
  XML_ATTR("IntegerAttrName", int                                   ) intAttr ; /*!< Integer attribute */
  XML_ATTR("EnumAttr"       , EnumType,  XML::ItemsNum::NOT_SPECIFIED) enumAttr; /*!< Enum attribute (optional) */
};
```

According XML data:


According XML Scheme:


<!-- LINKS -->

[boost]:       https://www.boost.org/
