/*
  Logging to Radmon.org
*/

#define RADMON_HOST "radmon.org"
#define HTTP_PORT   80

static void logToRadmon(int cpm)
{
    if (cpm == 0) return;
    if (WiFi.status() != WL_CONNECTED) return;
  
    Serial.print("connecting to ");
    Serial.println(RADMON_HOST);

    WiFiClient client;

    const int httpPort = 80;
    if (!client.connect(RADMON_HOST, HTTP_PORT)) {
        Serial.println("connection failed");
        return;
    }

    // We now create a URI for the request
    String url = "/radmon.php?function=submit&user=";
    url += RADMON_USER_ID;
    url += "&password=";
    url += RADMON_PRIVATE_KEY;
    url += "&value=";
    url += cpm;
    url += "&unit=CPM";

    Serial.print("Requesting URL: ");
    Serial.println(url);

    // This will send the request to the server
    client.print(String("GET ") + url + " HTTP/1.1\r\n" +
                 "Host: " + RADMON_HOST + "\r\n" +
                 "Connection: close\r\n\r\n");

    unsigned long timeout = millis();
    while (client.available() == 0) {
        if (millis() - timeout > 5000) {
            Serial.println(">>> Client Timeout !");
            client.stop();
            return;
        }
    }

    /*
    // Read all the lines of the reply from server and print them to Serial
    while(client.available()) {
        String line = client.readStringUntil('\r');
        Serial.print(line);
    }
    */
    Serial.println();
    Serial.print("Closing connection to");
    Serial.println(RADMON_HOST);
}
