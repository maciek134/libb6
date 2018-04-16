libb6
=====
This is a library for interfacing with SkyRC B6xx series chargers. It was tested on B6AC v2, but should work fine on 
other chargers from this series. Some of the packets are based on lachs0r's incredible work on https://github.com/lachs0r/b6mon 
and the rest were sniffed and analyzed with Wireshark.

Installation
------------
Dependencies:
```
libusb-1.0
```

Run these commands to install the library and it's headers on your system:
```bash
$ mkdir build
$ cd build
$ cmake .. -DCMAKE_INSTALL_PREFIX=/usr
$ make
$ sudo make install
```

Either run the programs that use it as root (**not recommended**) or create an udev rule similar to this one:
```udev
SUBSYSTEM=="usb", ATTRS{idVendor}=="0000", ATTRS{idProduct}=="0001", MODE:="666", GROUP="plugdev"
KERNEL=="hidraw*", ATTRS{idVendor}=="0000", ATTRS{idProduct}=="0001", MODE="0666", GROUP="plugdev"
```

Example
-------
Here is an example that reads the number of cells the charger supports:

```cpp
#include <iostream>
#include <b6/Device.hh>

int main() {
  auto dev = new b6::Device(); // the constructor handles libusb init, claiming the device interface etc.
  
  std::cout << "Number of cells: " << dev->getCellCount() << std::endl;
  
  delete dev; // this releases the device, reattaches the kernel driver and frees libusb
  return 0;
}
```

[Documentation](https://github.com/maciek134/libb6/wiki/Documentation)
-------------