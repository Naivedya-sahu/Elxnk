// Auto-generated component and font library
// Generated from SVG assets - DO NOT EDIT MANUALLY
// Build: make generate-library

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
    {"pen up"},
    {"pen move 2 0"},
    {"pen down"},
    {"pen move 2 2"},
    {"pen up"},
    {"pen up"},
    {"pen move 5 0"},
    {"pen down"},
    {"pen move 5 2"},
    {"pen move 2 1"},
    {"pen up"},
    {"pen up"},
    {"pen move 5 1"},
    {"pen down"},
    {"pen move 2 1"},
    {"pen up"},
    {"pen up"},
    {"pen move 2 1"},
    {"pen down"},
    {"pen move 0 1"},
    {"pen up"},
    {"pen up"},
    {"pen move 5 1"},
    {"pen down"},
    {"pen move 7 1"},
    {"pen up"},
};

// Component: GND
static const LampCommand GND_commands[] = {
    {"pen up"},
    {"pen move 135 118"},
    {"pen down"},
    {"pen move 136 118"},
    {"pen up"},
    {"pen up"},
    {"pen move 135 119"},
    {"pen down"},
    {"pen move 136 119"},
    {"pen up"},
    {"pen up"},
    {"pen move 135 118"},
    {"pen down"},
    {"pen move 135 116"},
    {"pen up"},
    {"pen up"},
    {"pen move 137 118"},
    {"pen down"},
    {"pen move 134 118"},
    {"pen up"},
    {"pen up"},
    {"pen move 135 116"},
    {"pen down"},
    {"pen move 135 116"},
    {"pen up"},
};

// Component: L
static const LampCommand L_commands[] = {
    {"pen up"},
    {"pen move 149 107"},
    {"pen up"},
    {"pen move 149 109"},
    {"pen up"},
    {"pen move 149 110"},
    {"pen up"},
    {"pen move 149 111"},
    {"pen up"},
    {"pen move 149 106"},
    {"pen down"},
    {"pen move 149 105"},
    {"pen up"},
    {"pen up"},
    {"pen move 149 111"},
    {"pen down"},
    {"pen move 149 113"},
    {"pen up"},
};

// Component: NMOS
static const LampCommand NMOS_commands[] = {
    {"pen up"},
    {"pen move 169 131"},
    {"pen down"},
    {"pen move 169 135"},
    {"pen up"},
    {"pen up"},
    {"pen move 169 133"},
    {"pen down"},
    {"pen move 166 133"},
    {"pen up"},
    {"pen up"},
    {"pen move 169 131"},
    {"pen down"},
    {"pen move 169 132"},
    {"pen up"},
    {"pen up"},
    {"pen move 169 132"},
    {"pen down"},
    {"pen move 169 133"},
    {"pen up"},
    {"pen up"},
    {"pen move 169 134"},
    {"pen down"},
    {"pen move 169 135"},
    {"pen up"},
    {"pen up"},
    {"pen move 169 135"},
    {"pen down"},
    {"pen move 172 135"},
    {"pen move 172 131"},
    {"pen move 169 131"},
    {"pen up"},
    {"pen up"},
    {"pen move 169 133"},
    {"pen down"},
    {"pen move 169 134"},
    {"pen up"},
    {"pen up"},
    {"pen move 171 130"},
    {"pen down"},
    {"pen move 171 131"},
    {"pen up"},
    {"pen up"},
    {"pen move 171 135"},
    {"pen down"},
    {"pen move 171 133"},
    {"pen move 169 133"},
    {"pen up"},
    {"pen up"},
    {"pen move 171 132"},
    {"pen down"},
    {"pen move 172 132"},
    {"pen up"},
    {"pen up"},
    {"pen move 172 132"},
    {"pen down"},
    {"pen move 172 132"},
    {"pen up"},
    {"pen up"},
    {"pen move 166 133"},
    {"pen down"},
    {"pen move 163 133"},
    {"pen up"},
    {"pen up"},
    {"pen move 171 130"},
    {"pen down"},
    {"pen move 171 128"},
    {"pen up"},
    {"pen up"},
    {"pen move 171 135"},
    {"pen down"},
    {"pen move 171 138"},
    {"pen up"},
    {"pen up"},
    {"pen move 166 133"},
    {"pen down"},
    {"pen move 163 133"},
    {"pen up"},
    {"pen up"},
    {"pen move 171 130"},
    {"pen down"},
    {"pen move 171 128"},
    {"pen up"},
    {"pen up"},
    {"pen move 171 135"},
    {"pen down"},
    {"pen move 171 138"},
    {"pen up"},
};

