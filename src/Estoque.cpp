#include "Estoque.h"
#include "Tela.h"
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <iostream>
#include <thread>
#include <algorithm>

// construtor inicializa o banco de dados com alguns funcionários
Estoque::Estoque() {
    try {
        carregarEstoqueDoArquivo("data/estoque.txt");
    } catch (const std::exception& e) {
        std::cerr << "Erro ao carregar estoque: " << e.what() << "\n";
        std::cerr << "Iniciando com banco de dados vazio.\n";
    }
}

void Estoque::gerenciarEstoque(Funcionario* gerente, Estoque& estoque) {
    int escolha;
    do {
        try {
            Tela::limpar(); // limpa tela

            std::cout << "=============================================\n";
            std::cout << "          GERENCIAMENTO DE ESTOQUE           \n";
            std::cout << "=============================================\n";
            std::cout << "1. Adicionar novo lote de um produto        \n";
            std::cout << "2. Remover lote                             \n";
            std::cout << "3. Exibir estoque                           \n";
            std::cout << "4. Alterar preço de um produto              \n";
            std::cout << "0. Sair                                     \n";
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
                    std::string nome, data_validade, descricao;
                    int tamanho;
                    double preco;
                    const size_t limite_nome = 70;
                    const size_t limite_descricao = 100;

                    Tela::limpar();
                    std::cout << "=============================================\n";
                    std::cout << "              ADICIONANDO PRODUTO           \n";
                    std::cout << "=============================================\n";
                    std::cout << "Nome: ";
                    std::cin.ignore();
                    std::getline(std::cin, nome);

                    if (nome.size() > limite_nome) {
                        throw std::out_of_range("O nome do produto deve ter no máximo " + std::to_string(limite_nome) + " caracteres.");
                    }

                    std::string codigo = gerarCodigoLote(); // Gera o código automaticamente

                    std::cout << "Data de validade (dd/mm/aaaa): ";
                    std::cin >> data_validade;
                    std::cout << "Quantidade: ";
                    std::cin >> tamanho;

                    if (std::cin.fail() || tamanho <= 0) {
                        throw std::invalid_argument("Quantidade inválida.");
                    }
                    
                    preco = estoque.gerarPrecoLote(nome); // configura o preço do produto

                    std::cout << "Descrição: ";
                    std::cin.ignore();
                    std::getline(std::cin, descricao);

                    if (descricao.size() > limite_descricao) {
                        throw std::out_of_range("A descrição deve ter no máximo " + std::to_string(limite_descricao) + " caracteres.");
                    }

                    // cria um lote temporário no heap para armazenar no estoque.
                    std::unique_ptr<Lote> lote = std::make_unique<Lote>(nome, codigo, data_validade, tamanho);
                    for (int i = 0; i < tamanho; i++) {
                        lote->preencherLote(nome, i + 1, preco, descricao);
                    }

                    if (lote->isExpired()) {
                        throw std::runtime_error("O lote contém produtos fora da validade.");
                    }

                    estoque.adicionarLote(gerente, std::move(lote)); // adiciona lote no estoque
                    estoque.salvarEstoqueNoArquivo("data/estoque.txt"); // salva o lote no arquivo texto
                    break;
                }

                case 2: {
                    std::string codigo;
                    std::cout << "Código do lote a ser removido: ";
                    std::cin >> codigo;

                    estoque.removerLote(gerente, codigo);  // remove lote do estoque
                    estoque.salvarEstoqueNoArquivo("data/estoque.txt"); // salva o lote no arquivo texto
                    break;
                }

                case 3:
                    exibirEstoque();    // exibe estoque na tela
                    break;

                case 4: {   // altera preço do produto
                    std::string nomeProduto;
                    Tela::limpar(); 
                    std::cout << "=============================================\n";
                    std::cout << "                ALTERANDO PREÇO           \n";
                    std::cout << "=============================================\n";
                    std::cout << "Nome do produto para alterar o preço: ";
                    std::cin.ignore();
                    std::getline(std::cin, nomeProduto);

                    alterarPreco(nomeProduto);  
                    estoque.salvarEstoqueNoArquivo("data/estoque.txt");
                    break;
                }

                case 0:
                    break;

                default:
                    throw std::invalid_argument("Opção inválida. Escolha um número entre 0 e 4.");
            }

        } catch (const std::exception& e) {
            std::cerr << "Erro: " << e.what() << "\n";
            std::cin.clear();
            std::this_thread::sleep_for(std::chrono::milliseconds(1500));
        }
    } while (escolha != 0);
}



