// Elxnk Main Controller
// Integrates lampv2 (drawing engine) and geniev2 (gesture detector)
// Provides unified process management and UI control

#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <errno.h>
#include <stdarg.h>
#include <string>
#include <vector>

// Configuration
#define LAMP_BINARY "/opt/bin/lamp"
#define GENIE_BINARY "/opt/bin/genie_lamp"
#define GENIE_CONFIG "/opt/etc/genie_ui.conf"
#define LAMP_PIPE "/tmp/elxnk_lamp.pipe"
#define UI_INIT_SCRIPT "/opt/bin/ui_state.sh"
#define LOG_FILE "/tmp/elxnk.log"
#define PID_FILE "/tmp/elxnk.pid"

// Global state
static pid_t lamp_pid = -1;
static pid_t genie_pid = -1;
static FILE* log_fp = NULL;
static int lamp_pipe_fd = -1;
static bool running = true;

// Logging
void log_msg(const char* level, const char* format, ...) {
    if (!log_fp) return;

    time_t now = time(NULL);
    struct tm* tm_info = localtime(&now);
    char timestamp[26];
    strftime(timestamp, 26, "%Y-%m-%d %H:%M:%S", tm_info);

    fprintf(log_fp, "[%s] [%s] ", timestamp, level);

    va_list args;
    va_start(args, format);
    vfprintf(log_fp, format, args);
    va_end(args);

    fprintf(log_fp, "\n");
    fflush(log_fp);
}

// Signal handler for graceful shutdown
void signal_handler(int signo) {
    log_msg("INFO", "Received signal %d, initiating shutdown", signo);
    running = false;
}

// Setup signal handlers
void setup_signals() {
    struct sigaction sa;
    memset(&sa, 0, sizeof(sa));
    sa.sa_handler = signal_handler;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;

    sigaction(SIGINT, &sa, NULL);
    sigaction(SIGTERM, &sa, NULL);
    sigaction(SIGHUP, &sa, NULL);

    // Ignore SIGCHLD to prevent zombie processes
    signal(SIGCHLD, SIG_IGN);
}

// Create named pipe for lamp
bool create_lamp_pipe() {
    // Remove old pipe if exists
    unlink(LAMP_PIPE);

    // Create new pipe
    if (mkfifo(LAMP_PIPE, 0666) < 0) {
        log_msg("ERROR", "Failed to create lamp pipe: %s", strerror(errno));
        return false;
    }

    log_msg("INFO", "Created lamp pipe at %s", LAMP_PIPE);
    return true;
}

// Start lamp process
bool start_lamp() {
    log_msg("INFO", "Starting lamp process...");

    lamp_pid = fork();
    if (lamp_pid < 0) {
        log_msg("ERROR", "Failed to fork lamp process: %s", strerror(errno));
        return false;
    }

    if (lamp_pid == 0) {
        // Child process - redirect stdin from pipe
        int fd = open(LAMP_PIPE, O_RDONLY);
        if (fd < 0) {
            fprintf(stderr, "Failed to open lamp pipe: %s\n", strerror(errno));
            exit(1);
        }

        dup2(fd, STDIN_FILENO);
        close(fd);

        // Redirect output to log
        int log_fd = open(LOG_FILE, O_WRONLY | O_APPEND | O_CREAT, 0644);
        if (log_fd >= 0) {
            dup2(log_fd, STDOUT_FILENO);
            dup2(log_fd, STDERR_FILENO);
            close(log_fd);
        }

        // Execute lamp
        execl(LAMP_BINARY, "lamp", NULL);

        // If we get here, exec failed
        fprintf(stderr, "Failed to exec lamp: %s\n", strerror(errno));
        exit(1);
    }

    // Parent process - open pipe for writing
    lamp_pipe_fd = open(LAMP_PIPE, O_WRONLY);
    if (lamp_pipe_fd < 0) {
        log_msg("ERROR", "Failed to open lamp pipe for writing: %s", strerror(errno));
        kill(lamp_pid, SIGTERM);
        lamp_pid = -1;
        return false;
    }

    log_msg("INFO", "Lamp process started with PID %d", lamp_pid);
    return true;
}

// Start genie_lamp process
bool start_genie() {
    log_msg("INFO", "Starting genie_lamp process...");

    genie_pid = fork();
    if (genie_pid < 0) {
        log_msg("ERROR", "Failed to fork genie_lamp process: %s", strerror(errno));
        return false;
    }

    if (genie_pid == 0) {
        // Child process - redirect output to log
        int log_fd = open(LOG_FILE, O_WRONLY | O_APPEND | O_CREAT, 0644);
        if (log_fd >= 0) {
            dup2(log_fd, STDOUT_FILENO);
            dup2(log_fd, STDERR_FILENO);
            close(log_fd);
        }

        // Execute genie_lamp with config
        execl(GENIE_BINARY, "genie_lamp", GENIE_CONFIG, NULL);

        // If we get here, exec failed
        fprintf(stderr, "Failed to exec genie_lamp: %s\n", strerror(errno));
        exit(1);
    }

    log_msg("INFO", "Genie_lamp process started with PID %d", genie_pid);
    return true;
}

