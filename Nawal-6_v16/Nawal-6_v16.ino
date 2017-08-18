/* --------------------------------------------------------
 *  // Codigo : por Luciano Fritschy
 *  // Web : www.tacuaramultimedia.com.ar
 *  // Facebook : https://www.facebook.com/chamaco.fritschy
 * --------------------------------------------------------
 * 
 * ----------> La libreria 'MIDI.h', funciona con errores cuando se utiliza la salida por DIN5. 
 * ----------> Por eso la saqué. Ahora los mensajes se envian utlizando funciones propias.
 * ----------> La unica cuestión, es que no estoy definiendo Canal MIDI. No se bien si se envian por OMNI, o qué.
 * ----------> (El uso de los pedales SIN la LIBRERIA, ya está probado y todo anda perfecto.)
 * 
 * 
 *  INFO: 
 *        - Este Sketch está hecho para un Controlador MIDI tipo Footswich/Pedal, para un ARDUINO UNO. (Para utilizarlo con MEGA quizas haya que modificar algo).
 *        - Posee 3 Escenas navegables utilizando 2 de los fooswitch.
 *        - Cada escena es indicada con un determinado color ( escena 1 = verde , escena 2 = rojo , escena 3 = naranja ).
 *        - Posee 4 switch que pueden funcionar como momentaneos y enviar mensajes de tipo ProgramChange; o como switch y en enviar mensajes de tipo Control Change ON/OFF (0/127) 
 *        - Para asiganar el comportamiento de cada footswitch, se debe ingresar al 'Modo Setup' (o Modo Configuracion).
 *        - Para ingresar al 'Modo Setup' se deben apretar al mismo tiempo los 2 botones de cambio de escena.
 *          - Una vez dentro, el led que indica la escena comenzara a PARPADEAR (a un ritmo lento). 
 *          - Entonces, para cambiar el comportamiento de cada footswitch (MOMENTANEO/SWITCH), solo hace falta apretar el footswitch desdeado.
 *            · Si el comportamiento es Momentaneo (Program Change), el led del footswitch PARPADEARÁ.
 *            · Si el comportamiento es Switch (Control Change), el led del footswitch SE MANTENDRÁ ENCENDIDO.
 *        
 *        - Los footswitch de cada escena, envian mensajes ProgramChange/ControlChange distintos en independientes de las demas escenas.       
 *        - En escena verde los footswitch envian mensajes ProgramChange o ControlChange: 0 , 1 , 2 , 3
 *        - En escena roja los footswitch envian mensajes ProgramChange o ControlChange: 4 , 5 , 6 , 7
 *        - En escena naranja los footswitch envian mensajes ProgramChange o ControlChange: 8 , 9 , 10 , 11
 *        
 *  INFO TECNICA: 
 *        - El Sketch está hecho para funcionar con:
 *          - 6 Pulsadores.
 *          - 5 Leds Bicolor (led RG), de 3 patas (con negativo al centro).
 *          - Un pedal de expresión conectado mediante un Jack Stereo Hembra con corte ( TRS con corte ).
 *            - El 'Corte' del pedal, funciona exactamente igual que un boton/pulsador.
 *          ----> Si no se utiliza este conector, es importante que se bloquee o  'comente' (//) la llamada la funcion encargada de leer el pedal (f_boton_de_corte_de_pedal_de_exp)
 *          ----> De otra manera, la funcion leerá valores erroneos y enviara Mensajes MIDI no deseados.
 *          
 *  CONEXIONES:        
 *          - Pulsadores: Se debe utilizar una resistencia de 1K en cada pulsador (ver en pagina de arduino).
 *          - Led bi color: · se debe conectar una resistencia de 220ohm a las patas VERDES. 
 *                          · se debe conectar una resistencia de 470ohm a las patas ROJAS.
 *                       ---> ESTO sirve para que el color NARANJA se mezcle bien, nada mas.
 *       
 * 
 *  REFER:
 *        - Navegacion de Escenas: pines 2 y 3 . 
 *        - Pulsadores Switch: pines 4, 5 , 6 y 7 .
 *        - Leds de Navegacion: Pines: 8 , 9 .
 *        - Pines Led Switch Verdes: 10 ,  11 , 12 , 13 . 
 *        - Pines Led Switch Rojos: 14 ,  15 , 16 , 17 .
 *        - Pin de Pedal de expresion: A5 .
 *        - Pin de Corte de Pedal de expresion: A4 .
 *        
 * 
 * 
 *  Estado:
 *        - Lectura de 2 botones para Seleccion de Escena.
 *        - Presionando ambos botones de navegacion al mismo tiempo: se ingresa al Modo Configuracion ( esto sirve para configurar el comportamietno de cada SWITCH )
 *        - La estrutura de lectura de los botones ya discrimina entre: Modo_Config > Comportamiento_Switch
 *        - Switch independientes en cada Escena: OK.
 *        - Leds independientes en cada Escena: OK.
 *        - Comportamiento tipo SWITCH: OK.
 *        - Comportamiento tipo MOMENTANEO: OK.
 *        - Configuracion de Modos de cada boton: OK.
 *        - Comportamientos de Leds en Modo Config: OK.
 *        - Comportamientos de Leds en Modo Ejecucion: OK.
 *        - Envio de Mensajes MIDI: OK.
 *        - Pedal de expresion con corte: OK.
 *        - comportamiento asignado a cada SWITCH se guarda en la EEPROM: OK .
 * 
 * 
 *  Se puede Mejorar:  
 *        - El funcionamiento de los LEDs no es 'dinamico', si agrego mas cantidad_de_escenas, hay que editar manualmente varias funciones
 * 
 * 
 * 
 *  In This:
 *        - Corrigiendo el Pedal de Expresión. 
 *        
 *        
 *        
 */



 
