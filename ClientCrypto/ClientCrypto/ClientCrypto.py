
import socket

def generate_key_order(key):
    key = key.upper()
    sorted_key = sorted(list(key))
    order = []
    for c in key:
        order.append(sorted_key.index(c) + 1)
        sorted_key[sorted_key.index(c)] = '_'  
    return order

def decrypt_transposition(encrypted_message, key):
    key_order = generate_key_order(key)
    columns = encrypted_message.strip().split()
    num_rows = len(columns[0])
    key_len = len(key)
   
    matrix = [['' for _ in range(key_len)] for _ in range(num_rows)]
    for col_num in range(1, key_len + 1):
        idx = key_order.index(col_num)
        col_data = columns[col_num - 1]
        for row in range(num_rows):
            matrix[row][idx] = col_data[row]
     
    decrypted = ""
    for row in matrix:
        decrypted += "".join(row)
    
    return decrypted.replace("_", " ")

def main():
    host = '127.0.0.1'   
    port = 12345
    with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as sock:
        sock.connect((host, port))
        data = sock.recv(4096).decode('utf-8')
        
        key, encrypted = data.split('\n', 1)
        print(f"[CLIENT] Cheie primita: {key}")
        print(f"[CLIENT] Mesaj criptat primit: {encrypted}")
        decrypted = decrypt_transposition(encrypted, key)
        print(f"[CLIENT] Mesaj decriptat: {decrypted}")

if __name__ == '__main__':
    main()