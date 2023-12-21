/*

    IN THIS :


        - Comportamientos de LEDs segun el estado del juego:
                                                          Etapa 1 - Nadie está jugando   ( indicacion hacia el ALFA)
                                                          Etapa 2 - Alguien esta seleccionando cantidad de jugadores
                                                          Etapa 3 - Juego ACTIVO   (cantidad de rondas depende de la cantidad de jugadores seleccionados)
                                                                          · Ronda 1
                                                                          · Ronda 2
                                                                          · Ronda 3
                                                                          · Ronda X
                                                          Etapa 4 - Fin del Juego = Festejo de luces

    *******       ALFA = Pulsador / Plataforma / Led que solo sirve para seleccionar cantidad de jugadores        *******




      ANOTACIONES - A resolver :

                                - en '2' = Cuando el usuario ya decidio que cantidades de jugadores quiere:   ¿COMO LE DA EL 'OK'?   .








    *******       LEDS
                        W  =  0
                        G  =  1
                        R  =  2
                        B  =  3





*/

//__ BOTON__
int pin_del_boton = 12 ;

int contador_del_boton = 0;
int estado_boton = 0;
int last_estado_boton = 0;

//__LEDS___
int leds_0_Rxx = 10;
int leds_0_xGx = 30;
int leds_0_xxB = 40;

int leds_1_Rxx = 2;
int leds_1_xGx = 22;
int leds_1_xxB = 32;

int leds_2_Rxx = 3;
int leds_2_xGx = 23;
int leds_2_xxB = 33;

int leds_3_Rxx = 4;
int leds_3_xGx = 24;
int leds_3_xxB = 34;

int leds_4_Rxx = 5;
int leds_4_xGx = 25;
int leds_4_xxB = 35;

int leds_5_Rxx = 6;
int leds_5_xGx = 26;
int leds_5_xxB = 36;


//__ MISC __

int contador_de_tiempo = 0 ;
unsigned long tiempo = 0;
unsigned long last_tiempo = 0;


boolean visor_ON = true ;
boolean visor_OFF = false ;


int  Etapa_Numero = 1 ;

//    Etapa 2   ··  Interrupt
int tiempo_de_espera_para_el_START = 1000;

int contador_etapa_de_seleccion = 0 ;
int contador_para_darle_el_OK_a_la_cantidad_de_jugadores = 0;
int cantidad_de_jugadores = 0 ;


//    Etapa 3     ··    LARGADA
boolean estado_1_leds = false ;
int cantidad_de_vueltas = 0 ;
int random_led = 0;
boolean salir_de_Etapa_3 = false;


int modulo_500 = 500;

//    Etapa 4     ··    Girando
int contador_de_vueltas = 0;
int tiempo_de_ronda = 2000;
boolean salir_de_Etapa_4 = false;
boolean bool_cant_jugadores_5 = false;
boolean bool_cant_jugadores_4 = false;
boolean bool_cant_jugadores_3 = false;
boolean bool_cant_jugadores_2 = false;
boolean bool_cant_jugadores_1 = false;


// Etapa 5
int numero_de_festejo = 1 ;
int contador_para_reinicio = 0;

int num_random = 0;
/*
    Etapa 1  =  STAND·BY   ···    Nadie esta jugando
    Etapa 2  =  IRRUPCION  ···    Interaccion, eleccion cantidad de jugadores
    Etapa 3  =  LARGADA    ···    Pitidos + Juegos de Luces
    Etapa X  =  IN·GAME    ···    Juego on-the-run
    Etapa X2 =  Loop

    Etapa 5  =  FESTEJO    ···    Alguien ganó, las luces apuntan al SPOT ganador

*/
/*·····························································································································*/
/*·····························································································································*/


void setup() {

  pinMode( leds_1_Rxx , OUTPUT );
  pinMode( leds_2_Rxx , OUTPUT );
  pinMode( leds_3_Rxx , OUTPUT );
  pinMode( leds_4_Rxx , OUTPUT );
  pinMode( leds_5_Rxx , OUTPUT );

  pinMode( leds_1_xGx , OUTPUT );
  pinMode( leds_2_xGx , OUTPUT );
  pinMode( leds_3_xGx , OUTPUT );
  pinMode( leds_4_xGx , OUTPUT );
  pinMode( leds_5_xGx , OUTPUT );

  pinMode( leds_1_xxB , OUTPUT );
  pinMode( leds_2_xxB , OUTPUT );
  pinMode( leds_3_xxB , OUTPUT );
  pinMode( leds_4_xxB , OUTPUT );
  pinMode( leds_5_xxB , OUTPUT );

  

  pinMode( leds_0_Rxx , OUTPUT );
  pinMode( leds_0_xGx , OUTPUT );
  pinMode( leds_0_xxB , OUTPUT );
  
  pinMode( pin_del_boton, INPUT );

  Serial.begin( 115200 );

}
/*·····························································································································*/
/*·····························································································································*/


