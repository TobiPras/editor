#include "editor.hpp"


void Editor::set_filename(std::string filename) {
    filename_ = filename;
    load_file();
}


void Editor::load_file() {
    std::ifstream in(filename_);
    if (!in.is_open()) {
        text_.push_back("");
        return ;
    }

    std::string line;
    while (std::getline(in, line)) {
        text_.push_back(line);
    }
    in.close();
}


void Editor::set_line(uint64_t line) {
    line_ = line;
    if (line_ >= text_.size()) {
        line_ = text_.size() - 1;
    }
}


void Editor::set_pos(uint64_t pos) {
    pos_ = pos;
    if (pos_ >= text_[line_].size()) {
        pos_ = text_[line_].size();
    }
}


void Editor::save_file() {
    if (filename_.empty()) {
        filename_input_ = true;
        return ;
    }
    create_file();
}


void Editor::create_file() {
    std::ofstream out(filename_);
    if (!out.is_open()) {
        std::cerr << "Failed to create file: " << filename_ << std::endl;
        return;
    }

    for (const auto& line : text_) {
        out << line << '\n';
    }
    out.close();
    filename_input_ = false;
}


void Editor::write_text(char chr) {
    std::string& line = text_[line_];
    if (pos_ == line.size()) {
       line.push_back(chr);
    } else {
        line.insert(line.begin() + pos_, chr);
    }
    pos_++;
}


void Editor::delete_text() {
    if (text_.size() == 1) {
        if (pos_ > 0) {
            text_[line_].erase(pos_ - 1, 1);
            pos_--;
        }
        return ;
    }

    if (pos_ == 0) {
        pos_ = text_[line_ - 1].size();
        text_[line_ - 1].append(text_[line_]);
        text_.erase(text_.begin() + line_);
        line_--;
        return ;
    }

    text_[line_].erase(pos_ - 1, 1);
    pos_--;
}


void Editor::new_line(std::string str) {
    if (line_ < text_.size() - 1) {
        text_.insert(text_.begin() + line_ + 1, str);
    } else {
        text_.push_back(str);
    }
    line_++;
    pos_ = 0;
}


void Editor::string_from_pos() {
    std::string sub = "";
    if (pos_ <= text_[line_].size()) {
        sub = text_[line_].substr(pos_);
        text_[line_].erase(pos_);
    }
    new_line(sub);
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
            if (pos_ > text_[line_].size()) pos_ = text_[line_].size();
            break;
        case Direction::right:
            if (pos_ < text_[line_].size()) pos_++;
            else if (line_ == text_.size() - 1) {
                new_line();
            } else {
                line_++;
                pos_ = 0;
            }
            break;
        case Direction::down:
            if (line_ == text_.size() - 1) {
                new_line();
            } else if (line_ < text_.size()) line_++;
            if (pos_ > text_[line_].size()) pos_ = text_[line_].size();
            break;
    }
}



