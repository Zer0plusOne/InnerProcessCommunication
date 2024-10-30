#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

// Funcion que manda a la pipe el numero del array que he definido en main
void enviarNumero(int pipe_fd, int numero) {
    write(pipe_fd, &numero, sizeof(numero));  // Se escribe en la pipe
}

// Funcion que recoje el contenido de la pipe y lo devuelve como resultado de la funcion
int recibirNumero(int pipe_fd) {
    int numero;
    read(pipe_fd, &numero, sizeof(numero));  // Se lee de la pipe
    return numero; // Retornamos el resultado como numero de la funcion
}

// Funcion del proceso padre Multiplica el numero que se le mande
void procesoPadre(int pipe_padre_a_hijo[2], int pipe_hijo_a_impar[2], int id) {
    close(pipe_padre_a_hijo[1]);  // Como con la llamada ya se rellena, cerramos el pipe del padre

    // Recibir el número del padre
    int numero = recibirNumero(pipe_padre_a_hijo[0]);
    close(pipe_padre_a_hijo[0]);  // Cerrar el pipe del padre

    // El numero recibido se multiplica por dos
    int resultado = numero * 2;

    // Mandamos el numero multiplicado al hijo para que este haga sus operaciones
    enviarNumero(pipe_hijo_a_impar[1], resultado);
    close(pipe_hijo_a_impar[1]);  // Cerramos la escritura del pipe

    // Mostramos el proceso
    printf("Padre (%d): recibe %d del gran padre, multiplica y envía %d al hijo.\n", id, numero, resultado);
}

// Función para el proceso Hijo que suma 10 a lo que le mande su papa
void procesoHijo(int pipe_hijo_a_padre[2], int id) {
    close(pipe_hijo_a_padre[1]);  // Una vez cargado, lo cerramos asi como con el padre

    // Recibir el número del proceso par
    int numero = recibirNumero(pipe_hijo_a_padre[0]);
    close(pipe_hijo_a_padre[0]);  // Cerramos el pipe

    // Sumamos 10 al numero que hemos recibido
    int resultado = numero + 10;

    // Devolvemos el numero a su papa
    enviarNumero(pipe_hijo_a_padre[1], resultado);
    
    // Mensaje de comunicación
    printf("Hijo (%d): recibe %d y suma 10 para enviar %d a su padre.\n", id, numero, resultado);

}

int main() {
    // Creamos los pipes emparejandolos segun si son pares o inpares (0 es par)
    int pipes[5][2][2];  // Declaro todos los pipes dentro de un array tal que asi:
                         // - pipes[i][0]: pipe para la comunicacion del proceso padre al proceso par (índice i).
                         // - pipes[i][1]: pipe para la comunicacion del proceso par al proceso impar (índice i).
                         // - El tamaño del primer índice me permite especificar la cantidad de parejas con las que vamos a trabajar.

    for (int i = 0; i < 5; i++) {
        pipe(pipes[i][0]);  // Pipe padre a par
        pipe(pipes[i][1]);  // Pipe par a impar
    }

    // Listado de numeros que vamos a mandar
    int numeros[] = {33, 420, 69, 13, 777};  // Es una larga historia cada numero elegido
    int n = sizeof(numeros) / sizeof(numeros[0]);

    for (int i = 0; i < n; i++) {
        pid_t pid_par = fork();  // Creacion del proceso par
        if (pid_par == 0) {
            // Proceso par
            procesoPadre(pipes[i][0], pipes[i][1], i * 2);  // Id del proceso par segun su iteracion en el for
            exit(0);
        } else {
            // Crear proceso impar
            pid_t pid_impar = fork();
            if (pid_impar == 0) {
                // Proceso impar
                procesoHijo(pipes[i][1], i * 2 + 1);  // Id del proceso inpar segun su iteracion en el for
                exit(0);
            }
        }

        // Proceso padre envía el número al proceso par
        enviarNumero(pipes[i][0][1], numeros[i]);
        close(pipes[i][0][1]);  // Cerrar la escritura del pipe del padre
    }

    // Esperar a que terminen todos los hijos (chivatazo de chatGPT: no era capaz de encontrar el motivo por el que se sobreescribian algunos mensajes o porque el orden no era correcto o porque aveces se conejelaba la comunicacion entre procesos)
    for (int i = 0; i < n * 2; i++) {
        wait(NULL);  // Espera por defecto
    }

    printf("\033[0;32mCompletado\033[0m\n"); // colorin colorado...
    return 0;
}
