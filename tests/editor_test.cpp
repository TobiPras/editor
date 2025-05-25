#include "catch_amalgamated.hpp"
#include "../editor.hpp"

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
        ed.set_pos(5);
        ed.string_from_pos();
        CHECK(ed.get_text().size() == 2);
        CHECK(ed.get_line() == 1);
        CHECK(ed.get_pos() == 0);
        CHECK(ed.get_text()[0] == "Hello");
        CHECK(ed.get_text()[1] == "World!");

        ed.set_pos(5);
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
        ed.set_line(1);
        CHECK(ed.get_line() == 1);
        CHECK(ed.get_pos() == 4);
        ed.set_line(2);
        CHECK(ed.get_line() == 2);
        CHECK(ed.get_text()[2] == "comes");
    }

    SECTION("set ilegal line") {
        REQUIRE(ed.get_text().size() == 4);
        ed.set_line(1001);
        CHECK(ed.get_line() == 3);
        ed.set_line(-1);
        CHECK(ed.get_line() == 0);
    }

    SECTION("set legit pos") {
        REQUIRE(ed.get_text()[ed.get_line()] == "santa");
        REQUIRE(ed.get_pos() == 5);
        ed.set_pos(2);
        CHECK(ed.get_pos() == 2);
        ed.set_pos(3);
        CHECK(ed.get_pos() == 3);
        CHECK(ed.get_text()[ed.get_line()][3] == 't');
    }


    SECTION("set ilegal pos") {
        REQUIRE(ed.get_text()[ed.get_line()].size() == 5);
        ed.set_pos(999);
        CHECK(ed.get_pos() == 5);
        ed.set_pos(-1);
        CHECK(ed.get_pos() == 0);
    }
}


TEST_CASE("Moving with arrow keys", "[Move]") {

}
