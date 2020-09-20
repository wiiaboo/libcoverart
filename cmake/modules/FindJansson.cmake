INCLUDE(FindPackageHandleStandardArgs)
FIND_PACKAGE(PkgConfig)
PKG_CHECK_MODULES(PKG_JANSSON jansson)

IF(PKG_JANSSON_FOUND)
    SET(JANSSON_INCLUDE_DIRS ${PKG_JANSSON_INCLUDE_DIRS})
    IF(BUILD_SHARED_LIBS)
        SET(JANSSON_LIBRARIES ${PKG_JANSSON_LIBRARIES})
    ELSE(BUILD_SHARED_LIBS)
        SET(JANSSON_LIBRARIES ${PKG_JANSSON_STATIC_LIBRARIES})
    ENDIF(BUILD_SHARED_LIBS)

ELSE(PKG_JANSSON_FOUND)
    FIND_PATH(JANSSON_INCLUDE_DIRS jansson.h
        PATHS
        ${PKG_JANSSON_INCLUDE_DIRS}
        /usr/include
        /usr/local/include
    )

    FIND_LIBRARY(JANSSON_LIBRARIES jansson
        ${PKG_JANSSON_LIBRARY_DIRS}
        /usr/lib
        /usr/local/lib
    )

    FIND_PACKAGE_HANDLE_STANDARD_ARGS(Jansson DEFAULT_MSG JANSSON_LIBRARIES JANSSON_INCLUDE_DIRS)
ENDIF(PKG_JANSSON_FOUND)
