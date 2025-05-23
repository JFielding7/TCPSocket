import socket

host = '127.0.0.1'
port = 7000

client_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

try:
    client_socket.connect((host, port))
    print(f"Connected to {host}:{port}")

    message = "DK won"
    while 1:
        client_socket.sendall(message.encode())
        print("Sent")
        message = input()

finally:
    client_socket.close()
