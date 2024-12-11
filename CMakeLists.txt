cmake_minimum_required(VERSION 3.12)

set(CMAKE_TOOLCHAIN_FILE "${CMAKE_BINARY_DIR}/conan/conan_toolchain.cmake")
include(${CMAKE_TOOLCHAIN_FILE})

project(gRPC_Task)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

find_package(Pistache REQUIRED)
find_package(OpenSSL REQUIRED)
find_package(absl REQUIRED)
find_package(c-ares REQUIRED)
find_package(re2 REQUIRED)
find_package(Protobuf REQUIRED)
find_package(gRPC CONFIG REQUIRED)
find_package(Threads REQUIRED)
find_package(nlohmann_json REQUIRED)
find_package(PahoMqttCpp  REQUIRED)

set(PROTOBUF_GENERATED_DIR "${CMAKE_CURRENT_BINARY_DIR}/protos")
file(MAKE_DIRECTORY "${PROTOBUF_GENERATED_DIR}")
include_directories(${PROTOBUF_GENERATED_DIR})
include_directories(include/gRPC/server)
include_directories(include/gRPC/client)
include_directories(include/JsonDataRW)
include_directories(include/SensorsServer)
include_directories(include/HTTPClient)
include_directories(include/MQTT)
include_directories(include/Sensors)

set(_PROTOBUF_PROTOC $<TARGET_FILE:protobuf::protoc>)
set(_GRPC_CPP_PLUGIN_EXECUTABLE $<TARGET_FILE:gRPC::grpc_cpp_plugin>)

set(PROTO_FILES ${CMAKE_CURRENT_SOURCE_DIR}/protos/factorial.proto)

add_custom_command(
    OUTPUT "${PROTOBUF_GENERATED_DIR}/factorial.pb.cc" "${PROTOBUF_GENERATED_DIR}/factorial.pb.h"
           "${PROTOBUF_GENERATED_DIR}/factorial.grpc.pb.cc" "${PROTOBUF_GENERATED_DIR}/factorial.grpc.pb.h"
    COMMAND ${_PROTOBUF_PROTOC}
    ARGS --grpc_out "${PROTOBUF_GENERATED_DIR}" --cpp_out "${PROTOBUF_GENERATED_DIR}"
         --plugin=protoc-gen-grpc="${_GRPC_CPP_PLUGIN_EXECUTABLE}"
         -I "${CMAKE_CURRENT_SOURCE_DIR}/protos" "${PROTO_FILES}"
    DEPENDS "${PROTO_FILES}"
    COMMENT "Generating protobuf and gRPC files from factorial.proto"
)

set(PROTO_SRCS "${PROTOBUF_GENERATED_DIR}/factorial.pb.cc")
set(PROTO_HDRS "${PROTOBUF_GENERATED_DIR}/factorial.pb.h")
set(GRPC_SRCS "${PROTOBUF_GENERATED_DIR}/factorial.grpc.pb.cc")
set(GRPC_HDRS "${PROTOBUF_GENERATED_DIR}/factorial.grpc.pb.h")

set(SOURCES 
    src/gRPC/server/factorialServer.cpp
    src/gRPC/client/factorialClient.cpp
    src/JsonDataRW/JsonDataRW.cpp
    src/SensorsServer/SensorsServer.cpp
    src/HTTPClient/HTTPClient.cpp
    src/MQTT/MQTT_Client.cpp
    src/Sensors/Sensor.cpp
    src/main.cpp
    ${PROTO_SRCS}
    ${GRPC_SRCS}
)

add_executable(gRPC_Task ${SOURCES})

target_link_libraries(gRPC_Task
    Pistache::Pistache
    abseil::abseil
    c-ares::cares
    re2::re2
    protobuf::libprotobuf
    nlohmann_json::nlohmann_json
    PahoMqttCpp::paho-mqttpp3-static
    gRPC::grpc++
    pthread)

target_include_directories(gRPC_Task PRIVATE ${CONAN_INCLUDE_DIRS})

# Add custom target to run the application
add_custom_target(run
    COMMAND $<TARGET_FILE:gRPC_Task>
    DEPENDS gRPC_Task
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
)
