const express = require('express');
const { exec } = require('child_process');

const app = express();
const PORT = 8080;

console.log("Booting server...")

app.get('/start-study', (req, res) => {
  exec('./studytyme start-study', (err, stdout, stderr) => {
    if (err) return res.status(500).send(stderr);
    res.send(stdout);
  });
});

app.get('/end-break', (req, res) => {
  exec('./studytyme end-break', (err, stdout, stderr) => {
    if (err) return res.status(500).send(stderr);
    res.send(stdout);
  });
});

app.listen(PORT, () => {
  console.log(`Bridge server running on http://localhost:${PORT}`);
});
