// Auto-generated component and font library - PIXEL SCALE VERSION
// Generated from SVG assets - DO NOT EDIT MANUALLY
// Build: make library (from src/ directory)
//
// COORDINATES: Stored in PIXELS at 10 pixels/mm scale
// - reMarkable 2: 1404x1872 pixels
// - Example: R resistor is 22x78 pixels (2.2mm x 7.8mm actual size)
// - render_component() should apply POSITION OFFSET ONLY, NO SCALING
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
    {"pen down 26 1"},
    {"pen move 26 27"},
    {"pen up"},
    {"pen down 52 1"},
    {"pen move 52 27"},
    {"pen move 26 14"},
    {"pen move 52 1"},
    {"pen up"},
    {"pen down 52 14"},
    {"pen move 26 14"},
    {"pen up"},
    {"pen down 26 14"},
    {"pen move 1 14"},
    {"pen up"},
    {"pen down 52 14"},
    {"pen move 77 14"},
    {"pen up"},
};

// Component: GND
static const LampCommand GND_commands[] = {
    {"pen down 7 20"},
    {"pen move 20 20"},
    {"pen up"},
    {"pen down 12 26"},
    {"pen move 15 26"},
    {"pen up"},
    {"pen down 13 13"},
    {"pen move 13 1"},
    {"pen up"},
    {"pen down 26 13"},
    {"pen move 1 13"},
    {"pen up"},
    {"pen down 13 1"},
    {"pen move 13 1"},
    {"pen up"},
};

// Component: L
static const LampCommand L_commands[] = {
    {"pen down 1 26"},
    {"pen up"},
    {"pen down 1 39"},
    {"pen up"},
    {"pen down 1 52"},
    {"pen up"},
    {"pen down 1 64"},
    {"pen up"},
    {"pen down 1 13"},
    {"pen move 1 1"},
    {"pen up"},
    {"pen down 1 64"},
    {"pen move 1 77"},
    {"pen up"},
};

// Component: NMOS
static const LampCommand NMOS_commands[] = {
    {"pen down 54 33"},
    {"pen move 54 71"},
    {"pen up"},
    {"pen down 54 52"},
    {"pen move 26 52"},
    {"pen up"},
    {"pen down 59 29"},
    {"pen move 59 39"},
    {"pen up"},
    {"pen down 59 46"},
    {"pen move 59 57"},
    {"pen up"},
    {"pen down 59 64"},
    {"pen move 59 74"},
    {"pen up"},
    {"pen down 59 69"},
    {"pen move 85 69"},
    {"pen move 85 34"},
    {"pen move 59 34"},
    {"pen up"},
    {"pen down 62 52"},
    {"pen move 72 48"},
    {"pen move 72 55"},
    {"pen move 62 52"},
    {"pen up"},
    {"pen down 77 26"},
    {"pen move 77 34"},
    {"pen up"},
    {"pen down 77 77"},
    {"pen move 77 52"},
    {"pen move 59 52"},
    {"pen up"},
    {"pen down 81 48"},
    {"pen move 88 48"},
    {"pen up"},
    {"pen down 85 48"},
    {"pen move 81 54"},
    {"pen move 88 54"},
    {"pen move 85 48"},
    {"pen up"},
    {"pen down 26 52"},
    {"pen move 1 52"},
    {"pen up"},
    {"pen down 77 26"},
    {"pen move 77 1"},
    {"pen up"},
    {"pen down 77 77"},
    {"pen move 77 102"},
    {"pen up"},
    {"pen down 26 52"},
    {"pen move 1 52"},
    {"pen up"},
    {"pen down 77 26"},
    {"pen move 77 1"},
    {"pen up"},
    {"pen down 77 77"},
    {"pen move 77 102"},
    {"pen up"},
    {"pen circle 68 52 28 28"},
    {"pen circle 77 34 3 3"},
    {"pen circle 77 69 3 3"},
};

