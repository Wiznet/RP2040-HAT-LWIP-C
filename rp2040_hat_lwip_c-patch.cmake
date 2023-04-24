# CMake minimum required version
cmake_minimum_required(VERSION 3.12)

# Find git
find_package(Git)

if(NOT Git_FOUND)
	message(FATAL_ERROR "Could not find 'git' tool for RP2040-HAT-LWIP-C patching")
endif()

message("RP2040-HAT-LWIP-C patch utils found")

set(RP2040_HAT_LWIP_C_SRC_DIR "${CMAKE_CURRENT_SOURCE_DIR}")
set(IOLIBRARY_DRIVER_SRC_DIR "${RP2040_HAT_LWIP_C_SRC_DIR}/libraries/ioLibrary_Driver")
set(PICO_SDK_SRC_DIR "${RP2040_HAT_LWIP_C_SRC_DIR}/libraries/pico-sdk")
set(PICO_SDK_LWIP_SRC_DIR "${PICO_SDK_SRC_DIR}/lib/lwip")
set(PICO_SDK_TINYUSB_SRC_DIR "${PICO_SDK_SRC_DIR}/lib/tinyusb")
set(RP2040_HAT_C_PATCH_DIR "${RP2040_HAT_LWIP_C_SRC_DIR}/patches")

# Delete untracked files in ioLibrary_Driver
if(EXISTS "${IOLIBRARY_DRIVER_SRC_DIR}/.git")
	message("cleaning ioLibrary_Driver...")
	execute_process(COMMAND ${GIT_EXECUTABLE} -C ${IOLIBRARY_DRIVER_SRC_DIR} clean -fdx)
	execute_process(COMMAND ${GIT_EXECUTABLE} -C ${IOLIBRARY_DRIVER_SRC_DIR} reset --hard)
	message("ioLibrary_Driver cleaned")
endif()

# Delete untracked files in pico-sdk
if(EXISTS "${PICO_SDK_SRC_DIR}/.git")
	message("cleaning pico-sdk...")
	execute_process(COMMAND ${GIT_EXECUTABLE} -C ${PICO_SDK_SRC_DIR} clean -fdx)
	execute_process(COMMAND ${GIT_EXECUTABLE} -C ${PICO_SDK_SRC_DIR} reset --hard)
	message("pico-sdk cleaned")
endif()

execute_process(COMMAND ${GIT_EXECUTABLE} -C ${RP2040_HAT_LWIP_C_SRC_DIR} submodule update --init)

# Delete untracked files in lwip
if(EXISTS "${PICO_SDK_LWIP_SRC_DIR}/.git")
	message("cleaning pico-sdk lwip...")
	execute_process(COMMAND ${GIT_EXECUTABLE} -C ${PICO_SDK_LWIP_SRC_DIR} clean -fdx)
	execute_process(COMMAND ${GIT_EXECUTABLE} -C ${PICO_SDK_LWIP_SRC_DIR} reset --hard)
	message("pico-sdk lwip cleaned")
endif()

# Delete untracked files in tinyusb
if(EXISTS "${PICO_SDK_TINYUSB_SRC_DIR}/.git")
	message("cleaning pico-sdk tinyusb...")
	execute_process(COMMAND ${GIT_EXECUTABLE} -C ${PICO_SDK_TINYUSB_SRC_DIR} clean -fdx)
	execute_process(COMMAND ${GIT_EXECUTABLE} -C ${PICO_SDK_TINYUSB_SRC_DIR} reset --hard)
	message("pico-sdk tinyusb cleaned")
endif()

execute_process(COMMAND ${GIT_EXECUTABLE} -C ${PICO_SDK_SRC_DIR} submodule update --init)
