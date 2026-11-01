#include "../include/database.hpp"
#include "../include/livro.hpp"
#include <cctype>
#include <cstdlib>
#include <iostream>
#include <sqlite3.h>

const char* sql_createtable = R"SQL(
  CREATE TABLE IF NOT EXISTS livros (
    id INTEGER PRIMARY KEY AUTOINCREMENT,
    nome_identificador TEXT NOT NULL UNIQUE,
    nome TEXT NOT NULL,
    autor TEXT NOT NULL,
    ano INTEGER NOT NULL
  );
)SQL";

const char* sql_searchbook_id = R"SQL(
  SELECT id, nome, autor, ano
  FROM livros
  WHERE id = ?;
)SQL";

const char* sql_searchbook_name = R"SQL(
  SELECT id, nome, autor, ano
  FROM livros
  WHERE nome_identificador = ?;
)SQL";

const char* sql_insertbook = R"SQL(
    INSERT INTO livros (
      nome_identificador,
      nome,
      autor,
      ano
    )
    VALUES (?, ?, ?, ?);
)SQL";

Livro* Database::buscarLivro(int _id) {
  sqlite3_stmt* stmt = nullptr;
  sqlite3_prepare_v2(
      this->connection,  // database
      sql_searchbook_id, // query
      -1,                // query length
      &stmt,             // statement
      nullptr            // something...
    );
  sqlite3_bind_int(
      stmt, // statement
      1,    // index
      _id   // integer
  );

  if (sqlite3_step(stmt) == SQLITE_ROW) {
    int   id    = sqlite3_column_int(stmt, 0);
    char* nome  = (char*)sqlite3_column_text(stmt, 1);
    char* autor = (char*)sqlite3_column_text(stmt, 2);
    int   ano   = sqlite3_column_int(stmt, 3);

    Livro* l = new Livro(id, nome, autor, ano);
    sqlite3_finalize(stmt);
    return l;
  } else {
    sqlite3_finalize(stmt);
    return nullptr;
  }
}

Livro* Database::buscarLivro(const std::string& _name) {
  std::string name = Livro::NameToIdentifier(_name);
  sqlite3_stmt* stmt = nullptr;
  
  sqlite3_prepare_v2(
      this->connection,
      sql_searchbook_name, // query
      -1,                // query length
      &stmt, 
      nullptr
    );
  sqlite3_bind_text(
    stmt,              // statement
    1,                 // index
    name.c_str(),      // value
    name.size(),       // text length
    SQLITE_TRANSIENT   // mem manager
  );

  if (sqlite3_step(stmt) == SQLITE_ROW) {
    int   id    = sqlite3_column_int(stmt, 0);
    char* nome  = (char*)sqlite3_column_text(stmt, 1);
    char* autor = (char*)sqlite3_column_text(stmt, 2);
    int   ano   = sqlite3_column_int(stmt, 3);

    Livro* l = new Livro(id, nome, autor, ano);
    sqlite3_finalize(stmt);
    return l;
  } else {
    sqlite3_finalize(stmt);
    return nullptr;
  }
}

char* Database::inserirLivro(Livro livro) {
  sqlite3_stmt* stmt = nullptr;
  sqlite3_prepare_v2(
      this->connection,
      sql_insertbook,
      -1,
      &stmt,
      nullptr
  );
  std::string nome_identificador = Livro::NameToIdentifier(livro.Nome());
  sqlite3_bind_text(
      stmt,                       // statement
      1,                          // index
      nome_identificador.c_str(), // value
      nome_identificador.size(),  // text length
      SQLITE_TRANSIENT            // mem manager
  );

  sqlite3_bind_text(
      stmt,                       // statement
      2,                          // index
      livro.Nome().c_str(),       // value
      livro.Nome().size(),        // text length
      SQLITE_TRANSIENT            // mem manager
  );


  sqlite3_bind_text(
      stmt,                       // statement
      3,                          // index
      livro.Autor().c_str(),      // value
      livro.Autor().size(),       // text length
      SQLITE_TRANSIENT            // mem manager
  );

  sqlite3_bind_int(
      stmt,                       // statement
      4,                          // index
      livro.Ano()                 // integer
  );

  // TODO: complete a função pra inserir o livro no banco de dados
  int result = sqlite3_step(stmt);
  char* res = nullptr;
  if (result != SQLITE_DONE) {
    res = (char*)sqlite3_errmsg(this->connection);
  }

  sqlite3_finalize(stmt);
  return res;
}

void Database::fecharConexao() {
  int res = sqlite3_close(this->connection);
  if (res != SQLITE_OK) {
    std::cerr << "Erro ao fechar o database: " << sqlite3_errmsg(this->connection) << std::endl;
  }
}

Database::Database(const std::string& db_path) {
  connection = nullptr;
  if (sqlite3_open(db_path.c_str(), &connection) != SQLITE_OK) {
    std::cerr << "Can not open database in " << db_path << ": " << sqlite3_errmsg(connection) << std::endl;
    sqlite3_close(connection);
    std::exit(1);
  }

  if (sqlite3_exec(connection, sql_createtable, nullptr, nullptr, nullptr) != SQLITE_OK) {
    std::cerr << "Erro ao criar a tabela 'livros': " << sqlite3_errmsg(connection) << std::endl;
    sqlite3_close(connection);
    std::exit(1);
  }
}
