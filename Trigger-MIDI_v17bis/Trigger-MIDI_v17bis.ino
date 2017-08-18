/*
    ********* IMPORTANTE *************
      si se manda MIDI por   SERIAL
       APAGAR _ mensajes SERIALES
         de Consola de ARDUINO
    **********************************
    *
    *        
    *        
    *        
    *        
    *        
  ------------------------------------------------------------------------------------------

   IN THIS: - DEBUGGUEANDO
      - El rango de los valores de Parametro es de 0 a 99 por lo sig:
        -- 0 a 99 son los valores del Display,
        -- La memoria EEPROM solo guarda datos de 0 a 255
      - BUG con el numero de NOTA MIDI. El display muestra algo distinto a la NOTA que configura
        (RESULETO a MEDIAS) Los valores_en_array[][] pasan todos por un MAPEO para la visualizacion. Ese MAP no es necesario para NUMERO de NOTA
        Puede que la carga en el SETUP este tirando numero malos.

  ------------------------------------------------------------------------------------------

   UPGRADE Posibles:
          - Agregar Pedal EXP.
          - Agregar Footswitch (tipo sustain de piano).
          - la 'CURVA de SENS' esta funcionando con valores INT de entre -5 y 5. Tendria que entregar valores FLOAT para mayor precision.
          - hacer que el limite MIN de la SENS sea el THRES.

   ANOTACIONES:
   Se guardar en un Array[][] doble
      - 1er [espacio][] del array :  nº de cada piezo.
      - 2do [][espacio] del array :  en cada indice, el valor de un parámetro diferente.

        Para modificar Cantidad_de_PIEZOS o PARAMETROS:
                      0- Si se agregan PIEZOS y la VAR 'leer_valor_del_condicional' (que sirve para cargar datos por primera vez) 
                         ya fue cambiada, osea es != 99; va a ser necesario cargar todos los valores a mano del nuevo piezo usando 
                         los botones, los encoders, y el display.          <--- Arreglar esto en el futuro
                      1- Modificar VAR 'cantidad_de_piezos' o VAR 'cantidad_de_parametros'.
                      2- Tener en cuenta ::   -    Ciclo FOR que asigna valores iniciales en Void Setup.
                                              -   'Switch-Case' de la FUNCION 'f_valores_en_display'.
                                              -    Ciclo FOR que recorre el Array de Piezos en la FUNCION f_lectura_de_piezos.
                                              -    el MAP - que se hace en el Void Loop - de la lectura del POTE.
                                              -    los 2 IF(parametro == SAVE) de la funcion f_lectura_botones_y_potes()

   PARAMETROS:
    1- Umbral               valores_en_array[ i ][ 0 ]
    2- Sensibilidad TOPE    valores_en_array[ i ][ 1 ]
    3- Mask TIME            valores_en_array[ i ][ 2 ]
    4- CUVA Sens            valores_en_array[ i ][ 3 ]
    5- Nota MIDI #          valores_en_array[ i ][ 4 ]
    6- DECAY                valores_en_array[ i ][ 5 ]
    7- SAVE                 valores_en_array[ i ][ 6 ]

        Mask time    = entre 0 y 495 millis.
        Decay        = entre 0 y 495 millis.
        Sensibilidad = tope MAX de lectura.
                      ** Si por ej: la SENS es 650, cualquier lectura del PIEZO por encima de 650 dará 127.
                      ** Si la SENS es menor al UMBRAL ==> todo lo ke supera umbral = ¿0? o ¿127?.



*/

//__MIDI_____________________
#include <MIDI.h>
#include <midi_Defs.h>
#include <midi_Namespace.h>
#include <midi_Settings.h>
// Descomentar la próxima línea si el compilador no encuentra MIDI
MIDI_CREATE_DEFAULT_INSTANCE();

//__FSCALE____________________
#include <math.h>

//__DISPLAY___________________
#include <Arduino.h>
#include <TM1637Display.h>

#define CLK 6
#define DIO 5

