#include <WiFi.h>
#include <ESP_Mail_Client.h>

#define WIFI_SSID "wifi"
#define WIFI_PASSWORD "senha_wifi"

#define SMTP_server "smtp.gmail.com"
#define SMTP_Port 467

#define sender_email "xxxxxxx@gmail.com"
#define sender_password "sender_password"

#define Recipient_email "Recipient_email"

SMTPSession smtp;

void setup(){
  Serial.begin(115200);
  Serial.println();
  Serial.print("Conectando...");
  WiFi.begin("Wokwi-GUEST", "", 6);
  while (WiFi.status() != WL_CONNECTED){
    Serial.print(".");
    delay(200);
  }
  Serial.println("");
  Serial.println("WiFi conectado.");
  Serial.println("Endereço IP: ");
  Serial.println(WiFi.localIP());
  Serial.println();
  smtp.debug(1);

  ESP_Mail_Session session;
  session.server.host_name = SMTP_server ;
  session.server.port = SMTP_Port;
  session.login.email = sender_email;
  session.login.password = sender_password;
  session.login.user_domain = "";

  SMTP_Message message;
  message.sender.name = "Paciente";
  message.sender.email = sender_email;
  message.subject = "Paciente em estravo grave!";
  message.addRecipient("Médico",Recipient_email);

  const char * textMsg = "<!DOCTYPE html>\n"
                          "<html>\n"
                          "<body>\n"
                          "  <h1>Paciente com xx,xxºC e yyy,yy batimentos por minuto em repouso.</h1>\n"
                          "  <p>Olá,</p>\n"
                          "  <p>Paciente XXXXXXXXX, está com os seguintes e sintomas, ...., e precisa de um atendimento urgente.</p>\n"
                          "  <p>Atenciosamente,</p>\n"
                          "  <p></p>\n"
                          "</body>\n"
                          "</html>\n";
  message.text.content = textMsg;
  message.text.charSet = "utf-8";
  message.text.transfer_encoding = Content_Transfer_Encoding::enc_7bit;

  if (!smtp.connect(&session))
    return;

  if (!MailClient.sendMail(&smtp, &message))
    Serial.println("Erro ao enviar email, " + smtp.errorReason());

}

void loop(){

}
