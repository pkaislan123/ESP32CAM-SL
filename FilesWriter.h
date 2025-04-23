void writeFile(String valor, String path) {
  Serial.print("Valor: ");
  Serial.println(valor);
  Serial.print("Path: ");
  Serial.println(path);

  //Abre o arquivo para escrita ("w" write)
  //Sobreescreve o conteúdo do arquivo
  File rFile = SPIFFS.open(path, "w+");
  if (!rFile) {
    Serial.println("Erro ao abrir arquivo!");
  } else {
    rFile.println(valor);
    Serial.print("gravou estado: ");
    Serial.println(valor);
  }
  rFile.close();
}