/*  Libreria EPPROM   ---------------------------------  */
#include <EEPROM.h>
int direccion_EEPROM = 0;
int direccion_del_condicional_para_carga_por_primera_y_unica_vez = 1000 ;   // Arduino UNO tiene 1kb = 1024 direcciones ; MEGA tiene 4kb == 4096 direcciones.
int valor_del_condicional_para_carga_por_primera_y_unica_vez = 99 ;


/*    Botones de Navegacion y Escenas   --------------  */
#define cant_botones_de_nav  2 
byte pin_botones_de_nav[ cant_botones_de_nav ] ;
byte estado_botones_nav[ cant_botones_de_nav ];
byte estado_botones_nav_last[ cant_botones_de_nav ];
byte cont_pulsaciones_de_nav[ cant_botones_de_nav ];

byte cant_escenas = 3;
int escena_en_foco = 0;
byte cambio_de_escena_habilitado[ cant_botones_de_nav ];

byte bot_DOWN = 0;
byte bot_UP = 1;
int mas_uno = 1;
int menos_uno = -1;

/*     Modo Configuracion  ---------------------------   */
boolean modo_configuracion_habilitado_out = true ;
byte cont_pulsaciones_de_modo_configuracion = 0 ;
byte estado_boton_config = 0 ;
byte estado_boton_config_last = 0 ;




/*     Botones de Switch  ---------------------------  */
#define cant_botones_de_switch  4 
byte pin_botones_de_switch[ cant_botones_de_switch ];
byte estado_botones_switch[ cant_botones_de_switch ];
byte estado_botones_switch_last[ cant_botones_de_switch ];
byte cont_pulsaciones_de_switch[ cant_botones_de_switch ];

/*   Cantidad de Botones VIRTUALES =  cant_escenas * cant_botones_de_switch = 12 */
#define cant_botones_virtuales  12 
byte i_virtual;

byte estado_botones_switch_virtual[ cant_botones_virtuales ];  
byte estado_botones_switch_virtual_last[ cant_botones_virtuales ];
byte cont_pulsaciones_de_switch_virtual[ cant_botones_virtuales ];
boolean comportamiento_de_switch_virtual_MOMENTANEO[ cant_botones_virtuales ];
boolean enviar_mensaje_ON_habilitado[ cant_botones_virtuales ];




/*  LEDS de Navegacion    ---------------------------  */
byte pin_led_nav_verde = 8 ;
byte pin_led_nav_rojo = 9 ;
boolean estado_led_nav_verde = false ;
boolean estado_led_nav_rojo = false ;
int contador_led_nav_parpadear = 0 ;

int intervalo_de_parpadeo = 500 ;
boolean estado_led_nav_parpadear = false ;
boolean estado_led_nav_parpadear_last = false ; 

