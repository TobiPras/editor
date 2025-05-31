#ifndef EDITOR_HPP
#define EDITOR_HPP

#include <wx/wx.h>
#include <iostream>
#include <vector>
#include <string>
#include <utility>
#include <regex>
#include <fstream>


const std::vector<std::vector<std::string>> keywords = {
    {"int", "float", "double", "bool", "void", "const", "static"},
    {"return", "if", "else", "for", "while", "switch", "case", "break", "continue", "default", "try", "catch"},
    {"namespace", "class", "public", "private", "protected", "virtual", "override", "new", "delete", "try", "catch", "using", "this"}
};


enum class Direction {
    left,
    up,
    right,
    down,
};


class Editor {
public:
    Editor();

    void write_text(char chr);
    void delete_text();

    void move(Direction direction);
    void string_from_pos();

    void set_filename(std::string filename);
    void write_filename(char chr) { filename_.push_back(chr); }
    void delete_filename() { filename_.pop_back(); }

    void load_file();
    void save_file();
    void create_file();

    void set_pos(std::pair<int, int> pos);
    void set_start_mark(std::pair<int, int> pos);
    void set_end_mark(std::pair<int, int> pos);
    void set_extention();
    void set_keywords_regex();

    void syntax_high();

    bool is_filename_input() { return filename_input_; }

    std::vector<std::string> get_text() const { return text_; }

    std::vector<std::pair<uint64_t, uint64_t>> get_mark_pos() const { return mark_pos_; }

    std::vector<std::vector<uint64_t>> get_high_pos() const { return high_pos_; }

    std::string get_filename() const { return filename_; }

    uint64_t get_line() const { return line_; }
    uint64_t get_pos() const { return pos_; }
private:
    void new_line(std::string str = "");

    std::vector<std::string> text_;
    std::vector<std::string> regex_keywords_;
    std::vector<std::vector<uint64_t>> high_pos_;

    std::vector<std::pair<uint64_t, uint64_t>> mark_pos_;

    std::string filename_;
    std::string file_ext_;

    bool filename_input_;
    bool syntax_high_;

    uint64_t line_;
    uint64_t pos_;
};




#endif
