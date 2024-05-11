#include <iostream>
#include <fstream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <cstring>
#include <string>

const int PORT = 8080;
const std::string HTML_FILE = "index.html";

std::string read_html_file(const std::string& filename) {
    std::ifstream file(filename);
    std::string html_content((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
    return html_content;
}

int main() {
    int server_fd, new_socket;
    struct sockaddr_in address;
    int addrlen = sizeof(address);

    // Create a socket
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    // Set up the address structure
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    // Bind the socket to the specified port
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }

    // Listen for incoming connections
    if (listen(server_fd, 3) < 0) {
        perror("listen");
        exit(EXIT_FAILURE);
    }

    std::cout << "Server is running on port " << PORT << std::endl;

    while (true) {
        // Accept incoming connection
        if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen)) < 0) {
            perror("accept");
            exit(EXIT_FAILURE);
        }

        // Read the HTTP request (not used in this example)

        // Send the HTTP response with proper headers
        std::string html_content = read_html_file(HTML_FILE);
        std::string http_response = "HTTP/1.1 200 OK\nContent-Type: text/html\nContent-Length: " + std::to_string(html_content.length()) + "\n\n" + html_content;
        send(new_socket, http_response.c_str(), http_response.length(), 0);

        // Close the socket
        close(new_socket);
    }

    return 0;
}
