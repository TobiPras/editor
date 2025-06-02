# Editor
A lightweight, cross-platform text editor built from scratch using C++ and wxWidgets. This project showcases a custom editor with keyboard navigation, text manipulation, C++ syntax highlighting, and basic file operations.

## Compile and Run the Project

### Compile
```shell
mkdir build
cd build
cmake ..
make -j8
```

### Run Application
```shell
./text_editor
```

### Run Tests
```shell
./editor_tests
```

## Features
- **Keyboard Navigation**
  - Arrow keys
- **Syntax Highlighting**
  - Supports C++-style:
    - Keywords
    - Strings: `"..."` and `'...'`
    - Comments: `//`
- **Text Editing**
  - Type, delete and select.
  - Mouse-based selection and cursor positioning.
- **File Handling**
  - `Ctrl+S`: Save (prompts for filename if none set).
  - `Ctrl+Shift+S`: Save (always prompts for filename).
  - `Ctrl+O`: Open file (always prompts for filename).
- **Clipboard Support**
  - `Ctrl+C`, `Ctrl+V`, `Ctrl+X

## License
This project is licensed under the MIT License – see the LICENSE for details.
