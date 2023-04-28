set(TARGET_NAME w5x00_loopback)
set(LWIP_PATH ${PICO_SDK_PATH}/lib/lwip)
set(TARGET_FILES    
    w5x00_loopback.c
    ${CMAKE_CURRENT_SOURCE_DIR}/tcp_echoserver/tcp_echoserver.c
    ${CMAKE_CURRENT_SOURCE_DIR}/tcp_echoclient/tcp_echoclient.c
    )

add_executable(${TARGET_NAME} ${TARGET_FILES})

target_include_directories(w5x00_loopback PUBLIC
        ${LWIP_PATH}/src/include
        ${CMAKE_CURRENT_SOURCE_DIR}/tcp_echoserver
        ${CMAKE_CURRENT_SOURCE_DIR}/tcp_echoclient
        )

# This is required to include lwipopts.h
target_include_directories(pico_lwip INTERFACE        
        ${PORT_DIR}/lwip        
        )

target_link_libraries(${TARGET_NAME} PRIVATE
        pico_stdlib
        hardware_spi
        hardware_dma
        ETHERNET_FILES
        IOLIBRARY_FILES
        LWIP_FILES
        )

pico_enable_stdio_usb(${TARGET_NAME} 1)
pico_enable_stdio_uart(${TARGET_NAME} 0)

pico_add_extra_outputs(${TARGET_NAME})