void loop() {
  num_random = random( 1 , 4 );   // Esto termina por decidir cuales siguen y cuales pierden

  
  lectura_Boton();
  
  if ( Etapa_Numero == 1 ) Etapa_1() ;      //      STAND·BY
  if ( Etapa_Numero == 2 ) Etapa_2() ;      //      Interrupcion
  if ( Etapa_Numero == 3 ) Etapa_3( cantidad_de_jugadores ) ;      //      Start   -   Pre-juego
  if ( Etapa_Numero == 4 ) Etapa_4( cantidad_de_jugadores ) ;
  if ( Etapa_Numero == 5 ) Etapa_5() ;

  //  Serial.print("   NUMERO   DE   ETAPA  -  " );
  //  Serial.println( Etapa_Numero );
  //Serial.print("   JUGADORES   -  " );
  //Serial.println( cantidad_de_jugadores );
  Serial.print("   cont de vueltas  -  " );
//  Serial.println( contador_de_vueltas );

//  Serial.print("   Random   -  " );
//  Serial.println( num_random );


  if ( visor_ON ) {

   // Serial.print( contador_de_tiempo );         //   Si comento esta linea no me funciona bien el programa (?) No se por que-
    if (contador_de_tiempo >= 500) {
      contador_de_tiempo = 0;
    }
    contador_de_tiempo++;
  }
}
/*·····························································································································*/
/*·····························································································································*/




void lectura_Boton() {
  estado_boton = digitalRead( pin_del_boton );

  if ( estado_boton != last_estado_boton ) {
    if ( estado_boton == HIGH ) {
      contador_del_boton++ ;
      Serial.println( "on" );
      Serial.println( contador_del_boton );
      digitalWrite( 10, HIGH );

      contador_para_darle_el_OK_a_la_cantidad_de_jugadores = 0 ;       //      Se reinicia el TIEMPO para el START

    } else {
      Serial.println( "off" );
      digitalWrite( 10, LOW );
      Serial.println( contador_del_boton );
    }
  } else {
    if ( contador_del_boton != 2 && contador_del_boton != 0 && contador_del_boton != 1 ) {
      contador_para_darle_el_OK_a_la_cantidad_de_jugadores++ ;
    }
  }


  if ( contador_del_boton != 2 && contador_del_boton != 0 && contador_del_boton != 1 ) {
    if ( contador_para_darle_el_OK_a_la_cantidad_de_jugadores >= tiempo_de_espera_para_el_START ) {     // el TIEMPO es igual a 1000ms, mas o menos.

      Etapa_Numero = 3;       ///         SOLO PARA PROBAR    <------- Aca tengo que pasar a la Etapa 3

    }

    contador_para_darle_el_OK_a_la_cantidad_de_jugadores == 0;

  }





  if ( contador_del_boton == 1 ) {
    Etapa_Numero = 2 ;
  }


  //  Cantidad de jugadores
  if ( contador_del_boton >= 7 ) {
    apagar_leds();
    contador_del_boton = 2;
  }
  contador_etapa_de_seleccion = contador_del_boton ;


  last_estado_boton = estado_boton;
}
/*·····························································································································*/



// INVITACION AL JUEGO    -   Direcciona la vista al PULSADOR
void Etapa_1() {

  ///////// desde el LED·3 tienen que encenderse los otros LEDs con direccion hacia el LED·Alfa


  ////// STEP 1------------------------------------------------
  //     LED3 Pivot
  if ( contador_de_tiempo > 0 && contador_de_tiempo < 120 ) {
    digitalWrite( 4 , HIGH);
  } else {
    digitalWrite( 4 , LOW);
  }
  ////// STEP 2------------------------------------------------
  //     Led 2
  if ( contador_de_tiempo > 40 && contador_de_tiempo < 160 ) {
    digitalWrite( 3 , HIGH);
  } else {
    digitalWrite( 3 , LOW);
  }
  //     Led 4
  if ( contador_de_tiempo > 40 && contador_de_tiempo < 160 ) {
    digitalWrite( 5 , HIGH);
  } else {
    digitalWrite( 5 , LOW);
  }
  ////// STEP 3------------------------------------------------
  //     Led 1
  if ( contador_de_tiempo > 80 && contador_de_tiempo < 200 ) {
    digitalWrite( 2 , HIGH);
  } else {
    digitalWrite( 2 , LOW);
  }
  //     Led 5
  if ( contador_de_tiempo > 80 && contador_de_tiempo < 200 ) {
    digitalWrite( 6 , HIGH);
  } else {
    digitalWrite( 6 , LOW);
  }
  ////// STEP 4------------------------------------------------
  //     LED Alfa
  if ( contador_de_tiempo > 140 && contador_de_tiempo < 500 ) {
    digitalWrite( 10 , HIGH);
  } else {
    digitalWrite( 10 , LOW);
  }

}
/*·····························································································································*/

