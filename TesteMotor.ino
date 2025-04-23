#include <Wire.h>
#include <Adafruit_MCP23X17.h>
#include <WiFi.h>
#include <WebServer.h>
#include <EEPROM.h>  // EEPROM para ESP8266
#include "SPIFFS.h"
#include <Arduino.h>
#include <Update.h>
#include <ArduinoJson.h>  // JSON
#include <ArduinoHttpClient.h>
#include <WiFiClient.h>  // WiFiClient (TCP Client)
#include <esp_system.h>
#include <esp_task_wdt.h>
#include "esp_camera.h"
#include <WiFi.h>
#include <ESPmDNS.h>

boolean logado = false;



/***************** METODOS *************************/
#include "AllMetods.h"
/***************** METODOS *************************/



#define POS_VEL_ARRANQUE 110
#define POS_VEL_TRAJETO 111
#define POS_FATOR_CORRECAO 112
#define POS_DELAY_BIF 112



#define PATH_GUID "/path_guid.txt.txt"

WiFiClient streamClient;
bool streaming = false;

boolean rodar = false;

boolean arrancar = true;
int velocidade_arranque = -1;
int velocidade_trajeto = -1;
int fator_correcao = -1;
int delay_bifurcacao = -1;


String path_buscar_acoes = "v1/protected/modulopgm/tarefas/listar/";
String path_responder_acao = "v1/protected/modulopgm/tarefas/responder/";
String path_informar_acao = "v1/protected/modulopgm/tarefas/informar/";
char path_buscar_att_eth[] = "/v1/protected/modulopgm/buscarattpgmrosinetosesp32";
const char *BASE_URL_ETH_ATT = "162.240.226.254";
const unsigned short SERVER_PORT_ETH_ATT = 10050;

const char BASE_URL_ETH[] = "162.240.226.254";
const int BASE_PORT_ETH = 10050;

char path_buscar_acoes_eth[] = "/v1/protected/modulopgm/tarefas/listar/";
char path_responder_acao_eth[] = "/v1/protected/modulopgm/tarefas/responder/";
char path_informar_acao_eth[] = "/v1/protected/modulopgm/tarefas/informar/";

#define CAMERA_MODEL_ESP32S3_EYE
#include "camera_pins.h"

#include "MakeParamsLocalSetup.h"
boolean conectado_wifi = false;
int configurado_wifi_global = -1;
boolean direcao = true;

#define PATH_IP_WIFI "/path_ip_wifi.txt"
#define PATH_GAT_WIFI "/path_gat_wifi.txt"
#define PATH_SUB_WIFI "/path_sub_wifi.txt"
#define PATH_CONFIG_WIFI "/path_config_wifi.txt"
#define PATH_GUID "/path_guid.txt.txt"
boolean cliente_wifi_salvo = false;
String ssid_wifi_global = "";
String passwd_wifi_global = "";

#define DELAY_SERVIDOR 500
String jsonResponse;
bool scanComplete = false;
WebServer server(80);

int arrayWifiIP[4] = { 255, 168, 100, 12 };
int arrayWifiGatewayIP[4] = { 255, 168, 100, 1 };
int arrayWifiSubnetIP[4] = { 255, 255, 255, 0 };
int contador_falhas_via_wifi = 0;
int contador_reconect_wifi = 0;

String senha_api = "1234";
#define BUFFER_SIZE 512  // Tamanho do buffer para leitura

#define TEMPO_ISPEIRA 2000
String BASE_URL = "http://162.240.226.254:10050/";



/***************** FilesWriter *************************/
#include "FilesWriter.h"
/***************** FilesWriter *************************/



/*************** CLASSES **************************************************/
#include "WEBPAGES.h"
/*************** CLASSES **************************************************/


/*************** SETUPS *************************/
#include "WifiLocalSetup.h"
#include "WebServerLocalSetup.h"
/*************** SETUPS *************************/

