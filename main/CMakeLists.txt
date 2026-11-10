idf_component_register(
    SRCS
        # Entry Point
        "bike-computer.c"

        # Drivers
        "drivers/uart/Uart.cpp"
        "drivers/spi/Spi.cpp"

        # GPS
        "gps/GpsTask.cpp"

        # Display/UI
        "display/DisplayTask.cpp"
        "display/St7796Display.cpp"
        "UI/Canvas.cpp"
        "UI/Renderer.cpp"
    INCLUDE_DIRS
        "."
)
