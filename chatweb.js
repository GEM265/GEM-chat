let user1, user2;

        function startChat() {
            user1 = document.getElementById("user1").value.trim();
            user2 = document.getElementById("user2").value.trim();

            if (!user1 || !user2) {
                alert("Please enter usernames for both users.");
                return;
            }

            document.getElementById("registration").style.display = "none";
            document.getElementById("chat").style.display = "block";

            // Update placeholders with registered usernames
            document.getElementById("user1MessageInput").placeholder = `${user1}: Type your message`;
            document.getElementById("user2MessageInput").placeholder = `${user2}: Type your message`;

            displayMessage(user1, `Chat started between ${user1} and ${user2}.`);
            displayMessage(user2, `Chat started between ${user1} and ${user2}.`);

            document.getElementById("user1MessageInput").focus();
        }

        function sendMessage(sender, receiver) {
            const messageInput = document.getElementById(`${sender}MessageInput`);
            const message = messageInput.value.trim();
            if (!message) return;

            displayMessage(sender, `<b>${sender}: </b>${message}`);
            displayMessage(receiver, `<b>${sender}: </b>${message}`);
            messageInput.value = "";
            messageInput.focus();
        }

        function displayMessage(user, message) {
            const messagesContainer = document.getElementById(`${user}Messages`);
            const messageElement = document.createElement("div");
            messageElement.innerHTML = message;
            messagesContainer.appendChild(messageElement);
            messagesContainer.scrollTop = messagesContainer.scrollHeight;
        }