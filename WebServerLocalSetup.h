/*************** SERVIDOR WEB FUNCOES *************************/










void handleLogin() {

  if (!logado) {
    String responsePage = (const __FlashStringHelper*)Login_page;  //Leia o conteúdo HTML
    server.send(200, "text/html", responsePage);
  } else {
    handleMain();
  }
}



void handleExecuteLogin() {
  String senha_admin = server.arg("password");

  String responsePage = (const __FlashStringHelper*)Login_page;  //Leia o conteúdo HTML

  String senha_admin_armazenada = "b0ndinh0";


  if (senha_admin_armazenada == senha_admin) {
    logado = true;
    handleMain();
    // return;
  } else {
    responsePage.replace("<br><br>", "<p id='status'>Senha Inválida!</p>");
    server.send(200, "text/html", responsePage);
  }
}






void iniciarServidorWeb() {


  server.on("/", handleLogin);
  server.on("/action_login", handleExecuteLogin);
  server.on("/wifi", handleMain);
  server.on("/action_new_connection", handleForm);
  server.on("/update_wifi_list", handleUpdateWifiList);
  server.on("/get_wifi_list", handleGetWifiList);



server.on("/controlviewv", []() {
  String html = "<html><head>"
                "<meta name='viewport' content='width=device-width, initial-scale=1.0, maximum-scale=1.0, user-scalable=no'>"
                "<style>"
                "* {margin: 0; padding: 0; box-sizing: border-box;}"
                "html, body {"
                "  height: 100%;"
                "  width: 100%;"
                "  display: flex;"
                "  flex-direction: column;"
                "  background-color: black;"
                "  color: white;"
                "  overflow: hidden;"
                "}"
                "#video-container {"
                "  flex: 1;"
                "  display: flex;"
                "  justify-content: center;"
                "  align-items: center;"
                "  overflow: hidden;"
                "  padding: 10px;"
                "}"
                "#video {"
                "  width: 100%;"
                "  height: 50%;"
                "  object-fit: contain;"
                "}"
                ".controle-container {"
                "  padding: 10px;"
                "  background-color: #000;"
                "  display: flex;"
                "  flex-direction: column;"
                "  align-items: center;"
                "  justify-content: center;"
                "}"
                ".linha-controles {"
                "  display: flex;"
                "  flex-direction: column;"
                "  gap: 10px;"
                "  align-items: center;"
                "}"
                ".grupo, .controle-acoes {"
                "  display: flex;"
                "  flex-direction: row;"
                "  gap: 10px;"
                "  align-items: center;"
                "}"
                "button {"
                "  padding: 12px 24px;"
                "  font-size: 16px;"
                "  cursor: pointer;"
                "  border: none;"
                "  border-radius: 6px;"
                "  transition: 0.2s;"
                "}"
                ".start { background: #4CAF50; color: white; }"
                ".stop { background: #f44336; color: white; }"
                ".seta {"
                "  font-size: 48px;"
                "  padding: 3px 3px;"
                "  border-radius: 8px;"
                "  cursor: pointer;"
                "  background: blue;"
                "  color: white;"
                "  border: 2px solid black;"
                "  transition: 0.2s;"
                "}"
                ".seta:hover { background: blue; }"
                ".ativo {"
                "  font-size: 56px;"
                "  border: 3px solid white;"
                "}"
                "@media (orientation: landscape) {"
                "  .linha-controles {"
                "    flex-direction: row;"
                "    justify-content: center;"
                "    gap: 20px;"
                "  }"
                "  .grupo, .controle-acoes {"
                "    flex-direction: column;"
                "  }"
                "}"
                "</style></head><body>"

                "<div>"
                "<div id='video-container'>"
                "<img src='/stream' id='video'>"
                "</div>"
                "<div class='linha-controles'>"
                "<div class='controle-acoes'>"
                "<button class='start' onclick='enviarAcao(\"/iniciar\")'>Iniciar</button>"
                "<button class='stop' onclick='enviarAcao(\"/parar\")'>Parar</button>"
                "</div>"
                "<div class='grupo'>"
                "<div id='setaEsq' class='seta " + String(direcao ? "" : "ativo") + "' onclick='selecionarDirecao(false)'>&larr;</div>"
                "<div id='setaDir' class='seta " + String(direcao ? "ativo" : "") + "' onclick='selecionarDirecao(true)'>&rarr;</div>"
                "</div>"
                "</div></div>"

                "<script>"
                "function selecionarDirecao(valor){"
                "  fetch('/definir_direcao?valor=' + valor);"
                "  document.getElementById('setaEsq').classList.remove('ativo');"
                "  document.getElementById('setaDir').classList.remove('ativo');"
                "  if(valor){document.getElementById('setaDir').classList.add('ativo');} "
                "  else{document.getElementById('setaEsq').classList.add('ativo');}"
                "} "
                "function enviarAcao(url){"
                "  fetch(url + '?redirect=none').then(()=>console.log('Comando enviado'));"
                "} "
                "function toggleFullscreen() {"
                "  var elem = document.getElementById('video');"
                "  if (document.fullscreenElement) {"
                "    document.exitFullscreen();"
                "  } else {"
                "    elem.requestFullscreen().catch(err => console.log(err));"
                "  }"
                "} "
                "</script>"

                "</body></html>";

  server.send(200, "text/html", html);
});


server.on("/controlviewh", []() {
  String html = "<!DOCTYPE html><html><head>"
                "<meta charset='UTF-8'>"
                "<meta name='viewport' content='width=device-width, initial-scale=1.0, maximum-scale=1.0, user-scalable=no'>"
                "<meta name='mobile-web-app-capable' content='yes'>"
                "<meta name='apple-mobile-web-app-capable' content='yes'>"
                "<meta name='apple-mobile-web-app-status-bar-style' content='black-translucent'>"
                "<style>"
                "* {margin: 0; padding: 0; box-sizing: border-box;}"
                "html, body {"
                "  height: 100%;"
                "  width: 100%;"
                "  display: flex;"
                "  flex-direction: column;"
                "  background-color: black;"
                "  color: white;"
                "  overflow: hidden;"
                "}"
                "#video-container {"
                "  position: relative;"
                "  flex: 1;"
                "  display: flex;"
                "  justify-content: center;"
                "  align-items: center;"
                "  overflow: hidden;"
                "  padding: 2px;"
                "}"
                "#video {"
                "  width: 100%;"
                "  height: 99%;"
                "}"
                ".linha-controles {"
                "  position: absolute;"
                "  bottom: 0;"
                "  left: 0;"
                "  right: 0;"
                "  display: flex;"
                "  flex-direction: row;"
                "  justify-content: space-between;"  // Alinha os grupos nas pontas
                "  padding: 10px 20px;"              // Recuo lateral de 20px
                "  background-color: rgba(0, 0, 0, 0.0);"
                "}"
                ".controle-acoes, .grupo {"
                "  display: flex;"
                "  flex-direction: row;"
                "  align-items: center;"
                "  gap: 10px;"
                "}"
                "button {"
                "  padding: 6px 6px;"
                "  font-size: 24px;"
                "  cursor: pointer;"
                "  border: none;"
                "  border-radius: 6px;"
                "  transition: 0.2s;"
                "}"
                ".start { background: #4CAF50; color: white; }"
                ".stop { background: #f44336; color: white; }"
                ".seta {"
                "  font-size: 28px;"
                "  padding: 10px 28px;"
                "  border-radius: 4px;"
                "  cursor: pointer;"
                "  background: blue;"
                "  color: white;"
                "  border: 2px solid transparent;"
                "  transition: all 0.2s ease;"
                "}"
                ".seta:hover { background: blue; }"
                ".ativo {"
                "  border: 3px solid white;"
                "}"
                "</style></head><body>"

                "<div id='video-container'>"
                "<img src='/stream' id='video'>"

                "<div class='linha-controles'>"
                "<div class='controle-acoes'>"
                "<button class='start' onclick='enviarAcao(\"/iniciar\")'>Iniciar</button>"
                "<button class='stop' onclick='enviarAcao(\"/parar\")'>Parar</button>"
                "</div>"
                "<div class='grupo'>"
                "<div id='setaEsq' class='seta " + String(direcao ? "" : "ativo") + "' onclick='selecionarDirecao(false)'>&larr;</div>"
                "<div id='setaDir' class='seta " + String(direcao ? "ativo" : "") + "' onclick='selecionarDirecao(true)'>&rarr;</div>"
                "</div>"
                "</div>"

                "</div>" // fecha video-container

                "<script>"
                "function selecionarDirecao(valor){"
                "  fetch('/definir_direcao?valor=' + valor);"
                "  document.getElementById('setaEsq').classList.remove('ativo');"
                "  document.getElementById('setaDir').classList.remove('ativo');"
                "  if(valor){document.getElementById('setaDir').classList.add('ativo');} "
                "  else{document.getElementById('setaEsq').classList.add('ativo');} "
                "} "
                "function enviarAcao(url){"
                "  fetch(url + '?redirect=none').then(()=>console.log('Comando enviado'));"
                "} "
                "window.onload = function() {"
                "  setTimeout(function() { window.scrollTo(0, 1); }, 100);"
                "};"
                "</script>"

                "</body></html>";

  server.send(200, "text/html", html);
});

server.on("/controlviewpc", []() {
  String html = "<!DOCTYPE html><html><head>"
                "<meta charset='UTF-8'>"
                "<meta name='viewport' content='width=device-width, initial-scale=1.0, maximum-scale=1.0, user-scalable=no'>"
                "<meta name='mobile-web-app-capable' content='yes'>"
                "<meta name='apple-mobile-web-app-capable' content='yes'>"
                "<meta name='apple-mobile-web-app-status-bar-style' content='black-translucent'>"
                "<style>"
                "* {margin: 0; padding: 0; box-sizing: border-box;}"
                "html, body {"
                "  height: 100%;"
                "  width: 100%;"
                "  background-color: black;"
                "  color: white;"
                "  overflow: hidden;"
                "  font-family: sans-serif;"
                "}"
                "#video-container {"
                "  position: relative;"
                "  height: 100%;"
                "  width: 100%;"
                "}"
                "#video {"
                "  width: 100%;"
                "  height: 100%;"
                "  object-fit: cover;"
                "}"
                ".painel-controles {"
                "  position: absolute;"
                "  bottom: 10px;"
                "  right: 10px;"
                "  display: flex;"
                "  flex-direction: column;"
                "  align-items: flex-end;"
                "  gap: 10px;"
                "}"
                "button, .seta {"
                "  padding: 10px 20px;"
                "  font-size: 24px;"
                "  cursor: pointer;"
                "  border: none;"
                "  border-radius: 6px;"
                "}"
                ".start { background: #4CAF50; color: white; }"
                ".stop { background: #f44336; color: white; }"
                ".grupo-setas {"
                "  display: flex;"
                "  gap: 10px;"
                "  justify-content: flex-end;"
                "}"
                ".seta {"
                "  font-size: 28px;"
                "  width: 60px;"
                "  height: 60px;"
                "  display: flex;"
                "  align-items: center;"
                "  justify-content: center;"
                "  background: blue;"
                "  color: white;"
                "  border: 3px solid transparent;"
                "  border-radius: 6px;"
                "  transition: all 0.2s ease;"
                "}"
                ".ativo {"
                "  border-color: white;"
                "}"
                "</style></head><body>"

                "<div id='video-container'>"
                "<img src='/stream' id='video'>"

                "<div class='painel-controles'>"
                "<button class='start' onclick='enviarAcao(\"/iniciar\")'>Iniciar</button>"
                "<button class='stop' onclick='enviarAcao(\"/parar\")'>Parar</button>"
                "<div class='grupo-setas'>"
                "<div id='setaEsq' class='seta " + String(direcao ? "" : "ativo") + "' onclick='selecionarDirecao(false)'>&larr;</div>"
                "<div id='setaDir' class='seta " + String(direcao ? "ativo" : "") + "' onclick='selecionarDirecao(true)'>&rarr;</div>"
                "</div>"
                "</div>" // fecha painel-controles

                "</div>" // fecha video-container

                "<script>"
                "function selecionarDirecao(valor){"
                "  fetch('/definir_direcao?valor=' + valor);"
                "  document.getElementById('setaEsq').classList.remove('ativo');"
                "  document.getElementById('setaDir').classList.remove('ativo');"
                "  if(valor){document.getElementById('setaDir').classList.add('ativo');} "
                "  else{document.getElementById('setaEsq').classList.add('ativo');} "
                "} "
                "function enviarAcao(url){"
                "  fetch(url + '?redirect=none').then(()=>console.log('Comando enviado'));"
                "} "
                "window.onload = function() {"
                "  setTimeout(function() { window.scrollTo(0, 1); }, 100);"
                "};"
                "</script>"

                "</body></html>";

  server.send(200, "text/html", html);
});

server.on("/control", []() {
  String html = "<html><head><meta name='viewport' content='width=device-width, initial-scale=1.0'>"
                "<style>"
                "html, body {margin:0; padding:0; height:100%; font-family:sans-serif; background:#f0f0f0;}"
                "body {display:flex; flex-direction:column; height:100vh;}"
                ".controle-container {flex: 1; display:flex; flex-direction:column; justify-content:center; align-items:center; padding:20px; box-sizing:border-box;}"
                ".grupo-botoes {width:100%; max-width:300px; display:flex; flex-direction:column; gap:12px; padding:0 10px; margin:0;}" // 🔽 Remove margem inferior
                ".grupo-botoes form {width: 100%;}"
                ".grupo-botoes button {width: 100%;}"
                ".grupo-setas {width:100%; max-width:300px; display:flex; flex-direction:row; justify-content:space-between; gap:16px; padding:0 10px; margin-top:0;}" // 🔽 Sem espaçamento
                "button {padding:14px; font-size:18px; cursor:pointer; border:none; border-radius:6px;}"
                ".start {background:#4CAF50; color:white;}"
                ".stop {background:#f44336; color:white;}"
                ".seta {"
                "  font-size: 32px;"
                "  padding: 10px 30px;" // ⬅️ Mais largura
                "  border-radius: 4px;"
                "  cursor: pointer;"
                "  background: #007BFF;"
                "  color: white;"
                "  border: 3px solid white;" // Borda branca padrão
                "  transition: all 0.2s ease;"
                "}"
                ".seta:hover {"
                "  background: #0056b3;"
                "}"
                ".ativo {"
                "  background: #3399FF;" // Fundo azul claro quando ativo
                "  border-color: #3399FF;" // Borda azul claro quando ativo
                "  transform: scale(1.1);"
                "}"
                "</style></head><body>"

                "<div class='controle-container'>"

                "<div class='grupo-botoes'>"
                "<form action='/iniciar' method='get'>"
                "<input type='hidden' name='redirect' value='/control'>"
                "<button class='start'>Iniciar</button>"
                "</form>"

                "<form action='/parar' method='get'>"
                "<input type='hidden' name='redirect' value='/control'>"
                "<button class='stop'>Parar</button>"
                "</form>"
                "</div>"

                "<div class='grupo-setas'>"
                "<div id='setaEsq' class='seta " + String(direcao ? "" : "ativo") + "' onclick='selecionarDirecao(false)'>&larr;</div>"
                "<div id='setaDir' class='seta " + String(direcao ? "ativo" : "") + "' onclick='selecionarDirecao(true)'>&rarr;</div>"
                "</div>"

                "</div>"

                "<script>"
                "function selecionarDirecao(valor){"
                "fetch('/definir_direcao?valor=' + valor);"
                "document.getElementById('setaEsq').classList.remove('ativo');"
                "document.getElementById('setaDir').classList.remove('ativo');"
                "if(valor){document.getElementById('setaDir').classList.add('ativo');}"
                "else{document.getElementById('setaEsq').classList.add('ativo');}"
                "}"
                "</script>"

                "</body></html>";

  server.send(200, "text/html", html);
});

  

server.on("/configs", []() {
  String html = "<html><head><meta name='viewport' content='width=device-width, initial-scale=1'>"
                "<style>"
                "body{display:flex;flex-direction:column;align-items:center;justify-content:center;background:#f0f0f0;font-family:sans-serif;margin:0;padding:20px;height:100vh;box-sizing:border-box;overflow:hidden;}"
                "button{padding:12px 24px;font-size:18px;cursor:pointer;border:none;border-radius:8px;margin:8px;}"
                ".slider-container{margin:10px;width:100%;max-width:400px;}"
                ".label{font-size:16px;margin-bottom:5px;font-weight:bold;}"
                ".versao{margin-top:20px;font-size:14px;color:#888;text-align:center;}"
                ".direcao-btns{display:flex;gap:20px;justify-content:center;margin:10px 0;}"
                ".seta{font-size:30px;padding:10px 20px;border-radius:10px;cursor:pointer;background:blue;color:white;border:2px solid white;}"
               ".ativo {"
                "  border-color: #66ccff !important;"  // azul claro
                "  transform: scale(1.15);"
                "  box-shadow: 0 0 12px rgba(102,204,255,0.7);"
                "}"
                "small{display:block;color:#555;margin-bottom:5px;}"
                "@media (max-height:600px){body{justify-content:flex-start;overflow:auto;padding:10px;}}"
                "</style></head><body>"

                // Botões Iniciar e Parar
                "<div>"
                "<form action=\"/iniciar\" method=\"get\" style='display:inline;'>"
                "<input type='hidden' name='redirect' value='/configs'>"
                "<button style=\"background:#4CAF50;color:white;\">Iniciar</button>"
                "</form>"

                "<form action=\"/parar\" method=\"get\" style='display:inline;'>"
                "<input type='hidden' name='redirect' value='/configs'>"
                "<button style=\"background:#f44336;color:white;\">Parar</button>"
                "</form>"
                "</div>"

                // Botões de direção (setas)
                "<div class='direcao-btns'>"
                "<div id='setaEsq' class='seta " + String(direcao ? "" : "ativo") + "' onclick='selecionarDirecao(false)'>&larr;</div>"
                "<div id='setaDir' class='seta " + String(direcao ? "ativo" : "") + "' onclick='selecionarDirecao(true)'>&rarr;</div>"
                "</div>"

                // Sliders
                "<div class='slider-container'>"
                "<div class='label'>Delay correcao: <span id='valArranque'>" + String(velocidade_arranque) + "</span></div>"
                "<small>Atraso a cada verificacao de posicao (quanto maior o valor, mais tempo leva entre cada leitura)</small>"
                "<input type='range' min='0' max='100' value='" + String(velocidade_arranque) + "' onchange='atualizarArranque(this.value)'/>"
                "</div>"

                "<div class='slider-container'>"
                "<div class='label'>Velocidade do carrinho: <span id='valTrajeto'>" + String(velocidade_trajeto) + "</span></div>"
                "<small>Velocidade do carrinho (quanto maior o valor, maior a velocidade de deslocamento)</small>"
                "<input type='range' min='0' max='100' value='" + String(velocidade_trajeto) + "' onchange='atualizarTrajeto(this.value)'/>"
                "</div>"

                "<div class='slider-container'>"
                "<div class='label'>Velocidade de Correcao: <span id='valCorrecao'>" + String(fator_correcao) + "</span></div>"
                "<small>Velocidade da roda contraria ao efetuar curvas (0 = parada, maior = mais velocidade)</small>"
                "<input type='range' min='0' max='100' value='" + String(fator_correcao) + "' onchange='atualizarCorrecao(this.value)'/>"
                "</div>"

                "<div class='slider-container'>"
                "<div class='label'>Delay Bifurcacao (ms): <span id='valDelayBif'>" + String(delay_bifurcacao) + "</span></div>"
                "<small>Tempo de curva ao detectar bifurcacao (maior = curva mais longa)</small>"
                "<input type='range' min='1' max='100' step='10' value='" + String(delay_bifurcacao) + "' onchange='atualizarDelayBif(this.value)'/>"
                "</div>"

                "<div class='versao'>Firmware v30.0</div>"

                "<script>"
                "function atualizarArranque(val){"
                "document.getElementById('valArranque').innerText = val;"
                "fetch('/salvar_arranque?valor=' + val);"
                "}"
                "function atualizarTrajeto(val){"
                "document.getElementById('valTrajeto').innerText = val;"
                "fetch('/salvar_trajeto?valor=' + val);"
                "}"
                "function atualizarCorrecao(val){"
                "document.getElementById('valCorrecao').innerText = val;"
                "fetch('/salvar_correcao?valor=' + val);"
                "}"
                "function atualizarDelayBif(val){"
                "document.getElementById('valDelayBif').innerText = val;"
                "fetch('/salvar_delaybifurcacao?valor=' + val);"
                "}"
                "function selecionarDirecao(valor){"
                "fetch('/definir_direcao?valor=' + valor);"
                "document.getElementById('setaEsq').classList.remove('ativo');"
                "document.getElementById('setaDir').classList.remove('ativo');"
                "if(valor){document.getElementById('setaDir').classList.add('ativo');}"
                "else{document.getElementById('setaEsq').classList.add('ativo');}"
                "}"
                "</script>"

                "</body></html>";

  server.send(200, "text/html", html);
});


 server.on("/iniciar", []() {
  rodar = true;
  if (server.hasArg("redirect") && server.arg("redirect") == "none") {
    server.send(200, "text/plain", "OK");
  } else {
    String redirectUrl = server.hasArg("redirect") ? server.arg("redirect") : "/controle";
    server.sendHeader("Location", redirectUrl);
    server.send(303);
  }
});

server.on("/parar", []() {
  rodar = false;
  if (server.hasArg("redirect") && server.arg("redirect") == "none") {
    server.send(200, "text/plain", "OK");
  } else {
    String redirectUrl = server.hasArg("redirect") ? server.arg("redirect") : "/controle";
    server.sendHeader("Location", redirectUrl);
    server.send(303);
  }
});

  server.on("/salvar_arranque", []() {
    if (server.hasArg("valor")) {
      int novoValor = server.arg("valor").toInt();
      EEPROM.begin(200);
      EEPROM.write(POS_VEL_ARRANQUE, novoValor);
      EEPROM.commit();
      EEPROM.end();
      velocidade_arranque = novoValor;
      Serial.print("Nova velocidade de arranque: ");
      Serial.println(novoValor);
    }
    server.send(200, "text/plain", "OK");
  });

  server.on("/salvar_trajeto", []() {
    if (server.hasArg("valor")) {
      int novoValor = server.arg("valor").toInt();
      EEPROM.begin(200);
      EEPROM.write(POS_VEL_TRAJETO, novoValor);
      EEPROM.commit();
      EEPROM.end();
      velocidade_trajeto = novoValor;
      Serial.print("Nova velocidade de trajeto: ");
      Serial.println(novoValor);
    }
    server.send(200, "text/plain", "OK");
  });


  server.on("/salvar_correcao", []() {
    if (server.hasArg("valor")) {

      int novoValor = server.arg("valor").toInt();
      EEPROM.begin(200);
      EEPROM.write(POS_FATOR_CORRECAO, novoValor);
      EEPROM.commit();
      EEPROM.end();
      fator_correcao = novoValor;
      Serial.print("Fator de correcao ");
      Serial.println(novoValor);
    }
    server.send(200, "text/plain", "OK");
  });

  // Rota que chama a função buscarAtt()
  server.on("/atualizar", []() {
    buscarAtt();  // Sua função de atualização
    server.send(200, "text/plain", "Atualizando firmware...");
  });

  server.on("/definir_direcao", []() {
    if (server.hasArg("valor")) {
      String val = server.arg("valor");
      direcao = (val == "true");
      Serial.println("Direção definida para: " + String(direcao ? "Direita" : "Esquerda"));
    }
    server.send(200, "text/plain", "Direção atualizada");
  });


  server.on("/salvar_delaybifurcacao", []() {
    if (server.hasArg("valor")) {
      int novoDelay = server.arg("valor").toInt();
      EEPROM.begin(200);
      EEPROM.write(POS_DELAY_BIF, novoDelay);
      EEPROM.commit();
      EEPROM.end();
      delay_bifurcacao = novoDelay;
      Serial.print("Novo delay de bifurcacao: ");
      Serial.println(novoDelay);
    }
    server.send(200, "text/plain", "OK");
  });

  // Rotas
  server.on("/view", handleRoot);

  server.on("/stream", HTTP_GET, []() {
    if (streaming) {
      server.send(503, "text/plain", "Stream já em andamento");
      return;
    }

    streamClient = server.client();
    streaming = true;

    xTaskCreatePinnedToCore(
      streamTask,
      "StreamTask",
      8192,
      NULL,
      1,
      NULL,
      0  // Core 0 para deixar o Core 1 livre para o loop
    );
  });

  server.begin();




  server.begin();
  Serial.println("[DEBUG] [WEB SERVER] [SUCESSO] [iniciarServidorWeb] [Servidor HTTP iniciado] []");
}

