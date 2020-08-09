cmake_minimum_required(VERSION 3.2.0)

project(server)

option(BUILD_CLIENT "build client to connect with server" ON)
option(BUILD_CGI "build cgi dynamic server" ON)
option(BUILD_TEST "build test for server and cgi" OFF)

# third parties
add_subdirectory(${PROJECT_SOURCE_DIR}/third_party/yaml-cpp)

include_directories(${PROJECT_SOURCE_DIR})
include_directories(${PROJECT_SOURCE_DIR}/third_party/yaml-cpp/include)
add_definitions(${PROJECT_BINARY_DIR}/third_party/yaml-cpp)


# build core library
set(CORE_FILENAMES connection.cc config_parser.cc)
foreach(file_path ${CORE_FILENAMES})
    set(CORE_FILES ${CORE_FILES} ${PROJECT_SOURCE_DIR}/server/core/${file_path})
endforeach()
add_library(core STATIC ${CORE_FILES})
target_link_libraries(core yaml-cpp)


# build echo server
set(ECHO_SERVER_FILENAMES echo.cc server.cc)
foreach(file_path ${ECHO_SERVER_FILENAMES})
    set(ECHO_SERVER_FILES ${ECHO_SERVER_FILES} ${PROJECT_SOURCE_DIR}/server/echo_server/${file_path})
endforeach()
add_executable(echo_server ${ECHO_SERVER_FILES})
target_link_libraries(echo_server core)


# build web server
set(WEB_SERVER_FILENAMES web_server.cc tiny.cc)
foreach(file_path ${WEB_SERVER_FILENAMES})
    set(WEB_SERVER_FILES ${WEB_SERVER_FILES} ${PROJECT_SOURCE_DIR}/server/web_server/${file_path})
endforeach()
add_executable(web_server ${WEB_SERVER_FILES})
target_link_libraries(web_server core)


# build client
set(CLIENT_FILENAMES client.cc)
foreach(file_path ${CLIENT_FILENAMES})
    set(CLIENT_FILES ${CLIENT_FILES} ${PROJECT_SOURCE_DIR}/server/client/${file_path})
endforeach()
add_executable(client ${CLIENT_FILES})
target_link_libraries(client core)

# build cgi to support dynamic
set(CGI_FILENAMES adder.cc)
foreach(file_path ${CGI_FILENAMES})
    set(CGI_FILES ${CGI_FILES} ${PROJECT_SOURCE_DIR}/server/cgi/${file_path})
endforeach()
add_executable(cgi_bin ${CGI_FILES})

if(BUILD_TEST)
    # collect all test files
endif()
