import socket

# Server configuration
HOST = '127.0.0.1'  # The server's hostname or IP address
PORT = 8080         # The port used by the server

# Create a socket object
with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as server_socket:
    # Bind the socket to the address
    server_socket.bind((HOST, PORT))

    # Listen for incoming connections
    server_socket.listen()

    print(f'Server listening on {HOST}:{PORT}')

    # Accept incoming connection
    client_socket, client_address = server_socket.accept()
    print(f'Connected to {client_address}')

    # Receive data from the client
    while True:
        data = client_socket.recv(1024)
        if not data:
            break
        print(f'Received data from client: {data.decode()}')

    # Close the connection
    client_socket.close()