// Component: NPN
static const LampCommand NPN_commands[] = {
    {"pen down 26 52"},
    {"pen move 58 52"},
    {"pen up"},
    {"pen down 58 33"},
    {"pen move 58 71"},
    {"pen up"},
    {"pen down 58 45"},
    {"pen move 77 26"},
    {"pen up"},
    {"pen down 58 58"},
    {"pen move 77 77"},
    {"pen up"},
    {"pen down 64 69"},
    {"pen move 69 64"},
    {"pen move 74 74"},
    {"pen move 64 69"},
    {"pen up"},
    {"pen down 26 52"},
    {"pen move 1 52"},
    {"pen up"},
    {"pen down 77 26"},
    {"pen move 77 1"},
    {"pen up"},
    {"pen down 77 77"},
    {"pen move 77 102"},
    {"pen up"},
    {"pen down 26 52"},
    {"pen move 1 52"},
    {"pen up"},
    {"pen down 77 26"},
    {"pen move 77 1"},
    {"pen up"},
    {"pen down 77 77"},
    {"pen move 77 102"},
    {"pen up"},
    {"pen circle 64 52 28 28"},
};

// Component: NP_C
static const LampCommand NP_C_commands[] = {
    {"pen down 3 31"},
    {"pen move 43 31"},
    {"pen up"},
    {"pen down 3 46"},
    {"pen move 43 46"},
    {"pen up"},
    {"pen down 23 29"},
    {"pen move 23 1"},
    {"pen up"},
    {"pen down 23 49"},
    {"pen move 23 77"},
    {"pen up"},
};

// Component: OPAMP
static const LampCommand OPAMP_commands[] = {
    {"pen down 128 77"},
    {"pen move 26 26"},
    {"pen move 26 128"},
    {"pen move 128 77"},
    {"pen up"},
    {"pen down 26 52"},
    {"pen move 1 52"},
    {"pen up"},
    {"pen down 33 53"},
    {"pen move 42 53"},
    {"pen up"},
    {"pen down 38 57"},
    {"pen move 38 48"},
    {"pen up"},
    {"pen down 26 102"},
    {"pen move 1 102"},
    {"pen up"},
    {"pen down 33 103"},
    {"pen move 42 103"},
    {"pen up"},
    {"pen down 52 39"},
    {"pen move 52 1"},
    {"pen up"},
    {"pen down 52 115"},
    {"pen move 52 153"},
    {"pen up"},
    {"pen down 128 77"},
    {"pen move 153 77"},
    {"pen up"},
};

// Component: PMOS
static const LampCommand PMOS_commands[] = {
    {"pen down 54 33"},
    {"pen move 54 71"},
    {"pen up"},
    {"pen down 54 52"},
    {"pen move 26 52"},
    {"pen up"},
    {"pen down 59 29"},
    {"pen move 59 39"},
    {"pen up"},
    {"pen down 59 34"},
    {"pen move 85 34"},
    {"pen move 85 69"},
    {"pen move 59 69"},
    {"pen up"},
    {"pen down 59 46"},
    {"pen move 59 57"},
    {"pen up"},
    {"pen down 59 64"},
    {"pen move 59 74"},
    {"pen up"},
    {"pen down 74 52"},
    {"pen move 64 48"},
    {"pen move 64 55"},
    {"pen move 74 52"},
    {"pen up"},
    {"pen down 77 26"},
    {"pen move 77 34"},
    {"pen up"},
    {"pen down 77 77"},
    {"pen move 77 52"},
    {"pen move 59 52"},
    {"pen up"},
    {"pen down 81 55"},
    {"pen move 88 55"},
    {"pen up"},
    {"pen down 85 55"},
    {"pen move 81 49"},
    {"pen move 88 49"},
    {"pen move 85 55"},
    {"pen up"},
    {"pen down 26 52"},
    {"pen move 1 52"},
    {"pen up"},
    {"pen down 77 26"},
    {"pen move 77 1"},
    {"pen up"},
    {"pen down 77 77"},
    {"pen move 77 102"},
    {"pen up"},
    {"pen down 26 52"},
    {"pen move 1 52"},
    {"pen up"},
    {"pen down 77 26"},
    {"pen move 77 1"},
    {"pen up"},
    {"pen down 77 77"},
    {"pen move 77 102"},
    {"pen up"},
    {"pen circle 68 52 28 28"},
    {"pen circle 77 34 3 3"},
    {"pen circle 77 69 3 3"},
};

