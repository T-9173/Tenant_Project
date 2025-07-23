#ifndef TRIE_H
#define TRIE_H

#include <string>
#include <vector>
#include <memory>

class Trie {
public:
    Trie();
    void insert(const std::string& word);
    bool search(const std::string& word) const;
    std::vector<std::string> getAllWords() const;

private:
    struct TrieNode {
        std::unique_ptr<TrieNode> children[26];
        bool isEndOfWord;

        TrieNode() : isEndOfWord(false) {
            for (int i = 0; i < 26; ++i) {
                children[i] = nullptr;
            }
        }
    };

    void getAllWordsRecursive(const TrieNode* node, std::string& currentPrefix, std::vector<std::string>& words) const;

    std::unique_ptr<TrieNode> root;
};

#endif 