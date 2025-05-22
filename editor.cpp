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
