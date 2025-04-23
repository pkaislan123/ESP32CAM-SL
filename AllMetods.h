
void iniciarServidorWeb();
void iniciarHostpotWifi();
void handleMain();
void handleForm();
void connectToWiFi(String ssidWifi, String passwordWifi, String index);
void connectToWiFi(String ssidWifi, String passwordWifi);
String listSSID();
void handleGetWifiList();
void handleUpdateWifiList();
String informarAcao(int id_requisicao, int origem);
boolean compareEeprom(String ssidWifi, String passwordWifi);
void iniciarClienteWifi();
void connectEeprom(String index);
void connectEeprom();
void handleUpdateWifiListEsp32();
void handleUpdateWifiListEsp8266();
void salvarEeprom(String ssidWifi, String passwordWifi);
boolean reconnectWiFi();
 
void handleLogin();
String lerSenhaAdmin();
void handleExecuteLogin();
void handleConfigs();

void handleExecuteConfigs();
void obterConfiguracoes();
 


 
bool validarEnderecamentoWifi();
 
void lerConfiguracoesWifi();
 
String updateStatusWifi();
boolean respostaAcao(int id_acao);
void lerConfiguracoesWifiSpiffs();
 void salvarConfiguracoesWifiSpiffs(String enderecoIp, String gateway, String subnet);
 

String buscarAcoes();
void buscarAtt();
String responderAcao(int id_requisicao);

void arranque();
void parar();
void tras();
void frente();
void esquerda();
void direita();
void trasEsquerda();
void trasDireita();
void streamTask(void *parameter);
void handleRoot();
     