void Etapa_2() {

  if ( contador_etapa_de_seleccion == 0 || contador_del_boton == 1 || contador_del_boton == 7 ) {
    apagar_leds();
  }

  //  Cantidad  de  Jugadores
  if ( contador_del_boton == 2 ) {
    led_spot_1( HIGH );
    cantidad_de_jugadores = 1 ;
  }

  if ( contador_del_boton == 3 ) {
    led_spot_2( HIGH );
    cantidad_de_jugadores = 2 ;
  }

  if ( contador_del_boton == 4 ) {
    led_spot_3( HIGH );
    cantidad_de_jugadores = 3 ;
  }
  if ( contador_del_boton == 5 ) {
    led_spot_4( HIGH );
    cantidad_de_jugadores = 4 ;
  }
  if ( contador_del_boton == 6 ) {
    led_spot_5( HIGH );
    cantidad_de_jugadores = 5 ;
  }



}
/*·····························································································································*/

void Etapa_3(int cant_de_jug ) {                              /*          START  -  LARGADA           */
  apagar_leds();


  if ( contador_de_tiempo == modulo_500 ) {
    estado_1_leds = !estado_1_leds ;
    cantidad_de_vueltas += contador_de_tiempo ;

  }

/*
  if (cant_de_jug  == 2 )    config_2_led( estado_1_leds );
  if (cant_de_jug  == 3 )    config_3_led( estado_1_leds );
  if (cant_de_jug  == 4 )    config_4_led( estado_1_leds );
  if (cant_de_jug  == 5 )    config_5_led( estado_1_leds );
*/

  Etapa_Numero = 4;



  ///   largada
  if ( salir_de_Etapa_3 == false ) {
    if ( cantidad_de_vueltas  ==  (  modulo_500  * cant_de_jug ) ) {

      Etapa_Numero = 4;       //      ACA VA A LA ETEPA SIGUIENTE
      salir_de_Etapa_3 = true ;
    }
  }



}
/*·····························································································································*/
void config_2_led( int estado_de_leds ) {

  digitalWrite(2 , estado_de_leds ) ;
  digitalWrite(4 , estado_de_leds ) ;

}
void config_3_led( int estado_de_leds ) {

  digitalWrite(3 , estado_de_leds ) ;
  digitalWrite(4 , estado_de_leds ) ;
  digitalWrite(6 , estado_de_leds ) ;

}
void config_4_led( int estado_de_leds ) {

  digitalWrite(2 , estado_de_leds ) ;
  digitalWrite(4 , estado_de_leds ) ;
  digitalWrite(5 , estado_de_leds ) ;
  digitalWrite(6 , estado_de_leds ) ;

}
void config_5_led( int estado_de_leds ) {

  digitalWrite(2 , estado_de_leds ) ;
  digitalWrite(3 , estado_de_leds ) ;
  digitalWrite(4 , estado_de_leds ) ;
  digitalWrite(5 , estado_de_leds ) ;
  digitalWrite(6 , estado_de_leds ) ;

}









