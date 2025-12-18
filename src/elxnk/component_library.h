// Auto-generated component and font library
// Generated from SVG assets - DO NOT EDIT MANUALLY
// Build: make library (from src/ directory)
//
// LAMP COMMAND SYNTAX:
//   pen down X Y        - Put pen down at (X,Y), start drawing
//   pen move X Y        - Draw line from current position to (X,Y)
//   pen up              - Lift pen, stop drawing
//   pen circle X Y R1 R2 - Draw circle at (X,Y) with radii R1,R2

#ifndef ELXNK_LIBRARY_H
#define ELXNK_LIBRARY_H

#include <string>
#include <vector>
#include <map>

namespace elxnk {

// Lamp command structure
struct LampCommand {
    const char* cmd;
};

// Component definition
struct Component {
    const char* name;
    const LampCommand* commands;
    int count;
};

// Font glyph definition
struct FontGlyph {
    char character;
    const LampCommand* commands;
    int count;
};

// Component: D
static const LampCommand D_commands[] = {
    {"pen down 3 0"},
    {"pen move 3 3"},
    {"pen up"},
    {"pen down 5 0"},
    {"pen move 5 3"},
    {"pen move 3 1"},
    {"pen move 5 0"},
    {"pen up"},
    {"pen down 5 1"},
    {"pen move 3 1"},
    {"pen up"},
    {"pen down 3 1"},
    {"pen move 0 1"},
    {"pen up"},
    {"pen down 5 1"},
    {"pen move 8 1"},
    {"pen up"},
};

// Component: GND
static const LampCommand GND_commands[] = {
    {"pen down 1 2"},
    {"pen move 2 2"},
    {"pen up"},
    {"pen down 1 3"},
    {"pen move 1 3"},
    {"pen up"},
    {"pen down 1 1"},
    {"pen move 1 0"},
    {"pen up"},
    {"pen down 3 1"},
    {"pen move 0 1"},
    {"pen up"},
    {"pen down 1 0"},
    {"pen move 1 0"},
    {"pen up"},
};

// Component: L
static const LampCommand L_commands[] = {
    {"pen down 0 3"},
    {"pen up"},
    {"pen down 0 4"},
    {"pen up"},
    {"pen down 0 5"},
    {"pen up"},
    {"pen down 0 6"},
    {"pen up"},
    {"pen down 0 1"},
    {"pen move 0 0"},
    {"pen up"},
    {"pen down 0 6"},
    {"pen move 0 8"},
    {"pen up"},
};

// Component: NMOS
static const LampCommand NMOS_commands[] = {
    {"pen down 5 3"},
    {"pen move 5 7"},
    {"pen up"},
    {"pen down 5 5"},
    {"pen move 3 5"},
    {"pen up"},
    {"pen down 6 3"},
    {"pen move 6 4"},
    {"pen up"},
    {"pen down 6 5"},
    {"pen move 6 6"},
    {"pen up"},
    {"pen down 6 6"},
    {"pen move 6 7"},
    {"pen up"},
    {"pen down 6 7"},
    {"pen move 8 7"},
    {"pen move 8 3"},
    {"pen move 6 3"},
    {"pen up"},
    {"pen down 6 5"},
    {"pen move 7 5"},
    {"pen move 7 6"},
    {"pen move 6 5"},
    {"pen up"},
    {"pen circle 7 5 3 3"},
    {"pen down 8 3"},
    {"pen move 8 3"},
    {"pen up"},
    {"pen circle 8 3 1 1"},
    {"pen circle 8 7 1 1"},
    {"pen down 8 8"},
    {"pen move 8 5"},
    {"pen move 6 5"},
    {"pen up"},
    {"pen down 8 5"},
    {"pen move 9 5"},
    {"pen up"},
    {"pen down 8 5"},
    {"pen move 8 5"},
    {"pen move 9 5"},
    {"pen move 8 5"},
    {"pen up"},
    {"pen down 3 5"},
    {"pen move 0 5"},
    {"pen up"},
    {"pen down 8 3"},
    {"pen move 8 0"},
    {"pen up"},
    {"pen down 8 8"},
    {"pen move 8 10"},
    {"pen up"},
    {"pen down 3 5"},
    {"pen move 0 5"},
    {"pen up"},
    {"pen down 8 3"},
    {"pen move 8 0"},
    {"pen up"},
    {"pen down 8 8"},
    {"pen move 8 10"},
    {"pen up"},
};

// Component: NPN
static const LampCommand NPN_commands[] = {
    {"pen down 3 5"},
    {"pen move 6 5"},
    {"pen up"},
    {"pen down 6 3"},
    {"pen move 6 7"},
    {"pen up"},
    {"pen down 6 5"},
    {"pen move 8 3"},
    {"pen up"},
    {"pen down 6 6"},
    {"pen move 8 8"},
    {"pen up"},
    {"pen circle 6 5 3 3"},
    {"pen down 6 7"},
    {"pen move 7 6"},
    {"pen move 7 7"},
    {"pen move 6 7"},
    {"pen up"},
    {"pen down 3 5"},
    {"pen move 0 5"},
    {"pen up"},
    {"pen down 8 3"},
    {"pen move 8 0"},
    {"pen up"},
    {"pen down 8 8"},
    {"pen move 8 10"},
    {"pen up"},
    {"pen down 3 5"},
    {"pen move 0 5"},
    {"pen up"},
    {"pen down 8 3"},
    {"pen move 8 0"},
    {"pen up"},
    {"pen down 8 8"},
    {"pen move 8 10"},
    {"pen up"},
};

// Component: NP_C
static const LampCommand NP_C_commands[] = {
    {"pen down 0 3"},
    {"pen move 4 3"},
    {"pen up"},
    {"pen down 0 5"},
    {"pen move 4 5"},
    {"pen up"},
    {"pen down 2 3"},
    {"pen move 2 0"},
    {"pen up"},
    {"pen down 2 5"},
    {"pen move 2 8"},
    {"pen up"},
};

// Component: OPAMP
static const LampCommand OPAMP_commands[] = {
    {"pen down 13 8"},
    {"pen move 3 3"},
    {"pen move 3 13"},
    {"pen move 13 8"},
    {"pen up"},
    {"pen down 3 5"},
    {"pen move 0 5"},
    {"pen up"},
    {"pen down 3 5"},
    {"pen move 4 5"},
    {"pen up"},
    {"pen down 4 6"},
    {"pen move 4 5"},
    {"pen up"},
    {"pen down 3 10"},
    {"pen move 0 10"},
    {"pen up"},
    {"pen down 3 10"},
    {"pen move 4 10"},
    {"pen up"},
    {"pen down 5 4"},
    {"pen move 5 0"},
    {"pen up"},
    {"pen down 5 12"},
    {"pen move 5 15"},
    {"pen up"},
    {"pen down 13 8"},
    {"pen move 15 8"},
    {"pen up"},
};

// Component: PMOS
static const LampCommand PMOS_commands[] = {
    {"pen down 5 3"},
    {"pen move 5 7"},
    {"pen up"},
    {"pen down 5 5"},
    {"pen move 3 5"},
    {"pen up"},
    {"pen down 6 3"},
    {"pen move 6 4"},
    {"pen up"},
    {"pen down 6 3"},
    {"pen move 8 3"},
    {"pen move 8 7"},
    {"pen move 6 7"},
    {"pen up"},
    {"pen down 6 5"},
    {"pen move 6 6"},
    {"pen up"},
    {"pen down 6 6"},
    {"pen move 6 7"},
    {"pen up"},
    {"pen circle 7 5 3 3"},
    {"pen down 7 5"},
    {"pen move 6 5"},
    {"pen move 6 6"},
    {"pen move 7 5"},
    {"pen up"},
    {"pen down 8 3"},
    {"pen move 8 3"},
    {"pen up"},
    {"pen circle 8 3 1 1"},
    {"pen circle 8 7 1 1"},
    {"pen down 8 8"},
    {"pen move 8 5"},
    {"pen move 6 5"},
    {"pen up"},
    {"pen down 8 6"},
    {"pen move 9 6"},
    {"pen up"},
    {"pen down 8 6"},
    {"pen move 8 5"},
    {"pen move 9 5"},
    {"pen move 8 6"},
    {"pen up"},
    {"pen down 3 5"},
    {"pen move 0 5"},
    {"pen up"},
    {"pen down 8 3"},
    {"pen move 8 0"},
    {"pen up"},
    {"pen down 8 8"},
    {"pen move 8 10"},
    {"pen up"},
    {"pen down 3 5"},
    {"pen move 0 5"},
    {"pen up"},
    {"pen down 8 3"},
    {"pen move 8 0"},
    {"pen up"},
    {"pen down 8 8"},
    {"pen move 8 10"},
    {"pen up"},
};

// Component: PNP
static const LampCommand PNP_commands[] = {
    {"pen down 3 5"},
    {"pen move 6 5"},
    {"pen up"},
    {"pen down 6 3"},
    {"pen move 6 7"},
    {"pen up"},
    {"pen down 6 5"},
    {"pen move 8 3"},
    {"pen up"},
    {"pen down 6 6"},
    {"pen move 8 8"},
    {"pen up"},
    {"pen circle 6 5 3 3"},
    {"pen down 7 7"},
    {"pen move 7 7"},
    {"pen move 6 6"},
    {"pen move 7 7"},
    {"pen up"},
    {"pen down 3 5"},
    {"pen move 0 5"},
    {"pen up"},
    {"pen down 8 3"},
    {"pen move 8 0"},
    {"pen up"},
    {"pen down 8 8"},
    {"pen move 8 10"},
    {"pen up"},
    {"pen down 3 5"},
    {"pen move 0 5"},
    {"pen up"},
    {"pen down 8 3"},
    {"pen move 8 0"},
    {"pen up"},
    {"pen down 8 8"},
    {"pen move 8 10"},
    {"pen up"},
};

// Component: P_C
static const LampCommand P_C_commands[] = {
    {"pen down 0 3"},
    {"pen move 4 3"},
    {"pen up"},
    {"pen down 1 2"},
    {"pen move 2 2"},
    {"pen up"},
    {"pen down 1 2"},
    {"pen move 1 1"},
    {"pen up"},
    {"pen down 4 5"},
    {"pen up"},
    {"pen down 2 3"},
    {"pen move 2 0"},
    {"pen up"},
    {"pen down 2 4"},
    {"pen move 2 8"},
    {"pen up"},
};

// Component: R
static const LampCommand R_commands[] = {
    {"pen down 1 2"},
    {"pen move 1 1"},
    {"pen up"},
    {"pen down 1 2"},
    {"pen move 2 2"},
    {"pen move 1 2"},
    {"pen move 0 3"},
    {"pen move 1 3"},
    {"pen up"},
    {"pen down 1 3"},
    {"pen move 2 4"},
    {"pen move 1 4"},
    {"pen move 0 4"},
    {"pen move 1 5"},
    {"pen up"},
    {"pen down 1 5"},
    {"pen move 2 5"},
    {"pen move 1 5"},
    {"pen move 0 6"},
    {"pen move 1 6"},
    {"pen up"},
    {"pen down 1 6"},
    {"pen move 1 6"},
    {"pen up"},
    {"pen down 1 1"},
    {"pen move 1 0"},
    {"pen up"},
    {"pen down 1 6"},
    {"pen move 1 8"},
    {"pen up"},
};

// Component: R_TRIM
static const LampCommand R_TRIM_commands[] = {
    {"pen down 1 1"},
    {"pen move 1 2"},
    {"pen up"},
    {"pen down 1 2"},
    {"pen move 2 2"},
    {"pen move 1 2"},
    {"pen move 0 3"},
    {"pen move 1 3"},
    {"pen up"},
    {"pen down 1 3"},
    {"pen move 2 4"},
    {"pen move 1 4"},
    {"pen move 0 4"},
    {"pen move 1 5"},
    {"pen up"},
    {"pen down 1 5"},
    {"pen move 2 5"},
    {"pen move 1 5"},
    {"pen move 0 6"},
    {"pen move 1 6"},
    {"pen up"},
    {"pen down 1 6"},
    {"pen move 1 6"},
    {"pen up"},
    {"pen down 2 4"},
    {"pen move 3 3"},
    {"pen move 3 4"},
    {"pen move 2 4"},
    {"pen up"},
    {"pen down 4 4"},
    {"pen move 3 4"},
    {"pen up"},
    {"pen down 1 1"},
    {"pen move 1 0"},
    {"pen up"},
    {"pen down 1 6"},
    {"pen move 1 8"},
    {"pen up"},
    {"pen down 4 4"},
    {"pen move 5 4"},
    {"pen up"},
};

// Component: TX
static const LampCommand TX_commands[] = {
    {"pen down 8 3"},
    {"pen up"},
    {"pen down 8 5"},
    {"pen up"},
    {"pen down 8 8"},
    {"pen up"},
    {"pen down 8 10"},
    {"pen up"},
    {"pen down 9 1"},
    {"pen up"},
    {"pen down 9 4"},
    {"pen up"},
    {"pen down 9 6"},
    {"pen up"},
    {"pen down 9 9"},
    {"pen up"},
    {"pen down 10 0"},
    {"pen move 10 10"},
    {"pen up"},
    {"pen down 11 10"},
    {"pen move 11 0"},
    {"pen up"},
    {"pen down 13 0"},
    {"pen up"},
    {"pen down 13 3"},
    {"pen up"},
    {"pen down 13 5"},
    {"pen up"},
    {"pen down 12 1"},
    {"pen up"},
    {"pen down 12 4"},
    {"pen up"},
    {"pen down 12 6"},
    {"pen up"},
    {"pen down 13 8"},
    {"pen up"},
    {"pen down 12 9"},
    {"pen up"},
    {"pen down 8 0"},
    {"pen move 0 0"},
    {"pen up"},
    {"pen down 8 10"},
    {"pen move 0 10"},
    {"pen up"},
    {"pen down 13 0"},
    {"pen move 20 0"},
    {"pen up"},
    {"pen down 13 10"},
    {"pen move 20 10"},
    {"pen up"},
};

// Component: VAC
static const LampCommand VAC_commands[] = {
    {"pen down 3 5"},
    {"pen up"},
    {"pen down 3 5"},
    {"pen up"},
    {"pen circle 3 5 3 3"},
    {"pen down 2 3"},
    {"pen move 3 3"},
    {"pen up"},
    {"pen down 3 4"},
    {"pen move 3 3"},
    {"pen up"},
    {"pen down 3 3"},
    {"pen move 3 0"},
    {"pen up"},
    {"pen down 3 8"},
    {"pen move 3 10"},
    {"pen up"},
    {"pen down 2 7"},
    {"pen move 3 7"},
    {"pen up"},
};

// Component: VAR
static const LampCommand VAR_commands[] = {
    {"pen down 2 2"},
    {"pen move 2 1"},
    {"pen up"},
    {"pen down 2 2"},
    {"pen move 3 2"},
    {"pen move 2 2"},
    {"pen move 1 3"},
    {"pen move 2 3"},
    {"pen up"},
    {"pen down 2 3"},
    {"pen move 3 4"},
    {"pen move 2 4"},
    {"pen move 1 4"},
    {"pen move 2 5"},
    {"pen up"},
    {"pen down 2 5"},
    {"pen move 3 5"},
    {"pen move 2 5"},
    {"pen move 1 6"},
    {"pen move 2 6"},
    {"pen up"},
    {"pen down 2 6"},
    {"pen move 2 6"},
    {"pen up"},
    {"pen down 4 2"},
    {"pen move 5 1"},
    {"pen move 4 2"},
    {"pen move 5 1"},
    {"pen move 0 6"},
    {"pen up"},
    {"pen down 2 1"},
    {"pen move 2 0"},
    {"pen up"},
    {"pen down 2 6"},
    {"pen move 2 8"},
    {"pen up"},
};

// Component: VDC
static const LampCommand VDC_commands[] = {
    {"pen down 1 5"},
    {"pen move 4 5"},
    {"pen up"},
    {"pen down 2 5"},
    {"pen move 1 5"},
    {"pen up"},
    {"pen down 3 5"},
    {"pen move 2 5"},
    {"pen up"},
    {"pen down 4 5"},
    {"pen move 3 5"},
    {"pen up"},
    {"pen circle 3 5 3 3"},
    {"pen down 2 3"},
    {"pen move 3 3"},
    {"pen up"},
    {"pen down 3 4"},
    {"pen move 3 3"},
    {"pen up"},
    {"pen down 3 3"},
    {"pen move 3 0"},
    {"pen up"},
    {"pen down 3 8"},
    {"pen move 3 10"},
    {"pen up"},
    {"pen down 2 7"},
    {"pen move 3 7"},
    {"pen up"},
};

// Component: ZD
static const LampCommand ZD_commands[] = {
    {"pen down 3 0"},
    {"pen move 3 3"},
    {"pen up"},
    {"pen down 5 0"},
    {"pen move 5 3"},
    {"pen move 3 1"},
    {"pen move 5 0"},
    {"pen up"},
    {"pen down 5 1"},
    {"pen move 3 1"},
    {"pen up"},
    {"pen down 3 1"},
    {"pen move 0 1"},
    {"pen up"},
    {"pen down 5 1"},
    {"pen move 8 1"},
    {"pen up"},
    {"pen down 3 3"},
    {"pen move 2 3"},
    {"pen up"},
    {"pen down 3 0"},
    {"pen move 3 0"},
    {"pen up"},
};

// Font: G
static const LampCommand font_G_commands[] = {
    {"pen down 25 16"},
    {"pen move 19 18"},
    {"pen move 7 20"},
    {"pen move 25 3"},
    {"pen move 26 10"},
    {"pen move 25 17"},
    {"pen move 25 24"},
    {"pen move 16 38"},
    {"pen move 1 33"},
    {"pen up"},
};

// Font: H
static const LampCommand font_H_commands[] = {
    {"pen down 8 0"},
    {"pen move 2 32"},
    {"pen up"},
    {"pen down 2 37"},
    {"pen move 13 27"},
    {"pen move 21 19"},
    {"pen move 33 33"},
    {"pen move 39 25"},
    {"pen up"},
    {"pen down 1 36"},
    {"pen move 2 37"},
    {"pen up"},
};

// Font: I
static const LampCommand font_I_commands[] = {
    {"pen down 8 0"},
    {"pen move 8 1"},
    {"pen up"},
    {"pen down 4 15"},
    {"pen move 19 29"},
    {"pen up"},
};

// Font: J
static const LampCommand font_J_commands[] = {
    {"pen down 16 1"},
    {"pen move 14 20"},
    {"pen move 14 29"},
    {"pen move 0 36"},
    {"pen up"},
};

// Font: K
static const LampCommand font_K_commands[] = {
    {"pen down 6 0"},
    {"pen move 3 20"},
    {"pen move 2 35"},
    {"pen up"},
    {"pen down 22 10"},
    {"pen move 5 28"},
    {"pen up"},
    {"pen down 9 25"},
    {"pen move 23 33"},
    {"pen move 34 35"},
    {"pen up"},
};

// Font: L
static const LampCommand font_L_commands[] = {
    {"pen down 6 0"},
    {"pen move 2 27"},
    {"pen move 3 36"},
    {"pen move 21 31"},
    {"pen up"},
};

// Font: M
static const LampCommand font_M_commands[] = {
    {"pen down 3 2"},
    {"pen move 2 12"},
    {"pen move 2 19"},
    {"pen move 17 3"},
    {"pen move 22 18"},
    {"pen move 35 3"},
    {"pen move 39 10"},
    {"pen move 51 16"},
    {"pen move 53 12"},
    {"pen up"},
};

// Font: N
static const LampCommand font_N_commands[] = {
    {"pen down 5 1"},
    {"pen move 3 10"},
    {"pen move 2 20"},
    {"pen up"},
    {"pen down 5 17"},
    {"pen move 11 11"},
    {"pen move 17 7"},
    {"pen move 19 11"},
    {"pen move 21 18"},
    {"pen move 35 18"},
    {"pen up"},
};

// Font: O
static const LampCommand font_O_commands[] = {
    {"pen down 11 2"},
    {"pen move 21 11"},
    {"pen move 8 23"},
    {"pen move 11 2"},
    {"pen up"},
};

// Font: P
static const LampCommand font_P_commands[] = {
    {"pen down 8 1"},
    {"pen move 7 10"},
    {"pen move 7 17"},
    {"pen move 13 24"},
    {"pen move 21 19"},
    {"pen move 27 4"},
    {"pen move 18 4"},
    {"pen move 9 15"},
    {"pen up"},
    {"pen down 6 22"},
    {"pen move 2 38"},
    {"pen up"},
};

// Font: Q
static const LampCommand font_Q_commands[] = {
    {"pen down 17 17"},
    {"pen move 12 21"},
    {"pen move 3 23"},
    {"pen move 20 3"},
    {"pen move 20 16"},
    {"pen move 16 29"},
    {"pen move 14 39"},
    {"pen move 13 42"},
    {"pen up"},
};

// Font: R
static const LampCommand font_R_commands[] = {
    {"pen down 2 16"},
    {"pen move 9 12"},
    {"pen move 22 2"},
    {"pen up"},
    {"pen down 5 2"},
    {"pen move 2 12"},
    {"pen up"},
    {"pen down 1 15"},
    {"pen move 2 16"},
    {"pen up"},
    {"pen down 4 18"},
    {"pen move 4 22"},
    {"pen up"},
};

// Font: S
static const LampCommand font_S_commands[] = {
    {"pen down 6 1"},
    {"pen move 9 21"},
    {"pen move 2 31"},
    {"pen up"},
};

// Font: T
static const LampCommand font_T_commands[] = {
    {"pen down 12 1"},
    {"pen move 23 11"},
    {"pen up"},
    {"pen down 9 11"},
    {"pen move 0 13"},
    {"pen up"},
    {"pen down 6 14"},
    {"pen move 5 21"},
    {"pen move 13 33"},
    {"pen move 19 28"},
    {"pen up"},
};

// Font: U
static const LampCommand font_U_commands[] = {
    {"pen down 6 1"},
    {"pen move 2 14"},
    {"pen move 7 20"},
    {"pen move 14 15"},
    {"pen move 23 1"},
    {"pen up"},
    {"pen down 21 10"},
    {"pen move 34 16"},
    {"pen up"},
};

// Font: V
static const LampCommand font_V_commands[] = {
    {"pen down 25 1"},
    {"pen move 7 22"},
    {"pen move 2 2"},
    {"pen up"},
};

// Font: W
static const LampCommand font_W_commands[] = {
    {"pen down 40 1"},
    {"pen move 33 15"},
    {"pen move 28 21"},
    {"pen move 23 14"},
    {"pen move 21 8"},
    {"pen move 9 24"},
    {"pen move 2 6"},
    {"pen up"},
    {"pen down 1 6"},
    {"pen move 2 6"},
    {"pen up"},
};

// Font: X
static const LampCommand font_X_commands[] = {
    {"pen down 4 1"},
    {"pen move 11 14"},
    {"pen move 33 15"},
    {"pen up"},
    {"pen down 21 1"},
    {"pen move 11 14"},
    {"pen up"},
    {"pen down 10 14"},
    {"pen move 1 23"},
    {"pen up"},
};

// Font: Y
static const LampCommand font_Y_commands[] = {
    {"pen down 26 0"},
    {"pen move 22 14"},
    {"pen move 19 23"},
    {"pen move 14 39"},
    {"pen up"},
    {"pen down 7 1"},
    {"pen move 3 10"},
    {"pen move 5 19"},
    {"pen move 21 15"},
    {"pen up"},
    {"pen down 0 16"},
    {"pen move 2 16"},
    {"pen up"},
};

// Font: Z
static const LampCommand font_Z_commands[] = {
    {"pen down 4 2"},
    {"pen move 21 3"},
    {"pen move 2 26"},
    {"pen up"},
    {"pen down 22 1"},
    {"pen move 21 2"},
    {"pen up"},
    {"pen down 33 18"},
    {"pen move 10 21"},
    {"pen up"},
};

// Component lookup table
static const Component COMPONENTS[] = {
    {"D", D_commands, 17},
    {"GND", GND_commands, 15},
    {"L", L_commands, 14},
    {"NMOS", NMOS_commands, 61},
    {"NPN", NPN_commands, 36},
    {"NP_C", NP_C_commands, 12},
    {"OPAMP", OPAMP_commands, 29},
    {"PMOS", PMOS_commands, 61},
    {"PNP", PNP_commands, 36},
    {"P_C", P_C_commands, 17},
    {"R", R_commands, 30},
    {"R_TRIM", R_TRIM_commands, 41},
    {"TX", TX_commands, 50},
    {"VAC", VAC_commands, 20},
    {"VAR", VAR_commands, 36},
    {"VDC", VDC_commands, 28},
    {"ZD", ZD_commands, 23},
};

static const int COMPONENT_COUNT = 17;

// Font glyph lookup table
static const FontGlyph FONT_GLYPHS[] = {
    {'G', font_G_commands, 10},
    {'H', font_H_commands, 12},
    {'I', font_I_commands, 6},
    {'J', font_J_commands, 5},
    {'K', font_K_commands, 11},
    {'L', font_L_commands, 5},
    {'M', font_M_commands, 10},
    {'N', font_N_commands, 11},
    {'O', font_O_commands, 5},
    {'P', font_P_commands, 12},
    {'Q', font_Q_commands, 9},
    {'R', font_R_commands, 13},
    {'S', font_S_commands, 4},
    {'T', font_T_commands, 11},
    {'U', font_U_commands, 9},
    {'V', font_V_commands, 4},
    {'W', font_W_commands, 11},
    {'X', font_X_commands, 10},
    {'Y', font_Y_commands, 13},
    {'Z', font_Z_commands, 10},
};

static const int FONT_GLYPH_COUNT = 20;

// Helper functions
inline const Component* find_component(const char* name) {
    for (int i = 0; i < COMPONENT_COUNT; i++) {
        if (strcmp(COMPONENTS[i].name, name) == 0) {
            return &COMPONENTS[i];
        }
    }
    return nullptr;
}

inline const FontGlyph* find_glyph(char c) {
    for (int i = 0; i < FONT_GLYPH_COUNT; i++) {
        if (FONT_GLYPHS[i].character == c) {
            return &FONT_GLYPHS[i];
        }
    }
    return nullptr;
}

} // namespace elxnk

#endif // ELXNK_LIBRARY_H
