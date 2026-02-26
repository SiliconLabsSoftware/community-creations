# Contributing Guide

This repository is a directory of community projects built with Silicon Labs technologies. Instead of hosting project source code here, we accept contributions as **directory entries** that link to a GitHub repository and provide a short description.

If you want to share your project, you will submit a pull request that edits the appropriate listing table under `projects/`.

## What You Can Submit

Each new entry must include:

- A link to a **public GitHub repository** for your project
- A **1-2 sentence description**
- A link to your **GitHub profile**

## What We Do Not Accept

- Project source code added to this repository
- Project binaries or build artifacts
- Full application trees exported from IDEs (for example `.slcp`, `.sls`)
- Large media files (use links instead)

## Where to Add Your Entry

Pick the single best-matching category under `projects/` and add a row to the table in the corresponding `*_applications/README.md` file.

Examples:

- `projects/bluetooth/bluetooth_applications/README.md`
- `projects/zigbee/zigbee_applications/README.md`
- `projects/wifi/wifi_applications/README.md`

To reduce merge conflicts when multiple PRs are opened around the same time, please append your new row to the bottom of the table and do not reorder or edit existing rows.

## Requirements

- The linked GitHub repository must be **public** and accessible.
- The linked repository must include a **license file** at the repository root.
- The project must be related to Silicon Labs hardware, software, or technologies.

Maintainers may request changes to improve clarity or may remove/delist entries (for example, if links break).

## Recommended Contents In Your Linked Repository

To help others reproduce and learn from your project, include a `README.md` with:

- Overview: what it does and what Silicon Labs parts/tech it uses
- Hardware: board/dev kit and any peripherals
- Software/tooling: SDK/tool versions and prerequisites
- Build/flash/run instructions and expected results
- License file

## How To Submit (Step-by-Step)

1. Fork this repository.
2. Create a branch in your fork.
3. Edit the relevant `projects/**/**_applications/README.md` listing table and append your row at the bottom.
4. Commit your change.
5. Open a pull request and fill out the pull request template (include the GitHub URL and the category file you edited).

## Contributor License Agreement (CLA)

This repository uses a CLA process. See the CLA document here:
https://github.com/SiliconLabsSoftware/agreements-and-guidelines/blob/main/contributor_license_agreement.md

## Reporting Issues

- For issues with a listed project, please use that project's GitHub repository (issues/discussions).
- For issues with this directory (broken links, wrong category, documentation problems), open an issue in this repository.
