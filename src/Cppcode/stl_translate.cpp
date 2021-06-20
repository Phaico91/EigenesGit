#include <cstdlib>
#include <fstream>
#include <iostream>
#include <string>
#include <cstring>

/* one would prefer to have the map as constexpr const value, but this
   require to initialize the map via a template function, see
   https://stackoverflow.com/questions/62262308/const-map-and-its-size
*/

#include <map>

using str_char_map = std::map<std::string,char>;

static char codon2aa(std::string codon) {
  static str_char_map codonmap = {
    {"TCA", 'S'},    // Serine
    {"TCC", 'S'},    // Serine
    {"TCG", 'S'},    // Serine
    {"TCT", 'S'},    // Serine
    {"TTC", 'F'},    // Phenylalanine
    {"TTT", 'F'},    // Phenylalanine
    {"TTA", 'L'},    // Leucine
    {"TTG", 'L'},    // Leucine
    {"TAC", 'Y'},    // Tyrosine
    {"TAT", 'Y'},    // Tyrosine
    {"TAA", '*'},    // Stop
    {"TAG", '*'},    // Stop
    {"TGC", 'C'},    // Cysteine
    {"TGT", 'C'},    // Cysteine
    {"TGA", '*'},    // Stop
    {"TGG", 'W'},    // Tryptophan
    {"CTA", 'L'},    // Leucine
    {"CTC", 'L'},    // Leucine
    {"CTG", 'L'},    // Leucine
    {"CTT", 'L'},    // Leucine
    {"CCA", 'P'},    // Proline
    {"CCC", 'P'},    // Proline
    {"CCG", 'P'},    // Proline
    {"CCT", 'P'},    // Proline
    {"CAC", 'H'},    // Histidine
    {"CAT", 'H'},    // Histidine
    {"CAA", 'Q'},    // Glutamine
    {"CAG", 'Q'},    // Glutamine
    {"CGA", 'R'},    // Arginine
    {"CGC", 'R'},    // Arginine
    {"CGG", 'R'},    // Arginine
    {"CGT", 'R'},    // Arginine
    {"ATA", 'I'},    // Isoleucine
    {"ATC", 'I'},    // Isoleucine
    {"ATT", 'I'},    // Isoleucine
    {"ATG", 'M'},    // Methionine
    {"ACA", 'T'},    // Threonine
    {"ACC", 'T'},    // Threonine
    {"ACG", 'T'},    // Threonine
    {"ACT", 'T'},    // Threonine
    {"AAC", 'N'},    // Asparagine
    {"AAT", 'N'},    // Asparagine
    {"AAA", 'K'},    // Lysine
    {"AAG", 'K'},    // Lysine
    {"AGC", 'S'},    // Serine
    {"AGT", 'S'},    // Serine
    {"AGA", 'R'},    // Arginine
    {"AGG", 'R'},    // Arginine
    {"GTA", 'V'},    // Valine
    {"GTC", 'V'},    // Valine
    {"GTG", 'V'},    // Valine
    {"GTT", 'V'},    // Valine
    {"GCA", 'A'},    // Alanine
    {"GCC", 'A'},    // Alanine
    {"GCG", 'A'},    // Alanine
    {"GCT", 'A'},    // Alanine
    {"GAC", 'D'},    // Aspartic Acid
    {"GAT", 'D'},    // Aspartic Acid
    {"GAA", 'E'},    // Glutamic Acid
    {"GAG", 'E'},    // Glutamic Acid
    {"GGA", 'G'},    // Glycine
    {"GGC", 'G'},    // Glycine
    {"GGG", 'G'},    // Glycine
    {"GGT", 'T'}    // Glycine

  };
  if (codonmap.count(codon) == 1)
    return codonmap[codon];
  return -1;
}

int main(int argc,const char *argv[]) {
  if (argc != 2) {
    std::cerr << "Usage: " << argv[0] << " <filename>"
              << std::endl;
    return EXIT_FAILURE;
  }
  std::ifstream infile(argv[1]);
  if (infile.fail()) {
    std::cerr << argv[0] << ": cannot open file " << argv[1]
              << std::endl;
    return EXIT_FAILURE;
  }
  std::string line, dna_seq, protein;
  while (std::getline (infile,line)) // read line by line
    dna_seq.append(line); // concatenate lines
  protein.reserve(dna_seq.size()/3);
  for (size_t idx = 0; idx <= dna_seq.size() - 3; idx += 3) {
    std::string codon = dna_seq.substr(idx,3);
    char aa = codon2aa(codon);
    if (aa == -1) {
      std::cerr << codon << " is not a valid codon"
                         << std::endl;
      exit(EXIT_FAILURE);
    }
    protein.push_back(aa);
  }
  std::cout << protein << std::endl;
  return EXIT_SUCCESS;
}
