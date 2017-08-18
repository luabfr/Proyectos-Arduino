/* ----------------------------------------- Sketch por: Luciano Fritschy ----------------------------------- --------------------------------- 
  - componentes:
  - potenciómetros: 16, cada uno un tiempo
  - leds: 16 (uno para cada potenciómetro) + 3 (uno para cada botón de grabar)
  - switch on/off
  - pulsadores = 6: play, vol +, vol -, grabar 1, grabar 2, grabar 3.

 *      --> Este programa está hecho para reproducir 4 sonidos distintos (agudo, medio, grave, silencio) 
 *          seleccionable de entre 4 bancos de sonido distintos ( banco 0, 1 , 2 , 3 )
 *      --> Para que el programa funcione correctamente, los archivos de sonido 
 *          en la memoria SD  deben estar en formato MP3 y tener los siguientes nombre:
 *                                                                    - 'tono3.mp3' , 'agudo1.mp3' , 'agudo2.mp3' , 'agudo3.mp3' .
 *                                                                    - 'tono2.mp3' , 'medio1.mp3' , 'medio2.mp3' , 'medio3.mp3' .
 *                                                                    - 'tono1.mp3' , 'grave1.mp3' , 'grave2.mp3' , 'grave3.mp3' .
 * ------------------------------------------------------------------------- --------------------------------- --------------------------------- 
 * 
 * 
 *  Refer OK:
 *          - Lectura de 8 leds de Tiempo     - Pines: D22 en adelante.
 *          - Lectura de 8 Potes              - Pines: A0 en adelante.
 *          - 4 Leds a modo de 'SONIDOS'      - Pines: D40 en adelante.
 *          - Lectura de 6 Botones            - Pines: D44 en adelante.
 *  
 *  Funciona OK:
 *          - El tiempo. OK.
 *          - LEDs de tiempo. OK.
 *          - Lectura Potes. OK.
 *          - Potes seleccion el sonido de cada tiempo. OK. 
 *          - Función que habilita/deshabilita el envio de mensajes por Serial.
 *          - Boton de PLAY. OK.
 *          - Boton Tempo DOWN. OK.
 *          - Boton Tempo UP. OK.   
 *          - Boton Rec 1. OK.      ---> Volumen ++
 *          - Boton Rec 2. OK.      ---> Volumen --
 *          - Boton Rec 3. OK.      ---> Cambia de Banco de Sonido    
 *          
 *  in THIS:
 *          - Desarrollando las funciones de los botones.
 *                  - Boton Volumen ++ 
 *                  - Boton Volumen --
 *                  - Boton 'Cambiar Banco de sonidos'
 *  
 *  Falta:
 *          - Mejorar funcion: f_leds_en_stadby();  --- Se ejecuta cuando la reproduccion está en 'STOP' ( activar_reproduccion_del_sequencer = false )
 * 
 */

/*------- GLOBALES ------------------------*/
const int cantidad_de_tiempos = 16 ;
const int cantidad_de_potes = 16 ;           /* La cantidad_de_potes es = cantidad_de_tiempos . A esta var solo la puse para tener un poquito mas de control nomás */
const int cantidad_de_sonidos = 4 ;          /* = Agudo, Medio, Grave, Silencio */
const int cantidad_de_botones = 6 ;

const int pin_base_de_leds_de_tiempo = 22 ; /*  Equivale al pin D22 en adelante*/
const int pin_base_leds_de_sonido = 40 ;    /*  Equivale al pin D40 en adelante. Estos leds solo sirven de referencia cuandono se trabaja con el Audio funcionando*/
const int pin_base_potes = 54 ;             /*  Equivale al pin A0  */
const int pin_base_de_botones = 44 ;        /*  Equivale al pin D44 en adelante */

boolean activar_reproduccion_del_sequencer = false ;                 /*  = Boton_Play  */
boolean envio_de_mensajes_por_serial_habilitado = false ;           /*   Esto deshabilita el Monitoreo por Serial. El Monitoreo realentiza la velocidad del programa. */

int intervalo_de_tiempo = 800 ;          /*   1000 = 1seg   */

int intervalo_de_tiempo_MAX = 2500 ;      //-----> Esta variable determina el TEMPO maximo. (en milisegundos)
int intervalo_de_tiempo_MIN = 200 ;       //-----> Esta variable determina el TEMPO minimo. (en milisegundos)


