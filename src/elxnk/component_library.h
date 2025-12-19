// Auto-generated component and font library
// Generated from SVG assets using svg_to_lamp_final.py
// Build: make library (from src/ directory)
// DO NOT EDIT MANUALLY - regenerate with svg2header.py
//
// COORDINATES: Stored in PIXELS at 10 pixels/mm scale
// - reMarkable 2: 1404x1872 pixels
// - Components scaled to actual pixel sizes
// - render_component() applies POSITION OFFSET only
//
// LAMP GEOMETRY COMMANDS (from rmkit):
//   pen down X Y              - Start drawing at (X,Y)
//   pen move X Y              - Draw line to (X,Y)
//   pen up                    - Lift pen
//   pen line X1 Y1 X2 Y2      - Draw line
//   pen circle CX CY R1 R2    - Draw circle/ellipse
//   pen rectangle X1 Y1 X2 Y2 - Draw rectangle
//   pen arc CX CY R1 R2 A1 A2 - Draw arc

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
    {"pen move 62 59"},
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
    {"pen move 92 48"},
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
    {"pen up"},
    {"pen down 58 58"},
    {"pen up"},
    {"pen down 64 69"},
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
    {"pen move 128 179"},
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
    {"pen move 74 59"},
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
    {"pen move 92 55"},
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
    {"pen up"},
    {"pen down 58 58"},
    {"pen up"},
    {"pen down 74 69"},
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
    {"pen up"},
    {"pen down 11 31"},
    {"pen up"},
    {"pen down 11 46"},
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
    {"pen up"},
    {"pen down 11 31"},
    {"pen up"},
    {"pen down 11 46"},
    {"pen up"},
    {"pen down 11 62"},
    {"pen move 11 64"},
    {"pen up"},
    {"pen down 22 39"},
    {"pen move 22 49"},
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
    {"pen up"},
    {"pen down 21 31"},
    {"pen up"},
    {"pen down 21 46"},
    {"pen up"},
    {"pen down 21 62"},
    {"pen move 21 64"},
    {"pen up"},
    {"pen down 44 24"},
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
    {"pen move 267 26"},
    {"pen move 303 46"},
    {"pen move 330 75"},
    {"pen move 347 111"},
    {"pen move 351 151"},
    {"pen move 340 191"},
    {"pen move 319 223"},
    {"pen move 293 251"},
    {"pen move 264 276"},
    {"pen move 212 310"},
    {"pen move 184 324"},
    {"pen move 124 340"},
    {"pen move 94 341"},
    {"pen move 61 332"},
    {"pen move 38 313"},
    {"pen move 25 287"},
    {"pen move 19 256"},
    {"pen move 21 223"},
    {"pen move 27 191"},
    {"pen move 44 151"},
    {"pen move 70 111"},
    {"pen move 103 75"},
    {"pen move 141 46"},
    {"pen move 182 26"},
    {"pen move 224 19"},
    {"pen up"},
};

// Font: 1
static const LampCommand font_1_commands[] = {
    {"pen down 157 294"},
    {"pen move 287 274"},
    {"pen up"},
};

// Font: 2
static const LampCommand font_2_commands[] = {
    {"pen down 118 295"},
    {"pen move 147 308"},
    {"pen move 181 318"},
    {"pen move 220 325"},
    {"pen move 261 328"},
    {"pen move 304 325"},
    {"pen move 348 315"},
    {"pen up"},
};

// Font: 3
static const LampCommand font_3_commands[] = {
    {"pen down 55 29"},
    {"pen move 104 32"},
    {"pen move 131 37"},
    {"pen move 156 48"},
    {"pen move 175 64"},
    {"pen move 185 89"},
    {"pen move 215 193"},
    {"pen move 202 263"},
    {"pen move 160 303"},
    {"pen move 104 322"},
    {"pen move 47 325"},
    {"pen move 5 319"},
    {"pen up"},
};

