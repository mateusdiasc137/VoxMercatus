#include "BancoFuncionario.h"
#include "Tela.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <iomanip>
#include <regex>
#include <thread>

// construtor
BancoFuncionario::BancoFuncionario() {}

// Definição do membro estático
std::map<std::string, std::unique_ptr<Funcionario>> BancoFuncionario::funcionarios;

// gerencia funcionarios
void BancoFuncionario::gerenciarFuncionarios(Funcionario* gerente, BancoFuncionario& banco) {
    int escolha;
    do {
        try {
            Tela::limpar(); // limpa tela

            std::cout << "=============================================\n";
            std::cout << "          GERENCIAMENTO DE FUNCIONÁRIOS      \n";
            std::cout << "=============================================\n";
            std::cout << "1. Adicionar Funcionário                  \n";
            std::cout << "2. Remover Funcionário                    \n";
            std::cout << "3. Exibir Informação de um Funcionário    \n";
            std::cout << "4. Top Caixas com Mais Vendas             \n";
            std::cout << "0. Sair                                   \n";
            std::cout << "=============================================\n";
            std::cout << "Escolha uma opção: ";
            std::cin >> escolha;

            if (std::cin.fail()) {
                std::cin.clear(); // limpa a flag de erro
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // descarta entrada inválida
                throw std::invalid_argument("Entrada inválida. Por favor, digite um número.");
            }

            switch (escolha) {
                case 1: {
                    Tela::limpar(); // limpa tela
                    std::cout << "=============================================\n";
                    std::cout << "          CADASTRO DE FUNCIONÁRIOS      \n";
                    std::cout << "=============================================\n";

                    std::string nome, id, cpf, email, senha, cargo;
                    std::cout << "Cargo (Gerente, Caixa, CaixaPCD): ";
                    std::cin >> cargo; 
                    cargo = toLowerCase(cargo); //Converte o cargo para minúsculas

                    // Verifica se o cargo esta correto
                    if (!(cargo == "gerente" || cargo == "caixa" || cargo == "caixapcd")) {
                        throw std::invalid_argument("Cargo inválido.");
                    }

                    std::cout << "Nome: ";
                    std::cin.ignore(); // limpa buffer
                    std::getline(std::cin, nome);
                    
                    // Verifica se o nome excede 50 caracteres
                    if (nome.length() > 50) {
                        throw std::invalid_argument("Nome muito longo! O nome deve ter no máximo 50 caracteres.");
                    }

                    
                    std::cout << "ID: ";
                    std::cin >> id;

                    // Verifica se o ID está correto de acordo com o cargo
                    if (cargo == "gerente" && id[0] != 'G') {
                        throw std::invalid_argument("ID de Gerente deve começar com 'G'.");
                    } else if (cargo == "caixa" && id[0] != 'C') {
                        throw std::invalid_argument("ID de Caixa deve começar com 'C'.");
                    } else if (cargo == "caixapcd" && id[0] != 'P') {
                        throw std::invalid_argument("ID de Caixa PCD deve começar com 'P'.");
                    }

                    // Verifica se o ID tem o formato correto (exemplo: G-01, C-05, P-10)
                    std::regex formato("^[GCP]-\\d{2}$");
                    if (!std::regex_match(id, formato)) {
                        throw std::invalid_argument("ID inválido. O ID deve ser no formato G-XX, C-XX ou P-XX, com X sendo números.");
                    }

                    std::cout << "CPF: ";
                    std::cin >> cpf;
                    cpf = filtrarCpf(cpf);

                    std::cout << "Email: ";
                    std::cin >> email;

                    // Define a expressão para validar o email no formato XXX@YYY.ZZZ
                    std::regex padraoEmail("^[a-zA-Z0-9._%+-]+@[a-zA-Z]+\\.[a-zA-Z]{2,}$");
                    
                    // Validar se o email corresponde ao padrão
                    if (!std::regex_match(email, padraoEmail)) {
                        throw std::invalid_argument("Email inválido. Formato esperado: XXX@YYY.ZZZ");
                    }

                    std::cout << "Senha: ";
                    std::cin >> senha;

                    if (cargo == "gerente") {
                        banco.adicionarFuncionario(gerente, std::make_unique<Gerente>(nome, id, cpf, email, senha));
                    } else if (cargo == "caixa") {
                        banco.adicionarFuncionario(gerente, std::make_unique<Caixa>(nome, id, cpf, email, senha, 0.0));
                    } else if (cargo == "caixapcd") {
                        banco.adicionarFuncionario(gerente, std::make_unique<CaixaPcd>(nome, id, cpf, email, senha, 0.0));
                    }

                    salvarFuncionariosNoArquivo("data/funcionarios.txt");
                    break;
                }
                case 2: {
                    Tela::limpar(); // limpa tela
                    std::cout << "=============================================\n";
                    std::cout << "             REMOVER FUNCIONÁRIO            \n";
                    std::cout << "=============================================\n";

                    std::string id;
                    std::cout << "ID do funcionário a ser removido: ";
                    std::cin >> id;

                    banco.removerFuncionario(gerente, id);
                    salvarFuncionariosNoArquivo("data/funcionarios.txt");
                    break;
                }
                case 3: {   // exibe detalhes dos funcionários

                Tela::limpar(); // limpa tela
                std::cout << "=============================================\n";
                std::cout << "             REMOVER FUNCIONÁRIO            \n";
                std::cout << "=============================================\n";
                
                std::string nome_funcionario;
                std::cout << "Digite o nome do funcionário: ";
                std::cin.ignore(); // Limpa o buffer
                std::getline(std::cin, nome_funcionario);

                Funcionario* funcionario = buscarFuncionarioPorNome(nome_funcionario);
                if (funcionario == nullptr) {
                    throw std::invalid_argument("Nenhum funcionário encontrado com o nome informado.");
                } else if(temChara(nome_funcionario)){
                    std::string id_funcionario;
                    std::cout << "Mais de um funcionário com mesmo nome.\nDigite o id do funcionário: ";
                    std::getline(std::cin, id_funcionario);

                    funcionario = buscarFuncionarioPorId(id_funcionario);

                    std::cout << id_funcionario << "\n";
                    
                    if (funcionario == nullptr) {
                        throw std::invalid_argument("Nenhum funcionário encontrado com o id informado.");
                    }
                }
                
                std::cout << "----------------------------------------\n";
                std::cout << "Nome: " << funcionario->getNome() << "\n"
                << "ID: " << funcionario->getId() << "\n"
                << "CPF: " << funcionario->getCpf() << "\n"
                << "Email: " << funcionario->getEmail() << "\n"
                << "Cargo: " << funcionario->getCargo() << "\n";
                if (funcionario->getCargo() == "caixa" || funcionario->getCargo() == "caixaPCD"){
                    const Caixa* caixa = dynamic_cast<const Caixa*>(funcionario);
                    if (caixa){
                        std::cout<<"Total vendido: R$ " << std::fixed << std::setprecision(2) << caixa->getTotalVendido() << "\n";
                    }
                }
                std::cout << "----------------------------------------\n";
                    

                std::cout << "Pressione enter para voltar...";
                std::cin.ignore(); // Aguarda a interação do usuário
                break;
                }
                case 4: {
                    //ranking de caixas com mais vendas em dinheiro

                    Tela::limpar(); // Limpa a tela
                    std::cout << "====================================================\n";
                    std::cout << "        TOP FUNCIONÁRIOS COM MAIS VENDAS       \n";
                    std::cout << "====================================================\n";

                    // Vetor para armazenar os caixas
                    std::vector<const Caixa*> caixas;
                    for (const auto& [id, funcionario] : funcionarios) {
                        if (funcionario->getCargo() == "caixa" || funcionario->getCargo() == "caixapcd") {
                            const Caixa* caixa = dynamic_cast<const Caixa*>(funcionario.get());
                            if (caixa) {
                                caixas.push_back(caixa);
                            }
                        }
                    }

                    // Ordena os caixas com base no total vendido (ordem decrescente)
                    std::sort(caixas.begin(), caixas.end(), [](const Caixa* a, const Caixa* b) {
                        return a->getTotalVendido() > b->getTotalVendido();
                    });

                    // Exibe o ranking formatado
                    int rank = 1;
                    std::cout << std::left << std::setw(5) << "Pos" 
                            << std::setw(30) << " Nome" 
                            << std::right << std::setw(10) << "    Total Vendido\n";
                    std::cout << "----------------------------------------------------\n";

                    for (const auto* caixa : caixas) {
                        std::cout << std::left << std::setw(5) << rank++ << " | " 
                                << std::setw(30) << caixa->getNome() 
                                << " | R$ " << std::fixed << std::setprecision(2) << caixa->getTotalVendido() 
                                << "\n";
                        std::cout << "----------------------------------------------------\n";
                    }

                    std::cout << "Pressione enter para voltar...";
                    std::cin.ignore(); // Aguarda a interação do usuário
                    std::cin.get(); // Espera o pressionamento de uma tecla
                    break;
                }
                case 0:
                    return;

                default:
                    throw std::invalid_argument("Opção inválida. Escolha um número entre 0 e 4.");
            }

        } catch (const std::exception& e) {
            std::cerr << "Erro: " << e.what() << "\n";
            std::this_thread::sleep_for(std::chrono::seconds(2));
        }
    } while (true);
}


