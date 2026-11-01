#include "../include/livro.hpp"
#include <cctype>
#include <iostream>
#include <string>

unsigned int Livro::Id() {
  return this->id;
}

std::string Livro::Nome() {
  return this->nome;
}

std::string Livro::Autor() {
  return this->autor;
}

unsigned int Livro::Ano() {
  return this->ano;
}

void Livro::Print() {
  std::cout <<
    id << " | " <<
    nome << " | " <<
    autor << " | " <<
    ano << std::endl;
}

std::string Livro::NameToIdentifier(const std::string& text) {
  std::string ident;
  ident.reserve(text.size());
  
  for (const char& letra : text) {
    if (letra == ' ') {
      ident.push_back('_');
      continue;
    }
    ident.push_back(tolower(letra));
  }

  return ident;
}