/***************** Atualizar *************************/
#include "Atualizar.h"
/***************** Atualizar *************************/






Adafruit_MCP23X17 mcp;

const uint8_t ENA = 21;
const uint8_t IN1 = 3;
const uint8_t IN2 = 4;

const uint8_t ENB = 14;
const uint8_t IN3 = 1;
const uint8_t IN4 = 2;

const uint8_t sensorMeio = 19;
const uint8_t sensorEsq = 20;
const uint8_t sensorDir = 41;



#define SDA_PIN 47
#define SCL_PIN 48



const char *ssid = "CLARO_2GA38057";
const char *password = "B7A38057";





void parar() {
  // Desliga o sinal PWM (coloca duty cycle em 0)
  ledcWrite(0, 0);  // Canal PWM de ENA
  ledcWrite(1, 0);  // Canal PWM de ENB

  // Garante que todos os pinos de controle da ponte H estejam em nível baixo
  mcp.digitalWrite(IN1, LOW);
  mcp.digitalWrite(IN2, LOW);
  mcp.digitalWrite(IN3, LOW);
  mcp.digitalWrite(IN4, LOW);

  arrancar = true;
}

void tras() {
  mcp.digitalWrite(IN1, HIGH);
  mcp.digitalWrite(IN2, LOW);
  mcp.digitalWrite(ENA, HIGH);

  mcp.digitalWrite(IN3, HIGH);
  mcp.digitalWrite(IN4, LOW);
  mcp.digitalWrite(ENB, HIGH);
}

void frente() {
  int pwm = map(velocidade_trajeto, 0, 100, 0, 1023);


  mcp.digitalWrite(IN1, LOW);
  mcp.digitalWrite(IN2, HIGH);
  ledcWrite(0, pwm);  // PWM no ENA

  mcp.digitalWrite(IN3, LOW);
  mcp.digitalWrite(IN4, HIGH);
  ledcWrite(1, pwm);  // PWM no ENB
}
void esquerda() {

  //velocidade da trajetoria
  int pwm = map(velocidade_trajeto, 0, 100, 0, 1023);
  mcp.digitalWrite(IN1, LOW);
  mcp.digitalWrite(IN2, HIGH);
  ledcWrite(0, pwm);


  //velocidade reduzida
  int velocidade = fator_correcao;
  int pwm_corrigido = map(velocidade, 0, 100, 0, 1023);
  mcp.digitalWrite(IN3, HIGH);
  mcp.digitalWrite(IN4, LOW);
  ledcWrite(1, pwm_corrigido);  // Canal PWM de ENB
}




void direita() {
  //velocidade trajetoria
  int pwm = map(velocidade_trajeto, 0, 100, 0, 1023);
  mcp.digitalWrite(IN3, LOW);
  mcp.digitalWrite(IN4, HIGH);
  ledcWrite(1, pwm);

  //velocidade reduzida
  int velocidade = fator_correcao;
  int pwm_corrigido = map(velocidade, 0, 100, 0, 1023);
  mcp.digitalWrite(IN1, HIGH);
  mcp.digitalWrite(IN2, LOW);
  ledcWrite(0, pwm_corrigido);
}


void trasEsquerda() {
  int pwm = map(velocidade_trajeto, 0, 100, 0, 1023);

  // Motor A (esquerdo) vai pra trás mais rápido
  mcp.digitalWrite(IN1, HIGH);
  mcp.digitalWrite(IN2, LOW);
  ledcWrite(0, pwm);

  // Motor B (direito) mais devagar
  int pwm_corrigido = map(fator_correcao, 0, 100, 0, 1023);
  mcp.digitalWrite(IN3, HIGH);
  mcp.digitalWrite(IN4, LOW);
  ledcWrite(1, pwm_corrigido);
}

