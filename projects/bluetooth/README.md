<table border="0">
  <tr>
    <td align="left" valign="middle">
    <h1>EFR32 Bluetooth Application Examples</h1>
  </td>
  <td align="left" valign="middle">
    <a href="https://www.silabs.com/wireless/bluetooth">
      <img src="http://pages.silabs.com/rs/634-SLU-379/images/WGX-transparent.png"  title="Silicon Labs Gecko and Wireless Gecko MCUs" alt="EFM32 32-bit Microcontrollers" width="250"/>
    </a>
  </td>
  </tr>
</table>

# Silicon Labs Bluetooth #

Silicon Labs' Bluetooth chipsets deliver high performance, low energy, and easy-to-use Bluetooth solutions integrated into a small form factor package. The ultra-low power operating modes and fast wake-up times of Silicon Labs' energy-friendly 32-bit MCUs, combined with the low transmit and receive power consumption of the Bluetooth radio, result in a solution optimized for battery-powered applications.

This repository is dedicated to the community—created by developers, for developers. Here, you can find, share, and contribute Bluetooth application examples, best practices, and innovative ideas. We encourage you to participate by submitting your own projects, improvements, and feedback. Together, we can accelerate development, solve challenges, and inspire new solutions for Bluetooth-enabled devices. Join the conversation, collaborate, and help grow a vibrant ecosystem around Silicon Labs Bluetooth technology!

## Submodules ##

- **bluetooth_applications:**
  Complete application examples utilizing the Bluetooth stack.

- **bluetooth_peripherals:**
  Examples demonstrating integration of SoC peripherals with the Bluetooth stack.

- **SDK:**
  - Simplicity Demos v2025.6.1
  - Simplicity SDK Suite v2025.6.1: Bluetooth 10.1.0, Bluetooth Mesh 9.0.1, Connect 4.1.1, EmberZNet 8.2.1.0, Micrium OS Kernel 5.18.01, OpenThread 2.7.1.0 (GitHub-fb0446f53), Platform 5.2.1.0, RAIL 2.19.1, USB 1.5.1.0, Wi-SUN 2.7.0, Z-Wave SDK 7.24.1.0
  - Stackless applications(version 1.0.0)
- **bluetooth_stack_features:**
  Examples showcasing key Bluetooth stack features, including:
  - Advertising
  - Connections
  - GATT Protocol
  - Security
  - Persistent Storage
  - Firmware Upgrade
  - NCP (Network Co-Processor)
  - System and Performance

- **bluetooth_mesh_applications:**
  Complete application examples using the Bluetooth Mesh stack.

- **bluetooth_mesh_stack_features:**
  Examples demonstrating key features of the Bluetooth Mesh stack.

## Recommended Devices ##

| **Series 1 Devices** | **Series 2 Devices** |
|----------------------|----------------------|
| [EFR32MG1](https://www.silabs.com/wireless/zigbee/efr32mg1-series-1-socs) | [EFR32MG21](https://www.silabs.com/wireless/zigbee/efr32mg21-series-2-socs) |
| [EFR32BG1](https://www.silabs.com/wireless/bluetooth/efr32bg1-series-1-socs) | [EFR32BG21](https://www.silabs.com/wireless/bluetooth/efr32bg21-series-2-socs) |
| [EFR32MG12](https://www.silabs.com/wireless/zigbee/efr32mg12-series-1-socs) | [EFR32MG22](https://www.silabs.com/wireless/zigbee/efr32mg21-series-2-socs) |
| [EFR32BG12](https://www.silabs.com/wireless/bluetooth/efr32bg12-series-1-socs) | [EFR32BG22](https://www.silabs.com/wireless/bluetooth/efr32bg22-series-2-socs) |
| [EFR32MG13](https://www.silabs.com/wireless/zigbee/efr32mg13-series-1-socs) | [EFR32MG24](https://www.silabs.com/wireless/zigbee/efr32mg24-series-2-socs)                      |
| [EFR32BG13](https://www.silabs.com/wireless/bluetooth/efr32bg13-series-1-socs) | [EFR32BG24](https://www.silabs.com/wireless/bluetooth/efr32bg24-series-2-socs)                      |
| [EFR32MG14](https://www.silabs.com/wireless/zigbee/efr32mg14-series-1-socs) | [EFR32MG26](https://www.silabs.com/wireless/zigbee/efr32mg26-series-2-socs)                     |
| [EFR32BG14](https://www.silabs.com/wireless/bluetooth/efr32bg14-series-1-socs) | [EFR32BG26](https://www.silabs.com/wireless/bluetooth/efr32bg26-series-2-socs)                     |

## Bootloader ##

Bluetooth Mesh projects do not include bootloaders by default, but a bootloader is required for device firmware upgrade (DFU) and reliable field updates. To ensure your mesh application runs correctly, you must either:

- Flash the appropriate bootloader to your device, or
- Remove DFU functionality from your project.

If your mesh application does not require DFU, you can remove the Bootloader Application Interface software component and its dependencies. This places your application code at the start of flash memory, eliminating the need for a bootloader. Note that removing DFU functionality will prevent future firmware upgrades via DFU.

To add a bootloader, you have two options:

- **Create and flash a bootloader project:**
    Build and flash a bootloader suitable for your device:
  - For Series 1 devices, use a Bluetooth in-place OTA DFU or Internal Storage bootloader.
  - For Series 2 devices, use a Bluetooth Apploader OTA DFU bootloader.

- **Use a pre-compiled demo:**
    Running a pre-compiled demo from the Launcher view will flash both the bootloader and application images to your device. Afterward, you can overwrite the demo application with your own image, while the bootloader remains intact.
  - For mesh projects, use the Bluetooth Mesh - Light or Bluetooth Mesh - Switch demo.

**Important Notes:**

- When flashing your application, use the `.hex` or `.s37` output files. Flashing `.bin` files may erase the bootloader.
- On Series 1 devices (EFR32xG1x), both first and second stage bootloaders must be flashed. This can be done by flashing the `-combined.s37` file generated by the bootloader project.
- On Series 2 devices, mesh example projects require a Bluetooth Apploader OTA DFU bootloader by default. This bootloader requires significant flash space and shifts the application start address. The shift is managed automatically by the Apploader Support for Applications software component, which is installed by default. If you choose a different bootloader type, remove this component to revert the application start address. Note that removing it disables Apploader OTA DFU, but you can still implement application-level OTA DFU by installing the Application OTA DFU software component.

For further details, refer to [UG103.6: Bootloader Fundamentals](https://www.silabs.com/documents/public/user-guides/ug103-06-fundamentals-bootloading.pdf) and [UG489: Silicon Labs Gecko Bootloader User's Guide for GSDK 4.0 and Higher](https://www.silabs.com/documents/public/user-guides/ug489-gecko-bootloader-user-guide-gsdk-4.pdf).

## Documentation ##

Official documentation can be found at our [Developer Documentation](https://docs.silabs.com/bluetooth/latest/) page.

## Reporting Bugs/Issues and Posting Questions and Comments ##

To report bugs, please create a new "Issue" in the "Issues" section of this repo. Please reference the board, project, and source files associated with the bug, and reference line numbers. If you are proposing a fix, also include information on the proposed fix. Since these examples are provided as-is, there is no guarantee that these examples will be updated to fix these issues.

Questions and comments related to these examples should be made by creating a new "Issue" in the "Issues" section of this repo.
