#include <iostream>
#include <string>
#include <vector>
using namespace std;

// ===================== CLASSE ASTRONAUTA =====================
class Astronauta {
public:
    string cpf;
    string nome;
    int idade;
    bool vivo;
    bool disponivel;
    vector<int> voosCursados;

    Astronauta(string cpf, int idade, string nome) {
        this->cpf = cpf;
        this->nome = nome;
        this->idade = idade;
        this->vivo = true;       // Todo astronauta cadastrado começa vivo [cite: 43]
        this->disponivel = true; // Todo astronauta cadastrado começa disponível [cite: 43]
    }
};

// ===================== CLASSE VOO =====================
// Estados: 0 = planejado, 1 = em curso, 2 = finalizado com sucesso, 3 = finalizado com explosao
class Voo {
public:
    int codigo;
    vector<string> cpfsPassageiros;
    int estado;

    Voo(int codigo) {
        this->codigo = codigo;
        this->estado = 0; // Todo voo começa planejado [cite: 48, 103]
    }

    string nomeEstado() {
        if (estado == 0) return "Planejado";
        if (estado == 1) return "Em curso";
        if (estado == 2) return "Finalizado com sucesso";
        return "Finalizado com explosao";
    }

    bool temPassageiro(string cpf) {
        for (int i = 0; i < (int)cpfsPassageiros.size(); i++) {
            if (cpfsPassageiros[i] == cpf) return true;
        }
        return false;
    }

    void removerPassageiro(string cpf) {
        for (int i = 0; i < (int)cpfsPassageiros.size(); i++) {
            if (cpfsPassageiros[i] == cpf) {
                cpfsPassageiros.erase(cpfsPassageiros.begin() + i); // Uso do erase [cite: 31]
                return;
            }
        }
    }
};

// ===================== SISTEMA =====================
vector<Astronauta> astronautas;
vector<Voo> voos;

int buscarAstronauta(string cpf) {
    for (int i = 0; i < (int)astronautas.size(); i++) {
        if (astronautas[i].cpf == cpf) return i;
    }
    return -1;
}

int buscarVoo(int codigo) {
    for (int i = 0; i < (int)voos.size(); i++) {
        if (voos[i].codigo == codigo) return i;
    }
    return -1;
}

// ===================== OPERAÇÕES =====================

void cadastrarAstronauta(string cpf, int idade, string nome) {
    if (buscarAstronauta(cpf) != -1) {
        cout << "Erro: Nao foi possivel cadastrar. Ja existe um astronauta com o CPF " << cpf << "." << endl; // Mensagem de recusa [cite: 96]
        return;
    }
    astronautas.push_back(Astronauta(cpf, idade, nome));
    cout << "Astronauta " << nome << " cadastrado com sucesso." << endl;
}

void cadastrarVoo(int codigo) {
    if (buscarVoo(codigo) != -1) {
        cout << "Erro: Nao foi possivel cadastrar. Ja existe um voo com o codigo " << codigo << "." << endl; // Mensagem de recusa [cite: 102]
        return;
    }
    voos.push_back(Voo(codigo));
    cout << "Voo " << codigo << " cadastrado com sucesso." << endl;
}

void adicionarAstronauta(string cpf, int codigoVoo) {
    int ia = buscarAstronauta(cpf);
    if (ia == -1) {
        cout << "Erro: Astronauta com CPF " << cpf << " nao cadastrado." << endl; // Regra de cadastro [cite: 109]
        return;
    }
    int iv = buscarVoo(codigoVoo);
    if (iv == -1) {
        cout << "Erro: Voo " << codigoVoo << " nao cadastrado." << endl; // Regra de cadastro [cite: 110]
        return;
    }
    if (voos[iv].estado != 0) {
        cout << "Erro: O voo " << codigoVoo << " nao esta planejado. Ele esta " << voos[iv].nomeEstado() << "." << endl; // Regra de estado [cite: 111]
        return;
    }
    if (!astronautas[ia].vivo) {
        cout << "Erro: O astronauta " << astronautas[ia].nome << " esta morto e nao pode embarcar." << endl; // Regra de vida [cite: 112]
        return;
    }
    if (voos[iv].temPassageiro(cpf)) {
        cout << "Erro: O astronauta " << astronautas[ia].nome << " ja esta no voo " << codigoVoo << "." << endl; // Regra de duplicidade [cite: 114]
        return;
    }
    voos[iv].cpfsPassageiros.push_back(cpf);
    cout << "Astronauta " << astronautas[ia].nome << " adicionado ao voo " << codigoVoo << "." << endl;
}

