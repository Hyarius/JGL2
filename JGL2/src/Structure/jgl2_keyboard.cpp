#include "Structure/jgl2_keyboard.h"
#include "Structure/jgl2_iostream.h"

namespace jgl
{
    std::map<Keyboard::Key, std::string> KeyToStringMap = {
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
        {Keyboard::Key::LeftArrow, "LeftArrow"},
        {Keyboard::Key::UpArrow, "UpArrow"},
        {Keyboard::Key::RightArrow, "RightArrow"},
        {Keyboard::Key::DownArrow, "DownArrow"},
        {Keyboard::Key::Select, "Select"},
        {Keyboard::Key::Print, "Print"},
        {Keyboard::Key::Execute, "Execute"},
        {Keyboard::Key::Snapshot, "Snapshot"},
        {Keyboard::Key::Insert, "Insert"},
        {Keyboard::Key::Delete, "Delete"},
        {Keyboard::Key::Help, "Help"},
        {Keyboard::Key::Key0, "Key0"},
        {Keyboard::Key::Key1, "Key1"},
        {Keyboard::Key::Key2, "Key2"},
        {Keyboard::Key::Key3, "Key3"},
        {Keyboard::Key::Key4, "Key4"},
        {Keyboard::Key::Key5, "Key5"},
        {Keyboard::Key::Key6, "Key6"},
        {Keyboard::Key::Key7, "Key7"},
        {Keyboard::Key::Key8, "Key8"},
        {Keyboard::Key::Key9, "Key9"},
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
        {Keyboard::Key::LeftWindows, "LeftWindows"},
        {Keyboard::Key::RightWindows, "RightWindows"},
        {Keyboard::Key::App, "App"},
        {Keyboard::Key::Sleep, "Sleep"},
        {Keyboard::Key::Numpad0, "Numpad0"},
        {Keyboard::Key::Numpad1, "Numpad1"},
        {Keyboard::Key::Numpad2, "Numpad2"},
        {Keyboard::Key::Numpad3, "Numpad3"},
        {Keyboard::Key::Numpad4, "Numpad4"},
        {Keyboard::Key::Numpad5, "Numpad5"},
        {Keyboard::Key::Numpad6, "Numpad6"},
        {Keyboard::Key::Numpad7, "Numpad7"},
        {Keyboard::Key::Numpad8, "Numpad8"},
        {Keyboard::Key::Numpad9, "Numpad9"},
        {Keyboard::Key::NumpadMultiply, "NumpadMultiply"},
        {Keyboard::Key::NumpadPlus, "NumpadPlus"},
        {Keyboard::Key::NumpadSeparator, "NumpadSeparator"},
        {Keyboard::Key::NumpadMinus, "NumpadMinus"},
        {Keyboard::Key::NumpadDecimal, "NumpadDecimal"},
        {Keyboard::Key::NumpadDivide, "NumpadDivide"},
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
        {Keyboard::Key::NumLock, "NumLock"},
        {Keyboard::Key::Scroll, "Scroll"},
        {Keyboard::Key::LeftShift, "LeftShift"},
        {Keyboard::Key::RightShift, "RightShift"},
        {Keyboard::Key::LeftControl, "LeftControl"},
        {Keyboard::Key::RightControl, "RightControl"},
        {Keyboard::Key::LeftMenu, "LeftMenu"},
        {Keyboard::Key::RightMenu, "RightMenu"},
        {Keyboard::Key::SemiColon, "SemiColon"},
        {Keyboard::Key::Plus, "Plus"},
        {Keyboard::Key::Comma, "Comma"},
        {Keyboard::Key::Minus, "Minus"},
        {Keyboard::Key::Period, "Period"},
        {Keyboard::Key::QuestionMark, "QuestionMark"},
        {Keyboard::Key::Tilde, "Tilde"},
        {Keyboard::Key::LeftBracket, "LeftBracket"},
        {Keyboard::Key::VerticalLine, "VerticalLine"},
        {Keyboard::Key::RightBracket, "RightBracket"},
        {Keyboard::Key::Quote, "Quote"},
        {Keyboard::Key::Unknow, "Unknow"},
        {Keyboard::Key::AngleBracket, "AngleBracket"},
        {Keyboard::Key::Process, "Process"},
    };

    Keyboard::Keyboard()
    {
        for (Size_t i = 0; i < C_NB_KEY; i++)
            _keys[i] = InputStatus::Up;
    }

    void Keyboard::_updateState()
    {
        _entry = '\0';
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

    void Keyboard::_updateEntry(UChar p_entry)
    {
        _entry = p_entry;
    }

    const InputStatus& Keyboard::getKey(Keyboard::Key p_key) const
    {
        return (_keys[static_cast<Size_t>(p_key)]);
    }
    
    const UChar& Keyboard::getEntry() const
    {
        return (_entry);
    }

    std::string to_string(const jgl::Keyboard::Key& p_key)
    {
        if (KeyToStringMap.count(p_key) == 0)
            return ("Unknow key");
        return (KeyToStringMap[p_key]);
    }
}