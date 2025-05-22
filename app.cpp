#include "app.hpp"
#include "frame.hpp"

bool App::OnInit() {
  MainFrame* mainFrame = new MainFrame("Text Editor");
  mainFrame->SetClientSize(800, 600);
  mainFrame->Center();
  mainFrame->Show();
  return true;
}

wxIMPLEMENT_APP(App);
