import { router } from './routes.js'
import express from 'express';
import path from 'path';
import dotenv from 'dotenv';
const __filename = fileURLToPath(import.meta.url);
const __dirname = path.dirname(__filename);
dotenv.config({ path: path.resolve(__dirname, '.env'), encoding: 'utf8' }); //dotenv.config({ path: path.resolve(__dirname, '.env') }); //{ path: path.join(__dirname, './env') }
import { fileURLToPath } from 'url';
import { createServer } from 'http';
import bodyParser from 'body-parser';
import { Server } from 'socket.io'; // Import the 'socket.io' library
import { device } from './models/device.js';
import { engine } from 'express-handlebars';
import { handlebarsHelpers } from './helpers/handlebars-helpers.js';
import { sensorDataDB } from './models/firebase.js';
import cors from 'cors';


const app = express();
const server = createServer(app);
const io = new Server(server); // Create a socket.io server with a specific path


app.use(cors());

app.use(bodyParser.urlencoded({ extended: false }));
app.use(bodyParser.json());

app.engine('.hbs', engine({ extname: '.hbs', helpers: handlebarsHelpers }));
app.set('view engine', '.hbs');
app.set('views', path.join(__dirname, 'views'));
app.use(express.static(path.join(__dirname, 'public')));
app.use('/', router);


// Handle WebSocket connections

io.on('connection', (socket) => {
 

 // Handle WebSocket events
 console.log('Client connected');
  // Fetch initial data
  device.fetchData().then((data) => {
    // Send initial data when a new client connects
    socket.emit('initial', data);
  });

  // Handle client disconnect
  socket.on('disconnect', () => {
    console.log('Client disconnected');
  });
});
// Subscribe to real-time updates
sensorDataDB.subscribe((data) => {
  // Broadcast updates to all connected clients
  io.emit('update', data);
});

app.get('/dashboard', async (req, res) => {
  try {
    // Fetch initial data
    const data = await device.fetchData();
    
    // Render the dashboard view and pass the initial data
    res.render('dashboard', { title: 'Dashboard', data });
  } catch (error) {
    console.error('Error rendering dashboard:', error);
    res.status(500).send('Internal Server Error');
  }
});

const listener = server.listen(process.env.PORT || 4000, function () {
  console.log(`mould-o-matic started on http://localhost:${listener.address().port}`);
});

