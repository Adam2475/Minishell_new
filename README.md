# 🔧 Minishell - 42 School Project

Minishell è un progetto della Scuola 42 che consiste nell'implementare
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
  - $ `Ctrl+D` per uscire
  - $ `make fclean`
