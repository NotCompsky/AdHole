An advert-blocker like PiHole, but for use on untrusted networks.

# Description

Many people use PiHole as a network-level advert-blocker. It works by blocking DNS requests to advertiser domains.

AdHole uses the same DNS utility as PiHole - dnsmasq - but is entirely local to your own computer.

AdHole blocks everything except whitelisted domains. Its settings are stored as files of whitelisted domains in a directory.

# Installation

## Prerequisites

You must have:

* systemd
* cmake
* Qt5 widgets development module (probably qtbase5-dev)
* Any C compiler (such as clang or gcc)
  * cmake might struggle to find it. If that happens, set environment variable CC to the path to the C compiler.
* Any C++ compiler (such as clang++ or g++)
  * cmake might struggle to find it. If that happens, set environment variable CXX to the path to the C++ compiler.
* libsystemd-dev

For Ubuntu, this is achieved with

    sudo apt install --no-install-recommends cmake clang qtbase5-dev libsystemd-dev

Your system must use dnsmasq, not systemd-resolved.

systemd-resolved is the default for Ubuntu.

To replace systemd-resolved with dnsmasq:

    sudo apt install --no-install-recommends dnsmasq
    sudo systemctl stop systemd-resolved
    sudo systemctl disable systemd-resolved
    systemctl status dnsmasq
    sudo systemctl stop dnsmasq
    sudo systemctl start dnsmasq

## Installation

    git clone https://github.com/NotCompsky/AdHole
    mkdir AdHole/build
    cd AdHole/build
    cmake ..
    sudo mv AdHole /usr/bin/AdHole

# Usage

Write each list of domain names to files within a directory (do not use subdirectories).

Then run

    sudo AdHole /path/to/that/directory

Then select or deselect any file names you wish to be whitelisted.

Then click 'OK' to apply these whitelists.