TM1637Display display(CLK, DIO);
uint8_t valores_display[] = { 0x01, 0x01, 0x00, 0x00 };

const uint8_t SEG_DONE[] = {
  SEG_B | SEG_C | SEG_D | SEG_E | SEG_G,           // d
  SEG_A | SEG_B | SEG_C | SEG_D | SEG_E | SEG_F,   // O
  SEG_C | SEG_E | SEG_G,                           // n
  SEG_A | SEG_D | SEG_E | SEG_F | SEG_G            // E
};

int letra_0 = SEG_A | SEG_B | SEG_C | SEG_D | SEG_E | SEG_F ;
int letra_1 = SEG_B | SEG_C ;
int letra_2 = SEG_A | SEG_B | SEG_D | SEG_E | SEG_G ;
int letra_3 = SEG_A | SEG_B | SEG_C | SEG_D | SEG_G ;
int letra_4 = SEG_B | SEG_C | SEG_F | SEG_G ;
int letra_5 = SEG_A | SEG_C | SEG_D | SEG_F | SEG_G ;
int letra_6 = SEG_A | SEG_C | SEG_D | SEG_E | SEG_F | SEG_G ;
int letra_7 = SEG_A | SEG_B | SEG_C ;
int letra_8 = SEG_A | SEG_B | SEG_C | SEG_D | SEG_E | SEG_F | SEG_G ;
int letra_9 = SEG_A | SEG_B | SEG_C | SEG_D | SEG_F | SEG_G ;

int letra_A = SEG_A | SEG_B | SEG_C | SEG_E | SEG_F | SEG_G ;
int letra_E = SEG_A | SEG_D | SEG_E | SEG_F | SEG_G ;
int letra_I = SEG_B | SEG_C ;
int letra_i = SEG_C ;
int letra_O = SEG_A | SEG_B | SEG_C | SEG_D | SEG_E | SEG_F;
int letra_o = SEG_C | SEG_D | SEG_E | SEG_G ;
int letra_U = SEG_B | SEG_C | SEG_D | SEG_E | SEG_F ;
int letra_u = SEG_C | SEG_D | SEG_E ;

int letra_u_down = SEG_A | SEG_C | SEG_D | SEG_E ;
int letra_u_up = SEG_B | SEG_D | SEG_F | SEG_G ;

int letra_B = SEG_A | SEG_B | SEG_C | SEG_D | SEG_E | SEG_F | SEG_G ;
int letra_b = SEG_C | SEG_D | SEG_E | SEG_F | SEG_G ;
int letra_C = SEG_A | SEG_D | SEG_E | SEG_F ;
int letra_c = SEG_D | SEG_E | SEG_G ;
int letra_D = SEG_A | SEG_B | SEG_C | SEG_D | SEG_E | SEG_F ;
int letra_d = SEG_B | SEG_C | SEG_D | SEG_E | SEG_G ;
int letra_F = SEG_A | SEG_E | SEG_F | SEG_G ;
int letra_H = SEG_B | SEG_C | SEG_E | SEG_F | SEG_G  ;
int letra_h = SEG_C | SEG_E | SEG_F | SEG_G  ;
int letra_L = SEG_D | SEG_E | SEG_F ;
int letra_l = SEG_D | SEG_E ;
int letra_N = SEG_A | SEG_B | SEG_C | SEG_E | SEG_F ;
int letra_n = SEG_C | SEG_E | SEG_G ;
int letra_P = SEG_A | SEG_B | SEG_E | SEG_F | SEG_G ;
int letra_S = SEG_A | SEG_C | SEG_D | SEG_F | SEG_G ;
int letra_t = SEG_E | SEG_F | SEG_D | SEG_G ;


//___ EEPROM _________________
#include <EEPROM.h>
int direccion_EEPROM = 0;
int direccion_del_condicional_para_carga_por_primera_y_unica_vez = 1000;   // Arduino UNO tiene 1kb = 1024 direcciones ; MEGA tiene 4kb == 4096 direcciones.
int valor_del_condicional_para_carga_por_primera_y_unica_vez = 99;

