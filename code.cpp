#include <iostream>
#include <string>
#include <map>
#include <random>
#include <sstream>
#include <vector>
#include <iomanip> // for std::setw

using namespace std;

// Simplified version of AES encryption and decryption
class AES {
public:
    static string encrypt(const string& plaintext, const string& key) {
        string ciphertext = plaintext;
        // Perform simple XOR encryption with the key
        for (size_t i = 0; i < plaintext.length(); i++) {
            ciphertext[i] ^= key[i % key.length()];
        }
        return ciphertext;
    }

    static string decrypt(const string& ciphertext, const string& key) {
        // Decryption is the same as encryption for a simple XOR cipher
        return encrypt(ciphertext, key);
    }
};

// Simplified version of RSA encryption and decryption
class RSA {
public:
    static string encrypt(const string& plaintext, long long publicKey, long long modulus) {
        string ciphertext;
        for (char c : plaintext) {
            // Encrypt each character using modular exponentiation
            long long encryptedChar = modExp(c, publicKey, modulus);
            // Ensure each encrypted character has a fixed width (for padding)
            ciphertext += std::to_string(encryptedChar) + " ";
        }
        return ciphertext;
    }

    static string decrypt(const string& encryptedMessage, long long privateKey, long long modulus) {
        string decryptedMessage;
        stringstream ss(encryptedMessage);
        string numStr;
        while (ss >> numStr) {
            // Decrypt each encrypted number using modular exponentiation
            long long encryptedNum = stoll(numStr);
            long long decryptedNum = modExp(encryptedNum, privateKey, modulus);
            decryptedMessage += static_cast<char>(decryptedNum);
        }
        return decryptedMessage;
    }

private:
    static long long modExp(long long base, long long exponent, long long modulus) {
        long long result = 1;
        base = base % modulus;
        while (exponent > 0) {
            if (exponent % 2 == 1) {
                result = (result * base) % modulus;
            }
            exponent = exponent >> 1;
            base = (base * base) % modulus;
        }
        return result;
    }
};

class User {
private:
    string username;
    string password;
    long long publicKey; // RSA public key
    long long privateKey; // RSA private key
    long long modulus; // RSA modulus
    string symmetricKey; // Symmetric key for AES encryption
    map<string, vector<string>> messageHistory; // Map to store message history

public:
    User() : username(""), password(""), publicKey(0), privateKey(0), modulus(0), symmetricKey("") {
        // Generate RSA keys
        generateRSAKeys();
        // Generate a random symmetric key for AES encryption
        generateSymmetricKey();
    }

    User(const string& username, const string& password) : username(username), password(password), publicKey(0), privateKey(0), modulus(0), symmetricKey("") {
        // Generate RSA keys
        generateRSAKeys();
        // Generate a random symmetric key for AES encryption
        generateSymmetricKey();
    }

    string getUsername() const {
        return username;
    }

    string getPassword() const {
        return password;
    }

    long long getPublicKey() const {
        return publicKey;
    }

    long long getModulus() const {
        return modulus;
    }

    const string& getSymmetricKey() const {
        return symmetricKey;
    }

    // Method to add message to message history
    void addMessage(const string& sender, const string& message) {
        messageHistory[sender].push_back(message);
    }

private:
    void generateRSAKeys() {
        // Generate random prime numbers p and q
        long long p = generatePrime(1000000000LL, 9999999999LL); // 10-digit primes for 256-bit key
        long long q = generatePrime(1000000000LL, 9999999999LL); // 10-digit primes for 256-bit key

        // Calculate n (modulus) and phi(n)
        modulus = p * q;
        long long phi_n = (p - 1) * (q - 1);

        // Choose public key e such that 1 < e < phi(n) and gcd(e, phi(n)) = 1
        long long e = 65537; // Common choice for e
        // Calculate private key d such that (d * e) % phi(n) = 1
        long long d = modInverse(e, phi_n);

        // Set public key, private key, and modulus
        publicKey = e;
        privateKey = d;
    }

    long long generatePrime(long long min, long long max) {
        random_device rd;
        mt19937 gen(rd());
        uniform_int_distribution<long long> dist(min, max);
        
        for (int attempts = 0; attempts < 1000; ++attempts) { // Limit attempts to prevent infinite loop
            long long num = dist(gen);
            if (isPrime(num)) {
                return num;
            }
        }
        
        throw runtime_error("Failed to generate a prime number within the specified range.");
    }

    bool isPrime(long long n) {
        if (n <= 1) {
            return false;
        }
        if (n <= 3) {
            return true;
        }
        if (n % 2 == 0 || n % 3 == 0) {
            return false;
        }
        for (long long i = 5; i * i <= n; i += 6) {
            if (n % i == 0 || n % (i + 2) == 0) {
                return false;
            }
        }
        return true;
    }

