project(H1Conformance)

set(CMAKE_CXX_STANDARD 17)

set(SOURCES
        connection/insecure_connection.cpp
        http/response_reader.cpp
        suites/basic/host_header.cpp
        suites/basic/minimal_request.cpp
        suites/startline/method.cpp
        suites/startline/request_target.cpp
        suites/startline/version.cpp
        utils.cpp
        main.cpp
)

add_executable(H1Conformance ${SOURCES})