void iniciarHostpotWifi() {


  //Coloca o dispositivo no modo Wi-Fi Station
  //WiFi.mode(WIFI_STA); //Importante Definir para que o espnow funcione
  //WiFi.mode(WIFI_MODE_AP);

  String ssid_complete = String(ssid_hostpot) + " " + SERIAL_NUMBER;
  const char* ssid_final = ssid_complete.c_str();

  WiFi.softAP(ssid_final, password_hostpot);


  Serial.println("[DEBUG] [WEB SERVER] [SUCESSO] [iniciarHostpotWifi] [HOSTPOT WIFI INICIADO] []");

  Serial.print("[DEBUG] [WEB SERVER] [SUCESSO] [iniciarHostpotWifi] [ENDEREÇO IP: ");
  Serial.print(WiFi.softAPIP());
  Serial.print("] []");

  Serial.print("[DEBUG] [WEB SERVER] [SUCESSO] [iniciarHostpotWifi] [REDE: ");
  Serial.print(ssid_final);
  Serial.print("] []");


  iniciarServidorWeb();
}







void parseIPString(String ipString, int& part1, int& part2, int& part3, int& part4) {
  int dotCount = 0;
  String part;
  for (char c : ipString) {
    if (c == '.') {
      switch (dotCount) {
        case 0:
          part1 = part.toInt();
          break;
        case 1:
          part2 = part.toInt();
          break;
        case 2:
          part3 = part.toInt();
          break;
        case 3:
          part4 = part.toInt();
          break;
      }
      dotCount++;
      part = "";
    } else {
      part += c;
    }
  }
  // Observe que este bloco finaliza a última parte, após o último ponto.
  switch (dotCount) {
    case 0:
      part1 = part.toInt();
      break;
    case 1:
      part2 = part.toInt();
      break;
    case 2:
      part3 = part.toInt();
      break;
    case 3:
      part4 = part.toInt();
      break;
  }
}



