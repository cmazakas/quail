set(Boost_DIR "/Users/cmaza/cpp/boost-171/lib/cmake/Boost-1.71.0")
set(BUILD_TESTING ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_compile_options("/permissive-")
add_link_options("/NODEFAULTLIB:library" "/debug:fastlink")
add_compile_definitions("BOOST_ASIO_NO_DEPRECATED")

link_libraries("/Program\ Files\ (x86)/Windows Kits/10/Lib/10.0.17763.0/um/x64/crypt32.lib")
link_libraries("/Program\ Files\ (x86)/Windows Kits/10/Lib/10.0.17763.0/um/x64/WS2_32.lib")
link_libraries("/Program\ Files\ (x86)/Windows Kits/10/Lib/10.0.17763.0/um/x64/Gdi32.lib")
link_libraries("/Program\ Files\ (x86)/Windows Kits/10/Lib/10.0.17763.0/um/x64/AdvAPI32.lib")
link_libraries("/Program\ Files\ (x86)/Windows Kits/10/Lib/10.0.17763.0/um/x64/User32.lib")

include("/Users/cmaza/cpp/quail/build_Debug/conan_paths.cmake")
