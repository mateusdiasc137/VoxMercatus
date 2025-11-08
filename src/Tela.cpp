#include "Tela.h"
#include <cstdlib> 
#include <locale>
#include <string>

void Tela::limpar() {
    #ifdef _WIN32
        system("cls"); // Comando para limpar a tela no Windows
    #else
        system("clear"); // Comando para limpar a tela no Linux
    #endif
}

void Tela::configurarIdioma() {
    // Configura o locale para o sistema operacional
    const std::string idioma = "pt_BR.UTF-8";
    std::setlocale(LC_ALL, idioma.c_str());
}