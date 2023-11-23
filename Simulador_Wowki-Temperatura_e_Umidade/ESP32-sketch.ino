// Bibliotecas necessárias
#include <WiFi.h>
#include <ThingSpeak.h>
#include <DHT.h>
#include <LiquidCrystal_I2C.h>

// Definindo o pino dos sensores, leds e o buzzer
#define dht_pin 5
#define led_verde 16
#define led_amarelo 0
#define led_laranja 15
#define buzzer 13
// Definindo o tipo do DHT
#define DHTTYPE DHT22

// Variáveis utilizadas para guardar as informações do sensor
float temperatura;
float batimentos;

// Configurando conexão
const char* wifi = "Wifi";
const char* senha = "senha";

// Configurando ThingSpeak
unsigned long myChannelNumber = 1 ;
const char * myWriteAPIKey = "LP2ITVBVSUP0MERJ";

// Deifinindo tempo de Atualização
unsigned long lastTime = 0;
unsigned long timerDelay = 30000;

// Instanciando objetos das classes 
WiFiClient client;
DHT dht(dht_pin, DHTTYPE);
LiquidCrystal_I2C lcd(0x27, 16, 2);

// Configurações
void setup() {
    // Inicializa a porta serial
    Serial.begin(115200);
    Serial.print("Conectando ");
    // Inicia conexao Wifi
    WiFi.begin("Wokwi-GUEST", "", 6);
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }

    Serial.println("");
    Serial.print("Conectado a: ");
    Serial.println(wifi);
    Serial.print("Endereço IP: ");
    Serial.println(WiFi.localIP());
    Serial.println("");
    Serial.println("Aguarde 30 segundos para a primeira postagem");

    // Configurando os pinos
    pinMode(led_verde, OUTPUT);
    pinMode(led_amarelo, OUTPUT);
    pinMode(led_laranja, OUTPUT);
    pinMode(buzzer, OUTPUT);

    // Iniciando os objetos das classes
    dht.begin();
    lcd.init();
    WiFi.mode(WIFI_STA);
    ThingSpeak.begin(client);
}

// Função para mostrar a temperatura no display LCD
void mostrar_temp() {
    lcd.setCursor(0, 0);
    lcd.print("Temperatura: ");
    lcd.setCursor(0, 1); 
    lcd.print(temperatura);
    for (int i = 0; i <= 14;  i++) {
    lcd.scrollDisplayRight();
    delay(300);
    }
    lcd.clear();
}

// Função para mostrar os batimentos no display LCD
void mostrar_batimentos() {
    lcd.setCursor(0, 0);
    lcd.print("batimentos: ");
    lcd.setCursor(0, 1);
    lcd.print(batimentos);
    for (int i = 0; i <= 15;  i++) {
        lcd.scrollDisplayRight();
        delay(300);
    }
    cd.clear();
}

// Função para emitir um alerta em casos graves
// Emite um alerta sonoro e visual
void alerta_vermelho(int ledPin) {
    for (int i = 0; i < 5; i++) {
        digitalWrite(ledPin, HIGH);
        delay(1000);
        digitalWrite(ledPin, LOW);
        digitalWrite(buzzer, HIGH);
        delay(1000);
        digitalWrite(buzzer, LOW);
    }
}

// Função para emitir um alerta em casos de atenção
// Emite um alerta visual
void alerta_amarelo(int ledPin) {
    for (int i = 0; i < 5; i++) {
        digitalWrite(ledPin, HIGH);
        delay(1000);
        digitalWrite(ledPin, LOW);
        delay(1000);
    }
}

// Função para passar no display lcd, as 2 informações
void apresenta_info() {
    lcd.backlight();
    mostrar_temp();
    delay(300);
    mostrar_batimentos();
}

