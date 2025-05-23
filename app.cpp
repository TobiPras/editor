#include "app.hpp"
#include "frame.hpp"

bool App::OnInit() {
  std::string filename;
  if (argc > 1) {
    filename = argv[1];
  }
  MainFrame* mainFrame = new MainFrame("Text Editor", filename);
  mainFrame->SetClientSize(800, 600);
  mainFrame->Center();
  mainFrame->Show();
  return true;
}

wxIMPLEMENT_APP(App);
