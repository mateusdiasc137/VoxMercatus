#include "Carrinho.h"
#include "CaixaPCD.h"
#include "Tela.h"
#include <iostream>
#include <iomanip>
#include <stdexcept>
#include <thread>
#include <algorithm>

// construtor
Carrinho::Carrinho(Cliente& cliente, Funcionario* caixa, Estoque& estoque) : cliente(cliente), caixa(caixa), estoque(estoque){}

// getters
Cliente& Carrinho::getCliente() const{    return cliente; }
Funcionario* Carrinho::getCaixa() const{    return caixa; }
Estoque& Carrinho::getEstoque() const{     return estoque; }
const std::map<std::string, std::pair<int, double>>& Carrinho::getResumoCarrinho() const{   return resumoCarrinho; }
double Carrinho::getValorTotal() const{     return valor_total;}

// setters
void Carrinho::setValorTotal(double valor_total){   this->valor_total = valor_total; }

// gerencia o carrinho durante a compra
void Carrinho::gerenciarCarrinho(Estoque& estoque) {
    int opcao;

    do {
        // Bloco de validação de entrada para evitar falhas no std::cin
        while (true) {
            try {
                Tela::limpar(); //limpa tela e exibe menu carrinho

                std::cout << "=============================================" << std::endl;
                std::cout << "                    CARRINHO                 " << std::endl;
                std::cout << "=============================================" << std::endl;
                std::cout << "1. Adicionar Produto\n";
                std::cout << "2. Remover Produto\n";
                std::cout << "3. Finalizar Compra\n";
                std::cout << "0. Cancelar Compra\n";
                std::cout << "=============================================" << std::endl;
                std::cout << "Escolha uma opcao: ";

                // abre as opções de acessibilidade para PCD
                if(caixa->getCargo() == "caixapcd"){
                    CaixaPcd* caixa_pcd = dynamic_cast<CaixaPcd*>(caixa);

                    std::string nomeArquivo = "features/accessibility/carrinho_opcao.txt";
                    caixa_pcd->falarTexto(nomeArquivo); // chama TTS
                }
                
                std::cin >> opcao;  // recebe opção do usuário

                if (std::cin.fail()) {  // trata entrada inválida
                    throw std::invalid_argument("Entrada inválida!");
                }

                // Saindo do laço de validação caso a entrada seja válida
                break;

            } catch (const std::exception& e) {
                std::cerr << "Erro: " << e.what() << "\n";
                // Limpa o estado de erro e descarta entradas inválidas
                std::cin.clear(); 
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                // Ativa acessibilidade para PCD em caso de erro
                if (caixa->getCargo() == "caixapcd") {
                    CaixaPcd* caixa_pcd = dynamic_cast<CaixaPcd*>(caixa);
                    std::string nomeArquivo = "features/accessibility/msg_erro01.txt";
                    caixa_pcd->falarTexto(nomeArquivo); // chama TTS
                }
                std::this_thread::sleep_for(std::chrono::milliseconds(1500)); // Espera antes de mostrar o menu novamente
            }
        }
        try {
            switch (opcao) {
                case 1: {   // adiciona produto no carrinho
                    std::string nomeProduto;
                    std::cout << "Digite o nome do produto: ";
                    std::cin.ignore();
                    std::getline(std::cin, nomeProduto);

                    std::vector<std::pair<Lote *, int>> lotesDisponiveis = estoque.buscarTodosLotesPorNome(nomeProduto); // Retorna todos os lotes disponíveis

                    if (lotesDisponiveis.empty()) {
                        throw std::invalid_argument("Produto não encontrado.");
                    }

                    int quantidade;
                    std::cout << "Digite a quantidade: ";
                    std::cin >> quantidade;

                    if (std::cin.fail() || quantidade <= 0) {   // trata entrada inválida
                        throw std::invalid_argument("Quantidade inválida.");
                    }

                    // adiciona produto no carrinho
                    adicionarProdutoAoCarrinho(nomeProduto, quantidade, lotesDisponiveis);
                    
                    break;
                }

                case 2: {   // Remove produto do carrinho
                    std::string nomeProduto;
                    std::cout << "Digite o nome do produto a ser removido: ";
                    std::cin.ignore();
                    std::getline(std::cin, nomeProduto);

                    int quantidade;
                    std::cout << "Digite a quantidade: ";
                    std::cin >> quantidade;

                    if (std::cin.fail() || quantidade <= 0) {
                        throw std::invalid_argument("Quantidade inválida.");
                    }

                    removerProdutoDoCarrinho(nomeProduto, quantidade);                     
                    break;
                }

                case 3: {   // Finaliza a compra
                    finalizarCompra();
                    return; // Sai da função
                }

                case 0: {   // Cancela a compra
                    cancelarCompra();
                    return; // Sai da função
                }

                default:
                    throw std::invalid_argument("Entrada inválida!");
            }

            Tela::limpar(); // limpa tela

            // Exibe o carrinho atualizado
            std::cout << "===================================================================================" << std::endl;
            std::cout << "                                CARRINHO             " << std::endl;
            std::cout << "===================================================================================" << std::endl;
            std::cout << "Quantidade  Produto                            Preço Unitário  Preço Total\n";

            
            // Exibe os produtos agrupados
            double subtotal = 0;  // Subtotal acumulado
            for (const auto& [nome, detalhes] : resumoCarrinho) {
                int quantidade = detalhes.first;
                double precoUnitario = detalhes.second;
                double precoTotal = quantidade * precoUnitario;

                std::cout << std::setw(10) << std::left << quantidade << "  "
                        << std::setw(35) << std::left << nome
                        << std::setw(16) << std::fixed << std::setprecision(2) << precoUnitario
                        << std::setw(16) << precoTotal << std::endl;

                subtotal += precoTotal;  // Soma ao subtotal
                setValorTotal(subtotal); // guarda valor total
            }

            std::cout << "-----------------------------------------------------------------------------------" << std::endl;
            std::cout << "Subtotal acumulado: R$" << std::fixed << std::setprecision(2) << subtotal << std::endl;
            std::cout << "-----------------------------------------------------------------------------------" << std::endl;

            // Aguarda o usuário pressionar Enter
            std::cout << "Pressione Enter para continuar...";
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cin.get();

        } catch (const std::exception& e) {
            // Trata exceções
            std::cout << "Erro: " << e.what() << "\n";
            std::this_thread::sleep_for(std::chrono::milliseconds(1500)); // Pausa para exibir a mensagem
        }       
    }while(opcao != 0);
}