/*  LEDS de SWITCH Verde   ---------------------------  */
byte pines_de_led_switch_verde[ cant_botones_de_switch ];
boolean estado_botones_led_virtual_verde[ cant_botones_virtuales ];  
/*  LEDS de SWITCH Rojo   ---------------------------  */
byte pines_de_led_switch_rojo[ cant_botones_de_switch ];
boolean estado_botones_led_virtual_rojo[ cant_botones_virtuales ];  



/*  Pedal de exp  */
#define pin_pedal_de_exp A5 
int lectura_pedal_de_exp = 0 ;
int lectura_pedal_de_exp_last = 0 ;
/*  Boton de Corte         */
#define pin_boton_de_corte_de_pedal_de_exp  A4
boolean estado_boton_de_corte_de_pedal_de_exp = false ;

#define numero_de_mensaje_midi_del_pedal  20 




void setup() {
  
  Serial.begin(31250);
  //Serial.begin(9600);

  /*  Seteo de pines NAV  */
  for (byte i = 0 ; i < cant_botones_de_nav; i++ )  {
    
    pin_botones_de_nav[ i ] = i + 2 ;     // Esto esta dando = 2 , 3 
    estado_botones_nav[ i ] = 0 ;
    estado_botones_nav_last[ i ] = 0 ;
    cont_pulsaciones_de_nav[ i ] = 0 ;
    cambio_de_escena_habilitado[ i ] = true;
    
    pinMode( pin_botones_de_nav[ i ] , INPUT) ;
//    Serial.print( "Pines Nav: " );
//    Serial.println( pin_botones_de_nav[ i ] );
  }
  
  /*  Seteo de pines SWITCH  */
  for (byte i = 0 ; i < cant_botones_de_switch; i++ )  {
    
    pin_botones_de_switch[ i ] = i + 4 ;     // Pines = 4 , 5 , 6 , 7 
    estado_botones_switch[ i ] = 0 ;
    estado_botones_switch_last[ i ] = 0 ;
    cont_pulsaciones_de_switch[ i ] = 0 ;
    
    pinMode( pin_botones_de_switch[ i ] , INPUT) ;
//    Serial.print( "Pines Switch: " );
//    Serial.println( pin_botones_de_switch[ i ] );
  }
  /*  Seteo de pines SWITCH VIRTUALES */
  for (byte i = 0 ; i < cant_botones_de_switch * cant_escenas ; i++ )  {

    estado_botones_switch_virtual[ i ] = 0 ;
    estado_botones_switch_virtual_last[ i ] = 0 ;
    cont_pulsaciones_de_switch_virtual[ i ] = 0 ;
    comportamiento_de_switch_virtual_MOMENTANEO[ i ] = false ;
    enviar_mensaje_ON_habilitado[ i ] = true ;
    
//    Serial.print( "Switch Virtuales: " );
//    Serial.println( i );
  }



  /* Seteo de Leds de Switch Verdes  */
  for (byte i = 0 ; i < cant_botones_de_switch; i++ )  {
    
    estado_botones_led_virtual_verde[ i ] = false ;   
    pines_de_led_switch_verde[ i ] = i + 10 ;     // Pines = 10 , 11 , 12 , 13 
    
    pinMode( pines_de_led_switch_verde[ i ] , OUTPUT ) ;
//    Serial.print( "Pines Led Switch Verdes: " );
//    Serial.println( pines_de_led_switch_verde[ i ] );
  }
  /* Seteo de Leds de Switch Rojos */
  for (byte i = 0 ; i < cant_botones_de_switch; i++ )  {

    estado_botones_led_virtual_rojo[ i ] = false ;
    pines_de_led_switch_rojo[ i ] = i + 14 ;     // Pines = 14 , 15 , 16 , 17 
    
    pinMode( pines_de_led_switch_rojo[ i ] , OUTPUT ) ;
//    Serial.print( "Pines Led Switch Rojos: " );
//    Serial.println( pines_de_led_switch_rojo[ i ] );
  }
  /* Seteo de Leds de Navegacion */
  pinMode( pin_led_nav_verde , OUTPUT) ;
  pinMode( pin_led_nav_rojo , OUTPUT) ;
 


  /* Seteo de EEPROM    */
  int leer_valor_del_condicional = EEPROM.read( direccion_del_condicional_para_carga_por_primera_y_unica_vez );
  if ( leer_valor_del_condicional == 99 ) {
    for (int i = 0; i < cant_botones_de_switch * cant_escenas ; i++) {
      comportamiento_de_switch_virtual_MOMENTANEO[ i ] = EEPROM.read( i ) ; 

    }

  } else {                                   /*  PRIMERA VEZ - Cuando aún nunca se ha guardado los valores */  

    for (int i = 0; i < cant_botones_de_switch * cant_escenas ; i++) {

      /* Asigna valor */
      comportamiento_de_switch_virtual_MOMENTANEO[ i ] = false ; 

      /* Almacena valores asignados */
      EEPROM.update( i , comportamiento_de_switch_virtual_MOMENTANEO[ i ] );
     
    }
    EEPROM.update( direccion_del_condicional_para_carga_por_primera_y_unica_vez, valor_del_condicional_para_carga_por_primera_y_unica_vez );
  }


}


