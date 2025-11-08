#include "BancoFuncionario.h"
#include "Tela.h"
#include "Menu.h"
#include "Estoque.h"
#include <chrono>
#include <thread>

using namespace std::chrono;

// exibe menu inicial
void exibirMenuInicial();

int main() {
    BancoFuncionario banco;
    Estoque estoque;

    try {
        banco.carregarFuncionariosDoArquivo("data/funcionarios.txt");     // carrega o banco de funcionário
    } catch (const std::exception& e) {
        std::cout << "Erro ao carregar funcionarios: " << e.what() << "\n";
        std::cout << "Iniciando com banco de dados vazio.\n";
        std::this_thread::sleep_for(std::chrono::milliseconds(1500)); // aguarda exibição de msg erro
    }

    std::string email, senha;
    int escolha = -1; // inicia o número sentinela 

    do {
        try {
            // Exibindo a tela inicial
            exibirMenuInicial();

            std::cin >> escolha; // loga usuário ou fecha programa

            if (std::cin.fail() || (escolha != 1 && escolha != 0)) {
                std::cin.clear(); // limpa a flag de erro
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // descarta entrada inválida
                throw std::invalid_argument("Opção inválida! Por favor, escolha 1 para login ou 0 para sair.");
            }

            switch (escolha) {
                case 1:
                    while (true) { // Loop para retornar à tela de login em caso de erro
                        try {
                            Tela::limpar(); // limpa tela

                            std::cout << "==================================================\n";
                            std::cout << "                      LOGIN              \n";
                            std::cout << "==================================================\n";

                            std::cout << "Digite seu email: ";
                            std::cin >> email;
                            std::cout << "Digite sua senha: ";
                            std::cin >> senha;

                            Funcionario* funcionario = banco.realizarLogin(email, senha);

                            // Determina o menu com base no cargo do funcionário
                            if (funcionario->getCargo() == "gerente") {
                                Menu::exibirMenuGerente(funcionario, banco, estoque);
                            } else if (funcionario->getCargo() == "caixa" || funcionario->getCargo() == "caixapcd") {
                                Menu::exibirMenuCaixa(funcionario, estoque);
                            } else {
                                std::cout << "Tipo de funcionário não reconhecido.\n";
                                std::this_thread::sleep_for(std::chrono::seconds(2)); // espera por 2 segundos
                            }
                            break; // Sai do loop de login se o login for bem-sucedido
                        } catch (const std::exception& e) {
                            std::cout << "Erro durante o login: " << e.what() << "\n";
                            std::this_thread::sleep_for(std::chrono::seconds(2)); // espera antes de retornar à tela de login
                        }
                    }
                    break;

                case 0:
                    std::cout << "Saindo do programa...\n";
                    return 0; // fecha o programa
            }
        } catch (const std::exception& e) {
            std::cout << "Erro: " << e.what() << "\n";
            std::this_thread::sleep_for(std::chrono::seconds(2)); // espera antes de continuar
        }
    } while (true);

    return 0; // fecha o programa
}


void exibirMenuInicial(){
    // mostra a data no menu iniciar
    system_clock::time_point today = system_clock::now();
    time_t tt;
    tt = system_clock::to_time_t ( today );

    std::string dataAtual = ctime(&tt);
    dataAtual.pop_back(); // Remove o caractere de nova linha gerado por ctime

    Tela::limpar(); // limpa tela

    // Exibindo a tela inicial
    std::cout << "**************************************************\n";
    std::cout << "                  SUPERMERCADO DA CCP             \n\n";
    std::cout << "**************************************************\n";
    std::cout << "                " << dataAtual << "              \n";
    std::cout << "**************************************************\n";
    std::cout << "1. LOGIN\n";
    std::cout << "0. SAIR\n";
    std::cout << "**************************************************\n";
    std::cout << "Escolha uma opção: ";
}
