#include <WiFi.h>
#include <Firebase_ESP_Client.h>

// Ferramentas auxiliares para uso do Firebase
#include "addons/TokenHelper.h"
#include "addons/RTDBHelper.h"

/* Prototipacao de funcoes */
void connectWiFi(void);
void connectFirebase(void);
// Declaração da função
void simularMovimento(float& latitude, float& longitude, float deltaLat, float deltaLong);


// Informações do Wi-Fi
const char *WIFI_SSID = "O nome da sua rede";
const char *WIFI_PASSWD = "Senha da sua rede";

// Chave API do projeto. Buscar no Firebase
#define API_KEY "Chave de acesso do firebase"
// URL do banco. Buscar no Firebase
#define DATABASE_URL "url de acesso do firebase" 

//Define Firebase Data object
FirebaseData fbdo;
// Autenticacao e configuracoes do Firebase
FirebaseAuth auth;
FirebaseConfig config;

unsigned long sendDataPrevMillis = 0;
bool signupOK = false;
// Valores para enviar ao Firebase
//float latitude, longitude;

float latitude, longitude;

// Coordenadas de latitude e longitude
float latitudes[] = {-21.2280, -21.2339, -21.2310, -21.2310, -21.2320};
float longitudes[] = {-44.9741, -44.9743, -44.9923, -44.9910, -44.9920};


float deltaLat = 0.0001;  // Alteração na latitude
float deltaLong = 0.0002; // Alteração na longitude
// Contem o endereço MAC do ESP32
char macStr[18];

void setup(){
  Serial.begin(115200);
  //inicia WiFi
  connectWiFi();

  //inicia Firebase
  connectFirebase();
  
  
  
}

void loop(){
  //Se WiFi nao conectada, conecta
  if(WiFi.status() != WL_CONNECTED)
    connectWiFi();
  
  // Se Firebase nao conectado, conecta
  if(!signupOK)
    connectFirebase();
  
  // Escolhe valores de latitude e longitude aleatoriamente
  int index = random(0, 4);
  latitude = latitudes[index];
  longitude = longitudes[index];

  /* Se conexao com Firebase ok, se o tempo minimo de submissao ja passou, entao
    envia novos dados.
  */
  if (Firebase.ready() && (millis() - sendDataPrevMillis > 10000 || sendDataPrevMillis == 0)){
    sendDataPrevMillis = millis();
    
    char indiceLatitude[24];
    strcpy(indiceLatitude, macStr);
    strcat(indiceLatitude, "/latitude");
    // Enviando valor de vazao
    if (Firebase.RTDB.setFloat(&fbdo, indiceLatitude, latitude)){
      Serial.print("Enviado. ");
      Serial.print("Path = ");
      Serial.println(fbdo.dataPath());
      Serial.print("; Valor: ");
      Serial.println(latitude);
    } else {
      Serial.print("Falha: ");
      Serial.println(fbdo.errorReason());
    }
    
    //Path no banco
    char indiceLongitude[24];
    strcpy(indiceLongitude, macStr);
    strcat(indiceLongitude, "/longitude");
    // Enviando valor de nivel
    if (Firebase.RTDB.setFloat(&fbdo, indiceLongitude, longitude)){
      Serial.print("Enviado. ");
      Serial.print("Path = ");
      Serial.println(fbdo.dataPath());
      Serial.print("; Valor: ");
      Serial.println(longitude);
    } else {
      Serial.print("Falha: ");
      Serial.println(fbdo.errorReason());
    }
  }
}

// inicializacao do WiFi
void connectWiFi() { 
  //define modo station para o ESP
  WiFi.mode(WIFI_STA);
  // tenta conexao com o AP
  WiFi.begin(WIFI_SSID, WIFI_PASSWD);
  //Obtem endereco MAC
  uint8_t mac[6];
  WiFi.macAddress(mac);
  sprintf(macStr, "%02X:%02X:%02X:%02X:%02X:%02X", mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
  
  Serial.print("Connecting to WiFi .."); 
  //permanece no laco enquanto WiFi nao conectar
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print('.');
    delay(1000);
  }
  // Imprime IP recebido do AP
  Serial.println(WiFi.localIP());
}


void connectFirebase() { 
  /* Controle de conexao ao Firebase - Real Time Data Base
   Define API e URL,
   Tenta conexao,
   Define função de callback */
  config.api_key = API_KEY;

  /* Assign the RTDB URL (required) */
  config.database_url = DATABASE_URL;

  // Conexao ao firebase como usuario anonimo
  if (Firebase.signUp(&config, &auth, "", "")){
    Serial.println("Firebase ok");
    signupOK = true;
  } else{
    Serial.printf("%s\n", config.signer.signupError.message.c_str());
  }

  // definicao de funcao de callback para evento de Token
  config.token_status_callback = tokenStatusCallback; //especificada em addons/TokenHelper.h
  
  Firebase.begin(&config, &auth);
  Firebase.reconnectWiFi(true);
}

void simularMovimento(float& latitude, float& longitude, float deltaLat, float deltaLong) {
    latitude += deltaLat;
    longitude += deltaLong;
}