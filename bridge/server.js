// server.js
const express = require('express');
const cors = require('cors');

const app = express();
const PORT = 8080;

// Enable CORS so React frontend can talk to backend
app.use(cors());

// Parse JSON if needed in the future
app.use(express.json());

// Current state/mode
let state = {
    mode: 'idle', // also 'study' and 'break'
    studyStart: null,
    breakStart: null,
    totalStudy: 0,
    totalBreak: 0,
    earnedBreak: 0 // seconds
};

// Start study route
app.get('/start-study', (req, res) => {
    if (state.mode === 'study') {
        return res.status(400).send('Already studying!');
    }
    state.studyStart = Date.now();
    state.mode = 'study';
    res.send('Study session started.');
});

// End study route
app.get('/end-study', (req, res) => {
    if (state.mode !== 'study') {
        return res.status(400).send('Not currently studying.');
    }
    const elapsedSec = Math.floor((Date.now() - state.studyStart) / 1000);
    state.totalStudy += elapsedSec;
    state.earnedBreak = Math.floor(elapsedSec / 3); // 1 second break per 3 second study
    state.studyStart = null;
    state.mode = 'break';
    res.json({
        success: true,
        message: `Study session ended. Studied ${Math.floor(elapsedSec / 60)} min.`,
        earnedBreakSeconds: state.earnedBreak,
        earnedBreakMinutes: Math.floor(state.earnedBreak / 60),
    });
});

// Start break route
app.get('/start-break', (req, res) => {
    if (state.mode !== 'break') {
        return res.status(400).send('Not currently on break.');
    }
    state.breakStart = Date.now();
    res.send(`Break started. You have ${Math.floor(state.earnedBreak / 60)} min.`);
    //state.mode = 'break';
});

// Stop break route
app.get('/end-break', (req, res) => {
    if (state.mode !== 'break' || !state.breakStart) {
        return res.status(400).send('Break has not started.');
    }
    const elapsedSec = Math.floor((Date.now() - state.breakStart) / 1000);
    state.totalBreak += elapsedSec;
    let overTime = elapsedSec - state.earnedBreak;
    state.breakStart = null;
    state.earnedBreak = 0;
    state.mode = 'idle';

    if (overTime > 0) {
        res.json({
            message: `Break ended. Went over by ${Math.floor(overTime / 60)} min.`
        });
    } else {
        res.json({
            message: `Break ended within earned time.`
        });
    }
});

// Get state
app.get('/state', (req, res) => {
    res.json(state);
});

// Start server
app.listen(PORT, () => {
    console.log(`Server running at http://localhost:${PORT}`);
});
