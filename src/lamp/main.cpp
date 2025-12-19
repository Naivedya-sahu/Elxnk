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
void write_events(int fd, const std::vector<input_event>& events, int sleep_time = 1000);
std::vector<input_event> pen_down(int x, int y, int points = 10);
std::vector<input_event> pen_move(int ox, int oy, int x, int y, int points = 10);
std::vector<input_event> pen_up();
std::vector<input_event> eraser_down(int x, int y, int pressure = 1700);
std::vector<input_event> eraser_move(int ox, int oy, int x, int y, int pressure = 1700);
std::vector<input_event> finger_move(int ox, int oy, int x, int y, int points = 10);

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
std::vector<input_event> pen_down(int x, int y, int points) {
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

std::vector<input_event> pen_move(int ox, int oy, int x, int y, int points) {
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
std::vector<input_event> eraser_down(int x, int y, int pressure) {
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

std::vector<input_event> eraser_move(int ox, int oy, int x, int y, int pressure) {
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

// Geometry functions from rmkit (iago/lamp)
// These functions enable drawing complex shapes

void trace_arc(int ox, int oy, int r1, int r2, int a1, int a2, int step = 1) {
    // Draw arc by tracing points along the curve
    for (int i = a1; i < a2 + 10; i += step) {
        double angle_rad = (double)i / denom;
        int rx = cos(angle_rad) * r1 + ox;
        int ry = sin(angle_rad) * r2 + oy;
        write_events(pen_fd, pen_move(pen_x, pen_y, rx, ry, move_pts), 2);
        pen_x = rx;
        pen_y = ry;
    }
}

void pen_draw_circle(int ox, int oy, int r1, int r2) {
    // Draw circle using arc tracing
    int start_x = ox + r1;
    int start_y = oy;

    write_events(pen_fd, pen_down(start_x, start_y));
    pen_x = start_x;
    pen_y = start_y;

    int old_move_pts = move_pts;
    move_pts = 10;
    trace_arc(ox, oy, r1, r2, 0, 360, 1);
    move_pts = old_move_pts;

    write_events(pen_fd, pen_up());
}

void pen_draw_arc(int ox, int oy, int r1, int r2, int a1 = 0, int a2 = 360) {
    // Normalize angles
    while (a2 < a1) {
        a2 += 360;
    }

    // Calculate starting point
    double angle_rad = (double)a1 / denom;
    int pointx = cos(angle_rad) * r1 + ox;
    int pointy = sin(angle_rad) * r2 + oy;

    write_events(pen_fd, pen_down(pointx, pointy));
    pen_x = pointx;
    pen_y = pointy;

    int old_move_pts = move_pts;
    move_pts = 10;
    trace_arc(ox, oy, r1, r2, a1, a2, 2);
    move_pts = old_move_pts;

    write_events(pen_fd, pen_up());
}

void pen_draw_line(int x1, int y1, int x2, int y2) {
    // Simple line drawing
    if (x2 == -1) {
        x2 = pen_x;
        y2 = pen_y;
    }

    write_events(pen_fd, pen_down(x1, y1));
    pen_x = x1;
    pen_y = y1;

    write_events(pen_fd, pen_move(pen_x, pen_y, x2, y2, move_pts));
    pen_x = x2;
    pen_y = y2;

    write_events(pen_fd, pen_up());
}

void pen_draw_rectangle(int x1, int y1, int x2, int y2) {
    // Draw rectangle as 4 connected lines
    if (x2 == -1) {
        x2 = pen_x;
        y2 = pen_y;
    }

    write_events(pen_fd, pen_down(x1, y1));
    pen_x = x1;
    pen_y = y1;

    write_events(pen_fd, pen_move(pen_x, pen_y, x1, y2, move_pts));
    pen_x = x1;
    pen_y = y2;

    write_events(pen_fd, pen_move(pen_x, pen_y, x2, y2, move_pts));
    pen_x = x2;
    pen_y = y2;

    write_events(pen_fd, pen_move(pen_x, pen_y, x2, y1, move_pts));
    pen_x = x2;
    pen_y = y1;

    write_events(pen_fd, pen_move(pen_x, pen_y, x1, y1, move_pts));
    pen_x = x1;
    pen_y = y1;

    write_events(pen_fd, pen_up());
}

void pen_draw_rounded_rectangle(int x1, int y1, int x2, int y2, int r) {
    // Ensure correct ordering
    if (x2 < x1) {
        int temp = x1;
        x1 = x2;
        x2 = temp;
    }
    if (y2 < y1) {
        int temp = y1;
        y1 = y2;
        y2 = temp;
    }

    int segmentx = abs(x2 - x1);
    int segmenty = abs(y2 - y1);

    // Limit radius to half the smallest dimension
    if (r > (0.5 * segmentx)) r = 0.5 * segmentx;
    if (r > (0.5 * segmenty)) r = 0.5 * segmenty;

    // Draw rounded rectangle using lines and arcs
    write_events(pen_fd, pen_down(x1 + r, y1));
    pen_x = x1 + r;
    pen_y = y1;

    // Top edge
    write_events(pen_fd, pen_move(pen_x, pen_y, x2 - r, y1, move_pts));
    pen_x = x2 - r;
    pen_y = y1;

    // Top-right corner arc
    trace_arc(x2 - r, y1 + r, r, r, 270, 360, 2);

    // Right edge
    write_events(pen_fd, pen_move(pen_x, pen_y, x2, y2 - r, move_pts));
    pen_x = x2;
    pen_y = y2 - r;

    // Bottom-right corner arc
    trace_arc(x2 - r, y2 - r, r, r, 0, 90, 2);

    // Bottom edge
    write_events(pen_fd, pen_move(pen_x, pen_y, x1 + r, y2, move_pts));
    pen_x = x1 + r;
    pen_y = y2;

    // Bottom-left corner arc
    trace_arc(x1 + r, y2 - r, r, r, 90, 180, 2);

    // Left edge
    write_events(pen_fd, pen_move(pen_x, pen_y, x1, y1 + r, move_pts));
    pen_x = x1;
    pen_y = y1 + r;

    // Top-left corner arc
    trace_arc(x1 + r, y1 + r, r, r, 180, 270, 2);

    write_events(pen_fd, pen_up());
}

void trace_bezier(const std::vector<int>& coors) {
    // Trace bezier curve points
    double pointx, pointy;
    double step = 0.01;

    if (coors.size() == 6) {
        // Quadratic bezier: 3 points (start, control, end)
        for (double t = step; t <= 1.0 + step; t += step) {
            double it = 1 - t;
            pointx = it * it * coors[0] + 2 * t * it * coors[2] + t * t * coors[4];
            pointy = it * it * coors[1] + 2 * t * it * coors[3] + t * t * coors[5];

            write_events(pen_fd, pen_move(pen_x, pen_y, (int)pointx, (int)pointy, move_pts), 2);
            pen_x = (int)pointx;
            pen_y = (int)pointy;
        }
    } else if (coors.size() == 8) {
        // Cubic bezier: 4 points (start, control1, control2, end)
        for (double t = step; t <= 1.0 + step; t += step) {
            double it = 1 - t;
            double it2 = it * it;
            double it3 = it2 * it;
            double t2 = t * t;
            double t3 = t2 * t;

            pointx = it3 * coors[0] + 3 * it2 * t * coors[2] + 3 * it * t2 * coors[4] + t3 * coors[6];
            pointy = it3 * coors[1] + 3 * it2 * t * coors[3] + 3 * it * t2 * coors[5] + t3 * coors[7];

            write_events(pen_fd, pen_move(pen_x, pen_y, (int)pointx, (int)pointy, move_pts), 2);
            pen_x = (int)pointx;
            pen_y = (int)pointy;
        }
    }
}

void pen_draw_bezier(const std::vector<int>& coors) {
    // Draw bezier curve
    if (coors.size() < 6) return;

    write_events(pen_fd, pen_down(coors[0], coors[1]));
    pen_x = coors[0];
    pen_y = coors[1];

    trace_bezier(coors);

    write_events(pen_fd, pen_up());
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

std::vector<input_event> finger_move(int ox, int oy, int x, int y, int points) {
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
void write_events(int fd, const std::vector<input_event>& events, int sleep_time) {
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

    int x = -1, y = -1, x2 = -1, y2 = -1, r = -1, r2 = -1, a1 = -1, a2 = -1;

    int sleep_val = 10;
    if (tool == "fastpen") sleep_val = 2;

    // Process pen commands
    if (tool == "pen" || tool == "fastpen") {
        if (action == "up") {
            write_events(pen_fd, pen_up());
        } else if (action == "down") {
            ss >> x >> y;
            write_events(pen_fd, pen_down(x, y));
            pen_x = x;
            pen_y = y;
        } else if (action == "move") {
            ss >> x >> y;
            write_events(pen_fd, pen_move(pen_x, pen_y, x, y, move_pts), sleep_val);
            pen_x = x;
            pen_y = y;
        }
        // Geometry commands
        else if (action == "line") {
            ss >> x >> y >> x2 >> y2;
            pen_draw_line(x, y, x2, y2);
        } else if (action == "circle") {
            ss >> x >> y >> r >> r2;
            if (r2 == -1) r2 = r;  // Default to circular
            pen_draw_circle(x, y, r, r2);
        } else if (action == "rectangle") {
            ss >> x >> y >> x2 >> y2;
            pen_draw_rectangle(x, y, x2, y2);
        } else if (action == "arc") {
            ss >> x >> y >> r >> r2 >> a1 >> a2;
            if (r2 == -1) r2 = r;
            pen_draw_arc(x, y, r, r2, a1, a2);
        } else if (action == "rounded_rectangle" || action == "roundrect") {
            ss >> x >> y >> x2 >> y2 >> r;
            pen_draw_rounded_rectangle(x, y, x2, y2, r);
        } else if (action == "bezier") {
            std::vector<int> coors;
            int val;
            while (ss >> val) {
                coors.push_back(val);
            }
            pen_draw_bezier(coors);
        }
    }
    // Process eraser commands
    else if (tool == "eraser" || tool == "erase") {
        if (action == "up") {
            write_events(pen_fd, eraser_up());
        } else if (action == "down") {
            ss >> x >> y;
            write_events(pen_fd, eraser_down(x, y, 1700));
            pen_x = x;
            pen_y = y;
        } else if (action == "move") {
            ss >> x >> y;
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
            ss >> x >> y;
            write_events(touch_fd, finger_down(x, y));
            finger_x = x;
            finger_y = y;
        } else if (action == "move") {
            ss >> x >> y;
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
