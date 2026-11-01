#include "./include/database.hpp"
#include "./include/livro.hpp"
#include <chrono>
#include <cstdio>
#include <exception>
#include <iostream>
#include <string>
#include <thread>
#include <cstdlib>

#define DEBUG_MODE

void clear_terminal() {
  #if defined(_WIN32)
    system("cls");
  #else
    system("clear");
  #endif
}

int main(int argc, char **argv) {
  Database db("banco.db");

  bool rodando = true;
  while (rodando) {
    clear_terminal();

    std::cout << "1. Inserir livro" << std::endl;
    std::cout << "2. Buscar livro por id" << std::endl;
    std::cout << "3. Buscar livro por nome" << std::endl;
    std::cout << "4. Sair" << std::endl;

    std::string opcao;
    std::cout << ">> ";
    std::getline(std::cin, opcao);
    if (opcao.empty()) {
      std::cout << "Insira uma opção." << std::endl;
      std::this_thread::sleep_for(std::chrono::milliseconds(1500));
      continue;
    }
    switch (opcao.at(0)) {
    case '1': { // insert
      std::string nome;
      std::string autor;
      std::string ano;

      std::cout << "Insira o nome da obra: ";
      std::getline(std::cin, nome);

      std::cout << "Insira o nome do autor: ";
      std::getline(std::cin, autor);

      std::cout << "Insira o ano de publicação: ";
      std::getline(std::cin, ano);

      if (nome.empty()) {
        std::cout << "Nome inválido. ";
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
        std::cout << "Aperte ENTER para continuar...";
        int _ = std::getchar();
        continue;
      }

      if (autor.empty()) {
        std::cout << "Nome de autor inválido. ";
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
        std::cout << "Aperte ENTER para continuar...";
        int _ = std::getchar();
        continue;
      }

      if (ano.empty()) {
        std::cout << "Ano de publicação inválido. ";
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
        std::cout << "Aperte ENTER para continuar...";
        int _ = std::getchar();
        continue;
      }

      int ano_fmt;
      try {
        ano_fmt = std::stoi(ano);
      } catch (std::exception &e) {
        std::cout << "Ano de publicação inválido. ";

        #ifdef DEBUG_MODE
          std::cerr << std::endl << e.what() << std::endl;
        #endif
        
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
        std::cout << "Aperte ENTER para continuar...";
        int _ = std::getchar();
        continue;
      }

      char *response = db.inserirLivro(Livro(0, nome, autor, ano_fmt));
      if (response != nullptr) {
        std::cout << "Ocorreu um erro ao inserir livro. Provavelmente já existe. ";

        #ifdef DEBUG_MODE
          std::cerr << std::endl << response << std::endl;
        #endif
        
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
        std::cout << "Aperte ENTER para continuar...";
        int _ = std::getchar();
        continue;
      }

      std::cout << "Livro inserido!" << std::endl;
      std::this_thread::sleep_for(std::chrono::milliseconds(1200));
      break;
    }

    case '2': { // search by id
      std::string id_str;
      int id;

      std::cout << "Digite o ID do livro: ";
      std::getline(std::cin, id_str);

      try {
        id = std::stoi(id_str);
      } catch (std::exception &e) {
        std::cout << "ID inválido. (" << e.what() << "). ";
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
        std::cout << "Aperte ENTER para continuar...";
        int _ = std::getchar();
        continue;
      }

      Livro *l = db.buscarLivro(id);
      if (l == nullptr) {
        std::cout << "Livro não encontrado. ";
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
        std::cout << "Aperte ENTER para continuar...";
        int _ = std::getchar();
        continue;
      }

      std::cout << "ID: " << l->Id() << std::endl;
      std::cout << "Nome: " << l->Nome() << std::endl;
      std::cout << "Autor: " << l->Autor() << std::endl;
      std::cout << "Ano de publicação: " << l->Ano() << std::endl << std::endl;

      std::cout << "Aperte ENTER para continuar...";
      int _ = std::getchar();
      delete l;

      break;
    }
    case '3': { // search by name
      std::string nome_livro;
      std::cout << "Digite o nome da obra: ";
      std::getline(std::cin, nome_livro);

      if (nome_livro.empty()) {
        std::cout << "Digite um nome válido. ";
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
        std::cout << "Aperte ENTER para continuar...";
        int _ = std::getchar();
        continue;
      }

      Livro *l = db.buscarLivro(nome_livro);
      if (l == nullptr) {
        std::cout << "Livro não encontrado. ";
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
        std::cout << "Aperte ENTER para continuar...";
        int _ = std::getchar();
        continue;
      }

      std::cout << "ID: " << l->Id() << std::endl;
      std::cout << "Nome: " << l->Nome() << std::endl;
      std::cout << "Autor: " << l->Autor() << std::endl;
      std::cout << "Ano de publicação: " << l->Ano() << std::endl << std::endl;

      std::cout << "Aperte ENTER para continuar...";
      int _ = std::getchar();

      delete l;
      break;
    }
    case '4': {
      std::cout << "Saindo..." << std::endl;
      rodando = false;
      break;
    }
    default: {
      std::cout << "Opção inválida. Selecione de 1 à 4. ";
      std::this_thread::sleep_for(std::chrono::milliseconds(500));
      std::cout << "Aperte ENTER para continuar...";
      int _ = std::getchar();
      continue;
    }
    } // switch end

  } // while end

  return 0;
} // function end
