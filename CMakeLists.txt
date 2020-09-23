project(H1Conformance)

set(CMAKE_CXX_STANDARD 17)

set(SOURCES
        connection/insecure_connection.cpp
        http/response_reader.cpp
        suites/basic/host_header.cpp
        suites/basic/invalid_request_line.cpp
        suites/basic/minimal_request.cpp
        main.cpp
)

add_executable(H1Conformance ${SOURCES})