/*-------- LEDS ---------------------------*/
int pines_de_leds[ cantidad_de_tiempos ];
int i_leds ;


/*-------- POTES ---------------------------*/
int pines_de_potes[ cantidad_de_potes ];
int lectura_de_potes[ cantidad_de_potes ];
int i_potes;


/*--------- TIEMPO -------------------------*/
unsigned long tiempo ;
unsigned long tiempo_last = 0 ;

const int mas_o_menos_tiempo = 100 ;
int tiempo_y_sonido[ cantidad_de_tiempos ][ cantidad_de_sonidos ];
int cont_tiempos = 0 ;

/*-------- Seleccion de SONIDO/LED----------*/
int sonido_seleccionado[ cantidad_de_tiempos ] ;  
const int sonido_silencio = 0 ;
const int sonido_agudo = 1 ;
const int sonido_medio = 2 ;
const int sonido_grave = 3 ;


/*-------- Botones ----------*/
byte pin_botones[ cantidad_de_botones ] ;
boolean estado_botones[ cantidad_de_botones ];
boolean estado_botones_last[ cantidad_de_botones ];
byte cont_pulsaciones_de_botones[ cantidad_de_botones ];
boolean botones_cambio_de_estado_habilitado[ cantidad_de_botones ];

const int boton_play = 0 ;
const int boton_volumen_DOWN = 1 ;
const int boton_volumen_UP = 2 ;
const int boton_rec_1 = 3 ;
const int boton_rec_2 = 4 ;
const int boton_rec_3 = 5 ;

unsigned long contador_standby = 0 ;
unsigned long contador_standby_last = 0 ;
int intervalo_contador = 1000 ;

/*------------------------- Shield MP3 : INI -------------------------*/
#include <SPI.h>
#include <Adafruit_VS1053.h>
#include <SD.h>

#define BREAKOUT_RESET  8      // VS1053 reset pin (output)
#define BREAKOUT_CS     6      // VS1053 chip select pin (output)
#define BREAKOUT_DCS    7      // VS1053 Data/command select pin (output)
#define CARDCS 9               // Card chip select pin
#define DREQ 2                 // VS1053 Data request, ideally an Interrupt pin
/* create breakout-example object!  */
Adafruit_VS1053_FilePlayer musicPlayer = Adafruit_VS1053_FilePlayer(BREAKOUT_RESET, BREAKOUT_CS, BREAKOUT_DCS, DREQ, CARDCS);


int volumen = 20 ;    /*  El volumen del SHIELD, mientra mas bajo, mas fuerte suena. El Maximo debe ser 0. Con el Volumen = 100, ya casi no se escucha. */
String track_en_reproduccion ;

int banco_de_sonido = 0 ;

/*------------------------- Shield MP3 : FIN -------------------------*/






