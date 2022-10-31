#include "Structure/jgl2_keyboard.h"

namespace jgl
{
    std::map<Keyboard::Key, std::string> Keyboard::_convertKeyToString = {
        {Keyboard::Key::Backspace, "Backspace"},
        {Keyboard::Key::Tab, "Tab"},
        {Keyboard::Key::Clear, "Clear"},
        {Keyboard::Key::Return, "Return"},
        {Keyboard::Key::Shift, "Shift"},
        {Keyboard::Key::Control, "Control"},
        {Keyboard::Key::Alt, "Alt"},
        {Keyboard::Key::Pause, "Pause"},
        {Keyboard::Key::Capslock, "Capslock"},
        {Keyboard::Key::Escape, "Escape"},
        {Keyboard::Key::Convert, "Convert"},
        {Keyboard::Key::Non_convert, "Non_convert"},
        {Keyboard::Key::Accept, "Accept"},
        {Keyboard::Key::Mode_change, "Mode_change"},
        {Keyboard::Key::Space, "Space"},
        {Keyboard::Key::Prior, "Prior"},
        {Keyboard::Key::Next, "Next"},
        {Keyboard::Key::End, "End"},
        {Keyboard::Key::Home, "Home"},
        {Keyboard::Key::Left_arrow, "Left_arrow"},
        {Keyboard::Key::Up_arrow, "Up_arrow"},
        {Keyboard::Key::Right_arrow, "Right_arrow"},
        {Keyboard::Key::Down_arrow, "Down_arrow"},
        {Keyboard::Key::Select, "Select"},
        {Keyboard::Key::Print, "Print"},
        {Keyboard::Key::Execute, "Execute"},
        {Keyboard::Key::Snapshot, "Snapshot"},
        {Keyboard::Key::Insert, "Insert"},
        {Keyboard::Key::Delete, "Delete"},
        {Keyboard::Key::Help, "Help"},
        {Keyboard::Key::Key_0, "Key_0"},
        {Keyboard::Key::Key_1, "Key_1"},
        {Keyboard::Key::Key_2, "Key_2"},
        {Keyboard::Key::Key_3, "Key_3"},
        {Keyboard::Key::Key_4, "Key_4"},
        {Keyboard::Key::Key_5, "Key_5"},
        {Keyboard::Key::Key_6, "Key_6"},
        {Keyboard::Key::Key_7, "Key_7"},
        {Keyboard::Key::Key_8, "Key_8"},
        {Keyboard::Key::Key_9, "Key_9"},
        {Keyboard::Key::A, "A"},
        {Keyboard::Key::B, "B"},
        {Keyboard::Key::C, "C"},
        {Keyboard::Key::D, "D"},
        {Keyboard::Key::E, "E"},
        {Keyboard::Key::F, "F"},
        {Keyboard::Key::G, "G"},
        {Keyboard::Key::H, "H"},
        {Keyboard::Key::I, "I"},
        {Keyboard::Key::J, "J"},
        {Keyboard::Key::K, "K"},
        {Keyboard::Key::L, "L"},
        {Keyboard::Key::M, "M"},
        {Keyboard::Key::N, "N"},
        {Keyboard::Key::O, "O"},
        {Keyboard::Key::P, "P"},
        {Keyboard::Key::Q, "Q"},
        {Keyboard::Key::R, "R"},
        {Keyboard::Key::S, "S"},
        {Keyboard::Key::T, "T"},
        {Keyboard::Key::U, "U"},
        {Keyboard::Key::V, "V"},
        {Keyboard::Key::W, "W"},
        {Keyboard::Key::X, "X"},
        {Keyboard::Key::Y, "Y"},
        {Keyboard::Key::Z, "Z"},
        {Keyboard::Key::Left_windows, "Left_windows"},
        {Keyboard::Key::Right_windows, "Right_windows"},
        {Keyboard::Key::App, "App"},
        {Keyboard::Key::Sleep, "Sleep"},
        {Keyboard::Key::Numpad_0, "Numpad_0"},
        {Keyboard::Key::Numpad_1, "Numpad_1"},
        {Keyboard::Key::Numpad_2, "Numpad_2"},
        {Keyboard::Key::Numpad_3, "Numpad_3"},
        {Keyboard::Key::Numpad_4, "Numpad_4"},
        {Keyboard::Key::Numpad_5, "Numpad_5"},
        {Keyboard::Key::Numpad_6, "Numpad_6"},
        {Keyboard::Key::Numpad_7, "Numpad_7"},
        {Keyboard::Key::Numpad_8, "Numpad_8"},
        {Keyboard::Key::Numpad_9, "Numpad_9"},
        {Keyboard::Key::Numpad_multiply, "Numpad_multiply"},
        {Keyboard::Key::Numpad_plus, "Numpad_plus"},
        {Keyboard::Key::Numpad_separator, "Numpad_separator"},
        {Keyboard::Key::Numpad_minus, "Numpad_minus"},
        {Keyboard::Key::Numpad_decimal, "Numpad_decimal"},
        {Keyboard::Key::Numpad_divide, "Numpad_divide"},
        {Keyboard::Key::F1, "F1"},
        {Keyboard::Key::F2, "F2"},
        {Keyboard::Key::F3, "F3"},
        {Keyboard::Key::F4, "F4"},
        {Keyboard::Key::F5, "F5"},
        {Keyboard::Key::F6, "F6"},
        {Keyboard::Key::F7, "F7"},
        {Keyboard::Key::F8, "F8"},
        {Keyboard::Key::F9, "F9"},
        {Keyboard::Key::F10, "F10"},
        {Keyboard::Key::F11, "F11"},
        {Keyboard::Key::F12, "F12"},
        {Keyboard::Key::F13, "F13"},
        {Keyboard::Key::F14, "F14"},
        {Keyboard::Key::F15, "F15"},
        {Keyboard::Key::F16, "F16"},
        {Keyboard::Key::F17, "F17"},
        {Keyboard::Key::F18, "F18"},
        {Keyboard::Key::F19, "F19"},
        {Keyboard::Key::F20, "F20"},
        {Keyboard::Key::F21, "F21"},
        {Keyboard::Key::F22, "F22"},
        {Keyboard::Key::F23, "F23"},
        {Keyboard::Key::F24, "F24"},
        {Keyboard::Key::Num_lock, "Num_lock"},
        {Keyboard::Key::Scroll, "Scroll"},
        {Keyboard::Key::Left_shift, "Left_shift"},
        {Keyboard::Key::Right_shift, "Right_shift"},
        {Keyboard::Key::Left_control, "Left_control"},
        {Keyboard::Key::Right_control, "Right_control"},
        {Keyboard::Key::Left_menu, "Left_menu"},
        {Keyboard::Key::Right_menu, "Right_menu"},
        {Keyboard::Key::Semi_colon, "Semi_colon"},
        {Keyboard::Key::Plus, "Plus"},
        {Keyboard::Key::Comma, "Comma"},
        {Keyboard::Key::Minus, "Minus"},
        {Keyboard::Key::Period, "Period"},
        {Keyboard::Key::Question_mark, "Question_mark"},
        {Keyboard::Key::Tilde, "Tilde"},
        {Keyboard::Key::Left_bracket, "Left_bracket"},
        {Keyboard::Key::Vertical_line, "Vertical_line"},
        {Keyboard::Key::Right_bracket, "Right_bracket"},
        {Keyboard::Key::Quote, "Quote"},
        {Keyboard::Key::Unknow, "Unknow"},
        {Keyboard::Key::Angle_bracket, "Angle_bracket"},
        {Keyboard::Key::Process, "Process"},
    };

    Keyboard::Keyboard()
    {
        for (Size_t i = 0; i < C_NB_KEY; i++)
            _keys[i] = InputStatus::Up;
    }

    void Keyboard::_updateState()
    {
        for (Size_t i = 0; i < C_NB_KEY; i++)
        {
            if (_keys[i] == InputStatus::Pressed)
            {
                _keys[i] = InputStatus::Down;
            }
            else if (_keys[i] == InputStatus::Released)
            {
                _keys[i] = InputStatus::Up;
            }
        }
    }

    void Keyboard::_updateEntry(Char p_entry)
    {
        _entry = p_entry;
    }

    InputStatus Keyboard::getKey(Keyboard::Key p_key)
    {
        return (_keys[static_cast<Size_t>(p_key)]);
    }
    
    Char Keyboard::getEntry()
    {
        return (_entry);
    }
}