// Font: 4
static const LampCommand font_4_commands[] = {
    {"pen down 34 197"},
    {"pen move 62 207"},
    {"pen move 90 211"},
    {"pen move 204 207"},
    {"pen move 205 280"},
    {"pen move 194 337"},
    {"pen up"},
};

// Font: 5
static const LampCommand font_5_commands[] = {
    {"pen down 10 309"},
    {"pen move 51 324"},
    {"pen move 104 331"},
    {"pen move 160 326"},
    {"pen move 209 309"},
    {"pen move 243 277"},
    {"pen move 253 229"},
    {"pen move 247 208"},
    {"pen move 235 192"},
    {"pen move 218 180"},
    {"pen move 199 172"},
    {"pen move 160 162"},
    {"pen move 115 156"},
    {"pen move 90 146"},
    {"pen move 81 134"},
    {"pen move 75 105"},
    {"pen move 86 80"},
    {"pen move 109 59"},
    {"pen move 137 44"},
    {"pen move 166 33"},
    {"pen move 190 29"},
    {"pen move 293 25"},
    {"pen move 340 19"},
    {"pen up"},
};

// Font: 6
static const LampCommand font_6_commands[] = {
    {"pen down 95 318"},
    {"pen move 105 279"},
    {"pen move 130 234"},
    {"pen move 165 193"},
    {"pen move 206 169"},
    {"pen move 248 170"},
    {"pen move 287 210"},
    {"pen move 293 229"},
    {"pen move 290 247"},
    {"pen move 266 278"},
    {"pen move 235 301"},
    {"pen move 204 317"},
    {"pen move 170 329"},
    {"pen move 134 334"},
    {"pen move 99 332"},
    {"pen move 67 319"},
    {"pen move 41 296"},
    {"pen move 19 239"},
    {"pen move 31 180"},
    {"pen move 66 123"},
    {"pen move 113 74"},
    {"pen move 163 37"},
    {"pen move 205 18"},
    {"pen up"},
};

// Font: 7
static const LampCommand font_7_commands[] = {
    {"pen down 0 22"},
    {"pen move 240 22"},
    {"pen up"},
};

// Font: 8
static const LampCommand font_8_commands[] = {
    {"pen down 0 297"},
    {"pen move 20 297"},
    {"pen up"},
};

// Font: 9
static const LampCommand font_9_commands[] = {
    {"pen down 204 158"},
    {"pen move 104 188"},
    {"pen move 80 191"},
    {"pen move 57 188"},
    {"pen move 38 180"},
    {"pen move 24 165"},
    {"pen move 18 144"},
    {"pen move 23 118"},
    {"pen move 36 93"},
    {"pen move 54 70"},
    {"pen move 77 51"},
    {"pen move 102 36"},
    {"pen move 128 25"},
    {"pen move 154 20"},
    {"pen move 187 20"},
    {"pen move 216 32"},
    {"pen move 227 43"},
    {"pen move 234 59"},
    {"pen move 232 92"},
    {"pen move 210 158"},
    {"pen move 174 308"},
    {"pen up"},
};

// Font: A
static const LampCommand font_A_commands[] = {
    {"pen down 185 157"},
    {"pen move 191 183"},
    {"pen move 203 205"},
    {"pen move 221 219"},
    {"pen move 244 226"},
    {"pen move 272 223"},
    {"pen move 305 207"},
    {"pen up"},
};

// Font: B
static const LampCommand font_B_commands[] = {
    {"pen down 0 356"},
    {"pen move 10 356"},
    {"pen up"},
};

// Font: C
static const LampCommand font_C_commands[] = {
    {"pen down 250 170"},
    {"pen move 189 211"},
    {"pen move 151 232"},
    {"pen move 112 246"},
    {"pen move 75 248"},
    {"pen move 43 234"},
    {"pen move 30 219"},
    {"pen move 23 201"},
    {"pen move 19 181"},
    {"pen move 19 160"},
    {"pen move 25 120"},
    {"pen move 38 86"},
    {"pen move 57 54"},
    {"pen move 81 30"},
    {"pen move 106 19"},
    {"pen move 130 27"},
    {"pen move 150 60"},
    {"pen up"},
};

