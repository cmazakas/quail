set(Boost_DIR "/Users/cmaza/cpp/boost-171/lib/cmake/Boost-1.71.0")
set(BUILD_TESTING ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_compile_options("/permissive-")
add_link_options("/NODEFAULTLIB:library" "/debug:fastlink")
add_compile_definitions("BOOST_ASIO_NO_DEPRECATED")

# include("/Users/cmaza/cpp/vcpkg/scripts/buildsystems/vcpkg.cmake")

include("/Users/cmaza/cpp/quail/build_Debug/conanbuildinfo.cmake")
set(OPENSSL_ROOT_DIR "${CONAN_OPENSSL_ROOT}")
set(CMAKE_PREFIX_PATH "${CMAKE_PREFIX_PATH};/Users/cmaza/.conan/data/Catch2/2.9.2/catchorg/stable/package/5ab84d6acfe1f23c4fae0ab88f26e3a396351ac9/lib/cmake/Catch2")