//___ ENCODER _________________
#include <Encoder.h>
Encoder mi_Encoder_A(8, 9);  // 8 y 9 son los Pines Digitales a utilizar
long nueva_posicion_Encoder_A = 0;
long vieja_posicion_Encoder_A  = 0;
int num_A = 0;

Encoder mi_Encoder_B(10, 11);
long nueva_posicion_Encoder_B = 0;
long vieja_posicion_Encoder_B  = 0;
int num_B = 0;

//____ Piezo___________________
const int cantidad_de_piezos = 6;
int lectura[ cantidad_de_piezos ];

//________Potenciometro
int pin_pote_selec_piezo = A2;
int n_piezo_seleccionado = 0;
int last_n_piezo_seleccionado = 0;

int pin_pote_selec_parametro = A1;
int n_parametro_seleccionado = 0;
int last_n_parametro_seleccionado = 0;

//________Boton_+
const int boton_pin_MAS = 28;

int estado_boton_MAS = 0;
int boton_MAS_contador = 0;
int last_estado_boton_MAS = 0;


//________Boton_-
const int boton_pin_MENOS = 30;

int estado_boton_MENOS = 0;
int boton_MENOS_contador = 0;
int last_estado_boton_MENOS = 0;


//_________________________________________PARAMETROS
const int cantidad_de_parametros = 7 ;


//_______Valores_de_parametros
int valor_contador = 0;               // esta variable sirve para entrar a la llamada de funcion
int last_valor_contador = 0;          // esta variable sirve para entrar a la llamada de funcion
int valores_en_array[ cantidad_de_piezos ][ cantidad_de_parametros ];

//...................Mask Time
unsigned long tiempo = 0 ;
unsigned long ultimo_hit[ cantidad_de_piezos ];         // Esta var tambien la uso para DECAY

//...................Sensibilidad
int velocity_nota_midi = 0;

//...................Decay
boolean activar_llamada_a_NoteOff[ cantidad_de_piezos ];

//...................SAVE
boolean SAVE = false ;
int contador_SAVE = 0 ;

//_________________________________________PARAMETROS____FIN












