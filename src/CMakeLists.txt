SET(CMAKE_INCLUDE_CURRENT_DIR ON)
INCLUDE_DIRECTORIES(
	${CMAKE_CURRENT_SOURCE_DIR}/..
	${CMAKE_CURRENT_SOURCE_DIR}/../include
	${CMAKE_CURRENT_BINARY_DIR}/../include
	${CMAKE_CURRENT_BINARY_DIR}/..
	${NEON_INCLUDE_DIRS}
	${JANSSON_INCLUDE_DIRS}
	${LIBXML2_INCLUDE_DIRS}
)

SET(_sourcescc HTTPFetch.cc CoverArt.cc ReleaseInfo.cc ImageList.cc Image.cc
	Thumbnails.cc TypeList.cc Type.cc)

SET(_sourcesc caa_c.cc)

# when crosscompiling import the executable targets from a file
IF(CMAKE_CROSSCOMPILING)
  SET(IMPORT_EXECUTABLES "IMPORTFILE-NOTFOUND" CACHE FILEPATH "Point it to the export file from a native build")
  INCLUDE(${IMPORT_EXECUTABLES})
ENDIF(CMAKE_CROSSCOMPILING)

# only build the generator if not crosscompiling
IF(NOT CMAKE_CROSSCOMPILING)
	ADD_EXECUTABLE(make-c-interface make-c-interface.cc xmlParser.cc)
	TARGET_LINK_LIBRARIES(make-c-interface ${LIBXML2_LIBRARIES})
ENDIF(NOT CMAKE_CROSSCOMPILING)

# export the generator target to a file, so it can be imported (see above) by another build
# the IF() is not necessary, but makes the intention clearer
IF(NOT CMAKE_CROSSCOMPILING)
  EXPORT(TARGETS make-c-interface FILE ${CMAKE_BINARY_DIR}/ImportExecutables.cmake )
ENDIF(NOT CMAKE_CROSSCOMPILING)

FILE(GLOB INCFILES *.inc)
ADD_CUSTOM_COMMAND(
	OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/caa_c.cc ${CMAKE_CURRENT_BINARY_DIR}/caa_c.h ${CMAKE_CURRENT_BINARY_DIR}/../include/coverart/caa_c.h
	COMMAND make-c-interface ${CMAKE_CURRENT_SOURCE_DIR} cinterface.xml ${CMAKE_CURRENT_BINARY_DIR} caa_c.cc caa_c.h
	COMMAND ${CMAKE_COMMAND} -E copy_if_different ${CMAKE_CURRENT_BINARY_DIR}/caa_c.h ${CMAKE_CURRENT_BINARY_DIR}/../include/coverart/caa_c.h
	DEPENDS make-c-interface cinterface.xml ${INCFILES}
)

ADD_CUSTOM_TARGET(src_gen DEPENDS caa_c.h)

ADD_LIBRARY(coverartcc ${_sourcescc})
ADD_LIBRARY(coverart ${_sourcesc})

SET_TARGET_PROPERTIES(coverartcc PROPERTIES
    VERSION ${coverart_VERSION}
    SOVERSION ${coverart_SOVERSION}
    DEFINE_SYMBOL MB_API_EXPORTS
)

SET_TARGET_PROPERTIES(coverart PROPERTIES
    VERSION ${coverartc_VERSION}
    SOVERSION ${coverartc_SOVERSION}
    DEFINE_SYMBOL MB_API_EXPORTS
)

if(CMAKE_BUILD_TYPE STREQUAL Debug)
	SET_TARGET_PROPERTIES(coverartcc PROPERTIES
		DEFINE_SYMBOL _COVERART_DEBUG_
	)
	IF(CMAKE_COMPILER_IS_GNUCXX)
		SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -ggdb -O0")
	ENDIF(CMAKE_COMPILER_IS_GNUCXX)
endif(CMAKE_BUILD_TYPE STREQUAL Debug)

TARGET_LINK_LIBRARIES(coverartcc ${NEON_LIBRARIES} ${JANSSON_LIBRARIES})
TARGET_LINK_LIBRARIES(coverart coverartcc)

IF(WIN32)
    TARGET_LINK_LIBRARIES(coverartcc wsock32 winmm ws2_32)
ENDIF(WIN32)

IF(CMAKE_COMPILER_IS_GNUCXX)
		SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Werror -Wextra -pedantic-errors")
		set_source_files_properties(ca_c.cc PROPERTIES COMPILE_FLAGS "-Wno-deprecated-declarations")
		set_source_files_properties(ReleaseInfo.cc PROPERTIES COMPILE_FLAGS "-Wno-long-long")
		set_source_files_properties(ImageList.cc PROPERTIES COMPILE_FLAGS "-Wno-long-long")
		set_source_files_properties(Image.cc PROPERTIES COMPILE_FLAGS "-Wno-long-long")
		set_source_files_properties(Thumbnails.cc PROPERTIES COMPILE_FLAGS "-Wno-long-long")
		set_source_files_properties(TypeList.cc PROPERTIES COMPILE_FLAGS "-Wno-long-long")
		set_source_files_properties(CoverArt.cc PROPERTIES COMPILE_FLAGS "-Wno-long-long")
		set_source_files_properties(caa_c.cc PROPERTIES COMPILE_FLAGS "-Wno-long-long")
ENDIF(CMAKE_COMPILER_IS_GNUCXX)

INSTALL(TARGETS coverartcc DESTINATION ${LIB_INSTALL_DIR})
INSTALL(TARGETS coverart DESTINATION ${LIB_INSTALL_DIR})
