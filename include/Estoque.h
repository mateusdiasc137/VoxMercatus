#ifndef ESTOQUE_H
#define ESTOQUE_H

#include "Lote.h"
#include "Funcionario.h"
#include <memory>
#include <map>

class Estoque{
private:
    std::map<std::string, std::unique_ptr<Lote>> estoque_lotes;

    // gera código para os lotes
    std::string gerarCodigoLote();

    // garante que produtos iguais de lote diferente tenham o mesmo preço
    double gerarPrecoLote(const std::string& nome);
    
public:

    // construtor
    Estoque();

    // gerencia o estoque (gerente tem permissão)
    void gerenciarEstoque(Funcionario* gerente, Estoque& estoque);

    // busca o produto(lote) por nome
    std::vector<std::pair<Lote*, int>> buscarTodosLotesPorNome(const std::string& nomeProduto);
    
    // adiciona um lote ao estoque (gerente tem permissão)
    void adicionarLote(Funcionario* gerente, std::unique_ptr<Lote> lote);
    
    // remove um lote do estoque
    void removerLote(Funcionario* gerente, const std::string& codigo);
    
    // salva o estoque em um arquivo txt (simula um banco de dados)
    void salvarEstoqueNoArquivo(const std::string& nomeArquivo) const;

    // carrega o estoque a partir de um arquivo txt (simula um banco de dados)
    void carregarEstoqueDoArquivo(const std::string& nomeArquivo);

    // exibe estoque na tela
    void exibirEstoque();

    // alterar preço de um produto
    void alterarPreco(const std::string nome_produto);

};

#endif