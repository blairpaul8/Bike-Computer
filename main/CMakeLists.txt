idf_component_register(
    SRCS
        # Main Entry Point
        "bike-computer.c"

        # Drivers
        "drivers/uart/Uart.cpp"
        "drivers/spi/Spi.cpp"
        "drivers/ble/BLE.cpp"

        # GPS
        "gps/GpsTask.cpp"
        "gps/GpsTask.cpp"
        "gps/Gps.cpp"
        
        # Heart Rate
        "HeartRate/HeartRateMonitor.cpp"
        "HeartRate/HeartRateTask.cpp"
        "HeartRate/HeartRateServer.cpp"

        # Display/UI
        "display/DisplayTask.cpp"
        "display/St7796Display.cpp"
        "UI/Canvas.cpp"
        "UI/Renderer.cpp"
      
    INCLUDE_DIRS
        "."
)
