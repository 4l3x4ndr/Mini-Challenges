#include <ESP8266WiFi.h>

const char* ssid="LUSAFMA";
const char* pass="Altma4020";

//const char* ssid = "LUSAFMA"; //Nombre de la red WiFi a la que se va a conectar
//const char* pass = "Altma4020"; //Contraseña de la red WiFi a la que se va a conectar

#define RED D2 //El Rojo se conecta al pin D2, que corresponde al GPIO4. Se marca como 4.
#define GREEN D6 //El Verde se conecta al pin D3, que corresponde al GPIO0. Se marca como 0.
#define YELLOW D7 //El Azul se conecta al pin D4, que corresponde al GPIO2. Se marca como 2.
WiFiServer server(80); //Crea el objeto server, con el puerto 80.

void setup() {
  pinMode(RED, OUTPUT);
  pinMode(GREEN, OUTPUT);
  pinMode(YELLOW, OUTPUT);
  
  digitalWrite(RED, HIGH);
  digitalWrite(GREEN, HIGH);
  digitalWrite(YELLOW, HIGH);

  Serial.begin(115200);
  delay(10);

  /*Conexión a la red WiFi*/
  Serial.println();
  Serial.print("Conectando a la red ");
  Serial.println(ssid);

  WiFi.begin(ssid, pass); //Inicia la conexión
  while (WiFi.status() != WL_CONNECTED) { //Sigue intentando conectarse
    delay(500);
    Serial.println(".");
  }
  Serial.println();
  Serial.println("WiFi conectado");

  /*Inicia el servidor*/
  server.begin();
  Serial.println("Inicia el servidor:");
  Serial.print("Para conectarse, utilizar la siguiente URL: ");
  Serial.print("http://");
  Serial.print(WiFi.localIP());
  Serial.println("/");
}

void loop() {
  WiFiClient client = server.available(); //Verifica si hay algún cliente conectado
  if (!client) { //Mientras no haya cliente conectado, no continúa.
    return;
  }
  //Una vez encuentra un cliente conectado...
  Serial.println("Nuevo cliente");
  while (!client.available()) {//Espera hasta recibir algún dato del cliente
    delay(1);
  }
  //Cuando recibe un dato. 1) Lee la primera línea del request del cliente
  String request = client.readStringUntil('\r');
  Serial.println(request);
  client.flush();
  //Asocia el request a la acción correspondiente:
  int valueR = LOW;
  int valueG = LOW;
  int valueY = LOW;
  
  if (request.indexOf("\LEDR=OFF") != -1) {
    digitalWrite(RED, HIGH);
    valueR = HIGH;
  }
  if (request.indexOf("\LEDR=ON") != -1) {
    digitalWrite(RED, LOW);
    valueR = LOW;
  }
    if (request.indexOf("\LEDG=OFF") != -1) {
    digitalWrite(GREEN, HIGH);
    valueG = HIGH;
  }
  if (request.indexOf("\LEDG=ON") != -1) {
    digitalWrite(GREEN, LOW);
    valueG = LOW;
  }
    if (request.indexOf("\LEDY=OFF") != -1) {
    digitalWrite(YELLOW, HIGH);
    valueY = HIGH;
  }
  if (request.indexOf("\LEDY=ON") != -1) {
    digitalWrite(YELLOW, LOW);
    valueY = LOW;
  }
  /*Retorna una respupesta: */
  client.println("http/1.1 200 OK");
  client.println("Content-Type: text/html");
  client.println("");
  client.println("<!DOCTYPE HTML>");
  client.println("<html>");

  client.println("<br><br>");
  client.println("<a href=\"/LEDR=ON\"><button> RED ON </button></a>");
  client.println("<a href=\"/LEDG=ON\"><button> GREEN ON </button></a>");
  client.println("<a href=\"/LEDY=ON\"><button> YELLOW ON </button></a> <br />");
  client.println("<a href=\"/LEDR=OFF\"><button> RED OFF </button></a>");
  client.println("<a href=\"/LEDG=OFF\"><button> GREEN OFF </button></a>");
  client.println("<a href=\"/LEDY=OFF\"><button> YELLOW OFF</button></a>");
  client.println("</html>");
  delay(1);
  Serial.println("Cliente desconectado");
  Serial.println("");
}
