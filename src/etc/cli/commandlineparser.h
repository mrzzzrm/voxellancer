#pragma once

#include <string>

#include <unordered_map>


class CommandLineParser {
public:
    CommandLineParser();

    void parse(int argc, char* argv[]);

    bool hmd() const;
    bool stereoView() const;
    bool fullScreen() const;
    bool showIntro() const;

private:
    std::unordered_map<std::string, std::string> m_options;

    bool isSet(const std::string& arg) const;
    void checkHelp();

};