void Etapa_4( int cant_de_jug ) {

  if ( cant_de_jug == 5 )   bool_cant_jugadores_5 = true ;
  if ( cant_de_jug == 4 )   bool_cant_jugadores_4 = true ;
  if ( cant_de_jug == 3 )   bool_cant_jugadores_3 = true ;
  if ( cant_de_jug == 2 )   bool_cant_jugadores_2 = true ;
  if ( cant_de_jug == 1 )   bool_cant_jugadores_1 = true ;



  //·········    5     Players    ·········
  if ( bool_cant_jugadores_5 ) {
    if ( contador_de_tiempo == modulo_500 )       contador_de_vueltas += contador_de_tiempo ;
    leds_en_ronda_5_players();
    if ( contador_de_vueltas == 2500 ) {        //       con esto yo se cuando llega a 2500 = 5 vueltas
      contador_de_vueltas = 0;
      bool_cant_jugadores_5 = false;
      cantidad_de_jugadores = 4 ;
      despachando( cantidad_de_jugadores , num_random );
    }
  }


  //·········    4     Players    ·········
  if ( bool_cant_jugadores_4 ) {
    if ( contador_de_tiempo == modulo_500 )       contador_de_vueltas += contador_de_tiempo ;
    leds_en_ronda_4_players();
    if ( contador_de_vueltas == 2000 ) {        //       con esto yo se cuando llega a 2000 = 4 vueltas
      contador_de_vueltas = 0;
      bool_cant_jugadores_4 = false;
      cantidad_de_jugadores = 3 ;
      despachando( cantidad_de_jugadores , num_random );
    }
  }


  //·········    3     Players    ·········
  if ( bool_cant_jugadores_3 ) {
    if ( contador_de_tiempo == modulo_500 )       contador_de_vueltas += contador_de_tiempo ;
    leds_en_ronda_3_players();
    if ( contador_de_vueltas == 1500 ) {        //       con esto yo se cuando llega a 1500 = 3 vueltas
      contador_de_vueltas = 0;
      bool_cant_jugadores_3 = false;
      cantidad_de_jugadores = 2 ;
      despachando( cantidad_de_jugadores , num_random );
    }
  }


  //·········    2     Players    ·········
  if ( bool_cant_jugadores_2 ) {
    if ( contador_de_tiempo == modulo_500 )       contador_de_vueltas += contador_de_tiempo ;
    leds_en_ronda_2_players();

    if ( contador_de_vueltas == 1000 ) {        //       con esto yo se cuando llega a 1000 = 2 vueltas
      contador_de_vueltas = 0;
      bool_cant_jugadores_2 = false;
      cantidad_de_jugadores = 1 ;
      despachando( cantidad_de_jugadores , num_random );
    }
  }



  //·········    1     Players    ·········
  if ( bool_cant_jugadores_1 ) {
    despachando( cantidad_de_jugadores , num_random );
    bool_cant_jugadores_1 = false;
  }

}


void despachando(int queda_X_spots, int num_random) {
  
  int tiempo_delay = 2500;

  para_un_cacho();
  
  digitalWrite(10, LOW );
  
  if ( num_random == 1 ) {
    switch ( queda_X_spots ) {
      case 4:
        digitalWrite(2, HIGH );
        digitalWrite(3, HIGH );
        digitalWrite(4, LOW );
        digitalWrite(5, HIGH );
        digitalWrite(6, HIGH );
        digitalWrite(10, LOW );
        delay( tiempo_delay );
        para_un_cacho();
        break;
      case 3:
        digitalWrite(2, LOW );
        digitalWrite(3, HIGH );
        digitalWrite(4, HIGH );
        digitalWrite(5, LOW );
        digitalWrite(6, HIGH );
        delay( tiempo_delay );
        para_un_cacho();
        break;
      case 2:
        digitalWrite(2, HIGH );
        digitalWrite(3, LOW );
        digitalWrite(4, LOW );
        digitalWrite(5, LOW );
        digitalWrite(6, HIGH );
        delay( tiempo_delay );
        para_un_cacho();
        break;
      case 1:
        digitalWrite(2, HIGH );
        digitalWrite(3, LOW );
        digitalWrite(4, LOW );
        digitalWrite(5, LOW );
        digitalWrite(6, LOW );
        delay( tiempo_delay );
        para_un_cacho();
        resetear();
        numero_de_festejo = 1;
        Etapa_Numero = 5  ;
        break;

    }
 
  }else  if ( num_random == 2 ) {
    switch ( queda_X_spots ) {
      case 4:
        digitalWrite(2, HIGH );
        digitalWrite(3, HIGH );
        digitalWrite(4, HIGH );
        digitalWrite(5, HIGH );
        digitalWrite(6, LOW );
        delay( tiempo_delay );
        para_un_cacho();
        break;
      case 3:
        digitalWrite(2, HIGH );
        digitalWrite(3, LOW );
        digitalWrite(4, HIGH );
        digitalWrite(5, HIGH );
        digitalWrite(6, LOW );
        delay( tiempo_delay );
        para_un_cacho();
        break;
      case 2:
        digitalWrite(2, LOW );
        digitalWrite(3, LOW );
        digitalWrite(4, HIGH );
        digitalWrite(5, LOW );
        digitalWrite(6, HIGH );
        delay( tiempo_delay );
        para_un_cacho();
        break;
      case 1:
        digitalWrite(2, LOW );
        digitalWrite(3, HIGH );
        digitalWrite(4, LOW );
        digitalWrite(5, LOW );
        digitalWrite(6, LOW );
        delay( tiempo_delay );
        para_un_cacho();
        resetear();
        numero_de_festejo = 2;
        Etapa_Numero = 5  ;
        break;

    }
 
  }else  if ( num_random == 3 ) {
    switch ( queda_X_spots ) {
      case 4:
        digitalWrite(2, LOW );
        digitalWrite(3, HIGH );
        digitalWrite(4, HIGH );
        digitalWrite(5, HIGH );
        digitalWrite(6, HIGH );
        delay( tiempo_delay );
        para_un_cacho();
        break;
      case 3:
        digitalWrite(2, HIGH );
        digitalWrite(3, LOW );
        digitalWrite(4, LOW );
        digitalWrite(5, HIGH );
        digitalWrite(6, HIGH );
        delay( tiempo_delay );
        para_un_cacho();
        break;
      case 2:
        digitalWrite(2, LOW );
        digitalWrite(3, HIGH );
        digitalWrite(4, LOW );
        digitalWrite(5, HIGH );
        digitalWrite(6, LOW );
        delay( tiempo_delay );
        para_un_cacho();
        break;
      case 1:
        digitalWrite(2, LOW );
        digitalWrite(3, LOW );
        digitalWrite(4, HIGH );
        digitalWrite(5, LOW );
        digitalWrite(6, LOW );
        delay( tiempo_delay );
        para_un_cacho();
        resetear();
        numero_de_festejo = 3;
        Etapa_Numero = 5  ;
        break;

    }
 
  }else  if ( num_random == 4 ) {
    switch ( queda_X_spots ) {
      case 4:
        digitalWrite(2, HIGH );
        digitalWrite(3, HIGH );
        digitalWrite(4, HIGH );
        digitalWrite(5, HIGH );
        digitalWrite(6, LOW );
        delay( tiempo_delay );
        para_un_cacho();
        break;
      case 3:
        digitalWrite(2, LOW );
        digitalWrite(3, LOW );
        digitalWrite(4, HIGH );
        digitalWrite(5, HIGH );
        digitalWrite(6, HIGH );
        delay( tiempo_delay );
        para_un_cacho();
        break;
      case 2:
        digitalWrite(2, HIGH );
        digitalWrite(3, HIGH );
        digitalWrite(4, LOW );
        digitalWrite(5, LOW );
        digitalWrite(6, LOW );
        delay( tiempo_delay );
        para_un_cacho();
        break;
      case 1:
        digitalWrite(2, LOW );
        digitalWrite(3, LOW );
        digitalWrite(4, LOW );
        digitalWrite(5, HIGH );
        digitalWrite(6, LOW );
        delay( tiempo_delay );
        para_un_cacho();
        resetear();
        numero_de_festejo = 4;
        Etapa_Numero = 5  ;
        break;

    }
 
  }


}

