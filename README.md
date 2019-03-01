### Objetivo
Criar um web server onde o cliente e o servidor possam se comunicar. Piscando led quando um botão é precionado e apagando quando o outro é precionado.

![](https://github.com/CAS-IEEE-UFJF/Server/blob/master/img_video/ezgif.com-video-to-gif.gif)


### Começando ...
<i>lembre-se que a comunicação entre esp e cliente sempre é feita usando um roteador</i>

## pt 1

* Incluir bibliotecas, declarar sua senha, porta e variáveis globais

```C++
  #include <Arduino.h>
  #include <ESP8266WiFi.h>

  const char* ssid = "Wifi";
  const char* password = "12345678";

  WiFiServer server(80);
  String header;
```

* No void setup:

``` C++
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
```

* Aqui vamos escrever nosso HTML. O retorno da função é a var _html

``` C++
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
```

* No void loop vamos verificar se tem algum cliente conentado.

``` C++
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
```

* Agora é só carregar o code e acessar o endereço de IP

## pt 2

E se os file html, css e javascript estiverem na memória flash do esp? como fica o web server????

<hr>

### Referência

https://randomnerdtutorials.com/esp8266-web-server-with-arduino-ide/  14h 01/03/2019