void removerAstronauta(string cpf, int codigoVoo) {
    int ia = buscarAstronauta(cpf);
    if (ia == -1) {
        cout << "Erro: Astronauta com CPF " << cpf << " nao cadastrado." << endl; // Regra [cite: 120]
        return;
    }
    int iv = buscarVoo(codigoVoo);
    if (iv == -1) {
        cout << "Erro: Voo " << codigoVoo << " nao cadastrado." << endl; // Regra [cite: 121]
        return;
    }
    if (voos[iv].estado != 0) {
        cout << "Erro: O voo " << codigoVoo << " nao esta planejado e nao pode ser alterado." << endl; // Regra [cite: 122]
        return;
    }
    if (!voos[iv].temPassageiro(cpf)) {
        cout << "Erro: O astronauta " << astronautas[ia].nome << " nao esta escalado no voo " << codigoVoo << "." << endl; // Regra [cite: 123]
        return;
    }
    voos[iv].removerPassageiro(cpf);
    cout << "Astronauta " << astronautas[ia].nome << " removido do voo " << codigoVoo << "." << endl;
}

void lancarVoo(int codigoVoo) {
    int iv = buscarVoo(codigoVoo);
    if (iv == -1) {
        cout << "Erro: Voo " << codigoVoo << " nao cadastrado." << endl; // Regra [cite: 130]
        return;
    }
    if (voos[iv].estado != 0) {
        cout << "Erro: O voo " << codigoVoo << " nao esta planejado para lancamento." << endl; // Regra [cite: 131]
        return;
    }
    if (voos[iv].cpfsPassageiros.empty()) {
        cout << "Erro: Lancamento recusado. O voo " << codigoVoo << " nao tem astronautas." << endl; // Regra [cite: 132]
        return;
    }
    
    // Verificacao rigorosa: Todos vivos e disponiveis [cite: 133]
    for (int i = 0; i < (int)voos[iv].cpfsPassageiros.size(); i++) {
        int ia = buscarAstronauta(voos[iv].cpfsPassageiros[i]);
        if (!astronautas[ia].vivo) {
            cout << "Erro: Lancamento recusado. O astronauta " << astronautas[ia].nome << " esta morto na lista." << endl; // Atende regra de recusa se houver morto [cite: 186]
            return;
        }
        if (!astronautas[ia].disponivel) {
            cout << "Erro: Lancamento recusado. O astronauta " << astronautas[ia].nome << " esta indisponivel (ja esta em outro voo em curso)." << endl;
            return;
        }
    }
    
    // Executa o lancamento
    voos[iv].estado = 1; // Passa para em curso [cite: 134]
    for (int i = 0; i < (int)voos[iv].cpfsPassageiros.size(); i++) {
        int ia = buscarAstronauta(voos[iv].cpfsPassageiros[i]);
        astronautas[ia].disponivel = false; // Ficam indisponiveis [cite: 135]
        astronautas[ia].voosCursados.push_back(codigoVoo); // Registra participacao 
    }
    cout << "Voo " << codigoVoo << " lancado com sucesso." << endl;
}

void explodirVoo(int codigoVoo) {
    int iv = buscarVoo(codigoVoo);
    if (iv == -1) {
        cout << "Erro: Voo " << codigoVoo << " nao encontrado." << endl; // Regra [cite: 141]
        return;
    }
    if (voos[iv].estado != 1) {
        cout << "Erro: Apenas voos em curso podem explodir." << endl; // Regra [cite: 142]
        return;
    }
    
    voos[iv].estado = 3; // Finalizado com explosao [cite: 143]
    for (int i = 0; i < (int)voos[iv].cpfsPassageiros.size(); i++) {
        int ia = buscarAstronauta(voos[iv].cpfsPassageiros[i]);
        if (ia != -1) {
            astronautas[ia].vivo = false;       // Morrem [cite: 144]
            astronautas[ia].disponivel = false; // Ficam indisponiveis permanentemente [cite: 145]
        }
    }
    cout << "Voo " << codigoVoo << " explodiu. Todos os astronautas a bordo morreram." << endl;
}

void finalizarVoo(int codigoVoo) {
    int iv = buscarVoo(codigoVoo);
    if (iv == -1) {
        cout << "Erro: Voo " << codigoVoo << " nao encontrado." << endl; // Regra [cite: 152]
        return;
    }
    if (voos[iv].estado != 1) {
        cout << "Erro: Apenas voos em curso podem ser finalizados com sucesso." << endl; // Regra [cite: 153]
        return;
    }
    
    voos[iv].estado = 2; // Finalizado com sucesso [cite: 154]
    for (int i = 0; i < (int)voos[iv].cpfsPassageiros.size(); i++) {
        int ia = buscarAstronauta(voos[iv].cpfsPassageiros[i]);
        // Verifica se realmente esta vivo antes de liberar
        if (ia != -1 && astronautas[ia].vivo) {
            astronautas[ia].disponivel = true; // Voltam a ficar disponiveis [cite: 155]
        }
    }
    cout << "Voo " << codigoVoo << " finalizado com sucesso. Astronautas disponiveis novamente." << endl;
}

