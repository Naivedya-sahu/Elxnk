// Standalone lamp - reMarkable 2 drawing engine
// Converted from rmkit main.cpy to pure C++
// Reads commands from stdin and writes to input devices

#include <linux/input.h>
#include <string>
#include <vector>
#include <sstream>
#include <iostream>
#include <cmath>
#include <cstring>
#include <cstdlib>
#include <unistd.h>
#include <fcntl.h>
#include <algorithm>

// reMarkable 2 constants
#define MTWIDTH 767
#define MTHEIGHT 1023
#define WACOMWIDTH 15725.0
#define WACOMHEIGHT 20967.0
#define DISPLAYWIDTH 1404
#define DISPLAYHEIGHT 1872.0
#define MT_X_SCALAR (float(DISPLAYWIDTH) / float(MTWIDTH))
#define MT_Y_SCALAR (float(DISPLAYHEIGHT) / float(MTHEIGHT))
#define WACOM_X_SCALAR (float(DISPLAYWIDTH) / float(WACOMWIDTH))
#define WACOM_Y_SCALAR (float(DISPLAYHEIGHT) / float(WACOMHEIGHT))

// Input device types
enum EV_TYPE { UNKNOWN, TOUCH, STYLUS };

// Global state
int offset = 0;
int move_pts = 500;
double denom = 360 / (2 * M_PI);
bool bsleep = false;
int finger_x = 0, finger_y = 0, pen_x = 0, pen_y = 0;
int touch_fd = -1, pen_fd = -1;

// Forward declarations
void act_on_line(const std::string& line);

// Coordinate transformations
int get_pen_x(int x) {
    return x / WACOM_X_SCALAR;
}

int get_pen_y(int y) {
    return WACOMHEIGHT - (y / WACOM_Y_SCALAR);
}

int get_touch_x(int x) {
    return x;  // RM2
}

int get_touch_y(int y) {
    return DISPLAYHEIGHT - y;  // RM2
}

// Input event helpers
std::vector<input_event> pen_down(int x, int y, int points = 10) {
    std::vector<input_event> ev;

    ev.push_back({.type = EV_SYN, .code = SYN_REPORT, .value = 1});
    ev.push_back({.type = EV_KEY, .code = BTN_TOOL_PEN, .value = 1});
    ev.push_back({.type = EV_KEY, .code = BTN_TOUCH, .value = 1});
    ev.push_back({.type = EV_ABS, .code = ABS_Y, .value = get_pen_x(x)});
    ev.push_back({.type = EV_ABS, .code = ABS_X, .value = get_pen_y(y)});
    ev.push_back({.type = EV_ABS, .code = ABS_DISTANCE, .value = 0});
    ev.push_back({.type = EV_ABS, .code = ABS_PRESSURE, .value = 4000});
    ev.push_back({.type = EV_SYN, .code = SYN_REPORT, .value = 1});

    for (int i = 0; i < points; i++) {
        ev.push_back({.type = EV_ABS, .code = ABS_PRESSURE, .value = 4000});
        ev.push_back({.type = EV_ABS, .code = ABS_PRESSURE, .value = 4001});
        ev.push_back({.type = EV_SYN, .code = SYN_REPORT, .value = 1});
    }

    return ev;
}

std::vector<input_event> pen_move(int ox, int oy, int x, int y, int points = 10) {
    auto ev = pen_down(ox, oy);
    double dx = float(x - ox) / float(points);
    double dy = float(y - oy) / float(points);

    ev.push_back({.type = EV_SYN, .code = SYN_REPORT, .value = 1});
    for (int i = 0; i <= points; i++) {
        ev.push_back({.type = EV_ABS, .code = ABS_Y, .value = get_pen_x(ox + (i * dx))});
        ev.push_back({.type = EV_ABS, .code = ABS_X, .value = get_pen_y(oy + (i * dy))});
        ev.push_back({.type = EV_SYN, .code = SYN_REPORT, .value = 1});
    }

    return ev;
}

std::vector<input_event> pen_up() {
    std::vector<input_event> ev;
    ev.push_back({.type = EV_SYN, .code = SYN_REPORT, .value = 1});
    ev.push_back({.type = EV_KEY, .code = BTN_TOOL_PEN, .value = 0});
    ev.push_back({.type = EV_KEY, .code = BTN_TOUCH, .value = 0});
    ev.push_back({.type = EV_SYN, .code = SYN_REPORT, .value = 1});
    return ev;
}

std::vector<input_event> pen_clear() {
    std::vector<input_event> ev;
    ev.push_back({.type = EV_ABS, .code = ABS_X, .value = -1});
    ev.push_back({.type = EV_ABS, .code = ABS_DISTANCE, .value = -1});
    ev.push_back({.type = EV_ABS, .code = ABS_PRESSURE, .value = -1});
    ev.push_back({.type = EV_ABS, .code = ABS_Y, .value = -1});
    ev.push_back({.type = EV_SYN, .code = SYN_REPORT, .value = 1});
    return ev;
}

