#ifndef EDITOR_HPP
#define EDITOR_HPP

#include <wx/wx.h>
#include <iostream>
#include <vector>
#include <string>
#include <fstream>


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
    void load_file();
    void save_file();
    void write_filename(char chr) { filename_.push_back(chr); }
    void delete_filename() { filename_.pop_back(); }
    void create_file();
    void start_syntax_high();

    void set_line(int line);
    void set_pos(int pos);

    std::vector<std::string> get_text() const { return text_; }

    std::string get_filename() const { return filename_; }

    uint64_t get_line() const { return line_; }
    uint64_t get_pos() const { return pos_; }

    bool is_filename_input() { return filename_input_; }
private:
    void new_line(std::string str = "");

    std::vector<std::string> text_;
    std::vector<std::vector<int>> high_pos_;

    std::string filename_;
    std::string file_ext_;

    bool filename_input_;
    bool syntax_high_;

    uint64_t line_;
    uint64_t pos_;
};




#endif
