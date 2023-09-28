#--------------------------------------------------------------------------
# spacegame project
#--------------------------------------------------------------------------

PROJECT(spacegame)
FILE(GLOB project_headers code/*.h)
FILE(GLOB project_sources code/*.cc)

SET(files_project ${project_headers} ${project_sources})
SET(files_proto)
FLAT_COMPILE(proto.fbs)
SOURCE_GROUP("spacegame" FILES ${files_project})

ADD_EXECUTABLE(spacegame ${files_project} ${files_proto})
TARGET_INCLUDE_DIRECTORIES(spacegame PRIVATE "${CMAKE_BINARY_DIR}/generated/flat")

TARGET_LINK_LIBRARIES(spacegame core render)
ADD_DEPENDENCIES(spacegame core render)

IF(MSVC)
    set_property(TARGET spacegame PROPERTY VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}/bin")
ENDIF()