// busca o produto(lote) por nome
std::vector<std::pair<Lote*, int>> Estoque::buscarTodosLotesPorNome(const std::string& nomeProduto) {
    std::vector<std::pair<Lote*, int>> lotesDisponiveis;

    for (auto& [codigo, lote] : estoque_lotes) {      // itera sobre todos os lotes no estoque
        if(lote->getNome() == nomeProduto && lote->isExpired()){
            throw std::runtime_error("Produtos do lote " + lote->getCodigo() + " estão vencidos.");
        }

        if (lote->getNome() == nomeProduto && lote->getTamanho() > 0) {    // verifica nome e se esta vazio
            lotesDisponiveis.emplace_back(lote.get(), lote->getTamanho());  // adiciona ao par Lote*, tamanho
        }
    }

    // Ordena os lotes pela data de validade (mais próximo primeiro)
    std::sort(lotesDisponiveis.begin(), lotesDisponiveis.end(), [](const auto& a, const auto& b) {
        return a.first->getValidade() < b.first->getValidade();
    });

    return lotesDisponiveis;    // retorna um vetor com pares de lotes com mesmo nome e não vazios 
}

// adiciona um lote ao estoque (gerente tem permissão)
void Estoque::adicionarLote(Funcionario* gerente, std::unique_ptr<Lote> lote){
    if (gerente->getCargo() != "gerente") {
        throw std::runtime_error("Apenas gerentes podem adicionar novos funcionarios.");
    }

    const std::string& codigo = lote->getCodigo();
    if (estoque_lotes.find(codigo) != estoque_lotes.end()) {
        throw std::runtime_error("Ja existe um lote com este codigo.");
    }

    estoque_lotes[codigo] = std::move(lote);
    std::cout << "Lote adicionado com sucesso!\n";
}

// remove um lote do estoque
void Estoque::removerLote(Funcionario* gerente, const std::string& codigo){
    if (gerente->getCargo() != "gerente") {
        throw std::runtime_error("Apenas gerentes podem remover funcionários.");
    }

    auto it = estoque_lotes.find(codigo);
    if(it == estoque_lotes.end()){
        throw std::runtime_error("Lote não existe");
    }

    estoque_lotes.erase(it);
    std::cout << "Lote removido.\n";
}

// salva o estoque em um arquivo txt (simula um banco de dados)
void Estoque::salvarEstoqueNoArquivo(const std::string& nomeArquivo) const{
    std::ofstream arquivo(nomeArquivo, std::ios::out); // abre para escrita e concatena o final
    if (!arquivo) {
        throw std::runtime_error("Erro ao abrir o arquivo para salvar Estoque.");
    }

    for (const auto& [codigo, lote] : estoque_lotes) { // percorre o Estoque e salva lote e produtos
        
        if(lote->getTamanho() > 0){
            arquivo << lote->getNome() << ","
                    << lote-> getCodigo()<< ","
                    << lote->getValidade() << ","
                    << lote->getTamanho() << "\n";
            for(size_t i=0; i<lote->getTamanho(); i++){
                Produto produto = lote->pesquisarProduto(static_cast<int>(i));
                arquivo << produto.getNome() << ","
                    << produto.getId() << ","
                    << produto.getPreco() << ","
                    << produto.getDescricao() << "\n";
            }        
        }
    }
    arquivo.close();
}

// carrega o estoque a partir de um arquivo txt (simula um banco de dados)
void Estoque::carregarEstoqueDoArquivo(const std::string& nomeArquivo){
    
    std::ifstream arquivo(nomeArquivo, std::ios::in); // abre somente para leitura
    if (!arquivo) {
        throw std::runtime_error("Erro ao abrir o arquivo para carregar estoque.");
    }

    estoque_lotes.clear(); // limpa o estoque

    std::string linha;
    while (std::getline(arquivo, linha)) {
        std::istringstream stream(linha);
        std::string nome, codigo, validade, descricao, tamanhoStr, idStr, precoStr;
        int id, tamanho;
        double preco;

        try {
            std::getline(stream, nome, ',');
            std::getline(stream, codigo, ',');
            std::getline(stream, validade, ',');
            std::getline(stream, tamanhoStr, ',');

            if (nome.empty() || codigo.empty() || validade.empty() || tamanhoStr.empty()) {
                throw std::runtime_error("Formato inválido no arquivo para o lote.");
            }

            // Converter tamanho
            tamanho = std::stoi(tamanhoStr);

            // Adiciona o lote ao estoque
            std::unique_ptr<Lote> lote = std::make_unique<Lote>(nome, codigo, validade, tamanho);
            estoque_lotes[codigo] = std::move(lote);

            for (int i = 0; i < tamanho; i++) {
                if (!std::getline(arquivo, linha)) {
                    throw std::runtime_error("Numero de produtos menor que o tamanho especificado para o lote.");
                }

                std::istringstream stream(linha);
                std::getline(stream, nome, ',');
                std::getline(stream, idStr, ',');
                std::getline(stream, precoStr, ',');
                std::getline(stream, descricao, ',');

                if (nome.empty() || idStr.empty() || precoStr.empty() || descricao.empty()) {
                    throw std::runtime_error("Formato inválido no arquivo para o produto.");
                }

                // Conversões
                id = std::stoi(idStr);
                preco = std::stod(precoStr);


                // Adiciona os produtos no lote
                Produto produto(nome, id, preco, descricao);
                estoque_lotes[codigo]->adicionarProduto(produto);

            }

        } catch (const std::exception& e) {
            throw std::runtime_error("Falha ao processar arquivo");
        }
    }
    arquivo.close();
}