    void generateSymmetricKey() {
        // Generate a random symmetric key using printable ASCII characters
        random_device rd;
        mt19937 gen(rd());
        uniform_int_distribution<int> dis(33, 126); // Printable ASCII characters
        stringstream ss;
        for (int i = 0; i < 32; ++i) { // Using 32-byte key (256 bits)
            ss << static_cast<char>(dis(gen));
        }
        symmetricKey = ss.str();
    }

    long long modInverse(long long a, long long m) {
        long long m0 = m, t, q;
        long long x0 = 0, x1 = 1;

        if (m == 1) return 0;

        // Apply extended Euclid Algorithm
        while (a > 1) {
            // q is quotient
            q = a / m;
            t = m;

            // m is remainder now, process same as euclid's algo
            m = a % m, a = t;

            t = x0;

            x0 = x1 - q * x0;

            x1 = t;
        }

        // Make x1 positive
        if (x1 < 0) x1 += m0;

        return x1;
    }
};

class ChatSystem {
private:
    map<string, User> users;

public:
    string registerUser(const string& username, const string& password) {
        if (users.find(username) != users.end()) {
            return "Username already exists. Please choose another.";
        }
        User newUser(username, password);
        users.insert({ username, newUser });
        return "User registered successfully.";
    }

    bool authenticateUser(const string& username, const string& password) {
        if (users.find(username) != users.end()) {
            return users[username].getPassword() == password;
        }
        return false;
    }

   string sendMessage(const string& sender, const string& recipient, const string& message) {
    cout << "Sender: " << sender << ", Recipient: " << recipient << endl; // Debug output

    if (users.find(sender) != users.end() && users.find(recipient) != users.end()) {
        // Sender and recipient exist, proceed with sending the message
        cout << "Message from " << sender << " to " << recipient << ": " << message << endl;

        // Encrypt the message using AES
        string encryptedMessage = AES::encrypt(message, users[sender].getSymmetricKey());
        cout << "Encrypted message using AES: " << encryptedMessage << endl;

        // Encrypt the AES key using the recipient's RSA public key
        string encryptedAESKey = RSA::encrypt(users[sender].getSymmetricKey(), users[recipient].getPublicKey(), users[recipient].getModulus());
        cout << "Encrypted AES key using RSA: " << encryptedAESKey << endl;

        // Add the message to the recipient's message history
        users[recipient].addMessage(sender, encryptedMessage + "|" + encryptedAESKey);

        return "Message sent successfully.";
    } else {
        cout << "Sender or recipient does not exist." << endl; // Debug output
        return "Sender or recipient does not exist.";
    }
}
};

int main() {
    ChatSystem chatSystem;

    cout << "Welcome to ChatSystem!" << endl;

    string username, password;

    // Registration for user 1
    cout << "Registration for User 1:" << endl;
    cout << "Please enter your desired username: ";
    getline(cin, username);

    cout << "Please enter your desired password: ";
    getline(cin, password);

    cout << chatSystem.registerUser(username, password) << endl;

    // Registration for user 2
    cout << "Registration for User 2:" << endl;
    cout << "Please enter your desired username: ";
    getline(cin, username);

    cout << "Please enter your desired password: ";
    getline(cin, password);

    cout << chatSystem.registerUser(username, password) << endl;

    // Authentication for user 1
    cout << "User 1 Authentication:" << endl;
    cout << "Enter username: ";
    getline(cin, username);

    cout << "Enter password: ";
    getline(cin, password);

    if (!chatSystem.authenticateUser(username, password)) {
        cout << "Authentication failed. Invalid username or password." << endl;
        return 1; // Exit program if authentication fails
    }

    // Authentication for user 2
    cout << "User 2 Authentication:" << endl;
    cout << "Enter username: ";
    getline(cin, username);

    cout << "Enter password: ";
    getline(cin, password);

    if (!chatSystem.authenticateUser(username, password)) {
        cout << "Authentication failed. Invalid username or password." << endl;
        return 1; // Exit program if authentication fails
    }

    // Non-stop messaging loop
    string message;
    while (true) {
        // Send message from user 1 to user 2
        cout << "User 1, enter your message to User 2: ";
        getline(cin, message);
        cout << "Enter recipient's username: ";
        string recipient;
        getline(cin, recipient);
        cout << chatSystem.sendMessage(username, recipient, message) << endl;

        // Send message from user 2 to user 1
        cout << "User 2, enter your message to User 1: ";
        getline(cin, message);
       cout << "Enter recipient's username: ";
        getline(cin, recipient);
        cout << chatSystem.sendMessage(username, recipient, message) << endl;
    }

    return 0;
}


