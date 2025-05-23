import socket


host = '127.0.0.1'
port = 7000

client_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

try:
    client_socket.connect((host, port))
    print(f"Connected to {host}:{port}")

    message = "DK won"
    client_socket.sendall(message.encode())
    print("Sent")

finally:
    client_socket.close()
