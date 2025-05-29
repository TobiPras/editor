#include "frame.hpp"
#include "editor.hpp"

#include <iostream>
#include <string>
#include <wx/wx.h>
#include <wx/spinctrl.h>

MainFrame::MainFrame(const wxString& title, std::string filename)
    : wxFrame(nullptr, wxID_ANY, title) {
    panel_ = new wxScrolledWindow(this, wxID_ANY);
    panel_->SetScrollRate(offset, pixel_height);
    panel_->SetBackgroundStyle(wxBG_STYLE_PAINT);
    panel_->SetCursor(wxCURSOR_IBEAM);
    panel_->SetFocus();

    status_bar_ = CreateStatusBar();
    status_bar_->SetBackgroundColour(wxColour(35, 35, 35));

    panel_->Bind(wxEVT_CHAR, &MainFrame::on_key_input, this);
    panel_->Bind(wxEVT_KEY_DOWN, &MainFrame::on_keydown, this);
    panel_->Bind(wxEVT_PAINT, &MainFrame::on_paint, this);
    panel_->Bind(wxEVT_LEFT_DOWN, &MainFrame::on_left_click, this);
    panel_->Bind(wxEVT_LEFT_UP, &MainFrame::on_left_release, this);
    panel_->Bind(wxEVT_MOTION, &MainFrame::on_mouse_move, this);

    editor_.set_filename(filename);
}


void MainFrame::on_key_input(wxKeyEvent& event) {
    char key = char(event.GetKeyCode());
    //std::cout << key << std::endl;
    if (editor_.is_filename_input()) {
        if (key == 13) {
            editor_.save_file();
        } else if (key == 8) {
            editor_.delete_filename();
        } else if (key >= 32 && key <= 126) {
            editor_.write_filename(key);
        }
        if (editor_.is_filename_input()) {
            std::string str = "filename: " + editor_.get_filename();
            status_bar_->SetStatusText(str);
        } else status_bar_->SetStatusText("");
    } else if (key >= 32 && key <= 126) {
        editor_.write_text(key);
    }
    Refresh();
}


void MainFrame::on_keydown(wxKeyEvent& event) {
    //std::cout << event.GetKeyCode() << std::endl;
    if (event.GetKeyCode() == 'S' && event.ControlDown()) {
        editor_.save_file();
        if (editor_.is_filename_input()) {
            status_bar_->SetStatusText("filename: ");
        }
        return;
    } else if (event.GetKeyCode() == 'C' && event.ControlDown()) copy_text();
    else if (event.GetKeyCode() == 'V' && event.ControlDown()) paste_text();

    bool arrow = false;
    if (!editor_.is_filename_input()) {
        switch(event.GetKeyCode()) {
            case 8:
                editor_.delete_text();
                break;
            case 9:
                for (int i = 0; i < 4; i++) editor_.write_text(32);
                break;
            case 13:
                editor_.string_from_pos();
                break;
            case 314:
                editor_.move(Direction::left);
                arrow = true;
                break;
            case 315:
                editor_.move(Direction::up);
                arrow = true;
                break;
            case 316:
                editor_.move(Direction::right);
                arrow = true;
                break;
            case 317:
                editor_.move(Direction::down);
                arrow = true;
                break;
        }
    }
    Refresh();
    if (!arrow) {
        event.Skip();
    }
}


void MainFrame::copy_text() {
    std::vector<std::string> text = editor_.get_text();
    std::string str;
    if (editor_.get_mark_pos().empty() || editor_.get_mark_pos().size() < 2) return ;
    auto [start, end] = start_end_check();
    for (uint32_t line = start.first; line <= end.first; line++) {
        for (uint32_t pos = 0; pos < text[line].size(); pos++) {
            if (line == end.first && pos > end.second) break;
            if (line == start.first && pos < start.second) continue;
            str.push_back(text[line][pos]);
        }
        if (line != end.first) str.push_back('\n');
    }

    if (wxTheClipboard->Open()) {
        wxTheClipboard->Clear();
        wxTheClipboard->SetData(new wxTextDataObject(str));
        wxTheClipboard->Close();
    }
}


void MainFrame::paste_text() {

}