void para_un_cacho(){
  apagar_leds();
  delay(200);
}

void Etapa_5() {


  switch ( numero_de_festejo ) {
      case 1:
        //STEP 1    ------    Leds Pivot
        if ( contador_de_tiempo > 0 && contador_de_tiempo < 250 ) {
          digitalWrite( 4 , HIGH);  
          digitalWrite( 5 , HIGH);
        }else {
          digitalWrite( 4 , LOW);   //    Led 3
          digitalWrite( 5 , LOW);   //    Led 4
        }
        
        // STEP 2   -------
        //     Led 4
        if ( contador_de_tiempo > 100 && contador_de_tiempo < 350 ) {
          digitalWrite( 6 , HIGH);
          digitalWrite( 3 , HIGH);
        } else {    
          digitalWrite( 6 , LOW);
          digitalWrite( 3 , LOW);
        }
        
        // STEP 3    ------   LED Alfa
        //     
        if ( contador_de_tiempo > 0 && contador_de_tiempo < 550 ) {
          digitalWrite( 2 , HIGH);  } 
        break;

        
        case 2:
        //STEP 1    ------    Pivot
        if ( contador_de_tiempo > 0 && contador_de_tiempo < 250 ) {
          digitalWrite( 5 , HIGH);  
          digitalWrite( 6 , HIGH);
        }else {
          digitalWrite( 5 , LOW); 
          digitalWrite( 6 , LOW); 
        }
        
        // STEP 2   -------
        if ( contador_de_tiempo > 100 && contador_de_tiempo < 350 ) {
          digitalWrite( 2 , HIGH);
          digitalWrite( 4 , HIGH);
        } else {    
          digitalWrite( 2 , LOW);
          digitalWrite( 4 , LOW);
        }
        
        // STEP 3    ------   LED Alfa
        //     
        if ( contador_de_tiempo > 0 && contador_de_tiempo < 550 ) {
          digitalWrite( 3 , HIGH);  } 
        break;


        case 3:
        //STEP 1    ------    Pivot
        if ( contador_de_tiempo > 0 && contador_de_tiempo < 250 ) {
          digitalWrite( 2 , HIGH);  
          digitalWrite( 6 , HIGH);
        }else {
          digitalWrite( 2 , LOW); 
          digitalWrite( 6 , LOW); 
        }
        
        // STEP 2   -------
        if ( contador_de_tiempo > 100 && contador_de_tiempo < 350 ) {
          digitalWrite( 3 , HIGH);
          digitalWrite( 5 , HIGH);
        } else {    
          digitalWrite( 3 , LOW);
          digitalWrite( 5 , LOW);
        }
        
        // STEP 3    ------   LED Alfa
        //     
        if ( contador_de_tiempo > 0 && contador_de_tiempo < 550 ) {
          digitalWrite( 4 , HIGH);  } 
        break;


        case 4:
        //STEP 1    ------    Pivot
        if ( contador_de_tiempo > 0 && contador_de_tiempo < 250 ) {
          digitalWrite( 2 , HIGH);  
          digitalWrite( 3 , HIGH);
        }else {
          digitalWrite( 2 , LOW); 
          digitalWrite( 3 , LOW); 
        }
        
        // STEP 2   -------
        if ( contador_de_tiempo > 100 && contador_de_tiempo < 350 ) {
          digitalWrite( 4 , HIGH);
          digitalWrite( 6 , HIGH);
        } else {    
          digitalWrite( 4 , LOW);
          digitalWrite( 6 , LOW);
        }
        
        // STEP 3    ------   LED Alfa
        //     
        if ( contador_de_tiempo > 0 && contador_de_tiempo < 550 ) {
          digitalWrite( 5 , HIGH);  } 
        break;
  }

  contador_para_reinicio++;
  if(contador_para_reinicio == 1500){
    resetear();
    apagar_leds();
    delay(1500);
    Etapa_Numero = 1; 
  }
}





