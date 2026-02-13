# 🧩 Palavras Cruzadas (Crossword Solver)

Projeto desenvolvido para a disciplina **Estrutura de Dados II**.

Um solucionador de palavras cruzadas com ajuda de dicas baseadas nos significados das palavras em **C puro** utilizando **Backtracking** com visualização gráfica em **Raylib**.

---

## 🎯 Objetivos

Aplicar na prática conceitos de:

* Backtracking
* Estruturas de dados em grid
* Validação de restrições
* Internacionalização (PT/EN)
* Integração com API de dicionário
* Visualização de algoritmos

---

## 🛠️ Tecnologias

* Linguagem: **C**
* Interface Gráfica: **Raylib**
* Algoritmo principal: **Backtracking**
* Plataforma: **Windows** 

---

## 📂 Estrutura (planejada)

```
├── src/
│   ├── app/
│   │   └── main.c
│   │
│   ├── ui/
│   │   ├── interface.c
│   │   ├── interface.h
│   │   ├── ui_components.c
│   │   └── ui_components.h
│   │
│   ├── game/
│   │   ├── solver.c
│   │   ├── solver.h
│   │   ├── validation.c
│   │   ├── validation.h
│   │   ├── game_mode.c
│   │   └── game_mode.h
│   │
│   ├── data/
│   │   ├── grid.c
│   │   ├── grid.h
│   │   ├── dictionary.c
│   │   ├── dictionary.h
│   │   └── structs.h
│   │
│   ├── external/
│   │   ├── api_client.c
│   │   └── api_client.h
│
├── data_files/
│   ├── dictionaries/
│   │   ├── pt.txt
│   │   └── en.txt
│
├── docs/
└── Makefile

```

---

## 🚧 Status do Projeto

Em desenvolvimento acadêmico.

---

## 👥 Equipe

* Daiane Santos ([@daianesnts](https://github.com/daianesnts/))
* Guilhereme Seixas ([@guilheeme1108-prog](https://github.com/guilheeme1108-prog/))
* Igor Dias ([@iidias](https://github.com/iidias/))
* Yasmim Passos ([@yasmim-passos](https://github.com/yasmim-passos/))

Projeto realizado em grupo como atividade avaliativa da disciplina de Estrutura de Dados II, na Universidade Federal de Sergipe (UFS).
