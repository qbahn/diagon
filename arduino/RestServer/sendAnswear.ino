void sendAnswear(String answear, WifiData client) {
  client.println("HTTP/1.1 200 OK\n");
  client.print(answear);
  client.print(EOL);
  return;
}
