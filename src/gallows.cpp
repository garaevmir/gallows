#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics.hpp>
#include <vector>
#include <string>
#include <ctime>
#include <direct.h>

const std::vector<std::string> easy_words = {
        "car", "dog", "cat", "book", "pen", "chair", "table", "phone", "sun", "moon",
        "bird", "fish", "tree", "flower", "rain", "snow", "apple", "banana", "grape", "peach",
        "blue", "red", "green", "yellow", "house", "milk", "bread", "water", "egg", "sand"
};

const std::vector<std::string> medium_words = {
        "computer", "tree", "flower", "river", "mountain", "baby", "bird", "fish", "friend", "teacher",
        "police", "doctor", "nurse", "artist", "actor", "singer", "writer", "scientist", "engineer", "student",
        "employee", "boss", "coworker", "library", "school", "university", "hospital", "office", "restaurant", "market"
};

const std::vector<std::string> hard_words = {
        "scientist", "engineer", "detective", "prisoner", "composer", "customer", "bicycle", "restaurant", "supermarket", "university",
        "economy", "politics", "history", "geography", "mathematics", "chemistry", "biology", "physics", "literature", "psychology",
        "philosophy", "architecture", "archaeology", "astrology", "anthropology", "cryptography", "biotechnology", "neuroscience", "nanotechnology", "quantum"
};

const std::vector<std::string> gall = {
        "|-----|\n\n|     |\n\n|     |\n\n|     |\n\n|     \n\n|-----|\n\n", "|-----|\n\n|     |\n\n|     |\n\n|     |\n\n|    /|\n\n|-----|\n\n",
        "|-----|\n\n|     |\n\n|     |\n\n|     |\n\n|    /|\\\n\n|-----|\n\n" ,"|-----|\n\n|     |\n\n|     |\n\n|     |\n\n|    /|\\\n\n|    /\n\n|-----|\n\n"
        ,"|-----|\n\n|     |\n\n|     |\n\n|     |\n\n|    /|\\\n\n|    / \\\n\n|-----|\n\n",
        "|-----|\n\n|     |\n\n|    /|\\\n\n|    / \\\n\n|     \n\n|     \n\n|-----|\n\n"};

std::string get_current_word(const std::vector<char>& ready) {
    return std::string(ready.begin(), ready.end());
}

std::string get_current_mistakes(const std::vector<char>& mistakes) {
    return std::string(mistakes.begin(), mistakes.end());
}

std::string get_play_screen(const std::string& input, const std::vector<char>& ready, const std::vector<char>& mistakes) {
    return gall[(mistakes.size() < 5 ? mistakes.size() : 5)] + "Guess the letter: " + input + "\n\nCurrent word: " + get_current_word(ready) + "\n\nCurrent mistakes: " + get_current_mistakes(mistakes);
}

bool find_mistake(const char& letter, const std::vector<char>& mistakes) {
    return std::find(mistakes.begin(), mistakes.end(), letter) != mistakes.end();
}

void process_input(const std::string& guess, std::vector<char>& ready, std::vector<char>& mistakes, int& number_correct, int& number_mistakes, const std::string& word) {
    if (word.find(guess[0]) != std::string::npos) {
        for (size_t i = 0; i < word.size(); ++i) {
            if (word[i] == guess[0] && ready[i] != guess[0]) {
                ready[i] = guess[0];
                ++number_correct;
            }
        }
    } else {
        if (!find_mistake(guess[0], mistakes)) {
            mistakes.push_back(guess[0]);
            ++number_mistakes;
        }
    }
}

void choose_difficulty(sf::RenderWindow& window, std::vector<std::string>& selected_words, sf::Text& text) {
    int difficulty = 0;
    text.setString("Select difficulty:\n\n1 - Easy\n\n2 - Medium\n\n3 - Hard");
    text.setPosition(400, 400);

    while (window.isOpen() && difficulty == 0) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            } else if (event.type == sf::Event::TextEntered) {
                if (event.text.unicode == '1') {
                    selected_words = easy_words;
                    difficulty = 1;
                } else if (event.text.unicode == '2') {
                    selected_words = medium_words;
                    difficulty = 2;
                } else if (event.text.unicode == '3') {
                    selected_words = hard_words;
                    difficulty = 3;
                }
            } else if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::Escape) {
                    window.close();
                }
            }
        }
        window.clear();
        window.draw(text);
        window.display();
    }
}

std::string get_input_word(sf::RenderWindow& window, sf::Text& text, sf::Color& curr_col) {
    std::string input_word;
    text.setString("Enter a word for your friend to guess:");
    text.setPosition(200, 400);
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            } else if (event.type == sf::Event::TextEntered) {
                if (event.text.unicode == '\n') { // Enter key
                    if (!input_word.empty()) {
                        return input_word;
                    }
                } else if (event.text.unicode == 8) { // Backspace key
                    if (!input_word.empty()) {
                        input_word.pop_back();
                    }
                } else if (event.text.unicode >= 32 && event.text.unicode <= 126) { // Printable characters
                    input_word += static_cast<char>(event.text.unicode);
                }
                text.setString("Enter a word for your friend to guess:\n\n" + input_word);
            } else if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::Escape) {
                    window.close();
                }
                if (event.key.code == sf::Keyboard::Tab) {
                    curr_col = sf::Color(100, 100, 100, 255);
                }
                if (event.key.code == sf::Keyboard::Enter) {
                    if (!input_word.empty()) {
                        return input_word;
                    }
                }
            }
        }
        window.clear(curr_col);
        window.draw(text);
        window.display();
    }
    return "";
}