// Component: PNP
static const LampCommand PNP_commands[] = {
    {"pen down 26 52"},
    {"pen move 58 52"},
    {"pen up"},
    {"pen down 58 33"},
    {"pen move 58 71"},
    {"pen up"},
    {"pen down 58 45"},
    {"pen move 77 26"},
    {"pen up"},
    {"pen down 58 58"},
    {"pen move 77 77"},
    {"pen up"},
    {"pen down 74 69"},
    {"pen move 69 74"},
    {"pen move 64 64"},
    {"pen move 74 69"},
    {"pen up"},
    {"pen down 26 52"},
    {"pen move 1 52"},
    {"pen up"},
    {"pen down 77 26"},
    {"pen move 77 1"},
    {"pen up"},
    {"pen down 77 77"},
    {"pen move 77 102"},
    {"pen up"},
    {"pen down 26 52"},
    {"pen move 1 52"},
    {"pen up"},
    {"pen down 77 26"},
    {"pen move 77 1"},
    {"pen up"},
    {"pen down 77 77"},
    {"pen move 77 102"},
    {"pen up"},
    {"pen circle 64 52 28 28"},
};

// Component: P_C
static const LampCommand P_C_commands[] = {
    {"pen down 3 31"},
    {"pen move 43 31"},
    {"pen up"},
    {"pen down 5 16"},
    {"pen move 15 16"},
    {"pen up"},
    {"pen down 10 21"},
    {"pen move 10 11"},
    {"pen up"},
    {"pen down 43 52"},
    {"pen up"},
    {"pen down 23 29"},
    {"pen move 23 1"},
    {"pen up"},
    {"pen down 23 44"},
    {"pen move 23 77"},
    {"pen up"},
};

// Component: R
static const LampCommand R_commands[] = {
    {"pen down 11 16"},
    {"pen move 11 13"},
    {"pen up"},
    {"pen down 11 16"},
    {"pen move 21 20"},
    {"pen move 11 24"},
    {"pen move 1 27"},
    {"pen move 11 31"},
    {"pen up"},
    {"pen down 11 31"},
    {"pen move 21 35"},
    {"pen move 11 39"},
    {"pen move 1 43"},
    {"pen move 11 46"},
    {"pen up"},
    {"pen down 11 46"},
    {"pen move 21 50"},
    {"pen move 11 54"},
    {"pen move 1 58"},
    {"pen move 11 62"},
    {"pen up"},
    {"pen down 11 62"},
    {"pen move 11 64"},
    {"pen up"},
    {"pen down 11 13"},
    {"pen move 11 1"},
    {"pen up"},
    {"pen down 11 64"},
    {"pen move 11 77"},
    {"pen up"},
};

// Component: R_TRIM
static const LampCommand R_TRIM_commands[] = {
    {"pen down 11 13"},
    {"pen move 11 16"},
    {"pen up"},
    {"pen down 11 16"},
    {"pen move 21 20"},
    {"pen move 11 24"},
    {"pen move 1 27"},
    {"pen move 11 31"},
    {"pen up"},
    {"pen down 11 31"},
    {"pen move 21 35"},
    {"pen move 11 39"},
    {"pen move 1 43"},
    {"pen move 11 46"},
    {"pen up"},
    {"pen down 11 46"},
    {"pen move 21 50"},
    {"pen move 11 54"},
    {"pen move 1 58"},
    {"pen move 11 62"},
    {"pen up"},
    {"pen down 11 62"},
    {"pen move 11 64"},
    {"pen up"},
    {"pen down 22 39"},
    {"pen move 34 34"},
    {"pen move 34 44"},
    {"pen move 22 39"},
    {"pen up"},
    {"pen down 36 39"},
    {"pen move 26 39"},
    {"pen up"},
    {"pen down 11 13"},
    {"pen move 11 1"},
    {"pen up"},
    {"pen down 11 64"},
    {"pen move 11 77"},
    {"pen up"},
    {"pen down 36 39"},
    {"pen move 49 39"},
    {"pen up"},
};

