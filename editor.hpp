#ifndef EDITOR_HPP
#define EDITOR_HPP

#include <wx/wx.h>
#include <iostream>
#include <vector>
#include <string>

enum class Direction {
    left,
    up,
    right,
    down,
};

class Editor {
public:
    Editor() = default;

    void write_text(char chr);
    void delete_text();
    void new_line();
    void move(Direction direction);

    std::vector<std::string> get_text() const { return text_; }
    std::string string_from_pos();

    uint64_t get_line() const { return line_; }
    uint64_t get_pos() const { return pos_; }
private:
    std::vector<std::string> text_ = {""};
    uint64_t line_ = 0;
    uint64_t pos_ = 0;
};




#endif
