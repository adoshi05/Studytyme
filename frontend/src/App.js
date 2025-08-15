import React, { useState, useEffect } from 'react';

function App() {
  const [state, setState] = useState({ mode: 'idle' });

  const fetchState = () => {
    fetch('http://localhost:8080/state')
      .then(res => res.json())
      .then(data => setState(data))
      .catch(err => console.error(err));
  };

  useEffect(() => {
    fetchState();
  }, []);

  const startStudy = () => {
    fetch('http://localhost:8080/start-study')
      .then(res => res.text())
      .then(msg => {
        alert(msg);
        fetchState();
      });
  };

  const endStudy = () => {
    fetch('http://localhost:8080/end-study')
      .then(res => res.json())
      .then(data => {
        alert(`${data.message} Earned break: ${data.earnedBreakMinutes} min`);
        fetchState();
      });
  };

  const startBreak = () => {
    fetch('http://localhost:8080/start-break')
      .then(res => res.text())
      .then(msg => {
        alert(msg);
        fetchState();
      });
  };

  const endBreak = () => {
    fetch('http://localhost:8080/end-break')
      .then(res => res.json())
      .then(data => {
        alert(data.message);
        fetchState();
      });
  };

  return (
    <div style={{ display: 'flex', flexDirection: 'column', alignItems: 'flex-start', padding: '20px' }}>
      <h1>Studytyme</h1>
      {state.mode === 'idle' && <button onClick={startStudy}>Start Study</button>}
      {state.mode === 'study' && <button onClick={endStudy}>End Study</button>}
      {state.mode === 'break' && !state.breakStart && <button onClick={startBreak}>Start Break</button>}
      {state.mode === 'break' && state.breakStart && <button onClick={endBreak}>End Break</button>}
    </div>
  );
}

export default App;
