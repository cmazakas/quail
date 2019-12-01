find_package(Catch2 CONFIG REQUIRED)

include(Catch)

function(quail_add_test test_name)
  add_executable(${test_name} "${test_name}.cpp")
  target_link_libraries(${test_name} PRIVATE quail Catch2::Catch2)
  catch_discover_tests(${test_name})
endfunction()

quail_add_test(hkdf_expand)
quail_add_test(hkdf_extract)
quail_add_test(version_negotiation_packet)