//.........................................................................................................Void..SETUP
void setup() {

  Serial.begin(38400);

  //__MIDI
  //   MIDI.begin();                                 // Iniciar la comunicación por MIDI
  //   Serial.begin(31250);                          // ESTA LINEA ES PARA QUE HAIRLESS-MIDI RECONOZCA LOS MENSAJES

  //__Display
  display.setBrightness(10);  // brillo va de 0 a 15;

  //__Piezos
  for (int i = 0 ; i < 4; i++ ) {
    lectura[ i ] = 0 ;
  }

  //__Boton
  pinMode(boton_pin_MAS, INPUT);
  pinMode(boton_pin_MENOS, INPUT);



  //__Parametros_____Array y EEPROM
  int leer_valor_del_condicional = EEPROM.read( direccion_del_condicional_para_carga_por_primera_y_unica_vez );


  if ( leer_valor_del_condicional == 99 ) {
    for (int i = 0; i < cantidad_de_piezos; i++) {
      valores_en_array[ i ][ 0 ] = EEPROM.read( cantidad_de_parametros * 0 + i ) ;
      valores_en_array[ i ][ 1 ] = EEPROM.read( cantidad_de_parametros * 1 + i ) ;
      valores_en_array[ i ][ 2 ] = EEPROM.read( cantidad_de_parametros * 2 + i ) ;
      valores_en_array[ i ][ 3 ] = EEPROM.read( cantidad_de_parametros * 3 + i ) ;
      valores_en_array[ i ][ 4 ] = EEPROM.read( cantidad_de_parametros * 4 + i ) ;
      valores_en_array[ i ][ 5 ] = EEPROM.read( cantidad_de_parametros * 5 + i ) ;
      valores_en_array[ i ][ 6 ] = EEPROM.read( cantidad_de_parametros * 6 + i ) ;
    }

  } else {    //  PRIMERA VEZ
    for (int i = 0; i < cantidad_de_piezos; i++) {
      // El sistema esta funcionando con valores que van de 0 a 396.      // Pero para la EEPROM estoy manejando valores entre 0 y 99
      // Asigna valor
      valores_en_array[ i ][ 0 ] = 25  ;        // Umbral
      valores_en_array[ i ][ 1 ] = 99  ;        // Sensibilidad TOPE
      valores_en_array[ i ][ 2 ] = 15  ;        // Mask TIME
      valores_en_array[ i ][ 3 ] = 49.5 ;       // CURVA Sens
      valores_en_array[ i ][ 4 ] = i + 60 ;     // Nota MIDI #
      valores_en_array[ i ][ 5 ] = 8 ;          // DECAY
      valores_en_array[ i ][ 6 ] = 0 ;          // SAVE
      // Almacena valores asignados
      EEPROM.update( cantidad_de_parametros * 0 + i   , valores_en_array[ i ][ 0 ] );
      EEPROM.update( cantidad_de_parametros * 1 + i   , valores_en_array[ i ][ 1 ] );
      EEPROM.update( cantidad_de_parametros * 2 + i   , valores_en_array[ i ][ 2 ] );
      EEPROM.update( cantidad_de_parametros * 3 + i   , valores_en_array[ i ][ 3 ] );
      EEPROM.update( cantidad_de_parametros * 4 + i   , valores_en_array[ i ][ 4 ] );
      EEPROM.update( cantidad_de_parametros * 5 + i   , valores_en_array[ i ][ 5 ] );
      EEPROM.update( cantidad_de_parametros * 6 + i   , valores_en_array[ i ][ 6 ] );

    }
    EEPROM.update( direccion_del_condicional_para_carga_por_primera_y_unica_vez, valor_del_condicional_para_carga_por_primera_y_unica_vez );
  }


  // Los valores en EEPROM se guardan de 0 a 99.
  // Aca rangueo nuevamente: algunos de 0 a 396, y otros a sus rangos ;
  for (int i = 0; i < cantidad_de_piezos; i++) {
    ultimo_hit[ i ] = 0;
    activar_llamada_a_NoteOff[ i ] = false;

    Serial.println(  "  ANTES  "  ) ;
    Serial.println(  valores_en_array[ i ][ 4 ]  ) ;
    valores_en_array[ i ][ 0 ] = valores_en_array[ i ][ 0 ] * 4 ;        // Umbral
    valores_en_array[ i ][ 1 ] = valores_en_array[ i ][ 1 ] * 4 ;        // Sensibilidad TOPE
    valores_en_array[ i ][ 2 ] = valores_en_array[ i ][ 2 ] * 2 ;        // Mask TIME
    valores_en_array[ i ][ 3 ] = valores_en_array[ i ][ 3 ] * 4 ;        // CURVA Sens
    valores_en_array[ i ][ 4 ] = valores_en_array[ i ][ 4 ] * 4 ;        // NOTA MIDI
    valores_en_array[ i ][ 5 ] = valores_en_array[ i ][ 5 ] * 2;         // DECAY
    Serial.println(  "  DESPUES  "  ) ;
    Serial.println(  valores_en_array[ i ][ 0 ]  ) ;
    Serial.println(  valores_en_array[ i ][ 1 ]  ) ;
    Serial.println(  valores_en_array[ i ][ 2 ]  ) ;
    Serial.println(  valores_en_array[ i ][ 3 ]  ) ;
    Serial.println(  valores_en_array[ i ][ 4 ]  ) ;
    Serial.println(  valores_en_array[ i ][ 5 ]  ) ;

  }

}







