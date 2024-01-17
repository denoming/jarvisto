FROM python:3.12-bookworm

ARG UNAME=dev
ARG UID=1000
ARG GID=1000

ENV VCPKG_ROOT="/home/$UNAME/.vcpkg"

USER root

RUN apt update && \
    apt install -y build-essential autoconf automake autopoint git cmake ninja-build curl tar zip unzip \
                   sudo flex bison nasm texinfo \
                   python3-jinja2

# Create default user
RUN groupadd -f -g $GID $UNAME
RUN useradd -l -g $GID --uid $UID -ms /bin/bash $UNAME
RUN echo $UNAME:$UNAME | chpasswd
RUN echo $UNAME 'ALL=(ALL) NOPASSWD:SETENV: ALL' > /etc/sudoers.d/$UNAME || true

USER $UNAME

RUN pip install Jinja2

RUN git clone https://github.com/Microsoft/vcpkg.git $HOME/.vcpkg && \
    bash $HOME/.vcpkg/bootstrap-vcpkg.sh && \
    mkdir -p $HOME/.local/bin && \
    ln -s $HOME/.vcpkg/vcpkg $HOME/.local/bin/vcpkg

CMD bash --rcfile "$HOME/.profile"