void leds_en_ronda_2_players () {           //      OK
  // LED 1
  if ( contador_de_tiempo > 0 && contador_de_tiempo < 150 ) {
    digitalWrite( leds_1_Rxx , HIGH );
    digitalWrite( leds_1_xGx , HIGH );
//    digitalWrite( 2 , HIGH);
  } else {
    digitalWrite( leds_1_Rxx , LOW );
    digitalWrite( leds_1_xGx , LOW );
//    digitalWrite( 2 , LOW);
  }

  // LED 2
  if ( contador_de_tiempo > 100 && contador_de_tiempo < 250 ) {
    digitalWrite( leds_2_Rxx , HIGH );
    digitalWrite( leds_2_xGx , HIGH );
//    digitalWrite( 3 , HIGH);
  } else {
    digitalWrite( leds_2_Rxx , LOW );
    digitalWrite( leds_2_xGx , LOW );
 //   digitalWrite( 3 , LOW);
  }

  // LED 3
  if ( contador_de_tiempo > 200 && contador_de_tiempo < 350 ) {
    digitalWrite( leds_3_Rxx , HIGH );
    digitalWrite( leds_3_xGx , HIGH );
 //   digitalWrite( 4 , HIGH);
  } else {
    digitalWrite( leds_3_Rxx , LOW );
    digitalWrite( leds_3_xGx , LOW );
 //   digitalWrite( 4 , LOW);
  }

  // LED 4
  if ( contador_de_tiempo > 300 && contador_de_tiempo < 450 ) {
    digitalWrite( leds_4_Rxx , HIGH );
    digitalWrite( leds_4_xGx , HIGH );
//    digitalWrite( 5 , HIGH);
  } else {
    digitalWrite( leds_4_Rxx , LOW );
    digitalWrite( leds_4_xGx , LOW );
//    digitalWrite( 5 , LOW);
  }

  // LED 5
  if ( contador_de_tiempo > 400 || contador_de_tiempo < 50 ) {
    digitalWrite( leds_5_Rxx , HIGH );
    digitalWrite( leds_5_xGx , HIGH );
//    digitalWrite( 6 , HIGH);
  } else {
    digitalWrite( leds_5_Rxx , LOW );
    digitalWrite( leds_5_xGx , LOW );
 //   digitalWrite( 6 , LOW);
  }
}
void leds_en_ronda_3_players () {           //      OK
  // LED 1
  if ( contador_de_tiempo > 0 && contador_de_tiempo < 250 ) {
    digitalWrite( leds_1_Rxx , HIGH);
    digitalWrite( leds_1_xxB , HIGH);
  } else {
    digitalWrite( leds_1_Rxx , LOW );
    digitalWrite( leds_1_xxB , LOW );
  }

  // LED 2
  if ( contador_de_tiempo > 100 && contador_de_tiempo < 350 ) {
    digitalWrite( leds_2_Rxx , HIGH );
    digitalWrite( leds_2_xxB , HIGH );
  } else {
    digitalWrite( leds_2_Rxx , LOW );
    digitalWrite( leds_2_xxB , LOW );
  }

  // LED 3
  if ( contador_de_tiempo > 200 && contador_de_tiempo < 450 ) {
    digitalWrite( leds_3_Rxx , HIGH );
    digitalWrite( leds_3_xxB , HIGH );
//    digitalWrite( 4 , HIGH);
  } else {
    digitalWrite( leds_3_Rxx , LOW );
    digitalWrite( leds_3_xxB , LOW );
 //   digitalWrite( 4 , LOW);
  }

  // LED 4
  if ( contador_de_tiempo > 300 && contador_de_tiempo < 550 ) {
//    digitalWrite( 5 , HIGH);
    digitalWrite( leds_4_Rxx , HIGH );
    digitalWrite( leds_4_xxB , HIGH );
  } else {
    digitalWrite( leds_4_Rxx , LOW );
    digitalWrite( leds_4_xxB , LOW );
//    digitalWrite( 5 , LOW);
  }

  // LED 5
  if ( contador_de_tiempo > 400 || contador_de_tiempo < 150 ) {
    digitalWrite( leds_5_Rxx , HIGH );
    digitalWrite( leds_5_xxB , HIGH );
//    digitalWrite( 6 , HIGH);
  } else {
    digitalWrite( leds_5_Rxx , LOW );
    digitalWrite( leds_5_xxB , LOW );
 //   digitalWrite( 6 , LOW);
  }
}
void leds_en_ronda_4_players () {           //      OK
  // LED 1
  if ( contador_de_tiempo > 0 && contador_de_tiempo < 320 ) {
    digitalWrite( leds_1_xGx , HIGH );
//    digitalWrite( 2 , HIGH);
  } else {
    digitalWrite( leds_1_xGx , LOW );
 //   digitalWrite( 2 , LOW);
  }

  // LED 2
  if ( contador_de_tiempo > 100 && contador_de_tiempo < 440 ) {
 //   digitalWrite( 3 , HIGH);
    digitalWrite( leds_2_xGx , HIGH );
  } else {
    digitalWrite( leds_2_xGx , LOW );
  //  digitalWrite( 3 , LOW);
  }

  // LED 3
  if ( contador_de_tiempo > 200 || contador_de_tiempo < 550 ) {
    digitalWrite( leds_3_xGx , HIGH );
 //   digitalWrite( 4 , HIGH);
  } else {
    digitalWrite( leds_3_xGx , LOW );
  //  digitalWrite( 4 , LOW);
  }

  // LED 4
  if ( contador_de_tiempo > 300 || contador_de_tiempo < 90 ) {
    digitalWrite( leds_4_xGx , HIGH );
 //   digitalWrite( 5 , HIGH);
  } else {
    digitalWrite( leds_4_xGx , LOW );
//    digitalWrite( 5 , LOW);
  }

  // LED 5
  if ( contador_de_tiempo > 400 || contador_de_tiempo < 180 ) {
    digitalWrite( leds_5_xGx , HIGH );
//    digitalWrite( 6 , HIGH);
  } else {
    digitalWrite( leds_5_xGx , LOW );
//    digitalWrite( 6 , LOW);
  }
}
void leds_en_ronda_5_players () {           //      OK
  // LED 1
  if ( contador_de_tiempo > 0 && contador_de_tiempo < 400 ) {
    digitalWrite( leds_1_xxB , HIGH );
//    digitalWrite( 2 , HIGH);
  } else {
    digitalWrite( leds_1_xxB , LOW );
//    digitalWrite( 2 , LOW);
  }

  // LED 2
  if ( contador_de_tiempo > 50 && contador_de_tiempo < 500 ) {
    digitalWrite( leds_2_xxB , HIGH );
 //   digitalWrite( 3 , HIGH);
  } else {
    digitalWrite( leds_2_xxB , LOW );
 //   digitalWrite( 3 , LOW);
  }

  // LED 3
  if ( contador_de_tiempo > 150 || contador_de_tiempo < 50 ) {
//    digitalWrite( 4 , HIGH);
    digitalWrite( leds_3_xxB , HIGH );
  } else {
    digitalWrite( leds_3_xxB , LOW );
//    digitalWrite( 4 , LOW);
  }

  // LED 4
  if ( contador_de_tiempo > 250 || contador_de_tiempo < 150 ) {
    digitalWrite( leds_4_xxB , HIGH );
  //  digitalWrite( 5 , HIGH);
  } else {
    digitalWrite( leds_4_xxB , LOW );
 //   digitalWrite( 5 , LOW);
  }

  // LED 5
  if ( contador_de_tiempo > 400 || contador_de_tiempo < 250 ) {
    digitalWrite( leds_5_xxB , HIGH );
 //   digitalWrite( 6 , HIGH);
  } else {
    digitalWrite( leds_5_xxB , LOW );
 //   digitalWrite( 6 , LOW);
  }
}






