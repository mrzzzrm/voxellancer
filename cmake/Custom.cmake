
# Group source files in folders (e.g. for MSVC solutions)
# Example: source_group_by_path_function("${CMAKE_CURRENT_SOURCE_DIR}/src" 
#   "\\.h$|\\.hpp$|\\.cpp$|\\.c$|\\.ui$|\\.qrc$" "Source Files" ${sources})

function(source_group_by_path PARENT_PATH REGEX GROUP)
    foreach(FILENAME ${ARGN})

        get_filename_component(FILEPATH "${FILENAME}" REALPATH)
        file(RELATIVE_PATH FILEPATH ${PARENT_PATH} ${FILEPATH})
        get_filename_component(FILEPATH "${FILEPATH}" PATH)

        string(REPLACE "/" "\\" FILEPATH "${FILEPATH}")
        if(${FILENAME} MATCHES "${REGEX}")
            source_group("${GROUP}\\${FILEPATH}" FILES ${FILENAME})  
        endif()

    endforeach()

endfunction()


function(list_extract OUTPUT REGEX)

    set(args ${ARGV})

    list(REMOVE_AT args 0)
    list(REMOVE_AT args 0)

    foreach(FILENAME ${args})

        if(${FILENAME} MATCHES "${REGEX}")
            list(APPEND ${OUTPUT} ${FILENAME})
        endif()

    endforeach()

endfunction()