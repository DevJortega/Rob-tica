// Pines para los sensores de ultrasonido
const int triggerPinLeft = 11;   // Pin de disparo del sensor izquierdo
const int echoPinLeft = 10;      // Pin de eco del sensor izquierdo
const int triggerPinFront = 6;   // Pin de disparo del sensor frontal
const int echoPinFront = 5;      // Pin de eco del sensor frontal

// Constante de velocidad del sonido en cm/us
const float velocidadSonido = 0.034; 

// Variables para almacenar las distancias medidas por los sensores
float distanciaIzquierda;
float distanciaFrontal;

// Pines de control para el motor izquierdo
const int pinMotorIzquierdoAdelante = 2; 
const int pinMotorIzquierdoAtras = 4;  

// Pines de control para el motor derecho
const int pinMotorDerechoAdelante = 8; 
const int pinMotorDerechoAtras = 7;  

// Pines PWM para control de velocidad de los motores
const int pinHabilitarMotorIzquierdo = 3;
const int pinHabilitarMotorDerecho = 9;

// Definición de velocidades de los motores
const int velocidadMaxima = 255;
const int velocidadMedia = 128;
const int velocidadBaja = 64;

// Definir las distancias de seguridad
const float distanciaSegura = 20.0;    // Distancia segura en cm
const float distanciaCercana = 12.0;   // Distancia considerada cercana
const float distanciaMuyCercana = 10.0; // Demasiado cerca para evitar colisión

void setup() {
    // Configurar pines de los sensores de ultrasonido
    pinMode(triggerPinLeft, OUTPUT);
    pinMode(echoPinLeft, INPUT);
    pinMode(triggerPinFront, OUTPUT);
    pinMode(echoPinFront, INPUT);

    // Configurar pines de los motores como salidas
    pinMode(pinMotorIzquierdoAdelante, OUTPUT);
    pinMode(pinMotorIzquierdoAtras, OUTPUT);
    pinMode(pinMotorDerechoAdelante, OUTPUT);
    pinMode(pinMotorDerechoAtras, OUTPUT);

    // Configurar pines de habilitación como salidas
    pinMode(pinHabilitarMotorIzquierdo, OUTPUT);
    pinMode(pinHabilitarMotorDerecho, OUTPUT);

    // Detener los motores al inicio
    digitalWrite(pinHabilitarMotorIzquierdo, LOW);
    digitalWrite(pinHabilitarMotorDerecho, LOW);

    // Iniciar comunicación serial para monitoreo
    Serial.begin(9600);
}

void loop() {
    // Obtener las mediciones de distancia
    distanciaIzquierda = medirDistancia(triggerPinLeft, echoPinLeft);
    distanciaFrontal = medirDistancia(triggerPinFront, echoPinFront);

    // Mostrar las distancias en el monitor serial
    Serial.print("Distancia Izquierda: ");
    Serial.print(distanciaIzquierda);
    Serial.println(" cm");
    
    Serial.print("Distancia Frontal: ");
    Serial.print(distanciaFrontal);
    Serial.println(" cm");

    // Evaluar las distancias y tomar decisiones de movimiento
    evaluarMovimiento(distanciaIzquierda, distanciaFrontal);

    delay(200);
}

// Función para medir la distancia con un sensor de ultrasonido
float medirDistancia(int pinTrigger, int pinEcho) {
    digitalWrite(pinTrigger, LOW);
    delayMicroseconds(2);
    digitalWrite(pinTrigger, HIGH);
    delayMicroseconds(10);
    digitalWrite(pinTrigger, LOW);
    
    // Medir el tiempo que tarda el eco
    float duracion = pulseIn(pinEcho, HIGH);
    
    // Calcular la distancia en cm
    return duracion * velocidadSonido / 2;
}

// Función para decidir el movimiento según las distancias
void evaluarMovimiento(float distanciaIzquierda, float distanciaFrontal) {
    if (distanciaFrontal < distanciaCercana) {
        Serial.println("Obstáculo cerca al frente, girando a la derecha.");
        girarDerecha();
    } else if (distanciaIzquierda < distanciaMuyCercana) {
        Serial.println("Obstáculo muy cerca a la izquierda, corrigiendo hacia la derecha.");
        ajustarDerecha();
    } else if (distanciaIzquierda > distanciaSegura) {
        Serial.println("Lejos de la pared a la izquierda, corrigiendo hacia la izquierda.");
        ajustarIzquierda();
    } else {
        Serial.println("Distancia segura, avanzando.");
        avanzar();
    }
}

// Función para avanzar
void avanzar() {
    digitalWrite(pinMotorIzquierdoAdelante, HIGH);
    digitalWrite(pinMotorIzquierdoAtras, LOW);
    digitalWrite(pinMotorDerechoAdelante, HIGH);
    digitalWrite(pinMotorDerechoAtras, LOW);
    analogWrite(pinHabilitarMotorIzquierdo, velocidadBaja);
    analogWrite(pinHabilitarMotorDerecho, velocidadBaja);
    Serial.println("Avanzando.");
}

// Función para girar a la derecha
void girarDerecha() {
    digitalWrite(pinMotorIzquierdoAdelante, HIGH);
    digitalWrite(pinMotorIzquierdoAtras, LOW);
    digitalWrite(pinMotorDerechoAdelante, LOW);
    digitalWrite(pinMotorDerechoAtras, HIGH);
    analogWrite(pinHabilitarMotorIzquierdo, velocidadMedia);
    analogWrite(pinHabilitarMotorDerecho, velocidadMedia);
    delay(500);  // Girar durante 500 ms
    detenerMotores();
}

// Función para ajustar hacia la derecha
void ajustarDerecha() {
    digitalWrite(pinMotorIzquierdoAdelante, HIGH);
    digitalWrite(pinMotorIzquierdoAtras, LOW);
    digitalWrite(pinMotorDerechoAdelante, LOW);
    digitalWrite(pinMotorDerechoAtras, HIGH);
    analogWrite(pinHabilitarMotorIzquierdo, velocidadBaja);
    analogWrite(pinHabilitarMotorDerecho, velocidadBaja);
    delay(300);  // Ajustar durante 300 ms
    avanzar();
}

// Función para ajustar hacia la izquierda
void ajustarIzquierda() {
    digitalWrite(pinMotorIzquierdoAdelante, LOW);
    digitalWrite(pinMotorIzquierdoAtras, HIGH);
    digitalWrite(pinMotorDerechoAdelante, HIGH);
    digitalWrite(pinMotorDerechoAtras, LOW);
    analogWrite(pinHabilitarMotorIzquierdo, velocidadBaja);
    analogWrite(pinHabilitarMotorDerecho, velocidadBaja);
    delay(300);  // Ajustar durante 300 ms
    avanzar();
}

// Función para detener los motores
void detenerMotores() {
    digitalWrite(pinMotorIzquierdoAdelante, LOW);
    digitalWrite(pinMotorIzquierdoAtras, LOW);
    digitalWrite(pinMotorDerechoAdelante, LOW);
    digitalWrite(pinMotorDerechoAtras, LOW);
    analogWrite(pinHabilitarMotorIzquierdo, 0);
    analogWrite(pinHabilitarMotorDerecho, 0);
    Serial.println("Motores detenidos.");
}
