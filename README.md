# qFlipper

### Flipper companion application (desktop)

![qFlipper screenshot](screenshot.png)

## Features:
* Update Flipper's firmware and supplemental data with a press of one button
* Repair a broken fimware installation
* Stream Flipper's display and control it remotely
* Install firmware from a `.dfu` file
* Backup and restore settings, progress and pairing data
* Automatic self-update feature
* Command line interface

## Build:
### Windows:

Build requirements:
- MS Visual Studio 2019 or newer
- Qt (MSVC build) >= 5.15.0
- Windows Driver Kit (to build libwdi)

Edit `build_windows.bat` to adjust to your build environment and then run:
```cmd
build_windows.bat
```

### Linux:

```sh
docker-compose exec dev ./build_linux.sh
```

If building qFlipper as a part of a native package, it is possible to disable the built-in application update feature by passing `DEFINES+=DISABLE_APPLICATION_UPDATES` to the `qmake` program.

### MacOS:

Build requirements:

- Xcode or command line tools
- Qt >= 5.15.0
- Libusb

If you want to sign binaries, set `SIGNING_KEY` environment variable:

	export SIGNING_KEY="Your Developer Key ID"

Building, signing and creating package:

	./build_mac.sh

Resulting image can be found in: `build_mac/qFlipper.dmg`

## Run:

### Linux:
```sh
./build/qFlipper-x86_64.AppImage
```

or just launch the file above from your favourite file manager.
You will likely need to set up udev rules in order to use qFlipper as a normal user. It can be done automatically by running [this script](setup_rules.sh):
```sh
chmod +x setup_rules.sh
./setup_rules.sh
```

### Nix
Clone the repo and run this in it:
```
nix build --experimental-features "nix-command flakes" '.?submodules=1'
```
Built binaries will be located in `./result/bin` folder.
You can install it in the environment afterwards:
```
nix-env -i ./result
```

If you want to have yourself a development environment with all the dependencies you need to build it:
```
nix develop --experimental-features "nix-command flakes" '.?submodules=1'
```

If you wish to never again write `--experimental-features "nix-command flakes"`:
```
mkdir -p ~/.config/nix
echo "experimental-features = nix-command flakes" >> ~/.config/nix/nix.conf
```

## Project structure:
- `application` - The main graphical application, written mostly in QML.
- `backend` - The backend library, written in C++. Takes care of most of the logic.
- `dfu` - Low level library for accessing USB and DFU devices.
- `tool` - The command line interface, provides nearly all main application's functionality.
- `3rdparty` - Third-party libraries.
- `driver-tool` - DFU driver installation tool for Windows (based on `libwdi`).
- `docker` - Docker configuration files..
- `installer-assets` - Supplementary data for deployment.

## Known bugs:

* Sometimes Flipper's serial port doesn't get recognised by the OS, which leads to firmware update errors. This is a firmware issue.
* On some systems, there is noticeable flicker during opening, closing or resizing of the log area