// Função principal de execução contínua
void loop() {
    if ((millis() - lastTime) > timerDelay) {

        if(WiFi.status() != WL_CONNECTED){
            // Tentativa de reconectar o WiFi caso se desconecte
            Serial.print("Tentando conectar...");
            while(WiFi.status() != WL_CONNECTED){
                WiFi.begin(wifi, senha);
                delay(2000);
            }
            Serial.println("\nConectado.");
        }

        // Lendo os dados de temperatura e "batimentos" do sensor DHT22
        temperatura = dht.readTemperature();
        batimentos = dht.readHumidity();

        Serial.println("-=-=-=-=-=-=-=--=-=-=-=-=-=");

        // Condição caso temperatura e/ou "batimentos" cardiacos estiverem baixos
        if (temperatura <= 35 || batimentos < 60){ 
            if (temperatura <= 35 && batimentos < 60) {
                apresenta_info();
                // Mostra as informações no terminal
                Serial.print(" *** GRAVE! " );
                Serial.println("Hipotermia e Bradicardia! ***");
                alerta_vermelho(led_laranja);
            } else if (temperatura <= 35) {
                apresenta_info();
                Serial.print(" *** GRAVE! " );
                Serial.println("Hipotermia! ***");
                alerta_vermelho(led_laranja);
            } else if (batimentos < 60) {
                apresenta_info();
                Serial.print(" *** GRAVE!" );
                Serial.println("Bradicardia! ***");
                alerta_vermelho(led_laranja);
            }
            /*
                Dentro de cada condição de temperatura ou batimentos desregulados, o código enviaria um email para 
                um parente próximo e os médicos responsáveis pelo atendimento do paciente.
            */
        } 

        // Condição caso temperatura e/ou "batimentos" cardiacos estiverem altos
        else if (temperatura > 37 || batimentos >= 100) {
            if (temperatura > 37 && batimentos >= 100) {
                apresenta_info();
                Serial.print(" *** GRAVE! " );
                Serial.println("Febre e Taquicardia! ***");
                alerta_vermelho(led_laranja);
            } else if (temperatura > 37) {
                apresenta_info();
                Serial.print(" *** GRAVE! " );
                Serial.println("Febre! ***");
                alerta_vermelho(led_laranja);
            } else if (batimentos >= 100) {
                apresenta_info();
                Serial.print(" *** GRAVE! " );
                Serial.println("Taquicardia! ***");
                alerta_vermelho(led_laranja);
            }
            /*
                Dentro de cada condição de temperatura ou batimentos desregulados, o código enviaria um email para 
                um parente próximo e os médicos responsáveis pelo atendimento do paciente.
            */
        }

        // Condição caso temperatura e/ou "batimentos" cardiacos estiverem normais
        else if (temperatura <= 37 && batimentos >= 60 || batimentos < 100) {
            apresenta_info();
            Serial.println(" --- Medidas normais. ---");
            digitalWrite(led_verde, HIGH);
            /*
                Como nessa condição os dados estão normais não é emitido nenhum alerta. 
                A luz verde é acesa indicando que esta tudo bem até a próxima aferição.
            */
        }

        // Seta os campos configurados no ThingSpeak
        // 1 campo - Dashboard de temperatura
        ThingSpeak.setField(1, temperatura);
        // 2 campo - Dashboard de batimentos
        ThingSpeak.setField(2, batimentos);

        // Envia os campos para o ThingSpeak
        int envio = ThingSpeak.writeFields(myChannelNumber, myWriteAPIKey);

        // Caso seja enviado os campos com sucesso
        if(envio == 200){
            Serial.print("Temperatura: ");
            Serial.println(temperatura);
            Serial.print("Batimentos: ");
            Serial.print(batimentos);
            Serial.println("");
            Serial.println("Dashboards atualizados");
        }
        // Caso de algum erro ao enviar os campos
        else{
            Serial.println("Problema na atualização do canal. Código do erro HTTP" + String(envio));
            
        }

        Serial.println("-=-=-=-=-=-=-=--=-=-=-=-=-=");

        // Reinicia as variaveis e pinos necessários
        lastTime = millis();
        digitalWrite(led_verde, LOW);
        digitalWrite(led_amarelo, LOW);
        digitalWrite(led_laranja, LOW);
        // Apaga a tela do lcd
        lcd.noBacklight();

    }
}
