#include <map>
#include <string>
#include <thread>
#include <vector>
#include <fstream>
#include <iostream>

std::vector<std::string> words;
std::vector<std::map<std::string, short>> count;

class csv_view {
public:
    std::vector<std::pair<std::string, std::string>> header;
    std::vector<std::vector<std::pair<std::string, int>>> body;

    void addHeader(const std::string& title) {
        header.emplace_back(title, "frequency");
        body.emplace_back();
    }

    void addToColumn(const int& index, const std::pair<std::string, int>& pair) {
        body[index].push_back(pair);
    }

    void writeToFile(const std::string& filename, const int& len) {
        std::ofstream writer(filename);

        if (!header.empty()) {
            writer << header[0].first << "," << header[0].second;
            for (int i = 1; i < header.size(); ++i) {
                writer << "," << header[i].first << "," << header[i].second;
            }
            writer << '\n';
        }

        for (int i = 0; i < len; ++i) {
            if (body[0][i].second > 1) {
                writer << body[0][i].first << "," << body[0][i].second;
            } else {
                writer << ",";
            }

            for (int j = 1; j < body.size(); ++j) {
                if (body[j][i].second > 1) {
                    writer << "," << body[j][i].first << "," << body[j][i].second;
                } else {
                    writer << ",,";
                }
            }
            writer << '\n';
        }

        writer.close();
    }
};

inline auto is_allowed(const char& c) -> bool {
    return (c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z') || (c >= '0' && c <= '9');
}

auto count_words(int phrase_len) -> void {
    const int len = phrase_len - 1;
    const unsigned size = words.size() - phrase_len;

    int j;
    std::string str{};

    for (int i = 0; i < size; ++i) {
        str = words[i];

        for (j = 1; j < phrase_len; ++j)
            str += " " + words[i + j];

        if (count[len].contains(str))
            ++count[len][str];
        else count[len][str] = 1;
    }
}

auto tokenize(std::ifstream chunk) -> void{
    auto i = 0;
    char c, word[70];
    count.emplace_back();

    while (chunk.get(c)) {
        if (c == ' ') {
            if (i == 0) continue;
            else {
                word[i] = '\0';
                const auto str = std::string(word);

                if (count[0].contains(str))
                    ++count[0][str];
                else count[0][str] = 2;

                words.push_back(str);

                i = 0;
            }
        } else if (is_allowed(c)) {
            word[i] = c;
            ++i;
        }
    }
}

auto main(int argc, char* argv[]) -> int {
    if (argc != 4) {
        std::cout << "Invalid number of arguments were passed";
        std::exit(-1);
    }

    csv_view view;
    view.addHeader("phrase length 1");
    tokenize(std::ifstream(argv[1]));

    const int len = std::stoi(argv[2]);
    for (int i = 2; i <= len; ++i) {
        view.addHeader("phrase length " + std::to_string(i));
        count.emplace_back();
        count_words(i);
    }

    int max_len = 0;
    for (int i = 0; i < len; ++i) {
        if (max_len < count[i].size())
            max_len = count[i].size();
    }

    for (int i = 0; i < len; ++i) {
        int _count = 0;
        for (const auto& item: count[i]) {
            if (item.second > 1) {
                view.addToColumn(i, item);
                ++_count;
            }
        }

        for (int j = _count; j < max_len; ++j) {
            view.addToColumn(i, std::pair("", 0));
        }
    }

    view.writeToFile(argv[3], max_len);

    return 0;
}
