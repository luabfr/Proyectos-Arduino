/*  --- 16/11/17 - 20:18
 *  --- Version Final
 *  --- Creemos que funciona perfecto
 * 
 * 
 *  in This:
 *    - Embebiendo Cebado
 *    
 *  Funciona OK:
 *    - Funciones de encendido, Emergencia, unificadas.
 *    - Funcion Seleccion de Funciones
 *    - Funcion Maquina Encendida
 *    - Funcion Encendido/Apagado
 *    - Funcion Produccion
 *    
 * 
 *  Anotaciones:
 *   - 
 *    
 */

/* ------------------ FUNCIONES -----*/
int standby = -1 ;
int encendido = 0 ;
int produccion = 2 ;
int funcion_seleccionada = standby ;

//prende valvula C, motor electrico, led 12v, y habilita el cebado.

boolean estado_MOTOR_Y_VALVULA_PRINCIPAL = false ; 
/* ------------------------------------------------ PINES -------- */
int pin_pedal = 6;

int pin_boton_encendido = 7 ;
int pin_boton_produccion = 10 ; /*No se porque, el PIN 9 no me anda*/


int pin_relay_valvula_A = 14 ;
int pin_relay_valvula_B = 15;
int pin_relay_valvula_C = 16 ;
int pin_relay_motor_A = 17 ;
int pin_relay_motor_B = 18 ;
int pin_relay_led_12v = 19 ;

int pin_hall_BOT = A0 ;
int pin_hall_TOP = A1 ;

int pin_sensor_pico_TOP = 47;
int pin_sensor_pico_BOT = 46;


/*------- Pedal -------------------*/


/* Var de botones de Funcion ------*/
int estado_boton_encendido = 0 ;
int estado_boton_encendido_anterior = 0 ;
int estado_boton_produccion = 0 ;
int estado_boton_produccion_anterior = 0 ;


/*-------------------- PEDAL ---------*/
int estado_pedal = 0;
int estado_pedal_anterior = 0;


/*-------------------- Valvula A ----- */
boolean estado_valvula_A = false ;
/*....... Sensores Hall....... */
int valor_hall_BOT = 0;
int valor_hall_TOP = 0;


/* -------------------- Valvula B ---- */
boolean estado_valvula_B = false ;
/*--- Sensores DMRC -----------*/
int estado_sensor_pico_TOP = 0;
int estado_sensor_pico_BOT = 0;


/*------------- Var de PRODUCCION -------*/
boolean descargando = false ; /*  esta creemos q no se usa */

boolean pedido_de_ciclo_de_descarga = false;
boolean pico_en_TOP = false ;
boolean pico_en_BOT = false ;
boolean bajar_pico = false ;     
boolean sensor_listo_para_descargar = false;
boolean escupir_para_adelante = false ;
boolean girar_servo = false ;




// CAMBIO DE ESTADO
int buttonPushCounter = 0;   // counter for the number of button presses




void setup() {
  Serial.begin( 9600 );

  /* -- Botones de Seleccion de Funcion ---------*/
  pinMode( pin_boton_encendido , INPUT );
  pinMode( pin_boton_produccion , INPUT );
  /* -- Pedal -----------------------------*/
  pinMode( pin_pedal , INPUT );
  /* -- Valvula A & Sensores HALL ---------*/
  pinMode( pin_hall_BOT , INPUT );
  pinMode( pin_hall_TOP , INPUT );
  /* -- Valvula B & Sensor DMRC -----------*/ 
  pinMode( pin_sensor_pico_TOP , INPUT );
  pinMode( pin_sensor_pico_BOT , INPUT );

  pinMode( pin_relay_valvula_A , OUTPUT );
  pinMode( pin_relay_valvula_B , OUTPUT );
  pinMode( pin_relay_valvula_C , OUTPUT );
  pinMode( pin_relay_motor_A , OUTPUT );
  pinMode( pin_relay_motor_B , OUTPUT );
  pinMode( pin_relay_led_12v , OUTPUT );

  Serial.println( "-------------------------- SETUP :: OK ----------------------------" );
  
}
void loop() {

  f_lectura_botones();
  f_prender_maquina(  );

  
  if ( funcion_seleccionada == 2 ) {
    f_produccion_NUEVA();
  }else{
    f_cebado();
  }

 f_control_de_relays();


  Serial.print( "FRONT: " );
  Serial.print( escupir_para_adelante );
  Serial.print(" | ");
}