void trasDireita() {
  int pwm = map(velocidade_trajeto, 0, 100, 0, 1023);

  // Motor B (direito) vai pra trás mais rápido
  mcp.digitalWrite(IN3, HIGH);
  mcp.digitalWrite(IN4, LOW);
  ledcWrite(1, pwm);

  // Motor A (esquerdo) mais devagar
  int pwm_corrigido = map(fator_correcao, 0, 100, 0, 1023);
  mcp.digitalWrite(IN1, HIGH);
  mcp.digitalWrite(IN2, LOW);
  ledcWrite(0, pwm_corrigido);
}



void arranque() {
  int velocidade_max = 1023;
  int passo = 20;                        // Incremento por passo de PWM (ajuste conforme quiser)
  int delayPasso = velocidade_arranque;  // Tempo entre cada incremento (ms) - controla a suavidade da aceleração

  // Começa do zero até a velocidade máxima
  for (int pwm = 0; pwm <= velocidade_max; pwm += passo) {
    // Motor A
    mcp.digitalWrite(IN1, LOW);
    mcp.digitalWrite(IN2, HIGH);
    ledcWrite(0, pwm);  // PWM no ENA

    // Motor B
    mcp.digitalWrite(IN3, LOW);
    mcp.digitalWrite(IN4, HIGH);
    ledcWrite(1, pwm);  // PWM no ENB

    delay(delayPasso);
  }

  // Garante que atinja exatamente o valor final
  ledcWrite(0, velocidade_max);
  ledcWrite(1, velocidade_max);
}





void streamTask(void *parameter) {
  WiFiClient client = streamClient;

  String response = "HTTP/1.1 200 OK\r\n";
  response += "Content-Type: multipart/x-mixed-replace; boundary=frame\r\n\r\n";
  client.print(response);

  while (client.connected()) {
    camera_fb_t *fb = esp_camera_fb_get();
    if (!fb) {
      Serial.println("Erro ao capturar frame");
      vTaskDelay(100 / portTICK_PERIOD_MS);  // Pequeno delay pra aliviar
      Serial.println("Stream finalizado");

      streaming = false;
      vTaskDelete(NULL);  // encerra a task
      break;
    }

    client.print("--frame\r\nContent-Type: image/jpeg\r\nContent-Length: ");
    client.print(fb->len);
    client.print("\r\n\r\n");
    client.write(fb->buf, fb->len);
    client.print("\r\n");

    esp_camera_fb_return(fb);

    vTaskDelay(30 / portTICK_PERIOD_MS);  // taxa de atualização
  }

  Serial.println("Stream finalizado");
  streaming = false;
  vTaskDelete(NULL);  // encerra a task
}

void handleRoot() {
  String html = "<!DOCTYPE html><html><head><title>ESP32-S3 CAM</title>"
                "<meta name='viewport' content='width=device-width, initial-scale=1.0, maximum-scale=1.0, user-scalable=no'>"
                "<style>"
                "* {box-sizing: border-box; margin: 0; padding: 0;}"
                "html, body {"
                "  height: 100vh;"
                "  width: 100vw;"
                "  background: #000;"
                "  padding: 0;"
                "  overflow: hidden;"
                "}"
                "body {"
                "  display: flex;"
                "  align-items: center;"
                "  justify-content: center;"
                "  overflow: hidden;"
                "}"
                "img {"
                "  width: 100vw;"
                "  height: 100vh;"
                "  object-fit: fill;"
                "  border-radius: 6px;"
                "  cursor: pointer;"
                "}"
                "@media (orientation: portrait) {"  // se estiver na vertical
                "  img {"
                "    object-fit: contain;"
                "  }"
                "}"
                "@media (min-width: 768px) {"
                "  body { padding: 10px; }"
                "}"
                "@media (min-width: 1200px) {"
                "  body { padding: 20px; }"
                "}"
                "</style></head><body>"

                "<img src='/stream' id='video' onclick='toggleFullscreen()'>"

                "<script>"
                "function esconderBarra() { window.scrollTo(0, 1); }"
                "window.addEventListener('load', () => { setTimeout(esconderBarra, 500); });"
                "['touchstart', 'scroll', 'click'].forEach(e => {"
                "  window.addEventListener(e, () => setTimeout(esconderBarra, 500));"
                "});"

                "function toggleFullscreen() {"
                "  const el = document.documentElement;"
                "  if (!document.fullscreenElement) {"
                "    if (el.requestFullscreen) el.requestFullscreen();"
                "    else if (el.webkitRequestFullscreen) el.webkitRequestFullscreen();"
                "    else if (el.msRequestFullscreen) el.msRequestFullscreen();"
                "  } else {"
                "    if (document.exitFullscreen) document.exitFullscreen();"
                "    else if (document.webkitExitFullscreen) document.webkitExitFullscreen();"
                "    else if (document.msExitFullscreen) document.msExitFullscreen();"
                "  }"
                "}"
                "</script>"

                "</body></html>";
  server.send(200, "text/html", html);
}