// Component: NPN
static const LampCommand NPN_commands[] = {
    {"pen up"},
    {"pen move 149 133"},
    {"pen down"},
    {"pen move 153 133"},
    {"pen up"},
    {"pen up"},
    {"pen move 153 131"},
    {"pen down"},
    {"pen move 153 135"},
    {"pen up"},
    {"pen up"},
    {"pen move 153 132"},
    {"pen up"},
    {"pen move 153 133"},
    {"pen up"},
    {"pen move 153 135"},
    {"pen up"},
    {"pen move 149 133"},
    {"pen down"},
    {"pen move 147 133"},
    {"pen up"},
    {"pen up"},
    {"pen move 154 130"},
    {"pen down"},
    {"pen move 154 128"},
    {"pen up"},
    {"pen up"},
    {"pen move 154 135"},
    {"pen down"},
    {"pen move 154 138"},
    {"pen up"},
    {"pen up"},
    {"pen move 149 133"},
    {"pen down"},
    {"pen move 147 133"},
    {"pen up"},
    {"pen up"},
    {"pen move 154 130"},
    {"pen down"},
    {"pen move 154 128"},
    {"pen up"},
    {"pen up"},
    {"pen move 154 135"},
    {"pen down"},
    {"pen move 154 138"},
    {"pen up"},
};

// Component: NP_C
static const LampCommand NP_C_commands[] = {
    {"pen up"},
    {"pen move 133 108"},
    {"pen down"},
    {"pen move 137 108"},
    {"pen up"},
    {"pen up"},
    {"pen move 133 109"},
    {"pen down"},
    {"pen move 137 109"},
    {"pen up"},
    {"pen up"},
    {"pen move 135 108"},
    {"pen down"},
    {"pen move 135 105"},
    {"pen up"},
    {"pen up"},
    {"pen move 135 110"},
    {"pen down"},
    {"pen move 135 113"},
    {"pen up"},
};

// Component: OPAMP
static const LampCommand OPAMP_commands[] = {
    {"pen up"},
    {"pen move 143 130"},
    {"pen down"},
    {"pen move 143 140"},
    {"pen up"},
    {"pen up"},
    {"pen move 133 128"},
    {"pen down"},
    {"pen move 130 128"},
    {"pen up"},
    {"pen up"},
    {"pen move 134 128"},
    {"pen down"},
    {"pen move 134 128"},
    {"pen up"},
    {"pen up"},
    {"pen move 134 128"},
    {"pen down"},
    {"pen move 134 127"},
    {"pen up"},
    {"pen up"},
    {"pen move 133 133"},
    {"pen down"},
    {"pen move 130 133"},
    {"pen up"},
    {"pen up"},
    {"pen move 134 133"},
    {"pen down"},
    {"pen move 134 133"},
    {"pen up"},
    {"pen up"},
    {"pen move 135 127"},
    {"pen down"},
    {"pen move 135 123"},
    {"pen up"},
    {"pen up"},
    {"pen move 135 134"},
    {"pen down"},
    {"pen move 135 138"},
    {"pen up"},
    {"pen up"},
    {"pen move 143 130"},
    {"pen down"},
    {"pen move 146 130"},
    {"pen up"},
};

