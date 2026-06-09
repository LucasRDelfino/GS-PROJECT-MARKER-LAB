/*
 * ============================================================
 *  BRAÇO ROBÓTICO DE COLETA DE AMOSTRAS
 *  Docking & Retrieval - Ambiente de Microgravidade
 * ============================================================
 *  Hardware:
 *    - Arduino UNO
 *    - Servo 1 (Braço/Elevação)  → Pino 7
 *    - Servo 2 (Garra/Abertura)  → Pino 8
 *    - LED de Status             → Pino 13
 *    - Fonte de bancada: 5V ou 6V (para os servos)
 *
 *  Comandos via Monitor Serial:
 *    U → Up    (Braço SOBE)
 *    D → Down  (Braço DESCE)
 *    O → Open  (Garra ABRE)
 *    C → Close (Garra FECHA)
 *
 *  LED de Status:
 *    - Piscando → Sistema em repouso (idle)
 *    - Aceso fixo → Comando sendo executado
 * ============================================================
 */

#include <Servo.h>

// ── Pinos ─────────────────────────────────────────────────────
const int PINO_SERVO_BRACO = 7;
const int PINO_SERVO_GARRA = 8;
const int PINO_LED         = 13;

// ── Ângulos ───────────────────────────────────────────────────
const int BRACO_BAIXO    = 0;
const int BRACO_CIMA     = 90;
const int GARRA_FECHADA  = 90;
const int GARRA_ABERTA   = 180;

// ── Objetos Servo ─────────────────────────────────────────────
Servo servoBraco;
Servo servoGarra;

// ── Estado atual ──────────────────────────────────────────────
int  anguloBraco = BRACO_BAIXO;
int  anguloGarra = GARRA_FECHADA;

// ── Controle do LED idle (sem delay) ──────────────────────────
unsigned long ultimoPisque   = 0;
const long    intervaloPisque = 800;
bool          estadoLed      = false;
bool          executando     = false;

// ─────────────────────────────────────────────────────────────
void setup() {
  Serial.begin(9600);

  servoBraco.attach(PINO_SERVO_BRACO);
  servoGarra.attach(PINO_SERVO_GARRA);
  pinMode(PINO_LED, OUTPUT);

  // Posição inicial
  servoBraco.write(anguloBraco);
  servoGarra.write(anguloGarra);

  Serial.println(F("============================================"));
  Serial.println(F("  BRACO ROBOTICO - Docking & Retrieval"));
  Serial.println(F("============================================"));
  Serial.println(F("  U = Up    | D = Down"));
  Serial.println(F("  O = Open  | C = Close"));
  Serial.println(F("============================================"));
  Serial.println(F("Aguardando comando..."));
}

// ─────────────────────────────────────────────────────────────
void loop() {
  // Lê comando do Serial
  if (Serial.available() > 0) {
    char cmd = Serial.read();

    // Ignora quebras de linha
    if (cmd == '\n' || cmd == '\r') return;

    // Aceita minúsculas também
    cmd = toupper(cmd);

    processarComando(cmd);
  }

  // Pisca LED enquanto idle
  if (!executando) {
    piscarLedIdle();
  }
}

// ─────────────────────────────────────────────────────────────
// Move o servo do BRAÇO suavemente
// ─────────────────────────────────────────────────────────────
void moverBraco(int origem, int destino) {
  if (origem < destino) {
    for (int pos = origem; pos <= destino; pos++) {
      servoBraco.write(pos);
      delay(15);
    }
  } else {
    for (int pos = origem; pos >= destino; pos--) {
      servoBraco.write(pos);
      delay(15);
    }
  }
}

// ─────────────────────────────────────────────────────────────
// Move o servo da GARRA suavemente
// ─────────────────────────────────────────────────────────────
void moverGarra(int origem, int destino) {
  if (origem < destino) {
    for (int pos = origem; pos <= destino; pos++) {
      servoGarra.write(pos);
      delay(15);
    }
  } else {
    for (int pos = origem; pos >= destino; pos--) {
      servoGarra.write(pos);
      delay(15);
    }
  }
}

// ─────────────────────────────────────────────────────────────
// Processa o comando recebido via Serial
// ─────────────────────────────────────────────────────────────
void processarComando(char cmd) {
  executando = true;
  digitalWrite(PINO_LED, HIGH); // LED fixo durante execução

  switch (cmd) {

    case 'U': // Braço sobe
      Serial.println(F("[U] Braco SUBINDO..."));
      moverBraco(anguloBraco, BRACO_CIMA);
      anguloBraco = BRACO_CIMA;
      Serial.print(F("    Angulo do braco: "));
      Serial.print(anguloBraco);
      Serial.println(F(" graus"));
      break;

    case 'D': // Braço desce
      Serial.println(F("[D] Braco DESCENDO..."));
      moverBraco(anguloBraco, BRACO_BAIXO);
      anguloBraco = BRACO_BAIXO;
      Serial.print(F("    Angulo do braco: "));
      Serial.print(anguloBraco);
      Serial.println(F(" graus"));
      break;

    case 'O': // Garra abre
      Serial.println(F("[O] Garra ABRINDO..."));
      moverGarra(anguloGarra, GARRA_ABERTA);
      anguloGarra = GARRA_ABERTA;
      Serial.print(F("    Angulo da garra: "));
      Serial.print(anguloGarra);
      Serial.println(F(" graus"));
      break;

    case 'C': // Garra fecha — captura amostra
      Serial.println(F("[C] Garra FECHANDO... Capturando amostra!"));
      moverGarra(anguloGarra, GARRA_FECHADA);
      anguloGarra = GARRA_FECHADA;
      Serial.print(F("    Angulo da garra: "));
      Serial.print(anguloGarra);
      Serial.println(F(" graus"));
      break;

    default:
      Serial.print(F("[!] Comando invalido: '"));
      Serial.print(cmd);
      Serial.println(F("'. Use U, D, O ou C."));
      break;
  }

  Serial.println(F("Aguardando proximo comando..."));
  executando = false;
}

// ─────────────────────────────────────────────────────────────
// Pisca o LED em idle usando millis() (sem travar o loop)
// ─────────────────────────────────────────────────────────────
void piscarLedIdle() {
  unsigned long agora = millis();
  if (agora - ultimoPisque >= intervaloPisque) {
    ultimoPisque = agora;
    estadoLed = !estadoLed;
    digitalWrite(PINO_LED, estadoLed);
  }
}
