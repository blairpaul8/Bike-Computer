# VeloSync Bike Computer

Embedded Bike Computer Senior Design COSC-401 @ UTK

The goal of this project is to create a custom bike computer
with GPS support and Bluetooth connections. Along with a mobile
app to view activities, and upload to Strava. See hardware and
software detail below.

## Interacting with the project

This project uses a Makefile as a facade to mask lengthly
CLI commands. Common commands will be covered here. Refer
to the Makefile for additional commands.

- Build Docker container

``` bash
make docker-build
```

- Run docker container with or without usb connected
  - This will drop you into the container at a bash prompt
    you should be in /workspace
  - All file interactions will also take effect outside the container
    in the repo.

``` bash
make docker-run-usb
```
``` bash
make docker-run
```

- Build the project

``` bash
make build
```

- Flash to ESP32

``` bash
make flash
```

### Windows Setup

To connect the esp32 to windows, you must attach the esp usb port to ubuntu to then
be passed into the docker container.

- Bind the usb port(Only needs to be done once)

```bash
make usb-prep-bind-w
make usb-bind-w BUSID=<bus id of Silicon Labs CP210x USB to UART Bridge>
```

- Attach usb port to ubuntu(Needs to be done every time and Ubuntu terminal needs to be open)

```bash
make usb-attach-w BUSID=<bus id of Silicon Labs CP210x USB to UART Bridge>
```

## Hardware and Software Details

### Hardware Components

- ESP32

Add other Components here

### Mobile App Architecture
