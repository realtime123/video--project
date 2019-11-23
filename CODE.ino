#include <ESP8266WiFi.h>

String  ClientRequest;
String ControlRequest;
IPAddress staticIP75_10(192, 168, 1, 203);
IPAddress gateway75_10(192, 168, 1, 1);
IPAddress subnet75_10(255, 255, 255, 0);


WiFiServer server(80);
WiFiClient client;

String currentLine = "";


String myresultat;

String ReadIncomingRequest() {
  String header = "";

  while (client.available()) {

    char c = client.read();             // read a byte, then
    //Serial.write(c);                    // print it out the serial monitor
    header += c;
    if (c == '\n') {                    // if the byte is a newline character
      // if the current line is blank, you got two newline characters in a row.
      // that's the end of the client HTTP request, so send a response:
      if (currentLine.length() == 0) {
        // HTTP headers always start with a response code (e.g. HTTP/1.1 200 OK)
        // and a content-type so the client knows what's coming, then a blank line:
        //        client.println("HTTP/1.1 200 OK");
        //        client.println("Content-type:text/html");
        //        client.println("Connection: close");
        //        client.println();

      }
      else { // if you got a newline, then clear currentLine
        currentLine = "";
      }
    }
    else if (c != '\r') {  // if you got anything else but a carriage return character,
      currentLine += c;      // add it to the end of the currentLine

    }
  }
  return header;
}

void setup()
{
  ClientRequest = "";

  Serial.begin(115200);

  pinMode(2, OUTPUT);
  WiFi.disconnect();
  delay(300);
  //Serial.println("START");
  WiFi.begin("NETWORK1", "1234");
  while ((!(WiFi.status() == WL_CONNECTED))) {
    delay(300);
    //  Serial.print("..");

  }
  // Serial.println("Connected");
  WiFi.config(staticIP75_10, gateway75_10, subnet75_10);
  // Serial.println("Your IP is");
  // Serial.println((WiFi.localIP()));
  server.begin();

}


void loop()
{

  client = server.available();
  if (client) {           // If a new client connects,
    while (client.connected()) {            // loop while the client's connected
      while (!client.available()) {
        delay(1);
      }
      //Serial.println("New Client.");          // print a message out in the serial port
      ClientRequest = (ReadIncomingRequest());
      // Serial.print("ClientRequest before :");  Serial.println(ClientRequest);
      ClientRequest = ClientRequest.substring(5, 8);
      // Serial.print("ClientRequest after :");
      // Serial.println(ClientRequest);
      Serial.println(ClientRequest);
      while (!Serial.available()) {
        delay(1);
        client.flush();
      }

      if (Serial.available() ) {

        ControlRequest = (Serial.readString());
        //Serial.print(ControlRequest);
        delay(1);
      }
      client.flush();
      client.println("HTTP/1.1 200 OK");
      client.println("Content-Type: text/html");
      client.println("");
      client.println("<!DOCTYPE HTML>");
      client.println("<html>");
      client.println(ControlRequest);
      client.println("</html>");
      client.stop();
      delay(1);


      //      }
      //      if (ClientRequest == "OFF") {
      //        digitalWrite(2, LOW);
      //        client.println("HTTP/1.1 200 OK");
      //        client.println("Content-Type: text/html");
      //        client.println("");
      //        client.println("<!DOCTYPE HTML>");
      //        client.println("<html>");
      //        client.println("LED OFF");
      //        Serial.println("LED OFF");
      //        client.println("</html>");
      //        client.stop();
      //        delay(1);
      //
      //      }
      //      else {
      //        client.println("HTTP/1.1 200 OK");
      //        client.println("Content-Type: text/html");
      //        client.println("");
      //        client.println("<!DOCTYPE HTML>");
      //        client.println("<html>");
      //        client.println("ok");
      //        client.stop();
      //      }
      client.stop();
      // Close the connection
      //Serial.println("Client disconnected.");
      //Serial.println("");
    }
  }
}
