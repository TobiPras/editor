#include "editor.hpp"

void Editor::write_text(char chr) {
    text_[line_].push_back(chr);
    pos_++;
}


void Editor::delete_text() {
    if (text_[line_].empty()) {
        text_.pop_back();
        line_--;
        return ;
    }

    text_[line_].pop_back();
    pos_--;
    std::cout << text_[line_] << std::endl;
}


void Editor::line_break() {
    text_.push_back("");
    line_++;
    pos_ = 0;
}


void Editor::move(Direction direction) {
    switch (direction) {
        case Direction::left:
            if (pos_ > 0) pos_--;
            else if (line_ > 0) {
                line_--;
                pos_ = text_[line_].size();
            }
            break;
        case Direction::up:
            if (line_ > 0) line_--;
            break;
        case Direction::right:
            if (pos_ < text_[line_].size() - 1) pos_++;
            else line_break();
            break;
        case Direction::down:
            if (line_ < text_.size() - 1) line_++;
            else line_break();
            break;
    }
}



