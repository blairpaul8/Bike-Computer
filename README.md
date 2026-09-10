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

- Run docker container
  - This will drop you into the container at a bash prompt
    you should be in /workspace
  - All file interactions will also take effect outside the container
    in the repo.

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

## Hardware and Software Details

### Hardware Components

- ESP32

Add other Components here

### Mobile App Architecture