// Font: D
static const LampCommand font_D_commands[] = {
    {"pen down 229 225"},
    {"pen move 244 259"},
    {"pen move 256 333"},
    {"pen move 270 364"},
    {"pen move 283 375"},
    {"pen move 304 383"},
    {"pen move 325 380"},
    {"pen move 346 370"},
    {"pen move 365 356"},
    {"pen move 399 325"},
    {"pen up"},
};

// Font: E
static const LampCommand font_E_commands[] = {
    {"pen down 19 151"},
    {"pen move 28 98"},
    {"pen move 46 64"},
    {"pen move 73 35"},
    {"pen move 100 21"},
    {"pen move 114 19"},
    {"pen move 127 23"},
    {"pen move 137 34"},
    {"pen move 142 66"},
    {"pen move 125 94"},
    {"pen move 36 168"},
    {"pen move 24 192"},
    {"pen move 35 244"},
    {"pen move 71 258"},
    {"pen move 121 246"},
    {"pen move 174 218"},
    {"pen move 221 187"},
    {"pen move 249 161"},
    {"pen up"},
};

// Font: F
static const LampCommand font_F_commands[] = {
    {"pen down 119 220"},
    {"pen move 229 220"},
    {"pen up"},
};

// Font: G
static const LampCommand font_G_commands[] = {
    {"pen down 251 158"},
    {"pen move 204 176"},
    {"pen move 130 215"},
    {"pen move 108 220"},
    {"pen move 88 218"},
    {"pen move 73 204"},
    {"pen move 70 168"},
    {"pen move 88 122"},
    {"pen move 122 76"},
    {"pen move 163 38"},
    {"pen move 207 19"},
    {"pen move 245 28"},
    {"pen move 254 37"},
    {"pen move 259 48"},
    {"pen move 262 72"},
    {"pen move 261 110"},
    {"pen move 254 168"},
    {"pen move 250 238"},
    {"pen move 237 293"},
    {"pen move 225 320"},
    {"pen move 209 344"},
    {"pen move 188 365"},
    {"pen move 161 379"},
    {"pen move 134 385"},
    {"pen move 107 382"},
    {"pen move 82 374"},
    {"pen move 57 361"},
    {"pen move 11 328"},
    {"pen up"},
};

// Font: H
static const LampCommand font_H_commands[] = {
    {"pen down 73 43"},
    {"pen up"},
};

// Font: I
static const LampCommand font_I_commands[] = {
    {"pen down 41 150"},
    {"pen move 20 224"},
    {"pen move 24 285"},
    {"pen move 47 328"},
    {"pen move 85 346"},
    {"pen move 135 335"},
    {"pen move 191 290"},
    {"pen up"},
};

// Font: J
static const LampCommand font_J_commands[] = {
    {"pen down 161 6"},
    {"pen move 147 67"},
    {"pen move 144 97"},
    {"pen move 141 256"},
    {"pen move 136 286"},
    {"pen move 122 322"},
    {"pen move 103 344"},
    {"pen move 80 355"},
    {"pen move 54 359"},
    {"pen move 1 356"},
    {"pen up"},
};

// Font: K
static const LampCommand font_K_commands[] = {
    {"pen down 259 71"},
    {"pen up"},
};

// Font: L
static const LampCommand font_L_commands[] = {
    {"pen down 59 4"},
    {"pen move 32 138"},
    {"pen move 20 226"},
    {"pen move 20 320"},
    {"pen move 33 363"},
    {"pen move 61 390"},
    {"pen move 93 394"},
    {"pen move 127 381"},
    {"pen move 160 359"},
    {"pen move 209 314"},
    {"pen up"},
};

