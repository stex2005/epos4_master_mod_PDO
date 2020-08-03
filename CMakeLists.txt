file(GLOB THIS_APP_SOURCES *.c *.cpp epos4_driver/*.cpp)
file(GLOB THIS_APP_HEADERS *.h epos4_driver/esmacat_epos4_mod.h)

include_directories(epos4_driver)

get_filename_component(THIS_APP_NAME ${CMAKE_CURRENT_SOURCE_DIR} NAME)
message(STATUS "Example application, ${THIS_APP_NAME}, is ready to build")

add_executable(${THIS_APP_NAME} ${THIS_APP_SOURCES} ${THIS_APP_HEADERS})
target_link_libraries(${THIS_APP_NAME} pthread rt esmacat_core esmacat_driver)

file(WRITE ${CMAKE_CURRENT_BINARY_DIR}/tmp/run.sh
"sudo ./${THIS_APP_NAME} &
sudo ${PROJECT_BINARY_DIR}/esmacat_core/application/ecat_main")

file (COPY ${CMAKE_CURRENT_BINARY_DIR}/tmp/run.sh DESTINATION ${CMAKE_CURRENT_BINARY_DIR} FILE_PERMISSIONS OWNER_EXECUTE OWNER_WRITE OWNER_READ)
file(REMOVE_RECURSE ${CMAKE_CURRENT_BINARY_DIR}/tmp)
