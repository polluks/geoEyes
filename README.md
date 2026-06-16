# geoEyes

xeyes lookalike for GEOS on the Commodore 64, using VIC-II hardware sprites.

Based on [a2d desk.acc/eyes.s](https://github.com/a2stuff/a2d/blob/main/desk.acc/eyes.s).
xeyes artwork (public domain) by Tizio via Wikimedia Commons.

## Build

Requires [cc65](https://cc65.github.io/).

    make

Output: `eyes.cvt` — GEOS application binary.

## Usage

Copy `eyes.cvt` to a C64 GEOS disk and launch from the desktop.
Two eyes follow the mouse cursor. Press any key to exit.
