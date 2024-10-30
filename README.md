# Inter-Process Communication Example

Este repositorio contiene un ejemplo de un programa en C que utiliza comunicación entre procesos a través de pipes. El programa está diseñado para crear múltiples procesos que intercambian datos de forma sincronizada.

## Descripción del Programa

El programa crea un proceso padre y varios procesos hijos. Cada proceso hijo realiza operaciones sobre los números enviados desde el proceso padre, usando pipes para la comunicación. En particular, el proceso padre multiplica un número por 2, y el proceso hijo suma 10 al resultado recibido.

### Funciones Clave

- **enviarNumero**: Envía un número a través de un pipe.
- **recibirNumero**: Recibe un número desde un pipe.
- **procesoPadre**: Multiplica el número recibido por 2 y lo envía al proceso hijo.
- **procesoHijo**: Suma 10 al número recibido y lo envía de vuelta al proceso padre.

## Requisitos

- Un entorno compatible con C
- Acceso a las funciones de sistema para la creación de procesos y pipes (`<unistd.h>`, `<sys/wait.h>`)

## Cómo Compilar y Ejecutar

Para compilar el programa, utiliza el siguiente comando en la terminal:

```bash
gcc -o ipc_example ipc_example.c
```

Para ejecutar el programa:

```bash
./ipc_example
```

## Salida Esperada

El programa imprimirá en la consola los números recibidos y enviados por cada proceso, mostrando el flujo de comunicación entre el proceso padre y los procesos hijos. Al final, se mostrará un mensaje indicando que el proceso ha finalizado.

## Contribuciones

Las contribuciones son bienvenidas. Siéntete libre de hacer un fork del repositorio y enviar pull requests para mejoras o correcciones.

## Licencia

Este proyecto está bajo la Licencia MIT - consulta el archivo [LICENSE](LICENSE) para más detalles.
