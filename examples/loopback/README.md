# How to Test Loopback Example



## Step 1: Prepare software

The following serial terminal programs are required for Loopback example test, download and install from below links.

- [**Tera Term**][link-tera_term]
- [**Hercules**][link-hercules]



## Step 2: Prepare hardware

If you are using W5100S-EVB-Pico or W5500-EVB-Pico, you can skip '1. Combine...'

1. Combine WIZnet Ethernet HAT with Raspberry Pi Pico.

2. Connect ethernet cable to WIZnet Ethernet HAT, W5100S-EVB-Pico or W5500-EVB-Pico ethernet port.

3. Connect Raspberry Pi Pico, W5100S-EVB-Pico or W5500-EVB-Pico to desktop or laptop using 5 pin micro USB cable.



## Step 3: Setup Loopback Example

To test the Loopback example, minor settings shall be done in code.

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

If you want to test with the Loopback example using SPI DMA, uncomment USE_SPI_DMA.

```cpp
/* Use SPI DMA */
//#define USE_SPI_DMA // if you want to use SPI DMA, uncomment.
```

2. Setup network configuration such as IP in 'w5x00_loopback.c' which is the Loopback example in 'RP2040-HAT-LWIP-C/examples/loopback/' directory.

Setup IP and other network settings to suit your network environment.

```cpp
// Initialize network configuration
IP4_ADDR(&g_ip, 192, 168, 11, 2);
IP4_ADDR(&g_mask, 255, 255, 255, 0);
IP4_ADDR(&g_gateway, 192, 168, 11, 1);
```

3. Allows users to choose between echoserver mode and echoclient mode.

- Setup loopback configuration in 'tcp_echoserver.c' in 'RP2040-HAT-LWIP-C/examples/loopback/tcp_echoserver/' directory.

```cpp
/* Source Port */
#define PORT_LOOPBACK 5001
```

- Setup loopback configuration in 'tcp_echoclient.c' in 'RP2040-HAT-LWIP-C/examples/loopback/tcp_echoclient/' directory.

```c
/* Source Port */
#define PORT_LOOPBACK 5001

/* Destination IP address & port */
static u8_t dest_ip[4] = {192, 168, 11, 74};
static u16_t dest_port = 5000;
```



## Step 4: Build

1. After completing the Loopback example configuration, click 'build' in the status bar at the bottom of Visual Studio Code or press the 'F7' button on the keyboard to build.

2. When the build is completed, 'w5x00_loopback.uf2' is generated in 'RP2040-HAT-LWIP-C/build/examples/loopback/' directory.

3. While pressing the BOOTSEL button of Raspberry Pi Pico, W5100S-EVB-Pico or W5500-EVB-Pico power on the board, the USB mass storage 'RPI-RP2' is automatically mounted.

![][link-raspberry_pi_pico_usb_mass_storage]

4. Drag and drop 'w5x00_loopback.uf2' onto the USB mass storage device 'RPI-RP2'.



## Step 5: Upload and Run

**TCP Server mode**

1. Connect to the serial COM port of Raspberry Pi Pico, W5100S-EVB-Pico or W5500-EVB-Pico with Tera Term.

![][link-connect_to_serial_com_port]

2. Reset your board.

3. If the Loopback example works normally on Raspberry Pi Pico, W5100S-EVB-Pico or W5500-EVB-Pico, you can see the IP of Raspberry Pi Pico, W5100S-EVB-Pico or W5500-EVB-Pico and the loopback server is open.

![][link-see_network_information_of_raspberry_pi_pico_and_open_loopback_server]

4. Connect to the open loopback server using Hercules TCP client. When connecting to the loopback server, you need to enter is the IP that was configured in Step 3, the port is 5001 by default.

![][link-connect_to_loopback_server_using_hercules_tcp_client_1]

5. Once connected if you send data to the loopback server, you should be able to receive back the sent message.

![][link-receive_back_sent_message]

**TCP Client mode**

1. Connect to the serial COM port of Raspberry Pi Pico, W5100S-EVB-Pico or W5500-EVB-Pico with Tera Term.

![][link-connect_to_serial_com_port]

2. Reset your board.
3. Open Hercules and set the port for the W5100S-EVB-Pico or W5500-EVB-Pico to connect to 5000.

![][link-hercules_server_open]

4. If the W5100S-EVB-Pico or W5500-EVB-Pico works properly, you should see it connect to the server.

![][link-lwip_client_connect]

5. Once connected if you send data to the loopback client, you should be able to receive back the sent message.

![][link-client_echo_data]

<!--
Link
-->

[link-tera_term]: https://osdn.net/projects/ttssh2/releases/
[link-hercules]: https://www.hw-group.com/software/hercules-setup-utility
[link-raspberry_pi_pico_usb_mass_storage]: https://github.com/Wiznet/RP2040-HAT-LWIP-C/blob/main/static/images/loopback/raspberry_pi_pico_usb_mass_storage.png
[link-connect_to_serial_com_port]: https://github.com/Wiznet/RP2040-HAT-LWIP-C/blob/main/static/images/loopback/connect_to_serial_com_port.png
[link-see_network_information_of_raspberry_pi_pico_and_open_loopback_server]: https://github.com/Wiznet/RP2040-HAT-LWIP-C/blob/main/static/images/loopback/see_network_information_of_raspberry_pi_pico_and_open_loopback_server.png
[link-connect_to_loopback_server_using_hercules_tcp_client_1]: https://github.com/Wiznet/RP2040-HAT-LWIP-C/blob/main/static/images/loopback/connect_to_loopback_server_using_hercules_tcp_client.png
[link-receive_back_sent_message]: https://github.com/Wiznet/RP2040-HAT-LWIP-C/blob/main/static/images/loopback/receive_back_sent_message.png
[link-hercules_server_open]:https://github.com/Wiznet/RP2040-HAT-LWIP-C/blob/main/static/images/loopback/hercules_server_open.png
[link-lwip_client_connect]:github.com/Wiznet/RP2040-HAT-LWIP-C/blob/main/static/images/loopback/lwip_client_connect.png
[link-client_echo_data]:github.com/Wiznet/RP2040-HAT-LWIP-C/blob/main/static/images/loopback/client_echo_data.png
