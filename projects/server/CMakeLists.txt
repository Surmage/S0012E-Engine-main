#--------------------------------------------------------------------------
# server project
#--------------------------------------------------------------------------

PROJECT(server)
FILE(GLOB project_headers code/*.h)
FILE(GLOB project_sources code/*.cc)

SET(files_project ${project_headers} ${project_sources})
SET(files_proto)
FLAT_COMPILE(proto.fbs)
SOURCE_GROUP("server" FILES ${files_project})

ADD_EXECUTABLE(server ${files_project} ${files_proto})
TARGET_INCLUDE_DIRECTORIES(server PRIVATE "${CMAKE_BINARY_DIR}/generated/flat")

TARGET_LINK_LIBRARIES(server core render)
ADD_DEPENDENCIES(server core render)

IF(MSVC)
    set_property(TARGET server PROPERTY VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}/bin")
ENDIF()