#include "Cliente.h"

// construtor
Cliente::Cliente(const std::string nome, const std::string cpf) : nome(nome), cpf(cpf){}

//getters
const std::string Cliente::getNome() const{     return nome; }
const std::string Cliente::getCpf() const{      return cpf; }