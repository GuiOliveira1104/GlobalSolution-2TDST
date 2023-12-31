# GlobalSolution-2TDST
Integrantes:

GUILHERME HENRIQUE MELO DE OLIVEIRA – RM: 95184

GUILHERME LUCAS ARTIGIANI – RM: 94322

VINICIUS GONÇALVES CARNEIRO – RM: 94154

# Ideia do projeto
Utilizamos o sensor DHT22 apenas para simulação, pois para desenvolvimento do projeto utilizamos o simulador Wowki, e o mesmo possui sensores limitados.

Nossa ideia era utilizar 2 sensores, TMP36 que pode ser usado ​​para medir a temperatura corporal, e o MAX30100 para detectar alterações no fluxo sanguíneo e nos batimentos cardíacos.

# Sistema de Monitoramento de Saúde com ESP32 e ThingSpeak

Este projeto utiliza um ESP32, com o sensor DHT22 para monitorar temperatura e umidade, e envia os dados para o ThingSpeak para visualização e monitoramento remoto.
Simulando a temperatura corporal e os batimentos cardíacos.

## Componentes Utilizados

- ESP32
- Sensor de temperatura e umidade DHT22
- Display LCD I2C
- LEDs e buzzer para alertas visuais e sonoros

## Configuração

### Bibliotecas Necessárias

- WiFi.h
- ThingSpeak.h
- DHT.h
- LiquidCrystal_I2C.h

### Pinos

- `dht_pin` - Pino conectado ao sensor DHT22
- `led_verde`, `led_amarelo`, `led_laranja` - Pinos para LEDs indicadores
- `buzzer` - Pino para o buzzer

## Funcionamento

- O código configura a conexão WiFi, inicializa sensores e LEDs.
- Realiza leituras de temperatura e batimentos a cada intervalo de tempo.
- Verifica se a temperatura ou batimentos estão alterados.
- Emite alertas visuais e sonoros com base nos limites definidos de temperatura e batimentos cardíacos.
- Alerta vermelho: responsável por emitir um alarme sonoro, e piscar a luz vermelha.
- Alerta amarelo: responsável por piscar a luz amarela, sem sinal sonoro.
- Medidas normais: aparece os dados no display lcd e no terminal, e acende uma luz verde, sem emitir sinais para o usuario.
- (Em casos mais graves ou extremos, é enviado um email para parentes ou médicos responsáveis (DEV))
- Envia os dados para o ThingSpeak para monitoramento remoto.

## Configuração Inicial

- Substitua as credenciais da rede WiFi (`wifi` e `senha`).
- Defina o número do canal ThingSpeak (`myChannelNumber`) e a chave de escrita (`myWriteAPIKey`).

## Execução

- Carregue o código para o ESP32 usando a IDE Arduino.
- Copie o "diagram.json", para montar o modelo.
- Abra o monitor serial para visualizar informações detalhadas.
- Monitore o ThingSpeak para ver os dados enviados, em um Dahboard personalizado.

## Observações

- Certifique-se de que as bibliotecas necessárias estão instaladas corretamente na sua IDE Arduino.
- Ajuste os limites de alerta conforme necessário para atender aos requisitos de monitoramento.


### Link para Wowki
 - https://wokwi.com/projects/382057105542785025
### Link para ThingSpeak
 - https://thingspeak.com/channels/2355139
