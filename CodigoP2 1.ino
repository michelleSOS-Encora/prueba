int enA = 9;       // Pin PWM para controlar velocidad
int in1 = 11;      // Pin para controlar la dirección del motor
int in2 = 10;      // Pin para controlar la dirección del motor
int potPin = A0;   // Pin donde está conectado el potenciómetro
int posDeseada;    // Variable para la posición deseada
int posActual;     // Variable para la posición actual del potenciómetro
int valor;

void setup() {
  pinMode(enA, OUTPUT);
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  pinMode(A0, INPUT);
  Serial.begin(9600);  
}

void loop() {
  posActual = analogRead(potPin); // Leer la posición actual desde el potenciómetro (0 a 1023)
  if (Serial.available() > 0) { //Grados maximos aceptados: -135 A 135
    Serial.print("Ingresa el angulo: ");
    Serial.flush();
    valor = Serial.parseInt();  
    Serial.flush();  // Limpiar el buffer serial después de leer el valor
    Serial.println(valor);
    if(valor != 0){ //Marca valor basura despues de ingresar numero, por lo cual se quita
    int gradosActuales = map(posActual, 0, 1023, -180, 180);
    int gradosDeseados = gradosActuales + valor;// Calcular el ángulo deseado en base al ángulo actual y el valor ingresado

    // Limitar el ángulo deseado entre -180 y 180 grados
    if (valor > 135) {
      Serial.println("Se paso");
      gradosDeseados = 180;
    } else if (valor < -135) {
      Serial.println("Se paso");
      gradosDeseados = -180;
    }

    // Mapear el ángulo deseado de -180 a 180 grados al rango del potenciómetro (0-1023)
    int posDeseada = map(gradosDeseados, -180, 180, 0, 1023);

    // Calcular la diferencia entre la posición actual y la posición deseada, pero se divide entre 2 porque el maximo que se puede mover son 135 de los 270 disponibles
    int diferencia = (posDeseada - posActual)/2; //Diferencia es aproximadamente 190 como maximo 
    
    if (diferencia > 7) {  
      digitalWrite(in1, HIGH);  // Girar en dirección "derecha"
      digitalWrite(in2, LOW);
      int correcion = map(diferencia, 0, 135, 0,33); //En base a los 135 maximos que se puede obtener
      diferencia -= correcion;
      int velocidad = map(diferencia, 0, 180, 0, 255);  // Como el valor inicial estaba calculado en este rango, se sigue usando este
      //Todavia puede girar mas pero se limita a que el maximo sea 147 que mapea a 205 aproximadamente para el PWM
      analogWrite(enA, velocidad);  //Salida a PWM
    } else if (diferencia < -7) {  // Si la posición deseada está a la izquierda
      digitalWrite(in1, LOW);  // Girar en dirección "izquierda"
      digitalWrite(in2, HIGH);
      diferencia = diferencia * -1; //Ya no importa el signo negativo
      int correcion = map(diferencia, 0, 135, 0,34);
      diferencia -= correcion;
      int velocidad = map(diferencia, 0, 180, 0, 255); 
      analogWrite(enA, velocidad);  
    } else {  // Si el error es menor que 7, detener el motor
      analogWrite(enA, 0);
      Serial.println("Error muy petit, por lo que no se mueve");
    } 
    }
  } else {
    analogWrite(enA, 0); // Si no se ingresa número, detener el motor
  }
}