void setup() {
  
  Serial.begin( 9600 );
  Serial.println( "--------------- SETUP : INICIO -----------------" );
  
  /* ----------- Pines - Leds de Tiempo ----------------------*/
  for( int i = 0 ; i < cantidad_de_tiempos ; i++ ){
    
    pinMode( pin_base_de_leds_de_tiempo + i , OUTPUT);

    Serial.print("Pines LEDS : ");
    Serial.println( pin_base_de_leds_de_tiempo + i );
    Serial.print("Tiempo : ");
    Serial.println( i );
    
    sonido_seleccionado[ i ] = 0 ;
    Serial.print("sonido_selecionado_por_tiempo : ");
    Serial.println(  sonido_seleccionado[ i ] );
      
    for( int j = 0 ; j < cantidad_de_sonidos ; j++ ){
      tiempo_y_sonido[ i ][ j ] = 0 ;
      Serial.print("Sonido: ");
      Serial.println( tiempo_y_sonido[ i ][ j ] );

      
    }
    Serial.println( "-" );
  }


  /* ----------- Pines - SONDIDOS (Leds) --------------- Inicializo los LEDS de Prueba de SONIDO ---- En el final esto NO va ----*/
  for( int i = 0 ; i < cantidad_de_sonidos ; i++ ){

    pinMode( pin_base_leds_de_sonido + i , OUTPUT);
  
    Serial.print("Pines LEDS de SONIDOS: ");
    Serial.println( pin_base_leds_de_sonido + i );
    
  }
 

  /*  Seteo de Botones  */
  for (byte i = 0 ; i < cantidad_de_botones ; i++ )  {
    
    pin_botones[ i ] = pin_base_de_botones + i;  
    estado_botones[ i ] = 0 ;
    estado_botones_last[ i ] = 0 ;
    cont_pulsaciones_de_botones[ i ] = 0 ;
    botones_cambio_de_estado_habilitado[ i ] = true;
    
    pinMode( pin_botones[ i ] , INPUT) ;
    Serial.print( "Pines Botones: " );
    Serial.println( pin_botones[ i ] );
  }
  
  Serial.println( "---------------- SHIELD : INI -------------------" );

  Serial.println("Adafruit VS1053 Simple Test");
  if (! musicPlayer.begin()) { /* initialise the music player */
     Serial.println(F("Couldn't find VS1053, do you have the right pins defined?"));
     while (1);
  }
  Serial.println(F("VS1053 found"));
   if (!SD.begin(CARDCS)) {
    Serial.println(F("SD failed, or not present"));
    while (1);  /* don't do anything more */
  }
  /* list files */
  printDirectory( SD.open("/"), 0 );
  /* Set volume for left, right channels. lower numbers == louder volume! */
  musicPlayer.setVolume( volumen , volumen );
  /* Timer interrupts are not suggested, better to use DREQ interrupt! */
  /*musicPlayer.useInterrupt(VS1053_FILEPLAYER_TIMER0_INT); // timer int  */
  musicPlayer.useInterrupt( VS1053_FILEPLAYER_PIN_INT );  // DREQ int
  /* Play another file in the background, REQUIRES interrupts!  */
  Serial.println(F( "Playing track 002" ));
  Serial.println(" -------------------- Setup : FIN ---------------------- ");
  
}
/* File listing helper  */
void printDirectory(File dir, int numTabs) {
   while(true) {
     
     File entry =  dir.openNextFile();
     if (! entry) {
       // no more files
       //Serial.println("**nomorefiles**");
       break;
     }
     for (uint8_t i=0; i<numTabs; i++) {
       Serial.print('\t');
     }
     Serial.print(entry.name());
     if (entry.isDirectory()) {
       Serial.println("/");
       printDirectory(entry, numTabs+1);
     } else {
       // files have sizes, directories do not
       Serial.print("\t\t");
       Serial.println(entry.size(), DEC);
     }
     entry.close();
   }
}

void loop() {
  f_lectura_botones();
  f_leer_potes();
  
  if( activar_reproduccion_del_sequencer ){
    f_tiempo();
    f_leds_tiempo();
    
    f_leds_sonido();
  }else{
    f_tiempo();
    f_leds_en_standby();
  }

 // delay( 350 );
}




void f_lectura_botones(){
  for (byte i = 0 ; i < cantidad_de_botones ; i++ ){
    estado_botones[ i ] = digitalRead(  pin_botones[ i ] );  
    
    if ( estado_botones[ i ] != estado_botones_last[ i ]) {
       if ( estado_botones[ i ] == HIGH) {
            cont_pulsaciones_de_botones[ i ]++ ;
       }
       delay( 10 );
    }
    estado_botones_last[ i ] = estado_botones[ i ];
  
    if ( cont_pulsaciones_de_botones[ i ] % 2 == 0 ) {
      if( !botones_cambio_de_estado_habilitado[ i ] ){
        /* Aca va la Accion de tu boton ON */
        f_accion_de_botones( i );
        
        botones_cambio_de_estado_habilitado[ i ] = true;
      } 
    }
    else{
      if( botones_cambio_de_estado_habilitado[ i ] ){
        /* Aca va la Accion de tu boton OFF */
        f_accion_de_botones( i );
        
        botones_cambio_de_estado_habilitado[ i ] = false;
      } 
    }  
  }
}
void f_accion_de_botones( int i ){
  switch( i ){
    
    case boton_play :
      activar_reproduccion_del_sequencer = !activar_reproduccion_del_sequencer ;
      if( activar_reproduccion_del_sequencer == false ){
        musicPlayer.stopPlaying();
        cont_tiempos = 0 ;
      }
      
      f_serial("Play: ");
      f_serial_ln( String( activar_reproduccion_del_sequencer ) );
      break;
      
    case boton_volumen_DOWN:
      
      volumen -= 10;
      if( volumen < 0 ){
        volumen = 0 ;
      }
      musicPlayer.setVolume( volumen , volumen );
      f_serial(".............................................. Boton VOL ++ : ");
      f_serial_ln( String( volumen ) );
      break;
      
    case boton_volumen_UP:
      volumen += 10;
      if( volumen > 160 ){
        volumen = 160 ;
      }
      musicPlayer.setVolume( volumen , volumen );
      f_serial(".............................................. Boton VOL-- : ");
      f_serial_ln( String( volumen ) );
      
      break;
      
    case boton_rec_1:                   /* Provisoriamente funciona como volumen_MAS      */
      intervalo_de_tiempo += mas_o_menos_tiempo ;
      if( intervalo_de_tiempo > 2500 ){
        intervalo_de_tiempo = intervalo_de_tiempo_MAX;
      }
      f_serial(".............................................. Intervalo UP: ");
      f_serial_ln( String( intervalo_de_tiempo ) );
      break; 
       
    case boton_rec_2:                    /* Provisoriamente funciona como volumen_MENOS   */
      intervalo_de_tiempo -= mas_o_menos_tiempo ;
      if( intervalo_de_tiempo < 200 ){
        intervalo_de_tiempo = intervalo_de_tiempo_MIN;
      }
      f_serial(".............................................. Intervalo DO: ");
      f_serial_ln( String( intervalo_de_tiempo ) );

      break; 
      
    case boton_rec_3:            /*  Provisoriamente funciona como cambiar_banco_de_sonidos  */
      banco_de_sonido++;
      if( banco_de_sonido >= 4 ){
        banco_de_sonido = 0 ;
      }

      f_serial( "Banco de Sonido: " );
      f_serial_ln( String(banco_de_sonido) );
      break; 
  }
}


