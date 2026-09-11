IMAGE_NAME := test-dev-env
CONTAINER_NAME := devbox

.PHONY: docker-build docker-run docker-start docker-stop docker-shell build

docker-build:
	docker build -t $(IMAGE_NAME) .

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
	docker exec -it $(CONTAINER_NAME) /bin/bash

build:
	echo Build with esp-idf **Replace with actual command**
