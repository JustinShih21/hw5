#ifndef RECCHECK
// For debugging
#include <iostream>
// For std::remove
#include <algorithm> 
#include <map>
#include <set>
#endif
#include "wordle.h"
#include "dict-eng.h"
using namespace std;


// Add prototypes of helper functions here

// Use Recursion to generate all possible "word" outcomes, and then see if they are in the dictionary

// Recursion

void possibleWords(const std::string& in,
    const std::string& floating, const std::set<std::string>& dict, 
    std::string& word, std::set<std::string>& words, size_t position = 0);



// Definition of primary wordle function
std::set<std::string> wordle(
    const std::string& in,
    const std::string& floating,
    const std::set<std::string>& dict)
{
    // Add your code here
    if (in.empty()) {
      return std::set<std::string>();
    }

    std::set<std::string> words;
      
    // Adding corner casees
    if (!dict.empty()) {
      
      std::string word(in.length(), ' ');

      possibleWords(in, floating, dict, word, words, 0);
    }
    return words;
}

// Uses recursion to create a list of possible words
void possibleWords(const std::string& in, const std::string& floating, const std::set<std::string>& dict, std::string& word, std::set<std::string>& words, size_t position) {

  // If all the letters in the word have been filled
  if (position >= in.length()) {

    // If the word built is in the dictionary, add it to the 
    if (dict.find(word) != dict.end() && floating.empty()) {
      words.insert(word);
    }
    return;
  }

  // Need to check to see if the letter at the current position is a fixed position 
  // Check if the current letter position is not a dash
  if (in[position] != '-') {

    // Recall the function after incrementing the position and changing the word position to be a dASH
    word[position] = in[position];
    possibleWords(in, floating, dict, word, words, position + 1);
    return;
  }

  // Manually check through how many dashes there are
  int dashes_remaining = 0;
  for (size_t i = position; i < in.length(); i++) {

    if (in[i] == '-') {
      dashes_remaining ++;
    }
  }

  // If there is floating letters, try to use them first
  for (size_t i = 0; i < floating.length(); i++) {

      char floater = floating[i];
      word[position] = floater;
      std::string remainingFloating = floating;
      remainingFloating.erase(i, 1);
      possibleWords(in, remainingFloating, dict, word, words, position + 1);
    }

  // If there are more dashes than floating letters then go throuigh the alphabet
  if (dashes_remaining > floating.length()) {
    for (char c = 'a'; c <= 'z'; c ++) {
      if(floating.find(c) == std::string::npos) {
        word[position] = c;
        possibleWords(in, floating, dict, word, words, position + 1);
      }
    }
  }


}
