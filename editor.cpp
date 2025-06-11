#include "editor.hpp"


Editor::Editor() {
    text_.push_back("");
    filename_input_ = false;
    syntax_high_ = false;
    line_ = 0;
    pos_ = 0;
}

void Editor::set_filename(std::string filename) {
    filename_ = filename;
    set_extention();
    load_file();
    filename_input_ = false;
}


void Editor::delete_filename() {
    if (!filename_.empty()) {
        filename_.pop_back();
    }
}


void Editor::set_extention() {
    if (!filename_.empty()) {
        std::string ext;
        bool end = false;
        for (uint64_t i = 0; i < filename_.size(); i++) {
            if (end && filename_[i] == '.') ext.clear();
            else if (filename_[i] == '.') end = true;
            if (end) {
                ext.push_back(filename_[i]);
            }
        }
        if (!ext.empty()) {
            file_ext_ = ext;
            if (ext == ".cpp") {
                set_keywords_regex();
                syntax_high_ = true;
            }
        }
    }
}


void Editor::load_file() {
    syntax_high_ = false;
    set_extention();
    high_pos_.clear();
    std::ifstream in(filename_);
    text_.clear();
    if (!in.is_open()) {
        text_.push_back("");
        return ;
    }

    std::string line;
    while (std::getline(in, line)) {
        text_.push_back(line);
    }
    in.close();
    if (text_.empty()) text_.push_back("");
    if (syntax_high_) syntax_high();
}


void Editor::set_pos(std::pair<int, int> pos) {
    line_ = pos.first;
    pos_ = pos.second;
    if (pos.first < 0) line_ = 0;
    else if (line_ >= text_.size()) line_ = text_.size() - 1;

    if (pos.second < 0) pos_ = 0;
    else if (pos_ > text_[line_].size()) pos_ = text_[line_].size();
}


void Editor::set_start_mark(std::pair<int, int> pos) {
    mark_pos_.clear();
    mark_pos_.push_back(pos);
    mark_pos_.push_back(pos);
}


void Editor::set_end_mark(std::pair<int, int> pos) {
    mark_pos_[1] = pos;
}


void Editor::save_file() {
    set_extention();
    create_file();
    filename_input_ = false;
}


void Editor::create_file() {
    std::ofstream out(filename_);

    for (const auto& line : text_) {
        out << line << '\n';
    }
    out.close();
}


void Editor::write_text(char chr) {
    std::string& line = text_[line_];
    if (pos_ == line.size()) {
       line.push_back(chr);
    } else {
        line.insert(line.begin() + pos_, chr);
    }
    pos_++;
    if (syntax_high_) syntax_high();
}


void Editor::delete_text() {
    if (text_.size() == 1) {
        if (pos_ > 0) {
            text_[line_].erase(pos_ - 1, 1);
            pos_--;
            if (syntax_high_) syntax_high();
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
    if (syntax_high_) syntax_high();
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


void Editor::set_keywords_regex() {
    regex_keywords_.push_back("//.*");
    regex_keywords_.push_back("\"(\\\\.|[^\"])*\"");
    regex_keywords_.push_back("'(\\\\.|[^'])*'");

    uint64_t offset = regex_keywords_.size();
    for (uint64_t type = 0; type < keywords.size(); type++) {
        regex_keywords_.push_back("\\b(");
        for (uint64_t i = 0; i < keywords[type].size(); i++) {
            regex_keywords_[offset + type] += keywords[type][i];
            if (i != keywords[type].size() - 1) regex_keywords_[offset + type] += "|";
        }
        regex_keywords_[offset + type] += ")\\b";
    }
}


void Editor::syntax_high() {
    if (regex_keywords_.empty() || !syntax_high_) return;
    high_pos_.clear();
    for (uint64_t type = 0; type < regex_keywords_.size(); type++) {
        std::regex regex(regex_keywords_[type]);
        std::smatch match;

        for (uint64_t line = 0; line < text_.size(); line++) {
            auto start = text_[line].cbegin();
            uint64_t pos = 0;
            while (std::regex_search(start, text_[line].cend(), match, regex)) {
                uint64_t length = match.length();
                if (length == 0) break;
                high_pos_.push_back({line, pos + match.position(), pos + match.position() + match.length() - 1, type});
                pos += match.position() + match.length();
                start = match.suffix().first;
            }
        }
    }
}