// adiciona produtos ao carrinho
void Carrinho::adicionarProdutoAoCarrinho(const std::string& nomeProduto, int quantidade, std::vector<std::pair<Lote*, int>>& lotesDisponiveis) {
    int quantidadeRestante = quantidade; // Quantidade que ainda precisa ser adicionada

    std::pair<std::string, int> nome_quantidade; // guarda o nome do produto, e a quantidade que dele no carrinho

    for (auto& [lote, disponivel] : lotesDisponiveis) {
        if (quantidadeRestante <= 0) break; // Já adicionou tudo o que foi solicitado

        // Calcula a quantidade que pode ser retirada deste lote
        int quantidadeAdicionar = std::min(quantidadeRestante, disponivel);

        // Atualiza o resumo do carrinho
        resumoCarrinho[nomeProduto].first += quantidadeAdicionar;
        resumoCarrinho[nomeProduto].second = lote->getProdutosPreco();


        // Atualiza a quantidade restante a ser adicionada
        quantidadeRestante -= quantidadeAdicionar;
 
    }

    if (quantidadeRestante > 0) {
        std::cout << "Estoque insuficiente. Apenas " << (quantidade - quantidadeRestante)
                  << " unidades do produto " << nomeProduto << " foram adicionadas ao carrinho.\n";
    } else {
        std::cout << "Todas as " << quantidade << " unidades do produto " << nomeProduto
                  << " foram adicionadas ao carrinho.\n";
        
    }
    std::this_thread::sleep_for(std::chrono::seconds(2)); // espera 2 segundos
}

// remove os produtos do carrinho
void Carrinho::removerProdutoDoCarrinho(const std::string& nomeProduto, int quantidade) {
    if (quantidade <= 0) {
        throw std::invalid_argument("Quantidade a ser removida deve ser maior que zero.");
    }

    // Atualiza o resumo do carrinho
    auto resumoIt = resumoCarrinho.find(nomeProduto);
    if (resumoIt != resumoCarrinho.end()) {
        resumoIt->second.first -= quantidade; // Atualiza a quantidade no resumo
        if (resumoIt->second.first <= 0) {
            resumoCarrinho.erase(resumoIt); // Remove do resumo se a quantidade for zero
        }
    }


    valor_total = 0;
    for (const auto& resumo : resumoCarrinho) {
        valor_total += resumo.second.first * resumo.second.second; // Recalcula o valor total
    }
}

// finaliza a compra limpando os lotes do estoque
void Carrinho::finalizarCompra() {
    // cria uma cópia do carrinho
    std::map<std::string, std::pair<int, double>> resumoCarrinhoCopia = resumoCarrinho;

    for(auto& [nome, detalhes] : resumoCarrinhoCopia){  // itera sobre todos os produtos no carrinho
        auto lotes = estoque.buscarTodosLotesPorNome(nome); // busca todos produtos de mesmo nome do carrinho
        for(auto& lote : lotes){ 
            if(lote.second >= detalhes.first){     // se o tamanho do lote for maior, remove a quantidade restante 
                lote.first->removerProdutos(detalhes.first);
                break;
            }
            else{                  // se o tamanho do lote for menor, remove o lote e atualiza a quantidade restante
                lote.first->removerProdutos(lote.second);
                detalhes.first -= lote.second;
            }
        }
    }
}

// cancela a compra
void Carrinho::cancelarCompra() {
    // Limpa os dados do carrinho após o cancelamento
    resumoCarrinho.clear();
}