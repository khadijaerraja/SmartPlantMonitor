🌿SmartPlantMonitor
Surveillance intelligente de plantes via un microcontrôleur ESP32, une application mobile Expo React Native, et Firebase Realtime Database.

## Exécuter le code Arduino (.ino)
1. Ouvrir le fichier : sketch_jun26a dans l’Arduino IDE

2. Remplir ces informations dans le code :
   
   const char* ssid = "VOTRE_WIFI" ;
   const char* password = "VOTRE_MDP_WIFI" ;
   const char* DATABASE_URL = "votre URL Firebase" ;
   const char* API_KEY = "clé API Firebase" ;
 

3. Téléverser le code.
Créer la base Firebase Realtime Database

## Lancer l'application mobile (Expo)
1. **Installer Node.js** : https://nodejs.org
2. **Installer Expo CLI** : npm install -g expo-cli

3. **Se placer dans le dossier de l'app mobile : **npm install
4. **Démarrer l'application : ** npx expo start

5. Scanner le QR code affiché avec l’application Expo Go (sur Android ou iOS).
 
## Résultat

- Les données lues par les capteurs (température, humidité de l'air et du sol) sont envoyées à Firebase.  
- L'application Expo affiche ces données en temps réel dans des onglets.  
- L'écran LCD I2C montre les valeurs localement.

![Capture d'écran de l'application Expo affichant les données](assets/app.jpg)