void MainFrame::on_left_click(wxMouseEvent& event) {
    wxPoint click = event.GetPosition();
    if (text_width_ >= 0 && text_height_ >= 0) {
        std::pair<int, int> curs_pos = cursor_to_pos(click);
        editor_.set_pos(curs_pos);
        editor_.set_start_mark(curs_pos);
        mark_mode_ = true;
        Refresh();
    }
}


void MainFrame::on_mouse_move(wxMouseEvent& event) {
    wxPoint pos = event.GetPosition();
    if (text_width_ >= 0 && text_height_ >= 0 && mark_mode_) {
        std::pair<int, int> curs_pos = cursor_to_pos(pos);
        editor_.set_end_mark(curs_pos);
        Refresh();
    }
}


void MainFrame::on_left_release(wxMouseEvent& event) {
    wxPoint release = event.GetPosition();

    if (text_width_ >= 0 && text_height_ >= 0) {
        std::pair<int, int> curs_pos = cursor_to_pos(release);
        editor_.set_pos(curs_pos);
        editor_.set_end_mark(curs_pos);
        mark_mode_ = false;
        Refresh();
    }
}

std::pair<int, int> MainFrame::cursor_to_pos(wxPoint point) {
    std::pair<int, int> curs_pos;
    curs_pos.first = (point.y - offset) / pixel_height;
    curs_pos.second = (point.x - offset + text_width_ / 2) / text_width_;
    return curs_pos;
}


void MainFrame::on_paint([[maybe_unused]] wxPaintEvent& event) {
    wxAutoBufferedPaintDC dc(panel_);
    panel_->DoPrepareDC(dc);
    dc.SetFont(wxFontInfo(11).Family(wxFONTFAMILY_TELETYPE));
    dc.GetTextExtent("a", &text_width_, &text_height_);
    render(dc);
}


void MainFrame::render(wxAutoBufferedPaintDC& dc) {
    int max_width = 0;
    uint32_t row = 0;

    draw_pos(dc);

    dc.SetTextForeground(wxColour(220, 220, 220));
    for (std::string str : editor_.get_text()) {
        wxSize str_size = dc.GetTextExtent(wxString(str));
        if (str_size.GetWidth() > max_width) max_width = str_size.GetWidth();
        for (uint32_t i = 0; i < str.size(); i++) {
            draw_mark(dc, row, i);
            draw_text(dc, str[i], row, i);
        }
        row++;
    }

    panel_->SetVirtualSize(max_width + 2 * offset, row * pixel_height + 2 * offset);
}


void MainFrame::draw_text(wxAutoBufferedPaintDC& dc, char chr, uint32_t row, uint32_t pos) {
    dc.DrawText(chr, pos * text_width_ + offset, row * pixel_height + offset);
}


void MainFrame::draw_mark(wxAutoBufferedPaintDC& dc, uint32_t row, uint32_t pos) {
    if (editor_.get_mark_pos().empty() || editor_.get_mark_pos().size() < 2) return ;
    auto [start, end] = start_end_check();
    if ((row > start.first && row < end.first) ||
        (row == start.first && pos >= start.second && (start.first != end.first || pos < end.second)) ||
        (row == end.first && pos < end.second && start.first != end.first)) {
        dc.SetBrush(wxBrush(wxColour(0, 0, 128)));
        dc.SetPen(*wxTRANSPARENT_PEN);
        dc.DrawRectangle(pos * text_width_ + offset, row * pixel_height + offset, text_width_, pixel_height);
    }
}


void MainFrame::draw_pos(wxAutoBufferedPaintDC& dc) {
    dc.SetTextForeground(wxColour(255, 255, 255));
    std::string line = editor_.get_text()[editor_.get_line()];
    wxString line_till_pos(line.substr(0, editor_.get_pos()));
    wxSize line_till_pos_size = dc.GetTextExtent(line_till_pos);

    wxString position("|");

    dc.DrawText(position, (int)(offset + line_till_pos_size.GetWidth() - 4), (int)(editor_.get_line() * pixel_height + offset - 1));
}


std::pair<std::pair<uint32_t, uint32_t>, std::pair<uint32_t, uint32_t>> MainFrame::start_end_check() {
    std::pair<uint32_t, uint32_t> start = editor_.get_mark_pos()[0];
    std::pair<uint32_t, uint32_t> end = editor_.get_mark_pos()[1];
    if (start > end) {
        start = end;
        end = editor_.get_mark_pos()[0];
    }
    return {start, end};
}





