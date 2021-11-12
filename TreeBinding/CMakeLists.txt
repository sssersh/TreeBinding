#
# Build origin sources to check, that not contain errors
#

cmake_minimum_required(VERSION 3.10.1)

project(TreeBindingOrigin)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/..)

set(SOURCES Details/Core/Empty.cpp)

set(INCLUDES
        JSON.h
        TreeBinding.h
        XML.h
        Details/Core/TreeBindingDecl.h
        Details/ArchiveSerializerDecl.h
        Details/ArchiveSerializer.h
        Details/PtreeWriter.h
        Details/TableParser.h
        Details/Core/Translator.h
        Details/Core/NodesNum.h
        Details/Core/Exceptions.h
        Details/Core/BasicNodeData.h
        Details/Core/NodeData.h
        Details/Core/StringContainer.h
        Details/Core/Node.h
        Details/Core/NodeIterator.h
        Details/Core/BasicTree.h
        Details/Core/MacroUtils.h
        Details/Core/Tree.h)

set(LIB TreeBindingOrigin)

add_library(${LIB} ${INCLUDES} ${SOURCES})
