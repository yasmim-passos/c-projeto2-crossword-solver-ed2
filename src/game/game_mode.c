#include "game_mode.h"
#include <stddef.h>

// ========== VARIÁVEIS GLOBAIS (ESTADO DO MÓDULO) ==========

/*
 * Modo de jogo atual
 * Inicializado como MODO_USUARIO (usuário joga manualmente)
 */
static ModoJogo modoAtual = MODO_USUARIO;

/*
 * Estado de execução do solver
 * Inicializado como SOLVER_PARADO
 */
static EstadoExecucaoSolver estadoSolver = SOLVER_PARADO;

// ========== IMPLEMENTAÇÃO - FUNÇÕES DE MODO DE JOGO ==========

ModoJogo obterModoAtual(void) { return modoAtual; }

void definirModo(ModoJogo modo) {
  modoAtual = modo;

  // Se mudou para modo usuário, parar o solver
  if (modo == MODO_USUARIO) {
    estadoSolver = SOLVER_PARADO;
  }
}

bool estaModoSolver(void) { return (modoAtual == MODO_SOLVER); }

bool estaModoUsuario(void) { return (modoAtual == MODO_USUARIO); }

// ========== IMPLEMENTAÇÃO - FUNÇÕES DE ESTADO DO SOLVER ==========

EstadoExecucaoSolver obterEstadoSolver(void) { return estadoSolver; }

void definirEstadoSolver(EstadoExecucaoSolver estado) {
  estadoSolver = estado;

  // Se o solver está executando ou pausado, mudar para modo solver
  if (estado == SOLVER_EXECUTANDO || estado == SOLVER_PAUSADO) {
    modoAtual = MODO_SOLVER;
  }
}

bool solverEstaExecutando(void) { return (estadoSolver == SOLVER_EXECUTANDO); }

bool solverEstaPausado(void) { return (estadoSolver == SOLVER_PAUSADO); }

bool solverConcluiu(void) { return (estadoSolver == SOLVER_CONCLUIDO); }

// ========== IMPLEMENTAÇÃO - FUNÇÕES UTILITÁRIAS ==========

void resetarModoJogo(void) {
  modoAtual = MODO_USUARIO;
  estadoSolver = SOLVER_PARADO;
}

const char *obterNomeModo(void) {
  switch (modoAtual) {
  case MODO_SOLVER:
    return "Solver Automático";
  case MODO_USUARIO:
    return "Modo Usuário";
  default:
    return "Desconhecido";
  }
}

const char *obterNomeEstadoSolver(void) {
  switch (estadoSolver) {
  case SOLVER_PARADO:
    return "Parado";
  case SOLVER_EXECUTANDO:
    return "Executando";
  case SOLVER_PAUSADO:
    return "Pausado";
  case SOLVER_CONCLUIDO:
    return "Concluído";
  default:
    return "Desconhecido";
  }
}