// Eraser functions (added for erase mode)
std::vector<input_event> eraser_down(int x, int y, int pressure = 1700) {
    std::vector<input_event> ev;
    ev.push_back({.type = EV_SYN, .code = SYN_REPORT, .value = 1});
    ev.push_back({.type = EV_KEY, .code = BTN_TOOL_RUBBER, .value = 1});
    ev.push_back({.type = EV_KEY, .code = BTN_TOUCH, .value = 1});
    ev.push_back({.type = EV_ABS, .code = ABS_Y, .value = get_pen_x(x)});
    ev.push_back({.type = EV_ABS, .code = ABS_X, .value = get_pen_y(y)});
    ev.push_back({.type = EV_ABS, .code = ABS_DISTANCE, .value = 0});
    ev.push_back({.type = EV_ABS, .code = ABS_PRESSURE, .value = pressure});
    ev.push_back({.type = EV_ABS, .code = ABS_TILT_X, .value = 50});
    ev.push_back({.type = EV_ABS, .code = ABS_TILT_Y, .value = -150});
    ev.push_back({.type = EV_SYN, .code = SYN_REPORT, .value = 1});
    return ev;
}

std::vector<input_event> eraser_move(int ox, int oy, int x, int y, int pressure = 1700) {
    std::vector<input_event> ev;

    int points = std::max(abs(x - ox), abs(y - oy)) / 3;
    if (points < 5) points = 5;

    double dx = float(x - ox) / float(points);
    double dy = float(y - oy) / float(points);

    for (int i = 0; i <= points; i++) {
        int curr_x = ox + (i * dx);
        int curr_y = oy + (i * dy);

        ev.push_back({.type = EV_ABS, .code = ABS_Y, .value = get_pen_x(curr_x)});
        ev.push_back({.type = EV_ABS, .code = ABS_X, .value = get_pen_y(curr_y)});
        ev.push_back({.type = EV_ABS, .code = ABS_PRESSURE, .value = pressure});
        ev.push_back({.type = EV_SYN, .code = SYN_REPORT, .value = 1});
    }

    return ev;
}

std::vector<input_event> eraser_up() {
    std::vector<input_event> ev;
    ev.push_back({.type = EV_SYN, .code = SYN_REPORT, .value = 1});
    ev.push_back({.type = EV_KEY, .code = BTN_TOOL_RUBBER, .value = 0});
    ev.push_back({.type = EV_KEY, .code = BTN_TOUCH, .value = 0});
    ev.push_back({.type = EV_ABS, .code = ABS_PRESSURE, .value = 0});
    ev.push_back({.type = EV_SYN, .code = SYN_REPORT, .value = 1});
    return ev;
}

// Finger/touch events
std::vector<input_event> finger_down(int x, int y) {
    std::vector<input_event> ev;
    time_t now = time(NULL) + offset++;
    ev.push_back({.type = EV_ABS, .code = ABS_MT_TRACKING_ID, .value = (int)now});
    ev.push_back({.type = EV_ABS, .code = ABS_MT_POSITION_X, .value = get_touch_x(x)});
    ev.push_back({.type = EV_ABS, .code = ABS_MT_POSITION_Y, .value = get_touch_y(y)});
    ev.push_back({.type = EV_SYN, .code = SYN_REPORT, .value = 1});
    return ev;
}

std::vector<input_event> finger_move(int ox, int oy, int x, int y, int points = 10) {
    auto ev = finger_down(ox, oy);
    double dx = float(x - ox) / float(points);
    double dy = float(y - oy) / float(points);

    for (int i = 0; i <= points; i++) {
        ev.push_back({.type = EV_ABS, .code = ABS_MT_POSITION_X, .value = get_touch_x(ox + (i * dx))});
        ev.push_back({.type = EV_ABS, .code = ABS_MT_POSITION_Y, .value = get_touch_y(oy + (i * dy))});
        ev.push_back({.type = EV_SYN, .code = SYN_REPORT, .value = 1});
    }

    return ev;
}

std::vector<input_event> finger_up() {
    std::vector<input_event> ev;
    ev.push_back({.type = EV_ABS, .code = ABS_MT_TRACKING_ID, .value = -1});
    ev.push_back({.type = EV_SYN, .code = SYN_REPORT, .value = 1});
    return ev;
}

// Write events to device
void write_events(int fd, const std::vector<input_event>& events, int sleep_time = 1000) {
    if (fd < 0) return;

    std::vector<input_event> send;
    for (const auto& event : events) {
        send.push_back(event);
        if (event.type == EV_SYN) {
            if (sleep_time) usleep(sleep_time);
            write(fd, send.data(), sizeof(input_event) * send.size());
            send.clear();
        }
    }
}

