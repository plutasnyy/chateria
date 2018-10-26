#include <pthread.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdio.h>

#define BUF_SIZE 1024
#define NUM_THREADS 5

//struktura zawierająca dane, które zostaną przekazane do wątku
struct thread_data_t {
    int cfd;
};

//wskaźnik na funkcję opisującą zachowanie wątku
void *ThreadBehavior(void *t_data) {
    char buf[7];
    struct thread_data_t *th_data = (struct thread_data_t *) t_data;
    //dostęp do pól struktury: (*th_data).pole
    //TODO (przy zadaniu 1) klawiatura -> wysyłanie albo odbieranie -> wyświetlanie
    while (1) {
        read(th_data->cfd, buf, 7);
        printf("Serwer: %s\n", buf);
    }
    pthread_exit(NULL);
}


//funkcja obsługująca połączenie z serwerem
void handleConnection(int connection_socket_descriptor) {
    //wynik funkcji tworzącej wątek
    int create_result = 0;
    //uchwyt na wątek
    pthread_t thread1;
    char buf[7];
    //dane, które zostaną przekazane do wątku
    struct thread_data_t t_data;
    t_data.cfd = connection_socket_descriptor;

    create_result = pthread_create(&thread1, NULL, ThreadBehavior, (void *) &t_data);
    if (create_result) {
        printf("1 Błąd przy próbie utworzenia wątku, kod błędu: %d\n", create_result);
        exit(-1);
    }
    while (1) {
        fgets(buf, 7, stdin);
        write(connection_socket_descriptor, buf, 7);
    }
    pthread_detach(thread1);
    //TODO (przy zadaniu 1) odbieranie -> wyświetlanie albo klawiatura -> wysyłanie
}


int main(int argc, char *argv[]) {
    int connection_socket_descriptor;
    int connect_result;
    struct sockaddr_in server_address;
    struct hostent *server_host_entity;

    if (argc != 3) {
        fprintf(stderr, "Sposób użycia: %s server_name port_number\n", argv[0]);
        exit(1);
    }

    server_host_entity = gethostbyname(argv[1]);
    if (!server_host_entity) {
        fprintf(stderr, "%s: Nie można uzyskać adresu IP serwera.\n", argv[0]);
        exit(1);
    }

    connection_socket_descriptor = socket(PF_INET, SOCK_STREAM, 0);
    if (connection_socket_descriptor < 0) {
        fprintf(stderr, "%s: Błąd przy probie utworzenia gniazda.\n", argv[0]);
        exit(1);
    }

    memset(&server_address, 0, sizeof(struct sockaddr));
    server_address.sin_family = AF_INET;
    memcpy(&server_address.sin_addr.s_addr, server_host_entity->h_addr, server_host_entity->h_length);
    server_address.sin_port = htons(atoi(argv[2]));
    connect_result = connect(connection_socket_descriptor, (struct sockaddr *) &server_address,
                             sizeof(struct sockaddr));
    if (connect_result < 0) {
        fprintf(stderr, "%s: Błąd przy próbie połączenia z serwerem (%s:%i).\n", argv[0], argv[1], atoi(argv[2]));
        exit(1);
    }

    handleConnection(connection_socket_descriptor);

    close(connection_socket_descriptor);
    return 0;

}