// Component: PMOS
static const LampCommand PMOS_commands[] = {
    {"pen up"},
    {"pen move 169 120"},
    {"pen down"},
    {"pen move 169 123"},
    {"pen up"},
    {"pen up"},
    {"pen move 169 121"},
    {"pen down"},
    {"pen move 166 121"},
    {"pen up"},
    {"pen up"},
    {"pen move 169 119"},
    {"pen down"},
    {"pen move 169 120"},
    {"pen up"},
    {"pen up"},
    {"pen move 169 120"},
    {"pen down"},
    {"pen move 172 120"},
    {"pen move 172 123"},
    {"pen move 169 123"},
    {"pen up"},
    {"pen up"},
    {"pen move 169 121"},
    {"pen down"},
    {"pen move 169 122"},
    {"pen up"},
    {"pen up"},
    {"pen move 169 123"},
    {"pen down"},
    {"pen move 169 124"},
    {"pen up"},
    {"pen up"},
    {"pen move 171 121"},
    {"pen down"},
    {"pen move 171 122"},
    {"pen up"},
    {"pen up"},
    {"pen move 171 119"},
    {"pen down"},
    {"pen move 171 120"},
    {"pen up"},
    {"pen up"},
    {"pen move 171 124"},
    {"pen down"},
    {"pen move 171 121"},
    {"pen move 169 121"},
    {"pen up"},
    {"pen up"},
    {"pen move 171 122"},
    {"pen down"},
    {"pen move 172 122"},
    {"pen up"},
    {"pen up"},
    {"pen move 172 122"},
    {"pen down"},
    {"pen move 172 122"},
    {"pen up"},
    {"pen up"},
    {"pen move 166 121"},
    {"pen down"},
    {"pen move 163 121"},
    {"pen up"},
    {"pen up"},
    {"pen move 171 119"},
    {"pen down"},
    {"pen move 171 116"},
    {"pen up"},
    {"pen up"},
    {"pen move 171 124"},
    {"pen down"},
    {"pen move 171 127"},
    {"pen up"},
    {"pen up"},
    {"pen move 166 121"},
    {"pen down"},
    {"pen move 163 121"},
    {"pen up"},
    {"pen up"},
    {"pen move 171 119"},
    {"pen down"},
    {"pen move 171 116"},
    {"pen up"},
    {"pen up"},
    {"pen move 171 124"},
    {"pen down"},
    {"pen move 171 127"},
    {"pen up"},
};

// Component: PNP
static const LampCommand PNP_commands[] = {
    {"pen up"},
    {"pen move 149 121"},
    {"pen down"},
    {"pen move 153 121"},
    {"pen up"},
    {"pen up"},
    {"pen move 153 120"},
    {"pen down"},
    {"pen move 153 123"},
    {"pen up"},
    {"pen up"},
    {"pen move 153 121"},
    {"pen up"},
    {"pen move 153 122"},
    {"pen up"},
    {"pen move 154 123"},
    {"pen up"},
    {"pen move 149 121"},
    {"pen down"},
    {"pen move 147 121"},
    {"pen up"},
    {"pen up"},
    {"pen move 154 119"},
    {"pen down"},
    {"pen move 154 116"},
    {"pen up"},
    {"pen up"},
    {"pen move 154 124"},
    {"pen down"},
    {"pen move 154 127"},
    {"pen up"},
    {"pen up"},
    {"pen move 149 121"},
    {"pen down"},
    {"pen move 147 121"},
    {"pen up"},
    {"pen up"},
    {"pen move 154 119"},
    {"pen down"},
    {"pen move 154 116"},
    {"pen up"},
    {"pen up"},
    {"pen move 154 124"},
    {"pen down"},
    {"pen move 154 127"},
    {"pen up"},
};

// Component: P_C
static const LampCommand P_C_commands[] = {
    {"pen up"},
    {"pen move 141 108"},
    {"pen down"},
    {"pen move 145 108"},
    {"pen up"},
    {"pen up"},
    {"pen move 141 106"},
    {"pen down"},
    {"pen move 142 106"},
    {"pen up"},
    {"pen up"},
    {"pen move 142 107"},
    {"pen down"},
    {"pen move 142 106"},
    {"pen up"},
    {"pen up"},
    {"pen move 145 110"},
    {"pen up"},
    {"pen move 143 108"},
    {"pen down"},
    {"pen move 143 105"},
    {"pen up"},
    {"pen up"},
    {"pen move 143 109"},
    {"pen down"},
    {"pen move 143 113"},
    {"pen up"},
};

// Component: R
static const LampCommand R_commands[] = {
    {"pen up"},
    {"pen move 165 106"},
    {"pen down"},
    {"pen move 165 106"},
    {"pen up"},
    {"pen up"},
    {"pen move 165 106"},
    {"pen up"},
    {"pen move 165 108"},
    {"pen up"},
    {"pen move 165 109"},
    {"pen up"},
    {"pen move 165 111"},
    {"pen down"},
    {"pen move 165 111"},
    {"pen up"},
    {"pen up"},
    {"pen move 165 106"},
    {"pen down"},
    {"pen move 165 105"},
    {"pen up"},
    {"pen up"},
    {"pen move 165 111"},
    {"pen down"},
    {"pen move 165 113"},
    {"pen up"},
};

