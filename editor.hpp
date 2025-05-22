#ifndef EDITOR_HPP
#define EDITOR_HPP

#include <wx/wx.h>
#include <iostream>
#include <vector>
#include <string>


class Editor {
public:
    Editor() = default;

    void write_text(char chr);
    void delete_text();
    void line_break();

    std::vector<std::string> get_text() const { return text_; }
    uint32_t get_line() const { return line_; }
    uint32_t get_pos() const { return pos_; }
private:
    std::vector<std::string> text_ = {""};
    uint32_t line_ = 0;
    uint32_t pos_ = 0;
};




#endif
