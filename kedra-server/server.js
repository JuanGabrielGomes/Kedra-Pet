const express = require('express');
const WebSocket = require('ws');
const bodyParser = require('body-parser');
const admin = require('firebase-admin');

// Inicializa Firebase Admin
const serviceAccount = require('./path/kedra-pet-firebase-adminsdk-48gl3-a092054b1d.json');
admin.initializeApp({
    credential: admin.credential.cert(serviceAccount),
    databaseURL: 'https://kedra-pet-default-rtdb.firebaseio.com'
});

const db = admin.database();
const app = express();
const port = 3000;

app.use(bodyParser.json());
app.use(express.static('public'));

app.get('/', (req, res) => {
    res.sendFile(__dirname + '/public/landingPage.html');
});

// Rotas HTTP
app.post('/api/cadastrar', async (req, res) => {
    const { nome, email, senha } = req.body;
    try {
        const ref = db.ref('usuarios').push();
        await ref.set({ nome, email, senha });
        res.status(200).send({ sucesso: true, mensagem: 'Usuário cadastrado com sucesso!' });
    } catch {
        res.status(500).send({ sucesso: false, mensagem: 'Erro ao cadastrar usuário.' });
    }
});

app.post('/api/login', async (req, res) => {
    const { email, senha } = req.body;
    try {
        const snapshot = await db.ref('usuarios').orderByChild('email').equalTo(email).once('value');
        if (snapshot.exists()) {
            const usuario = Object.values(snapshot.val())[0];
            if (usuario.senha === senha) {
                res.status(200).send({
                    sucesso: true,
                    userId: Object.keys(snapshot.val())[0],
                    mensagem: 'Login realizado com sucesso!'
                });
            } else {
                res.status(401).send({ sucesso: false, mensagem: 'Senha incorreta.' });
            }
        } else {
            res.status(404).send({ sucesso: false, mensagem: 'Usuário não encontrado.' });
        }
    } catch (error) {
        console.error('Erro ao autenticar usuário:', error);
        res.status(500).send({ sucesso: false, mensagem: 'Erro ao autenticar usuário.' });
    }
});

app.post('/api/cadastrarRefeicao', async (req, res) => {
    const { userId, nome, quantidade, intervalo } = req.body;
    try {
        const ref = db.ref(`refeicoes/${userId}`).push();
        await ref.set({ nome, quantidade, intervalo });
        res.status(200).send({ sucesso: true, mensagem: 'Refeição cadastrada com sucesso!' });
    } catch {
        res.status(500).send({ sucesso: false, mensagem: 'Erro ao cadastrar refeição.' });
    }
});

app.get('/api/listarRefeicoes/:userId', async (req, res) => {
    const userId = req.params.userId;
    try {
        const snapshot = await db.ref(`refeicoes/${userId}`).once('value');
        if (snapshot.exists()) {
            res.status(200).send({ sucesso: true, dados: snapshot.val() });
        } else {
            res.status(404).send({ sucesso: false, mensagem: 'Nenhuma refeição encontrada.' });
        }
    } catch {
        res.status(500).send({ sucesso: false, mensagem: 'Erro ao listar refeições.' });
    }
});

// Configuração WebSocket
const wss = new WebSocket.Server({ port: 8080 });
let esp32Connection = null;

wss.on('connection', (ws) => {
    ws.on('message', (message) => {
        const comando = JSON.parse(message);
        if (comando.acao === 'esp32-client') {
            esp32Connection = ws;
        } else if (esp32Connection) {
            esp32Connection.send(message);
        }
    });
});

app.listen(port, () => console.log(`Servidor rodando em http://localhost:${port}`));
