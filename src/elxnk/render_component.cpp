// Component Renderer - Uses embedded component_library.h
// Renders components by sending lamp commands to the pipe
// This is the binary that actually USES the embedded library!
//
// COORDINATE SYSTEM:
//   - Component library: Normalized coordinates (e.g., 0-10 for small components)
//   - This binary: Transforms to ABSOLUTE screen pixels (1404x1872)
//   - Lamp receives: Absolute pixel coordinates for reMarkable 2 display
//
//   Example: Component has "pen down 1 1"
//            User calls: render_component("R", 500, 500, 2.0)
//            Transform: (1 * 2.0) + 500 = 502
//            Lamp gets: "pen down 502 502" (absolute screen pixel)

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <ctype.h>
#include "component_library.h"

#define LAMP_PIPE "/tmp/elxnk_lamp.pipe"

void print_usage(const char* prog) {
    printf("Usage: %s <component|text> <x> <y> [scale]\n", prog);
    printf("\nCommands:\n");
    printf("  list              - List all available components\n");
    printf("  <name> <x> <y>    - Render component at position\n");
    printf("  text <x> <y> <string> - Render text\n");
    printf("\nExamples:\n");
    printf("  %s R 500 500           # Render resistor\n", prog);
    printf("  %s list                # Show all components\n", prog);
    printf("  %s text 100 100 ABC    # Render text 'ABC'\n", prog);
}

// List all available components
void list_components() {
    printf("Available Components (%d):\n", elxnk::COMPONENT_COUNT);
    for (int i = 0; i < elxnk::COMPONENT_COUNT; i++) {
        printf("  %-10s (%d commands)\n",
               elxnk::COMPONENTS[i].name,
               elxnk::COMPONENTS[i].count);
    }
    printf("\nAvailable Font Glyphs (%d):\n", elxnk::FONT_GLYPH_COUNT);
    for (int i = 0; i < elxnk::FONT_GLYPH_COUNT; i++) {
        printf("  '%c' (%d commands)\n",
               elxnk::FONT_GLYPHS[i].character,
               elxnk::FONT_GLYPHS[i].count);
    }
}

// Send component to lamp pipe
void render_component(const char* name, int x, int y, float scale) {
    // Find component
    const elxnk::Component* comp = elxnk::find_component(name);
    if (!comp) {
        fprintf(stderr, "Error: Component '%s' not found\n", name);
        fprintf(stderr, "Use 'list' to see available components\n");
        return;
    }

    // Open lamp pipe
    int fd = open(LAMP_PIPE, O_WRONLY);
    if (fd < 0) {
        perror("Failed to open lamp pipe");
        fprintf(stderr, "Is elxnk running?\n");
        return;
    }

    printf("Rendering %s at (%d, %d) scale=%.2f (%d commands)\n",
           name, x, y, scale, comp->count);

    // Send commands
    for (int i = 0; i < comp->count; i++) {
        const char* cmd = comp->commands[i].cmd;
        char transformed[256];

        // Transform pen down X Y
        if (strncmp(cmd, "pen down ", 9) == 0) {
            int cx, cy;
            if (sscanf(cmd + 9, "%d %d", &cx, &cy) == 2) {
                cx = (int)(cx * scale) + x;
                cy = (int)(cy * scale) + y;
                snprintf(transformed, sizeof(transformed), "pen down %d %d\n", cx, cy);
                write(fd, transformed, strlen(transformed));
                continue;
            }
        }

        // Transform pen move X Y
        if (strncmp(cmd, "pen move ", 9) == 0) {
            int cx, cy;
            if (sscanf(cmd + 9, "%d %d", &cx, &cy) == 2) {
                cx = (int)(cx * scale) + x;
                cy = (int)(cy * scale) + y;
                snprintf(transformed, sizeof(transformed), "pen move %d %d\n", cx, cy);
                write(fd, transformed, strlen(transformed));
                continue;
            }
        }

        // Transform pen circle X Y R1 R2
        if (strncmp(cmd, "pen circle ", 11) == 0) {
            int cx, cy, r1, r2;
            if (sscanf(cmd + 11, "%d %d %d %d", &cx, &cy, &r1, &r2) == 4) {
                cx = (int)(cx * scale) + x;
                cy = (int)(cy * scale) + y;
                r1 = (int)(r1 * scale);
                r2 = (int)(r2 * scale);
                snprintf(transformed, sizeof(transformed), "pen circle %d %d %d %d\n", cx, cy, r1, r2);
                write(fd, transformed, strlen(transformed));
                continue;
            }
        }

        // Send as-is (pen up, etc.)
        write(fd, cmd, strlen(cmd));
        write(fd, "\n", 1);
    }

    close(fd);
}

