#ifndef FRAME_HPP
#define FRAME_HPP

#include "editor.hpp"

#include <wx/wx.h>
#include <wx/dcbuffer.h>
#include <wx/scrolwin.h>


class MainFrame : public wxFrame {
public:
    MainFrame(const wxString& title, std::string filename = "");
private:
    void on_key_input(wxKeyEvent& event);
    void on_keydown(wxKeyEvent& event);
    void on_paint(wxPaintEvent& dc);

    void render(wxAutoBufferedPaintDC& dc);
    void draw(wxAutoBufferedPaintDC& dc, std::string str, uint32_t row);
    void draw_pos(wxAutoBufferedPaintDC& dc);

    wxScrolledWindow* panel_;
    Editor editor_;

    int pixel_height = 15;
    int offset = 15;
};

#endif