void listarVoos() {
    cout << "===== LISTAGEM DE VOOS =====" << endl;

    // Agrupados estritamente na ordem do PDF [cite: 158, 161, 162, 163, 164]
    int ordemEstados[] = {0, 1, 2, 3};
    string nomesEstados[] = {"planejados", "em curso", "finalizados com sucesso", "finalizados com explosao"};

    for (int e = 0; e < 4; e++) {
        cout << "--- " << nomesEstados[e] << " ---" << endl;
        bool algum = false;
        for (int i = 0; i < (int)voos.size(); i++) {
            if (voos[i].estado == ordemEstados[e]) {
                algum = true;
                cout << "Codigo do Voo: " << voos[i].codigo << endl; // Imprime codigo [cite: 166]
                
                if (voos[i].cpfsPassageiros.empty()) {
                    cout << "  -> Sem passageiros registrados." << endl;
                } else {
                    cout << "  -> Passageiros:" << endl;
                    for (int j = 0; j < (int)voos[i].cpfsPassageiros.size(); j++) {
                        string cpf = voos[i].cpfsPassageiros[j];
                        int ia = buscarAstronauta(cpf);
                        if (ia != -1) {
                            // Imprime CPF e nome [cite: 168]
                            cout << "     CPF: " << cpf << " | Nome: " << astronautas[ia].nome << endl; 
                        }
                    }
                }
            }
        }
        if (!algum) {
            cout << "  Nenhum voo neste estado." << endl;
        }
    }
    cout << "============================" << endl;
}

void listarMortos() {
    cout << "===== ASTRONAUTAS MORTOS =====" << endl;
    bool algum = false;
    for (int i = 0; i < (int)astronautas.size(); i++) {
        if (!astronautas[i].vivo) {
            algum = true;
            cout << "CPF: " << astronautas[i].cpf << " | Nome: " << astronautas[i].nome << endl; // Imprime CPF e nome [cite: 174, 175]
            cout << "Voos que participou: ";
            
            if (astronautas[i].voosCursados.empty()) {
                cout << "nenhum.";
            } else {
                for (int j = 0; j < (int)astronautas[i].voosCursados.size(); j++) { // Imprime codigos dos voos participados [cite: 176]
                    if (j > 0) cout << ", ";
                    cout << astronautas[i].voosCursados[j];
                }
            }
            cout << "\n\n";
        }
    }
    if (!algum) {
        cout << "Nenhum astronauta morto registrado." << endl;
    }
    cout << "==============================" << endl;
}

// ===================== MAIN =====================
int main() {
    string comando;
    // O programa le comandos da entrada padrao ate receber FIM [cite: 55]
    while (cin >> comando) {
        if (comando == "FIM") {
            break;
        } else if (comando == "CADASTRAR_ASTRONAUTA") {
            string cpf, nome;
            int idade;
            cin >> cpf >> idade;
            getline(cin >> ws, nome); // Funciona para nomes com espacos no final da linha [cite: 61, 92]
            cadastrarAstronauta(cpf, idade, nome);
        } else if (comando == "CADASTRAR_VOO") {
            int codigo;
            cin >> codigo;
            cadastrarVoo(codigo);
        } else if (comando == "ADICIONAR_ASTRONAUTA") {
            string cpf;
            int codigo;
            cin >> cpf >> codigo;
            adicionarAstronauta(cpf, codigo);
        } else if (comando == "REMOVER_ASTRONAUTA") {
            string cpf;
            int codigo;
            cin >> cpf >> codigo;
            removerAstronauta(cpf, codigo);
        } else if (comando == "LANCAR_VOO") {
            int codigo;
            cin >> codigo;
            lancarVoo(codigo);
        } else if (comando == "EXPLODIR_VOO") {
            int codigo;
            cin >> codigo;
            explodirVoo(codigo);
        } else if (comando == "FINALIZAR_VOO") {
            int codigo;
            cin >> codigo;
            finalizarVoo(codigo);
        } else if (comando == "LISTAR_VOOS") {
            listarVoos();
        } else if (comando == "LISTAR_MORTOS") {
            listarMortos();
        } else {
            cout << "Erro: Comando nao reconhecido pelo sistema." << endl;
        }
    }
    return 0;
}