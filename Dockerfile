FROM ubuntu:22.04

ENV DEBIAN_FRONTEND=noninteractive
ENV IDF_PATH=/opt/esp/esp-idf

RUN apt-get update \
    && apt-get install -y \
        build-essential \
        git \
        vim \
        curl \
        sudo \
        doxygen \
        software-properties-common \
        wget \
        flex \
        bison \
        gperf \
        python3 \
        python3-pip \
        python3-venv \
        cmake \
        ninja-build \
        ccache \
        ripgrep \
        fd-find \
        nodejs \
        npm \
        libffi-dev \
        libssl-dev \
        dfu-util \
        libusb-1.0-0 \
    && rm -rf /var/lib/apt/lists/*

RUN add-apt-repository ppa:neovim-ppa/unstable -y \
    && apt-get update \
    && apt-get install -y neovim

ARG USERNAME=dev
ARG USER_UID=1000
ARG USER_GID=1000

RUN groupadd --gid $USER_GID $USERNAME \
    && useradd --uid $USER_UID --gid $USER_GID -m $USERNAME \
    && usermod -aG dialout $USERNAME \
    && echo "$USERNAME ALL=(ALL) NOPASSWD:ALL" >> /etc/sudoers \
    && mkdir -p /opt/esp \
    && chown -R $USERNAME:$USERNAME /opt/esp

COPY .bashrc /home/$USERNAME/.bashrc

RUN chown $USERNAME:$USERNAME /home/$USERNAME/.bashrc

USER $USERNAME
WORKDIR /workspace

RUN git clone https://github.com/LazyVim/starter ~/.config/nvim \
    && rm -rf ~/.config/nvim/.git

RUN git clone \
        --branch v6.1 \
        --recursive \
        https://github.com/espressif/esp-idf.git \
        "${IDF_PATH}" \
    && "${IDF_PATH}/install.sh" esp32


CMD ["/bin/bash"]