/*·····························································································································*/
void apagar_leds() {

  digitalWrite( leds_1_Rxx , LOW );
  digitalWrite( leds_2_Rxx , LOW );
  digitalWrite( leds_3_Rxx , LOW );
  digitalWrite( leds_4_Rxx , LOW );
  digitalWrite( leds_5_Rxx , LOW );
  digitalWrite( leds_0_Rxx , LOW );

  digitalWrite( leds_1_xGx , LOW );
  digitalWrite( leds_2_xGx , LOW );
  digitalWrite( leds_3_xGx , LOW );
  digitalWrite( leds_4_xGx , LOW );
  digitalWrite( leds_5_xGx , LOW );
  digitalWrite( leds_0_xGx , LOW );

  digitalWrite( leds_1_xxB , LOW );
  digitalWrite( leds_2_xxB , LOW );
  digitalWrite( leds_3_xxB , LOW );
  digitalWrite( leds_4_xxB , LOW );
  digitalWrite( leds_5_xxB , LOW );
  digitalWrite( leds_0_xxB , LOW );

}
void prender_leds() {

  digitalWrite( leds_1_Rxx , HIGH );
  digitalWrite( leds_2_Rxx , HIGH );
  digitalWrite( leds_3_Rxx , HIGH );
  digitalWrite( leds_4_Rxx , HIGH );
  digitalWrite( leds_5_Rxx , HIGH );
  digitalWrite( leds_0_Rxx , HIGH );

  digitalWrite( leds_1_xGx , LOW );
  digitalWrite( leds_2_xGx , LOW );
  digitalWrite( leds_3_xGx , LOW );
  digitalWrite( leds_4_xGx , LOW );
  digitalWrite( leds_5_xGx , LOW );
  digitalWrite( leds_0_xGx , LOW );

  digitalWrite( leds_1_xxB , HIGH );
  digitalWrite( leds_2_xxB , HIGH );
  digitalWrite( leds_3_xxB , HIGH );
  digitalWrite( leds_4_xxB , HIGH );
  digitalWrite( leds_5_xxB , HIGH );
  digitalWrite( leds_0_xxB , HIGH );


}
/*·····························································································································*/
void led_spot_1( int estado_on_o_off ) {

  digitalWrite( leds_1_Rxx , estado_on_o_off );

}
void led_spot_2( int estado_on_o_off ) {

  digitalWrite( leds_2_Rxx , estado_on_o_off );
  digitalWrite( leds_2_xGx , estado_on_o_off );

}
void led_spot_3( int estado_on_o_off ) {
  
  digitalWrite( leds_3_Rxx , estado_on_o_off );
  digitalWrite( leds_3_xxB , estado_on_o_off );

}
void led_spot_4( int estado_on_o_off ) {

  digitalWrite( leds_4_xGx , estado_on_o_off );

}
void led_spot_5( int estado_on_o_off ) {

  digitalWrite( leds_5_xxB , estado_on_o_off );

}