// Component: R_TRIM
static const LampCommand R_TRIM_commands[] = {
    {"pen up"},
    {"pen move 157 106"},
    {"pen down"},
    {"pen move 157 106"},
    {"pen up"},
    {"pen up"},
    {"pen move 157 106"},
    {"pen up"},
    {"pen move 157 108"},
    {"pen up"},
    {"pen move 157 109"},
    {"pen up"},
    {"pen move 157 111"},
    {"pen down"},
    {"pen move 157 111"},
    {"pen up"},
    {"pen up"},
    {"pen move 158 109"},
    {"pen down"},
    {"pen move 158 110"},
    {"pen up"},
    {"pen up"},
    {"pen move 160 109"},
    {"pen down"},
    {"pen move 159 109"},
    {"pen up"},
    {"pen up"},
    {"pen move 157 106"},
    {"pen down"},
    {"pen move 157 105"},
    {"pen up"},
    {"pen up"},
    {"pen move 157 111"},
    {"pen down"},
    {"pen move 157 113"},
    {"pen up"},
    {"pen up"},
    {"pen move 160 109"},
    {"pen down"},
    {"pen move 161 109"},
    {"pen up"},
};

// Component: TX
static const LampCommand TX_commands[] = {
    {"pen up"},
    {"pen move 199 116"},
    {"pen up"},
    {"pen move 199 119"},
    {"pen up"},
    {"pen move 199 121"},
    {"pen up"},
    {"pen move 199 124"},
    {"pen up"},
    {"pen move 200 115"},
    {"pen up"},
    {"pen move 200 118"},
    {"pen up"},
    {"pen move 200 120"},
    {"pen up"},
    {"pen move 200 123"},
    {"pen up"},
    {"pen move 201 114"},
    {"pen down"},
    {"pen move 201 124"},
    {"pen up"},
    {"pen up"},
    {"pen move 202 124"},
    {"pen down"},
    {"pen move 202 114"},
    {"pen up"},
    {"pen up"},
    {"pen move 204 114"},
    {"pen up"},
    {"pen move 204 116"},
    {"pen up"},
    {"pen move 204 119"},
    {"pen up"},
    {"pen move 203 115"},
    {"pen up"},
    {"pen move 203 118"},
    {"pen up"},
    {"pen move 203 120"},
    {"pen up"},
    {"pen move 204 121"},
    {"pen up"},
    {"pen move 203 123"},
    {"pen up"},
    {"pen move 199 114"},
    {"pen down"},
    {"pen move 191 114"},
    {"pen up"},
    {"pen up"},
    {"pen move 199 124"},
    {"pen down"},
    {"pen move 191 124"},
    {"pen up"},
    {"pen up"},
    {"pen move 204 114"},
    {"pen down"},
    {"pen move 212 114"},
    {"pen up"},
    {"pen up"},
    {"pen move 204 124"},
    {"pen down"},
    {"pen move 212 124"},
    {"pen up"},
};

// Component: VAC
static const LampCommand VAC_commands[] = {
    {"pen up"},
    {"pen move 123 111"},
    {"pen up"},
    {"pen move 123 111"},
    {"pen up"},
    {"pen move 122 109"},
    {"pen down"},
    {"pen move 123 109"},
    {"pen up"},
    {"pen up"},
    {"pen move 123 110"},
    {"pen down"},
    {"pen move 123 109"},
    {"pen up"},
    {"pen up"},
    {"pen move 123 109"},
    {"pen down"},
    {"pen move 123 106"},
    {"pen up"},
    {"pen up"},
    {"pen move 123 114"},
    {"pen down"},
    {"pen move 123 116"},
    {"pen up"},
    {"pen up"},
    {"pen move 122 113"},
    {"pen down"},
    {"pen move 123 113"},
    {"pen up"},
};

// Component: VAR
static const LampCommand VAR_commands[] = {
    {"pen up"},
    {"pen move 172 108"},
    {"pen down"},
    {"pen move 172 107"},
    {"pen up"},
    {"pen up"},
    {"pen move 172 108"},
    {"pen up"},
    {"pen move 172 109"},
    {"pen up"},
    {"pen move 172 111"},
    {"pen up"},
    {"pen move 172 112"},
    {"pen down"},
    {"pen move 172 113"},
    {"pen up"},
    {"pen up"},
    {"pen move 175 108"},
    {"pen up"},
    {"pen move 172 107"},
    {"pen down"},
    {"pen move 172 106"},
    {"pen up"},
    {"pen up"},
    {"pen move 172 113"},
    {"pen down"},
    {"pen move 172 114"},
    {"pen up"},
};

