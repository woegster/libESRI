# libESRI
A telnet framework for applications

## The Problem
**As a developer/maintainer** of a (legacy) "Big Business App"
**I want to** execute my code without using the offical frontend
**So that** i can test my code independet of the frontend

Assumed that on one or more things apply
- There are no tests fo the apllication
- There is no gated checkin
- The frontend is currently broken
- Complex interaction before your code is reached
 
This was the situation that lead to the development of ESRI.

## The Solution
Integrate a telnet server into the application as frontend replacement.
With this the code you write can always easily be executed.
*Disclaimer: It's of course still a hack and you should not need it in the first place*

## Usage
* `#include <libESRI.h>`
* Create an Instance with `EsriCreateInstsance()`
* Set your callback handlers with `EsriSetHandlersForInstance(...)`
* Start accepting connections with `EsriStartInstance(esriInstance, port, maxConnections)`
* Do your code in the `OnCommitCommand(..., sring command, ...)` handler
* Delete the instance with `EsriDeleteInstance()` when exiting

## Features
- **Telnet** works with most clients, make sure "local line editing" is turned off
- **C API** allows most languages to call the library as well
- **Crossplattform** thanks to CMake
 
See the **Advanced Usage & Features** Wiki for more things like autocomplete and batch processing.

## Requirements
* A C++11 compiler
* CMake 3.1.3