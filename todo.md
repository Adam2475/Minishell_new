# fixme

<!-- - [ ] Exit con lo status code dell'ultimo commando invece di 126 quando si fa CTRL + D

    ```
    ^D
    ``` -->
<!-- 
- [ ] Unset deve verificare se il nome della variabile e' corretta o meno. Se c'e' n'e' almeno una non valida, imposta lo status_code a 1 ma prosegue comunque con gli altri argomenti
    ```
    unset PWD 1err HOME
    ``` -->
TODO: impostare err_state a 1 nel caso sopracitato

<!-- - [ ] Export += lascia `1 byte` di leak e non funziona.
    ```
    export cia+=cia
    ^D
    ```
- [ ] Export di una variabile senza un valore (`...=...`) dovrebbe creare la variabile senza mostrarla in `env` e non scrivere `=...` dopo il suo nome
    ```
    export a
    ```
    - Es:
        ```
        export
        ...
        declare -x a
        ...
        ```
        ```
        ...
        env
        ...
        ```
- [ ] `ctrl + \` nell'input principale non deve fare niente
    ```
    echo ^\
    ```
- [ ] `cd` senza argomenti non funziona bene dopo aver fatto `unset` di `HOME`. Cio' funziona pero' con `cd ~`
    ```
    unset HOME
    cd
    ```
    <!-- ```
- [ ] segmentation fault con `""`/`''`
    ```
    ""
    ```
    oppure
    ```
    <builtin> "ciao" > out
    <builtin> "ciao" | ls
    ```
 -->

--------------------------------------------------------------------------------  DONE !!

<!-- - [ ] gestire meglio la priorita' dei dup2 per i builtin:
    - I redirect prevalgono sulle pipe.
        - se ho un redirect di input, ignoro la pipe[0] del commando precedente
        - se ho un redirect di output, ignoro la pipe[1] del commando corrente
    ```
    cat << eof | cat
    ciao
    eof
- [ ] myprompt$ echo caoisao>try/prova madonna puttan
        Type: 12, Value: echo
        Type: 11, Value:  
        Type: 13, Value: caoisao
        Type: 4, Value: >
        Type: 13, Value: try/prova
        Type: 11, Value:  
        Type: 12, Value: madonna --- questa non dovrebbe essere un comando
        Type: 11, Value:  
        Type: 13, Value: puttan
        Type: 7, Value: 
- [ ] `exit` in pipeline non freea tutto prima di uscire
    ```
    exit | echo
- [ ] ignorare l'input se e' composto da solo `whitespaces` (`man isspace`)

- [ ] 'echo by>try/prova jo' nelle builtin il prompt si ripresenta nel file e rimane la' fino a chiusura
    ``` -->
----------------------------------------------------------------------------------------------- DONE | TODO: set up (*data)->local_err_state after command
- [ ] Invece di `errno`/`g_status_code` usare una variabile "locale" in `t_data` in quanto non si possa modificare le variabili globali non propri (a parte quella per gli status code).

- [ ] problema redirect con errore nel file - stampa corretta ma deve interrompere l'esecuzione della pipe senza uscire

- [ ] <--- echo ciao | "|" ---> la seconda pipe dovrebbe essere presa come comando ma viene all'execve gli
    gli arriva una holder vuoto perche' nell'execute_command non trova il tkn_comand dopo la pipe ma tkn_word_in_qt



cat <"./test_files/infile" | echo hi  : LEAKS

'' ctrl + D ''