const admin = require('firebase-admin');
const serviceAccount = require('./mould-o-matic-ebf187b7f188.json');

// Initialize Firebase admin SDK
admin.initializeApp({
  credential: admin.credential.cert(serviceAccount),
  databaseURL: 'https://mould-o-matic-default-rtdb.europe-west1.firebasedatabase.app/',
});

// Reference to the Firebase Realtime Database
const db = admin.database();

// Reference to the 'sensors' node
const sensorsRef = db.ref('sensors');

// Fetch data once
sensorsRef.once('value', (snapshot) => {
  const data = snapshot.val();
  console.log('Data from Firebase:', data);
})
  .catch((error) => {
    console.error('Error fetching data from Firebase:', error);
  });