//...................................................................................................................f.SAVE
void f_SAVE() {

  for (int i = 0; i < cantidad_de_piezos; i++) {
    EEPROM.update( cantidad_de_parametros * 0 + i   , valores_en_array[ i ][ 0 ] / 4 );
    EEPROM.update( cantidad_de_parametros * 1 + i   , valores_en_array[ i ][ 1 ] / 4 );
    EEPROM.update( cantidad_de_parametros * 2 + i   , valores_en_array[ i ][ 2 ] / 2 );
    EEPROM.update( cantidad_de_parametros * 3 + i   , valores_en_array[ i ][ 3 ] / 4 );
    EEPROM.update( cantidad_de_parametros * 4 + i   , valores_en_array[ i ][ 4 ] / 4 );
    EEPROM.update( cantidad_de_parametros * 5 + i   , valores_en_array[ i ][ 5 ] / 2 );
  }

  EEPROM.update( direccion_del_condicional_para_carga_por_primera_y_unica_vez, 99 );

  valores_display[ 0 ] = letra_S ;
  valores_display[ 1 ] = letra_A ;
  valores_display[ 2 ] = letra_U ;
  valores_display[ 3 ] = letra_E ;
  display.setSegments( valores_display );
  delay( 2000 );
  SAVE = false ;
  contador_SAVE = 0 ;
  valores_en_array[ n_piezo_seleccionado ][ n_parametro_seleccionado ] = 0 ;

}







//.......................................................................................................LEER..BOT..y..POT
void f_lectura_botones_y_potes() {

  // Pmetr  !=  SAVE
  if ( n_parametro_seleccionado !=  6    ) {
    contador_SAVE = 0 ;

    //____Boton +
    if (estado_boton_MAS != last_estado_boton_MAS ) {
      boton_MAS_contador++;
      if (boton_MAS_contador % 2 == 0)  n_piezo_seleccionado ++;
      last_estado_boton_MAS = estado_boton_MAS ;
      delay(10);
    }

    //____Boton -
    if (estado_boton_MENOS != last_estado_boton_MENOS) {
      boton_MENOS_contador++;
      if (boton_MENOS_contador % 2 == 0)     n_piezo_seleccionado --;
      last_estado_boton_MENOS = estado_boton_MENOS ;
      delay(10);
    }

    // Seleccion de Piezo tipo "toroide"
    if ( n_piezo_seleccionado  >= cantidad_de_piezos )    n_piezo_seleccionado = 0 ;
    if ( n_piezo_seleccionado  < 0 )    n_piezo_seleccionado = cantidad_de_piezos - 1 ;
  }


  // Pmetr  =  SAVE
  if ( n_parametro_seleccionado ==  6  ) {
    contador_SAVE = valores_en_array[ n_piezo_seleccionado ][ n_parametro_seleccionado ];
    Serial.print ( "Contador_SAVE = ");
    Serial.println ( contador_SAVE );
    if ( contador_SAVE == 50 ) {
      SAVE = true ;
      delay ( 20 );
    }
  }

  if ( SAVE ) {
    f_SAVE();
  }

  //__Llamada_a_funcion_de_parametros_____
  if ( valor_contador != last_valor_contador || n_piezo_seleccionado != last_n_piezo_seleccionado || n_parametro_seleccionado != last_n_parametro_seleccionado ) {
    f_valores_en_display( n_piezo_seleccionado , n_parametro_seleccionado , valores_en_array[ n_piezo_seleccionado ][ n_parametro_seleccionado ] );
  }
  last_n_piezo_seleccionado = n_piezo_seleccionado ;
  last_n_parametro_seleccionado = n_parametro_seleccionado;
}










//..........................................................................................................FUNC..2-DIGITOS
int f_digito_a_letra (int numero_a_convertir ) {
  switch ( numero_a_convertir ) {
    case 0:      return numero_a_convertir = letra_0 ;      break;
    case 1:      return numero_a_convertir = letra_1 ;      break;
    case 2:      return numero_a_convertir = letra_2 ;      break;
    case 3:      return numero_a_convertir = letra_3 ;      break;
    case 4:      return numero_a_convertir = letra_4 ;      break;
    case 5:      return numero_a_convertir = letra_5 ;      break;
    case 6:      return numero_a_convertir = letra_6 ;      break;
    case 7:      return numero_a_convertir = letra_7 ;      break;
    case 8:      return numero_a_convertir = letra_8 ;      break;
    case 9:      return numero_a_convertir = letra_9 ;      break;
  }
}







