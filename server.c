#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <time.h>

#define PORT 8080
#define BUFFER_SIZE 1024

const char *get_current_time() {
    static char time_str[100];
    time_t now = time(NULL);
    struct tm *t = localtime(&now);
    strftime(time_str, sizeof(time_str), "%Y-%m-%d %H:%M:%S", t);
    return time_str;
}

void handle_request(int client_socket) {
    char buffer[BUFFER_SIZE];
    read(client_socket, buffer, BUFFER_SIZE - 1);
    
    const char *http_response = 
        "HTTP/1.1 200 OK\r\n"
        "Content-Type: text/html; charset=UTF-8\r\n"
        "Connection: close\r\n"
        "\r\n"
        "<!DOCTYPE html>"
        "<html lang='es'>"
        "<head>"
        "<meta charset='UTF-8'>"
        "<meta name='viewport' content='width=device-width, initial-scale=1.0'>"
        "<title>Servidor en C</title>"
        "<style>"
        "body { font-family: Arial, sans-serif; background: linear-gradient(135deg, #667eea 0%, #764ba2 100%); "
        "margin: 0; padding: 0; display: flex; justify-content: center; align-items: center; height: 100vh; }"
        ".container { background: white; padding: 40px; border-radius: 10px; box-shadow: 0 10px 30px rgba(0,0,0,0.3); text-align: center; }"
        "h1 { color: #667eea; margin-bottom: 20px; }"
        "p { color: #555; font-size: 18px; margin: 10px 0; }"
        ".highlight { color: #764ba2; font-weight: bold; }"
        ".info { background: #f0f0f0; padding: 15px; border-radius: 5px; margin-top: 20px; }"
        "</style>"
        "</head>"
        "<body>"
        "<div class='container'>"
        "<h1> Servidor HTTP en C</h1>"
        "<p>Desarrollado con <span class='highlight'>C puro</span> y <span class='highlight'>sockets</span></p>"
        "<div class='info'>"
        "<p><strong>Puerto:</strong> 8080</p>"
        "<p><strong>Hora del servidor:</strong> %s</p>"
        "<p><strong>CI/CD:</strong> GitHub Actions + Docker</p>"
        "</div>"
        "<p style='margin-top: 20px; font-size: 14px; color: #888;'>Integración y despliegue continuo automatizado</p>"
        "</div>"
        "</body>"
        "</html>";
    
    char response[4096];
    snprintf(response, sizeof(response), http_response, get_current_time());
    
    write(client_socket, response, strlen(response));
    close(client_socket);
}

int main() {
    int server_socket, client_socket;
    struct sockaddr_in server_addr, client_addr;
    socklen_t client_len = sizeof(client_addr);
    
    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket < 0) {
        perror("Error al crear socket");
        exit(1);
    }
    
    int opt = 1;
    setsockopt(server_socket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
    
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);
    
    if (bind(server_socket, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        perror("Error en bind");
        exit(1);
    }
    
    if (listen(server_socket, 10) < 0) {
        perror("Error en listen");
        exit(1);
    }
    
    printf("Servidor HTTP en C escuchando en puerto %d...\n", PORT);
    printf("Accede desde: http://localhost:%d\n", PORT);
    
    while (1) {
        client_socket = accept(server_socket, (struct sockaddr*)&client_addr, &client_len);
        if (client_socket < 0) {
            perror("Error al aceptar conexión");
            continue;
        }
        
        handle_request(client_socket);
    }
    
    close(server_socket);
    return 0;
}