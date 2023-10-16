#include "ArvoreBinaria.hpp"
#include "Pilha.hpp"
#include "Expressao.hpp"
#include <string.h>

ArvoreBinaria::ArvoreBinaria()
{
    raiz = NULL;
}

ArvoreBinaria::~ArvoreBinaria()
{
    Limpa();
}

void ArvoreBinaria::Insere(std::string item)
{
    InsereRecursivo(raiz, item);
}

void ArvoreBinaria::InsereRecursivo(TipoNo *&p, std::string item)
{
    if (p == NULL)
    {
        p = new TipoNo(item);
    }
    else
    {
        size_t pos = p->item.find("e"||"a");
        if (pos != std::string::npos)
        {
            TipoNo *NoEsq = new TipoNo(p->item);
            NoEsq->item[pos] = '0';
            InsereRecursivo(p->esq, NoEsq->item);

            TipoNo *NoDir = new TipoNo(p->item);
            NoDir->item[pos] = '1';
            InsereRecursivo(p->dir, NoDir->item);
        }
    }
}


void ArvoreBinaria::PosOrdem(TipoNo *p)
{
    if (p != NULL)
    {
        PosOrdem(p->esq);
        PosOrdem(p->dir);
        std::string resultado = p.avaliaExpressao()->item;
    }
}

void ArvoreBinaria::Limpa()
{
    ApagaRecursivo(raiz);
    raiz = NULL;
}

void ArvoreBinaria::ApagaRecursivo(TipoNo *p)
{
    if (p != NULL)
    {
        ApagaRecursivo(p->esq);
        ApagaRecursivo(p->dir);
        delete p;
    }
}


std::string ArvoreBinaria::GerarValoracoes(const std::string& expressao, const std::string& variaveis, const std::string& valoracao) {
    if (variaveis.empty()) {
        // Todas as variáveis foram substituídas na valoração, avalie a expressão
        int resultado = AvaliaExpressao(expressao, valoracao);
        return resultado == 1 ? valoracao : "";
    }

    char primeiraVar = variaveis[0];
    std::string restanteVariaveis = variaveis.substr(1);

    std::string valoraçãoComZero = valoracao + primeiraVar + "=0, ";
    std::string valoraçãoComUm = valoracao + primeiraVar + "=1, ";

    std::string resultadoComZero = GerarValoracoes(expressao, restanteVariaveis, valoraçãoComZero);
    std::string resultadoComUm = GerarValoracoes(expressao, restanteVariaveis, valoraçãoComUm);

    return resultadoComZero + resultadoComUm;
}



std::string ArvoreBinaria::Satisfabilidade() {
    std::string valoraçãoSatisfatória = ""; // String para armazenar a valoração satisfatória
    bool existeValoraçãoSatisfatória = encontraValoracaoSatisfatoria(raiz, valoraçãoSatisfatória);

    if (existeValoraçãoSatisfatória) {
        return "1 " + valoraçãoSatisfatória;
    } else {
        return "0";
    }
}

bool ArvoreBinaria::encontraValoracaoSatisfatoria(TipoNo *p, std::string &valoração) {
    if (p == nullptr) {
        return true; // Se chegamos ao final da árvore, a valoração é satisfatória
    }

    // Extrair a variável atual
    std::string variavel = p->item;

    // Verificar o quantificador atual
    if (variavel[0] == 'e') { // Se é um quantificador de existência
        for (char valor : {'0', '1'}) {
            valoração += valor;
            if (encontraValoracaoSatisfatoria(p->esq, valoração)) {
                return true; // Se encontrarmos uma valoração satisfatória, pare
            }
            valoração.pop_back(); // Desfaz a última valoração para tentar outra
        }
    } else if (variavel[0] == 'a') { // Se é um quantificador universal
        for (char valor : {'0', '1'}) {
            valoração += valor;
            if (!encontraValoracaoSatisfatoria(p->esq, valoração)) {
                return false; // Se não encontrarmos uma valoração satisfatória, pare
            }
            valoração.pop_back(); // Desfaz a última valoração para tentar outra
        }
    }

    return false; // Se não foi possível encontrar uma valoração satisfatória
}