void choose_mode(sf::RenderWindow& window, int& mode, sf::Text& text) {
    text.setString("Select mode:\n\n1 - Solo\n\n2 - With Friend");
    text.setPosition(400, 400);

    while (window.isOpen() && mode == 0) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            } else if (event.type == sf::Event::TextEntered) {
                if (event.text.unicode == '1') {
                    mode = 1; // Solo
                } else if (event.text.unicode == '2') {
                    mode = 2; // With Friend
                }
            } else if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::Escape) {
                    window.close();
                }
            }
        }
        window.clear();
        window.draw(text);
        window.display();
    }
}

std::string get_font_path() {
    char get_path[1024];
    getcwd(get_path, sizeof(get_path));
    std::string path = get_path;
    path.resize(path.find("gallows") + 7);
    return path + "\\assets\\silkscreen.ttf";
}

void gallows() {
    bool is_end = false;
    std::srand(static_cast<unsigned>(std::time(nullptr)));
    std::string word;
    std::vector<std::string> selected_words;

    // Создание окна
    sf::Color curr_col;
    sf::RenderWindow window(sf::VideoMode(1000, 1000), "Gallows");
    sf::Text text;
    sf::Font font;
    font.loadFromFile(get_font_path());
    text.setFont(font);
    text.setCharacterSize(20);

    int mode = 0;
    choose_mode(window, mode, text);

    if (mode == 1) {
        choose_difficulty(window, selected_words, text);
        word = selected_words[std::rand() % selected_words.size()];
    } else if (mode == 2) {
        word = get_input_word(window, text, curr_col);
    }

    int number_mistakes = 0;
    int number_correct = 0;
    int number_entered = 0;
    std::vector<char> mistakes;
    std::vector<char> ready(word.size(), '.');
    text.setString("Game started\n\nPress any button");
    text.setPosition(400, 400);

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            } else if (event.type == sf::Event::TextEntered) {
                ++number_entered;
                if (number_entered == 1) {
                    text.setString(get_play_screen(" ", ready, mistakes));
                    continue;
                }
                if (event.text.unicode == 8 && is_end) { // Check for Backspace key
                    is_end = false;
                    number_mistakes = 0;
                    number_correct = 0;
                    number_entered = 0;
                    mistakes.clear();
                    ready.assign(word.size(), '.');
                    mode = 0;
                    choose_mode(window, mode, text);
                    if (mode == 1) {
                        choose_difficulty(window, selected_words, text);
                        word = selected_words[std::rand() % selected_words.size()];
                    } else if (mode == 2) {
                        word = get_input_word(window, text, curr_col);
                    }
                    text.setString(get_play_screen(" ", ready, mistakes));
                    continue;
                }
                if (event.text.unicode == 27) { // Check for Escape key
                    window.close();
                    return;
                }
                if (96 < event.text.unicode && event.text.unicode < 123) {
                    std::string inputText = "";
                    inputText += static_cast<char>(event.text.unicode);
                    text.setString(get_play_screen(inputText, ready, mistakes));
                    process_input(inputText, ready, mistakes, number_correct, number_mistakes, word);
                    text.setString(get_play_screen(inputText, ready, mistakes));
                    if (number_correct == word.size()) {
                        text.setString("\t\tGame won!\n\nCorrect word: " + word + "\n\nPress Enter to restart\n\nPress Backspace to change mode\n");
                        is_end = true;
                        break;
                    }
                    if (number_mistakes > 5) {
                        text.setString("\t\tGame over\n\nCorrect word: " + word + "\n\nPress Enter to restart\n\nPress Backspace to change mode\n");
                        is_end = true;
                        break;
                    }
                }
            } else if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::Escape) {
                    window.close();
                }
                if (event.key.code == sf::Keyboard::Tab) {
                    curr_col = sf::Color(100, 100, 100, 255);
                }
                if (event.key.code == sf::Keyboard::Enter && is_end) { // Check for Enter key
                    is_end = false;
                    if (mode == 1) {
                        word = selected_words[std::rand() % selected_words.size()]; // Generate new word
                    } else if (mode == 2) {
                        word = get_input_word(window, text, curr_col);
                    }
                    number_mistakes = 0;
                    number_correct = 0;
                    number_entered = 0;
                    mistakes.clear();
                    ready.assign(word.size(), '.');
                    text.setString(get_play_screen(" ", ready, mistakes));
                    continue;
                }
            }
        }
        window.clear(curr_col);
        window.draw(text);
        window.display();
    }
}
