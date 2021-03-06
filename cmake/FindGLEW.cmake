
# GLEW_FOUND
# GLEW_INCLUDE_DIR
# GLEW_LIBRARY
# GLEW_BINARY


find_path(GLEW_INCLUDE_DIR GL/glew.h
    ${LIB_DIR}/glew-1.10.0/include
    ${LIB_DIR}/glew/include
    $ENV{GLEWDIR}/include
    $ENV{GLEW_HOME}/include
    $ENV{PROGRAMFILES}/GLEW/include
    /usr/include
    /usr/local/include
    /sw/include
    /opt/local/include
    DOC "The directory where GL/glew.h resides")

find_library(GLEW_LIBRARY
    NAMES GLEW glew glew32 glew32s
    PATHS
    ${LIB_DIR}/glew-1.10.0/lib/Release/x64
    ${LIB_DIR}/glew/lib/
    $ENV{GLEWDIR}/lib
    $ENV{GLEW_HOME}/lib
    $ENV{GLEW_HOME}/lib/Release/x64
    /usr/lib64
    /usr/local/lib64
    /sw/lib64
    /opt/local/lib64
    /usr/lib
    /usr/local/lib
    /sw/lib
    /opt/local/lib
    DOC "The GLEW library")

if(WIN32)
    find_file(GLEW_BINARY
        NAMES glew32.dll glew32s.dll
        PATHS
        ${LIB_DIR}/glew-1.10.0/bin/Release/x64
        $ENV{GLEWDIR}/bin
        $ENV{GLEW_HOME}/bin
        $ENV{GLEW_HOME}/bin/Release/x64
        DOC "The GLEW binary")

endif()
    
if(GLEW_INCLUDE_DIR AND GLEW_LIBRARY)
    set(GLEW_FOUND 1 CACHE STRING "Set to 1 if GLEW is found, 0 otherwise")
else()
    set(GLEW_FOUND 0 CACHE STRING "Set to 1 if GLEW is found, 0 otherwise")
    message(WARNING "Note: an envvar like GLEW_HOME assists this script to locate glew.")
endif()

mark_as_advanced( GLEW_FOUND )