// Render text using font glyphs
void render_text(int x, int y, const char* text, float scale, int spacing) {
    int fd = open(LAMP_PIPE, O_WRONLY);
    if (fd < 0) {
        perror("Failed to open lamp pipe");
        return;
    }

    printf("Rendering text: '%s' at (%d, %d)\n", text, x, y);

    int offset_x = x;
    for (const char* p = text; *p; p++) {
        char c = toupper(*p);

        if (c == ' ') {
            offset_x += spacing;
            continue;
        }

        const elxnk::FontGlyph* glyph = elxnk::find_glyph(c);
        if (!glyph) {
            fprintf(stderr, "Warning: Glyph '%c' not found\n", c);
            offset_x += spacing;
            continue;
        }

        // Render glyph
        for (int i = 0; i < glyph->count; i++) {
            const char* cmd = glyph->commands[i].cmd;
            char transformed[256];

            // Transform pen down X Y
            if (strncmp(cmd, "pen down ", 9) == 0) {
                int gx, gy;
                if (sscanf(cmd + 9, "%d %d", &gx, &gy) == 2) {
                    gx = (int)(gx * scale) + offset_x;
                    gy = (int)(gy * scale) + y;
                    snprintf(transformed, sizeof(transformed), "pen down %d %d\n", gx, gy);
                    write(fd, transformed, strlen(transformed));
                    continue;
                }
            }

            // Transform pen move X Y
            if (strncmp(cmd, "pen move ", 9) == 0) {
                int gx, gy;
                if (sscanf(cmd + 9, "%d %d", &gx, &gy) == 2) {
                    gx = (int)(gx * scale) + offset_x;
                    gy = (int)(gy * scale) + y;
                    snprintf(transformed, sizeof(transformed), "pen move %d %d\n", gx, gy);
                    write(fd, transformed, strlen(transformed));
                    continue;
                }
            }

            // Transform pen circle X Y R1 R2
            if (strncmp(cmd, "pen circle ", 11) == 0) {
                int gx, gy, r1, r2;
                if (sscanf(cmd + 11, "%d %d %d %d", &gx, &gy, &r1, &r2) == 4) {
                    gx = (int)(gx * scale) + offset_x;
                    gy = (int)(gy * scale) + y;
                    r1 = (int)(r1 * scale);
                    r2 = (int)(r2 * scale);
                    snprintf(transformed, sizeof(transformed), "pen circle %d %d %d %d\n", gx, gy, r1, r2);
                    write(fd, transformed, strlen(transformed));
                    continue;
                }
            }

            write(fd, cmd, strlen(cmd));
            write(fd, "\n", 1);
        }

        offset_x += (int)(25 * scale);
    }

    close(fd);
}

int main(int argc, char** argv) {
    if (argc < 2) {
        print_usage(argv[0]);
        return 1;
    }

    // List command
    if (strcmp(argv[1], "list") == 0) {
        list_components();
        return 0;
    }

    // Text command
    if (strcmp(argv[1], "text") == 0) {
        if (argc < 5) {
            fprintf(stderr, "Usage: %s text <x> <y> <string>\n", argv[0]);
            return 1;
        }
        int x = atoi(argv[2]);
        int y = atoi(argv[3]);
        float scale = (argc > 5) ? atof(argv[5]) : 1.0;
        int spacing = 25;

        render_text(x, y, argv[4], scale, spacing);
        return 0;
    }

    // Component rendering
    if (argc < 4) {
        fprintf(stderr, "Usage: %s <component> <x> <y> [scale]\n", argv[0]);
        return 1;
    }

    const char* name = argv[1];
    int x = atoi(argv[2]);
    int y = atoi(argv[3]);
    float scale = (argc > 4) ? atof(argv[4]) : 1.0;

    render_component(name, x, y, scale);

    return 0;
}