// exibe estoque na tela
void Estoque::exibirEstoque(){
    // pega o caminho do arquivo
    std::string nomeArquivo = "data/estoque.txt";

    // Verifica se o arquivo existe
    std::ifstream arquivo(nomeArquivo);
    if (!arquivo.is_open()) {
        throw std::runtime_error("Falha ao tentar abrir arquivo estoque");
    }
    arquivo.close(); // Fecha o arquivo, pois só queremos verificar sua existência

    // Abre o arquivo no editor de texto padrão
    #if defined(_WIN32) || defined(_WIN64)
        // Comando para Windows
        std::string comando = "start \"\" \"" + nomeArquivo + "\"";
    #elif defined(__linux__)
        // Comando para Linux
        std::string comando = "xdg-open \"" + nomeArquivo + "\"";
    #elif defined(__APPLE__)
        // Comando para macOS
        std::string comando = "open \"" + nomeArquivo + "\"";
    #else
        throw std::runtime_error("Sistema operacional não suportado.");
    #endif

    int resultado = system(comando.c_str());

    // Verifica se houve erro ao abrir o editor de texto
    if (resultado != 0) {
        throw std::runtime_error("Erro ao abrir o editor de texto padrão.");
    }
}

// alterar preço de um produto
void Estoque::alterarPreco(const std::string nome_produto){
    auto lotes = buscarTodosLotesPorNome(nome_produto);

    if(lotes.empty()){
        throw std::invalid_argument("Produto não encontrado");
    }
    
    std::cout << "Preço atual de " << lotes[0].first->getNome() 
    << ": R$ " << lotes[0].first->getProdutosPreco() 
    << "\nInsira novo preço: ";

    double novoPreco;
    std::cin >> novoPreco; // recebe novo preço do gerente

    if(novoPreco <= 0.0){
        throw std::invalid_argument("Novo preço inválido.");
    }

    for(auto& lote : lotes){
        lote.first->setProdutoPreco(novoPreco); // altera o preço dos produtos
    }
}

// gera código para os lotes
std::string Estoque::gerarCodigoLote() {
    int maiorNumero = 0;

    // Percorre o mapa para encontrar o maior número de lote
    for (const auto& [codigo, lote] : estoque_lotes) {
        if (codigo.size() > 1 && codigo[0] == 'L' && isdigit(codigo[1])) {
            try {
                // Extrai a parte numérica do código
                int numero = std::stoi(codigo.substr(1));
                if (numero > maiorNumero) {
                    maiorNumero = numero;
                }
            } catch (const std::exception& e) {
                // Ignora erros de conversão (códigos inválidos, etc.)
                continue;
            }
        }
    }

    // Incrementa para gerar o próximo código
    maiorNumero++;
    return "L" + std::to_string(maiorNumero);
}

// garante que produtos iguais de lote diferente tenham o mesmo preço
double Estoque::gerarPrecoLote(const std::string& nome){
    
    double preco = 0.0;
    // percorre o estoque para achar produto com msm nome
    for (const auto& lote : estoque_lotes) {
        if(lote.second->getNome() == nome){
            preco = lote.second->getProdutosPreco();
        }
    }
    // se for produto novo pede entrada ao usuário
    if(preco <= 0.0){
        std::cout << "Preço da unidade: ";
        std::cin >> preco;
    }

    if (std::cin.fail() || preco <= 0.0) {
        throw std::invalid_argument("Preço inválido.");
    }

    return preco;
}