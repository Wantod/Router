#pragma once

#include <vector>
#include <string>


struct BinaryCheck
{
    struct Node {
        char v;
        int skip;
        int nextPos;
    };

    std::vector<Node> nexts;
    std::vector<std::string> _data2;

    void compile(std::vector<std::string> &datas) {
        for (auto &data : datas) {
            std::cout << data << "\n";
        }

        std::vector<std::string *> limits;
        for (auto &data : datas) {
            limits.push_back(&data);
        }

        {
            char alphabet[28] = {};

            for (auto &data : datas)
            {
                char c = std::toupper(data[0]) - 'A';
                alphabet[c]++;
            }

            for (int i = 0; i < 28; i++)
            {
                if (alphabet[i] <= 1)
                {
                    // unique - find
                }
                else
                {
                    // plusieur
                }
            }
        }

        /*
        for (int i = 0; i < 100; i++)
        {
            char alphabet[256] = {};
            int nb = 0;
            for (auto &data : datas)
            {
                if (i < data.size())
                {
                    char c = data[i];
                    alphabet[c]++;
                }
            }

            for (int j = 0; j < 256; j++)
            {
                if (alphabet[j] <= 1) continue;
                for (auto &data : datas)
                {
                    if (i < data.size() && data[i] == j) data[i] = '-';
                }
            }
        }
        */

        for (auto &data : datas) {
            _data2.push_back(data);
        }
    }

    int check() {

    }

    void showTree() {
        for (auto &str : _data2)
            std::cout << str << "\n";
        // for (auto &str : _data) {
        //     std::string out;
        //     for (auto c : str) {
        //         out += c.v + "[skip: " + std::to_string(c.skip) + "] "; 
        //     }
        //     std::cout << out << "\n"; 
        // }
    }
};