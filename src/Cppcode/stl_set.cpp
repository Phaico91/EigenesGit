#include <iostream>
#include <string>
#include <utility>
#include <set>

using str_set = std::set<std::string>;

int main(void)
{
  str_set words;
  std::string word;
  while (std::getline (std::cin,word))
  {
    std::pair<str_set::iterator, bool> result;
    result = words.insert(word); // try to insert word
    if (std::get<1>(result)) // result[1] is true iff inserted
      std::cout << "new word \"" << word << "\"" << std::endl;
  }
  std::cout << "# different words: " << words.size()
                                     << std::endl;
  for (auto it = words.begin(); it != words.end(); it++)
    std::cout << *it << std::endl;
  for (str_set::iterator it = words.begin(); it != words.end(); it++)
    std::cout << *it << std::endl;
}