// Adiciona um funcionário ao banco de dados
void BancoFuncionario::adicionarFuncionario(Funcionario* gerente, std::unique_ptr<Funcionario> novoFuncionario) {
    if (gerente->getCargo() != "gerente") {
        throw std::runtime_error("Apenas gerentes podem adicionar novos funcionários.");
    }

    const std::string& id = novoFuncionario->getId();
    if (funcionarios.find(id) != funcionarios.end()) {
        throw std::runtime_error("Já existe um funcionário com este ID.");
    }

    funcionarios[id] = std::move(novoFuncionario);
    std::cout << "Funcionário adicionado com sucesso.\n";
    std::this_thread::sleep_for(std::chrono::milliseconds(1500)); // aguarda exibição de msg
}

// Remove um funcionário do banco de dados
void BancoFuncionario::removerFuncionario(Funcionario* gerente, const std::string& id) {
    if (gerente->getCargo() != "gerente") {
        throw std::runtime_error("Apenas gerentes podem remover funcionários.");
    }

    auto it = funcionarios.find(id);
    if (it == funcionarios.end()) {
        throw std::runtime_error("Funcionário nao encontrado.");
    }

    funcionarios.erase(it);
    std::cout << "Funcionário removido com sucesso.\n";
    std::this_thread::sleep_for(std::chrono::milliseconds(1500)); // aguarda exibição de msg
}