void setup() {
  Serial.begin(115200);
  delay(3000);

  //padroesFabrica();

  obterConfiguracoes();



  Wire.begin(SDA_PIN, SCL_PIN);

  byte error, address;
  int devices = 0;

  for (address = 1; address < 127; address++) {
    Wire.beginTransmission(address);
    error = Wire.endTransmission();

    if (error == 0) {
      Serial.print("Dispositivo I2C encontrado no endereço 0x");
      if (address < 16) Serial.print("0");
      Serial.println(address, HEX);
      devices++;
    } else if (error == 4) {
      Serial.print("Erro desconhecido no endereço 0x");
      if (address < 16) Serial.print("0");
      Serial.println(address, HEX);
    }
  }

  if (devices == 0) {
    Serial.println("Nenhum dispositivo I2C encontrado.");
  } else {
    Serial.println("Scanner finalizado.");
  }

  if (!mcp.begin_I2C(0x20, &Wire)) {
    Serial.println("Falha ao inicializar MCP23017");
    while (1)
      ;
  }

  mcp.pinMode(IN1, OUTPUT);
  mcp.pinMode(IN2, OUTPUT);

  mcp.pinMode(IN3, OUTPUT);
  mcp.pinMode(IN4, OUTPUT);

  pinMode(sensorMeio, INPUT);
  pinMode(sensorEsq, INPUT);
  pinMode(sensorDir, INPUT);


  ledcSetup(0, 3000, 10);  // 10 bits → 0 a 1023
  ledcAttachPin(ENA, 0);

  ledcSetup(1, 3000, 10);  // 10 bits → 0 a 1023
  ledcAttachPin(ENB, 1);

  parar();

  iniciarHostpotWifi();  //Ininicar a Placa como HOSTPOT e Servidor WEB


  iniciarClienteWifi();








  camera_config_t config;
  config.ledc_channel = LEDC_CHANNEL_0;
  config.ledc_timer = LEDC_TIMER_0;
  config.pin_d0 = Y2_GPIO_NUM;
  config.pin_d1 = Y3_GPIO_NUM;
  config.pin_d2 = Y4_GPIO_NUM;
  config.pin_d3 = Y5_GPIO_NUM;
  config.pin_d4 = Y6_GPIO_NUM;
  config.pin_d5 = Y7_GPIO_NUM;
  config.pin_d6 = Y8_GPIO_NUM;
  config.pin_d7 = Y9_GPIO_NUM;
  config.pin_xclk = XCLK_GPIO_NUM;
  config.pin_pclk = PCLK_GPIO_NUM;
  config.pin_vsync = VSYNC_GPIO_NUM;
  config.pin_href = HREF_GPIO_NUM;
  config.pin_sccb_sda = SIOD_GPIO_NUM;
  config.pin_sccb_scl = SIOC_GPIO_NUM;
  config.pin_pwdn = PWDN_GPIO_NUM;
  config.pin_reset = RESET_GPIO_NUM;
  config.xclk_freq_hz = 20000000;
  config.frame_size = FRAMESIZE_HD;
  config.pixel_format = PIXFORMAT_JPEG;  // for streaming
  //config.pixel_format = PIXFORMAT_RGB565; // for face detection/recognition
  config.grab_mode = CAMERA_GRAB_WHEN_EMPTY;
  config.fb_location = CAMERA_FB_IN_PSRAM;
  config.jpeg_quality = 12;
  config.fb_count = 1;

  // if PSRAM IC present, init with UXGA resolution and higher JPEG quality
  //                      for larger pre-allocated frame buffer.
  if (config.pixel_format == PIXFORMAT_JPEG) {
    if (psramFound()) {
      config.jpeg_quality = 10;
      config.fb_count = 2;
      config.grab_mode = CAMERA_GRAB_LATEST;
    } else {
      // Limit the frame size when PSRAM is not available
      config.frame_size = FRAMESIZE_HD;
      config.fb_location = CAMERA_FB_IN_DRAM;
    }
  } else {
    // Best option for face detection/recognition
    config.frame_size = FRAMESIZE_240X240;
#if CONFIG_IDF_TARGET_ESP32S3
    config.fb_count = 2;
#endif
  }

#if defined(CAMERA_MODEL_ESP_EYE)
  pinMode(13, INPUT_PULLUP);
  pinMode(14, INPUT_PULLUP);
#endif

  // camera init
  Serial.println("Antes de iniciar");

  esp_err_t err = esp_camera_init(&config);
  if (err != ESP_OK) {
    Serial.printf("Camera init failed with error 0x%x", err);
    return;
  }

  sensor_t *s = esp_camera_sensor_get();
  // initial sensors are flipped vertically and colors are a bit saturated
  if (s->id.PID == OV3660_PID) {
    s->set_vflip(s, 1);        // flip it back
    s->set_brightness(s, 1);   // up the brightness just a bit
    s->set_saturation(s, -2);  // lower the saturation
  }
  // drop down frame size for higher initial frame rate
  if (config.pixel_format == PIXFORMAT_JPEG) {
    s->set_framesize(s, FRAMESIZE_VGA);
  }

#if defined(CAMERA_MODEL_M5STACK_WIDE) || defined(CAMERA_MODEL_M5STACK_ESP32CAM)
  s->set_vflip(s, 1);
  s->set_hmirror(s, 1);
#endif

#if defined(CAMERA_MODEL_ESP32S3_EYE)
  s->set_vflip(s, 1);
#endif



  if (WiFi.status() == WL_CONNECTED) {
    boolean processar = false;

    String acao = buscarAcoesWifi();
    if (acao.equals("errou")) {
      processar = false;
    } else {
      processar = true;
    }

    if (processar) {
      if (acao != "" && acao.length() > 5) {
        processarRequisicao(acao);
      }
    }
  }

  if (MDNS.begin("bondinho" + SERIAL_NUMBER)) {  // Isso cria o domínio bondinho.local
    Serial.print("MDNS responder started: bondinho");
    Serial.print(SERIAL_NUMBER);
    Serial.print(".local");
  } else {
    Serial.println("Erro ao iniciar o MDNS");
  }




  Serial.println("Fim do setup 2!");
}

