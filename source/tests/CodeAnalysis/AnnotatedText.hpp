#pragma once

#include <limits>
#include <sstream>
#include <stack>
#include <stdexcept>
#include <string>
#include <vector>

extern "C"
{
#include <minsk/CodeAnalysis/Text/TextSpan.h>
}

class AnnotatedText
{
  public:
    std::string text;
    std::vector<TextSpan> spans;
    AnnotatedText(const std::string &text, const std::vector<TextSpan> &spans) : text{text}, spans{spans}
    {
    }

    static AnnotatedText parse(std::string text)
    {
        text = unindent(text);

        std::stringstream text_builder;
        std::vector<TextSpan> spans;
        std::stack<int> start_stack;

        int position = 0;

        for (char c : text)
        {
            if (c == '[')
            {
                start_stack.push(position);
            }
            else if (c == ']')
            {
                if (start_stack.empty())
                {
                    throw std::runtime_error{"too many closing brackets in text"};
                }

                int start = start_stack.top();
                start_stack.pop();
                int end = position;
                auto span = text_span_from_bounds(start, end);
                spans.emplace_back(*span);
            }
            else
            {
                ++position;
                text_builder << c;
            }
        }

        if (!start_stack.empty())
        {
            throw std::runtime_error{"too many opening brackets in text"};
        }
        return AnnotatedText{text_builder.str(), spans};
    }

    static std::vector<std::string> unindent_lines(const std::string &text)
    {
        std::vector<std::string> lines;
        std::stringstream string_reader{text};
        std::string line;
        while (std::getline(string_reader, line))
        {
            lines.push_back(line);
        }
        int min_indentation = std::numeric_limits<int>::max();
        for (auto &line : lines)
        {
            int indentation = line.find_first_not_of(" \t");
            if (indentation == std::string::npos)
            {
                line.clear();
                continue;
            }
            min_indentation = std::min(min_indentation, indentation);
        }

        for (int i = 0; i < lines.size(); ++i)
        {
            if (lines[i].empty())
            {
                continue;
            }
            lines[i] = lines[i].substr(min_indentation);
        }

        while (lines.size() > 0 && lines[0].empty())
        {
            lines.erase(lines.begin());
        }

        while (lines.size() > 0 && lines.back().empty())
        {
            lines.pop_back();
        }

        return lines;
    }

  private:
    static std::string unindent(const std::string &text)
    {
        std::vector<std::string> lines;
        std::stringstream string_reader{text};
        std::string line;
        while (std::getline(string_reader, line))
        {
            lines.push_back(line);
        }
        int min_indentation = std::numeric_limits<int>::max();
        for (auto &line : lines)
        {
            int indentation = line.find_first_not_of(" \t");
            if (indentation == std::string::npos)
            {
                line.clear();
                continue;
            }
            min_indentation = std::min(min_indentation, indentation);
        }

        for (int i = 0; i < lines.size(); ++i)
        {
            if (lines[i].empty())
            {
                continue;
            }
            lines[i] = lines[i].substr(min_indentation);
        }

        while (lines.size() > 0 && lines[0].empty())
        {
            lines.erase(lines.begin());
        }

        std::stringstream out;
        for (auto &line : lines)
        {
            out << line << '\n';
        }
        return out.str();
    }
};