void loop() {
  
  f_lectura_botones_de_nav();
  f_lectura_botones_de_switch();
  f_pedal_de_exp();

  f_led_nav();
  if( modo_configuracion_habilitado_out == true ){
    f_led_switch_verde();
    f_led_switch_rojo();
  }else if(modo_configuracion_habilitado_out == false ){
    f_led_switch_verde_modo_configuracion();
    f_led_switch_rojo_modo_configuracion();
  }

  
}





void f_lectura_botones_de_nav(){
  for (byte i = 0 ; i < cant_botones_de_nav ; i++ ){
    estado_botones_nav[ i ] = digitalRead(  pin_botones_de_nav[ i ] );  
    estado_boton_config = estado_botones_nav[ 0 ] && estado_botones_nav[ 1 ];
    /*   Boton de Modo Configuracion   -------------------------------------------  */
    if( estado_boton_config != estado_boton_config_last ){
      if( estado_botones_nav[ bot_DOWN ] == HIGH && estado_botones_nav[ bot_UP ] == HIGH ){

        cont_pulsaciones_de_modo_configuracion ++;
      
      }
      delay(50);
    }
    estado_boton_config_last = estado_boton_config ;


    if ( cont_pulsaciones_de_modo_configuracion % 2 == 0 ) {
      if( !modo_configuracion_habilitado_out ){
//        Serial.println( "--------------------------------------------------- Modo Config: OUT" );
        f_SAVE();
        modo_configuracion_habilitado_out = true;
      } 
    }
    else{
      if( modo_configuracion_habilitado_out ){
//        Serial.println( "--------------------------------------------------- Modo Config: IN" );
        modo_configuracion_habilitado_out = false;
      } 
    }

    
    /*    Botones de Escena   ------------------------------------------- */
    if ( estado_botones_nav[ i ] != estado_botones_nav_last[ i ]) {
       if ( estado_botones_nav[ i ] == HIGH) {
            cont_pulsaciones_de_nav[ i ]++ ;
       }
       delay(50);
    }
    estado_botones_nav_last[ i ] = estado_botones_nav[ i ];

  
    if ( cont_pulsaciones_de_nav[ i ] % 2 == 0 ) {
      if( !cambio_de_escena_habilitado[ i ] ){
        if( i == bot_DOWN ){
          f_cambiar_escena( menos_uno );
        }else if( i == bot_UP ){
          f_cambiar_escena( mas_uno );
        }
        cambio_de_escena_habilitado[ i ] = true;
      } 
    }
    else{
      if( cambio_de_escena_habilitado[ i ] ){
        if( i == bot_DOWN ){
          f_cambiar_escena( menos_uno );
        }else if( i == bot_UP ){
          f_cambiar_escena( mas_uno );
        }
        cambio_de_escena_habilitado[ i ] = false;
      } 
    }
    
  }
}
void f_cambiar_escena( int mas_o_menos ){

  escena_en_foco += mas_o_menos;
  if( escena_en_foco >= cant_escenas ) {
    escena_en_foco -= cant_escenas  ;
  }else if( escena_en_foco < 0 ){
    escena_en_foco += cant_escenas  ;
  }
//  Serial.print("---------------------------------- Escena: ");
//  Serial.println( escena_en_foco );
}