void loop() {
  server.handleClient();

  if (Serial.available()) {
    char c = Serial.read();
    if (c == 'a') {
      rodar = true;
    }
    if (c == 'b') {
      rodar = false;
    }
    if (c == 'c') {
      ESP.restart();
    }
  }


  int valorSensorEsq = digitalRead(sensorEsq);
  int valorSensorDir = digitalRead(sensorDir);

  // Faixa preta = 1, branca = 0

  if (rodar) {


    if (valorSensorEsq == 0 && valorSensorDir == 0) {
      //Serial.println("Frente ok");
      frente();  // Centralizado na linha
      delay(velocidade_arranque * 1);
    } else if (valorSensorDir == 1 && valorSensorEsq == 0) {
      //Serial.println("Virar Esquerda levemente");
      esquerda();
      delay(velocidade_arranque * 1);
    } else if (valorSensorEsq == 1 && valorSensorDir == 0) {
      //Serial.println("Virar Direita levemente");
      direita();
      delay(velocidade_arranque * 1);
    } else if (valorSensorEsq == 1 && valorSensorDir == 1) {
      // Serial.println("Bifurcação!");


      if (direcao) {
        for (int i = 0; i < 10; i++) {
          esquerda();
          delay(delay_bifurcacao);
        }
      } else {
        for (int i = 0; i < 10; i++) {
          direita();
          delay(delay_bifurcacao);
        }
      }
    }
  } else {
    parar();
  }
}




