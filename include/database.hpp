#pragma once

#include <sqlite3.h>
#include <string>
#include "../include/livro.hpp"

class Database {
private:
  sqlite3* connection;

public:
  Livro* buscarLivro(int id);
  Livro* buscarLivro(const std::string& name);
  char* inserirLivro(Livro l);
  void fecharConexao();

  Database(const std::string& db_path);
};
