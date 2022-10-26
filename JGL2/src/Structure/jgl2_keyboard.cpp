#include "Structure/jgl2_keyboard.h"

namespace jgl
{
    std::map<jgl::Keyboard::Key, jgl::String> jgl::Keyboard::_convertKeyToString = {
        {jgl::Keyboard::Key::Backspace, "Backspace"},
        {jgl::Keyboard::Key::Tab, "Tab"},
        {jgl::Keyboard::Key::Clear, "Clear"},
        {jgl::Keyboard::Key::Return, "Return"},
        {jgl::Keyboard::Key::Shift, "Shift"},
        {jgl::Keyboard::Key::Control, "Control"},
        {jgl::Keyboard::Key::Alt, "Alt"},
        {jgl::Keyboard::Key::Pause, "Pause"},
        {jgl::Keyboard::Key::Capslock, "Capslock"},
        {jgl::Keyboard::Key::Escape, "Escape"},
        {jgl::Keyboard::Key::Convert, "Convert"},
        {jgl::Keyboard::Key::Non_convert, "Non_convert"},
        {jgl::Keyboard::Key::Accept, "Accept"},
        {jgl::Keyboard::Key::Mode_change, "Mode_change"},
        {jgl::Keyboard::Key::Space, "Space"},
        {jgl::Keyboard::Key::Prior, "Prior"},
        {jgl::Keyboard::Key::Next, "Next"},
        {jgl::Keyboard::Key::End, "End"},
        {jgl::Keyboard::Key::Home, "Home"},
        {jgl::Keyboard::Key::Left_arrow, "Left_arrow"},
        {jgl::Keyboard::Key::Up_arrow, "Up_arrow"},
        {jgl::Keyboard::Key::Right_arrow, "Right_arrow"},
        {jgl::Keyboard::Key::Down_arrow, "Down_arrow"},
        {jgl::Keyboard::Key::Select, "Select"},
        {jgl::Keyboard::Key::Print, "Print"},
        {jgl::Keyboard::Key::Execute, "Execute"},
        {jgl::Keyboard::Key::Snapshot, "Snapshot"},
        {jgl::Keyboard::Key::Insert, "Insert"},
        {jgl::Keyboard::Key::Delete, "Delete"},
        {jgl::Keyboard::Key::Help, "Help"},
        {jgl::Keyboard::Key::Key_0, "Key_0"},
        {jgl::Keyboard::Key::Key_1, "Key_1"},
        {jgl::Keyboard::Key::Key_2, "Key_2"},
        {jgl::Keyboard::Key::Key_3, "Key_3"},
        {jgl::Keyboard::Key::Key_4, "Key_4"},
        {jgl::Keyboard::Key::Key_5, "Key_5"},
        {jgl::Keyboard::Key::Key_6, "Key_6"},
        {jgl::Keyboard::Key::Key_7, "Key_7"},
        {jgl::Keyboard::Key::Key_8, "Key_8"},
        {jgl::Keyboard::Key::Key_9, "Key_9"},
        {jgl::Keyboard::Key::A, "A"},
        {jgl::Keyboard::Key::B, "B"},
        {jgl::Keyboard::Key::C, "C"},
        {jgl::Keyboard::Key::D, "D"},
        {jgl::Keyboard::Key::E, "E"},
        {jgl::Keyboard::Key::F, "F"},
        {jgl::Keyboard::Key::G, "G"},
        {jgl::Keyboard::Key::H, "H"},
        {jgl::Keyboard::Key::I, "I"},
        {jgl::Keyboard::Key::J, "J"},
        {jgl::Keyboard::Key::K, "K"},
        {jgl::Keyboard::Key::L, "L"},
        {jgl::Keyboard::Key::M, "M"},
        {jgl::Keyboard::Key::N, "N"},
        {jgl::Keyboard::Key::O, "O"},
        {jgl::Keyboard::Key::P, "P"},
        {jgl::Keyboard::Key::Q, "Q"},
        {jgl::Keyboard::Key::R, "R"},
        {jgl::Keyboard::Key::S, "S"},
        {jgl::Keyboard::Key::T, "T"},
        {jgl::Keyboard::Key::U, "U"},
        {jgl::Keyboard::Key::V, "V"},
        {jgl::Keyboard::Key::W, "W"},
        {jgl::Keyboard::Key::X, "X"},
        {jgl::Keyboard::Key::Y, "Y"},
        {jgl::Keyboard::Key::Z, "Z"},
        {jgl::Keyboard::Key::Left_windows, "Left_windows"},
        {jgl::Keyboard::Key::Right_windows, "Right_windows"},
        {jgl::Keyboard::Key::App, "App"},
        {jgl::Keyboard::Key::Sleep, "Sleep"},
        {jgl::Keyboard::Key::Numpad_0, "Numpad_0"},
        {jgl::Keyboard::Key::Numpad_1, "Numpad_1"},
        {jgl::Keyboard::Key::Numpad_2, "Numpad_2"},
        {jgl::Keyboard::Key::Numpad_3, "Numpad_3"},
        {jgl::Keyboard::Key::Numpad_4, "Numpad_4"},
        {jgl::Keyboard::Key::Numpad_5, "Numpad_5"},
        {jgl::Keyboard::Key::Numpad_6, "Numpad_6"},
        {jgl::Keyboard::Key::Numpad_7, "Numpad_7"},
        {jgl::Keyboard::Key::Numpad_8, "Numpad_8"},
        {jgl::Keyboard::Key::Numpad_9, "Numpad_9"},
        {jgl::Keyboard::Key::Numpad_multiply, "Numpad_multiply"},
        {jgl::Keyboard::Key::Numpad_plus, "Numpad_plus"},
        {jgl::Keyboard::Key::Numpad_separator, "Numpad_separator"},
        {jgl::Keyboard::Key::Numpad_minus, "Numpad_minus"},
        {jgl::Keyboard::Key::Numpad_decimal, "Numpad_decimal"},
        {jgl::Keyboard::Key::Numpad_divide, "Numpad_divide"},
        {jgl::Keyboard::Key::F1, "F1"},
        {jgl::Keyboard::Key::F2, "F2"},
        {jgl::Keyboard::Key::F3, "F3"},
        {jgl::Keyboard::Key::F4, "F4"},
        {jgl::Keyboard::Key::F5, "F5"},
        {jgl::Keyboard::Key::F6, "F6"},
        {jgl::Keyboard::Key::F7, "F7"},
        {jgl::Keyboard::Key::F8, "F8"},
        {jgl::Keyboard::Key::F9, "F9"},
        {jgl::Keyboard::Key::F10, "F10"},
        {jgl::Keyboard::Key::F11, "F11"},
        {jgl::Keyboard::Key::F12, "F12"},
        {jgl::Keyboard::Key::F13, "F13"},
        {jgl::Keyboard::Key::F14, "F14"},
        {jgl::Keyboard::Key::F15, "F15"},
        {jgl::Keyboard::Key::F16, "F16"},
        {jgl::Keyboard::Key::F17, "F17"},
        {jgl::Keyboard::Key::F18, "F18"},
        {jgl::Keyboard::Key::F19, "F19"},
        {jgl::Keyboard::Key::F20, "F20"},
        {jgl::Keyboard::Key::F21, "F21"},
        {jgl::Keyboard::Key::F22, "F22"},
        {jgl::Keyboard::Key::F23, "F23"},
        {jgl::Keyboard::Key::F24, "F24"},
        {jgl::Keyboard::Key::Num_lock, "Num_lock"},
        {jgl::Keyboard::Key::Scroll, "Scroll"},
        {jgl::Keyboard::Key::Left_shift, "Left_shift"},
        {jgl::Keyboard::Key::Right_shift, "Right_shift"},
        {jgl::Keyboard::Key::Left_control, "Left_control"},
        {jgl::Keyboard::Key::Right_control, "Right_control"},
        {jgl::Keyboard::Key::Left_menu, "Left_menu"},
        {jgl::Keyboard::Key::Right_menu, "Right_menu"},
        {jgl::Keyboard::Key::Semi_colon, "Semi_colon"},
        {jgl::Keyboard::Key::Plus, "Plus"},
        {jgl::Keyboard::Key::Comma, "Comma"},
        {jgl::Keyboard::Key::Minus, "Minus"},
        {jgl::Keyboard::Key::Period, "Period"},
        {jgl::Keyboard::Key::Question_mark, "Question_mark"},
        {jgl::Keyboard::Key::Tilde, "Tilde"},
        {jgl::Keyboard::Key::Left_bracket, "Left_bracket"},
        {jgl::Keyboard::Key::Vertical_line, "Vertical_line"},
        {jgl::Keyboard::Key::Right_bracket, "Right_bracket"},
        {jgl::Keyboard::Key::Quote, "Quote"},
        {jgl::Keyboard::Key::Unknow, "Unknow"},
        {jgl::Keyboard::Key::Angle_bracket, "Angle_bracket"},
        {jgl::Keyboard::Key::Process, "Process"},
    };

    jgl::Keyboard::Keyboard()
    {
        for (jgl::Size_t i = 0; i < C_NB_KEY; i++)
            _keys[i] = jgl::InputStatus::Up;
    }

    void Keyboard::_updateState()
    {
        for (jgl::Size_t i = 0; i < C_NB_KEY; i++)
        {
            if (_keys[i] == jgl::InputStatus::Pressed)
            {
                _keys[i] = jgl::InputStatus::Down;
            }
            else if (_keys[i] == jgl::InputStatus::Released)
            {
                _keys[i] = jgl::InputStatus::Up;
            }
        }
    }

    void Keyboard::_updateEntry(jgl::Glyph p_entry)
    {
        _entry = p_entry;
    }

    jgl::InputStatus Keyboard::getKey(Keyboard::Key p_key)
    {
        return (_keys[static_cast<jgl::Size_t>(p_key)]);
    }
    
    jgl::Glyph Keyboard::getEntry()
    {
        return (_entry);
    }
}