// Component: VDC
static const LampCommand VDC_commands[] = {
    {"pen up"},
    {"pen move 121 125"},
    {"pen down"},
    {"pen move 124 125"},
    {"pen up"},
    {"pen up"},
    {"pen move 122 125"},
    {"pen down"},
    {"pen move 121 125"},
    {"pen up"},
    {"pen up"},
    {"pen move 123 125"},
    {"pen down"},
    {"pen move 122 125"},
    {"pen up"},
    {"pen up"},
    {"pen move 124 125"},
    {"pen down"},
    {"pen move 123 125"},
    {"pen up"},
    {"pen up"},
    {"pen move 122 123"},
    {"pen down"},
    {"pen move 123 123"},
    {"pen up"},
    {"pen up"},
    {"pen move 123 124"},
    {"pen down"},
    {"pen move 123 123"},
    {"pen up"},
    {"pen up"},
    {"pen move 123 123"},
    {"pen down"},
    {"pen move 123 120"},
    {"pen up"},
    {"pen up"},
    {"pen move 123 128"},
    {"pen down"},
    {"pen move 123 130"},
    {"pen up"},
    {"pen up"},
    {"pen move 122 127"},
    {"pen down"},
    {"pen move 123 127"},
    {"pen up"},
};

// Component: ZD
static const LampCommand ZD_commands[] = {
    {"pen up"},
    {"pen move 180 109"},
    {"pen down"},
    {"pen move 180 111"},
    {"pen up"},
    {"pen up"},
    {"pen move 182 109"},
    {"pen down"},
    {"pen move 182 111"},
    {"pen move 180 110"},
    {"pen up"},
    {"pen up"},
    {"pen move 182 110"},
    {"pen down"},
    {"pen move 180 110"},
    {"pen up"},
    {"pen up"},
    {"pen move 180 110"},
    {"pen down"},
    {"pen move 177 110"},
    {"pen up"},
    {"pen up"},
    {"pen move 182 110"},
    {"pen down"},
    {"pen move 185 110"},
    {"pen up"},
    {"pen up"},
    {"pen move 180 111"},
    {"pen down"},
    {"pen move 179 111"},
    {"pen up"},
    {"pen up"},
    {"pen move 181 109"},
    {"pen down"},
    {"pen move 180 109"},
    {"pen up"},
};

// Font: 0
static const LampCommand font_0_commands[] = {
    {"pen up"},
    {"pen move 22 1"},
    {"pen down"},
    {"pen move 14 10"},
    {"pen move 22 1"},
    {"pen up"},
};

// Font: 1
static const LampCommand font_1_commands[] = {
    {"pen up"},
    {"pen move 1 8"},
    {"pen down"},
    {"pen move 1 35"},
    {"pen move -13 41"},
    {"pen up"},
    {"pen move 1 35"},
    {"pen down"},
    {"pen move 14 33"},
    {"pen up"},
};

// Font: 2
static const LampCommand font_2_commands[] = {
    {"pen up"},
    {"pen move 6 7"},
    {"pen down"},
    {"pen move -2 16"},
    {"pen up"},
    {"pen move 7 7"},
    {"pen down"},
    {"pen move 30 9"},
    {"pen up"},
};

// Font: 3
static const LampCommand font_3_commands[] = {
    {"pen up"},
    {"pen move 1 1"},
    {"pen move 0 3"},
    {"pen move 5 2"},
    {"pen down"},
    {"pen move 18 8"},
    {"pen move 0 31"},
    {"pen up"},
};

// Font: 4
static const LampCommand font_4_commands[] = {
    {"pen up"},
    {"pen move 25 0"},
    {"pen down"},
    {"pen move 34 3"},
    {"pen up"},
    {"pen move 3 3"},
    {"pen down"},
    {"pen move 25 -15"},
    {"pen up"},
    {"pen move 5 1"},
    {"pen down"},
    {"pen move 4 14"},
    {"pen up"},
};

// Font: 5
static const LampCommand font_5_commands[] = {
    {"pen up"},
    {"pen move 0 30"},
    {"pen down"},
    {"pen move 25 22"},
    {"pen move 18 2"},
    {"pen move 33 1"},
    {"pen up"},
};

