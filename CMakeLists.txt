cmake_minimum_required(VERSION 3.5)

project(CryptedVault VERSION 1.0)


set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_VERBOSE_MAKEFILE True)


add_executable(CryptedVault src/main.cpp src/crypto.cpp)

# target_include_directories(CryptedVault PRIVATE ${PROJECT_SOURCE_DIR}/src)
find_package(OpenSSL REQUIRED)
target_link_libraries(CryptedVault OpenSSL::SSL)
