
String buscarAcoesWifi() {
  Serial.println("Buscando ações via WiFi...");

  String response = "";
  WiFiClient client;

  String sn = String(SERIAL_NUMBER);
  String url = String(path_buscar_acoes_eth) + sn + "/" + senha_api;

  if (client.connect(BASE_URL_ETH, BASE_PORT_ETH)) {
    Serial.println(url);

    client.println("GET " + url + " HTTP/1.1");
    client.println("Host: " + String(BASE_URL_ETH));
    client.println("Content-Type: application/json");
    client.println("Connection: keep-alive");
    client.println();

    unsigned long tempoInicio = millis();  // Marca o tempo inicial
    String respostaServidor = "";

    // Aguarda a resposta sem travar o loop
    while (millis() - tempoInicio < TEMPO_ISPEIRA) {  // Timeout de 3 segundos
      while (client.available()) {
        char c = client.read();
        respostaServidor += c;
        tempoInicio = millis();  // Reseta o tempo se receber dados
      }
      if (!client.connected()) break;  // Sai se a conexão for fechada
      yield();                         // Evita o watchdog reset no ESP8266
    }

    int indexI = respostaServidor.indexOf("{");
    int indexF = respostaServidor.lastIndexOf("}");

    if (indexI >= 0 && indexF > indexI) {
      response = respostaServidor.substring(indexI, indexF + 1);
    }

  } else {
    Serial.println("Falha na conexão ao buscar ações via WiFi");
    return "errou";
  }

  client.stop();
  return response;
}

String responderAcao(int id_requisicao) {
  Serial.println("Respondendo requisição de ação via WiFi");

  String Sid_requisicao = String(id_requisicao);
  String sn = String(SERIAL_NUMBER);
  String url = String(path_responder_acao_eth) + Sid_requisicao + "/" + sn + "/" + senha_api;

  WiFiClient client;

  if (client.connect(BASE_URL_ETH, BASE_PORT_ETH)) {
    Serial.println(url);

    client.println("GET " + url + " HTTP/1.1");
    client.println("Host: " + String(BASE_URL_ETH));
    client.println("Content-Type: application/json");
    client.println("Connection: close");
    client.println();

    unsigned long tempoInicio = millis();  // Marca o tempo inicial
    String respostaServidor = "";

    // Aguarda a resposta sem bloquear
    while (millis() - tempoInicio < TEMPO_ISPEIRA) {  // Timeout de 3 segundos
      while (client.available()) {
        char c = client.read();
        respostaServidor += c;
        tempoInicio = millis();  // Reseta o tempo se receber dados
      }
      if (!client.connected()) break;  // Sai se a conexão for fechada
      yield();                         // Evita watchdog reset no ESP8266
    }

    if (respostaServidor.length() > 0) {
      //Serial.println("Resposta do servidor:");
      //Serial.println(respostaServidor);
    } else {
      Serial.println("Nenhuma resposta recebida.");
    }

  } else {
    Serial.println("Falha na conexão ao responder ação via WiFi");
    return "errou";
  }

  client.stop();
  return "sucesso";
}



void buscarAtt() {


#ifdef ESP32
  esp_task_wdt_delete(NULL);
#elif defined(ESP8266)
  ESP.wdtDisable();
#else
#error "Placa não suportada. Escolha ESP32 ou ESP8266."
#endif

  WiFiClient transport;
  HttpClient client(transport, BASE_URL_ETH_ATT, SERVER_PORT_ETH_ATT);


  client.setTimeout(15000);  // 10 segundos

  client.get(path_buscar_att_eth);

  int statusCode = client.responseStatusCode();
  Serial.print("Update status code: ");
  Serial.println(statusCode);
  if (statusCode != 200) {

    client.stop();
    return;
  }

  long length = client.contentLength();
  if (length == HttpClient::kNoContentLengthHeader) {
    client.stop();
    Serial.println("Server didn't provide Content-length header. Can't continue with update.");
    return;
  }
  Serial.print("Server returned update file of size ");
  Serial.print(length);
  Serial.println(" bytes");

  if (!Update.begin(length, U_FLASH)) {
    client.stop();
    Serial.println("There is not enough space to store the update. Can't continue with update.");
    return;
  }
  byte buffer[BUFFER_SIZE];

  while (length > 0) {
    int readSize = min((long)BUFFER_SIZE, length);  // Converte BUFFER_SIZE para long
    int bytesRead = client.readBytes(buffer, readSize);
    if (bytesRead <= 0) {
      Serial.println("Erro ao ler os dados do servidor");
      break;
    }
    Update.write(buffer, bytesRead);
    length -= bytesRead;
  }
  Update.end(false);
  client.stop();
  if (length > 0) {
    Serial.print("Timeout downloading update file at ");
    Serial.print(length);
    Serial.println(" bytes. Can't continue with update.");
    return;
  }

  Serial.println("Sketch update apply and reset.");
  Serial.flush();
  ESP.restart();
}
