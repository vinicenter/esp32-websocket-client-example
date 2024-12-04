const WebSocket = require('ws');
const readline = require('readline');

const wss = new WebSocket.Server({ port: 8080, host: '0.0.0.0' });

console.log('Servidor WebSocket iniciado em ws://localhost:8080');

const rl = readline.createInterface({
  input: process.stdin,
  output: process.stdout
});

const clients = [];

wss.on('connection', (ws) => {
  console.log('Novo cliente conectado');

  clients.push(ws);

  ws.send('Bem-vindo ao servidor WebSocket!');

  ws.on('message', (message) => {
    console.log(`Recebido: ${message}`);
  });

  ws.on('close', () => {
    console.log('Cliente desconectado');
    clients = clients.filter(client => client !== ws);
  });

  ws.on('error', (error) => {
    console.error('Erro WebSocket:', error);
  });
});

rl.on('line', (input) => {
  clients.forEach(client => {
    if (client.readyState === WebSocket.OPEN) {
      client.send(input);
    }
  });
});
