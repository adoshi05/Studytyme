import React, { useState, useEffect, useRef } from 'react';

function App() {
  const [state, setState] = useState({ mode: 'idle' }); // Initial state
  const [timerDisplay, setTimerDisplay] = useState("00:00"); // Timer set
  const [overBreak, setOverBreak] = useState(null); // Track break 'overtime'
  const [notes, setNotes] = useState("");

  const studyStartRef = useRef(null);
  const breakStartRef = useRef(null);

  // Load notes initially
  useEffect(() => {
    fetch('http://localhost:8080/load-notes')
      .then(res => res.json())
      .then(data => {
        setNotes(data.notes || "");
      })
      .catch(err => console.error("Error loading notes:", err));
  }, []);

  // Time formatting
  const formatTime = (sec) => {
    const m = Math.floor(sec / 60).toString().padStart(2, '0');
    const s = (sec % 60).toString().padStart(2, '0');
    return `${m}:${s}`;
  };

  /*const fetchState = () => {
    fetch('http://localhost:8080/state')
      .then(res => res.json())
      .then(data => setState(data))
      .catch(err => console.error(err));
  };*/

  useEffect(() => {
    const interval = setInterval(() => {
      if (state.mode === 'study' && studyStartRef.current) { // CHANGED
        const elapsed = Math.floor((Date.now() - studyStartRef.current) / 1000);
        setTimerDisplay(formatTime(elapsed));
      } else if (state.mode === 'break' && breakStartRef.current) { // CHANGED
        const elapsed = Math.floor((Date.now() - breakStartRef.current) / 1000);
        setTimerDisplay(formatTime(elapsed));
       // Warn if going over earned break
        if (elapsed > state.earnedBreak) {
          setOverBreak(elapsed - state.earnedBreak);
        } else {
          setOverBreak(null);
        }
      } else {
        setTimerDisplay("00:00");
        setOverBreak(null);
      }
    }, 1000);

    return () => clearInterval(interval);
  }, [state.mode]);


  const fetchState = () => {
    fetch('http://localhost:8080/state')
      .then(res => res.json())
      .then(data => {
        setState(data);
        if (data.studyStart) studyStartRef.current = data.studyStart;
        if (data.breakStart) breakStartRef.current = data.breakStart;
      })
      .catch(err => console.error(err));
  };

  const startStudy = () => {
    fetch('http://localhost:8080/start-study')
      .then(res => res.text())
      .then(msg => {
        if (msg.includes("started")) {  // only update if success
          studyStartRef.current = Date.now();
          setState({ mode: 'study', earnedBreak: 0 });
        }
        alert(msg);
      });
  };

  const endStudy = () => {
    fetch('http://localhost:8080/end-study')
      .then(res => res.json())
      .then(data => {
         //if (data.success) {
          studyStartRef.current = null;
          // Only use seconds to avoid undef issue
          setState({ mode: 'break', earnedBreak: data.earnedBreakSeconds });
          alert(`${data.message} Earned break: ${Math.floor(data.earnedBreakSeconds/60)} min`);
        /*} else {
          alert(data.message);
        }*/
      });
  };

  const startBreak = () => {
    fetch('http://localhost:8080/start-break')
      .then(res => res.text())
      .then(msg => {
        alert(msg);
        breakStartRef.current = Date.now();
        setState(prev => ({ ...prev, breakStart: true }));
      });
  };

  const endBreak = () => {
    fetch('http://localhost:8080/end-break')
      .then(res => res.json())
      .then(data => {
        alert(data.message);
        breakStartRef.current = null;
        setState({ ...state, mode: 'idle' });
      });
  };

  const saveNotes = () => {
    fetch('http://localhost:8080/save-notes', {
      method: 'POST',
      headers: { 'Content-Type': 'application/json' },
      body: JSON.stringify({ notes })
    })
      .then(res => res.json())
      .then(data => {
        alert(data.message);
      });
  };

  return (
     <div style={{ display: 'flex', padding: '20px' }}>
      {/* Timer area */}
      <div style={{ marginRight: '20px' }}>
        <h2>Timer</h2>
        <div style={{ fontSize: '2em', fontWeight: 'bold' }}>{timerDisplay}</div>

        {state.mode === 'break' && (
          <div style={{ marginTop: '5px', color: 'red' }}>
            Earned break: {Math.floor(state.earnedBreak / 60)} min
            {overBreak && (
              <div style={{ marginTop: '5px', color: 'darkred', fontWeight: 'bold' }}>
                Over break by {formatTime(overBreak)}
              </div>
            )}
          </div>
        )}
      </div>

      {/* Notepad temp */}
      <div style={{ flex: 1 }}>
        <h1>Studytyme</h1>
        {state.mode === 'idle' && <button onClick={startStudy}>Start Study</button>}
        {state.mode === 'study' && <button onClick={endStudy}>End Study</button>}
        {state.mode === 'break' && !state.breakStart && <button onClick={startBreak}>Start Break</button>}
        {state.mode === 'break' && state.breakStart && <button onClick={endBreak}>End Break</button>}

        <h2 style={{ marginTop: '20px' }}>Notes</h2>
        <textarea
          value={notes}
          onChange={(e) => setNotes(e.target.value)}
          rows="15"
          cols="50"
          style={{ width: '80%', fontSize: '1em', padding: '10px' }}
        />
        <div>
          <button onClick={saveNotes} style={{ marginTop: '10px' }}>Save Notes</button>
        </div>
      </div>
    </div>
  );
}

export default App;