//................................................................................................................f_de_X_a_99
int f_descomponer_unidad ( int numero , int operacion ) {
  int decimal;
  int unidad ;
  numero = map ( numero , 0 , 1023 , 0 , 99);
  decimal = numero / 10 ;
  unidad = numero - decimal * 10 ;
  if ( operacion == 10 )     return decimal ;
  if ( operacion == 1 )      return unidad  ;
  if ( operacion == 99 )     return numero  ;
}









//....................................................................................................................DISPLAY
void f_valores_en_display( int piezo_elegido, int parametro_elegido , int valor_parametro ) {

  //remapeo
  valor_parametro = map ( valor_parametro , 0 , 396 , 0 , 1023  );

  switch ( piezo_elegido ) {
    case 0:      valores_display[0] = letra_0;      break;
    case 1:      valores_display[0] = letra_1;      break;
    case 2:      valores_display[0] = letra_2;      break;
    case 3:      valores_display[0] = letra_3;      break;
    case 4:      valores_display[0] = letra_4;      break;
    case 5:      valores_display[0] = letra_5;      break;
  }

  switch ( parametro_elegido ) {
    case 0:      valores_display[1] = letra_L ;      break;   // Pmtr: THRESHOLD
    case 1:      valores_display[1] = letra_H ;      break;   // Pmtr: SENSIBILIDAD
    case 2:      valores_display[1] = letra_t ;      break;   // Pmtr: MASK TIME
    case 3:      valores_display[1] = letra_c ;      break;   // Pmtr: CURVA SENS
    case 4:      valores_display[1] = letra_n ;      break;   // Pmtr: NUMERO NOTA
    case 5:      valores_display[1] = letra_d ;      break;   // Pmtr: DECAY
    case 6:      valores_display[1] = letra_E ;      break;   // Pmtr: SAVE
  }

  // No es SAVE
  if ( n_parametro_seleccionado != 6 ) {
    int digito_decimal = f_descomponer_unidad( valor_parametro , 10 );
    int digito_unidad  = f_descomponer_unidad( valor_parametro , 1 );

    valores_display[2] = f_digito_a_letra ( digito_decimal ) ;
    valores_display[3] = f_digito_a_letra ( digito_unidad );
  }

  //  SAVE
  if ( n_parametro_seleccionado == 6 ) {
    valores_display[ 0 ] = letra_E;
    valores_display[ 1 ] = letra_d;
    valores_display[ 2 ] = letra_i;
    valores_display[ 3 ] = letra_t;
  }

  display.setSegments( valores_display );

}









//......................................................................................................................................fscale..
float fscale( float originalMin, float originalMax, float newBegin, float newEnd, float inputValue, float curve) {
  float OriginalRange = 0;
  float NewRange = 0;
  float zeroRefCurVal = 0;
  float normalizedCurVal = 0;
  float rangedValue = 0;
  boolean invFlag = 0;

  // LAF :: mapeo el valor (0 a 396) y lo limito para que entregue curvas de entre -5 y 5 (curvas mayores son muy extremas y NO garpan )
  // curve = map ( curve , 0 , 396 , -5 , 5 );   <--- Este mapeo solo da INT, por lo que tiene menos resolucion que hacerlo como esta mas abajo
  curve = curve / 39.6 - 5 ;      // LAF - Mapeo que da numeros FLOAT

  if (curve > 5) curve = 5 ;
  if (curve < -5) curve = -5 ;
  curve = (curve * -.1) ; // - invert and scale - this seems more intuitive - postive num_Abers give more weight to high end on output
  curve = pow(10, curve); // convert linear scale into lograthimic exponent for other pow function
  // Check for out of range inputValues
  if (inputValue < originalMin) inputValue = originalMin ;
  if (inputValue > originalMax) inputValue = originalMax ;
  // Zero Refference the values
  OriginalRange = originalMax - originalMin;
  if (newEnd > newBegin) {
    NewRange = newEnd - newBegin;
  }  else  {
    NewRange = newBegin - newEnd;
    invFlag = 1;
  }
  zeroRefCurVal = inputValue - originalMin;
  normalizedCurVal  =  zeroRefCurVal / OriginalRange;   // normalize to 0 - 1 float
  // Check for originalMin > originalMax  - the math for all other cases i.e. negative num_Abers seems to work out fine
  if (originalMin > originalMax )  return 0 ;
  if (invFlag == 0) {
    rangedValue =  (pow(normalizedCurVal, curve) * NewRange) + newBegin;
  }  else   {
    // invert the ranges
    rangedValue =  newBegin - (pow(normalizedCurVal, curve) * NewRange);
  }
  return rangedValue;
}