void f_tiempo(){
  tiempo = millis();

  if ( tiempo >= tiempo_last + intervalo_de_tiempo ){
    cont_tiempos ++ ;
    tiempo_last = tiempo ;
  }

  if( cont_tiempos > cantidad_de_tiempos ){
    cont_tiempos = 1 ;
  }
  f_serial(">> cont_tiempos  <<  =  ");
  f_serial( String( cont_tiempos ) );
  
}
void f_leds_tiempo(){

  for( int i = 0 ; i < cantidad_de_tiempos ; i++ ){
    if( i == cont_tiempos-1 ){
      digitalWrite( pin_base_de_leds_de_tiempo + i , HIGH );
    }else{
      digitalWrite( pin_base_de_leds_de_tiempo + i , LOW );
    }
    
  } 

}
void f_leds_en_standby(){
  
  contador_standby = millis();

  if( contador_standby >= contador_standby_last + intervalo_contador ){
    for( int i = 0 ; i < cantidad_de_tiempos ; i++ ){
       digitalWrite( pin_base_de_leds_de_tiempo + i , HIGH );      
    }
    
    contador_standby_last = contador_standby ;
  }else{
    for( int i = 0 ; i < cantidad_de_tiempos ; i++ ){
       digitalWrite( pin_base_de_leds_de_tiempo + i , LOW );      
    }
  }
  
}





