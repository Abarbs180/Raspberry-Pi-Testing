#include <iostream>
#include <cstdlib>
#include <cstdio>
#include <cstring>
#include <unistd.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <bluetooth/bluetooth.h>
#include <bluetooth/hci.h>
#include <bluetooth/hci_lib.h>

int main() {
    std::cout << "Starting BLE scan..." << std::endl;

    // Open Bluetooth device
    int device_id = hci_get_route(nullptr);
    if (device_id < 0) {
        std::cerr << "Error: No Bluetooth device found." << std::endl;
        return 1;
    }

    int socket = hci_open_dev(device_id);
    if (socket < 0) {
        std::cerr << "Error: Could not open device." << std::endl;
        return 1;
    }

    // Set BLE scan parameters
    constexpr int len = 8;  // Scan duration (in seconds)
    constexpr int max_rsp = 255;  // Maximum number of devices to respond
    inquiry_info *info = new inquiry_info[max_rsp];

    std::cout << "Scanning for BLE devices..." << std::endl;

    int num_rsp = hci_inquiry(device_id, len, max_rsp, nullptr, &info, IREQ_CACHE_FLUSH);
    if (num_rsp < 0) {
        std::cerr << "Error: Inquiry failed." << std::endl;
        delete[] info;
        close(socket);
        return 1;
    }

    // Print out device information
    char addr[19] = {0};
    char name[248] = {0};
    for (int i = 0; i < num_rsp; ++i) {
        ba2str(&info[i].bdaddr, addr);
        memset(name, 0, sizeof(name));
        if (hci_read_remote_name(socket, &info[i].bdaddr, sizeof(name), name, 0) < 0) {
            strncpy(name, "[unknown]", sizeof(name) - 1);
        }
        std::cout << "Device found: " << addr << " (" << name << ")" << std::endl;
    }

    // Cleanup
    delete[] info;
    close(socket);
    std::cout << "BLE scan complete." << std::endl;

    return 0;
}
