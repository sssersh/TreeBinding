#
# Build origin sources to check, that not contain errors
#

cmake_minimum_required(VERSION 3.10.1)

project(TreeBindingOrigin)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/..)

set(SOURCES Details/TreeBinding.cpp)

set(INCLUDES
        JSON.h
        TreeBinding.h
        XML.h
        Details/TreeBindingDecl.h
        Details/TreeBindingDefs.h
        Details/TreeBindingDetails.h
        Details/Parsers/ArchiveSerializerDecl.h
        Details/Parsers/ArchiveSerializer.h
        Details/Parsers/PtreeWriter.h
        Details/Parsers/TableParser.h
        Details/Translator.h
        Details/NodesNum.h
        Details/Exceptions.h
        Details/BasicNodeData.h
        Details/NodeData.h
        Details/StringContainer.h)

set(LIB TreeBindingOrigin)

add_library(${LIB} ${INCLUDES} ${SOURCES})
