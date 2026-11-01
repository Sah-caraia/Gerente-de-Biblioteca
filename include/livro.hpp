#pragma once

#include <string>

class Livro {
private:
  unsigned int id;
  unsigned int ano;
  std::string nome;
  std::string autor;

public:
  unsigned int Id();
  std::string Nome();
  std::string Autor();
  unsigned int Ano();
  void Print();

  static std::string NameToIdentifier(const std::string& text);

  Livro(unsigned int _id, const std::string& _nome, const std::string& _autor, unsigned int _ano) :
    id(_id), ano(_ano), nome(_nome), autor(_autor) {}
};