// Font: 6
static const LampCommand font_6_commands[] = {
    {"pen up"},
    {"pen move 9 31"},
    {"pen down"},
    {"pen move 20 1"},
    {"pen up"},
};

// Font: 7
static const LampCommand font_7_commands[] = {
    {"pen up"},
    {"pen move 27 1"},
    {"pen down"},
    {"pen move 10 32"},
    {"pen up"},
    {"pen move 0 2"},
    {"pen down"},
    {"pen move 24 2"},
    {"pen up"},
};

// Font: 8
static const LampCommand font_8_commands[] = {
    {"pen up"},
    {"pen move 11 18"},
    {"pen down"},
    {"pen move 15 18"},
    {"pen up"},
    {"pen move 0 29"},
    {"pen down"},
    {"pen move 2 29"},
    {"pen up"},
};

// Font: 9
static const LampCommand font_9_commands[] = {
    {"pen up"},
    {"pen move 20 15"},
    {"pen down"},
    {"pen move 16 30"},
    {"pen up"},
};

// Font: A
static const LampCommand font_A_commands[] = {
    {"pen up"},
    {"pen move 13 1"},
    {"pen down"},
    {"pen move 2 11"},
    {"pen move 13 1"},
    {"pen up"},
    {"pen move 18 15"},
    {"pen down"},
    {"pen move 30 20"},
    {"pen up"},
};

// Font: B
static const LampCommand font_B_commands[] = {
    {"pen up"},
    {"pen move 11 0"},
    {"pen down"},
    {"pen move 21 -2"},
    {"pen move 4 27"},
    {"pen up"},
    {"pen move 0 35"},
    {"pen down"},
    {"pen move 1 35"},
    {"pen up"},
};

// Font: C
static const LampCommand font_C_commands[] = {
    {"pen up"},
    {"pen move 24 17"},
    {"pen down"},
    {"pen move 2 12"},
    {"pen move 14 6"},
    {"pen up"},
};

// Font: D
static const LampCommand font_D_commands[] = {
    {"pen up"},
    {"pen move 27 0"},
    {"pen down"},
    {"pen move 23 7"},
    {"pen move 15 14"},
    {"pen move 21 22"},
    {"pen up"},
    {"pen move 22 22"},
    {"pen down"},
    {"pen move 34 17"},
    {"pen up"},
};

// Font: E
static const LampCommand font_E_commands[] = {
    {"pen up"},
    {"pen move 1 15"},
    {"pen down"},
    {"pen move 2 19"},
    {"pen move 24 16"},
    {"pen up"},
};

// Font: F
static const LampCommand font_F_commands[] = {
    {"pen up"},
    {"pen move 0 52"},
    {"pen down"},
    {"pen move 12 48"},
    {"pen up"},
    {"pen move 0 63"},
    {"pen down"},
    {"pen move 11 63"},
    {"pen up"},
};

// Font: G
static const LampCommand font_G_commands[] = {
    {"pen up"},
    {"pen move 1227 1164"},
    {"pen down"},
    {"pen move 1212 1158"},
    {"pen up"},
};

// Font: H
static const LampCommand font_H_commands[] = {
    {"pen up"},
    {"pen move 180 1303"},
    {"pen move 180 1308"},
    {"pen down"},
    {"pen move 186 1300"},
    {"pen up"},
    {"pen move 148 1311"},
};

// Font: I
static const LampCommand font_I_commands[] = {
    {"pen up"},
    {"pen move 349 1307"},
    {"pen down"},
    {"pen move 349 1308"},
    {"pen up"},
    {"pen move 345 1322"},
    {"pen down"},
    {"pen move 360 1336"},
    {"pen up"},
};

// Font: J
static const LampCommand font_J_commands[] = {
    {"pen up"},
    {"pen move 515 1320"},
    {"pen down"},
    {"pen move 501 1327"},
    {"pen up"},
};

// Font: K
static const LampCommand font_K_commands[] = {
    {"pen up"},
    {"pen move 683 1307"},
    {"pen down"},
    {"pen move 681 1322"},
    {"pen up"},
    {"pen move 701 1297"},
    {"pen move 705 1294"},
};

// Font: L
static const LampCommand font_L_commands[] = {
    {"pen up"},
    {"pen move 852 1304"},
    {"pen down"},
    {"pen move 869 1299"},
    {"pen up"},
};