// Initialize UI system
bool init_ui() {
    log_msg("INFO", "Initializing UI system...");

    // Run UI initialization script
    char cmd[512];
    snprintf(cmd, sizeof(cmd), "%s init 2>&1 >> %s", UI_INIT_SCRIPT, LOG_FILE);

    int ret = system(cmd);
    if (ret != 0) {
        log_msg("WARN", "UI init returned %d, continuing anyway", ret);
    }

    // Give UI time to initialize
    sleep(1);

    log_msg("INFO", "UI system initialized");
    return true;
}

// Send command to lamp
void send_lamp_command(const char* cmd) {
    if (lamp_pipe_fd < 0) return;

    size_t len = strlen(cmd);
    write(lamp_pipe_fd, cmd, len);
    write(lamp_pipe_fd, "\n", 1);
}

// Check if process is alive
bool is_process_alive(pid_t pid) {
    if (pid <= 0) return false;
    return kill(pid, 0) == 0;
}

// Monitor child processes and restart if needed
void monitor_processes() {
    // Check lamp
    if (!is_process_alive(lamp_pid)) {
        log_msg("WARN", "Lamp process died, restarting...");
        if (lamp_pipe_fd >= 0) {
            close(lamp_pipe_fd);
            lamp_pipe_fd = -1;
        }
        sleep(1);
        start_lamp();
    }

    // Check genie_lamp
    if (!is_process_alive(genie_pid)) {
        log_msg("WARN", "Genie_lamp process died, restarting...");
        sleep(1);
        start_genie();
    }
}

// Cleanup and shutdown
void cleanup() {
    log_msg("INFO", "Cleaning up...");

    // Stop genie_lamp first (no more gestures)
    if (genie_pid > 0) {
        log_msg("INFO", "Stopping genie_lamp (PID %d)", genie_pid);
        kill(genie_pid, SIGTERM);

        // Wait up to 3 seconds
        for (int i = 0; i < 30; i++) {
            if (!is_process_alive(genie_pid)) break;
            usleep(100000);
        }

        if (is_process_alive(genie_pid)) {
            log_msg("WARN", "Force killing genie_lamp");
            kill(genie_pid, SIGKILL);
        }
        genie_pid = -1;
    }

    // Stop lamp
    if (lamp_pid > 0) {
        log_msg("INFO", "Stopping lamp (PID %d)", lamp_pid);

        // Close pipe first
        if (lamp_pipe_fd >= 0) {
            close(lamp_pipe_fd);
            lamp_pipe_fd = -1;
        }

        kill(lamp_pid, SIGTERM);

        // Wait up to 3 seconds
        for (int i = 0; i < 30; i++) {
            if (!is_process_alive(lamp_pid)) break;
            usleep(100000);
        }

        if (is_process_alive(lamp_pid)) {
            log_msg("WARN", "Force killing lamp");
            kill(lamp_pid, SIGKILL);
        }
        lamp_pid = -1;
    }

    // Remove pipe
    unlink(LAMP_PIPE);

    // Remove PID file
    unlink(PID_FILE);

    log_msg("INFO", "Cleanup complete");

    if (log_fp) {
        fclose(log_fp);
        log_fp = NULL;
    }
}

// Write PID file
bool write_pid_file() {
    FILE* fp = fopen(PID_FILE, "w");
    if (!fp) {
        fprintf(stderr, "Failed to create PID file: %s\n", strerror(errno));
        return false;
    }

    fprintf(fp, "%d\n", getpid());
    fclose(fp);
    return true;
}

// Check if another instance is running
bool check_existing_instance() {
    FILE* fp = fopen(PID_FILE, "r");
    if (!fp) return false;

    int pid;
    if (fscanf(fp, "%d", &pid) == 1) {
        fclose(fp);
        if (is_process_alive(pid)) {
            fprintf(stderr, "Another instance is already running (PID %d)\n", pid);
            return true;
        }
    } else {
        fclose(fp);
    }

    // Stale PID file
    unlink(PID_FILE);
    return false;
}

// Main entry point
int main(int argc, char** argv) {
    printf("Elxnk Controller v1.0\n");
    printf("Integrating lampv2 and geniev2\n\n");

    // Check for existing instance
    if (check_existing_instance()) {
        return 1;
    }

    // Open log file
    log_fp = fopen(LOG_FILE, "a");
    if (!log_fp) {
        fprintf(stderr, "Failed to open log file: %s\n", strerror(errno));
        return 1;
    }

    log_msg("INFO", "=== Elxnk starting ===");
    log_msg("INFO", "PID: %d", getpid());

    // Write PID file
    if (!write_pid_file()) {
        return 1;
    }

    // Setup signal handlers
    setup_signals();

    // Create lamp pipe
    if (!create_lamp_pipe()) {
        cleanup();
        return 1;
    }

    // Start lamp
    if (!start_lamp()) {
        cleanup();
        return 1;
    }

    // Initialize UI
    if (!init_ui()) {
        log_msg("WARN", "UI initialization had issues, continuing anyway");
    }

    // Start genie_lamp
    if (!start_genie()) {
        cleanup();
        return 1;
    }

    log_msg("INFO", "All systems started successfully");
    printf("Elxnk is running. Check %s for logs.\n", LOG_FILE);
    printf("Use 'kill %d' or Ctrl+C to stop.\n\n", getpid());

    // Main loop - monitor processes
    while (running) {
        monitor_processes();
        sleep(5);
    }

    log_msg("INFO", "Shutdown requested");
    cleanup();

    printf("Elxnk stopped.\n");
    return 0;
}
