#include "Trie.h"
#include <cctype>

Trie::Trie() : root(std::make_unique<TrieNode>()) {}

void Trie::insert(const std::string& word) {
    TrieNode* current = root.get();
    for (char ch : word) {
        char lower_ch = std::tolower(ch);
        if (lower_ch >= 'a' && lower_ch <= 'z') {
            int index = lower_ch - 'a';
            if (!current->children[index]) {
                current->children[index] = std::make_unique<TrieNode>();
            }
            current = current->children[index].get();
        }
    }
    current->isEndOfWord = true;
}

bool Trie::search(const std::string& word) const {
    const TrieNode* current = root.get();
    for (char ch : word) {
        char lower_ch = std::tolower(ch);
        if (lower_ch >= 'a' && lower_ch <= 'z') {
            int index = lower_ch - 'a';
            if (!current->children[index]) {
                return false;
            }
            current = current->children[index].get();
        }
    }
    return current && current->isEndOfWord;
}

std::vector<std::string> Trie::getAllWords() const {
    std::vector<std::string> words;
    std::string currentPrefix;
    getAllWordsRecursive(root.get(), currentPrefix, words);
    return words;
}

void Trie::getAllWordsRecursive(const TrieNode* node, std::string& currentPrefix, std::vector<std::string>& words) const {
    if (!node) {
        return;
    }

    if (node->isEndOfWord) {
        words.push_back(currentPrefix);
    }

    for (int i = 0; i < 26; ++i) {
        if (node->children[i]) {
            currentPrefix.push_back('a' + i);
            getAllWordsRecursive(node->children[i].get(), currentPrefix, words);
            currentPrefix.pop_back();
        }
    }
}