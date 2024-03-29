#include "stdio.h"
#include "string.h"
#include "stdlib.h"
#include "unistd.h"
#include "ctype.h"

//Error CONSTANTES
#define DELIM_TRIM " \t\n"
#define DEFAULT_ERR_MSG "\nOcorreu um problema: "
#define CLOSE_MSG "PROGRAMA ENCERRADO\n"
#define MAX_LINE 80
#define seq_str "jos seq> "
#define par_str "jos par> "

/*    SHELL TYPE
    - 0: shell seq
    - 1: shell par
*/
int shell_type = 0; 

/*    SHELL MODE
    - 0: shell interative
    - 1: shell batch
*/
int shell_mode = 0;


//Global VARS
char *arguments[MAX_LINE/2 + 1]; //A linha de comandos podem ter no máximo 40 argumentos
int should_run = 1; //Flag de controle para ajudar a execução 
FILE *arq_address; //Endereço do arquivo de entrada total
char *commands[MAX_LINE]; //Lista de comandos
char *lines_commands[MAX_LINE]; //Lista de linhas de comandos
char *temp_commands[MAX_LINE];
int lines_count = 0;
int commands_size = 0; //Tamanho da lista de comandos
char *last_line; //Última linha lida
char *str_shell_type = seq_str; //String que indica o tipo de shell
int split_commands_count = 0;
int break_split_count = 0;

// Function PROTOTYPES
void print_err(char *err);
int exists_file_path(char *path);
int process_address(char *file_name);
void shell_loop();
void process_commands(char *command);
void process_file();
void split_commands(char *line);
void exists_exit_command(char *command);
void exists_exit_in_file(char *commands);
char *trim (char *string);

int main(int argc, char **argv) {
    printf("Dev area operating...\n");
    char *address_path = argv[1];

    //Verifica existencia de parametro passando path do arquivo
    if (exists_file_path(address_path) == 1){
        // Verifica existencia do arquivo
        // Verifica existencia de conteúdo no arquivo
        // Verifica extensão do arquivo
        int proceed = process_address(address_path);
        if (proceed == 0){
            shell_mode = 1;
            //inicializar shell por arquivo (batch) caso o shell_mode seja 1
            shell_loop();
        }else{
            //irá encerrar a aplicação com mensagem de erro
            exit(1);
        }
    }

    //inicializar shell por linha de comando caso o shell_mode seja 0
    shell_loop();
    return 0;
}

void print_err(char *err) {
    printf("%s\n", err);
    printf(CLOSE_MSG);
    exit(1);
}

int exists_file_path(char *path){
    if (path == NULL){
        return 0;
    }
    return 1;
}

int process_address(char *file_name){
    char *temp[3];
    *temp = &file_name[strlen(file_name)-3];

    if(strcmp(*temp, "txt") != 0){
        char *ERR = DEFAULT_ERR_MSG"O arquivo passado não é um arquivo .txt";
        print_err(ERR);
        return 1;
    }

    arq_address = fopen(file_name, "r");

    if (arq_address == NULL){
        char *ERR = DEFAULT_ERR_MSG"Ao abrir o arquivo. Está vazio ou não existe";
        fclose(arq_address);
        print_err(ERR);
        return 1;
    }

    return 0;
}

void shell_loop(){
    char line[MAX_LINE];
    while(should_run){
        if (shell_type == 0){
            str_shell_type = seq_str;
            

        }else{
            str_shell_type = par_str;
        }
        //shell interativo
        if (shell_mode == 0){
            printf("%s", str_shell_type);
            fgets(line, MAX_LINE, stdin);

            if (feof(stdin)){
                //resolve problema de Control+D loop infinito
                should_run = 0;
                printf(CLOSE_MSG);
                exit(0);
            }else if(strcmp(line, "exit\n") == 0){
                should_run = 0;
                printf(CLOSE_MSG);
                exit(0);
            }

            process_commands(line);
            fflush(stdout);
        }

        //shell batch
        if (shell_mode == 1){
            //executar comandos lidos
            process_file();
        }
    }
}

void process_commands(char *line){
    //irá dividir os comandos colocando todos dentro do vetor global commands
    split_commands(line);

    for (int i = 0; i < commands_size; i++){
        exists_exit_command(commands[i]);
        system(commands[i]);
    }
}

void process_file(){

    //inicializar o shell por leitura do arquivo
    char line_arq[MAX_LINE];

    pid_t pid;
    int count = 0;

    while (!feof(arq_address)){
        fgets(line_arq, sizeof(line_arq), arq_address);

        split_commands(line_arq);

        commands_size = split_commands_count;
        split_commands_count = 0;
        pid = fork();
        if (pid == 0){
            for (int i = 0; i < commands_size; i++){
                printf("Comando: %s\n", commands[i]);
                exists_exit_command(commands[i]);
                system(commands[i]);
            }
        }else{
            wait(NULL);
        }
        
        if (feof(arq_address)){
            break;
        }
    }

    exit(0);

}

void split_commands(char *line){
    line[strcspn(line, "\n")] = '\0';

    char delimit[] = ";";
    char *token = strtok(line, delimit);
    int count = 0;

    while(token != NULL){
        commands[count++] = trim(token);

        split_commands_count++;

        token = strtok(NULL, delimit);
    }
}

void exists_exit_command(char *command){
    if(strcmp(command, "exit\n") == 0){
        should_run = 0;
        printf(CLOSE_MSG);
        exit(0);
    }else if(strcmp(command, " exit\n") == 0){
        should_run = 0;
        printf(CLOSE_MSG);
        exit(0);
    }else if(strcmp(command, "exit") == 0){
        should_run = 0;
        printf(CLOSE_MSG);
        exit(0);
    }else if(strcmp(command, " exit") == 0){
        should_run = 0;
        printf(CLOSE_MSG);
        exit(0);
    }else if(strcmp(command, "exit \n") == 0){
        should_run = 0;
        printf(CLOSE_MSG);
        exit(0);
    }
}

void exists_exit_in_file(char *commands){
    long initialize_exit = strstr(commands, "exit") - commands;
    // printf("valor: %ld\n", initialize_exit);

    if (initialize_exit < 0){
        printf("Não existe exit no arquivo de entrada\n");
        should_run = 0;
        printf(CLOSE_MSG);
        exit(0);
    }
}

char *trim (char *string){
    size_t  beg = strspn(string, DELIM_TRIM), len = strlen (string);

    if (beg == len) {
        *string = 0;
        return string;
    }

    memmove (string, string + beg, len - beg + 1);
    for (int i = (int)(len - beg - 1); i >= 0; i--) {
        if (isspace(string[i])){
            string[i] = 0;
        }else{
            break;
        }
    }
    return string;
}