FROM ubuntu:22.04

ENV DEBIAN_FRONTEND=noninteractive

RUN apt-get update && apt-get install -y \
    build-essential \
    git \
    vim \
    curl \
    sudo \
    doxygen \
    software-properties-common \
    && rm -rf /var/lib/apt/lists/*

RUN add-apt-repository ppa:neovim-ppa/unstable -y \
    && apt-get update \
    && apt-get install -y neovim

ARG USERNAME=dev
ARG USER_UID=1000
ARG USER_GID=1000

RUN groupadd --gid $USER_GID $USERNAME \
    && useradd --uid $USER_UID --gid $USER_GID -m $USERNAME \
    && echo "$USERNAME ALL=(ALL) NOPASSWD:ALL" >> /etc/sudoers


COPY .bashrc /home/$USERNAME/.bashrc
RUN chown $USERNAME:$USERNAME /home/$USERNAME/.bashrc

USER $USERNAME
WORKDIR /workspace

RUN git clone https://github.com/LazyVim/starter ~/.config/nvim \
    && rm -rf ~/.config/nvim/.git

CMD ["/bin/bash"]