// Component: TX
static const LampCommand TX_commands[] = {
    {"pen down 77 26"},
    {"pen up"},
    {"pen down 77 51"},
    {"pen up"},
    {"pen down 77 77"},
    {"pen up"},
    {"pen down 77 102"},
    {"pen up"},
    {"pen down 90 13"},
    {"pen up"},
    {"pen down 90 39"},
    {"pen up"},
    {"pen down 90 64"},
    {"pen up"},
    {"pen down 90 90"},
    {"pen up"},
    {"pen down 96 1"},
    {"pen move 96 102"},
    {"pen up"},
    {"pen down 109 102"},
    {"pen move 109 1"},
    {"pen up"},
    {"pen down 128 1"},
    {"pen up"},
    {"pen down 128 26"},
    {"pen up"},
    {"pen down 128 52"},
    {"pen up"},
    {"pen down 115 13"},
    {"pen up"},
    {"pen down 115 39"},
    {"pen up"},
    {"pen down 115 64"},
    {"pen up"},
    {"pen down 128 77"},
    {"pen up"},
    {"pen down 116 90"},
    {"pen up"},
    {"pen down 77 1"},
    {"pen move 1 1"},
    {"pen up"},
    {"pen down 77 102"},
    {"pen move 1 102"},
    {"pen up"},
    {"pen down 128 1"},
    {"pen move 204 1"},
    {"pen up"},
    {"pen down 128 102"},
    {"pen move 204 102"},
    {"pen up"},
};

// Component: VAC
static const LampCommand VAC_commands[] = {
    {"pen down 27 52"},
    {"pen up"},
    {"pen down 27 52"},
    {"pen up"},
    {"pen down 22 33"},
    {"pen move 32 33"},
    {"pen up"},
    {"pen down 27 38"},
    {"pen move 27 29"},
    {"pen up"},
    {"pen down 27 26"},
    {"pen move 27 1"},
    {"pen up"},
    {"pen down 27 77"},
    {"pen move 27 102"},
    {"pen up"},
    {"pen down 22 66"},
    {"pen move 32 66"},
    {"pen up"},
    {"pen circle 27 52 25 25"},
};

// Component: VAR
static const LampCommand VAR_commands[] = {
    {"pen down 21 16"},
    {"pen move 21 13"},
    {"pen up"},
    {"pen down 21 16"},
    {"pen move 31 20"},
    {"pen move 21 24"},
    {"pen move 11 27"},
    {"pen move 21 31"},
    {"pen up"},
    {"pen down 21 31"},
    {"pen move 31 35"},
    {"pen move 21 39"},
    {"pen move 11 43"},
    {"pen move 21 46"},
    {"pen up"},
    {"pen down 21 46"},
    {"pen move 31 50"},
    {"pen move 21 54"},
    {"pen move 11 58"},
    {"pen move 21 62"},
    {"pen up"},
    {"pen down 21 62"},
    {"pen move 21 64"},
    {"pen up"},
    {"pen down 44 24"},
    {"pen move 46 13"},
    {"pen move 36 16"},
    {"pen move 46 13"},
    {"pen move 1 59"},
    {"pen up"},
    {"pen down 21 13"},
    {"pen move 21 1"},
    {"pen up"},
    {"pen down 21 64"},
    {"pen move 21 77"},
    {"pen up"},
};

// Component: VDC
static const LampCommand VDC_commands[] = {
    {"pen down 14 49"},
    {"pen move 39 49"},
    {"pen up"},
    {"pen down 19 54"},
    {"pen move 14 54"},
    {"pen up"},
    {"pen down 29 54"},
    {"pen move 24 54"},
    {"pen up"},
    {"pen down 39 54"},
    {"pen move 34 54"},
    {"pen up"},
    {"pen down 22 33"},
    {"pen move 32 33"},
    {"pen up"},
    {"pen down 27 38"},
    {"pen move 27 29"},
    {"pen up"},
    {"pen down 27 26"},
    {"pen move 27 1"},
    {"pen up"},
    {"pen down 27 77"},
    {"pen move 27 102"},
    {"pen up"},
    {"pen down 22 66"},
    {"pen move 32 66"},
    {"pen up"},
    {"pen circle 27 52 25 25"},
};

// Component: ZD
static const LampCommand ZD_commands[] = {
    {"pen down 26 1"},
    {"pen move 26 27"},
    {"pen up"},
    {"pen down 52 1"},
    {"pen move 52 27"},
    {"pen move 26 14"},
    {"pen move 52 1"},
    {"pen up"},
    {"pen down 52 14"},
    {"pen move 26 14"},
    {"pen up"},
    {"pen down 26 14"},
    {"pen move 1 14"},
    {"pen up"},
    {"pen down 52 14"},
    {"pen move 77 14"},
    {"pen up"},
    {"pen down 26 27"},
    {"pen move 19 27"},
    {"pen up"},
    {"pen down 34 1"},
    {"pen move 26 1"},
    {"pen up"},
};

