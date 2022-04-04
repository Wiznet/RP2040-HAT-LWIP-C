# How to Test iPerf Example



## Step 1: Prepare software

The following serial terminal program and iPerf are required for iPerf example test, download and install from below links.

Note that iPerf uses iPerf 2.0.9.

- [**Tera Term**][link-tera_term]
- [**iPerf**][link-iperf]



## Step 2: Prepare hardware

If you are using W5100S-EVB-Pico or W5500-EVB-Pico, you can skip '1. Combine...'

1. Combine WIZnet Ethernet HAT with Raspberry Pi Pico.

2. Connect ethernet cable to WIZnet Ethernet HAT, W5100S-EVB-Pico or W5500-EVB-Pico ethernet port.

3. Connect Raspberry Pi Pico, W5100S-EVB-Pico or W5500-EVB-Pico to desktop or laptop using 5 pin micro USB cable.



## Step 3: Setup iPerf Example

To test the iPerf example, minor settings shall be done in code.

1. Setup SPI port and pin in 'w5x00_spi.h' in 'RP2040-HAT-LWIP-C/port/ioLibrary_Driver/' directory.

Setup the SPI interface you use.

```cpp
/* SPI */
#define SPI_PORT spi0

#define PIN_SCK 18
#define PIN_MOSI 19
#define PIN_MISO 16
#define PIN_CS 17
#define PIN_RST 20
```

If you want to test with the iPerf example using SPI DMA, uncomment USE_SPI_DMA.

```cpp
/* Use SPI DMA */
//#define USE_SPI_DMA // if you want to use SPI DMA, uncomment.
```

2. Setup network configuration such as IP in 'w5x00_lwiperf.c' which is the iPerf example in 'RP2040-HAT-LWIP-C/examples/lwiperf/' directory.

Setup IP and other network settings to suit your network environment.

```cpp
// Initialize network configuration
IP4_ADDR(&g_ip, 192, 168, 11, 2);
IP4_ADDR(&g_mask, 255, 255, 255, 0);
IP4_ADDR(&g_gateway, 192, 168, 11, 1);
```

3. Setup iPerf configuration in 'w5x00_lwiperf.c' in 'RP2040-HAT-LWIP-C/examples/lwiperf/' directory.

```cpp
/* Port */
#define PORT_LWIPERF 5001
```



## Step 4: Build

1. After completing the iPerf example configuration, click 'build' in the status bar at the bottom of Visual Studio Code or press the 'F7' button on the keyboard to build.

2. When the build is completed, 'w5x00_lwiperf.uf2' is generated in 'RP2040-HAT-LWIP-C/build/examples/lwiperf/' directory.



## Step 5: Upload and Run

1. While pressing the BOOTSEL button of Raspberry Pi Pico, W5100S-EVB-Pico or W5500-EVB-Pico power on the board, the USB mass storage 'RPI-RP2' is automatically mounted.

![][link-raspberry_pi_pico_usb_mass_storage]

2. Drag and drop 'w5x00_lwiperf.uf2' onto the USB mass storage device 'RPI-RP2'.

3. Connect to the serial COM port of Raspberry Pi Pico, W5100S-EVB-Pico or W5500-EVB-Pico with Tera Term.

![][link-connect_to_serial_com_port]

4. Reset your board.

5. If the iPerf example works normally on Raspberry Pi Pico, W5100S-EVB-Pico or W5500-EVB-Pico, you can see the IP of Raspberry Pi Pico, W5100S-EVB-Pico or W5500-EVB-Pico and the TCP server is open.

![][link-see_network_information_of_raspberry_pi_pico_and_open_tcp_server]

6. Run command prompt to enter the iPerf command and move to iPerf path.

```cpp
/* Change directory */
// change to the 'iperf-x.x.x-winxx' directory.
cd [user path]/iperf-x.x.x-winxx

// e.g.
cd D:/iperf-2.0.9-win64
```

![][link-run_command_prompt]

![][link-move_to_iperf_path]

7. In the command prompt, enter the following command to connect to Raspberry Pi Pico, W5100S-EVB-Pico or W5500-EVB-Pico running as a TCP server and test.

```cpp
/* Network performance measurement test */
iperf -c [connecting to] -i [seconds between periodic bandwidth reports] -t [time in seconds to transmit for]

// e.g.
iperf -c 192.168.11.253 -i 1 -t 10
```

![][link-run_network_performance_measurement_test]



<!--
Link
-->

[link-tera_term]: https://osdn.net/projects/ttssh2/releases/
[link-iperf]: https://iperf.fr/iperf-download.php
[link-raspberry_pi_pico_usb_mass_storage]: https://github.com/Wiznet/RP2040-HAT-LWIP-C/blob/main/static/images/lwiperf/raspberry_pi_pico_usb_mass_storage.png
[link-connect_to_serial_com_port]: https://github.com/Wiznet/RP2040-HAT-LWIP-C/blob/main/static/images/lwiperf/connect_to_serial_com_port.png
[link-see_network_information_of_raspberry_pi_pico_and_open_tcp_server]: https://github.com/Wiznet/RP2040-HAT-LWIP-C/blob/main/static/images/lwiperf/see_network_information_of_raspberry_pi_pico_and_open_tcp_server.png
[link-run_command_prompt]: https://github.com/Wiznet/RP2040-HAT-LWIP-C/blob/main/static/images/lwiperf/run_command_prompt.png
[link-move_to_iperf_path]: https://github.com/Wiznet/RP2040-HAT-LWIP-C/blob/main/static/images/lwiperf/move_to_iperf_path.png
[link-run_network_performance_measurement_test]: https://github.com/Wiznet/RP2040-HAT-LWIP-C/blob/main/static/images/lwiperf/run_network_performance_measurement_test.png