//..............................................................................................................................lectura..piezos
void f_lectura_de_piezos() {
  tiempo = millis();

  // lectura
  for (int i = 0 ; i < cantidad_de_piezos ; i++ ) {
    lectura[ i ] = analogRead(  i + 10 );      // numero de Entrada Analogica

    //   millis > ( millis_del ultim_hit +  MASK TIME   )
    if ( tiempo > (  ultimo_hit[ i ]     +  valores_en_array[ i ][ 2 ] * 2 ) ) {      // de 0 a 396 * 2 = de 0 a 792 millis

      // ( valor[piezo] > THRESHOLD )                * se multiplica 2.584 para reescalar el numero de 396 a 1023
      if ( lectura[ i ] > valores_en_array[ i ][ 0 ] * 2.584) {

        // Pone TOPE a la lectura del Piezo
        //   lectura      >= Sensibilidad
        if ( lectura[ i ] >= valores_en_array[ i ][ 1 ] * 2.584 )  lectura[ i ] = valores_en_array[ i ][ 1 ]  * 2.584 ;

        //                 = fscale( min ( THRESH )                     , max ( SENS )                             ,newmin  , newmax  , valor        , curva_entre_-10_y_10     )
        velocity_nota_midi = fscale( valores_en_array[ i ][ 0 ] * 2.584 , int(valores_en_array[ i ][ 1 ]  * 2.584) , 0      , 127     , lectura[ i ] , valores_en_array[ i ][ 3 ] );

        //   int n_nota = f_descomponer_unidad( valores_en_array[ i ][ 4 ] , 99 );
        int n_nota = valores_en_array[ i ][ 4 ] / 4 ;
        Serial.print( "n_nota: " );
        Serial.println( n_nota );
        // ACA MANDA LA NOTA MIDI
              Serial.print( " Lectura       " );
              Serial.print( lectura[ i ] );
              Serial.print( " Piezo       " );
              Serial.print( i );
              Serial.print( " Velocity " );
              Serial.print( velocity_nota_midi );
              Serial.print( "        n_nota: " );
              Serial.println( n_nota );
        

        // MIDI           ( Nota MIDI # , Velocity           , CANAL  ( 0 = omni )
        // MIDI.sendNoteOn( n_nota      , velocity_nota_midi , 1 );
        activar_llamada_a_NoteOff[ i ] = true ;

        ultimo_hit[ i ] = tiempo;
      }
    } else {
      //      Serial.println( " NO supero MASK " );
    }
  }
}