// Font: 0
static const LampCommand font_0_commands[] = {
    {"pen down 224 19"},
    {"pen move 340 191"},
    {"pen move 264 276"},
    {"pen move 94 341"},
    {"pen move 27 191"},
    {"pen move 224 19"},
    {"pen up"},
};

// Font: 1
static const LampCommand font_1_commands[] = {
    {"pen down 17 84"},
    {"pen move 157 24"},
    {"pen move 157 294"},
    {"pen move 7 354"},
    {"pen up"},
    {"pen down 157 294"},
    {"pen move 287 274"},
    {"pen up"},
};

// Font: 2
static const LampCommand font_2_commands[] = {
    {"pen down 68 75"},
    {"pen move 178 20"},
    {"pen move 271 115"},
    {"pen move 168 244"},
    {"pen move 106 299"},
    {"pen move 18 385"},
    {"pen up"},
    {"pen down 118 295"},
    {"pen move 348 315"},
    {"pen up"},
};

// Font: 3
static const LampCommand font_3_commands[] = {
    {"pen down 15 19"},
    {"pen move 145 29"},
    {"pen move 235 19"},
    {"pen up"},
    {"pen down 215 39"},
    {"pen move 166 86"},
    {"pen move 85 159"},
    {"pen up"},
    {"pen down 145 149"},
    {"pen move 275 209"},
    {"pen move 5 319"},
    {"pen up"},
};

// Font: 4
static const LampCommand font_4_commands[] = {
    {"pen down 254 7"},
    {"pen move 229 107"},
    {"pen move 233 191"},
    {"pen move 324 217"},
    {"pen up"},
    {"pen down 14 217"},
    {"pen move 234 27"},
    {"pen up"},
    {"pen down 34 197"},
    {"pen move 204 207"},
    {"pen move 194 337"},
    {"pen up"},
};

// Font: 5
static const LampCommand font_5_commands[] = {
    {"pen down 10 309"},
    {"pen move 253 229"},
    {"pen move 160 162"},
    {"pen move 81 134"},
    {"pen move 190 29"},
    {"pen move 340 19"},
    {"pen up"},
};

// Font: 6
static const LampCommand font_6_commands[] = {
    {"pen down 95 318"},
    {"pen move 287 210"},
    {"pen move 235 301"},
    {"pen move 41 296"},
    {"pen move 205 18"},
    {"pen up"},
};

// Font: 7
static const LampCommand font_7_commands[] = {
    {"pen down 270 12"},
    {"pen move 100 322"},
    {"pen up"},
    {"pen down 0 22"},
    {"pen move 240 22"},
    {"pen up"},
};

// Font: 8
static const LampCommand font_8_commands[] = {
    {"pen down 110 187"},
    {"pen move 45 87"},
    {"pen move 194 41"},
    {"pen move 71 227"},
    {"pen move 35 305"},
    {"pen move 130 337"},
    {"pen move 150 187"},
    {"pen up"},
    {"pen down 0 297"},
    {"pen move 20 297"},
    {"pen up"},
};

// Font: 9
static const LampCommand font_9_commands[] = {
    {"pen down 204 158"},
    {"pen move 104 188"},
    {"pen move 23 118"},
    {"pen move 154 20"},
    {"pen move 234 59"},
    {"pen move 210 158"},
    {"pen move 174 308"},
    {"pen up"},
};

// Font: A
static const LampCommand font_A_commands[] = {
    {"pen down 135 19"},
    {"pen move 169 137"},
    {"pen move 56 233"},
    {"pen move 22 167"},
    {"pen move 135 19"},
    {"pen up"},
    {"pen down 185 157"},
    {"pen move 305 207"},
    {"pen up"},
};

// Font: B
static const LampCommand font_B_commands[] = {
    {"pen down 110 6"},
    {"pen move 50 266"},
    {"pen move 120 207"},
    {"pen move 221 172"},
    {"pen move 130 360"},
    {"pen move 40 276"},
    {"pen up"},
    {"pen down 0 356"},
    {"pen move 10 356"},
    {"pen up"},
};

// Font: C
static const LampCommand font_C_commands[] = {
    {"pen down 250 170"},
    {"pen move 43 234"},
    {"pen move 25 120"},
    {"pen move 150 60"},
    {"pen up"},
};

