#include "catch_amalgamated.hpp"
#include "../editor.hpp"
#include <cstdio>

TEST_CASE("Editor initializes correctly", "[Editor]") {
    Editor ed;
    REQUIRE(ed.get_text().size() == 1);
    REQUIRE(ed.get_text()[0] == "");
    REQUIRE(ed.get_line() == 0);
    REQUIRE(ed.get_pos() == 0);
    REQUIRE(ed.is_filename_input() == false);
    REQUIRE(ed.get_filename().empty());

    SECTION("set filename") {
        ed.set_filename("hello.txt");
        REQUIRE(ed.get_filename() == "hello.txt");
    }

    SECTION("write and delete filename characters after open") {
        std::string fn = "world.txt";
        for (char chr : fn) {
            ed.write_filename(chr);
        }
        REQUIRE(ed.get_filename() == "world.txt");
        for (int i = 0; i < 4; i++) {
            ed.delete_filename();
        }
        REQUIRE(ed.get_filename() == "world");
    }
}


TEST_CASE("Write text", "[Write]") {
    Editor ed;

    SECTION("write characters") {
        ed.write_text('x');
        ed.write_text('y');
        ed.write_text('z');
        CHECK(ed.get_line() == 0);
        CHECK(ed.get_pos() == 3);
        CHECK(ed.get_text()[0] == "xyz");
    }
}


TEST_CASE("Make Newlines", "[Newline]") {
    Editor ed;

    SECTION("normal newline") {
        REQUIRE(ed.get_text().size() == 1);
        ed.string_from_pos();
        CHECK(ed.get_text().size() == 2);
        CHECK(ed.get_line() == 1);
        CHECK(ed.get_pos() == 0);
        CHECK(ed.get_text()[1] == "");
    }

    std::string l1 = "HelloWorld!";
    for (char chr : l1) {
        ed.write_text(chr);
    }

    SECTION("newline in text") {
        REQUIRE(ed.get_text()[0] == "HelloWorld!");
        ed.set_pos({ed.get_line(), 5});
        ed.string_from_pos();
        CHECK(ed.get_text().size() == 2);
        CHECK(ed.get_line() == 1);
        CHECK(ed.get_pos() == 0);
        CHECK(ed.get_text()[0] == "Hello");
        CHECK(ed.get_text()[1] == "World!");

        ed.set_pos({ed.get_line(), 5});
        ed.string_from_pos();
        CHECK(ed.get_text().size() == 3);
        CHECK(ed.get_line() == 2);
        CHECK(ed.get_pos() == 0);
        CHECK(ed.get_text()[0] == "Hello");
        CHECK(ed.get_text()[1] == "World");
        CHECK(ed.get_text()[2] == "!");
    }
}


TEST_CASE("Delete text", "[Delete]") {
    Editor ed;
    std::string l1 = "Hello";
    std::string l2 = "you";
    for (char chr : l1) {
        ed.write_text(chr);
    }
    ed.string_from_pos();
    for (char chr : l2) {
        ed.write_text(chr);
    }

    SECTION("delete character") {
        REQUIRE(ed.get_line() == 1);
        REQUIRE(ed.get_pos() == 3);
        REQUIRE(ed.get_text()[1] == "you");
        ed.delete_text();
        CHECK(ed.get_text()[1] == "yo");
        ed.delete_text();
        CHECK(ed.get_text()[1] == "y");
        ed.delete_text();
        CHECK(ed.get_text()[1] == "");
    }

    SECTION("delete empty line") {
        ed.string_from_pos();
        REQUIRE(ed.get_text().size() == 3);
        REQUIRE(ed.get_line() == 2);
        ed.delete_text();
        CHECK(ed.get_line() == 1);
        CHECK(ed.get_pos() == 3);
        CHECK(ed.get_text()[1] == "you");
    }
}


TEST_CASE("Set line and pos", "[Set]") {
    Editor ed;
    std::string l1 = "Hohoho";
    std::string l2 = "here";
    std::string l3 = "comes";
    std::string l4 = "santa";
    std::vector<std::string> lines {l1, l2, l3, l4};

    for (std::string l : lines) {
        for (char chr : l) {
            ed.write_text(chr);
        }
        ed.string_from_pos();
    }
    ed.delete_text();

    SECTION("set legit line") {
        REQUIRE(ed.get_line() == 3);
        REQUIRE(ed.get_pos() == 5);
        ed.set_pos({1, ed.get_pos()});
        CHECK(ed.get_line() == 1);
        CHECK(ed.get_pos() == 4);
        ed.set_pos({2, ed.get_pos()});
        CHECK(ed.get_line() == 2);
        CHECK(ed.get_text()[2] == "comes");
    }

    SECTION("set ilegal line") {
        REQUIRE(ed.get_text().size() == 4);
        ed.set_pos({1001, ed.get_pos()});
        CHECK(ed.get_line() == 3);
        ed.set_pos({-1, ed.get_pos()});
        CHECK(ed.get_line() == 0);
    }

    SECTION("set legit pos") {
        REQUIRE(ed.get_text()[ed.get_line()] == "santa");
        REQUIRE(ed.get_pos() == 5);
        ed.set_pos({ed.get_line(), 2});
        CHECK(ed.get_pos() == 2);
        ed.set_pos({ed.get_line(), 3});
        CHECK(ed.get_pos() == 3);
        CHECK(ed.get_text()[ed.get_line()][3] == 't');
    }

    SECTION("set ilegal pos") {
        REQUIRE(ed.get_text()[ed.get_line()].size() == 5);
        ed.set_pos({ed.get_line(), 999});
        CHECK(ed.get_pos() == 5);
        ed.set_pos({ed.get_line(), -1});
        CHECK(ed.get_pos() == 0);
    }
}


