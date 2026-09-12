IMAGE_NAME := test-dev-env
CONTAINER_NAME := devbox
BUSID ?=
ESP_PORT ?= /dev/ttyUSB0

.PHONY: docker-build docker-run docker-start docker-stop docker-shell build

# Docker commands
docker-build:
	docker build -t $(IMAGE_NAME) .

# Create the container
# --device passes the usb connection into the container
docker-run-usb:
	docker run -dit \
		--device=/dev/ttyUSB0:/dev/ttyUSB0 \
		-v $(shell pwd):/workspace \
		--name $(CONTAINER_NAME) \
		$(IMAGE_NAME)

docker-run:
	docker run -dit \
		-v $(shell pwd):/workspace \
		--name $(CONTAINER_NAME) \
		$(IMAGE_NAME)

docker-start:
	docker start $(CONTAINER_NAME)

docker-stop:
	docker stop $(CONTAINER_NAME)

docker-shell:
	docker exec -it $(CONTAINER_NAME) /bin/bash -i

# Command that was used for project creation
set-target:
	idf.py set-target esp32

# Commands for packaging and flashing code
build:
	idf.py build

flash:
	idf.py -p /dev/ttyUSB0 flash monitor

# Windows commands for setting up esp32 connection on windows
usb-prep-bind-w:
	@powershell.exe -NoProfile -Command \
		"winget install --interactive --exact dorssel.usbipd-win" \
		"usbipd list"

usb-bind-w:
	@powershell.exe -NoProfile -Command \
		"usbipd bind --busid $(BUSID)"

usb-attach-w:
	@powershell.exe -NoProfile -Command \
		"usbipd attach --wsl --busid $(BUSID)"

usb-list-w:
	@powershell.exe -NoProfile -Command "usbipd list"

usb-check-w:
	@if [ ! -e "$(ESP_PORT)" ]; then \
		echo "ESP32 serial port not found at $(ESP_PORT)."; \
		echo "Available serial devices:"; \
		ls -l /dev/ttyUSB* /dev/ttyACM* 2>/dev/null || true; \
		exit 1; \
	fi
	@echo "ESP32 found at $(ESP_PORT)"