class ChatSystem {
    constructor() {
        this.users = new Map(); // Map to store users
        this.messages = []; // Array to store messages
    }

    registerUser(username) {
        if (this.users.has(username)) {
            return `${username} already exists. Please choose another username.`;
        }
        this.users.set(username, []);
        return `${username} registered successfully.`;
    }

    sendMessage(sender, recipient, message) {
        if (!this.users.has(sender) || !this.users.has(recipient)) {
            return "Sender or recipient does not exist.";
        }
        const timestamp = new Date().toLocaleString();
        const newMessage = { sender, recipient, message, timestamp };
        this.messages.push(newMessage);
        this.users.get(recipient).push(newMessage);
        return "Message sent successfully.";
    }

    getMessagesForUser(username) {
        if (!this.users.has(username)) {
            return "User does not exist.";
        }
        return this.users.get(username);
    }
}

// Example usage:
const chatSystem = new ChatSystem();

console.log(chatSystem.registerUser("Alice")); // Alice registered successfully.
console.log(chatSystem.registerUser("Bob")); // Bob registered successfully.

console.log(chatSystem.sendMessage("Alice", "Bob", "Hello Bob!")); // Message sent successfully.
console.log(chatSystem.sendMessage("Bob", "Alice", "Hi Alice!")); // Message sent successfully.

console.log(chatSystem.getMessagesForUser("Alice"));
// Output:
// [ { sender: 'Bob', recipient: 'Alice', message: 'Hi Alice!', timestamp: '5/13/2024, 12:00:00 PM' } ]

console.log(chatSystem.getMessagesForUser("Bob"));
// Output:
// [ { sender: 'Alice', recipient: 'Bob', message: 'Hello Bob!', timestamp: '5/13/2024, 12:00:00 PM' } ]