// Font: D
static const LampCommand font_D_commands[] = {
    {"pen down 279 5"},
    {"pen move 251 155"},
    {"pen move 234 215"},
    {"pen move 185 285"},
    {"pen move 25 371"},
    {"pen move 158 145"},
    {"pen move 219 225"},
    {"pen up"},
    {"pen down 229 225"},
    {"pen move 249 285"},
    {"pen move 283 375"},
    {"pen move 399 325"},
    {"pen up"},
};

// Font: E
static const LampCommand font_E_commands[] = {
    {"pen down 19 151"},
    {"pen move 63 45"},
    {"pen move 137 34"},
    {"pen move 24 192"},
    {"pen move 249 161"},
    {"pen up"},
};

// Font: F
static const LampCommand font_F_commands[] = {
    {"pen down 9 530"},
    {"pen move 99 300"},
    {"pen move 110 220"},
    {"pen move 125 110"},
    {"pen move 239 70"},
    {"pen up"},
    {"pen down 119 220"},
    {"pen move 229 220"},
    {"pen up"},
};

// Font: G
static const LampCommand font_G_commands[] = {
    {"pen down 251 158"},
    {"pen move 191 183"},
    {"pen move 73 204"},
    {"pen move 245 28"},
    {"pen move 261 98"},
    {"pen move 254 168"},
    {"pen move 250 238"},
    {"pen move 161 379"},
    {"pen move 11 328"},
    {"pen up"},
};

// Font: H
static const LampCommand font_H_commands[] = {
    {"pen down 83 3"},
    {"pen move 23 323"},
    {"pen up"},
    {"pen down 23 373"},
    {"pen move 130 273"},
    {"pen move 213 193"},
    {"pen move 333 325"},
    {"pen move 393 253"},
    {"pen up"},
    {"pen down 13 363"},
    {"pen move 23 373"},
    {"pen up"},
};

// Font: I
static const LampCommand font_I_commands[] = {
    {"pen down 81 0"},
    {"pen move 81 10"},
    {"pen up"},
    {"pen down 41 150"},
    {"pen move 191 290"},
    {"pen up"},
};

// Font: J
static const LampCommand font_J_commands[] = {
    {"pen down 161 6"},
    {"pen move 141 196"},
    {"pen move 136 286"},
    {"pen move 1 356"},
    {"pen up"},
};

// Font: K
static const LampCommand font_K_commands[] = {
    {"pen down 59 1"},
    {"pen move 31 201"},
    {"pen move 19 351"},
    {"pen up"},
    {"pen down 219 101"},
    {"pen move 49 281"},
    {"pen up"},
    {"pen down 89 251"},
    {"pen move 229 335"},
    {"pen move 339 351"},
    {"pen up"},
};

// Font: L
static const LampCommand font_L_commands[] = {
    {"pen down 59 4"},
    {"pen move 19 274"},
    {"pen move 33 363"},
    {"pen move 209 314"},
    {"pen up"},
};

// Font: M
static const LampCommand font_M_commands[] = {
    {"pen down 35 17"},
    {"pen move 19 117"},
    {"pen move 25 187"},
    {"pen move 175 27"},
    {"pen move 225 177"},
    {"pen move 355 27"},
    {"pen move 387 97"},
    {"pen move 508 155"},
    {"pen move 535 117"},
    {"pen up"},
};

// Font: N
static const LampCommand font_N_commands[] = {
    {"pen down 48 6"},
    {"pen move 29 96"},
    {"pen move 18 196"},
    {"pen up"},
    {"pen down 48 166"},
    {"pen move 106 106"},
    {"pen move 165 66"},
    {"pen move 188 106"},
    {"pen move 212 183"},
    {"pen move 348 176"},
    {"pen up"},
};

// Font: O
static const LampCommand font_O_commands[] = {
    {"pen down 114 23"},
    {"pen move 214 105"},
    {"pen move 84 228"},
    {"pen move 114 23"},
    {"pen up"},
};

// Font: P
static const LampCommand font_P_commands[] = {
    {"pen down 78 13"},
    {"pen move 68 103"},
    {"pen move 70 173"},
    {"pen move 128 237"},
    {"pen move 208 192"},
    {"pen move 275 44"},
    {"pen move 179 44"},
    {"pen move 88 153"},
    {"pen up"},
    {"pen down 58 223"},
    {"pen move 18 383"},
    {"pen up"},
};