void f_lectura_botones(){
  /*---------------------------------- Boton encendido ---------------------------*/
  estado_boton_encendido = digitalRead( pin_boton_encendido );
  if ( estado_boton_encendido != estado_boton_encendido_anterior ) {
    if ( estado_boton_encendido ) {
      Serial.println(" ------------------------------ Boton encendido :: ACTIVADO ");
      buttonPushCounter++;
      funcion_seleccionada = encendido ;

    }
    delay(50);
  }
  estado_boton_encendido_anterior = estado_boton_encendido ;
  if (buttonPushCounter % 2 == 0) {
    estado_MOTOR_Y_VALVULA_PRINCIPAL = false ;
  } else {
    estado_MOTOR_Y_VALVULA_PRINCIPAL = true ;
  }
  
  
    /*---------------------------------- Boton Produccion ---------------------------*/
  estado_boton_produccion = digitalRead( pin_boton_produccion );
  
  if ( estado_boton_produccion != estado_boton_produccion_anterior ) {
    if ( estado_boton_produccion == HIGH) {
      Serial.println(" ------------------------------Boton PRODUCCION :: ACTIVADO ");
      funcion_seleccionada = produccion ;
    }
    delay(50);
  }
  estado_boton_produccion_anterior = estado_boton_produccion ;
  
  
}
void f_prender_maquina(  ){
  /*  EL MODULO DE RELAY POR DEFECTO COMIENZAN PRENDIDOS, ES SOLO POR EL SHIELD LA NEGACION  */
  digitalWrite( pin_relay_valvula_C , !estado_MOTOR_Y_VALVULA_PRINCIPAL );
  digitalWrite( pin_relay_motor_A   , !estado_MOTOR_Y_VALVULA_PRINCIPAL );
  digitalWrite( pin_relay_motor_B   , !estado_MOTOR_Y_VALVULA_PRINCIPAL );
  digitalWrite( pin_relay_led_12v   , !estado_MOTOR_Y_VALVULA_PRINCIPAL );
}
void f_leer_sensores_y_pedal(){/* -------------------------------------------- Lectura Sensores y Pedal | PRODUCCION ----*/

  /*---- Pedal  ---------------------------*/
  estado_pedal = digitalRead( pin_pedal );

  if ( estado_pedal != estado_pedal_anterior ) {
    if ( estado_pedal == HIGH) {
      pedido_de_ciclo_de_descarga = true ;
      Serial.print ("Pedal: ON  ");
    }
    delay(50);
  }else{
      Serial.print ("Pedal: OFF ");
  }
  Serial.print(" | ");
  estado_pedal_anterior = estado_pedal ;

  /*---- Sensores HALL ---------------------------*/
  valor_hall_BOT = analogRead( pin_hall_BOT );
  valor_hall_TOP = analogRead( pin_hall_TOP );
  

  Serial.print(" Hall BOT: ");
  if( valor_hall_BOT > 900){
    Serial.print("1");
    sensor_listo_para_descargar = true ;
  }else{
    Serial.print("0");
  }
  Serial.print("   Hall TOP: ");
  if( valor_hall_TOP > 900){
    Serial.print("1");
    sensor_listo_para_descargar = false ;
  }else{
    Serial.print("0");
  }
  Serial.print("   | ");

  /*---- Sensores PICO ---------------------------*/
  estado_sensor_pico_BOT = digitalRead( pin_sensor_pico_BOT );
  estado_sensor_pico_TOP = digitalRead( pin_sensor_pico_TOP );
  
  if ( estado_sensor_pico_BOT == HIGH ) {
    pico_en_BOT = true ;
    Serial.print( " Sen BOT: 1   " );
  } else {
    pico_en_BOT = false ;
    Serial.print( " Sen BOT: 0   " );
  }
  if ( estado_sensor_pico_TOP == HIGH ) {
    pico_en_TOP = true ;
    Serial.print( "Sen TOP: 1" );
  } else {
    pico_en_TOP = false ;
    Serial.print( "Sen TOP: 0" );
  }
  Serial.print("   | ");

  
  
}
void f_girar_servo(){
  Serial.println("Servo GIRANDO");
}
void f_control_de_relays(){

  if( bajar_pico ){
    Serial.print("PICO: Bajando ");
    digitalWrite( pin_relay_valvula_B , LOW );
  }else{
    Serial.print("PICO: Subiendo  ");
    digitalWrite( pin_relay_valvula_B , HIGH);
  }
  
  if( escupir_para_adelante ){
    Serial.println("Escupe FRONT");
    digitalWrite( pin_relay_valvula_A , LOW );
  }else{
    Serial.println("Escupe BACK  ");
    digitalWrite( pin_relay_valvula_A , HIGH );
  }
  
}




void f_cebado() {
  f_leer_sensores_y_pedal();
  
  if ( digitalRead( pin_pedal ) == HIGH ) {
    Serial.print("PEDAL : ON   ");
    if( sensor_listo_para_descargar ){
      Serial.print( "TOP ");
      escupir_para_adelante = true ;
    }else{
      Serial.print( "BOT ");
      escupir_para_adelante = false ; 
    }
    
  } else {
    escupir_para_adelante = false ; 
    Serial.print("PEDAL : OFF   ");
  }

}





void f_produccion_NUEVA(){
  
  f_leer_sensores_y_pedal();

  f_control_y_condiciones_para_la_descarga();
  
  Serial.print("PCD: ");
  Serial.print( pedido_de_ciclo_de_descarga );
  Serial.print("   | ");
  
  
}
void f_control_y_condiciones_para_la_descarga(){
  if( pedido_de_ciclo_de_descarga ){
    bajar_pico = true ; //esto controla la valvula B
    
  }
  if( pico_en_BOT && sensor_listo_para_descargar ){
    pedido_de_ciclo_de_descarga = false ;
    
    bajar_pico = false;               // Valvula B: Pico comienza a subir.
    escupir_para_adelante = true ;    // Valvula A: Escupe para adelante.
    girar_servo = true ;              // Servo: comienza a girar.
    
  }
    
  if( sensor_listo_para_descargar == false ){
    escupir_para_adelante = false ;   //valvula A deshabilitar 
  }

  
}
