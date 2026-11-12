idf_component_register(
    SRCS
        # Main Entry Point
        "bike-computer.c"

        # Drivers
        "drivers/ble/BLE.cpp"
        "drivers/uart/Uart.cpp"
        "display/DisplayTask.cpp"
        "drivers/spi/Spi.cpp"
        "display/St7796Display.cpp"

        # GPS
        "gps/GpsTask.cpp"
        "gps/Gps.cpp"

        # Heart Rate
        "HeartRate/HeartRateMonitor.cpp"
        "HeartRate/HeartRateTask.cpp"
        "HeartRate/HeartRateServer.cpp"
    INCLUDE_DIRS
        "."
)