// Retorna um ponteiro para o funcionário, se encontrado usando email
Funcionario* BancoFuncionario::buscarFuncionarioPorEmail(const std::string& email) const {
    for (const auto& [id, funcionario] : funcionarios) {
        if (funcionario->getEmail() == email) {
            return funcionario.get();
        }
    }
    return nullptr;
}

// Retorna um ponteiro para o funcionário, se encontrado usando nome
Funcionario* BancoFuncionario::buscarFuncionarioPorNome(const std::string& nome) const {
    for (const auto& [id, funcionario] : funcionarios) {
        if (funcionario->getNome() == nome) {
            return funcionario.get();
        }
    }
    return nullptr; 
}

// Retorna um ponteiro para o funcionário, se encontrado usando id
Funcionario* BancoFuncionario::buscarFuncionarioPorId(const std::string& id_funcionario) const {
    for (const auto& [id, funcionario] : funcionarios) {
        if (funcionario->getId() == id_funcionario) {
            return funcionario.get();
        }
    }
    return nullptr; 
}

// verifica se mais de um funcionário tem o mesmo nome
bool BancoFuncionario::temChara(const std::string& nome){
    int contagem = 0;
    for (const auto& funcionario : funcionarios) {
        if (funcionario.second->getNome() == nome) {
            contagem++;
        }
    }

    if(contagem > 1){
        return true;
    } else if(contagem == 1){
        return false;
    }else{
        throw std::runtime_error("Funcionário não esta cadastrado.");
    }
}

// realiza o login do funcionario
Funcionario* BancoFuncionario::realizarLogin(const std::string& email, const std::string& senha) const {
    Funcionario* funcionario = buscarFuncionarioPorEmail(email);
    if (funcionario && funcionario->validarSenha(senha)) {
        return funcionario;
    }
    throw std::runtime_error("Email ou senha inválidos.");
}

