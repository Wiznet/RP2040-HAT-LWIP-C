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
set(PICO_EXTRAS_SRC_DIR "${RP2040_HAT_LWIP_C_SRC_DIR}/libraries/pico-extras")
set(PICO_EXTRAS_LWIP_SRC_DIR "${RP2040_HAT_LWIP_C_SRC_DIR}/libraries/pico-extras/lib/lwip")
set(PICO_SDK_SRC_DIR "${RP2040_HAT_LWIP_C_SRC_DIR}/libraries/pico-sdk")
set(PICO_SDK_TINYUSB_SRC_DIR "${RP2040_HAT_LWIP_C_SRC_DIR}/libraries/lib/tinyusb")
set(RP2040_HAT_C_PATCH_DIR "${RP2040_HAT_LWIP_C_SRC_DIR}/patches")

# Delete untracked files in ioLibrary_Driver
if(EXISTS "${IOLIBRARY_DRIVER_SRC_DIR}/.git")
	message("cleaning ioLibrary_Driver...")
	execute_process(COMMAND ${GIT_EXECUTABLE} -C ${IOLIBRARY_DRIVER_SRC_DIR} clean -fdx)
	execute_process(COMMAND ${GIT_EXECUTABLE} -C ${IOLIBRARY_DRIVER_SRC_DIR} reset --hard)
	message("ioLibrary_Driver cleaned")
endif()

# Delete untracked files in pico-extras
if(EXISTS "${PICO_EXTRAS_SRC_DIR}/.git")
	message("cleaning pico-extras...")
	execute_process(COMMAND ${GIT_EXECUTABLE} -C ${PICO_EXTRAS_SRC_DIR} clean -fdx)
	execute_process(COMMAND ${GIT_EXECUTABLE} -C ${PICO_EXTRAS_SRC_DIR} reset --hard)
	message("pico-extras cleaned")
endif()

# Delete untracked files in pico-sdk
if(EXISTS "${PICO_SDK_SRC_DIR}/.git")
	message("cleaning pico-sdk...")
	execute_process(COMMAND ${GIT_EXECUTABLE} -C ${PICO_SDK_SRC_DIR} clean -fdx)
	execute_process(COMMAND ${GIT_EXECUTABLE} -C ${PICO_SDK_SRC_DIR} reset --hard)
	message("pico-sdk cleaned")
endif()

execute_process(COMMAND ${GIT_EXECUTABLE} -C ${RP2040_HAT_LWIP_C_SRC_DIR} submodule update --init)

# Delete untracked files in pico-extras lwip
if(EXISTS "${PICO_EXTRAS_LWIP_SRC_DIR}/.git")
	message("cleaning pico-extras lwip...")
	execute_process(COMMAND ${GIT_EXECUTABLE} -C ${PICO_EXTRAS_LWIP_SRC_DIR} clean -fdx)
	execute_process(COMMAND ${GIT_EXECUTABLE} -C ${PICO_EXTRAS_LWIP_SRC_DIR} reset --hard)
	message("pico-extras lwip cleaned")
endif()

execute_process(COMMAND ${GIT_EXECUTABLE} -C ${PICO_EXTRAS_SRC_DIR} submodule update --init)

# Delete untracked files in tinyusb
if(EXISTS "${PICO_SDK_TINYUSB_SRC_DIR}/.git")
	message("cleaning pico-sdk tinyusb...")
	execute_process(COMMAND ${GIT_EXECUTABLE} -C ${PICO_SDK_TINYUSB_SRC_DIR} clean -fdx)
	execute_process(COMMAND ${GIT_EXECUTABLE} -C ${PICO_SDK_TINYUSB_SRC_DIR} reset --hard)
	message("pico-sdk tinyusb cleaned")
endif()

execute_process(COMMAND ${GIT_EXECUTABLE} -C ${PICO_SDK_SRC_DIR} submodule update --init)

# pico-extras patch
message("submodules pico-extras initialised")

file(GLOB PICO_EXTRAS_PATCHES 
	"${RP2040_HAT_C_PATCH_DIR}/01_pico-extras_lwip_cmakelists.patch" 
	)

foreach(PICO_EXTRAS_PATCH IN LISTS PICO_EXTRAS_PATCHES)
	message("Running patch ${PICO_EXTRAS_PATCH}")
	execute_process(
		COMMAND ${GIT_EXECUTABLE} apply --ignore-whitespace ${PICO_EXTRAS_PATCH}
		WORKING_DIRECTORY ${PICO_EXTRAS_SRC_DIR}
	)
endforeach()

# pico-extras lwip patch
message("submodules pico-extras lwip initialised")

file(GLOB PICO_EXTRAS_LWIP_PATCHES 	
	"${RP2040_HAT_C_PATCH_DIR}/02_pico-extras_lwip_dns.patch"	
	"${RP2040_HAT_C_PATCH_DIR}/03_pico-extras_lwip_opt.patch"
	)

foreach(PICO_EXTRAS_LWIP_PATCH IN LISTS PICO_EXTRAS_LWIP_PATCHES)
	message("Running patch ${PICO_EXTRAS_LWIP_PATCH}")
	execute_process(
		COMMAND ${GIT_EXECUTABLE} apply --ignore-whitespace ${PICO_EXTRAS_LWIP_PATCH}				
		WORKING_DIRECTORY ${PICO_EXTRAS_LWIP_SRC_DIR}
	)
endforeach()
