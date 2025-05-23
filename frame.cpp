#include "frame.hpp"
#include "editor.hpp"

#include <iostream>
#include <string>
#include <wx/wx.h>
#include <wx/spinctrl.h>

MainFrame::MainFrame(const wxString& title)
    : wxFrame(nullptr, wxID_ANY, title) {
    panel_ = new wxScrolledWindow(this, wxID_ANY);
    panel_->SetScrollRate(offset, pixel_height);
    panel_->SetBackgroundStyle(wxBG_STYLE_PAINT);
    panel_->SetFocus();

    //CreateStatusBar();

    panel_->Bind(wxEVT_CHAR, &MainFrame::on_key_input, this);
    panel_->Bind(wxEVT_KEY_DOWN, &MainFrame::on_keydown, this);
    panel_->Bind(wxEVT_PAINT, &MainFrame::on_paint, this);
}


void MainFrame::on_key_input(wxKeyEvent& event) {
    auto key = event.GetKeyCode();
    //std::cout << key << std::endl;
    if (key >= 32 && key <= 126) {
        editor_.write_text(char(event.GetKeyCode()));
    }
    Refresh();
}


void MainFrame::on_keydown(wxKeyEvent& event) {
    //std::cout << event.GetKeyCode() << std::endl;
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
    Refresh();
    event.Skip();
}


void MainFrame::on_paint([[maybe_unused]] wxPaintEvent& event) {
    wxAutoBufferedPaintDC dc(panel_);
    panel_->DoPrepareDC(dc);
    render(dc);
}


void MainFrame::render(wxAutoBufferedPaintDC& dc) {
    dc.SetFont(wxFontInfo(10).Family(wxFONTFAMILY_TELETYPE));
    int max_width = 0;
    uint32_t row = 0;

    for (std::string str : editor_.get_text()) {
        wxSize str_size = dc.GetTextExtent(wxString(str));
        if (str_size.GetWidth() > max_width) max_width = str_size.GetWidth();
        draw(dc, str, row);
        row++;
    }

    draw_pos(dc);

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

    dc.DrawText(
        position,
        static_cast<wxCoord>(offset + line_till_pos_size.GetWidth()),
        static_cast<wxCoord>(editor_.get_line() * pixel_height + offset)
    );

}