void f_lectura_botones_de_switch(){
  for (byte i = 0 ; i < cant_botones_de_switch ; i++ ){
    estado_botones_switch[ i ] = digitalRead(  pin_botones_de_switch[ i ] );  

    f_lectura_botones_de_switch_2( i );

  }
}
void f_lectura_botones_de_switch_2( int i ){
  i_virtual = ( i + ( escena_en_foco + 1 ) * cant_botones_de_switch ) - cant_botones_de_switch ;

  estado_botones_switch_virtual[ i_virtual ] = estado_botones_switch[ i ];

  if ( estado_botones_switch_virtual[ i_virtual ] != estado_botones_switch_virtual_last[ i_virtual ]) {
     if ( estado_botones_switch[ i ] == HIGH ) {
          cont_pulsaciones_de_switch_virtual[ i_virtual ]++ ;

          if( modo_configuracion_habilitado_out ) {                                /* Si estamos FUERA del Modo Config            */
            if( comportamiento_de_switch_virtual_MOMENTANEO[ i_virtual ] == false){   /* Si el switch se comporta como SWITCH     */
              f_boton_tipo_SWITCH( i_virtual , i );
            }
            if( comportamiento_de_switch_virtual_MOMENTANEO[ i_virtual ] == true){    /* Si el switch se comporta como MOMENTANEO */
              f_boton_tipo_MOMENTANEO( i_virtual );
            }
            
          }
          else if( !modo_configuracion_habilitado_out ){                           /* Si estamos DENTRO del Modo Config */
            f_boton_modo_configuracion_INSIDE( i_virtual );
          }
     }
     delay(50);
  }
  
  estado_botones_switch_virtual_last[ i_virtual ] = estado_botones_switch_virtual[ i_virtual ]; 
}
void f_boton_tipo_SWITCH( int i_virtual , int i ){
  
//  Serial.print("ControlChange:    ");
//  Serial.println(  i_virtual  );
  
  if( enviar_mensaje_ON_habilitado[ i_virtual ] == true ){

    controlChange( i_virtual , enviar_mensaje_ON_habilitado[ i_virtual ] * 127 );          // <----------------------------- Mensaje MIDI
    
//    Serial.println("   ON  ");
    estado_botones_led_virtual_verde[ i_virtual ] = true ;
    estado_botones_led_virtual_rojo[ i_virtual ] = true ;
    enviar_mensaje_ON_habilitado[ i_virtual ] = false;
    
  }else  if( enviar_mensaje_ON_habilitado[ i_virtual ] == false ){
    controlChange( i_virtual , enviar_mensaje_ON_habilitado[ i_virtual ] * 127 );           // <----------------------------- Mensaje MIDI
//    Serial.println("   OFF ");
    estado_botones_led_virtual_verde[ i_virtual ] = false ;
    estado_botones_led_virtual_rojo[ i_virtual ] = false ;
    enviar_mensaje_ON_habilitado[ i_virtual ] = true ;
  }

  
}
void f_boton_tipo_MOMENTANEO( int i_virtual ){

//  Serial.print("ProgramChange:    ");
//  Serial.println(  i_virtual  );
  
  programChange( i_virtual );                                                              // <----------------------------- Mensaje MIDI
}
void f_boton_modo_configuracion_INSIDE( int i_virtual ){

//  Serial.print("CONFIG:    ");
//  Serial.print(  i_virtual  );
  
  if( comportamiento_de_switch_virtual_MOMENTANEO[ i_virtual ] == true ){
//    Serial.println("   SWITCH  ");
    comportamiento_de_switch_virtual_MOMENTANEO[ i_virtual ] = false;
  }
  else  if( comportamiento_de_switch_virtual_MOMENTANEO[ i_virtual ] == false ){
//    Serial.println("   MOMENTANEO ");
    comportamiento_de_switch_virtual_MOMENTANEO[ i_virtual ] = true ;
  }  
  
}





