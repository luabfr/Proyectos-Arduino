
 
 #  Author
 
       - Codigo : por Luciano Fritschy
       - Web : www.tacuaramultimedia.com.ar
       - Facebook : https://www.facebook.com/chamaco.fritschy

# Comentarios
      - La libreria 'MIDI.h', funciona con errores cuando se utiliza la salida por DIN5.  Por eso la saqué. Ahora los mensajes se envian utlizando funciones propias. La unica cuestión, es que no estoy definiendo Canal MIDI. No se bien si se envian por OMNI, o qué. (El uso de los pedales SIN la LIBRERIA, ya está probado y todo anda perfecto.)
 
  
 
 #  INFO: 
         - Este Sketch está hecho para un Controlador MIDI tipo Footswich/Pedal, para un ARDUINO UNO. (Para utilizarlo con MEGA quizas haya que modificar algo).
         - Posee 3 Escenas navegables utilizando 2 de los fooswitch.
         - Cada escena es indicada con un determinado color ( escena 1 = verde , escena 2 = rojo , escena 3 = naranja ).
         - Posee 4 switch que pueden funcionar como momentaneos y enviar mensajes de tipo ProgramChange; o como switch y en enviar mensajes de tipo Control Change ON/OFF (0/127) 
         - Para asiganar el comportamiento de cada footswitch, se debe ingresar al 'Modo Setup' (o Modo Configuracion).
         - Para ingresar al 'Modo Setup' se deben apretar al mismo tiempo los 2 botones de cambio de escena.
           - Una vez dentro, el led que indica la escena comenzara a PARPADEAR (a un ritmo lento). 
           - Entonces, para cambiar el comportamiento de cada footswitch (MOMENTANEO/SWITCH), solo hace falta apretar el footswitch desdeado.
             - Si el comportamiento es Momentaneo (Program Change), el led del footswitch PARPADEARÁ.
             - Si el comportamiento es Switch (Control Change), el led del footswitch SE MANTENDRÁ ENCENDIDO.
         
         - Los footswitch de cada escena, envian mensajes ProgramChange/ControlChange distintos en independientes de las demas escenas.       
         - En escena verde los footswitch envian mensajes ProgramChange o ControlChange: 0 , 1 , 2 , 3
         - En escena roja los footswitch envian mensajes ProgramChange o ControlChange: 4 , 5 , 6 , 7
         - En escena naranja los footswitch envian mensajes ProgramChange o ControlChange: 8 , 9 , 10 , 11
         
 
 #  INFO TECNICA: 
         - El Sketch está hecho para funcionar con:
           - 6 Pulsadores.
           - 5 Leds Bicolor (led RG), de 3 patas (con negativo al centro).
           - Un pedal de expresión conectado mediante un Jack Stereo Hembra con corte ( TRS con corte ).
             - El 'Corte' del pedal, funciona exactamente igual que un boton/pulsador.
           ----> Si no se utiliza este conector, es importante que se bloquee o  'comente' (//) la llamada la funcion encargada de leer el pedal (f_boton_de_corte_de_pedal_de_exp)
           ----> De otra manera, la funcion leerá valores erroneos y enviara Mensajes MIDI no deseados.
 
 
 #  CONEXIONES:        
           - Pulsadores: Se debe utilizar una resistencia de 1K en cada pulsador (ver en pagina de arduino).
           - Led bi color: · se debe conectar una resistencia de 220ohm a las patas VERDES. 
                           · se debe conectar una resistencia de 470ohm a las patas ROJAS.
                        ---> ESTO sirve para que el color NARANJA se mezcle bien, nada mas.
        
  
 
 #  REFER:
         - Navegacion de Escenas: pines 2 y 3 . 
         - Pulsadores Switch: pines 4, 5 , 6 y 7 .
         - Leds de Navegacion: Pines: 8 , 9 .
         - Pines Led Switch Verdes: 10 ,  11 , 12 , 13 . 
         - Pines Led Switch Rojos: 14 ,  15 , 16 , 17 .
         - Pin de Pedal de expresion: A5 .
         - Pin de Corte de Pedal de expresion: A4 .
         
  
 
 
 #  Estado:
         - Lectura de 2 botones para Seleccion de Escena.
         - Presionando ambos botones de navegacion al mismo tiempo: se ingresa al Modo Configuracion ( esto sirve para configurar el comportamietno de cada SWITCH )
         - La estrutura de lectura de los botones ya discrimina entre: Modo_Config > Comportamiento_Switch
         - Switch independientes en cada Escena: OK.
         - Leds independientes en cada Escena: OK.
         - Comportamiento tipo SWITCH: OK.
         - Comportamiento tipo MOMENTANEO: OK.
         - Configuracion de Modos de cada boton: OK.
         - Comportamientos de Leds en Modo Config: OK.
         - Comportamientos de Leds en Modo Ejecucion: OK.
         - Envio de Mensajes MIDI: OK.
         - Pedal de expresion con corte: OK.
         - comportamiento asignado a cada SWITCH se guarda en la EEPROM: OK .
  
 
 
 #  Se puede Mejorar:  
         - El funcionamiento de los LEDs no es 'dinamico', si agrego mas cantidad_de_escenas, hay que editar manualmente varias funciones
  
  
 
 
 #  In This:
         - Corrigiendo el Pedal de Expresión. 
         
         
         
 
