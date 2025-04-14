# Silicon Labs Energy Harvesting Applications - Simplicity SDK Extension #

This repo contains documentation, demos, examples and all the code needed for the Energy Harvesting extension. The content of the repository is meant to be accessed through Silicon Labs Simplicity Studio and should not be used directly.

## Project Examples ##
[Energy Harvesting Examples](https://github.com/SiliconLabs/energy_harvesting_applications)

## Requirements ##

### Hardware ###

- [xG22-EK8200A](https://www.silabs.com/development-tools/wireless/efr32xg22e-energy-harvesting-explorer-kit?tab=overview) - EFR32xG22E Energy Harvesting Explorer Kit
- [xG24-EK2703A](https://www.silabs.com/development-tools/wireless/efr32xg24-explorer-kit?tab=overview) - xG24 Explorer Kit

### Software ###

- [Simplicity Studio v5 IDE](https://www.silabs.com/developers/simplicity-studio)
- [Simplicity SDK Version 2024.6.2](https://github.com/SiliconLabs/simplicity_sdk/releases/tag/v2024.6.2)
- [Simplicity Connect Mobile App](https://www.silabs.com/developer-tools/simplicity-connect-mobile-app)

### External tools ###

- Power profiling tool: [Qoitech Otii Ace Pro](https://www.qoitech.com/otii-ace/)

## How to add to Simplicity Studio IDE ##

- Clone the repository somewhere on your PC

  `git clone https://github.com/SiliconLabs/energy_harvesting_applications.git`

- Add the SDK extension to the SiSDK

  - In Simplicity Studio go to **Preferences** → **Simplicity Studio** → **SDKs** and select the Simplicity SDK Suite to which the SDK extension will be added. Click **Add Extension…**
  - Click **Browse** and navigate to the root folder of the package above and click **Select Folder**.
  - The SDK extension should be displayed in the Detected SDK Extension window with the **Energy Harvesting** name, version, and path. Click **OK** and then **Trust** and **Apply and Close**.
  - Restart Simplicity Studio

> Make sure that **Evaluation** quality software components are enabled in the Software Component view.


## Drivers ##

<table>
    <tbody>
        <tr>
            <td colspan="3" align="left"><b>Power Management</b></td>
        </tr>
      <tr></tr>
        <tr>
            <td>&nbsp;&nbsp;&nbsp;&nbsp;AEM13920 - Energy Harvesting PMIC</td>
            <td>
                <a href="./driver/public/aem13920/">Driver</a>
            </td>
            <td>
                <a href="./driver/docs/AEM13920.md">Document</a>
            </td>
        </tr>
    </tbody>
</table>

## Report Bugs & Get Support ##

To report bugs in the [Energy Harvesting Extension](https://github.com/SiliconLabs/energy_harvesting_applications) projects, you can either

- create a new "Issue" in the "Issues" section of this repo
- or report any issues you found to us via [Silicon Labs Community](https://www.silabs.com/community)

Please reference the board, project, and source files associated with the bug, and reference line numbers. If you are proposing a fix, also include information on the proposed fix. Since these examples are provided as-is, there is no guarantee that these examples will be updated to fix these issues.