# Password-Generator
import hashlib
def encrypt(text, shift):
    encrypted_text = ""
    for char in text:
        if char.isalpha():  
            shift_amount = shift % 26
            new_char = chr(((ord(char.lower()) - ord('a') + shift_amount) % 26) + ord('a'))
            encrypted_text += new_char.upper() if char.isupper() else new_char
        else:
            encrypted_text += char  
    return encrypted_text
def decrypt(text, shift):
    return encrypt(text, -shift)  


def hash_text(text):
    return hashlib.sha256(text.encode()).hexdigest()

print("Welcome to the Secure Message Encoder!")
message = input("Enter your message: ")
shift_key = int(input("Enter shift key (number): "))
encrypted_message = encrypt(message, shift_key)
hashed_message = hash_text(encrypted_message)
decrypted_message = decrypt(encrypted_message, shift_key)
print("\n--- Encryption Results ---")
print("Encrypted Message:", encrypted_message)
print("Hashed Message (SHA-256):", hashed_message)
print("\n--- Decryption Results ---")
print("Decrypted Message:", decrypted_message)
