#pragma once


enum class InputType {
    None = 0,
    Keyboard,
    GamePadKey,
    GamePadAxis
};

enum class InputClass {
    Primary,
    Secondary
};

class InputMapping {
public:
    InputMapping();
    InputMapping(InputType type, int index, float maxValue, float idleValue);

    InputType type() const;
    int index() const;
    float maxValue() const;
    float idleValue() const;

private:
    InputType m_type;
    int m_index;
    float m_maxValue;
    float m_idleValue;
};

