#ifndef FRAME_HPP
#define FRAME_HPP

#include "editor.hpp"

#include <wx/wx.h>
#include <wx/dcbuffer.h>


class MainFrame : public wxFrame {
public:
    MainFrame(const wxString& title);
private:
    void on_key_input(wxKeyEvent& event);
    void on_keydown(wxKeyEvent& event);
    void on_paint(wxPaintEvent& dc);

    void render(wxAutoBufferedPaintDC& dc);
    void draw(wxAutoBufferedPaintDC& dc, std::string str, uint32_t row);
    void draw_pos(wxAutoBufferedPaintDC& dc);

    wxPanel* panel_;
    Editor editor_;

    uint32_t pixel_height = 15;
    uint32_t offset = 5;
};

#endif