void f_leds_sonido(){
  int i_tiempo = cont_tiempos-1;
  //                  1      - sonido[1]
  tiempo_y_sonido[ i_tiempo ][ i_tiempo ] = sonido_seleccionado[ i_tiempo ];

  f_serial("    Tiempo: ");
  f_serial( String( i_tiempo ) );
  f_serial("  -  Sonido: ");
  f_serial( String( tiempo_y_sonido[ i_tiempo ][ i_tiempo ] ) );

  switch ( tiempo_y_sonido[ i_tiempo ][ i_tiempo ] ) {
    case sonido_silencio:
      f_leds_sonido_codigo( sonido_silencio );
      break;
    case sonido_agudo:
      f_leds_sonido_codigo( sonido_agudo );
      break;
    case sonido_medio:
      f_leds_sonido_codigo( sonido_medio );
      break;
    case sonido_grave:
      f_leds_sonido_codigo( sonido_grave );
      break;
  }
 
}
void f_leds_sonido_codigo( int indicador ){       /*    Esta función ejecuta los sonidos  */
  switch ( indicador ) {
    
    case sonido_silencio:
      f_serial_ln(" (SILENCIO) "); //Serial.println(" (SILENCIO) ");
      digitalWrite( 40 , HIGH );
      digitalWrite( 41 , LOW );
      digitalWrite( 42 , LOW );
      digitalWrite( 43 , LOW );
      if( track_en_reproduccion != "silencio"){
        musicPlayer.stopPlaying();
        track_en_reproduccion = "silencio";
      }
      break;
      
    case sonido_agudo:
      f_serial_ln(" (AGUDO) ");
      digitalWrite( 40 , LOW );
      digitalWrite( 41 , HIGH );
      digitalWrite( 42 , LOW );
      digitalWrite( 43 , LOW );
      if( track_en_reproduccion != "agudo"){
        
        musicPlayer.stopPlaying();
        if( banco_de_sonido == 0){
          musicPlayer.startPlayingFile( "tono3.mp3" );
        }else if( banco_de_sonido == 1){
          musicPlayer.startPlayingFile( "agudo1.mp3" );
        }else if( banco_de_sonido == 2){
          musicPlayer.startPlayingFile( "agudo2.mp3" );
        }else if( banco_de_sonido == 3){
          musicPlayer.startPlayingFile( "agudo3.mp3" );
        }
        
        track_en_reproduccion = "agudo";
      }
      break;
      
    case sonido_medio:
      f_serial_ln(" (MEDIO) ");
      digitalWrite( 40 , LOW );
      digitalWrite( 41 , LOW );
      digitalWrite( 42 , HIGH );
      digitalWrite( 43 , LOW );
      if( track_en_reproduccion != "medio"){

        musicPlayer.stopPlaying();
        if( banco_de_sonido == 0){
          musicPlayer.startPlayingFile( "tono2.mp3" );
        }else if( banco_de_sonido == 1){
          musicPlayer.startPlayingFile( "medio1.mp3" );
        }else if( banco_de_sonido == 2){
          musicPlayer.startPlayingFile( "medio2.mp3" );
        }else if( banco_de_sonido == 3){
          musicPlayer.startPlayingFile( "medio3.mp3" );
        }
        track_en_reproduccion = "medio";
      }
      break;
      
    case sonido_grave:
      f_serial_ln(" (GRAVE) ");
      digitalWrite( 40 , LOW );
      digitalWrite( 41 , LOW );
      digitalWrite( 42 , LOW );
      digitalWrite( 43 , HIGH );
      if( track_en_reproduccion != "grave"){
        musicPlayer.stopPlaying();
        if( banco_de_sonido == 0){
          musicPlayer.startPlayingFile( "tono1.mp3" );
        }else if( banco_de_sonido == 1){
          musicPlayer.startPlayingFile( "grave1.mp3" );
        }else if( banco_de_sonido == 2){
          musicPlayer.startPlayingFile( "grave2.mp3" );
        }else if( banco_de_sonido == 3){
          musicPlayer.startPlayingFile( "grave3.mp3" );
        }
        track_en_reproduccion = "grave";
      }
      break;
      
  }
}
int volumen_last;
void f_fadeout(){
  volumen_last = volumen ;
  for( int i = 0 ; i < 20 ; i++ ){
    volumen += 5  ;
    if( volumen > 160 ){
      volumen = 160 ;
    }
    Serial.print("Volumen Fadeout: ");
    Serial.println( volumen );
    musicPlayer.setVolume( volumen , volumen );
  }
}
void f_fadein(){
  for( int i = 0 ; i < 20 ; i++ ){
    volumen -= 5  ;
    if( volumen < volumen_last ){
      volumen = volumen_last ;
    }
    Serial.print("Volumen Fadeout: ");
    Serial.println( volumen );
    musicPlayer.setVolume( volumen , volumen );
  }
}




void f_leer_potes(){
  f_serial_ln( "--------------------------------------------------------------------" );
  for( int i = 0 ; i < cantidad_de_potes ; i++ ){
    i_potes = pin_base_potes + i ;  /*  Esto empieza con la entrada Analógica 0, y va aumentando  */

    lectura_de_potes[ i ] = analogRead( i_potes );
    sonido_seleccionado[ i ] = lectura_de_potes[ i ] / 256 ;    /* El map no me funcionaba del todo bien*/

    f_serial("    Pote de Tiempo: ");
    f_serial( String( i ) );
    f_serial( "   -   Sonido Seleccionado: ");
    f_serial_ln( String( sonido_seleccionado[ i ] ) );
   
  }

}




void f_serial( String mensaje ){
  if( envio_de_mensajes_por_serial_habilitado == true){
    Serial.print( mensaje );
  }
}
void f_serial_ln( String mensaje ){
  if( envio_de_mensajes_por_serial_habilitado == true){
    Serial.println( mensaje );
  }
}