// Font: M
static const LampCommand font_M_commands[] = {
    {"pen up"},
    {"pen move 1018 1322"},
    {"pen down"},
    {"pen move 1017 1339"},
    {"pen move 1019 1335"},
    {"pen up"},
};

// Font: N
static const LampCommand font_N_commands[] = {
    {"pen up"},
    {"pen move 1186 1321"},
    {"pen down"},
    {"pen move 1184 1331"},
    {"pen up"},
    {"pen move 1187 1328"},
    {"pen down"},
    {"pen move 1201 1327"},
    {"pen up"},
};

// Font: O
static const LampCommand font_O_commands[] = {
    {"pen up"},
    {"pen move 194 1487"},
    {"pen down"},
    {"pen move 197 1467"},
    {"pen up"},
};

// Font: P
static const LampCommand font_P_commands[] = {
    {"pen up"},
    {"pen move 339 1487"},
    {"pen down"},
    {"pen move 329 1497"},
    {"pen up"},
    {"pen move 326 1504"},
    {"pen down"},
    {"pen move 322 1520"},
    {"pen up"},
};

// Font: Q
static const LampCommand font_Q_commands[] = {
    {"pen up"},
    {"pen move 516 1500"},
    {"pen down"},
    {"pen move 515 1503"},
    {"pen up"},
};

// Font: R
static const LampCommand font_R_commands[] = {
    {"pen up"},
    {"pen move 671 1502"},
    {"pen down"},
    {"pen move 684 1492"},
    {"pen up"},
    {"pen move 667 1492"},
    {"pen move 666 1495"},
    {"pen move 668 1497"},
    {"pen down"},
    {"pen move 668 1501"},
    {"pen up"},
};

// Font: S
static const LampCommand font_S_commands[] = {
    {"pen up"},
    {"pen move 846 1487"},
    {"pen down"},
    {"pen move 839 1497"},
    {"pen up"},
};

// Font: T
static const LampCommand font_T_commands[] = {
    {"pen up"},
    {"pen move 1015 1478"},
    {"pen down"},
    {"pen move 1026 1488"},
    {"pen up"},
    {"pen move 1012 1488"},
    {"pen down"},
    {"pen move 1003 1490"},
    {"pen up"},
    {"pen move 1009 1491"},
    {"pen down"},
    {"pen move 1015 1485"},
    {"pen up"},
};

// Font: U
static const LampCommand font_U_commands[] = {
    {"pen up"},
    {"pen move 1177 1489"},
    {"pen down"},
    {"pen move 1185 1475"},
    {"pen up"},
    {"pen move 1183 1484"},
    {"pen down"},
    {"pen move 1196 1490"},
    {"pen up"},
};

// Font: V
static const LampCommand font_V_commands[] = {
    {"pen up"},
    {"pen move 200 1662"},
    {"pen down"},
    {"pen move 195 1642"},
    {"pen up"},
};

// Font: W
static const LampCommand font_W_commands[] = {
    {"pen up"},
    {"pen move 378 1660"},
    {"pen down"},
    {"pen move 371 1642"},
    {"pen up"},
    {"pen move 370 1642"},
    {"pen down"},
    {"pen move 371 1642"},
    {"pen up"},
};

// Font: X
static const LampCommand font_X_commands[] = {
    {"pen up"},
    {"pen move 518 1658"},
    {"pen down"},
    {"pen move 540 1659"},
    {"pen up"},
    {"pen move 528 1645"},
    {"pen down"},
    {"pen move 518 1658"},
    {"pen up"},
    {"pen move 517 1658"},
    {"pen down"},
    {"pen move 508 1667"},
    {"pen up"},
};

// Font: Y
static const LampCommand font_Y_commands[] = {
    {"pen up"},
    {"pen move 705 1656"},
    {"pen down"},
    {"pen move 700 1672"},
    {"pen up"},
    {"pen move 693 1634"},
    {"pen down"},
    {"pen move 709 1630"},
    {"pen up"},
    {"pen move 688 1631"},
    {"pen down"},
    {"pen move 690 1631"},
    {"pen up"},
};

// Font: Z
static const LampCommand font_Z_commands[] = {
    {"pen up"},
    {"pen move 852 1658"},
    {"pen down"},
    {"pen move 833 1681"},
    {"pen up"},
    {"pen move 853 1656"},
    {"pen move 865 1672"},
    {"pen down"},
    {"pen move 842 1675"},
    {"pen up"},
};