TEST_CASE("Move with arrow keys", "[Move]") {
    Editor ed;

    SECTION("move left without text") {
        ed.move(Direction::left);
        CHECK(ed.get_text().size() == 1);
        CHECK(ed.get_line() == 0);
        CHECK(ed.get_pos() == 0);
        CHECK(ed.get_text()[0] == "");
    }

    SECTION("move up without text") {
        ed.move(Direction::up);
        CHECK(ed.get_text().size() == 1);
        CHECK(ed.get_line() == 0);
        CHECK(ed.get_pos() == 0);
        CHECK(ed.get_text()[0] == "");
    }

    SECTION("move right without text") {
        ed.move(Direction::right);
        CHECK(ed.get_text().size() == 2);
        CHECK(ed.get_line() == 1);
        CHECK(ed.get_pos() == 0);
        CHECK(ed.get_text()[1] == "");
    }

    SECTION("move down without text") {
        ed.move(Direction::down);
        CHECK(ed.get_text().size() == 2);
        CHECK(ed.get_line() == 1);
        CHECK(ed.get_pos() == 0);
        CHECK(ed.get_text()[1] == "");
    }

    std::string l1 = "#include <iostream>";
    std::string l2 = "";
    std::string l3 = "int main() {";
    std::string l4 = "  std::cout << \"Hello World!\" << std::endl;";
    std::string l5 = "}";
    std::vector<std::string> lines {l1, l2, l3, l4, l5};

    for (std::string l : lines) {
        for (char chr : l) {
            ed.write_text(chr);
        }
        ed.string_from_pos();
    }
    ed.delete_text();

    SECTION("move left with text") {
        REQUIRE(ed.get_line() == 4);
        REQUIRE(ed.get_pos() == 1);
        ed.move(Direction::left);
        CHECK(ed.get_line() == 4);
        CHECK(ed.get_pos() == 0);
        ed.move(Direction::left);
        CHECK(ed.get_line() == 3);
        CHECK(ed.get_pos() == 43);
        CHECK(ed.get_text()[ed.get_line()] == l4);
    }

    SECTION("move up with text") {
        REQUIRE(ed.get_line() == 4);
        REQUIRE(ed.get_pos() == 1);
        ed.move(Direction::up);
        CHECK(ed.get_line() == 3);
        CHECK(ed.get_pos() == 1);
        CHECK(ed.get_text()[ed.get_line()] == l4);
        ed.move(Direction::up);
        CHECK(ed.get_line() == 2);
        CHECK(ed.get_pos() == 1);
        CHECK(ed.get_text()[ed.get_line()] == l3);
        ed.move(Direction::up);
        CHECK(ed.get_line() == 1);
        CHECK(ed.get_pos() == 0);
        CHECK(ed.get_text()[ed.get_line()] == l2);
    }

    SECTION("move right with text") {
        REQUIRE(ed.get_text().size() == 5);
        REQUIRE(ed.get_line() == 4);
        REQUIRE(ed.get_pos() == 1);
        ed.move(Direction::right);
        CHECK(ed.get_text().size() == 6);
        CHECK(ed.get_line() == 5);
        CHECK(ed.get_pos() == 0);
        CHECK(ed.get_text()[ed.get_line()] == "");
        ed.set_pos({0, 0});
        REQUIRE(ed.get_line() == 0);
        REQUIRE(ed.get_pos() == 0);
        ed.move(Direction::right);
        CHECK(ed.get_text().size() == 6);
        CHECK(ed.get_line() == 0);
        CHECK(ed.get_pos() == 1);
        CHECK(ed.get_text()[ed.get_line()] == l1);
        ed.set_pos({ed.get_line(), 1000});
        REQUIRE(ed.get_pos() == ed.get_text()[ed.get_line()].size());
        ed.move(Direction::right);
        CHECK(ed.get_text().size() == 6);
        CHECK(ed.get_line() == 1);
        CHECK(ed.get_pos() == 0);
        CHECK(ed.get_text()[ed.get_line()] == l2);
    }

    SECTION("move down without text") {
        REQUIRE(ed.get_text().size() == 5);
        REQUIRE(ed.get_line() == 4);
        REQUIRE(ed.get_pos() == 1);
        ed.move(Direction::down);
        CHECK(ed.get_text().size() == 6);
        CHECK(ed.get_line() == 5);
        CHECK(ed.get_pos() == 0);
        CHECK(ed.get_text()[ed.get_line()] == "");
        ed.set_pos({1, 0});
        REQUIRE(ed.get_line() == 1);
        REQUIRE(ed.get_pos() == 0);
        ed.move(Direction::down);
        CHECK(ed.get_text().size() == 6);
        CHECK(ed.get_line() == 2);
        CHECK(ed.get_pos() == 0);
        CHECK(ed.get_text()[ed.get_line()] == l3);
        ed.set_pos({ed.get_line(), 3});
        REQUIRE(ed.get_pos() == 3);
        ed.move(Direction::down);
        CHECK(ed.get_text().size() == 6);
        CHECK(ed.get_line() == 3);
        CHECK(ed.get_pos() == 3);
        CHECK(ed.get_text()[ed.get_line()] == l4);
    }
}


