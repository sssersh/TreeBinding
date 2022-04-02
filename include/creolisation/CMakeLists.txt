#
# Build origin sources to check, that not contain errors
#

cmake_minimum_required(VERSION 3.10.1)

project(TreeBindingOrigin)

include_directories(../..)

set(SOURCES details/Core/dummy.cpp)

set(INCLUDES
        JSON.h
        serializable_types.h
        XML.h
        details/Core/TreeBindingDecl.h
        details/ArchiveSerializerDecl.h
        details/ArchiveSerializer.h
        details/PtreeWriter.h
        details/TableParser.h
        details/Core/Translator.h
        details/Core/NodesNum.h
        details/Core/Exceptions.h
        details/Core/BasicNodeData.h
        details/Core/NodeData.h
        details/Core/StringContainer.h
        details/Core/Node.h
        details/Core/NodeIterator.h
        details/Core/BasicTree.h
        details/Core/MacroUtils.h
        details/Core/Tree.h)

set(LIB TreeBindingOrigin)

add_library(${LIB} ${INCLUDES} ${SOURCES})
