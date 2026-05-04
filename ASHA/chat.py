from groq import Groq

client = Groq(api_key="YOUR_GROK_KEY")

print("Chat running. Type 'exit' to quit.\n")

messages = []

while True:
    user_input = input("You: ")
    if user_input.lower() == "exit":
        break

    messages.append({"role": "user", "content": user_input})

    completion = client.chat.completions.create(
        model="llama-3.1-8b-instant",
        messages=messages
    )

    reply = completion.choices[0].message.content
    print("AI:", reply)

    messages.append({"role": "assistant", "content": reply})