// Font: M
static const LampCommand font_M_commands[] = {
    {"pen down 35 17"},
    {"pen move 25 187"},
    {"pen move 47 164"},
    {"pen move 109 82"},
    {"pen move 137 54"},
    {"pen move 175 27"},
    {"pen move 189 57"},
    {"pen move 198 84"},
    {"pen move 207 129"},
    {"pen move 225 177"},
    {"pen move 248 155"},
    {"pen move 300 80"},
    {"pen move 323 54"},
    {"pen move 355 27"},
    {"pen move 378 70"},
    {"pen move 398 131"},
    {"pen move 413 159"},
    {"pen move 431 178"},
    {"pen move 453 185"},
    {"pen move 479 179"},
    {"pen move 513 150"},
    {"pen move 535 117"},
    {"pen up"},
};

// Font: N
static const LampCommand font_N_commands[] = {
    {"pen down 48 166"},
    {"pen move 134 78"},
    {"pen move 143 71"},
    {"pen move 165 66"},
    {"pen move 174 69"},
    {"pen move 184 82"},
    {"pen move 197 159"},
    {"pen move 212 183"},
    {"pen move 235 199"},
    {"pen move 259 204"},
    {"pen move 284 202"},
    {"pen move 308 195"},
    {"pen move 348 176"},
    {"pen up"},
};

// Font: O
static const LampCommand font_O_commands[] = {
    {"pen down 114 23"},
    {"pen move 138 19"},
    {"pen move 164 24"},
    {"pen move 186 36"},
    {"pen move 204 55"},
    {"pen move 214 78"},
    {"pen move 214 105"},
    {"pen move 204 133"},
    {"pen move 187 159"},
    {"pen move 138 203"},
    {"pen move 84 228"},
    {"pen move 38 225"},
    {"pen move 20 194"},
    {"pen move 24 147"},
    {"pen move 45 95"},
    {"pen move 76 50"},
    {"pen move 114 23"},
    {"pen up"},
};

// Font: P
static const LampCommand font_P_commands[] = {
    {"pen down 58 223"},
    {"pen move 18 383"},
    {"pen up"},
};

// Font: Q
static const LampCommand font_Q_commands[] = {
    {"pen down 172 170"},
    {"pen move 109 220"},
    {"pen move 78 238"},
    {"pen move 61 243"},
    {"pen move 45 242"},
    {"pen move 32 233"},
    {"pen move 19 191"},
    {"pen move 33 138"},
    {"pen move 67 85"},
    {"pen move 111 43"},
    {"pen move 158 20"},
    {"pen move 198 28"},
    {"pen move 215 47"},
    {"pen move 220 69"},
    {"pen move 218 93"},
    {"pen move 196 160"},
    {"pen move 147 339"},
    {"pen move 132 420"},
    {"pen up"},
};

// Font: R
static const LampCommand font_R_commands[] = {
    {"pen down 63 69"},
    {"pen move 63 109"},
    {"pen up"},
};

// Font: S
static const LampCommand font_S_commands[] = {
    {"pen down 62 13"},
    {"pen move 26 59"},
    {"pen move 19 91"},
    {"pen move 32 116"},
    {"pen move 76 170"},
    {"pen move 88 213"},
    {"pen move 87 238"},
    {"pen move 78 259"},
    {"pen move 65 275"},
    {"pen move 22 313"},
    {"pen up"},
};

// Font: T
static const LampCommand font_T_commands[] = {
    {"pen down 60 139"},
    {"pen move 46 277"},
    {"pen move 49 312"},
    {"pen move 61 338"},
    {"pen move 87 346"},
    {"pen move 130 329"},
    {"pen move 165 304"},
    {"pen move 190 279"},
    {"pen up"},
};

// Font: U
static const LampCommand font_U_commands[] = {
    {"pen down 206 96"},
    {"pen move 215 124"},
    {"pen move 229 149"},
    {"pen move 247 168"},
    {"pen move 271 177"},
    {"pen move 301 175"},
    {"pen move 336 156"},
    {"pen up"},
};

// Font: V
static const LampCommand font_V_commands[] = {
    {"pen down 251 14"},
    {"pen move 213 53"},
    {"pen move 142 165"},
    {"pen move 112 196"},
    {"pen move 71 224"},
    {"pen move 40 158"},
    {"pen move 29 127"},
    {"pen move 22 96"},
    {"pen move 19 62"},
    {"pen move 21 24"},
    {"pen up"},
};