// salva os funcionarios em um arquivo txt (simula o banco de dados)
void BancoFuncionario::salvarFuncionariosNoArquivo(const std::string& nomeArquivo) const {
    std::ofstream arquivo(nomeArquivo, std::ios::out); // abre somente para escrita
    if (!arquivo) {
        throw std::runtime_error("Erro ao abrir o arquivo para salvar funcionários.");
    }

    for (const auto& [id, funcionario] : funcionarios) { // percorre o banco de funcionarios
        arquivo << funcionario->getCargo() << ","
                << funcionario->getNome() << ","
                << funcionario->getId() << ","
                << funcionario->getCpf() << ","
                << funcionario->getEmail() << ","
                << funcionario->getSenha();

                // Verifica se é do tipo Caixa ou CaixaPCD
                if (funcionario->getCargo() == "caixa" || funcionario->getCargo() == "caixapcd") {
                    const Caixa* caixa = dynamic_cast<const Caixa*>(funcionario.get());
                    if (caixa) {
                        arquivo << "," << std::fixed << std::setprecision(2) << caixa->getTotalVendido();
                    }
                }

        arquivo << "\n"; // Nova linha para o próximo funcionário                
    }

    arquivo.close();
}

// carrega os funcionarios a partir de um arquivo txt (simula o banco de dados)
void BancoFuncionario::carregarFuncionariosDoArquivo(const std::string& nomeArquivo) {
    std::ifstream arquivo(nomeArquivo, std::ios::in); // abre somente para leitura
    if (!arquivo) {
        throw std::runtime_error("Erro ao abrir o arquivo para carregar funcionários.");
    }

    funcionarios.clear(); // limpa o banco de funcionarios

    std::string linha;
    while (std::getline(arquivo, linha)) {
        std::istringstream stream(linha);
        std::string cargo, nome, id, cpf, email, senha, valor_total_str;
                double valor_total = 0.0; // inicializa o valor total

        // Lê os campos obrigatórios
        std::getline(stream, cargo, ',');
        std::getline(stream, nome, ',');
        std::getline(stream, id, ',');
        std::getline(stream, cpf, ',');
        std::getline(stream, email, ',');
        std::getline(stream, senha, ',');

        // Se for "Caixa" ou "CaixaPCD", tenta ler o valor total vendido
        if (cargo == "caixa" || cargo == "caixapcd") {
            if (!std::getline(stream, valor_total_str, ',')) {
                throw std::runtime_error("Campo valor_total ausente para um cargo de Caixa.");
            }
            try {
                valor_total = std::stod(valor_total_str);
            } catch (const std::invalid_argument&) {
                throw std::runtime_error("Valor total vendido inválido no arquivo.");
            } catch (const std::out_of_range&) {
                throw std::runtime_error("Valor total vendido fora do intervalo válido.");
            }
        }

        if (cargo == "gerente") {
            funcionarios[id] = std::make_unique<Gerente>(nome, id, cpf, email, senha);
        } else if (cargo == "caixa") {
            funcionarios[id] = std::make_unique<Caixa>(nome, id, cpf, email, senha, valor_total);
        } else if (cargo == "caixapcd") {
            funcionarios[id] = std::make_unique<CaixaPcd>(nome, id, cpf, email, senha, valor_total);
        } else {
            throw std::runtime_error("Cargo desconhecido no arquivo.");
        }
    }

    // fecha arquivo
    arquivo.close();
}

// Função para validar e formatar o CPF no formato xxx.xxx.xxx-xx
std::string BancoFuncionario::filtrarCpf(const std::string& cpf) {
    // Remove quaisquer caracteres que não sejam dígitos
    std::string apenasNumeros;
    for (char c : cpf) {
        if (std::isdigit(c)) {
            apenasNumeros += c;
        }
    }

    // Verifica se a quantidade de números é válida (11 dígitos)
    if (apenasNumeros.size() != 11) {
        throw std::invalid_argument("CPF inválido. Deve conter exatamente 11 dígitos.");
    }

    // Formata o CPF no formato xxx.xxx.xxx-xx
    return apenasNumeros.substr(0, 3) + "." + 
           apenasNumeros.substr(3, 3) + "." + 
           apenasNumeros.substr(6, 3) + "-" + 
           apenasNumeros.substr(9, 2);
}

// Função que converte uma string para minúsculas
std::string BancoFuncionario::toLowerCase(const std::string& palavra) {
    std::string result = palavra;
    for (size_t i = 0; i < result.length(); i++) {
        result[i] = std::tolower(result[i]);
    }
    return result;
}