// Font: Q
static const LampCommand font_Q_commands[] = {
    {"pen down 172 170"},
    {"pen move 121 211"},
    {"pen move 32 233"},
    {"pen move 198 28"},
    {"pen move 196 160"},
    {"pen move 162 290"},
    {"pen move 136 388"},
    {"pen move 132 420"},
    {"pen up"},
};

// Font: R
static const LampCommand font_R_commands[] = {
    {"pen down 23 159"},
    {"pen move 90 119"},
    {"pen move 223 19"},
    {"pen up"},
    {"pen down 53 19"},
    {"pen move 23 119"},
    {"pen up"},
    {"pen down 13 149"},
    {"pen move 23 159"},
    {"pen up"},
    {"pen down 43 179"},
    {"pen move 43 219"},
    {"pen up"},
};

// Font: S
static const LampCommand font_S_commands[] = {
    {"pen down 62 13"},
    {"pen move 88 213"},
    {"pen move 22 313"},
    {"pen up"},
};

// Font: T
static const LampCommand font_T_commands[] = {
    {"pen down 120 9"},
    {"pen move 230 109"},
    {"pen up"},
    {"pen down 90 109"},
    {"pen move 0 129"},
    {"pen up"},
    {"pen down 60 139"},
    {"pen move 53 209"},
    {"pen move 130 329"},
    {"pen move 190 279"},
    {"pen up"},
};

// Font: U
static const LampCommand font_U_commands[] = {
    {"pen down 56 6"},
    {"pen move 19 136"},
    {"pen move 66 202"},
    {"pen move 144 146"},
    {"pen move 226 6"},
    {"pen up"},
    {"pen down 206 96"},
    {"pen move 336 156"},
    {"pen up"},
};

// Font: V
static const LampCommand font_V_commands[] = {
    {"pen down 251 14"},
    {"pen move 71 224"},
    {"pen move 21 24"},
    {"pen up"},
};

// Font: W
static const LampCommand font_W_commands[] = {
    {"pen down 395 8"},
    {"pen move 328 148"},
    {"pen move 285 206"},
    {"pen move 231 138"},
    {"pen move 205 78"},
    {"pen move 85 238"},
    {"pen move 15 58"},
    {"pen up"},
    {"pen down 5 58"},
    {"pen move 15 58"},
    {"pen up"},
};

// Font: X
static const LampCommand font_X_commands[] = {
    {"pen down 43 10"},
    {"pen move 107 140"},
    {"pen move 333 150"},
    {"pen up"},
    {"pen down 213 10"},
    {"pen move 113 140"},
    {"pen up"},
    {"pen down 103 140"},
    {"pen move 13 230"},
    {"pen up"},
};

// Font: Y
static const LampCommand font_Y_commands[] = {
    {"pen down 260 4"},
    {"pen move 217 144"},
    {"pen move 190 234"},
    {"pen move 140 394"},
    {"pen up"},
    {"pen down 70 14"},
    {"pen move 32 104"},
    {"pen move 45 188"},
    {"pen move 210 154"},
    {"pen up"},
    {"pen down 0 164"},
    {"pen move 20 164"},
    {"pen up"},
};

// Font: Z
static const LampCommand font_Z_commands[] = {
    {"pen down 37 25"},
    {"pen move 207 35"},
    {"pen move 17 265"},
    {"pen up"},
    {"pen down 217 15"},
    {"pen move 207 25"},
    {"pen up"},
    {"pen down 327 185"},
    {"pen move 97 215"},
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
    {'0', font_0_commands, 7},
    {'1', font_1_commands, 8},
    {'2', font_2_commands, 10},
    {'3', font_3_commands, 12},
    {'4', font_4_commands, 12},
    {'5', font_5_commands, 7},
    {'6', font_6_commands, 6},
    {'7', font_7_commands, 6},
    {'8', font_8_commands, 11},
    {'9', font_9_commands, 8},
    {'A', font_A_commands, 9},
    {'B', font_B_commands, 10},
    {'C', font_C_commands, 5},
    {'D', font_D_commands, 13},
    {'E', font_E_commands, 6},
    {'F', font_F_commands, 9},
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

static const int FONT_GLYPH_COUNT = 36;

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
