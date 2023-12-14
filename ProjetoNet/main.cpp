#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <chrono>
#include <thread>
#include <math.h>

int NUM_MAQUINAS = 64;
int MAX_TENTATIVAS = 10;

std::vector<std::pair<std::string, int>> buffer(NUM_MAQUINAS);

int pacotesEnviadosComSucesso = 0;
int totalPacotesEnviados = 0;

void enviarPacote(int maquinaOrigem, int tentativa = 1) {
    int bytes = rand() % 200 + 1;
    int maquinaDestino;

    do {
        maquinaDestino = rand() % NUM_MAQUINAS;
    } while (maquinaDestino == maquinaOrigem);

    std::cout << "Maquina " << maquinaOrigem << " enviando " << bytes << " bytes para Maquina " << maquinaDestino << " (Tentativa: " << tentativa << ")" << std::endl;

        if (maquinaDestino >= 0 && maquinaDestino < NUM_MAQUINAS) {
        if (!buffer[maquinaDestino].first.empty()) {
            std::cout << "Colisao! Maquina " << maquinaOrigem << " e Maquina " << maquinaDestino << std::endl;

                if (tentativa <= MAX_TENTATIVAS) {

                int espera = rand() % static_cast<int>(pow(2, tentativa) * 1000);
                std::this_thread::sleep_for(std::chrono::milliseconds(espera));

                enviarPacote(maquinaOrigem, tentativa + 1);
            } else {
                std::cout << "Numero maximo de tentativas atingido. Desistindo da transmissao." << std::endl;
                        buffer[maquinaDestino] = {"Colisao", 0};
            }
        } else {
            if (bytes <= 100) {
                buffer[maquinaDestino] = {"Pacote de Maquina " + std::to_string(maquinaOrigem) + " OK", bytes};
                pacotesEnviadosComSucesso++;
            } else {
                std::cout << "Erro! Pacote excede 100 bytes. Desistindo da transmissao." << std::endl;
                        buffer[maquinaDestino] = {"Erro", 0};
            }
            totalPacotesEnviados++;
        }
    } else {
        std::cout << "Destino invalido: " << maquinaDestino << std::endl;
    }
}

void simularRede() {
    srand(static_cast<unsigned>(time(0)));

    auto start = std::chrono::system_clock::now();
    while (std::chrono::duration_cast<std::chrono::seconds>(std::chrono::system_clock::now() - start).count() < 60) {
        int maquinaOrigem = rand() % NUM_MAQUINAS;

        enviarPacote(maquinaOrigem);

        std::this_thread::sleep_for(std::chrono::milliseconds(rand() % 1000));
    }

    std::cout << "\nConteudo do Buffer:\n";
        for (int i = 0; i < NUM_MAQUINAS; ++i) {
        std::cout << "Maquina " << i << ": " << buffer[i].first << " (" << buffer[i].second << " bytes)" << std::endl;
    }

    double porcentagemSucesso = (static_cast<double>(pacotesEnviadosComSucesso) / totalPacotesEnviados) * 100;

    std::cout << "\nPorcentagem de sucesso: " << porcentagemSucesso << "%" << std::endl;
}

int main() {
    simularRede();

    return 0;
}
