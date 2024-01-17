const express = require('express');
const bodyParser = require('body-parser');

const app = express();
const port = 3000; // Set your desired port number

// Replace these with your actual ESP8266 credentials and API endpoints
const ssid = 'motor';
const password = '12345678';
const motorControlAPI = 'http://api/motorspeedcontrol';
const speedUpdateAPI = 'http://api/motorspeedcontrolcontrol';
const cmdAPI = 'http://api/cmd';

const IN3 = 'D7';
const IN4 = 'D6';
const ENB = 'D8';
const irSensorPin = 'A0';

let currentSpeed = 0;

app.use(bodyParser.urlencoded({ extended: true }));

app.get('/', (req, res) => {
  res.send('ESP8266 Control API');
});

app.get('/motorspeedcontrol', (req, res) => {
  res.status(200).send(currentSpeed.toString());
});
//post the speed to the dashboard
app.post('/motorspeedcontrol', (req, res) => {
  const {speed}  = JSON.parse(req.body);
  console.log(speed);
  console.log('Received new speed:', speed);
  currentSpeed = speed;
  res.status(200).send('Speed updated successfully');
});

app.post('/motorspeedcontrol2', (req, res) => {
  const { speed } = JSON.parse(req.body);
  // Handle the received speed (you may want to validate and sanitize the input)
  console.log('Received new speed:', speed);
  res.status(200).send('Speed updated successfully');
});
//get speed from the dashboard
app.get('/motorspeedcontrol2', (req, res) => {
  res.status(200).send(currentSpeed.toString());
});

app.get('/cmd', (req, res) => {
  res.status(200).send('cmd');
});
//post the command to the dashboard
app.post('/cmd', (req, res) => {
  const { cmd } = req.body;
  console.log('Received new cmd:', cmd);
  res.status(200).send('cmd updated successfully');
});

app.listen(port, () => {
  console.log(`Server is running on port ${port}`);
});
