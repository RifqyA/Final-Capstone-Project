document.addEventListener('DOMContentLoaded', () => {
  if (!firebase || firebase.apps.length === 0) {
    console.error('Firebase is not initialized. Check your initialization script.');
    return;
  } else {
    console.log('Firebase initialized successfully.');
  }

  // Define database reference
  const database = firebase.database();

  // Database Paths
  const dataStringPath = 'Kondisi/Padat';
  const dataIntPath = 'Kondisi/Jumlah';
  const dataWaktuPath = 'Kondisi/Waktu';
  

  // Get database references
  const databaseString = database.ref(dataStringPath);
  const databaseInt = database.ref(dataIntPath);
  const databaseWaktu = database.ref(dataWaktuPath);

  // Fetch data from Firebase
  fetchData(databaseString, databaseInt, databaseWaktu);
});

function fetchData(databaseString, databaseInt, databaseWaktu) {
  // Reading data for "Padat"
  databaseString.on('value', (snapshot) => {
    const StringReading = snapshot.val();
    console.log('Padat:', StringReading);
    document.getElementById("Padat").innerHTML = StringReading;
  }, (errorObject) => {
    console.log('The read failed for Padat: ' + errorObject.name);
  });

  // Reading data for "Jumlah"
  databaseInt.on('value', (snapshot) => {
    const intReading = snapshot.val();
    console.log('Jumlah:', intReading);
    document.getElementById("Jumlah").innerHTML = intReading;
  }, (errorObject) => {
    console.log('The read failed for Jumlah: ' + errorObject.name);
  });

  // Reading data for "Waktu"
  databaseWaktu.on('value', (snapshot) => {
    const WaktuReading = snapshot.val();
    console.log('Waktu:', WaktuReading);
    document.getElementById("Waktu").innerHTML = WaktuReading;
  }, (errorObject) => {
    console.log('The read failed for Waktu: ' + errorObject.name);
  });
}