void resetear(){
   pin_del_boton = 12 ;

 contador_del_boton = 0;
 estado_boton = 0;
 last_estado_boton = 0;

//__ MISC __

 contador_de_tiempo = 0 ;
  tiempo = 0;
  last_tiempo = 0;


 visor_ON = true ;
 visor_OFF = false ;


  Etapa_Numero = 1 ;

//    Etapa 2   ··  Interrupt
 tiempo_de_espera_para_el_START = 1000;

 contador_etapa_de_seleccion = 0 ;
 contador_para_darle_el_OK_a_la_cantidad_de_jugadores = 0;
 cantidad_de_jugadores = 0 ;


//    Etapa 3     ··    LARGADA
 estado_1_leds = false ;
 cantidad_de_vueltas = 0 ;
 random_led = 0;
 salir_de_Etapa_3 = false;


 modulo_500 = 500;

//    Etapa 4     ··    Girando
 contador_de_vueltas = 0;
 tiempo_de_ronda = 2000;
 salir_de_Etapa_4 = false;
 bool_cant_jugadores_5 = false;
 bool_cant_jugadores_4 = false;
 bool_cant_jugadores_3 = false;
 bool_cant_jugadores_2 = false;
 bool_cant_jugadores_1 = false;


// Etapa 5
 numero_de_festejo = 1 ;
 contador_para_reinicio = 0;

 num_random = 0;
}