void padroesFabrica() {

  EEPROM.begin(200);
  EEPROM.write(POS_VEL_ARRANQUE, 50);
  EEPROM.write(POS_VEL_TRAJETO, 50);
  EEPROM.write(POS_FATOR_CORRECAO, 50);


  EEPROM.commit();  // Salva alterações na FLASH
  EEPROM.end();     // Apaga a cópia da EEPROM salva na RAM
}




void obterConfiguracoes() {
  delay(500);
  EEPROM.begin(900);
  velocidade_arranque = EEPROM.read(POS_VEL_ARRANQUE);

  velocidade_trajeto = EEPROM.read(POS_VEL_TRAJETO);

  fator_correcao = EEPROM.read(POS_FATOR_CORRECAO);

  delay_bifurcacao = EEPROM.read(POS_DELAY_BIF);


  EEPROM.end();

  Serial.print("Velocidade arranque: ");
  Serial.println(velocidade_arranque);

  Serial.print("Velocidade trajeto: ");
  Serial.println(velocidade_trajeto);

  Serial.print("Fator de correção: ");
  Serial.println(fator_correcao);

  Serial.print("Delay Bifurcacao: ");
  Serial.println(delay_bifurcacao);
}





void processarRequisicao(String acao) {

  // Tamanho da string JSON
  const size_t capacity = JSON_OBJECT_SIZE(4) + 256;
  DynamicJsonDocument doc(capacity);

  // Parse da string JSON
  DeserializationError error = deserializeJson(doc, acao);

  // Verificar se houve erro no parse
  if (error) {
    Serial.print("Falha ao analisar JSON: ");
    Serial.println(error.c_str());

    return;
  }

  // Obter os valores de "tipo_requisicao" e "id_tarefa" como inteiros
  int tipoRequisicao = doc["tipo_requisicao"].as<int>();
  int idTarefa = doc["id_tarefa"].as<int>();

  String valorString1 = "";
  String valorString2 = "";
  String valorString3 = "";
  String valorString4 = "";


  if (tipoRequisicao >= 90) {
    valorString1 = doc["valor_string1"].as<String>();
    valorString2 = doc["valor_string2"].as<String>();
    valorString3 = doc["valor_string3"].as<String>();
    valorString4 = doc["valor_string4"].as<String>();
  }


  Serial.print("Valor string 1: ");
  Serial.println(valorString1);
  Serial.print("Valor string 2: ");
  Serial.println(valorString2);



  // Usar os valores conforme necessário
  Serial.print("Tipo de Requisicao: ");
  Serial.println(tipoRequisicao);
  Serial.print("ID da Tarefa: ");
  Serial.println(idTarefa);

  if (tipoRequisicao == 70) {
    respostaAcao(idTarefa);
    buscarAtt();
  }
}



boolean respostaAcao(int id_acao) {



  String acao = responderAcao(id_acao);
  if (acao.equals("sucesso"))
    return true;


  return false;
}