// Font: W
static const LampCommand font_W_commands[] = {
    {"pen down 195 -12"},
    {"pen move 205 -12"},
    {"pen up"},
};

// Font: X
static const LampCommand font_X_commands[] = {
    {"pen down 103 140"},
    {"pen move 13 230"},
    {"pen up"},
};

// Font: Y
static const LampCommand font_Y_commands[] = {
    {"pen down 0 164"},
    {"pen move 20 164"},
    {"pen up"},
};

// Font: Z
static const LampCommand font_Z_commands[] = {
    {"pen down 337 175"},
    {"pen move 302 194"},
    {"pen move 267 205"},
    {"pen move 231 211"},
    {"pen move 192 212"},
    {"pen move 107 205"},
    {"pen up"},
};

// Component registry
static const Component COMPONENTS[] = {
    {"D", D_commands, 17},
    {"GND", GND_commands, 15},
    {"L", L_commands, 14},
    {"NMOS", NMOS_commands, 59},
    {"NPN", NPN_commands, 31},
    {"NP_C", NP_C_commands, 12},
    {"OPAMP", OPAMP_commands, 28},
    {"PMOS", PMOS_commands, 59},
    {"PNP", PNP_commands, 31},
    {"P_C", P_C_commands, 17},
    {"R", R_commands, 18},
    {"R_TRIM", R_TRIM_commands, 28},
    {"TX", TX_commands, 50},
    {"VAC", VAC_commands, 20},
    {"VAR", VAR_commands, 20},
    {"VDC", VDC_commands, 28},
    {"ZD", ZD_commands, 23},
};

// Font glyph registry
static const FontGlyph FONT_GLYPHS[] = {
    {'0', font_0_commands, 27},
    {'1', font_1_commands, 3},
    {'2', font_2_commands, 8},
    {'3', font_3_commands, 13},
    {'4', font_4_commands, 7},
    {'5', font_5_commands, 24},
    {'6', font_6_commands, 24},
    {'7', font_7_commands, 3},
    {'8', font_8_commands, 3},
    {'9', font_9_commands, 22},
    {'A', font_A_commands, 8},
    {'B', font_B_commands, 3},
    {'C', font_C_commands, 18},
    {'D', font_D_commands, 11},
    {'E', font_E_commands, 19},
    {'F', font_F_commands, 3},
    {'G', font_G_commands, 29},
    {'H', font_H_commands, 2},
    {'I', font_I_commands, 8},
    {'J', font_J_commands, 11},
    {'K', font_K_commands, 2},
    {'L', font_L_commands, 11},
    {'M', font_M_commands, 23},
    {'N', font_N_commands, 14},
    {'O', font_O_commands, 18},
    {'P', font_P_commands, 3},
    {'Q', font_Q_commands, 19},
    {'R', font_R_commands, 3},
    {'S', font_S_commands, 11},
    {'T', font_T_commands, 9},
    {'U', font_U_commands, 8},
    {'V', font_V_commands, 11},
    {'W', font_W_commands, 3},
    {'X', font_X_commands, 3},
    {'Y', font_Y_commands, 3},
    {'Z', font_Z_commands, 7},
};

// Lookup functions
const Component* find_component(const std::string& name) {
    for (const auto& comp : COMPONENTS) {
        if (comp.name == name) {
            return &comp;
        }
    }
    return nullptr;
}

const FontGlyph* find_glyph(char c) {
    for (const auto& glyph : FONT_GLYPHS) {
        if (glyph.character == c) {
            return &glyph;
        }
    }
    return nullptr;
}

// Component count
inline int get_component_count() {
    return sizeof(COMPONENTS) / sizeof(Component);
}

// Font glyph count
inline int get_glyph_count() {
    return sizeof(FONT_GLYPHS) / sizeof(FontGlyph);
}

}  // namespace elxnk

#endif  // ELXNK_LIBRARY_H
