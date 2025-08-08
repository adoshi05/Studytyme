// server.js
const express = require('express');
const cors = require('cors');

const app = express();
const PORT = 8080;

// Enable CORS so React frontend can talk to backend
app.use(cors());

// Parse JSON if needed in the future
app.use(express.json());

// Example start study route
app.get('/start-study', (req, res) => {
    console.log("Study session started.");
    res.json({ message: "Study session started." });
});

// Example stop study route
app.get('/end-study', (req, res) => {
    console.log("Study session stopped.");
    res.json({ message: "Study session stopped." });
});

// Example start break route
app.get('/start-break', (req, res) => {
    console.log("Break started.");
    res.json({ message: "Break session started." });
});

// Example stop break route
app.get('/stop-break', (req, res) => {
    console.log("Break stopped.");
    res.json({ message: "Break session stopped." });
});

// Start server
app.listen(PORT, () => {
    console.log(`Server running at http://localhost:${PORT}`);
});
