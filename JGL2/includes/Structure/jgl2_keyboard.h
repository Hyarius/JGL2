#pragma once

#include "jgl2_includes.h"
#include "jgl2_enums.h"
#include "jgl2_basic_types.h"

namespace jgl
{

    class Keyboard
    {
    public:
        static const Size_t C_NB_KEY = 255;

        enum Key
        {
            Backspace = 8,
            Tab = 9,
            Clear = 12,
            Return = 13,
            Shift = 16,
            Control = 17,
            Alt = 18,
            Pause = 19,
            Capslock = 20,
            Escape = 27,
            Convert = 28,
            Non_convert = 29,
            Accept = 30,
            Mode_change = 31,
            Space = 32,
            Prior = 33,
            Next = 34,
            End = 35,
            Home = 36,
            Left_arrow = 37,
            Up_arrow = 38,
            Right_arrow = 39,
            Down_arrow = 40,
            Select = 41,
            Print = 42,
            Execute = 43,
            Snapshot = 44,
            Insert = 45,
            Delete = 46,
            Help = 47,
            Key_0 = 48,
            Key_1 = 49,
            Key_2 = 50,
            Key_3 = 51,
            Key_4 = 52,
            Key_5 = 53,
            Key_6 = 54,
            Key_7 = 55,
            Key_8 = 56,
            Key_9 = 57,
            A = 65,
            B = 66,
            C = 67,
            D = 68,
            E = 69,
            F = 70,
            G = 71,
            H = 72,
            I = 73,
            J = 74,
            K = 75,
            L = 76,
            M = 77,
            N = 78,
            O = 79,
            P = 80,
            Q = 81,
            R = 82,
            S = 83,
            T = 84,
            U = 85,
            V = 86,
            W = 87,
            X = 88,
            Y = 89,
            Z = 90,
            Left_windows = 91,
            Right_windows = 92,
            App = 93,
            Sleep = 95,
            Numpad_0 = 96,
            Numpad_1 = 97,
            Numpad_2 = 98,
            Numpad_3 = 99,
            Numpad_4 = 100,
            Numpad_5 = 101,
            Numpad_6 = 102,
            Numpad_7 = 103,
            Numpad_8 = 104,
            Numpad_9 = 105,
            Numpad_multiply = 106,
            Numpad_plus = 107,
            Numpad_separator = 108,
            Numpad_minus = 109,
            Numpad_decimal = 110,
            Numpad_divide = 111,
            F1 = 112,
            F2 = 113,
            F3 = 114,
            F4 = 115,
            F5 = 116,
            F6 = 117,
            F7 = 118,
            F8 = 119,
            F9 = 120,
            F10 = 121,
            F11 = 122,
            F12 = 123,
            F13 = 124,
            F14 = 125,
            F15 = 126,
            F16 = 127,
            F17 = 128,
            F18 = 129,
            F19 = 130,
            F20 = 131,
            F21 = 132,
            F22 = 133,
            F23 = 134,
            F24 = 135,
            Num_lock = 144,
            Scroll = 145,
            Left_shift = 160,
            Right_shift = 161,
            Left_control = 162,
            Right_control = 163,
            Left_menu = 164,
            Right_menu = 165,
            Semi_colon = 186,
            Plus = 187,
            Comma = 188,
            Minus = 189,
            Period = 190,
            Question_mark = 191,
            Tilde = 192,
            Left_bracket = 219,
            Vertical_line = 220,
            Right_bracket = 221,
            Quote = 222,
            Unknow = 223,
            Angle_bracket = 226,
            Process = 229,
        };

        friend class Application;
    private:
        static std::map<Keyboard::Key, std::string> _convertKeyToString;
        InputStatus _keys[C_NB_KEY];
        WChar _entry;

        Keyboard();
        void _updateState();
        void _updateEntry(WChar p_entry);

    public:
        InputStatus getKey(Keyboard::Key p_key);
        WChar getEntry();
    };
}