void f_led_nav(){
  
  if( modo_configuracion_habilitado_out ){
    f_led_modo_configuracion_OUT();
  }else{
    f_led_modo_configuracion_INSIDE();
  }
  
  f_prender_o_apagar_leds();
  
}
void f_led_nav_codigo_de_color(){
  switch ( escena_en_foco ) {
    case 0:
      estado_led_nav_verde = true ;
      estado_led_nav_rojo = false ;

      break;
    case 1:
      estado_led_nav_verde = false ;
      estado_led_nav_rojo = true ;
      break;
    case 2:
      estado_led_nav_verde = true ;
      estado_led_nav_rojo = true ;
      
      break;
    default:
      
    break;
  }
}
void f_led_nav_apagar(){
  estado_led_nav_verde = false ;
  estado_led_nav_rojo = false ;
}
void f_led_modo_configuracion_OUT(){
  f_led_nav_codigo_de_color();
}
void f_led_modo_configuracion_INSIDE(){

  contador_led_nav_parpadear++ ;
  if ( contador_led_nav_parpadear % intervalo_de_parpadeo == 0 ){
    estado_led_nav_parpadear = !estado_led_nav_parpadear_last ;
    contador_led_nav_parpadear = 0;
  }
  
  if( estado_led_nav_parpadear ){
    f_led_nav_codigo_de_color();
  }else{
    f_led_nav_apagar();
  }
  estado_led_nav_parpadear_last = estado_led_nav_parpadear ;
  
}
void f_prender_o_apagar_leds(){
  digitalWrite( pin_led_nav_verde , estado_led_nav_verde );
  digitalWrite( pin_led_nav_rojo , estado_led_nav_rojo );
}





void f_led_switch_verde(  ){
  if( escena_en_foco == 0 ){
    for( int i = 0 ; i < cant_botones_de_switch ; i++ ){
      if(comportamiento_de_switch_virtual_MOMENTANEO[ i ]){
        digitalWrite( pines_de_led_switch_verde[ i ] , estado_botones_switch[ i ] );
      }else{
        digitalWrite( pines_de_led_switch_verde[ i ] , estado_botones_led_virtual_verde[ i ] );
      }
    }
  }else if ( escena_en_foco == 1 ){
    for( int i = 0 ; i < cant_botones_de_switch ; i++ ){
      digitalWrite( pines_de_led_switch_verde[ i ] , LOW );
    }
  }else if ( escena_en_foco == 2 ){
    for( int i = 0 ; i < cant_botones_de_switch ; i++ ){
      if(comportamiento_de_switch_virtual_MOMENTANEO[ i + cant_botones_de_switch * 2 ]){
        digitalWrite( pines_de_led_switch_verde[ i ] , estado_botones_switch[ i ] );
      }else{
        digitalWrite( pines_de_led_switch_verde[ i ] , estado_botones_led_virtual_verde[ i + cant_botones_de_switch * 2 ]  );
      }
    }
  }
}
void f_led_switch_verde_modo_configuracion(){
  if( escena_en_foco == 0 ){
    for( int i = 0 ; i < cant_botones_de_switch ; i++ ){
      if( comportamiento_de_switch_virtual_MOMENTANEO[ i ] ){       // Parpadear
        if( estado_led_nav_parpadear ){
          digitalWrite( pines_de_led_switch_verde[ i ] , HIGH );
        }else{
          digitalWrite( pines_de_led_switch_verde[ i ] , LOW );
        }
      }else{                                                        //dejar prendido
        digitalWrite( pines_de_led_switch_verde[ i ] , HIGH );
      }
    }
  }else if ( escena_en_foco == 1 ){
    for( int i = 0 ; i < cant_botones_de_switch ; i++ ){
      digitalWrite( pines_de_led_switch_verde[ i ] , LOW );
    }
  }else if ( escena_en_foco == 2 ){
    for( int i = 0 ; i < cant_botones_de_switch ; i++ ){
      if(comportamiento_de_switch_virtual_MOMENTANEO[ i + cant_botones_de_switch * 2 ]){
        if( estado_led_nav_parpadear ){
          digitalWrite( pines_de_led_switch_verde[ i ] , HIGH );
        }else{
          digitalWrite( pines_de_led_switch_verde[ i ] , LOW );
        }
      }else{
        digitalWrite( pines_de_led_switch_verde[ i ] , HIGH  );
      }
    }
  }  
}
void f_led_switch_rojo(  ){
  if( escena_en_foco == 0 ){
    for( int i = 0 ; i < cant_botones_de_switch ; i++ ){
      digitalWrite( pines_de_led_switch_rojo[ i ] , LOW );
    }
  }else if ( escena_en_foco == 1 ){
    for( int i = 0 ; i < cant_botones_de_switch ; i++ ){
      
      if(comportamiento_de_switch_virtual_MOMENTANEO[ i + cant_botones_de_switch ]){
        digitalWrite( pines_de_led_switch_rojo[ i ] , estado_botones_switch[ i ] );
      }else{
        digitalWrite( pines_de_led_switch_rojo[ i ] , estado_botones_led_virtual_rojo[ i + cant_botones_de_switch ] );
      }
    }
  }else if ( escena_en_foco == 2 ){
    for( int i = 0 ; i < cant_botones_de_switch ; i++ ){
      if(comportamiento_de_switch_virtual_MOMENTANEO[ i + cant_botones_de_switch * 2 ]){
        digitalWrite( pines_de_led_switch_rojo[ i ] , estado_botones_switch[ i ] );
      }else{
        digitalWrite( pines_de_led_switch_rojo[ i ] , estado_botones_led_virtual_rojo[ i + cant_botones_de_switch * 2 ]  );
      }
    }
  }

  
}
void f_led_switch_rojo_modo_configuracion(){
  if( escena_en_foco == 0 ){
    for( int i = 0 ; i < cant_botones_de_switch ; i++ ){
      digitalWrite( pines_de_led_switch_rojo[ i ] , LOW );
    }
  }else if ( escena_en_foco == 1 ){
    for( int i = 0 ; i < cant_botones_de_switch ; i++ ){
      if( comportamiento_de_switch_virtual_MOMENTANEO[ i + cant_botones_de_switch ] ){       // Parpadear
        if( estado_led_nav_parpadear ){
          digitalWrite( pines_de_led_switch_rojo[ i ] , HIGH );
        }else{
          digitalWrite( pines_de_led_switch_rojo[ i ] , LOW );
        }
      }else{                                                        //dejar prendido
        digitalWrite( pines_de_led_switch_rojo[ i ] , HIGH );
      }
    }
  }else if ( escena_en_foco == 2 ){
    for( int i = 0 ; i < cant_botones_de_switch ; i++ ){
      if(comportamiento_de_switch_virtual_MOMENTANEO[ i + cant_botones_de_switch * 2 ]){
        if( estado_led_nav_parpadear ){
          digitalWrite( pines_de_led_switch_rojo[ i ] , HIGH );
        }else{
          digitalWrite( pines_de_led_switch_rojo[ i ] , LOW );
        }
      }else{
        digitalWrite( pines_de_led_switch_rojo[ i ] , HIGH  );
      }
    }
  }  
}