TEST_CASE("Editor saves and loads correctly", "[Save/Load]") {
    Editor ed;
    std::string temp_filename = "test.txt";

    SECTION("save and load without filename") {
        ed.write_text('H');
        ed.write_text('i');
        ed.save_file();
        REQUIRE(!ed.is_filename_input());
        for (char chr : temp_filename) {
            ed.write_filename(chr);
        }
        ed.save_file();
        REQUIRE(ed.get_text().size() == 1);

        Editor ed2;
        ed2.set_filename(temp_filename);
        CHECK(ed2.get_text().size() == 1);
        CHECK(ed2.get_line() == 0);
        CHECK(ed2.get_pos() == 0);
        CHECK(ed2.get_text()[0] == "Hi");
        std::remove(temp_filename.c_str());
    }

    std::string l1 = "#include <iostream>";
    std::string l2 = "";
    std::string l3 = "int main() {";
    std::string l4 = "  std::cout << \"!Dlrow Olleh\" << std::endl;";
    std::string l5 = "}";
    std::vector<std::string> lines {l1, l2, l3, l4, l5};

    for (std::string l : lines) {
        for (char chr : l) {
            ed.write_text(chr);
        }
        ed.string_from_pos();
    }
    ed.delete_text();

    SECTION("save and load with filename(+ more lines)") {
        ed.set_filename(temp_filename);
        ed.save_file();
        REQUIRE(ed.get_text().size() == 5);

        Editor ed2;
        ed2.set_filename(temp_filename);
        CHECK(ed2.get_text().size() == 5);
        CHECK(ed2.get_line() == 0);
        CHECK(ed2.get_pos() == 0);
        CHECK(ed2.get_text()[0] == l1);
        CHECK(ed2.get_text()[1] == l2);
        CHECK(ed2.get_text()[2] == l3);
        CHECK(ed2.get_text()[3] == l4);
        CHECK(ed2.get_text()[4] == l5);
        std::remove(temp_filename.c_str());
    }
}


TEST_CASE("Filename input logic", "[FilenameInput]") {
    Editor ed;
    CHECK(!ed.is_filename_input());

    ed.set_input_filename();
    CHECK(ed.is_filename_input());

    ed.clear_filename();
    CHECK(ed.get_filename().empty());
}


TEST_CASE("Syntax highlighting", "[Highlighting]") {
    Editor ed;
    ed.set_filename("test.cpp");
    ed.set_keywords_regex();

    std::string l1 = "int main() { // hahaha";
    std::string l2 = "\"hello dear\";";
    std::string l3 = "char c = 'x';";
    std::vector<std::string> lines {l1, l2, l3};

    for (std::string l : lines) {
        for (char chr : l) {
            ed.write_text(chr);
        }
        ed.string_from_pos();
    }
    ed.delete_text();

    REQUIRE(!ed.get_high_pos().empty());

    SECTION("Comment detection") {
        bool found = false;
        for (std::vector<uint64_t> pos : ed.get_high_pos()) {
            if (pos[0] == 0 && pos[1] == 13 && pos[2] == ed.get_text()[0].size() - 1 && pos[3] == 0) {
                found = true;
                break;
            }
        }
        CHECK(found);
    }

    SECTION("Double quotes detection") {
        bool found = false;
        for (std::vector<uint64_t> pos : ed.get_high_pos()) {
            if (pos[0] == 1 && pos[1] == 0 && pos[2] == ed.get_text()[1].size() - 2 && pos[3] == 1) {
                found = true;
                break;
            }
        }
        CHECK(found);
    }

    SECTION("keyword detection") {
        bool found = false;
        for (std::vector<uint64_t> pos : ed.get_high_pos()) {
            if (pos[0] == 2 && pos[1] == 0 && pos[2] == 3 && pos[3] == 3) {
                found = true;
                break;
            }
        }
        CHECK(found);
    }

    SECTION("Single quotes detection") {
        bool found = false;
        for (std::vector<uint64_t> pos : ed.get_high_pos()) {
            if (pos[0] == 2 && pos[1] == 9 && pos[2] == 11 && pos[3] == 2) {
                found = true;
                break;
            }
        }
        CHECK(found);
    }
}