void salvarConfiguracoesWifiSpiffs(String enderecoIp, String gateway, String subnet) {

  writeFile(enderecoIp, PATH_IP_WIFI);
  writeFile(gateway, PATH_GAT_WIFI);
  writeFile(subnet, PATH_SUB_WIFI);
  writeFile("1", PATH_CONFIG_WIFI);
}



void lerConfiguracoesWifiSpiffs() {


  File Fconfig_wifi = SPIFFS.open(PATH_CONFIG_WIFI, "r");
  if (!Fconfig_wifi) {
    Serial.println("Erro ao abrir arquivo com Configurado?  Wifi!");

    configurado_wifi_global = 0;
    return;
  }
  String configurado = Fconfig_wifi.readStringUntil('\r');
  Serial.print("IP Wifi Configurado?: ");
  Serial.println(configurado);
  Fconfig_wifi.close();

  int iconfig = configurado.toInt();

  if (iconfig == 1) {

    File FipWifi = SPIFFS.open(PATH_IP_WIFI, "r");
    if (!FipWifi) {
      Serial.println("Erro ao abrir arquivo com IP do Wifi!");
    }
    String ipWifi = FipWifi.readStringUntil('\r');
    Serial.print("Ip wifi em Spiffs: ");
    Serial.println(ipWifi);
    FipWifi.close();

    parseIPString(ipWifi, arrayWifiIP[0], arrayWifiIP[1], arrayWifiIP[2], arrayWifiIP[3]);

    ////
    File FgatWifi = SPIFFS.open(PATH_GAT_WIFI, "r");
    if (!FgatWifi) {
      Serial.println("Erro ao abrir arquivo com Gateway do Wifi!");
    }
    String gatWifi = FgatWifi.readStringUntil('\r');
    Serial.print("Gate wifi em Spiffs: ");
    Serial.println(gatWifi);
    FgatWifi.close();

    parseIPString(gatWifi, arrayWifiGatewayIP[0], arrayWifiGatewayIP[1], arrayWifiGatewayIP[2], arrayWifiGatewayIP[3]);

    ////
    File FsubWifi = SPIFFS.open(PATH_SUB_WIFI, "r");
    if (!FsubWifi) {
      Serial.println("Erro ao abrir arquivo com Submascara do Wifi!");
    }
    String subWifi = FsubWifi.readStringUntil('\r');
    Serial.print("Submascara wifi em Spiffs: ");
    Serial.println(subWifi);
    FsubWifi.close();

    parseIPString(subWifi, arrayWifiSubnetIP[0], arrayWifiSubnetIP[1], arrayWifiSubnetIP[2], arrayWifiSubnetIP[3]);

    configurado_wifi_global = 1;
  } else {
    Serial.println("Wifi não configurado com SPIFFS");
    configurado_wifi_global = 0;
    return;
  }
}




/*************** SERVIDOR WEB FUNCOES *************************/