void f_pedal_de_exp(){
  estado_boton_de_corte_de_pedal_de_exp = digitalRead( pin_boton_de_corte_de_pedal_de_exp );
  
  if( estado_boton_de_corte_de_pedal_de_exp == LOW ){       /*--- TIENE QUE IR    --->LOW<---   */
    lectura_pedal_de_exp = analogRead( pin_pedal_de_exp );
    lectura_pedal_de_exp = lectura_pedal_de_exp / 8;
       
    if( lectura_pedal_de_exp !=  lectura_pedal_de_exp_last ){
   //    Serial.print( "DIFIERE -------------- " );
   //    Serial.println( lectura_pedal_de_exp );
       //····Aca mandar MIDI ····//
       controlChange( numero_de_mensaje_midi_del_pedal , lectura_pedal_de_exp );   
       
       lectura_pedal_de_exp_last = lectura_pedal_de_exp ; 
    }
  }

}




void f_SAVE() {

  for ( int i = 0; i < cant_botones_de_switch * cant_escenas ; i++ ) {
    
    EEPROM.update( i , comportamiento_de_switch_virtual_MOMENTANEO[ i ] );
    
  }

  EEPROM.update( direccion_del_condicional_para_carga_por_primera_y_unica_vez, 99 );

  delay( 100 ); /* Esto se puede sacar. */


}



void noteOn( int num_de_nota , int velocity ) {
  Serial.write( 0x90 );        // NoteOn = 0x90 .
  Serial.write( num_de_nota );
  Serial.write( velocity );
}
void controlChange( int num_de_CC , int valor_de_CC ) {
  Serial.write( 0xb0 );        //  ControlChange = 0xb0 .
  Serial.write( num_de_CC );
  Serial.write( valor_de_CC );
}
void programChange( int num_de_PG ) {
  Serial.write( 0xc0 );        //  ProgramChange = 0xc0 .
  Serial.write( num_de_PG );
}




