## 🇮🇹 ITALIANO

# 🔧 Minishell - 42 School Project

**Minishell** è un progetto della Scuola 42 che consiste nell'implementare
una miniatura di una shell Unix, scritta interamente in C. L'obiettivo
è comprendere a fondo il funzionamento di una shell, la gestione dei
processi, delle pipe, delle variabili d'ambiente, e del parsing degli
input utente.

## 🎯 Obiettivi principali
  - Creare un prompt interattivo che riceve comandi da tastiera.
  - Eseguire comandi semplici e concatenati (es: ls -l | grep txt).
## 📌 Gestire:
  - Quote singole e doppie
  - Redirezioni e .heredoc (>, <, >>, <<)
  - Pipe
  - Variabili d’ambiente
  - Built-in (es: cd, echo, export, unset, exit)
  - Gestione dei segnali (Ctrl+C, Ctrl+\)
  - Eseguire comandi esterni tramite execve.

## 🛠️ Skills sviluppate
  - Parsing complesso e gestione degli errori
  - Processi, fork e file descriptor
  - Costruzione e manipolazione di AST o strutture simili
  - Buona padronanza di C e gestione della memoria
  - Comprensione del comportamento reale di una shell

## 👨‍💻 Come provarlo dalla tua shell
  - $ `make`
  - $ `./minishell` entri nella shell
  - $ Ctrl+D o `exit` per uscire
  - $ `make fclean`

## 🇬🇧 English

# 🔧 Minishell - 42 School Project

**Minishell** is a project from 42 School that consists of implementing  
a miniature version of a Unix shell, entirely written in C. The goal  
is to deeply understand how a shell works, including the handling of  
processes, pipes, environment variables, and user input parsing.

## 🎯 Main objectives
  - Create an interactive prompt that receives keyboard commands.
  - Execute simple and chained commands (e.g.: ls -l | grep txt).
## 📌 Handle:
  - Single and double quotes
  - Redirections and .heredoc (>, <, >>, <<)
  - Pipes
  - Environment variables
  - Built-ins (e.g.: cd, echo, export, unset, exit)
  - Signal handling (Ctrl+C, Ctrl+\)
  - Execute external commands via execve

## 🛠️ Skills developed
  - Complex parsing and error handling
  - Processes, fork, and file descriptors
  - Building and managing AST or similar structures
  - Solid understanding of C and memory management
  - Real shell behavior comprehension

## 👨‍💻 How to try it from your shell
  - $ `make`
  - $ `./minishell` to enter the shell
  - $ Ctrl+D or `exit` to exit
  - $ `make fclean`
