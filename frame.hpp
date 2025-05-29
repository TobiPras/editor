#ifndef FRAME_HPP
#define FRAME_HPP

#include "editor.hpp"

#include <wx/wx.h>
#include <wx/dcbuffer.h>
#include <wx/scrolwin.h>
#include <wx/clipbrd.h>
#include <wx/dataobj.h>


class MainFrame : public wxFrame {
public:
    MainFrame(const wxString& title, std::string filename = "");
private:
    void on_key_input(wxKeyEvent& event);
    void on_keydown(wxKeyEvent& event);
    void on_paint(wxPaintEvent& event);
    void on_left_click(wxMouseEvent& event);
    void on_left_release(wxMouseEvent& event);
    void on_mouse_move(wxMouseEvent& event);

    void render(wxAutoBufferedPaintDC& dc);
    void draw_text(wxAutoBufferedPaintDC& dc, char chr, uint32_t row, uint32_t pos);
    void draw_mark(wxAutoBufferedPaintDC& dc, uint32_t row, uint32_t pos);
    void draw_pos(wxAutoBufferedPaintDC& dc);

    void copy_text();
    void paste_text();
    void delete_selected();

    std::pair<int, int> cursor_to_pos(wxPoint point);
    std::pair<std::pair<uint32_t, uint32_t>, std::pair<uint32_t, uint32_t>> start_end_check();

    wxScrolledWindow* panel_;
    wxStatusBar *status_bar_;
    Editor editor_;

    int text_width_ = -1;
    int text_height_ = -1;
    int pixel_height = 20;
    int offset = 15;

    bool mark_mode_ = false;
};

#endif
