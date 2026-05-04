require('dotenv').config();
const twilio = require('twilio');

const client = twilio(
  process.env.TWILIO_ACCOUNT_SID,
  process.env.TWILIO_AUTH_TOKEN
);

// 📍 Example location (you can change later)
const lat = 12.98;
const lng = 80.30;
const name = "Anuja";

// 📍 Google Maps link
const locationLink = `https://maps.google.com/?q=${lat},${lng}`;

// 🎤 Detailed voice message
const voiceMessage = `
Emergency alert. Patient ${name} appears to be in an unfamiliar location 
and may be disoriented. Immediate attention is required. 
A live location link has been sent to your mobile phone. Please respond quickly.
`;

// 📩 SMS message
const smsMessage = `
🚨 EMERGENCY ALERT 🚨

Patient Name: ${name}

The patient is currently at an unknown or unfamiliar location and may not be aware of their surroundings.

This could indicate disorientation or a potential cognitive issue.

📍 Live Location:
${locationLink}

Please check on the patient immediately.

- CogniCare System
`;

// 📞 CALL FUNCTION
async function makeCall() {
  try {
    const call = await client.calls.create({
      twiml: `<Response><Say voice="alice">${voiceMessage}</Say></Response>`,
      to: process.env.MY_PHONE,
      from: process.env.TWILIO_PHONE
    });

    console.log("📞 Call triggered:", call.sid);
  } catch (err) {
    console.error("❌ Call Error:", err.message);
  }
}

// 📩 SMS FUNCTION
async function sendSMS() {
  try {
    const sms = await client.messages.create({
      body: smsMessage,
      to: process.env.MY_PHONE,
      from: process.env.TWILIO_PHONE
    });

    console.log("📩 SMS sent:", sms.sid);
  } catch (err) {
    console.error("❌ SMS Error:", err.message);
  }
}

// 👉 RUN BOTH
async function run() {
  await makeCall();
  await sendSMS();
}

run();