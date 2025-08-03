import React from 'react';

function App() {
  const startStudy = () => {
    fetch('http://localhost:8080/start-study')
      .then(res => res.text())
      .then(data => alert(data))
      .catch(err => alert('Failed to start study: ' + err));
  };

  const endStudy = () => {
    fetch('http://localhost:8080/end-study')
      .then(res => res.text())
      .then(data => alert(data))
      .catch(err => alert('Failed to end study: ' + err));
  };

  return (
    <div style={{ display: 'flex', flexDirection: 'column', alignItems: 'flex-start', padding: '20px' }}>
      <h1>Studytyme</h1>
      <button onClick={startStudy} style={{ marginBottom: '10px' }}>Start Study</button>
      <button onClick={endStudy}>End Study</button>
    </div>
  );
}

export default App;