// Component lookup table
static const Component COMPONENTS[] = {
    {"D", D_commands, 26},
    {"GND", GND_commands, 25},
    {"L", L_commands, 18},
    {"NMOS", NMOS_commands, 88},
    {"NPN", NPN_commands, 46},
    {"NP_C", NP_C_commands, 20},
    {"OPAMP", OPAMP_commands, 45},
    {"PMOS", PMOS_commands, 88},
    {"PNP", PNP_commands, 46},
    {"P_C", P_C_commands, 27},
    {"R", R_commands, 26},
    {"R_TRIM", R_TRIM_commands, 41},
    {"TX", TX_commands, 62},
    {"VAC", VAC_commands, 29},
    {"VAR", VAR_commands, 28},
    {"VDC", VDC_commands, 45},
    {"ZD", ZD_commands, 36},
};

static const int COMPONENT_COUNT = 17;

// Font glyph lookup table
static const FontGlyph FONT_GLYPHS[] = {
    {'0', font_0_commands, 6},
    {'1', font_1_commands, 10},
    {'2', font_2_commands, 9},
    {'3', font_3_commands, 8},
    {'4', font_4_commands, 13},
    {'5', font_5_commands, 7},
    {'6', font_6_commands, 5},
    {'7', font_7_commands, 9},
    {'8', font_8_commands, 9},
    {'9', font_9_commands, 5},
    {'A', font_A_commands, 10},
    {'B', font_B_commands, 10},
    {'C', font_C_commands, 6},
    {'D', font_D_commands, 11},
    {'E', font_E_commands, 6},
    {'F', font_F_commands, 9},
    {'G', font_G_commands, 5},
    {'H', font_H_commands, 7},
    {'I', font_I_commands, 9},
    {'J', font_J_commands, 5},
    {'K', font_K_commands, 7},
    {'L', font_L_commands, 5},
    {'M', font_M_commands, 6},
    {'N', font_N_commands, 9},
    {'O', font_O_commands, 5},
    {'P', font_P_commands, 9},
    {'Q', font_Q_commands, 5},
    {'R', font_R_commands, 11},
    {'S', font_S_commands, 5},
    {'T', font_T_commands, 13},
    {'U', font_U_commands, 9},
    {'V', font_V_commands, 5},
    {'W', font_W_commands, 9},
    {'X', font_X_commands, 13},
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

// Send component to lamp with offset and scale
inline void render_component(const char* name, int offset_x, int offset_y, float scale, int lamp_fd) {
    const Component* comp = find_component(name);
    if (!comp) return;

    for (int i = 0; i < comp->count; i++) {
        std::string cmd = comp->commands[i].cmd;

        // Parse and transform coordinates
        if (cmd.find("pen move") == 0) {
            int x, y;
            if (sscanf(cmd.c_str(), "pen move %d %d", &x, &y) == 2) {
                x = (int)(x * scale) + offset_x;
                y = (int)(y * scale) + offset_y;
                cmd = "pen move " + std::to_string(x) + " " + std::to_string(y);
            }
        }

        cmd += "\n";
        write(lamp_fd, cmd.c_str(), cmd.length());
    }
}

// Render text with fonts
inline void render_text(const char* text, int x, int y, float scale, int spacing, int lamp_fd) {
    int offset_x = x;

    for (const char* p = text; *p; p++) {
        char c = toupper(*p);  // Convert to uppercase

        if (c == ' ') {
            offset_x += spacing;
            continue;
        }

        const FontGlyph* glyph = find_glyph(c);
        if (!glyph) continue;

        for (int i = 0; i < glyph->count; i++) {
            std::string cmd = glyph->commands[i].cmd;

            if (cmd.find("pen move") == 0) {
                int gx, gy;
                if (sscanf(cmd.c_str(), "pen move %d %d", &gx, &gy) == 2) {
                    gx = (int)(gx * scale) + offset_x;
                    gy = (int)(gy * scale) + y;
                    cmd = "pen move " + std::to_string(gx) + " " + std::to_string(gy);
                }
            }

            cmd += "\n";
            write(lamp_fd, cmd.c_str(), cmd.length());
        }

        offset_x += (int)(25 * scale);  // Character spacing
    }
}

} // namespace elxnk

#endif // ELXNK_LIBRARY_H