void f_mandar_note_off() {
  for (int i = 0 ; i < cantidad_de_piezos ; i++ ) {
    if ( activar_llamada_a_NoteOff[ i ] == true  ) {

      // ( tiempo >    ultimo_hit      + tiempo_de_decay ( valorn entre 0 a 792 )
      if ( tiempo > (  ultimo_hit[ i ] + valores_en_array[ i ][ 5 ] * 2 ) ) {

        //                                 Nota MIDI numero##         , devuelve ese valor mapeado
        int n_nota = f_descomponer_unidad( valores_en_array[ i ][ 4 ] , 99 );
        //    MIDI.sendNoteOff( n_nota , 0 , 1 );

        Serial.print( " THERSHOLD    = " );
        Serial.println( valores_en_array[ i ][ 0 ] * 2.584 );
        Serial.print( " SENS         = " );
        Serial.println( valores_en_array[ i ][ 1 ] * 2.584 );
        Serial.print( " MASK T       = " );
        Serial.println( valores_en_array[ i ][ 2 ] * 2 );
        Serial.print( " CURVA        = " );
        Serial.println( valores_en_array[ i ][ 3 ] / 39.6 - 5 );
        Serial.print( " NUM NOTA     = " );
        Serial.println( valores_en_array[ i ][ 4 ] / 4 );
        Serial.print( " DECAY        = " );
        Serial.println( valores_en_array[ i ][ 5 ] * 2);
        Serial.println("      ___      ");

        activar_llamada_a_NoteOff[ i ] = false ;

      }
    }
  }
}









void f_lectura_encoders() {

  // ____Encoder A ____PARAMETRO_selec
  if (nueva_posicion_Encoder_A != vieja_posicion_Encoder_A) {
    if (nueva_posicion_Encoder_A > vieja_posicion_Encoder_A) {
      num_A++;
      vieja_posicion_Encoder_A = nueva_posicion_Encoder_A;
    }
    if (nueva_posicion_Encoder_A < vieja_posicion_Encoder_A) {
      num_A--;
      vieja_posicion_Encoder_A = nueva_posicion_Encoder_A;
    }

    // Restringe el valor de 'num_A' a un ambito_____
    // cantidad_d_para * 8 se debe a que: cada movimiento del Encoder = 4 o -4. Entonces = 2 *tics* del encoder = cambio de parametro
    if (num_A > cantidad_de_parametros * 8 ) num_A = 0 ;
    if (num_A < 0 ) num_A = cantidad_de_parametros * 8 ;

    n_parametro_seleccionado = map( num_A - 1, 0 , cantidad_de_parametros * 8 , 0 , cantidad_de_parametros  );
  }


  // ____Encoder B ____Amount
  if ( nueva_posicion_Encoder_B != vieja_posicion_Encoder_B ) {
    if ( nueva_posicion_Encoder_B > vieja_posicion_Encoder_B )   valores_en_array[ n_piezo_seleccionado ][ n_parametro_seleccionado ] ++ ;
    if ( nueva_posicion_Encoder_B < vieja_posicion_Encoder_B )   valores_en_array[ n_piezo_seleccionado ][ n_parametro_seleccionado ] -- ;

    //Restringe a Ambito de 0 a 396 (= 99 * 4 ; 99 de lo digitos del Display; 4 por los movimientos del Encoder)
    if ( valores_en_array[ n_piezo_seleccionado ][ n_parametro_seleccionado ] >= 396 )   valores_en_array[ n_piezo_seleccionado ][ n_parametro_seleccionado ] = 396 ;
    if ( valores_en_array[ n_piezo_seleccionado ][ n_parametro_seleccionado ] <=  0  )   valores_en_array[ n_piezo_seleccionado ][ n_parametro_seleccionado ] =  0  ;

    vieja_posicion_Encoder_B = nueva_posicion_Encoder_B ;
  }

  // Dibuja en Display...(lo hice de esta manera porque de otras formas la lectura del Encoder era mala).
  if ( tiempo % 50 == 0 ) f_valores_en_display( n_piezo_seleccionado , n_parametro_seleccionado , valores_en_array[ n_piezo_seleccionado ][ n_parametro_seleccionado ] ) ;

  if ( n_parametro_seleccionado == 6 ) {
    Serial.println( "  SAVE  " );
  }


}









//..................................................................void..loop
void loop() {


  estado_boton_MAS = digitalRead(boton_pin_MAS);
  estado_boton_MENOS = digitalRead(boton_pin_MENOS);

  nueva_posicion_Encoder_A = mi_Encoder_A.read();
  nueva_posicion_Encoder_B = mi_Encoder_B.read();

  f_lectura_encoders();
  f_lectura_botones_y_potes();
  f_lectura_de_piezos();
  f_mandar_note_off();


}


