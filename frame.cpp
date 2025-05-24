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
    }
    if (!editor_.is_filename_input()) {
        switch(event.GetKeyCode()) {
            case 8:
                editor_.delete_text();
                break;
            case 13:
                editor_.string_from_pos();
                break;
            case 314:
                editor_.move(Direction::left);
                break;
            case 315:
                editor_.move(Direction::up);
                break;
            case 316:
                editor_.move(Direction::right);
                break;
            case 317:
                editor_.move(Direction::down);
                break;
        }
    }
    Refresh();
    event.Skip();
}


void MainFrame::on_paint([[maybe_unused]] wxPaintEvent& event) {
    wxAutoBufferedPaintDC dc(panel_);
    panel_->DoPrepareDC(dc);
    dc.SetFont(wxFontInfo(11).Family(wxFONTFAMILY_TELETYPE));
    dc.GetTextExtent("a", &text_width_, &text_height_);
    render(dc);
}


void MainFrame::on_left_click(wxMouseEvent& event) {
    wxPoint click = event.GetPosition();
    if (text_width_ >= 0 && text_height_ >= 0) {
        int pos = (click.x - offset + text_width_ / 2) / text_width_;
        int line = (click.y - offset) / pixel_height;
        editor_.set_line(line);
        editor_.set_pos(pos);
        Refresh();
    }
}


void MainFrame::render(wxAutoBufferedPaintDC& dc) {
    dc.SetTextForeground(wxColour(255, 255, 255));

    int max_width = 0;
    uint32_t row = 0;

    draw_pos(dc);
    dc.SetTextForeground(wxColour(220, 220, 220));

    for (std::string str : editor_.get_text()) {
        wxSize str_size = dc.GetTextExtent(wxString(str));
        if (str_size.GetWidth() > max_width) max_width = str_size.GetWidth();
        draw(dc, str, row);
        row++;
    }

    panel_->SetVirtualSize(max_width + 2 * offset, row * pixel_height + 2 * offset);
}


void MainFrame::draw(wxAutoBufferedPaintDC& dc, std::string str, uint32_t row) {
    wxString line(str);
    dc.DrawText(line, offset, row * pixel_height + offset);
}


void MainFrame::draw_pos(wxAutoBufferedPaintDC& dc) {
    std::string line = editor_.get_text()[editor_.get_line()];
    wxString line_till_pos(line.substr(0, editor_.get_pos()));
    wxSize line_till_pos_size = dc.GetTextExtent(line_till_pos);

    wxString position("|");

    dc.DrawText(position, (int)(offset + line_till_pos_size.GetWidth() - 4), (int)(editor_.get_line() * pixel_height + offset - 1));
}





