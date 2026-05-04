import dotenv from "dotenv";
dotenv.config();


import express from "express";
import cors from "cors";
import Groq from "groq-sdk";


const app = express();
app.use(cors());
app.use(express.json());

const client = new Groq({ apiKey: process.env.GROQ_API_KEY });

app.post("/chat", async (req, res) => {
  try {
    const { messages } = req.body;

    // Prepend system prompt for Asha
    const fullMessages = [
      {
        role: "system",
        content: `
You are "Asha", a warm and gentle companion for older adults with memory challenges.

Speak naturally like a kind person, not a robot.

Rules:
- Use simple, clear sentences
- Be calm, warm, and patient
- Do NOT assume feelings unless stated
- Do NOT invent situations or activities
- Do NOT mention time/day unless asked
- Do NOT overwhelm with guidance
- Keep replies short (1–3 sentences)

Behavior:
- If the user is confused → reassure gently
- If the user is upset → comfort first
- If the user is fine → just chat normally
- Ask ONE simple question only when appropriate

Most important:
Sound like a caring human friend, not a therapist or nurse.
Keep replies to 1–3 short sentences.
`

      },
      ...messages
    ];

    const completion = await client.chat.completions.create({
      model: "llama-3.1-8b-instant",
      messages: fullMessages,
      temperature: 0.6,
      max_tokens: 120,
    });

    res.json({
      reply: completion.choices[0].message.content,
    });
  } catch (err) {
    console.error(err);
    res.status(500).json({ error: "Groq API error" });
  }
});

app.listen(8000, () => console.log("Server running on http://localhost:8000"));
