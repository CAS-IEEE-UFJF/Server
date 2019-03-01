#include <Arduino.h>
#include <ESP8266WiFi.h>

const char* ssid = "Wifi";
const char* password = "12345678";

WiFiServer server(80);
String header;

void setup()
{
  pinMode(2, OUTPUT);

  Serial.begin(115200);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED)
  {
    delay(200);
    Serial.print(".");
  }

  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  server.begin();
}

String all_html ()
{
  String _html = "";
  //_html += "HTTP/1.1 200 OK";
  //_html += "Content-type:text/html";
  //_html += "Connection: close";
  // Display the HTML web page

  _html += "<!DOCTYPE html><html>";
  _html += "<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">";
  _html += "<link rel=\"icon\" href=\"data:,\">";

  _html += "<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}";
  _html += ".button { background-color: #195B6A; border: none; color: white; padding: 16px 40px;";
  _html += "text-decoration: none; font-size: 30px; margin: 2px; cursor: pointer;}";
  _html += ".button2 {background-color: #77878A;}</style></head>";

  // Web Page Heading
  _html += "<body><h1>ESP8266 Web Server</h1>";

  _html += "<p><a href=\"/button1\"><button class=\"button\">CLICK button 1</button></a></p>";
  _html += "<p><a href=\"/button2\"><button class=\"button\">CLICK button 2</button></a></p>";

  _html += "</body></html>";

  return _html;
}

void loop()
{
  WiFiClient client = server.available();

  if (client)
  {
    Serial.println("New Client.");
    String currentLine = "";

    while (client.connected())
    {
      if (client.available())
      {
        char c = client.read();
        Serial.write(c);
        header += c;

        if (c == '\n')
        {
          if (currentLine.length() == 0)
          {
            client.println(all_html()); //envia o html para o cliente

            break;
          }
          else
          {
            currentLine = "";
          }
        }
        else if (c != '\r')
        {
          currentLine += c;
        }
      }
    }

    if (header.indexOf("GET /button1") >= 0)
    {
      Serial.println("button1");
      digitalWrite(2, HIGH);
      delay(100);
    }

    if (header.indexOf("GET /button2") >= 0)
    {
      Serial.println("button2");
      digitalWrite(2, LOW);
      delay(100);
    }

    header = "";
    client.stop();
  }
}
