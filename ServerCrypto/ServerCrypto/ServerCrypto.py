
import socket

def generate_key_order(key):
    
    key = key.upper()
    sorted_key = sorted(list(key))
    order = []
    for c in key:
        order.append(sorted_key.index(c) + 1)
        sorted_key[sorted_key.index(c)] = '_'  
    return order

def pad_message(message, key_length):
     
    alphabet = 'abcdefghijklmnopqrstuvwxyz'
    pad_len = (key_length - len(message) % key_length) % key_length
    pad_chars = ""
    i = 0
    while len(pad_chars) < pad_len:
        pad_chars += alphabet[i % len(alphabet)]
        i += 1
    return message + pad_chars

def create_matrix(message, key):
    key_length = len(key)
    padded_message = pad_message(message, key_length)
    rows = [padded_message[i:i+key_length] for i in range(0, len(padded_message), key_length)]
    return rows

def encrypt_transposition(message, key):
    
    message = message.replace(" ", "_")
    key_order = generate_key_order(key)
    matrix = create_matrix(message, key)
    
    encrypted_columns = ['' for _ in range(len(key))]
    for col_num in range(1, len(key) + 1):
        idx = key_order.index(col_num)
        for row in matrix:
            encrypted_columns[col_num - 1] += row[idx]
    
    encrypted_message = ' '.join(encrypted_columns)
    return encrypted_message


def are_litere_distincte(s):
    return len(set(s)) == len(s)

def main():
    host = '0.0.0.0'
    port = 12345
    print(f"[SERVER] Listening on {host}:{port}")
    with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as server_sock:
        server_sock.bind((host, port))
        server_sock.listen(1)
        conn, addr = server_sock.accept()
        with conn:
            print(f"[SERVER] Connected by {addr}")
            while True:
                key = input("Introdu cheia (litere DISTINCTE, ex. SALUT): ").upper()
                if not are_litere_distincte(key):
                    print("Cheia trebuie sa contina doar litere DISTINCTE! Incearca din nou.")
                    continue
                break
            message = input("Introdu mesajul de criptat: ")
            encrypted = encrypt_transposition(message, key)
            print(f"[SERVER] Mesaj criptat trimis: {encrypted}")
            to_send = key + "\n" + encrypted
            conn.sendall(to_send.encode('utf-8'))

if __name__ == '__main__':
    main()