// Identify device type by checking capabilities
EV_TYPE identify_device(int fd) {
    if (fd < 0) return UNKNOWN;

    unsigned char evbit[EV_MAX/8 + 1];
    memset(evbit, 0, sizeof(evbit));

    if (ioctl(fd, EVIOCGBIT(0, sizeof(evbit)), evbit) < 0) {
        return UNKNOWN;
    }

    // Check if device supports absolute position (touchscreen/stylus)
    if (evbit[EV_ABS/8] & (1 << (EV_ABS % 8))) {
        unsigned char absbit[ABS_MAX/8 + 1];
        memset(absbit, 0, sizeof(absbit));

        if (ioctl(fd, EVIOCGBIT(EV_ABS, sizeof(absbit)), absbit) >= 0) {
            // Check for multitouch (touch screen)
            if (absbit[ABS_MT_POSITION_X/8] & (1 << (ABS_MT_POSITION_X % 8))) {
                return TOUCH;
            }
            // Check for stylus
            if (absbit[ABS_X/8] & (1 << (ABS_X % 8))) {
                return STYLUS;
            }
        }
    }

    return UNKNOWN;
}

// Command processor
void act_on_line(const std::string& line) {
    if (line.empty() || line[0] == '#') return;

    std::istringstream ss(line);
    std::string tool, action;
    ss >> tool >> action;

    int x = -1, y = -1, ox = -1, oy = -1;

    // Parse coordinates based on action
    if (action == "move" || action == "down") {
        ss >> x >> y;
    } else if (action == "up") {
        // No coordinates needed
    } else if (action == "on" || action == "off") {
        // Mode toggles
    } else {
        return;  // Unknown action
    }

    int sleep_val = 10;
    if (tool == "fastpen") sleep_val = 2;

    // Process pen commands
    if (tool == "pen" || tool == "fastpen") {
        if (action == "up") {
            write_events(pen_fd, pen_up());
        } else if (action == "down") {
            write_events(pen_fd, pen_down(x, y));
            pen_x = x;
            pen_y = y;
        } else if (action == "move") {
            write_events(pen_fd, pen_move(pen_x, pen_y, x, y, move_pts), sleep_val);
            pen_x = x;
            pen_y = y;
        }
    }
    // Process eraser commands
    else if (tool == "eraser" || tool == "erase") {
        if (action == "up") {
            write_events(pen_fd, eraser_up());
        } else if (action == "down") {
            write_events(pen_fd, eraser_down(x, y, 1700));
            pen_x = x;
            pen_y = y;
        } else if (action == "move") {
            write_events(pen_fd, eraser_move(pen_x, pen_y, x, y, 1700));
            pen_x = x;
            pen_y = y;
        } else if (action == "on") {
            // Switch to eraser mode (handled by caller sending eraser commands)
        } else if (action == "off") {
            // Switch back to pen mode
            write_events(pen_fd, eraser_up());
        }
    }
    // Process finger/touch commands
    else if (tool == "finger") {
        if (action == "up") {
            write_events(touch_fd, finger_up());
        } else if (action == "down") {
            write_events(touch_fd, finger_down(x, y));
            finger_x = x;
            finger_y = y;
        } else if (action == "move") {
            write_events(touch_fd, finger_move(finger_x, finger_y, x, y));
            finger_x = x;
            finger_y = y;
        }
    }
    // Sleep command
    else if (tool == "sleep") {
        int val = atoi(action.c_str());
        if (val > 0 && val <= 10000) {
            usleep(val * 1000);
        }
    }
}

int main(int argc, char** argv) {
    // Open input devices
    int fd0 = open("/dev/input/event0", O_RDWR);
    int fd1 = open("/dev/input/event1", O_RDWR);
    int fd2 = open("/dev/input/event2", O_RDWR);

    // Identify devices
    if (identify_device(fd0) == TOUCH) touch_fd = fd0;
    if (identify_device(fd1) == TOUCH) touch_fd = fd1;
    if (identify_device(fd2) == TOUCH) touch_fd = fd2;

    if (identify_device(fd0) == STYLUS) pen_fd = fd0;
    if (identify_device(fd1) == STYLUS) pen_fd = fd1;
    if (identify_device(fd2) == STYLUS) pen_fd = fd2;

    if (touch_fd < 0 || pen_fd < 0) {
        std::cerr << "Error: Could not find touch/stylus devices\n";
        return 1;
    }

    // Initialize devices
    write_events(touch_fd, finger_up());
    write_events(pen_fd, pen_clear());

    // Read commands from stdin
    std::string line;
    while (std::getline(std::cin, line)) {
        act_on_line(line);
    }

    // Cleanup
    write_events(touch_fd, finger_up());
    write_events(pen_fd, pen_up());

    close(fd0);
    close(fd1);
    close(fd2);

    return 0;
}
