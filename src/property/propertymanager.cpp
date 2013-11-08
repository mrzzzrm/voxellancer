#include <regex>
#include <string>
#include <fstream>
#include <iostream>

#include <glow/logging.h>

#include "propertymanager.h"

#include "propertycollection.h"


// some string, some spaces, equals, some spaces, some string, maybe a comment
static std::regex line_regex() { return std::regex(R"(^([\w\.]*) *= *(.+?)( *#.*)?$)"); }
static std::regex title_regex() { return std::regex(R"(^\[(\w+)\])"); }

static std::regex float_regex() { return std::regex(R"(^[-+]?\d*\.?\d*$)"); }
static std::regex int_regex() { return std::regex(R"(^[-+]?\d+$)"); }
static std::regex bool_regex() { return std::regex(R"(^(true|false)$)"); }
static std::regex char_regex() { return std::regex(R"(^\w$)"); }
static std::regex string_regex() { return std::regex(R"(^.*$)"); }
static std::regex vec3_regex() { return std::regex(R"(^([-+]?\d*\.?\d*), ?([-+]?\d*\.?\d*), ?([-+]?\d*\.?\d*)$)"); }

static glm::vec3 vec3converter(const std::string &s) {
    std::smatch matches;
    std::regex_match(s, matches, vec3_regex());

    float x = std::stof(matches[1]);
    float y = std::stof(matches[2]);
    float z = std::stof(matches[3]);

    return glm::vec3(x, y, z);
}

PropertyManager::PropertyManager():
    m_floatProperties(new PropertyCollection<float>(float_regex(), [](std::string s) { return std::stof(s); })),
    m_intProperties(new PropertyCollection<int>(int_regex(), [](std::string s) { return std::stoi(s); })),
    m_charProperties(new PropertyCollection<char>(char_regex(), [](std::string s) { return s[0]; })),
    m_boolProperties(new PropertyCollection<bool>(bool_regex(), [](std::string s) { return s == "true" ? true : false; })),
    m_stringProperties(new PropertyCollection<std::string>(string_regex(), [](std::string s) { return s; })),
    m_vec3Properties(new PropertyCollection<glm::vec3>(vec3_regex(), vec3converter))
{

}

PropertyManager::~PropertyManager() {
    delete m_floatProperties;
    delete m_intProperties;
    delete m_charProperties;
    delete m_boolProperties;
    delete m_stringProperties;
    delete m_vec3Properties;
}


void PropertyManager::load(std::string file)
{
// TODO: REMOVE DEFAULT VALUE WHEN LINUX IS WORKING
#ifdef __linux__
    return;
#endif

    std::ifstream input(file);
    std::string line;
    std::string title = "";

    if (!input.is_open()) {
        glow::warning("PropertyManager: could not open %;", file);
    }

    while (std::getline(input, line))
    {
        std::smatch matches;

        std::regex_match(line, matches, title_regex());
        if (matches.size() > 0) {
            title = matches[1];
            continue;
        }
        std::regex_match(line, matches, line_regex());
        if (matches.size() > 0) {
            std::string key, key_temp, value;
            key_temp = matches[1];
            key = title + '.' + key_temp;
            value = matches[2];
            int success = 0;

            if (m_floatProperties->update(key, value)) success++;
            if (m_intProperties->update(key, value)) success++;
            if (m_boolProperties->update(key, value)) success++;
            if (m_charProperties->update(key, value)) success++;
            if (m_stringProperties->update(key, value)) success++;
            if (m_vec3Properties->update(key, value)) success++;

            if (success == 0) {
                glow::warning("PropertyManager: no match %;: %; (line: %;)", key, value, line);
            }
        }
    }

    changed();
}

PropertyManager * PropertyManager::getInstance()
{
    if (s_instance == nullptr) {
        s_instance = new PropertyManager();
    }
    return s_instance;
}


void PropertyManager::reset()
{
    if (s_instance != nullptr) {
        delete s_instance;
        s_instance = nullptr;
    }
}

PropertyManager * PropertyManager::s_instance;

// any better idea or maybe generate these with macros?
template <>
PropertyCollection<int> * PropertyManager::getPropertyCollection(Property<int> * prop)
{
    return m_intProperties;
}

template <>
PropertyCollection<char> * PropertyManager::getPropertyCollection(Property<char> * prop)
{
    return m_charProperties;
}

template <>
PropertyCollection<float> * PropertyManager::getPropertyCollection(Property<float> * prop)
{
    return m_floatProperties;
}

template <>
PropertyCollection<bool> * PropertyManager::getPropertyCollection(Property<bool> * prop)
{
    return m_boolProperties;
}

template <>
PropertyCollection<std::string> * PropertyManager::getPropertyCollection(Property<std::string> * prop)
{
    return m_stringProperties;
}

template <>
PropertyCollection<glm::vec3> * PropertyManager::getPropertyCollection(Property<glm::vec3> * prop)
{
    return m_vec3Properties;
}
